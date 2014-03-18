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
	wxString query = "SELECT id FROM Leagues WHERE name = ?;";
	if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
	{
		sqlite3_bind_text( stmt, 1, league, league.length(), 0 );
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
			deleteTrans.push_back( "DELETE FROM ScoreHitsForLeague WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM ScorePitchForLeague WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM TeamsForLeague WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM positionsforleague WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM Owners WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM Targetshits WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM TargetsPitches WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM leaguescorehitter WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM leaguescorepitcher WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM playerpositioninleague WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM playerdraft WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM playersinleague WHERE id = ?;" );
			deleteTrans.push_back( "DELETE FROM Leagues WHERE id = ?;" );
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
						sqlite3_bind_int64( stmt, 1, leagueid );
						result = sqlite3_step( stmt );
						if( result != SQLITE_OK && result != SQLITE_DONE )
						{
							wxMessageBox( wxString::Format( "Error editing the league: %s", sqlite3_errmsg( m_handle ) ) );
							good = false;
						}
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

bool CDb::CreateLeague(CLeagueSettings &league, bool isUpdate, const wxString oldLeague, sqlite3_int64 &leagueId)
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
		insertTrans[0] = "INSERT INTO Leagues(id, name, drafttype, scoringtype, roundvalues, salary, benchplayers) VALUES( NULL, ?, ?, ?, ?, ?, ? );";
	else
	{
		insertTrans[0] = "UPDATE Leagues SET name = ?, drafttype = ?, scoringtype = ?, roundvalues = ?, salary = ?, benchplayers = ? WHERE name = ?;";
		if( ( result = sqlite3_prepare_v2( m_handle, insertTrans[0], -1, &stmt, 0 ) ) == SQLITE_OK )
		{
			wxString name = league.GetLeagueName();
			sqlite3_bind_text( stmt, 1, name, name.length(), 0 );
			sqlite3_bind_int( stmt, 2, league.GetDraftType() + 1 );
			sqlite3_bind_int( stmt, 3, league.GetScoringType() + 1 );
			sqlite3_bind_int( stmt, 4, league.GetRoundingtype() + 1 );
			sqlite3_bind_int( stmt, 5, league.GetSalary() );
			sqlite3_bind_int( stmt, 6, league.IsBenchPlayerIncluded() ? 1 : 0 );
			if( isUpdate )
				sqlite3_bind_text( stmt, 7, oldLeague, oldLeague.length(), 0 );
			result = sqlite3_step( stmt );
			sqlite3_finalize( stmt );
			if( result != SQLITE_DONE )
			{
				wxMessageBox( wxString::Format( "Problem creating the league: %s", sqlite3_errmsg( m_handle ) ) );
				sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
				return false;
			}
		}
		else
		{
			wxMessageBox( wxString::Format( "Problem updating league name: %s", sqlite3_errmsg( m_handle ) ) );
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
	if( ( result = sqlite3_prepare_v2( m_handle, insertTrans[0], -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		wxString name = league.GetLeagueName();
		sqlite3_bind_text( stmt, 1, name, name.length(), 0 );
		sqlite3_bind_int( stmt, 2, league.GetDraftType() + 1 );
		sqlite3_bind_int( stmt, 3, league.GetScoringType() + 1 );
		sqlite3_bind_int( stmt, 4, league.GetRoundingtype() + 1 );
		sqlite3_bind_int( stmt, 5, league.GetSalary() );
		sqlite3_bind_int( stmt, 6, league.IsBenchPlayerIncluded() ? 1 : 0 );
		if( isUpdate )
			sqlite3_bind_text( stmt, 7, oldLeague, oldLeague.length(), 0 );
		result = sqlite3_step( stmt );
		sqlite3_finalize( stmt );
		if( result != SQLITE_DONE )
		{
			wxMessageBox( wxString::Format( "Problem creating the league: %s", sqlite3_errmsg( m_handle ) ) );
			sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
			return false;
		}
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
						if( InsertUpdateTeams( league, leagueId ) != SQLITE_OK )
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
	else
	{
		wxMessageBox( wxString::Format( "Problem creating the league: %s", error ) );
		sqlite3_free( error );
		return false;
	}
}

int CDb::InsertUpdatePlayersPosition(sqlite3_int64 leagueId)
{
	int res = SQLITE_OK;
	wxString query = "INSERT INTO playerpositioninleague SELECT playerposition.playerid, playerposition.positionid, ? FROM playerposition, playersinleague WHERE playerposition.playerid = playersinleague.playerid AND playersinleague.id = ?;";
	if( ( res = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		res = sqlite3_bind_int64( stmt, 1, leagueId );
		res = sqlite3_bind_int64( stmt, 2, leagueId );
		if( res == SQLITE_OK )
		{
			res = sqlite3_step( stmt );
			if( res != SQLITE_DONE )
				wxMessageBox( wxString::Format( "Error inserting positions information for the league: %s", sqlite3_errmsg( m_handle ) ) );
			sqlite3_finalize( stmt );
		}
		else
			wxMessageBox( wxString::Format( "Error inserting positions information for the league: %s", sqlite3_errmsg( m_handle ) ) );
	}
	else
		wxMessageBox( wxString::Format( "Error inserting positions information for the league: %s", sqlite3_errmsg( m_handle ) ) );
	if( res == SQLITE_DONE )
		res = SQLITE_OK;
	return res;
}

int CDb::InsertUpdatePlayers(const std::vector<int> &americans, const std::vector<int> &nationals, sqlite3_int64 leagueid)
{
	int result = SQLITE_OK;
	std::vector<CPlayer> temp_players;
	wxString query = "INSERT INTO playersinleague(id, playerid, ishitter, age, value, currvalue, draft, isnew, deleted, teamid) SELECT ?, players.playerid, players.ishitter, players.age, players.value, players.value, players.isdrafted, \'0\', 0, players.teamid FROM players WHERE isnew = '0' AND ";
	if( americans.size() > 0 )
	{
		if( americans.size() == 1 && americans.at( 0 ) == 0 )
			query += "( teamid >= 1 AND teamid <= 15 )";
		else if( americans.size() >= 1 )
		{
			query += "(";
			for( std::vector<int>::const_iterator it = americans.begin(); it < americans.end(); it++ )
			{
				query += "teamid = ";
				query << (*it);
				if( it == americans.end() - 1 )
					query += ")";
				else
					query += " OR ";
			}
		}
	}
	if( nationals.size() > 0 )
	{
		if( americans.size() > 0 )
			query += " OR ";
		if( nationals.size() == 1 && nationals.at( 0 ) == 16 )
			query += "( teamid >= 17 AND teamid <= 31 )";
		else if( nationals.size() >= 1 )
		{
			query += "(";
			for( std::vector<int>::const_iterator it1 = nationals.begin(); it1 < nationals.end(); it1++ )
			{
				query += "teamid = ";
				query << (*it1);
				if( it1 == nationals.end() - 1 )
					query += ")";
				else
					query += " OR ";
			}
		}
	}
	query += " ORDER BY players.rank;";
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		result = sqlite3_bind_int64( stmt, 1, leagueid );
		result = sqlite3_step( stmt );
		if( result != SQLITE_DONE )
			wxMessageBox( wxString::Format( "Error inserting players for the new league: %s", sqlite3_errmsg( m_handle ) ) );
		sqlite3_finalize( stmt );
	}
	else if( result != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Error inserting players for the new league: %s", sqlite3_errmsg( m_handle ) ) );
	}
	if( result == SQLITE_DONE )
	{
		query = "INSERT INTO leaguescorehitter SELECT ?,playerscorehits.playerid,playerscorehits.scoreid,playerscorehits.value FROM playerscorehits, playersinleague WHERE playerscorehits.playerid = playersinleague.playerid AND playersinleague.id = ?;";
		wxString query1 = "INSERT INTO leaguescorepitcher SELECT ?,playerscorepitches.playerid,playerscorepitches.scoreid,playerscorepitches.value FROM playerscorepitches, playersinleague WHERE playerscorepitches.playerid = playersinleague.playerid AND playersinleague.id = ?;";
		if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
		{
			result = sqlite3_bind_int64( stmt, 1, leagueid );
			result = sqlite3_bind_int64( stmt, 2, leagueid );
		}
		if( ( result = sqlite3_prepare_v2( m_handle, query1, -1, &stmt1, 0 ) ) == SQLITE_OK )
		{
			result = sqlite3_bind_int64( stmt1, 1, leagueid );
			result = sqlite3_bind_int64( stmt1, 2, leagueid );
		}
		if( result == SQLITE_OK )
		{
			result = sqlite3_step( stmt );
			if( result == SQLITE_DONE )
				result = sqlite3_step( stmt1 );
			if( result != SQLITE_DONE )
				wxMessageBox( wxString::Format( "Error inserting players for the new league: %s", sqlite3_errmsg( m_handle ) ) );
			sqlite3_finalize( stmt );
			sqlite3_finalize( stmt1 );
		}
		else
			wxMessageBox( wxString::Format( "Error inserting players for the new league: %s", sqlite3_errmsg( m_handle ) ) );
	}
	if( result == SQLITE_DONE )
		result = SQLITE_OK;
	return result;
}

int CDb::InsertUpdateOwners(const std::vector<std::string> &owners, sqlite3_int64 leagueId)
{
	int result = SQLITE_OK;
	wxString query = "INSERT INTO Owners VALUES( NULL, ?, ? );";
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		sqlite3_bind_int64( stmt, 1, leagueId );
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

int CDb::InsertUpdatePositions(const std::map<wxString,int> &positions, sqlite3_int64 leagueId)
{
	int result = SQLITE_OK;
	wxString query = "INSERT INTO positionsforleague VALUES( (SELECT positionid FROM positions WHERE positionname = ?), ?, ? );";
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		sqlite3_bind_int64( stmt, 2, leagueId );
		for( std::map<wxString,int>::const_iterator it = positions.begin(); it != positions.end() && result == SQLITE_OK; it++ )
		{
			wxString position = (*it).first;
			sqlite3_bind_text( stmt, 1, position, position.length(), 0 );
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

int CDb::InsertUpdateHitScoring(const std::vector<wxString> &hits, const std::vector<wxString> &pitch, sqlite3_int64 leagueId)
{
	int result = SQLITE_OK;
	wxString query = "INSERT INTO scorehitsforleague VALUES( ?, (SELECT scoreid FROM scorehits WHERE scorename = ? ) );";
	unsigned int i;
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		sqlite3_bind_int64( stmt, 1, leagueId );
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
			sqlite3_bind_int64( stmt, 1, leagueId );
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

int CDb::GetLeagueSettings(CLeagueSettings &settings, sqlite3_int64 &leagueId)
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
				leagueId = sqlite3_column_int64( stmt, 0 );
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
			sqlite3_bind_int64( stmt, 1, leagueId );
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
			sqlite3_bind_int64( stmt, 1, leagueId );
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
			sqlite3_bind_int64( stmt, 1, leagueId );
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
			sqlite3_bind_int64( stmt, 1, leagueId );
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
		query = "SELECT teams.name, teams.shortname FROM teams WHERE ";
		std::vector<int> american = settings.GetAmericanLeague();
		std::vector<int> national = settings.GetNationalLeague();
		if( american.size() > 0 )
		{
			if( american.size() == 1 && american.at( 0 ) == 0 )
				query += "( teamid >= 1 AND teamid <= 15 )";
			else if( american.size() >= 1 )
			{
				query += "(";
				for( std::vector<int>::const_iterator it = american.begin(); it < american.end(); it++ )
				{
					query += "teamid = ";
					query << (*it);
					if( it == american.end() - 1 )
						query += ")";
					else
						query += " OR ";
				}
			}
			if( national.size() > 0 )
			{
				if( american.size() > 0 )
					query += " OR ";
				if( national.size() == 1 && national.at( 0 ) == 16 )
					query += "( teamid >= 17 AND teamid <= 31 )";
				else if( national.size() >= 1 )
				{
					query += "(";
					for( std::vector<int>::const_iterator it1 = national.begin(); it1 < national.end(); it1++ )
					{
						query += "teamid = ";
						query << (*it1);
						if( it1 == national.end() - 1 )
							query += ")";
						else
							query += " OR ";
					}
				}
			}
		}
		query += " ORDER BY shortname ASC;";
		if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
		{
			while( true )
			{
				result = sqlite3_step( stmt );
				if( result == SQLITE_ROW )
					settings.AddTeamAbbreviation( sqlite3_column_text( stmt, 0 ), sqlite3_column_text( stmt, 1 ) );
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
		}
		if( result != SQLITE_OK )
			return result;
		query = "SELECT scorehits.scorename FROM scorehits,scorehitsforleague WHERE scorehits.scoreid = scorehitsforleague.scoreid AND scorehitsforleague.id = ? ORDER BY scorehits.scoreid;";
		if( sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) == SQLITE_OK )
		{
			sqlite3_bind_int64( stmt, 1, leagueId );
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
			sqlite3_bind_int64( stmt, 1, leagueId );
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
			sqlite3_bind_int64( stmt, 1, leagueId );
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
			sqlite3_bind_int64( stmt, 1, leagueId );
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

int CDb::InsertUpdateTeams(CLeagueSettings &league, sqlite3_int64 leagueId)
{
	int result = SQLITE_OK;
	std::vector<int> americans = league.GetAmericanLeague();
	std::vector<int> nationals = league.GetNationalLeague();
	unsigned int i;
	wxString query = "INSERT INTO teamsforleague VALUES( ?, ? );";
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
	{
		sqlite3_bind_int64( stmt, 1, leagueId );
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
		wxMessageBox( wxString::Format( "Error preparing the query for inserting teams: %s", sqlite3_errmsg( m_handle ) ) );
	if( result == SQLITE_OK )
	{
		query = "INSERT INTO teamsforleague VALUES( ?, ? );";
		if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
		{
			sqlite3_bind_int64( stmt, 1, leagueId );
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
		else
			wxMessageBox( wxString::Format( "Error preparing the query for inserting teams: %s", sqlite3_errmsg( m_handle ) ) );
		if( result == SQLITE_OK )
		{
			query = "SELECT teams.name, teams.shortname FROM teams WHERE ";
			std::vector<int> american = league.GetAmericanLeague();
			std::vector<int> national = league.GetNationalLeague();
			if( american.size() > 0 )
			{
				if( american.size() == 1 && american.at( 0 ) == 0 )
					query += "( teamid >= 1 AND teamid <= 15 )";
				else if( american.size() >= 1 )
				{
					query += "(";
					for( std::vector<int>::const_iterator it = american.begin(); it < american.end(); it++ )
					{
						query += "teamid = ";
						query << (*it);
						if( it == american.end() - 1 )
							query += ")";
						else
							query += " OR ";
					}
				}
			}
			if( national.size() > 0 )
			{
				if( american.size() > 0 )
					query += " OR ";
				if( national.size() == 1 && national.at( 0 ) == 16 )
					query += "( teamid >= 17 AND teamid <= 31 )";
				else if( national.size() >= 1 )
				{
					query += "(";
					for( std::vector<int>::const_iterator it1 = national.begin(); it1 < national.end(); it1++ )
					{
						query += "teamid = ";
						query << (*it1);
						if( it1 == national.end() - 1 )
							query += ")";
						else
							query += " OR ";
					}
				}
			}
			query += " ORDER BY shortname ASC;";
			if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
			{
				while( true )
				{
					result = sqlite3_step( stmt );
					if( result == SQLITE_ROW )
						league.AddTeamAbbreviation( sqlite3_column_text( stmt, 0 ), sqlite3_column_text( stmt, 1 ) );
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
				if( result != SQLITE_OK )
					wxMessageBox( wxString::Format( "Error retrieving team names for the league: %s", sqlite3_errmsg( m_handle ) ) );
				sqlite3_finalize( stmt );
			}
			else
				wxMessageBox( wxString::Format( "Error retrieving team names for the league: %s", sqlite3_errmsg( m_handle ) ) );
		}
	}
	return result;
}

sqlite3_int64 CDb::GetLeagueId(bool isUpdate, const wxString &name)
{
	char *error = NULL;
	sqlite3_int64 leagueid = -1;
	int result;
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

int CDb::InsertPlayers(const std::vector<std::vector<wxString> > &hitters, const std::vector<std::vector<wxString> > &pitchers)
{
	sqlite3_int64 playerid = -1;
	int result;
	std::vector<std::vector<wxString> > hits = const_cast<std::vector<std::vector<wxString> > &>( hitters );
	std::vector<std::vector<wxString> > pits = const_cast<std::vector<std::vector<wxString> > &>( pitchers );
	if( sqlite3_exec( m_handle, "BEGIN", 0, 0, 0 ) != SQLITE_OK )
	{
		wxMessageBox( "Database error: Can't insert players info into the database" );
		return -1;
	}
	for( std::vector<std::vector<wxString> >::iterator it_out = hits.begin(); it_out < hits.end(); it_out++ )
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
					playerid = sqlite3_last_insert_rowid( m_handle );
			}
			else
			{
				wxString temp = *it_in;
				if( ( result = sqlite3_prepare_v2( m_handle, temp, -1, &stmt, 0 ) ) != SQLITE_OK )
				{
					wxMessageBox( wxString::Format( "Error inserting hitters: %s", sqlite3_errmsg( m_handle ) ) );
					sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
					return -1;
				}
				else
				{
					sqlite3_bind_int64( stmt, 1, playerid );
					result = sqlite3_step( stmt );
					sqlite3_finalize( stmt );
					if( result != SQLITE_DONE )
					{
						wxMessageBox( wxString::Format( "Error inserting hitters data: %s", sqlite3_errmsg( m_handle ) ) );
						sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
						return -1;
					}
				}
			}
		}
	}
	for( std::vector<std::vector<wxString> >::iterator it_out = pits.begin(); it_out < pits.end(); it_out++ )
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
					playerid = sqlite3_last_insert_rowid( m_handle );
			}
			else
			{
				wxString temp = *it_in;
				if( ( result = sqlite3_prepare_v2( m_handle, temp, -1, &stmt, 0 ) ) != SQLITE_OK )
				{
					wxMessageBox( wxString::Format( "Error inserting pitchers: %s", sqlite3_errmsg( m_handle ) ) );
					sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
					return -1;
				}
				else
				{
					sqlite3_bind_int64( stmt, 1, playerid );
					result = sqlite3_step( stmt );
					sqlite3_finalize( stmt );
					if( result != SQLITE_DONE )
					{
						wxMessageBox( wxString::Format( "Error inserting pitchers data: %s", sqlite3_errmsg( m_handle ) ) );
						sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 );
						return -1;
					}
				}
			}
		}
	}
	if( sqlite3_exec( m_handle, "COMMIT", 0, 0, 0 ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Error committing player's data: %s", sqlite3_errmsg( m_handle ) ) );
		if( sqlite3_exec( m_handle, "ROLLBACK", 0, 0, 0 ) != SQLITE_OK )
			wxMessageBox( "Serious problem with the database occur. Please restart and try again." );
		return -1;
	}
	return 0;
}

int CDb::LoadLeagueData(CLeagueSettings *league, sqlite3_int64 &leagueId)
{
	int result;
	return ( result = GetLeagueSettings( *league, leagueId ) );
}

int CDb::GetPlayersForLeague(CLeagueData &m_data, const int &leagueId, int &numPlayers, std::vector<CPlayer> &draftResult, int &zeroRanked)
{
	sqlite3_stmt *stm1 = NULL, *stm2 = NULL, *stm3 = NULL, *stm4 = NULL, *stm5 = NULL;
	bool error = false, deleted;
	int result = SQLITE_OK;
	sqlite3_int64 playerid;
	char ishitter;
	std::map<wxString,double> scoring, originalScoring;
	std::vector<wxString> positions;
	wxString query = "SELECT playersinleague.playerid, players.name, playersinleague.age, playersinleague.value, teams.name, teams.shortname, playersinleague.currvalue, players.notes, playersinleague.draft, players.value, playersinleague.isnew, playersinleague.current_rank, playersinleague.original_rank, playersinleague.ishitter, playersinleague.deleted FROM players, playersinleague, teams WHERE players.playerid = playersinleague.playerid AND teams.teamid = playersinleague.teamid AND playersinleague.id = ? ORDER BY playersinleague.currvalue DESC, playersinleague.current_rank ASC;";
	wxString score = "SELECT playersinleague.playerid,scorehits.scorename,leaguescorehitter.value FROM playersinleague,scorehits,leaguescorehitter WHERE leaguescorehitter.scoreid = scorehits.scoreid AND playersinleague.playerid = leaguescorehitter.playerid AND playersinleague.playerid = ? AND playersinleague.id = ? UNION ALL SELECT playersinleague.playerid,scorepitch.scorename,leaguescorepitcher.value FROM playersinleague,scorepitch,leaguescorepitcher WHERE playersinleague.playerid = leaguescorepitcher.playerid AND leaguescorepitcher.scoreid = scorepitch.scoreid AND playersinleague.playerid = ? AND playersinleague.id = ?;";
	wxString pos = "SELECT positions.positionname FROM playerpositioninleague, positions WHERE playerpositioninleague.positionid = positions.positionid AND playerpositioninleague.playerid = ? AND playerpositioninleague.id = ?;";
	wxString draft = "SELECT ownername, draftprice, draftorder, draftposition FROM playerdraft, owners WHERE playerdraft.id = ? AND playerid = ? AND owners.ownerid = playerdraft.ownerid ORDER BY draftorder;";
	wxString originalScoreHits = "SELECT scorehits.scorename, playerscorehits.value FROM scorehits, playerscorehits WHERE (scorehits.scoreid = playerscorehits.scoreid) AND playerscorehits.playerid = ?;";
	wxString originalScorePitch = "SELECT scorepitch.scorename, playerscorepitches.value FROM scorepitch, playerscorepitches WHERE scorepitch.scoreid = playerscorepitches.scoreid AND playerscorepitches.playerid = ?;";
	if( ( result = sqlite3_prepare_v2( m_handle, originalScorePitch, -1, &stm5, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Could not read score information for the player: %s", sqlite3_errmsg( m_handle ) ) );
		error = true;
	}
	if( !error && ( result = sqlite3_prepare_v2( m_handle, originalScoreHits, -1, &stm4, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Could not read score information for player: %s", sqlite3_errmsg( m_handle ) ) );
		error = true;
	}
	if( !error && ( result = sqlite3_prepare_v2( m_handle, draft, -1, &stm3, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Could not read draft information for player: %s", sqlite3_errmsg( m_handle ) ) );
		error = true;
	}
	else if( !error )
		sqlite3_bind_int( stm3, 1, leagueId );
	if( !error && ( result = sqlite3_prepare_v2( m_handle, pos, -1, &stm2, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Could not prepare SELECT query for getting players information: %s" ), sqlite3_errmsg( m_handle ) );
		error = true;
	}
	else if( !error )
		sqlite3_bind_int( stm2, 2, leagueId );
	if( !error && ( result = sqlite3_prepare_v2( m_handle, score, -1, &stm1, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Could not prepare the query to retrieve the players scoring information for league: %s", sqlite3_errmsg( m_handle ) ) );
		error = true;
	}
	else if( !error )
	{
		sqlite3_bind_int( stm1, 2, leagueId );
		sqlite3_bind_int( stm1, 4, leagueId );
	}
	if( !error && ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
	{
		wxMessageBox( wxString::Format( "Could not prepare the query to retrieve players information for league: %s", sqlite3_errmsg( m_handle ) ) );
		error = true;
	}
	if( error )
	{
		sqlite3_finalize( stmt );
		sqlite3_finalize( stm1 );
		sqlite3_finalize( stm2 );
		sqlite3_finalize( stm3 );
		sqlite3_finalize( stm4 );
		sqlite3_finalize( stm5 );
		return result;
	}
	sqlite3_bind_int( stmt, 1, leagueId );
	while( true )
	{
		result = sqlite3_step( stmt );
		if( result == SQLITE_ROW )
		{
			playerid = sqlite3_column_int( stmt, 0 );
			ishitter = sqlite3_column_text( stmt, 13 )[0];
			deleted = sqlite3_column_int( stmt, 14 ) == 1 ? true : false;
			sqlite3_bind_int64( stm1, 1, playerid );
			sqlite3_bind_int64( stm1, 3, playerid );
			while( true )
			{
				result = sqlite3_step( stm1 );
				if( result == SQLITE_ROW )
					scoring[sqlite3_column_text( stm1, 1 )] = sqlite3_column_double( stm1, 2 );
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
				sqlite3_finalize( stm5 );
				sqlite3_finalize( stm4 );
				sqlite3_finalize( stm3 );
				sqlite3_finalize( stm2 );
				sqlite3_finalize( stm1 );
				sqlite3_finalize( stmt );
				return result;
			}
			sqlite3_bind_int64( stm2, 1, playerid );
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
				sqlite3_finalize( stm5 );
				sqlite3_finalize( stm4 );
				sqlite3_finalize( stm3 );
				sqlite3_finalize( stm2 );
				sqlite3_finalize( stm1 );
				sqlite3_finalize( stmt );
				return result;
			}
			while( true )
			{
				if( ishitter == '1' )
				{
					sqlite3_bind_int64( stm4, 1, playerid );
					result = sqlite3_step( stm4 );
				}
				else
				{
					sqlite3_bind_int64( stm5, 1, playerid );
					result = sqlite3_step( stm4 );
				}
				if( result == SQLITE_ROW )
					if( ishitter == '1' )
						originalScoring[sqlite3_column_text( stm4, 0 )] = sqlite3_column_double( stm4, 1 );
					else
						originalScoring[sqlite3_column_text( stm5, 0 )] = sqlite3_column_double( stm5, 1 );
				else if( result == SQLITE_DONE )
				{
					result = SQLITE_OK;
					break;
				}
				else
				{
					wxMessageBox( wxString::Format( "Error retrieving score information for the player: %s", sqlite3_errmsg( m_handle ) ) );
					break;
				}
			}
			sqlite3_reset( stm4 );
			sqlite3_reset( stm5 );
			if( result != SQLITE_OK )
			{
				sqlite3_finalize( stm5 );
				sqlite3_finalize( stm4 );
				sqlite3_finalize( stm3 );
				sqlite3_finalize( stm2 );
				sqlite3_finalize( stm1 );
				sqlite3_finalize( stmt );
				return result;
			}
			if( result == SQLITE_OK )
			{
				CPlayer player( sqlite3_column_int( stmt, 0 ), sqlite3_column_text( stmt, 1 ), positions, sqlite3_column_int( stmt, 2 ), sqlite3_column_double( stmt, 3 ), sqlite3_column_text( stmt, 4 ), sqlite3_column_text( stmt, 5 ), scoring, ishitter == '1' ? true : false, sqlite3_column_double( stmt, 6 ), sqlite3_column_text( stmt, 7 ), sqlite3_column_int( stmt, 9 ), deleted );
				player.SetRange( sqlite3_column_int( stmt, 11 ) );
				player.SetOriginalRange( sqlite3_column_int( stmt, 12 ) );
				player.SetOriginalScore( originalScoring );
				player.SetNewPlayer( wxAtoi( sqlite3_column_text( stmt, 10 ) ) == 1 ? true : false );
				if( sqlite3_column_int( stmt, 8 ) == 1 )
				{
					player.DraftPlayer( true );
					sqlite3_bind_int64( stm3, 2, playerid );
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
						sqlite3_finalize( stm5 );
						sqlite3_finalize( stm4 );
						sqlite3_finalize( stm3 );
						sqlite3_finalize( stm2 );
						sqlite3_finalize( stm1 );
						sqlite3_finalize( stmt );
						return result;
					}
				}
				double value = sqlite3_column_double( stmt, 6 );
				if( value > 0.0 && !deleted )
					numPlayers++;
				if( value == 0.0 && !deleted )
					zeroRanked++;
				m_data.m_players->push_back( player );
				if( player.IsPlayerDrafted() )
					draftResult.push_back( player );
				originalScoring.clear();
				scoring.clear();
				positions.clear();
			}
		}
		else if( result == SQLITE_DONE )
		{
			result = SQLITE_OK;
			break;
		}
		else
		{
			wxMessageBox( wxString::Format( "Error retrieving information about the player: %s", sqlite3_errmsg( m_handle ) ) );
			break;
		}
	}
	sqlite3_finalize( stmt );
	sqlite3_finalize( stm1 );
	sqlite3_finalize( stm2 );
	sqlite3_finalize( stm3 );
	sqlite3_finalize( stm4 );
	sqlite3_finalize( stm5 );
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

int CDb::AddNewPlayer(CPlayer &player, const sqlite3_int64 leagueId, bool skipCheck, wxString &errorMessage)
{
	int result = SQLITE_OK;
	sqlite3_stmt *stm1;
	sqlite3_int64 playerid = 0;
	bool errorCheck = false, playerExistInPool = false;
	if( !skipCheck )
	{
		bool found = IsPlayerExist( const_cast<CPlayer &>( player ), errorCheck, errorMessage );
		if( errorCheck )
			return 1;
		if( found )
		{
			playerExistInPool = true;
			bool found = IsPlayerInLeague( player, leagueId, errorCheck, errorMessage );
			if( errorCheck )
				return 1;
			if( found )
				return false;
		}
	}
	wxString query1, query2, query3, query4;
	wxString query0 = "UPDATE players SET rank = rank + 1 WHERE players.rank > ?;";
	if( !playerExistInPool )
		query1 = "INSERT INTO players VALUES( NULL, ?, ?, ?, ?, (SELECT teamid FROM teams WHERE shortname = ?), ?, ?, ?, ?, ? );";
	else
		query1 = "SELECT playerid FROM players WHERE name = ? AND teamid = (SELECT teamid FROM teams WHERE shortname = ?);";
	wxString query;
	if( ( result = sqlite3_prepare_v2( m_handle, query0, -1, &stmt, 0 ) ) != SQLITE_OK )
	{
		errorMessage = wxString::Format( "Error inserting new player in the player pool: %s", sqlite3_errmsg( m_handle ) );
		return true;
	}
	else
	{
		sqlite3_bind_int( stmt, 1, player.GetRange() );
		result = sqlite3_step( stmt );
		sqlite3_finalize( stmt );
		if( result != SQLITE_DONE )
		{
			errorMessage = wxString::Format( "Error inserting new player in the player pool: %s", sqlite3_errmsg( m_handle ) );
			return true;
		}
		else
		{
			if( ( result = sqlite3_prepare_v2( m_handle, query1, -1, &stmt, 0 ) ) != SQLITE_OK )
			{
				errorMessage = wxString::Format( "Error inserting new player in the player pool: %s", sqlite3_errmsg( m_handle ) );
				return true;
			}
			else
			{
				if( playerExistInPool )
				{
					wxString name = player.GetName();
					wxString teamName = player.GetAbbeviatedTeamName();
					sqlite3_bind_text( stmt, 1, name, name.length(), 0 );
					sqlite3_bind_text( stmt, 2, teamName, teamName.length(), 0 );
				}
				else
				{
					wxString name = player.GetName();
					wxString teamName = player.GetAbbeviatedTeamName();
					wxString isHitter = player.IsHitter() ? "1" : "0";
					wxString isDrafted = player.IsPlayerDrafted() ? "1" : "0";
					wxString isNew = "1";
					wxString notes = "";
					sqlite3_bind_text( stmt, 1, name, name.length(), 0 );
					sqlite3_bind_text( stmt, 2, isHitter, isHitter.length(), 0 );
					sqlite3_bind_int( stmt, 3, player.GetAge() );
					sqlite3_bind_double( stmt, 4, player.GetValue() );
					sqlite3_bind_text( stmt, 5, teamName, teamName.length(), 0 );
					sqlite3_bind_double( stmt, 6, player.GetCurrentValue() );
					sqlite3_bind_text( stmt, 7, notes, notes.length(), 0 );
					sqlite3_bind_text( stmt, 8, isDrafted, isDrafted.length(), 0 );
					sqlite3_bind_text( stmt, 9, isNew, isNew.length(), 0 );
					sqlite3_bind_int( stmt, 10, player.GetRange() );
				}
				result = sqlite3_step( stmt );
				if( playerExistInPool && result == SQLITE_ROW )
					playerid = sqlite3_column_int( stmt, 0 );
				else if( !playerExistInPool && result == SQLITE_DONE )
					playerid = sqlite3_last_insert_rowid( m_handle );
				else
				{
					errorMessage = wxString::Format( "Error inserting new player in the player pool: %s", sqlite3_errmsg( m_handle ) );
					sqlite3_finalize( stmt );
					return true;
				}
				sqlite3_finalize( stmt );
				query0 = "INSERT INTO playersinleague VALUES(?,?,?,?,?,?,?,?,?,?,?,(SELECT teamid FROM teams WHERE name = ?),?);";
				if( ( result = sqlite3_prepare_v2( m_handle, query0, -1, &stmt, 0 ) ) != SQLITE_OK )
				{
					errorMessage = wxString::Format( "Error inserting new player for the league: %s", sqlite3_errmsg( m_handle ) );
					return true;
				}
				else
				{
					wxString team = player.GetTeam();
					wxString notes = player.GetNotes();
					sqlite3_bind_int64( stmt, 1, leagueId );
					sqlite3_bind_int64( stmt, 2, playerid );
					sqlite3_bind_text( stmt, 3, player.IsHitter() ? "1" : "0", 1, 0 );
					sqlite3_bind_int( stmt, 4, player.GetAge() );
					sqlite3_bind_double( stmt, 5, player.GetValue() );
					sqlite3_bind_double( stmt, 6, player.GetCurrentValue() );
					sqlite3_bind_int( stmt, 7, player.IsPlayerDrafted() ? 1 : 0 );
					sqlite3_bind_text( stmt, 8, "1", 1, 0 );
					sqlite3_bind_int( stmt, 9, player.GetRange() );
					sqlite3_bind_int( stmt, 10, player.GetOriginalRange() );
					sqlite3_bind_int( stmt, 11, player.IsPlayerDeleted() );
					sqlite3_bind_text( stmt, 12, team, team.length(), 0 );
					sqlite3_bind_text( stmt, 13, notes, notes.length(), 0 );
					result = sqlite3_step( stmt );
					sqlite3_finalize( stmt );
					if( result != SQLITE_DONE )
					{
						errorMessage = wxString::Format( "Error inserting new player for the league: %s", sqlite3_errmsg( m_handle ) );
						return true;
					}
					else
					{
						query = "INSERT INTO playerposition VALUES( ?, ( SELECT positionid FROM positions WHERE positionname = ? ) );";
						query2 = "INSERT INTO playerpositioninleague VALUES( ?, (SELECT positionid FROM positions WHERE positionname = ?), ? )";
						if( !playerExistInPool && ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) == SQLITE_OK )
						{
							sqlite3_bind_int64( stmt, 1, playerid );
						}
						else if( !playerExistInPool )
						{
							errorMessage = wxString::Format( "Error inserting new player position: %s", sqlite3_errmsg( m_handle ) );
							return true;
						}
						if( ( result = sqlite3_prepare_v2( m_handle, query2, -1, &stm1, 0 ) ) == SQLITE_OK )
						{
							sqlite3_bind_int64( stm1, 1, playerid );
							sqlite3_bind_int64( stm1, 3, leagueId );
						}
						else
						{
							errorMessage = wxString::Format( "Error inserting new player position: %s", sqlite3_errmsg( m_handle ) );
							return true;
						}
						result = SQLITE_DONE;
						for( std::vector<wxString>::iterator it_pos = const_cast<CPlayer &>( player ).GetPositions().begin(); it_pos < const_cast<CPlayer &>( player ).GetPositions().end() && result == SQLITE_DONE; it_pos++ )
						{
							wxString position = (*it_pos);
							if( !playerExistInPool )
								sqlite3_bind_text( stmt, 2, position, position.length(), 0 );
							sqlite3_bind_text( stm1, 2, position, position.length(), 0 );
							if( !playerExistInPool )
							{
								result = sqlite3_step( stmt );
								if( result != SQLITE_DONE )
									break;
							}
							result = sqlite3_step( stm1 );
							if( result != SQLITE_DONE )
								break;
							if( !playerExistInPool )
								sqlite3_reset( stmt );
							sqlite3_reset( stm1 );
						}
						if( !playerExistInPool )
							sqlite3_finalize( stmt );
						sqlite3_finalize( stm1 );
						if( result != SQLITE_DONE )
						{
							errorMessage = wxString::Format( "Error inserting new player position: %s", sqlite3_errmsg( m_handle ) );
							return true;
						}
						if( player.IsHitter() )
						{
							query = "INSERT INTO playerscorehits VALUES(?,(SELECT scoreid FROM scorehits WHERE scorename = ?),?);";
							query1 = "INSERT INTO leaguescorehitter VALUES(?,?,(SELECT scoreid FROM scorehits WHERE scorename = ?),?);";
						}
						else
						{
							query = "INSERT INTO playerscorepitches VALUES(?,(SELECT scoreid FROM scorepitch WHERE scorename = ?),?);";
							query1 = "INSERT INTO leaguescorepitcher VALUES(?,?,(SELECT scoreid FROM scorepitch WHERE scorename = ?),?);";
						}
						if( !playerExistInPool && ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
						{
							errorMessage = wxString::Format( "Error inserting the score for the new player: %s", sqlite3_errmsg( m_handle ) );
							return true;
						}
						if( ( result = sqlite3_prepare_v2( m_handle, query1, -1, &stm1, 0 ) ) != SQLITE_OK )
						{
							errorMessage = wxString::Format( "Error inserting the score for the new player: %s", sqlite3_errmsg( m_handle ) );
							return true;
						}
						else
						{
							if( !playerExistInPool )
								sqlite3_bind_int64( stmt, 1, playerid );
							sqlite3_bind_int64( stm1, 1, leagueId );
							sqlite3_bind_int64( stm1, 2, playerid );
							for( std::map<wxString,double>::iterator it = player.GetScoring().begin(); it != player.GetScoring().end(); it++ )
							{
								wxString score = (*it).first;
								if( !playerExistInPool )
								{
									sqlite3_bind_text( stmt, 2, score, score.length(), 0 );
									sqlite3_bind_double( stmt, 3, (*it).second );
								}
								sqlite3_bind_text( stm1, 3, score, score.length(), 0 );
								sqlite3_bind_double( stm1, 4, (*it).second );
								if( !playerExistInPool )
								{
									result = sqlite3_step( stmt );
									sqlite3_reset( stmt );
									if( result != SQLITE_DONE )
									{
										errorMessage = wxString::Format( "Error inserting scores for the new player: %s", sqlite3_errmsg( m_handle ) );
										sqlite3_finalize( stmt );
										return true;
									}
								}
								result = sqlite3_step( stm1 );
								sqlite3_reset( stm1 );
								if( result != SQLITE_DONE )
								{
									errorMessage = wxString::Format( "Error inserting scores for the new player: %s", sqlite3_errmsg( m_handle ) );
									return true;
								}
							}
							if( !playerExistInPool )
								sqlite3_finalize( stmt );
							sqlite3_finalize( stm1 );
						}
					}
				}
			}
		}
	}
	return false;
}

bool CDb::UpdatePlayer(const CPlayer &player, const sqlite3_int64 leagueId, wxString &errorMessage)
{
	int result = SQLITE_OK;
	wxString query = "UPDATE playersinleague SET age = ?, teamid = (SELECT teamid FROM teams WHERE teams.shortname = ?), notes = ?, current_rank = ?, value = ?, currvalue = ?, deleted = ?, draft = ? WHERE playerid = ? AND id = ?;";
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
	{
		errorMessage = wxString::Format( "Error updating player: %s", sqlite3_errmsg( m_handle ) );
		return true;
	}
	else
	{
		wxString team = const_cast<CPlayer &>( player ).GetAbbeviatedTeamName();
		wxString notes = const_cast<CPlayer &>( player ).GetNotes();
		sqlite3_bind_int( stmt, 1, player.GetAge() );
		sqlite3_bind_text( stmt, 2, team, team.length() , 0 );
		sqlite3_bind_text( stmt, 3, notes, notes.length(), 0 );
		sqlite3_bind_int( stmt, 4, const_cast<CPlayer &>( player ).GetRange() );
		sqlite3_bind_double( stmt, 5, player.GetValue() );
		sqlite3_bind_double( stmt, 6, player.GetCurrentValue() );
		sqlite3_bind_int( stmt, 7, player.IsPlayerDeleted() ? 1 : 0 );
		sqlite3_bind_int( stmt, 8, player.IsPlayerDrafted() ? 1 : 0 );
		sqlite3_bind_int64( stmt, 9, player.GetPlayerId() );
		sqlite3_bind_int64( stmt, 10, leagueId );
		result = sqlite3_step( stmt );
	}
	sqlite3_finalize( stmt );
	if( result == SQLITE_DONE )
	{
		query = "DELETE FROM playerpositioninleague WHERE id = ? AND playerid = ?";
		if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
		{
			errorMessage = wxString::Format( "Error updating player: %s", sqlite3_errmsg( m_handle ) );
			return true;
		}
		else
		{
			sqlite3_bind_int64( stmt, 1, leagueId );
			sqlite3_bind_int64( stmt, 2, player.GetPlayerId() );
			result = sqlite3_step( stmt );
		}
		sqlite3_finalize( stmt );
		if( result != SQLITE_DONE )
		{
			errorMessage = wxString::Format( "Error updating player: %s", sqlite3_errmsg( m_handle ) );
			return true;
		}
		else
		{
			query = "INSERT INTO playerpositioninleague VALUES( ?, (SELECT positionid FROM positions WHERE positionname = ?), ? );";
			if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
			{
				errorMessage = wxString::Format( "Error updating player positions: %s", sqlite3_errmsg( m_handle ) );
				return true;
			}
			else
			{
				sqlite3_bind_int64( stmt, 1, player.GetPlayerId() );
				sqlite3_bind_int64( stmt, 3, leagueId );
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
						query = "UPDATE leaguescorehitter SET value = ? WHERE id = ? AND playerid = ? AND scoreid = (SELECT scoreid FROM scorehits WHERE scorename = ?);"; 
					else
						query = "UPDATE leaguescorepitcher SET value = ? WHERE id = ? AND playerid = ? AND scoreid = (SELECT scoreid FROM scorepitch WHERE scorename = ?);";
					if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
					{
						errorMessage = wxString::Format( "Error preparing query for inserting score information: %s", sqlite3_errmsg( m_handle ) );
						return true;
					}
					else
					{
						sqlite3_bind_int64( stmt, 2, leagueId );
						sqlite3_bind_int64( stmt, 3, player.GetPlayerId() );
						for( std::map<wxString,double>::iterator it = const_cast<CPlayer &>( player ).GetScoring().begin(); it != const_cast<CPlayer &>( player ).GetScoring().end(); it++ )
						{
							wxString score = (*it).first;
							sqlite3_bind_text( stmt, 4, score, score.length(), 0 );
							sqlite3_bind_double( stmt, 1, (*it).second );
							result = sqlite3_step( stmt );
							sqlite3_reset( stmt );
						}
						sqlite3_finalize( stmt );
						if( result == SQLITE_DONE && player.IsPlayerDrafted() )
						{
							query = "INSERT INTO playerdraft VALUES( ?, ?, ( SELECT ownerid FROM owners WHERE ownername = ? AND id = ?2 ), ?, ?, ? );";
							if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
							{
								errorMessage = wxString::Format( "Error updating players: %s", sqlite3_errmsg( m_handle ) );
								return true;
							}
							else
							{
								wxString owner = const_cast<CPlayer &>( player ).GetOwner();
								wxString position = const_cast<CPlayer &>( player ).GetDraftedPosition();
								sqlite3_bind_int64( stmt, 1, player.GetPlayerId() );
								sqlite3_bind_int64( stmt, 2, leagueId );
								sqlite3_bind_text( stmt, 3, owner, owner.length(), 0 );
								sqlite3_bind_int( stmt, 4, player.GetAmountPaid() );
								sqlite3_bind_int( stmt, 5, player.GetDraftOrder() );
								sqlite3_bind_text( stmt, 6, position, position.length(), 0 );
								result = sqlite3_step( stmt );
							}
							sqlite3_finalize( stmt );
							if( result != SQLITE_DONE )
							{
								errorMessage = wxString::Format( "Error saving player information: %s", sqlite3_errmsg( m_handle ) );
								return true;
							}
						}
					}
				}
				else
				{
					errorMessage = wxString::Format( "Error updating players positions: %s", sqlite3_errmsg( m_handle ) );
					return true;
				}
			}
		}
	}
	else
	{
		errorMessage = wxString::Format( "Error updating player information: %s", sqlite3_errmsg( m_handle ) );
		return true;
	}
	if( result == SQLITE_DONE )
		return false;
	else
		return true;
}

bool CDb::IsPlayerExist(CPlayer &player, bool &errorCheck, wxString &errorMessage)
{
	int result = SQLITE_OK;
	bool found = false;
	wxString query = "SELECT players.playerid FROM players WHERE players.teamid = (SELECT teamid FROM teams WHERE teams.name = ?) AND players.age = ? AND players.name = ?;";
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
	{
		errorCheck = true;
		errorMessage = sqlite3_errmsg( m_handle );
		return !result;
	}
	else
	{
		wxString name = player.GetName(), teamName = player.GetTeam();
		sqlite3_bind_text( stmt, 1, teamName, teamName.length(), 0 );
		sqlite3_bind_int( stmt, 2, player.GetAge() );
		sqlite3_bind_text( stmt, 3, name, name.length(), 0 );
		result = sqlite3_step( stmt );
		if( result == SQLITE_ROW )
		{
			found = true;
			player.SetNewPlayerId( sqlite3_column_int( stmt, 0 ) );
		}
		sqlite3_finalize( stmt );
		if( result != SQLITE_DONE && result != SQLITE_ROW )
		{
			errorCheck = true;
			errorMessage = sqlite3_errmsg( m_handle );
			return !result;
		}
	}
	return found;
}

bool CDb::IsPlayerInLeague(const CPlayer &player, sqlite3_int64 leagueId, bool &errorCheck, wxString &errorMessage)
{
	int result = SQLITE_OK;
	bool found = false;
	wxString query = "SELECT playersinleague.playerid FROM playersinleague WHERE playersinleague.playerid = ? AND playersinleague.id = ?;";
	if( ( result = sqlite3_prepare_v2( m_handle, query, -1, &stmt, 0 ) ) != SQLITE_OK )
	{
		errorCheck = true;
		errorMessage = wxString::Format( "Error retrieving information about the player", sqlite3_errmsg( m_handle ) );
		return !result;
	}
	else
	{
		sqlite3_bind_int64( stmt, 1, player.GetPlayerId() );
		sqlite3_bind_int64( stmt, 2, leagueId );
		if( ( result = sqlite3_step( stmt ) ) == SQLITE_ROW )
		{
			found = true;
		}
		sqlite3_finalize( stmt );
		if( result != SQLITE_DONE && result != SQLITE_ROW )
		{
			errorCheck = true;
			errorMessage = wxString::Format( "Error retrieving the player for the league: %s", sqlite3_errmsg( m_handle ) );
			return !result;
		}
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

bool CDb::DeleteAllDraft(const sqlite3_int64 leagueId)
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
		sqlite3_bind_int64( stmt, 1, leagueId );
		result = sqlite3_step( stmt );
		if( result != SQLITE_DONE )
		{
			wxMessageBox( wxString::Format( "Error updating drafted players: %s", sqlite3_errmsg( m_handle ) ) );
			res = true;
		}
		else
			res = false;
	}
	sqlite3_finalize( stmt );
	return res;
}

int CDb::RemoveNewPlayers(const int leagueId)
{
	int result = SQLITE_OK, res = 0;
	sqlite3_int64 playerid;
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
					sqlite3_bind_int64( stmt1, 0, playerid );
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
