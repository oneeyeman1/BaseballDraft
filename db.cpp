// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include <string.h>
#include <algorithm>
#include <map>
#include "sqlite3.h"
#include "wx/tipdlg.h"
#include "leaguesettings.h"
#include "db.h"

CDb::CDb(void)
{
	m_connected = false;
}

CDb::~CDb(void)
{
	sqlite3_close( m_handle );
	m_connected = false;
}

void CDb::OpenConnection(const wxString &dbName)
{
	int result = sqlite3_open( dbName, &m_handle );
	if( result == SQLITE_OK )
	{
		if( sqlite3_exec( m_handle, "PRAGMA foreign_keys = ON", NULL, NULL, 0 ) != SQLITE_OK )
			wxMessageBox( "Internal database failure. Can't set database parameter for foreign key" );
		else
			m_connected = true;
	}
}

bool CDb::IsConnected()
{
	return m_connected;
}

void CDb::GetLeagueList(std::vector<std::string> &leagues)
{
	int result;
	wxString query = "SELECT name FROM Leagues";
	if( m_connected )
	{
		if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
		{
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result  == SQLITE_ROW )
					leagues.push_back( std::string( reinterpret_cast<const char *>( sqlite3_column_text( stmt, 0 ) ) ) );
				else if( result == SQLITE_DONE )
					break;
				else
				{
					wxMessageBox( wxString::Format( "Error getting league list: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			sqlite3_finalize( stmt );
		}
		else
			wxMessageBox( wxString::Format( "Could not prepare SELECT query for leagues name ", sqlite3_errmsg( m_handle ) ) );
	}
}

int CDb::DeleteLeague(const wxString &league, bool isUpdate)
{
	int leagueid = 0, result;
	bool good = true;
	wxString query = wxString::Format( "SELECT id FROM Leagues WHERE name = \"%s\";", league );
	if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
	{
		while( true )
		{
			result = sqlite3_step( stmt );
			if( result == SQLITE_ROW )
				leagueid = sqlite3_column_int( stmt, 0 );
			else if( result == SQLITE_DONE )
				break;
			else
			{
				wxMessageBox( wxString::Format( "Error retrieving league id: %s", sqlite3_errmsg( m_handle ) ) );
				good = false;
				break;
			}
		}
		sqlite3_finalize( stmt );
		if( result != SQLITE_DONE )
			good = false;
		else
		{
			std::vector<wxString> deleteTrans;
			deleteTrans.push_back( wxString::Format( "DELETE FROM ScoreHitsForLeague WHERE id = %d;", leagueid ) );
			deleteTrans.push_back( wxString::Format( "DELETE FROM ScorePitchForLeague WHERE id = %d;", leagueid ) );
			deleteTrans.push_back( wxString::Format( "DELETE FROM TeamsForLeague WHERE id = %d;", leagueid ) );
			deleteTrans.push_back( wxString::Format( "DELETE FROM positionsforleague WHERE id = %d;", leagueid ) );
			deleteTrans.push_back( wxString::Format( "DELETE FROM Owners WHERE id = %d;", leagueid ) );
			deleteTrans.push_back( wxString::Format( "DELETE FROM Targetshits WHERE id = %d;", leagueid ) );
			deleteTrans.push_back( wxString::Format( "DELETE FROM TargetsPitches WHERE id = %d;", leagueid ) );
			deleteTrans.push_back( wxString::Format( "DELETE FROM leagueplayershitter WHERE id = %d;", leagueid ) );
			deleteTrans.push_back( wxString::Format( "DELETE FROM leagueplayerspitcher WHERE id = %d;", leagueid ) );
			deleteTrans.push_back( wxString::Format( "DELETE FROM playerpositioninleague WHERE id = %d;", leagueid ) );
			deleteTrans.push_back( wxString::Format( "DELETE FROM playerdraft WHERE id = %d;", leagueid ) );
			deleteTrans.push_back( wxString::Format( "DELETE FROM Leagues WHERE id = %d;", leagueid ) );
			if( leagueid )
			{
				if( isUpdate )
					sqlite3_exec( m_handle, "SAVEPOINT delete_league", 0, 0, 0 );
				else
					sqlite3_exec( m_handle, "BEGIN", 0, 0, 0 );
				for( unsigned int i = 0; i < deleteTrans.size() && good; i++ )
				{
					if( i == deleteTrans.size() - 1 && isUpdate )
						continue;
					if( sqlite3_prepare_v2( m_handle, deleteTrans[i], -1, &stmt, 0 ) == SQLITE_OK )
					{
						result = sqlite3_step( stmt );
						if( result != SQLITE_OK && result != SQLITE_DONE )
							good = false;
						sqlite3_finalize( stmt );
					}
					else
					{
						wxMessageBox( wxString::Format( "Could not delete league \"%s\": %s", league, sqlite3_errmsg( m_handle ) ) );
						sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
						good = false;
					}
				}
				if( good && !isUpdate )
				{
					result = sqlite3_exec( m_handle, "COMMIT", 0, 0, 0 );
					if( result != SQLITE_OK )
					{
						wxMessageBox( wxString::Format( "Couldn't delete the league: %s", sqlite3_errmsg( m_handle ) ) );
						result = sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
						if( result != SQLITE_OK )
						{
							wxMessageBox( "Serious problem with the database. Try restarting the program and try again" );
						}
					}
				}
				else if( !isUpdate )
				{
					result = sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
					if( result != SQLITE_OK )
						wxMessageBox( "Serious problem with the database occur. Try restarting the program and try again." );
				}
			}
		}
	}
	else
	{
		wxMessageBox( wxString::Format( "Could not query leagues for deletion: %s" ), sqlite3_errmsg( m_handle ) );
		good = false;
	}
	return good;
}

bool CDb::CreateLeague(const CLeagueSettings &league, bool isUpdate, const wxString oldLeague, int &leagueId)
{
	wxString insertTrans[6];
	int result;
	char *error = NULL;
	int good = !SQLITE_OK;
	result = sqlite3_exec( m_handle, "BEGIN", 0, 0, 0 );
	if( result != SQLITE_OK )
	{
		wxMessageBox( "Can't create league: %s", sqlite3_errmsg( m_handle ) );
		return false;
	}
	if( !isUpdate )
		insertTrans[0] = wxString::Format( "INSERT INTO Leagues(id, name, drafttype, scoringtype, roundvalues, salary, benchplayers) VALUES( NULL, \"%s\", %d, %d, %d, %d, %d );", league.GetLeagueName(), league.GetDraftType() + 1, league.GetScoringType() + 1, league.GetRoundingtype() + 1, league.GetSalary(), league.IsBenchPlayerIncluded() );
	else
	{
		insertTrans[0] = wxString::Format( "UPDATE Leagues SET name = \"%s\", drafttype = %d, scoringtype = %d, roundvalues = %d, salary = %d, benchplayers = %d WHERE name = \"%s\";", league.GetLeagueName(), league.GetDraftType() + 1, league.GetScoringType() + 1, league.GetRoundingtype() + 1, league.GetSalary(), league.IsBenchPlayerIncluded() ? 1 : 0, oldLeague );
		if( sqlite3_exec( m_handle, insertTrans[0], 0, 0, &error ) != SQLITE_OK )
		{
			wxMessageBox( wxString::Format( "Problem updating league name: %s", error ) );
			sqlite3_free( error );
			sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
			return false;
		}
	}
	if( isUpdate )
	{
		if( !DeleteLeague( league.GetLeagueName(), true ) )
		{
			sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
			return false;
		}
	}
	result = sqlite3_exec( m_handle, insertTrans[0], 0, 0, &error );
	if( result != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Problem creating the league: %s", error ) );
		sqlite3_free( error );
		return false;
	}
	else
	{
		leagueId = GetLeagueId( isUpdate, league.GetLeagueName() );
		if( !leagueId )
			good = false;
		else
		{
			if( InsertUpdateOwners( league.GetOwners(), leagueId ) != SQLITE_OK )
				good = false;
			else
			{
				if( InsertUpdatePositions( league.GetPositions(), leagueId ) != SQLITE_OK )
					good = false;
				else
				{
					if( InsertUpdateHitScoring( league.GetHitScoring(), league.GetPitchScoring(), leagueId ) != SQLITE_OK )
						good = false;
					else
					{
						if( InsertUpdateTeams( league.GetAmericanLeague(), league.GetNationalLeague(), leagueId ) != SQLITE_OK )
							good = false;
						else
						{
							if( InsertUpdatePlayers( league.GetAmericanLeague(), league.GetNationalLeague(), leagueId ) != SQLITE_OK )
								good = false;
							else
							{
								if( InsertUpdatePlayersPosition( leagueId) != SQLITE_OK )
									good = false;
							}
						}
					}
				}
			}
		}
	}
	if( good )
	{
		result = sqlite3_exec( m_handle, "COMMIT", 0, 0, 0 );
		if( result != SQLITE_OK )
		{
			wxMessageBox( wxString::Format( "Problem creating league: %s", sqlite3_errmsg( m_handle ) ) );
			result = sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
			if( result != SQLITE_OK )
			{
				wxMessageBox( "Serious problem with the database occur. Please restart the program and try again" );
				return false;
			}
		}
		return true;
	}
	else
	{
		result = sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
		if( result != SQLITE_OK )
				wxMessageBox( "Serious problem with the database occur. Please restart the program and try again" );
		return false;
	}
}

int CDb::InsertUpdatePlayersPosition(int leagueId)
{
	int res = SQLITE_OK;
	char *error;
	wxString query = wxString::Format( "INSERT INTO playerpositioninleague SELECT DISTINCT playerposition.playerid, playerposition.positionid, %d FROM playerposition, leagueplayershitter WHERE playerposition.playerid = leagueplayershitter.playerid AND leagueplayershitter.id = %d;", leagueId, leagueId );
	res = sqlite3_exec( m_handle, query, 0, 0, &error );
	if( res != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Error inserting positions information for the league: %s", error ) );
		sqlite3_free( error );
	}
	else
	{
		query = wxString::Format( "INSERT INTO playerpositioninleague SELECT DISTINCT playerposition.playerid, playerposition.positionid, %d FROM playerposition, leagueplayerspitcher WHERE playerposition.playerid = leagueplayerspitcher.playerid AND leagueplayerspitcher.id = %d;", leagueId, leagueId );
		res = sqlite3_exec( m_handle, query, 0, 0, &error );
		if( res != SQLITE_OK )
		{
			wxMessageBox( wxString::Format( "Error inserting positions information for the league: %s", error ) );
			sqlite3_free( error );
		}
	}
	return res;
}

int CDb::InsertUpdatePlayers(const std::vector<int> &americans, const std::vector<int> &nationals, int leagueid)
{
	int result = SQLITE_OK; 
	char *error;
	wxString query_hit = wxString::Format( "INSERT INTO leagueplayershitter SELECT %d, players.playerid, players.value, currvalue, scoreid, playerscorehits.value, 0, \"0\" FROM players, playerscorehits WHERE players.ishitter=\"1\" AND players.isnew=\"0\" AND players.playerid = playerscorehits.playerid AND ", leagueid );
	wxString query_pitch = wxString::Format( "INSERT INTO leagueplayerspitcher SELECT %d, players.playerid, players.value, currvalue, scoreid, playerscorepitches.value, 0, \"0\" FROM players, playerscorepitches WHERE players.ishitter=\"0\" AND players.isnew=\"0\" AND players.playerid = playerscorepitches.playerid AND ", leagueid );
	wxString query1 = query_hit;
	wxString query2 = query_pitch;
	if( americans.size() > 0 )
	{
		if( americans.size() == 1 && americans.at( 0 ) == 0 )
		{
			query1 += "teamid >= 1 AND teamid <= 15;";
			query2 += "teamid >= 1 AND teamid <= 15;";
		}
		else if( americans.size() >= 1 )
		{
			query1 += "(";
			query2 += "(";
			for( std::vector<int>::const_iterator it = americans.begin(); it < americans.end(); it++ )
			{
				query1 += "teamid = ";
				query2 += "teamid = ";
				query1 << (*it);
				query2 << (*it);
				if( it == americans.end() - 1 )
				{
					query1 += ");";
					query2 += ")";
				}
				else
				{
					query1 += " OR ";
					query2 += " OR ";
				}
			}
		}
		result = sqlite3_exec( m_handle, query1, 0, 0, &error );
		if( result != SQLITE_OK )
		{
			wxMessageBox( wxString::Format( "Error inserting players for the new league: %s", error ) );
			sqlite3_free( error );
		}
		else
		{
			result = sqlite3_exec( m_handle, query2, 0, 0, &error );
			if( result != SQLITE_OK )
			{
				wxMessageBox( wxString::Format( "Error inserting players for the new league: %s", error ) );
				sqlite3_free( error );
			}
		}
	}
	if( nationals.size() > 0 )
	{
		query1 = query_hit;
		query2 = query_pitch;
		if( nationals.size() == 1 && nationals.at( 0 ) == 16 )
		{
			query1 += "teamid >= 17 AND teamid <= 31;";
			query2 += "teamid >= 17 AND teamid <= 31;";
		}
		else if( nationals.size() >= 1 )
		{
			query1 += "(";
			query2 += "(";
			for( std::vector<int>::const_iterator it = nationals.begin(); it < nationals.end(); it++ )
			{
				query1 += "teamid = ";
				query2 += "teamid = ";
				query1 << (*it);
				query2 << (*it);
				if( it == nationals.end() - 1 )
				{
					query1 += ");";
					query2 += ");";
				}
				else
				{
					query1 += " OR ";
					query2 += " OR ";
				}
			}
		}
		result = sqlite3_exec( m_handle, query1, 0, 0, &error );
		if( result != SQLITE_OK )
		{
			wxMessageBox( wxString::Format( "Error inserting players for the new league: %s", error ) );
			sqlite3_free( error );
		}
		else
		{
			result = sqlite3_exec( m_handle, query2, 0, 0, &error );
			if( result != SQLITE_OK )
			{
				wxMessageBox( wxString::Format( "Error inserting players for the new league: %s", error ) );
				sqlite3_free( error );
			}
		}
	}
	return result;
}

int CDb::InsertUpdateOwners(const std::vector<std::string> &owners, int leagueId)
{
	int result = SQLITE_OK;
	wxString query = "INSERT INTO Owners VALUES( NULL, ?, ? );";
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		sqlite3_bind_int( stmt, 1, leagueId );
		std::vector<std::string>::iterator it;
		for( it = const_cast<std::vector<std::string> &>( owners ).begin(); it < const_cast<std::vector<std::string> &>( owners ).end() && result == SQLITE_OK; it++ )
		{
			sqlite3_bind_text( stmt, 2, (*it).c_str(), (*it).length(), 0 );
			result = sqlite3_step( stmt );
			if( result != SQLITE_DONE )
			{
				wxMessageBox( wxString::Format( "Error inserting owners for the new league: %s", sqlite3_errmsg( m_handle ) ) );
				break;
			}
			else
			{
				sqlite3_reset( stmt );
				result = SQLITE_OK;
			}
		}
		sqlite3_finalize( stmt );
	}
	else
	{
		wxMessageBox( wxString::Format( "Error preparing query for inserting the owners: %s", sqlite3_errmsg( m_handle ) ) );
	}
	return result;
}

int CDb::InsertUpdatePositions(const std::map<wxString,int> &positions, int leagueId)
{
	int result = SQLITE_OK;
	wxString query = "INSERT INTO positionsforleague VALUES( (SELECT positionid FROM positions WHERE positionname = ?), ?, ? );";
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		sqlite3_bind_int( stmt, 2, leagueId );
		for( std::map<wxString,int>::const_iterator it = positions.begin(); it != positions.end() && result == SQLITE_OK; it++ )
		{
			sqlite3_bind_text( stmt, 1, (*it).first, (*it).first.length(), 0 );
			sqlite3_bind_int( stmt, 3, (*it).second );
			result = sqlite3_step( stmt );
			if( result != SQLITE_DONE )
			{
				wxMessageBox( wxString::Format( "Error inserting positions for the new league: %s", sqlite3_errmsg( m_handle ) ) );
				break;
			}
			else
			{
				sqlite3_reset( stmt );
				result = SQLITE_OK;
			}
		}
		sqlite3_finalize( stmt );
	}
	else
		wxMessageBox( wxString::Format( "Error preparing query for inserting positions: %s", sqlite3_errmsg( m_handle ) ) );
	return result;
}

int CDb::InsertUpdateHitScoring(const std::vector<wxString> &hits, const std::vector<wxString> &pitch, int leagueId)
{
	int result = SQLITE_OK;
	wxString query = "INSERT INTO scorehitsforleague VALUES( ?, (SELECT scoreid FROM scorehits WHERE scorename = ? ) );";
	unsigned int i;
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		sqlite3_bind_int( stmt, 1, leagueId );
		for( i = 0; i < hits.size() && result == SQLITE_OK; i++ )
		{
			wxString score = hits.at( i );
			sqlite3_bind_text( stmt, 2, score, score.length(), 0 );
			result = sqlite3_step( stmt );
			if( result != SQLITE_DONE )
			{
				wxMessageBox( wxString::Format( "Error inserting score hits for the new league: %s", sqlite3_errmsg( m_handle ) ) );
				break;
			}
			else
			{
				sqlite3_reset( stmt );
				result = SQLITE_OK;
			}
		}
		sqlite3_finalize( stmt );
	}
	else
		wxMessageBox( wxString::Format( "Cannot prepare query for hit scoring for league: %s", sqlite3_errmsg( m_handle ) ) );
	if( result == SQLITE_OK )
	{
		query = "INSERT INTO scorepitchforleague VALUES( ?, (SELECT scoreid FROM scorepitch WHERE scorename = ?) );";
		if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			for( i = 0; i < pitch.size() && result == SQLITE_OK; i++ )
			{
				wxString score = pitch.at( i );
				sqlite3_bind_text( stmt, 2, score, score.length(), 0 );
				result = sqlite3_step( stmt );
				if( result != SQLITE_DONE )
				{
					wxMessageBox( wxString::Format( "Error inserting score pitch for the new league: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
				else
				{
					sqlite3_reset( stmt );
					result = SQLITE_OK;
				}
			}
			sqlite3_finalize( stmt );
		}
		else
			wxMessageBox( wxString::Format( "Cannot prepare query for pitch scoring for league: %s", sqlite3_errmsg( m_handle ) ) );
	}
	return result;
}

int CDb::GetLeagueSettings(CLeagueSettings &settings, int &leagueId)
{
	int result = SQLITE_OK;
	wxString query = "SELECT * FROM Leagues WHERE name = ?;";
	if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
	{
		wxString name = settings.GetLeagueName();
		sqlite3_bind_text( stmt, 1, name, name.length(), 0 );
		while( true )
		{
			result = sqlite3_step( stmt );
			if( result == SQLITE_ROW )
			{
				leagueId = sqlite3_column_int( stmt, 0 );
				settings.SetDraftType( sqlite3_column_int( stmt, 2 ) );
				settings.SetScoringtype( sqlite3_column_int( stmt, 3 ) );
				settings.SetRoundingType( sqlite3_column_int( stmt, 4 ) );
				settings.SetLeagueType( wxString( sqlite3_column_text( stmt, 5 ) ) == wxString( "demo" ) );
				settings.SetSalary( sqlite3_column_int( stmt, 6 ) );
				settings.SetBenchPlayerIncluded( sqlite3_column_int( stmt, 7 ) == 1 );
			}
			else if( result == SQLITE_DONE )
			{
				result = SQLITE_OK;
				break;
			}
			else
			{
				wxMessageBox( wxString::Format( "Error retrieving information about the league: %s", sqlite3_errmsg( m_handle ) ) );
				break;
			}
		}
		sqlite3_finalize( stmt );
		if( result != SQLITE_OK )
			return result;
		query = "SELECT * FROM Owners WHERE id = ?;";
		int count = 0;
		if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
				{
					count++;
					settings.AddOwner( sqlite3_column_text( stmt, 2 ) );
				}
				else if( result == SQLITE_DONE )
				{
					result = SQLITE_OK;
					break;
				}
				else
				{
					wxMessageBox( wxString::Format( "Error retrieving information about the owners: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			settings.SetOwnersNumber( count );
			sqlite3_finalize( stmt );
			if( result != SQLITE_OK )
				return result;
		}
		query = "SELECT positions.positionname, positionsforleague.value FROM positionsforleague, positions WHERE positions.positionid = positionsforleague.positionid AND id = ? ORDER BY positions.positionid;";
		if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
					settings.AddPosition( sqlite3_column_text( stmt, 0 ), sqlite3_column_int( stmt, 1 ) );
				else if( result == SQLITE_DONE )
				{
					result = SQLITE_OK;
					break;
				}
				else
				{
					wxMessageBox( wxString::Format( "Error retrieving information about league positions: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			sqlite3_finalize( stmt );
			if( result != SQLITE_OK )
				return result;
		}
		query = "SELECT * FROM teamsforleague, teams WHERE id = ? AND teamsforleague.teamid = teams.teamid AND teams.teamnational = 0;";
		if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
					settings.AddAmericanLeague( sqlite3_column_int( stmt, 1 ) );
				else if( result == SQLITE_DONE )
				{
					result = SQLITE_OK;
					break;
				}
				else
				{
					wxMessageBox( wxString::Format( "Error retrieving information about teams: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			sqlite3_finalize( stmt );
			if( result != SQLITE_OK )
				return result;
		}
		query = "SELECT * FROM teamsforleague, teams WHERE id = ? AND teamsforleague.teamid = teams.teamid AND teams.teamnational = 1;";
		if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
					settings.AddNationalLeague( sqlite3_column_int( stmt, 1 ) );
				else if( result == SQLITE_DONE )
				{
					result = SQLITE_OK;
					break;
				}
				else
				{
					wxMessageBox( wxString::Format( "Error retrieving information about teams: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			sqlite3_finalize( stmt );
			if( result != SQLITE_OK )
				return result;
		}
		query = "SELECT scorehits.scorename FROM scorehits,scorehitsforleague WHERE scorehits.scoreid = scorehitsforleague.scoreid AND scorehitsforleague.id = ? ORDER BY scorehits.scoreid;";
		if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
					settings.AddHitScoring( sqlite3_column_text( stmt, 0 ) );
				else if( result == SQLITE_DONE )
				{
					result = SQLITE_OK;
					break;
				}
				else
				{
					wxMessageBox( wxString::Format( "Error retrieving information about hitters scoring: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			sqlite3_finalize( stmt );
			if( result != SQLITE_OK )
				return result;
		}
		query = "SELECT scorepitch.scorename FROM scorepitch, scorepitchforleague WHERE scorepitch.scoreid = scorepitchforleague.scoreid AND scorepitchforleague.id = ? ORDER BY scorepitch.scoreid;";
		if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
					settings.AddPitchScoring( sqlite3_column_text( stmt, 0 ) );
				else if( result == SQLITE_DONE )
				{
					result = SQLITE_OK;
					break;
				}
				else
				{
					wxMessageBox( wxString::Format( "Error retrieving information about pitchers scoring: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			sqlite3_finalize( stmt );
			if( result != SQLITE_OK )
				return result;
		}
		query = "SELECT scorehits.scorename, targetshits.value FROM scorehits, targetshits WHERE scorehits.scoreid = targetshits.scoreid AND targetshits.id = ?;";
		if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
					settings.AddTargetHitter( sqlite3_column_text( stmt, 0 ), sqlite3_column_double( stmt, 1 ) );
				else if( result == SQLITE_DONE )
				{
					result = SQLITE_OK;
					break;
				}
				else
				{
					wxMessageBox( wxString::Format( "Error retrieving information about targets: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			sqlite3_finalize( stmt );
			if( result != SQLITE_OK )
				return result;
		}
		query = "SELECT scorepitches.scorename, targetspitches.value FROM scorepitches, targetspitches WHERE scorepitches.scoreid = targetspitches.scoreid AND targetspitches.id = ?;";
		if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
					settings.AddTargetHitter( sqlite3_column_text( stmt, 0 ), sqlite3_column_double( stmt, 1 ) );
				else if( result == SQLITE_DONE )
				{
					result = SQLITE_OK;
					break;
				}
				else
				{
					wxMessageBox( wxString::Format( "Error retrieving information about targets: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			sqlite3_finalize( stmt );
			if( result != SQLITE_OK )
				return result;
		}
	}
	else
		wxMessageBox( wxString::Format( "Could not prepare SELECT query for leagues name: %s", sqlite3_errmsg( m_handle ) ) );
	return result;
}

int CDb::InsertUpdateTeams(const std::vector<int> &americans, const std::vector<int> &nationals, int leagueId)
{
	int result = SQLITE_OK;
	unsigned int i;
	wxString query = "INSERT INTO teamsforleague VALUES( ?, ? );";
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		sqlite3_bind_int( stmt, 1, leagueId );
		for( i = 0; i < americans.size() && result == SQLITE_OK; i++ )
		{
			sqlite3_bind_int( stmt, 2, americans.at( i ) );
			result = sqlite3_step( stmt );
			if( result != SQLITE_DONE )
			{
				wxMessageBox( wxString::Format( "Error inserting score hits for the new league: %s", sqlite3_errmsg( m_handle ) ) );
				break;
			}
			else
			{
				sqlite3_reset( stmt );
				result = SQLITE_OK;
			}
		}
		sqlite3_finalize( stmt );
	}
	else
		wxMessageBox( wxString::Format( "Error reparing the query for inserting teams: %s", sqlite3_errmsg( m_handle ) ) );
	if( result == SQLITE_OK )
	{
		query = "INSERT INTO teamsforleague VALUES( ?, ? );";
		if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			for( i = 0; i < nationals.size() && result == SQLITE_OK; i++ )
			{
				sqlite3_bind_int( stmt, 2, nationals.at( i ) );
				result = sqlite3_step( stmt );
				if( result != SQLITE_DONE )
				{
					wxMessageBox( wxString::Format( "Error inserting teams for the new league: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
				else
				{
					sqlite3_reset( stmt );
					result = SQLITE_OK;
				}
			}
			sqlite3_finalize( stmt );
		}
	}
	return result;
}

int CDb::GetLeagueId(bool isUpdate, const wxString &name)
{
	char *error = NULL;
	int leagueid = -1, result;
	if( !isUpdate )
		return sqlite3_last_insert_rowid( m_handle );
	else
	{
		wxString query = "SELECT id FROM leagues WHERE name = ?;";
		if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) != SQLITE_OK )
		{
			wxMessageBox( wxString::Format( "Could not prepare SELECT query for league id: %s ", error ) );
			return 0;
		}
		else
		{
			sqlite3_bind_text( stmt, 1, name, name.length(), 0 );
			result = sqlite3_step( stmt );
			if( result == SQLITE_ROW )
				leagueid = sqlite3_column_int( stmt, 0 );
			else
			{
				wxMessageBox( wxString::Format( "Error retrieving league id: %s", sqlite3_errmsg( m_handle ) ) );
				leagueid = -1;
			}
		}
		sqlite3_finalize( stmt );
		return leagueid;
	}
}

int CDb::InsertPlayers(const std::vector<std::vector<wxString>> &hitters, const std::vector<std::vector<wxString>> &pitchers)
{
	wxString playerid = wxEmptyString;
	std::vector<std::vector<wxString>> hits = const_cast<std::vector<std::vector<wxString>> &>( hitters );
	std::vector<std::vector<wxString>> pits = const_cast<std::vector<std::vector<wxString>> &>( pitchers );
	if( sqlite3_exec( m_handle, "BEGIN", 0, 0, 0 ) != SQLITE_OK )
	{
		wxMessageBox( "Database error: Can't insert players info into the database" );
		return -1;
	}
	for( std::vector<std::vector<wxString>>::iterator it_out = hits.begin(); it_out < hits.end(); it_out++ )
	{
		for( std::vector<wxString>::iterator it_in = it_out->begin(); it_in < it_out->end(); it_in++ )
		{
			if( it_in == it_out->begin() )
			{
				if( sqlite3_exec( m_handle, *it_in, 0, 0, 0 ) != SQLITE_OK )
				{
					wxMessageBox( wxString::Format( "Error inserting hitters: %s", sqlite3_errmsg( m_handle ) ) );
					sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
					return -1;
				}
				else
					playerid << sqlite3_last_insert_rowid( m_handle );
			}
			else
			{
				wxString temp = *it_in;
				temp.Replace( "NULL", playerid );
				if( sqlite3_exec( m_handle, temp, 0, 0, 0 ) != SQLITE_OK )
				{
					wxMessageBox( wxString::Format( "Error inserting hitters data: %s", sqlite3_errmsg( m_handle ) ) );
					sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
					return -1;
				}
			}
		}
		playerid = wxEmptyString;
	}
	for( std::vector<std::vector<wxString>>::iterator it_out = pits.begin(); it_out < pits.end(); it_out++ )
	{
		for( std::vector<wxString>::iterator it_in = it_out->begin(); it_in < it_out->end(); it_in++ )
		{
			if( it_in == it_out->begin() )
			{
				if( sqlite3_exec( m_handle, *it_in, 0, 0, 0 ) != SQLITE_OK )
				{
					wxMessageBox( wxString::Format( "Error inserting pitchers: %s", sqlite3_errmsg( m_handle ) ) );
					sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
					return -1;
				}
				else
					playerid << sqlite3_last_insert_rowid( m_handle );
			}
			else
			{
				wxString temp = *it_in;
				temp.Replace( "NULL", playerid );
				if( sqlite3_exec( m_handle, temp, 0, 0, 0 ) != SQLITE_OK )
				{
					wxMessageBox( wxString::Format( "Error inserting pitchers data: %s", sqlite3_errmsg( m_handle ) ) );
					sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
					return -1;
				}
			}
		}
		playerid = wxEmptyString;
	}
	if( sqlite3_exec( m_handle, "COMMIT", 0, 0, 0 ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Error committing player's data: %s", sqlite3_errmsg( m_handle ) ) );
		if( sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 ) != SQLITE_OK )
			wxMessageBox( "Serious problem with the database occur. Please restart and try again." );
		return -1;
	}
	else if( sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 ) != SQLITE_OK )
	{
		wxMessageBox( "Serious problem with the database occur. Please restart and try again." );
		return -1;
	}
	return 0;
}

int CDb::LoadLeagueData(CLeagueSettings *league, int &leagueId)
{
	int result;
	return ( result = GetLeagueSettings( *league, leagueId ) );
}

int CDb::GetPlayersForLeague(CLeagueData &m_data, const CLeagueSettings &settings, const int &leagueId, int &numPlayers, std::vector<CPlayer> &draftResult)
{
	sqlite3_stmt *stm1, *stm2, *stm3, *stm4;
	int result = SQLITE_OK, playerid;
	wxString query1, query;
	query1 = "SELECT scorehits.scorename, leagueplayershitter.score FROM scorehits, leagueplayershitter WHERE scorehits.scoreid = leagueplayershitter.scoreid AND leagueplayershitter.playerid = ? AND leagueplayershitter.id = ? ORDER BY scorehits.scoreid;";
	if( ( result = sqlite3_prepare_v2( m_handle, query1, -1, &stm1, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s", sqlite3_errmsg( m_handle ) ) );
		return result;
	}
	else
		sqlite3_bind_int( stm1, 2, leagueId );
	wxString query2 = "SELECT positions.positionname FROM players, playerpositioninleague, positions WHERE players.playerid = playerpositioninleague.playerid AND playerpositioninleague.positionid = positions.positionid AND players.playerid = ? AND playerpositioninleague.id = ?;";
	if( ( result = sqlite3_prepare_v2( m_handle, query2, -1, &stm2, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s" ), sqlite3_errmsg( m_handle ) );
		return result;
	}
	else
		sqlite3_bind_int( stm2, 2, leagueId );
	wxString query3 = "SELECT ownername, draftprice, draftorder, draftposition FROM playerdraft, owners WHERE playerdraft.id = ? AND playerid = ? AND owners.ownerid = playerdraft.ownerid ORDER BY draftorder;";
	if( ( result = sqlite3_prepare_v2( m_handle, query3, -1, &stm3, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Could not read draft information for player: %s", sqlite3_errmsg( m_handle ) ) );
		return result;
	}
	else
		sqlite3_bind_int( stm3, 1, leagueId );
	wxString query4 = "SELECT scorehits.scorename, playerscorehits.value FROM scorehits, playerscorehits WHERE scorehits.scoreid = playerscorehits.scoreid AND playerscorehits.playerid = ?;";
	if( ( result = sqlite3_prepare_v2( m_handle, query4, -1, &stm4, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Could not read score information for player: %s", sqlite3_errmsg( m_handle ) ) );
		return result;
	}
	std::vector<int> americanleague = settings.GetAmericanLeague();
	std::vector<int> nationalleague = settings.GetNationalLeague();
	std::vector<wxString> positions;
	std::map<wxString,double> scoring, originalScoring;
	query = "SELECT DISTINCT players.playerid, players.name, players.age, leagueplayershitter.value, teams.name, teams.shortname, leagueplayershitter.currvalue, players.notes, leagueplayershitter.draft, players.value, leagueplayershitter.isnew FROM players, teams, leagueplayershitter WHERE leagueplayershitter.playerid = players.playerid AND players.teamid = teams.teamid AND leagueplayershitter.id = ?;";
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		sqlite3_bind_int( stmt, 1, leagueId );
		while( true )
		{
			result = sqlite3_step( stmt );
			if( result == SQLITE_ROW )
			{
				playerid = sqlite3_column_int( stmt, 0 );
				sqlite3_bind_int( stm1, 1, playerid );
				while( true )
				{
					result = sqlite3_step( stm1 );
					if( result == SQLITE_ROW )
						scoring[sqlite3_column_text( stm1, 0)] = sqlite3_column_double( stm1, 1 );
					else if( result == SQLITE_DONE )
					{
						result = SQLITE_OK;
						break;
					}
					else
					{
						wxMessageBox( wxString::Format( "Error retrieving score information for the player %d: %s", playerid, sqlite3_errmsg( m_handle ) ) );
						break;
					}
				}
				sqlite3_reset( stm1 );
				if( result != SQLITE_OK )
				{
					sqlite3_finalize( stmt );
					sqlite3_finalize( stm1 );
					sqlite3_finalize( stm2 );
					sqlite3_finalize( stm3 );
					sqlite3_finalize( stm4 );
					return result;
				}
				sqlite3_bind_int( stm2, 1, playerid );
				while( true )
				{
					result = sqlite3_step( stm2 );
					if( result == SQLITE_ROW )
						positions.push_back( sqlite3_column_text( stm2, 0 ) );
					else if( result == SQLITE_DONE )
					{
						result = SQLITE_OK;
						break;
					}
					else
					{
						wxMessageBox( wxString::Format( "Error retrieving positions for the player %d: %s", playerid, sqlite3_errmsg( m_handle ) ) );
						break;
					}
				}
				sqlite3_reset( stm2 );
				if( result != SQLITE_OK )
				{
					sqlite3_finalize( stmt );
					sqlite3_finalize( stm1 );
					sqlite3_finalize( stm2 );
					sqlite3_finalize( stm3 );
					sqlite3_finalize( stm4 );
					return result;
				}
				sqlite3_bind_int( stm4, 1, playerid );
				while( true )
				{
					result = sqlite3_step( stm4 );
					if( result == SQLITE_ROW )
						originalScoring[sqlite3_column_text( stm4, 0 )] = sqlite3_column_double( stm4, 1 );
					else if( result == SQLITE_DONE )
					{
						result = SQLITE_OK;
						break;
					}
					else
					{
						wxMessageBox( wxString::Format( "Error retrieving scoring information for a player: %s", sqlite3_errmsg( m_handle ) ) );
						break;
					}
				}
				sqlite3_reset( stm4 );
				if( result != SQLITE_OK )
				{
					sqlite3_finalize( stmt );
					sqlite3_finalize( stm1 );
					sqlite3_finalize( stm2 );
					sqlite3_finalize( stm3 );
					sqlite3_finalize( stm4 );
					return result;
				}
				CPlayer player( sqlite3_column_int( stmt, 0 ), sqlite3_column_text( stmt, 1 ), positions, sqlite3_column_int( stmt, 2 ), sqlite3_column_int( stmt, 3 ), sqlite3_column_text( stmt, 4 ), sqlite3_column_text( stmt, 5 ), scoring, true, sqlite3_column_double( stmt, 6 ), sqlite3_column_text( stmt, 7 ), sqlite3_column_int( stmt, 9 ) );
				player.SetOriginalScore( originalScoring );
				player.SetNewPlayer( wxAtoi( sqlite3_column_text( stmt, 10 ) ) == 1 ? true : false );
				if( result == SQLITE_OK && sqlite3_column_int( stmt, 8 ) == 1 )
				{
					player.DraftPlayer( true );
					sqlite3_bind_int( stm3, 2, playerid );
					while( true )
					{
						result = sqlite3_step( stm3 );
						if( result == SQLITE_ROW )
						{
							player.SetOwner( sqlite3_column_text( stm3, 0 ) );
							player.SetAmountPaid( sqlite3_column_int( stm3, 1 ) );
							player.SetDraftOrder( sqlite3_column_int( stm3, 2 ) );
							player.SetDraftedPosition( sqlite3_column_text( stm3, 3 ) );
						}
						else if( result == SQLITE_DONE )
						{
							result = SQLITE_OK;
							break;
						}
						else
						{
							wxMessageBox( wxString::Format( "Error retrieving draft information for player %d: %s", playerid, sqlite3_errmsg( m_handle ) ) );
							break;
						}
					}
					sqlite3_reset( stm3 );
					if( result != SQLITE_OK )
					{
						sqlite3_finalize( stmt );
						sqlite3_finalize( stm1 );
						sqlite3_finalize( stm2 );
						sqlite3_finalize( stm3 );
						sqlite3_finalize( stm4 );
						return result;
					}
				}
				if( sqlite3_column_double( stmt, 6 ) > 0 )
					numPlayers++;
				m_data.m_players->push_back( player );
				if( player.IsPlayerDrafted() )
					draftResult.push_back( player );
				scoring.clear();
				positions.clear();
			}
			else if( result == SQLITE_DONE )
			{
				result = SQLITE_OK;
				break;
			}
			else
			{
				wxMessageBox( wxString::Format ( "Could not prepare SELECT query for getting players information: %s" ), sqlite3_errmsg( m_handle ) );
				break;
			}
		}
		sqlite3_finalize( stmt );
		if( result != SQLITE_OK )
		{
			sqlite3_finalize( stm1 );
			sqlite3_finalize( stm2 );
			sqlite3_finalize( stm3 );
			sqlite3_finalize( stm4 );
			return result;
		}
	}
	else
		wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s" ), sqlite3_errmsg( m_handle ) );
	sqlite3_finalize( stm1 );
	sqlite3_finalize( stm4 );
	if( result == SQLITE_OK || result == SQLITE_DONE )
	{
		query = "SELECT DISTINCT players.playerid, players.name, players.age, leagueplayerspitcher.value, teams.name, teams.shortname, leagueplayerspitcher.currvalue, players.notes, leagueplayerspitcher.draft, players.value, leagueplayerspitcher.isnew FROM players, teams, leagueplayerspitcher WHERE leagueplayerspitcher.playerid = players.playerid AND players.teamid = teams.teamid AND leagueplayerspitcher.id = ?;";
		query1 = "SELECT scorepitch.scorename, leagueplayerspitcher.score FROM scorepitch, leagueplayerspitcher WHERE scorepitch.scoreid = leagueplayerspitcher.scoreid AND leagueplayerspitcher.playerid = ? AND leagueplayerspitcher.id = ? ORDER BY scorepitch.scoreid;";
		if( ( result = sqlite3_prepare_v2( m_handle, query1, -1, &stm1, 0 ) ) != SQLITE_OK )
		{
			wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting score information: %s", sqlite3_errmsg( m_handle ) ) );
			return result;
		}
		else
			sqlite3_bind_int( stm1, 2, leagueId );
		query4 = "SELECT scorehits.scorename, playerscorepitches.value FROM scorehits, playerscorepitches WHERE scorehits.scoreid = playerscorepitches.scoreid AND playerscorepitches.playerid = ?;";
		if( ( result = sqlite3_prepare_v2( m_handle, query4, -1, &stm4, 0 ) ) != SQLITE_OK )
		{
			wxMessageBox( wxString::Format( "Could not read score information for player: %s", sqlite3_errmsg( m_handle ) ) );
			return result;
		}
		if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
				{
					playerid = sqlite3_column_int( stmt, 0 );
					sqlite3_bind_int( stm1, 1, playerid );
					while( true )
					{
						result = sqlite3_step( stm1 );
						if( result == SQLITE_ROW )
							scoring[sqlite3_column_text( stm1, 0)] = sqlite3_column_double( stm1, 1 );
						else if( result == SQLITE_DONE )
						{
							result = SQLITE_OK;
							break;
						}
						else
						{
							wxMessageBox( wxString::Format( "Error retrieving score information for the player %d: %s", playerid, sqlite3_errmsg( m_handle ) ) );
							break;
						}
					}
					sqlite3_reset( stm1 );
					if( result != SQLITE_OK )
					{
						sqlite3_finalize( stmt );
						sqlite3_finalize( stm1 );
						sqlite3_finalize( stm2 );
						sqlite3_finalize( stm3 );
						sqlite3_finalize( stm4 );
						return result;
					}
					sqlite3_bind_int( stm2, 1, playerid );
					while( true )
					{
						result = sqlite3_step( stm2 );
						if( result == SQLITE_ROW )
							positions.push_back( sqlite3_column_text( stm2, 0 ) );
						else if( result == SQLITE_DONE )
						{
							result = SQLITE_OK;
							break;
						}
						else
						{
							wxMessageBox( wxString::Format( "Error retrieving positions for the player %d: %s", playerid, sqlite3_errmsg( m_handle ) ) );
							break;
						}
					}
					sqlite3_reset( stm2 );
					if( result != SQLITE_OK )
					{
						sqlite3_finalize( stmt );
						sqlite3_finalize( stm1 );
						sqlite3_finalize( stm2 );
						sqlite3_finalize( stm3 );
						sqlite3_finalize( stm4 );
						return result;
					}
					sqlite3_bind_int( stm4, 1, playerid );
					while( true )
					{
						result = sqlite3_step( stm4 );
						if( result == SQLITE_ROW )
							originalScoring[sqlite3_column_text( stm4, 0 )] = sqlite3_column_double( stm4, 1 );
						else if( result == SQLITE_DONE )
						{
							result = SQLITE_OK;
							break;
						}
						else
						{
							wxMessageBox( wxString::Format( "Error retrieving scoring information for a player: %s", sqlite3_errmsg( m_handle ) ) );
							break;
						}
					}
					sqlite3_reset( stm4 );
					if( result != SQLITE_OK )
					{
						sqlite3_finalize( stmt );
						sqlite3_finalize( stm1 );
						sqlite3_finalize( stm2 );
						sqlite3_finalize( stm3 );
						sqlite3_finalize( stm4 );
						return result;
					}
					CPlayer player( sqlite3_column_int( stmt, 0 ), sqlite3_column_text( stmt, 1 ), positions, sqlite3_column_int( stmt, 2 ), sqlite3_column_int( stmt, 3 ), sqlite3_column_text( stmt, 4 ), sqlite3_column_text( stmt, 5 ), scoring, false, sqlite3_column_double( stmt, 6 ), sqlite3_column_text( stmt, 7 ), sqlite3_column_int( stmt, 9 ) );
					player.SetOriginalScore( originalScoring );
					player.SetNewPlayer( wxAtoi( sqlite3_column_text( stmt, 10 ) ) == 1 ? true : false );
					if( result == SQLITE_OK && sqlite3_column_int( stmt, 8 ) == 1 )
					{
						player.DraftPlayer( true );
						sqlite3_bind_int( stm3, 2, playerid );
						while( true )
						{
							result = sqlite3_step( stm3 );
							if( result == SQLITE_ROW )
							{
								player.SetOwner( sqlite3_column_text( stm3, 0 ) );
								player.SetAmountPaid( sqlite3_column_int( stm3, 1 ) );
								player.SetDraftOrder( sqlite3_column_int( stm3, 2 ) );
								player.SetDraftedPosition( sqlite3_column_text( stm3, 3 ) );
							}
							else if( result == SQLITE_DONE )
							{
								result = SQLITE_OK;
								break;
							}
							else
							{
								wxMessageBox( wxString::Format( "Error retrieving draft information for player %d: %s", playerid, sqlite3_errmsg( m_handle ) ) );
								break;
							}
						}
						sqlite3_reset( stm3 );
						if( result != SQLITE_OK )
						{
							sqlite3_finalize( stmt );
							sqlite3_finalize( stm1 );
							sqlite3_finalize( stm2 );
							sqlite3_finalize( stm3 );
							sqlite3_finalize( stm4 );
							return result;
						}
					}
					if( sqlite3_column_double( stmt, 3 ) > 0 )
						numPlayers++;
					m_data.m_players->push_back( player );
					if( player.IsPlayerDrafted() )
						draftResult.push_back( player );
					scoring.clear();
					positions.clear();
				}
				else if( result == SQLITE_DONE )
				{
					result = SQLITE_OK;
					break;
				}
				else
				{
					wxMessageBox( wxString::Format ( "Could not prepare SELECT query for getting players information: %s" ), sqlite3_errmsg( m_handle ) );
					break;
				}
			}
			sqlite3_finalize( stmt );
			if( result != SQLITE_OK )
			{
				sqlite3_finalize( stm1 );
				sqlite3_finalize( stm2 );
				sqlite3_finalize( stm3 );
				sqlite3_finalize( stm4 );
				return result;
			}
		}
	}
	sqlite3_finalize( stm1 );
	sqlite3_finalize( stm2 );
	sqlite3_finalize( stm3 );
	sqlite3_finalize( stm4 );
	return result;
}

int CDb::GetSelectedTeams(const CLeagueSettings &settings, std::map<wxString,wxString> &teams)
{
	int result = SQLITE_OK;
	std::vector<int> americanleague = settings.GetAmericanLeague();
	std::vector<int> nationalleague = settings.GetNationalLeague();
	wxString query = "SELECT name, shortname FROM teams WHERE ";
	wxString query1 = query;
	if( americanleague.size() == 1 && americanleague.at( 0 ) == 0 )
	{
		query1 += "teamnational = 0 AND name NOT LIKE \"ALL%\";";
		if( ( result = sqlite3_prepare_v2( m_handle, query1, -1, &stmt, 0 ) ) == SQLITE_OK )
		{
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
					teams[sqlite3_column_text( stmt, 1)] = sqlite3_column_text( stmt, 0 );
				else if( result == SQLITE_DONE )
					break;
				else
				{
					wxMessageBox( wxString::Format( "Error retrieving information about teams: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			sqlite3_finalize( stmt );
			if( result != SQLITE_DONE )
				return result;
		}
		else
			wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s", sqlite3_errmsg( m_handle ) ) );
	}
	else
	{
		if( americanleague.size() > 0 )
		{
			for( std::vector<int>::iterator it = americanleague.begin(); it < americanleague.end(); it++ )
			{
				query1 += "teamid = ";
				query1 << *it;
				if( it == americanleague.end() - 1 )
					query1 += ";";
				else
					query1 += " OR ";
			}
			if( ( result = sqlite3_prepare_v2( m_handle, query1, -1, &stmt, 0 ) ) == SQLITE_OK )
			{
				while( true )
				{
					result = sqlite3_step( stmt );
					if( result == SQLITE_ROW )
						teams[sqlite3_column_text( stmt, 1 )] = sqlite3_column_text( stmt, 0 );
					else if( result == SQLITE_DONE )
						break;
					else
					{
						wxMessageBox( wxString::Format( "Error retrieving information about teams: %s", sqlite3_errmsg( m_handle ) ) );
						break;
					}
				}
				sqlite3_finalize( stmt );
				if( result != SQLITE_DONE )
					return result;
			}
			else
				wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s", sqlite3_errmsg( m_handle ) ) );
		}
	}
	if( result == SQLITE_OK )
	{
		if( nationalleague.size() == 1 && nationalleague.at( 0 ) == 16 )
		{
			query += "teamnational = 1 AND name NOT LIKE \"ALL%\";";
			if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
			{
				while( true )
				{
					result = sqlite3_step( stmt );
					if( result == SQLITE_ROW )
						teams[sqlite3_column_text( stmt, 1)] = sqlite3_column_text( stmt, 0 );
					else if( result == SQLITE_DONE )
						break;
					else
					{
						wxMessageBox( wxString::Format( "Error retrieving information about teams: %s", sqlite3_errmsg( m_handle ) ) );
						break;
					}
				}
				sqlite3_finalize( stmt );
				if( result != SQLITE_DONE )
					return result;
			}
			else
				wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s", sqlite3_errmsg( m_handle ) ) );
		}
		else
		{
			if( nationalleague.size() > 0 )
			{
				for( std::vector<int>::iterator it = nationalleague.begin(); it < nationalleague.end(); it++ )
				{
					query += "teamid = ";
					query << *it;
					if( it == nationalleague.end() - 1 )
						query += ";";
					else
						query += " OR ";
				}
			}
			if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
			{
				while( true )
				{
					result = sqlite3_step( stmt );
					if( result == SQLITE_ROW )
						teams[sqlite3_column_text( stmt, 1 )] = sqlite3_column_text( stmt, 0 );
					else if( result == SQLITE_DONE )
						break;
					else
					{
						wxMessageBox( wxString::Format( "Error retrieving information about teams: %s", sqlite3_errmsg( m_handle ) ) );
						break;
					}
				}
				sqlite3_finalize( stmt );
				if( result != SQLITE_DONE )
					return result;
			}
			else
				wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s", sqlite3_errmsg( m_handle ) ) );
		}
	}
	return result;
}

int CDb::GetPositionCount(wxString leagueid, std::map<wxString,int> &positions)
{
	int result = SQLITE_OK;
	wxString query = wxString::Format( "SELECT positions.positionname, positionsforleague.value FROM positionsforleague, positions, leagues WHERE leagues.id = positionsforleague.id AND positions.positionid = positionsforleague.positionid AND leagues.name = \"%s\";", leagueid );
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		while( true )
		{
			result = sqlite3_step( stmt );
			if( result == SQLITE_ROW )
				positions[sqlite3_column_text( stmt, 0 )] = sqlite3_column_int( stmt, 1 );
			else if( result == SQLITE_DONE )
				break;
			else
			{
				wxMessageBox( wxString::Format( "Error retrieving positions: %s", sqlite3_errmsg( m_handle ) ) );
				break;
			}
		}
		sqlite3_finalize( stmt );
		if( result != SQLITE_DONE )
			return result;
	}
	else
		wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s", sqlite3_errmsg( m_handle ) ) );
	return result;
}

int CDb::GetLeagueScoring(wxString league, std::vector<wxString> &hitters, std::vector<wxString> &pitchers)
{
	int result = SQLITE_OK;
	wxString query = wxString::Format( "SELECT scorehits.scorename FROM scorehits, scorehitsforleague, leagues WHERE leagues.id = scorehitsforleague.id AND scorehitsforleague.scoreid = scorehits.scoreid AND leagues.name = \"%s\";", league );
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		while( true )
		{
			result = sqlite3_step( stmt );
			if( result == SQLITE_ROW )
				hitters.push_back( sqlite3_column_text( stmt, 0 ) );
			else if( result == SQLITE_DONE )
				break;
			else
			{
				wxMessageBox( wxString::Format( "Error retrieving scoring information: %s", sqlite3_errmsg( m_handle ) ) );
				break;
			}
		}
		sqlite3_finalize( stmt );
		if( result != SQLITE_DONE )
			return result;
		query = wxString::Format( "SELECT scorepitch.scorename FROM scorepitch, scorepitchforleague, leagues WHERE leagues.id = scorepitchforleague.id AND scorepitchforleague.scoreid = scorepitch.scoreid AND leagues.name = \"%s\";", league );
		if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
		{
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
					pitchers.push_back( sqlite3_column_text( stmt, 0 ) );
				else if( result == SQLITE_DONE )
					break;
				else
				{
					wxMessageBox( wxString::Format( "Error retrieving scoring informmation: %S", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			sqlite3_finalize( stmt );
			if( result != SQLITE_DONE )
				return result;
		}
		else
			wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s", sqlite3_errmsg( m_handle ) ) );
	}
	else
		wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s", sqlite3_errmsg( m_handle ) ) );
	return result;
}

int CDb::AddNewPlayer(CPlayer &player, const int leagueId, bool skipCheck)
{
	int result = SQLITE_OK;
	sqlite3_stmt *stm1, *stm2;
	long playerid = 0;
	if( !skipCheck )
	{
		if( IsPlayerExist( const_cast<CPlayer &>( player ) ) )
		{
			if( IsPlayerInLeague( player, leagueId ) )
			{
				wxMessageBox( "Such player already exist and is added to the current league." );
				return -1;
			}
			else
			{
				wxMessageBox( "This player already exist and will be added to the current league." );
				if( AddPlayerToLeague( player, leagueId ) != SQLITE_OK )
					return -1;
			}
		}
	}
	wxString query2, query3, query4;
	wxString query1 = wxString::Format( "INSERT INTO players VALUES( NULL, \"%s\", \"%d\", %d, %d, (SELECT teamid FROM teams WHERE shortname = \"%s\"), %.3f, \"\", \"0\", \"1\" );", const_cast<CPlayer &>( player ).GetName(), player.IsHitter() ? 1 : 0, player.GetAge(), player.GetValue(), const_cast<CPlayer &>( player ).GetAbbeviatedTeamName(), player.GetCurrentValue() );
	wxString query;
	sqlite3_exec( m_handle, "BEGIN", 0, 0, 0 );
	if( ( result = sqlite3_prepare_v2( m_handle, query1, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		result = sqlite3_step( stmt );
		sqlite3_finalize( stmt );
		if( result == SQLITE_DONE )
		{
			playerid = sqlite3_last_insert_rowid( m_handle );
			query = "INSERT INTO playerposition VALUES( ?, ( SELECT positionid FROM positions WHERE positionname = ? ) );";
			query2 = "INSERT INTO playerpositioninleague VALUES( ?, (SELECT positionid FROM positions WHERE positionname = ?), ? )";
			if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
			{
				sqlite3_bind_int( stmt, 1, playerid );
			}
			if( ( result = sqlite3_prepare_v2( m_handle, query2, -1, &stm1, 0 ) ) == SQLITE_OK )
			{
				sqlite3_bind_int( stm1, 1, playerid );
				sqlite3_bind_int( stm1, 3, leagueId );
			}
			for( std::vector<wxString>::iterator it_pos = const_cast<CPlayer &>( player ).GetPositions().begin(); it_pos < const_cast<CPlayer &>( player ).GetPositions().end() && result == SQLITE_OK; it_pos++ )
			{
				wxString position = (*it_pos);
				sqlite3_bind_text( stmt, 2, position, position.length(), 0 );
				sqlite3_bind_text( stm1, 2, position, position.length(), 0 );
				result = sqlite3_step( stmt );
				result = sqlite3_step( stm1 );
				sqlite3_reset( stmt );
				sqlite3_reset( stm1 );
				if( result == SQLITE_DONE )
					result = SQLITE_OK;
			}
			sqlite3_finalize( stmt );
			sqlite3_finalize( stm1 );
			if( player.GetScoring().size() > 0 && result == SQLITE_OK )
			{
				if( player.IsHitter() )
				{
					query2 = "INSERT INTO leagueplayershitter VALUES( ?, ?, ?, ?, (SELECT scoreid FROM scorehits WHERE scorename = ?), ?, 0, \"1\" );";
					query4 = "INSERT INTO playerscorehits VALUES( ?, (SELECT scoreid FROM scorehits WHERE scorename = ?), ? );";
				}
				else
				{
					query2 = "INSERT INTO leagueplayerspitcher VALUES( ?, ?, ?, ?, (SELECT scoreid FROM scorepitch WHERE scorename = ?), ?, 0, \"1\" );";
					query4 = "INSERT INTO playerscorepitches VALUES( ?, (SELECT scoreid FROM scorepitch WHERE scorename = ?), ? );";
				}
				if( ( result = sqlite3_prepare_v2( m_handle, query2, -1, &stm1, 0 ) ) == SQLITE_OK )
				{
					sqlite3_bind_int( stm1, 1, leagueId );
					sqlite3_bind_int( stm1, 2, playerid );
					sqlite3_bind_int( stm1, 3, player.GetValue() );
					sqlite3_bind_double( stm1, 4, player.GetCurrentValue() );
				}
				if( ( result = sqlite3_prepare_v2( m_handle, query4, -1, &stm2, 0 ) ) == SQLITE_OK )
				{
					sqlite3_bind_int( stm2, 1, playerid );
				}
				for( std::map<wxString,double>::const_iterator it = const_cast<CPlayer &>( player ).GetScoring().begin(); it != const_cast<CPlayer &>( player ).GetScoring().end() && result == SQLITE_OK; it++ )
				{
					wxString scorename = (*it).first;
					sqlite3_bind_text( stm1, 5, scorename, scorename.length(), 0 );
					sqlite3_bind_text( stm2, 2, scorename, scorename.length(), 0 );
					sqlite3_bind_double( stm1, 6, (*it).second );
					sqlite3_bind_double( stm2, 3, (*it).second );
					result = sqlite3_step( stm1 );
					sqlite3_reset( stm1 );
					result = sqlite3_step( stm2 );
					sqlite3_reset( stm2 );
					if( result == SQLITE_DONE )
						result = SQLITE_OK;
				}
				sqlite3_finalize( stm1 );
				sqlite3_finalize( stm2 );
			}
		}
	}
	if( result == SQLITE_OK || result == SQLITE_DONE )
	{
		result = sqlite3_exec( m_handle, "COMMIT", 0, 0, 0 );
		if( result != SQLITE_OK )
		{
			wxMessageBox( "Error committing player information: %s", sqlite3_errmsg( m_handle ) );
			result = sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
			if( result != SQLITE_OK )
				wxMessageBox( "Serious problem with the database occur. Please restart and try again." );
			return result;
		}
		player.SetNewPlayerId( playerid );
	}
	else
	{
		wxMessageBox( wxString::Format( "Error inserting player's information: %s", sqlite3_errmsg( m_handle ) ) );
		result = sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
		if( result != SQLITE_OK )
			wxMessageBox( "Serious problem with the database occur. Please restart and try again" );
	}
	return result;
}

bool CDb::UpdatePlayer(const CPlayer &player, const int leagueId)
{
	int result = SQLITE_OK;
	wxString query = wxString::Format( "UPDATE players SET age = ?, teamid = (SELECT teamid FROM teams WHERE teams.shortname = ?), notes = ? WHERE playerid = ?;" );
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Error updating player: %s", sqlite3_errmsg( m_handle ) ) );
	}
	else
	{
		wxString team = const_cast<CPlayer &>( player ).GetAbbeviatedTeamName();
		wxString notes = const_cast<CPlayer &>( player ).GetNotes();
		sqlite3_bind_int( stmt, 1, player.GetAge() );
		sqlite3_bind_text( stmt, 2, team, team.length() , 0 );
		sqlite3_bind_text( stmt, 3, notes, notes.length(), 0 );
		sqlite3_bind_int( stmt, 4, player.GetPlayerId() );
		result = sqlite3_step( stmt );
	}
	sqlite3_finalize( stmt );
	if( result == SQLITE_DONE )
	{
		query = "DELETE FROM playerpositioninleague WHERE id = ? AND playerid = ?";
		if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
			wxMessageBox( wxString::Format( "Error updating player: %s", sqlite3_errmsg( m_handle ) ) );
		else
		{
			sqlite3_bind_int( stmt, 1, leagueId );
			sqlite3_bind_int( stmt, 2, player.GetPlayerId() );
			result = sqlite3_step( stmt );
		}
		sqlite3_finalize( stmt );
		if( result != SQLITE_DONE )
			wxMessageBox( wxString::Format( "Error updating player: %s", sqlite3_errmsg( m_handle ) ) );
		else
		{
			query = "INSERT INTO playerpositioninleague VALUES( ?, (SELECT positionid FROM positions WHERE positionname = ?), ? );";
			if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
				wxMessageBox( wxString::Format( "Error updating player positions: %s", sqlite3_errmsg( m_handle ) ) );
			else
			{
				sqlite3_bind_int( stmt, 1, player.GetPlayerId() );
				sqlite3_bind_int( stmt, 3, leagueId );
				for( std::vector<wxString>::iterator it_pos = const_cast<CPlayer &>( player ).GetPositions().begin(); it_pos < const_cast<CPlayer &>( player ).GetPositions().end(); it_pos++ )
				{
					wxString position = (*it_pos);
					sqlite3_bind_text( stmt, 2, position, position.length(), 0 );
					result = sqlite3_step( stmt );
					sqlite3_reset( stmt );
				}
				sqlite3_finalize( stmt );
				if( result == SQLITE_DONE )
				{
					bool isHitter = player.IsHitter();
					if( isHitter )
						query = "UPDATE leagueplayershitter SET value = ?, currvalue = ?, draft = ?, score = ? WHERE id = ? AND playerid = ? AND scoreid = (SELECT scoreid FROM scorehits WHERE scorename = ?);"; 
					else
						query = "UPDATE leagueplayerspitcher SET value = ?, currvalue = ?, draft = ?, score = ? WHERE id = ? AND playerid = ? AND scoreid = (SELECT scoreid FROM scorepitch WHERE scorename = ?);";
					if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
						wxMessageBox( wxString::Format( "Error preparing query for inserting score information: %s", sqlite3_errmsg( m_handle ) ) );
					else
					{
						sqlite3_bind_int( stmt, 1, player.GetValue() );
						sqlite3_bind_double( stmt, 2, player.GetCurrentValue() );
						sqlite3_bind_int( stmt, 3, player.IsPlayerDrafted() ? 1 : 0 );
						sqlite3_bind_int( stmt, 5, leagueId );
						sqlite3_bind_int( stmt, 6, player.GetPlayerId() );
						for( std::map<wxString,double>::iterator it = const_cast<CPlayer &>( player ).GetScoring().begin(); it != const_cast<CPlayer &>( player ).GetScoring().end(); it++ )
						{
							wxString score = (*it).first;
							sqlite3_bind_double( stmt, 4, (*it).second );
							sqlite3_bind_text( stmt, 7, score, score.length(), 0 );
							result = sqlite3_step( stmt );
							sqlite3_reset( stmt );
						}
						sqlite3_finalize( stmt );
						if( result == SQLITE_DONE && player.IsPlayerDrafted() )
						{
							query = wxString::Format( "INSERT INTO playerdraft VALUES( ?, ?, ( SELECT ownerid FROM owners WHERE ownername = ? AND id = ?2 ), ?, ?, ? );" );
							if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
								wxMessageBox( wxString::Format( "Error updating players: %s", sqlite3_errmsg( m_handle ) ) );
							else
							{
								wxString owner = const_cast<CPlayer &>( player ).GetOwner();
								wxString position = const_cast<CPlayer &>( player ).GetDraftedPosition();
								sqlite3_bind_int( stmt, 1, player.GetPlayerId() );
								sqlite3_bind_int( stmt, 2, leagueId );
								sqlite3_bind_text( stmt, 3, owner, owner.length(), 0 );
								sqlite3_bind_int( stmt, 4, player.GetAmountPaid() );
								sqlite3_bind_int( stmt, 5, player.GetDraftOrder() );
								sqlite3_bind_text( stmt, 6, position, position.length(), 0 );
								result = sqlite3_step( stmt );
							}
							sqlite3_finalize( stmt );
						}
					}
				}
				else
					wxMessageBox( wxString::Format( "Error updating players positions: %s", sqlite3_errmsg( m_handle ) ) );
			}
		}
	}
	else
		wxMessageBox( wxString::Format( "Error updating player information: %s", sqlite3_errmsg( m_handle ) ) );
	if( result == SQLITE_DONE )
		return false;
	else
		return true;
}

bool CDb::IsPlayerExist(CPlayer &player)
{
	int result = SQLITE_OK;
	bool found = false;
	wxString query = wxString::Format( "SELECT players.playerid FROM players WHERE players.name = \"%s\" AND players.age = %d AND players.teamid = (SELECT teamid FROM teams WHERE teams.name = \"%s\");", const_cast<CPlayer &>( player ).GetName(), player.GetAge(), const_cast<CPlayer &>( player ).GetTeam() );
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
		wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s", sqlite3_errmsg( m_handle ) ) );
	else
	{
		result = sqlite3_step( stmt );
		if( result == SQLITE_ROW )
		{
			found = true;
			player.SetNewPlayerId( sqlite3_column_int( stmt, 0 ) );
		}
		else if( result != SQLITE_DONE )
			wxMessageBox( wxString::Format( "Error checking player for existence: %s", sqlite3_errmsg( m_handle ) ) );
	}
	sqlite3_finalize( stmt );
	return found;
}

bool CDb::IsPlayerInLeague(const CPlayer &player, int leagueId)
{
	int result = SQLITE_OK;
	bool found = false;
	wxString query1 = wxString::Format( "SELECT leagueplayershitter.playerid FROM leagueplayershitter, players WHERE leagueplayershitter.playerid = players.playerid AND leagueplayershitter.id = %d AND players.name = \"%s\"", leagueId, const_cast<CPlayer &>( player ).GetName() );
	wxString query2 = wxString::Format( "SELECT leagueplayerspitcher.playerid FROM leagueplayerspitcher, players WHERE leagueplayerspitcher.playerid = players.playerid AND leagueplayerspitcher.id = %d AND players.name = \"%s\"", leagueId, const_cast<CPlayer &>( player ).GetName() );
	if( ( result = sqlite3_prepare_v2( m_handle, query1, -1, &stmt, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Error retrieving information about the player", sqlite3_errmsg( m_handle ) ) );
	}
	else
	{
		if( sqlite3_step( stmt ) == SQLITE_ROW )
		{
			found = true;
		}
	}
	sqlite3_finalize( stmt );
	if( !found )
	{
		if( ( result = sqlite3_prepare_v2( m_handle, query2, -1, &stmt, 0 ) ) != SQLITE_OK )
			wxMessageBox( wxString::Format( "Error retrieving information about the player", sqlite3_errmsg( m_handle ) ) );
		else
		{
			if( sqlite3_step( stmt ) == SQLITE_ROW )
				found = true;
		}
		sqlite3_finalize( stmt );
	}
	return found;
}

void CDb::StartTransaction()
{
	sqlite3_exec( m_handle, "BEGIN", 0, 0, 0 );
}

void CDb::FinishTransaction(bool result)
{
	wxString query;
	if( result )
		query = "COMMIT";
	else
		query = "ROLLBACK";
	int res = sqlite3_exec( m_handle, query, 0, 0, 0 );
	if( res != SQLITE_OK )
	{
		if( result )
		{
			res = sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
			if( res != SQLITE_OK )
			{
				wxMessageBox( "Serious problem with the database occur. Please try to restart the program and start again." );
			}
		}
		else
		{
			wxMessageBox( "Serious problem with the database occur. Please try to restart the program and start again." );
		}
	}
}

int CDb::AddPlayerToLeague(const CPlayer &player, const int &leagueId)
{
	std::map<wxString,double> scoring = const_cast<CPlayer &>( player ).GetScoring();
	std::vector<wxString> positions = const_cast<CPlayer &>( player ).GetPositions();
	int success = SQLITE_OK;
	wxString query, query1;
	if( player.IsHitter() )
		query = wxString::Format( "INSERT INTO leagueplayershitter VALUES( %d, %d, %d, %.2f, (SELECT scoreid FROM scorehits WHERE scorename = ?), ?, 0, \"1\" );", leagueId, player.GetPlayerId(), player.GetValue(), player.GetCurrentValue() );
	else
		query = wxString::Format( "INSERT INTO leagueplayerspitcher VALUES( %d, %d, %d, %.2f, (SELECT scoreid FROM scorepitch WHERE scorename = ?), ?, 0, \"1\" );", leagueId, player.GetPlayerId(), player.GetValue(), player.GetCurrentValue() );
	query1 = wxString::Format( "INSERT INTO playerpositioninleague VALUES( %d, ?, %d );", player.GetPlayerId(), leagueId );
	success = sqlite3_exec( m_handle, "BEGIN", 0, 0, 0 );
	if( success != SQLITE_OK )
	{
		wxMessageBox( "Error adding player to league: %s", sqlite3_errmsg( m_handle ) );
		return success;
	}
	if( ( success = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Couldn't append player to the league: %s", sqlite3_errmsg( m_handle ) ) );
	}
	else
	{
		for( std::map<wxString,double>::iterator it = scoring.begin(); it != scoring.end(); it++ )
		{
			sqlite3_bind_text( stmt, 0, (*it).first.c_str(), (*it).first.length(), 0 );
			sqlite3_bind_double( stmt, 1, (*it).second );
			success = sqlite3_step( stmt );
			if( success != SQLITE_DONE )
			{
				wxMessageBox( "Couldn't append player to the league: %s", sqlite3_errmsg( m_handle ) );
				break;
			}
			else
				success = SQLITE_OK;
		}
	}
	sqlite3_finalize( stmt );
	if( success == SQLITE_OK )
	{
		if( ( success = sqlite3_prepare_v2( m_handle, query1, -1, &stmt, 0 ) ) != SQLITE_OK )
		{
			wxMessageBox( wxString::Format( "Couldn't append player to the league: %s", sqlite3_errmsg( m_handle ) ) );
		}
		else
		{
			for( std::vector<wxString>::iterator it = positions.begin(); it < positions.end(); it++ )
			{
				sqlite3_bind_text( stmt, 0, (*it).c_str(), (*it).length(), 0 );
				success = sqlite3_step( stmt );
				if( success != SQLITE_DONE )
				{
					wxMessageBox( "Couldn't append player to the league: %s", sqlite3_errmsg( m_handle ) );
					break;
				}
				else
					success = SQLITE_OK;
			}
		}
		sqlite3_finalize( stmt );
		if( success == SQLITE_OK )
		{
			success = sqlite3_exec( m_handle, "COMMIT", 0, 0, 0 );
			if( success != SQLITE_OK )
			{
				wxMessageBox( "Error committing new player to the league: %s", sqlite3_errmsg( m_handle ) );
				success = sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
				if( success != SQLITE_OK )
					wxMessageBox( "Serious problem with the database occurs. Please restart the program and try again" );
				return success;
			}
		}
		else
		{
			success = sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
			if( success != SQLITE_OK )
			{
				wxMessageBox( "Serious problem with the database occurs. Please restart the program and try again" );
			}
		}
	}
	else
	{
		success = sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
		if( success != SQLITE_OK )
		{
			wxMessageBox( "Serious problem with the database occurs. Please restart the program and try again" );
		}
	}
	return success;
}

bool CDb::DeleteAllDraft(const int leagueId)
{
	int result = SQLITE_OK;
	bool res = false;
	wxString query = wxString::Format( "DELETE FROM playerdraft WHERE id = ?;" );
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Error removing all drafts for league: %s", sqlite3_errmsg( m_handle ) ) );
		res = true;
	}
	else
	{
		sqlite3_bind_int( stmt, 1, leagueId );
		sqlite3_step( stmt );
	}
	sqlite3_finalize( stmt );
	return res;
}

int CDb::RemoveNewPlayers(const int leagueId)
{
	int result = SQLITE_OK, res = 0, playerid;
	char *error;
	wxString queryNewHitters = wxString::Format( "SELECT playerid FROM leagueplayershitter WHERE id = %d AND isnew = \"1\";", leagueId );
	wxString queryNewPitchers = wxString::Format( "SELECT playerid FROM leagueplayerspitcher WHERE id = %d AND isnew = \"1\";", leagueId );
	wxString deleteNewPlayerPositions = wxString::Format( "DELETE FROM playerpositioninleague WHERE playerid = ? AND id = %d;", leagueId );
	wxString removeHitterLeague = wxString::Format( "DELETE FROM leagueplayershitter WHERE id = %d AND isnew = \"1\";", leagueId );
	wxString removePitcherLeague = wxString::Format( "DELETE FROM leagueplayerspitcher WHERE id = %d AND isnew = \"1\";", leagueId );
	if( sqlite3_exec( m_handle, "BEGIN", NULL, NULL, &error ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Internal database failure. Can't remove new players records: %s", error ) );
		sqlite3_free( error );
	}
	else
	{
		if( ( result = sqlite3_prepare_v2( m_handle, queryNewHitters, -1, &stmt, 0 ) ) == SQLITE_OK && ( result = sqlite3_prepare_v2( m_handle, deleteNewPlayerPositions, -1, &stmt1, 0 ) ) == SQLITE_OK )
		{
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
				{
					playerid = sqlite3_column_int( stmt, 0 );
					sqlite3_bind_int( stmt1, 0, playerid );
					result = sqlite3_step( stmt1 );
					if( result != SQLITE_DONE )
					{
						wxMessageBox( "Cannot remove new players when resetting the league. Aborting." );
						res = -1;
					}
					sqlite3_finalize( stmt1 );
					if( res == -1 )
						break;
				}
				else if( result != SQLITE_DONE )
				{
					wxMessageBox( "Cannot remove new players when resetting the league. Aborting." );
					res = -1;
				}
				break;
			}
		}
		else
		{
			wxMessageBox( "Cannot remove new players when resetting the league. Aborting" );
			res = -1;
		}
		sqlite3_finalize( stmt );
		sqlite3_finalize( stmt1 );
		if( ( result = sqlite3_prepare_v2( m_handle, removeHitterLeague, -1, &stmt, 0 ) ) != SQLITE_OK )
		{
			wxMessageBox( "Error querying the information for the new hitters." );
			res = -1;
		}
		else
		{
			result = sqlite3_step( stmt );
			if( result != SQLITE_DONE )
			{
				wxMessageBox( wxString::Format( "Error checking player for existence: %s", sqlite3_errmsg( m_handle ) ) );
				res = -1;
			}
			sqlite3_finalize( stmt );
			if( ( result = sqlite3_prepare_v2( m_handle, removePitcherLeague, -1, &stmt, 0 ) ) != SQLITE_OK )
			{
				wxMessageBox( "Error querying the information for the new hitters." );
				res = -1;
			}
			else
			{
				result = sqlite3_step( stmt );
				if( result != SQLITE_DONE )
				{
					wxMessageBox( "Error deleting new hitters in the league" );
					res = -1;
				}
				sqlite3_finalize( stmt );
			}
		}
	}
	if( res == -1 )
	{
		res = sqlite3_exec( m_handle, "ROLLBACK", NULL, NULL, 0 );
		if( res != SQLITE_OK )
			wxMessageBox( "Serious problem with the database occur. Please restart the program and try again." );
	}
	else
		res = sqlite3_exec( m_handle, "COMMIT", NULL, NULL, 0 );
		if( res != SQLITE_OK )
		{
			wxMessageBox( wxString::Format( "Error removing new player from the league: %s", sqlite3_errmsg( m_handle ) ) );
			res = sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
			if( res != SQLITE_OK )
				wxMessageBox( "Serious problem with the database occur. Please restart the program and try again." );
			return -1;
		}
	return res;
}
