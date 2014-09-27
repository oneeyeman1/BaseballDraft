// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include <map>
#include <algorithm>
#include "wx/tipdlg.h"
#include "wx/textcompleter.h"
#include "leaguesettings.h"
#include "namecompleter.h"

CNameCompleter::CNameCompleter(const std::vector<CPlayer> *players, bool strict)
{
	m_strict = strict;
	m_players = *players;
	m_positionFilter = 0;
}

CNameCompleter::~CNameCompleter(void)
{
}

void CNameCompleter::GetCompletions(const wxString& prefix, wxArrayString& res)
{
	std::vector<CPlayer>::iterator it;
	if( prefix.empty() )
		return;
	if( !wxIsalpha( prefix[0] ) )
		return;
	wxString temp = prefix.Capitalize();
	int pos1 = temp.Find( ' ' );
	int pos2 = temp.Find( '.' );
	if( pos1 != wxNOT_FOUND )
	{
		wxString temp1 = temp.substr( 0, pos1 );
		wxString temp2 = temp.substr( pos1 + 1 );
		temp2 = temp2.MakeCapitalized();
		temp = temp1 + ' ' + temp2;
	}
	else if( pos2 != wxNOT_FOUND )
	{
		wxString temp1 = temp.substr( 0, pos1 );
		wxString temp2 = temp.substr( pos1 + 1 );
		temp2 = temp2.MakeCapitalized();
		temp = temp1 + ' ' + temp2;
	}
	for( it = m_players.begin(); it < m_players.end(); it++ )
	{
		if( it->IsPlayerDeleted() )
			continue;
		if( m_strict && it->IsPlayerDrafted() )
			continue;
		if( ( it->IsPlayerDrafted() && m_generalFilter == 0 ) || ( !it->IsPlayerDrafted() && m_generalFilter == 1 ) )
			continue;
		if( ( m_positionFilterFull == "All Hitters" && !it->IsHitter() ) || ( m_positionFilterFull == "All Pitchers" && it->IsHitter() ) )
			continue;
		wxString playerName = it->GetName();
		wxString lname = playerName.substr( playerName.find( ' ' ) + 1 );
		if( playerName.StartsWith( temp ) )
		{
			std::vector<wxString> positions = (*it).GetPositions();
			int res1 = m_positionFilter & BASEBALLDRAFT_1B_FILTER;
			bool res2 = std::find( positions.begin(), positions.end(), wxString( "1B" ) ) == positions.end();
			res1 = m_positionFilter & BASEBALLDRAFT_C_FILTER;
			res2 = std::find( positions.begin(), positions.end(), wxString( "C" ) ) == positions.end();
			if( ( m_positionFilter == 0 ) ||
				( m_positionFilter & BASEBALLDRAFT_C_FILTER && std::find( positions.begin(), positions.end(), wxString( "C" ) ) != positions.end() ) ||
			    ( m_positionFilter & BASEBALLDRAFT_1B_FILTER && std::find( positions.begin(), positions.end(), wxString( "1B" ) ) != positions.end() ) ||
			    ( m_positionFilter & BASEBALLDRAFT_2B_FILTER && std::find( positions.begin(), positions.end(), wxString( "2B" ) ) != positions.end() ) ||
			    ( m_positionFilter & BASEBALLDRAFT_SS_FILTER && std::find( positions.begin(), positions.end(), wxString( "SS" ) ) != positions.end() ) ||
			    ( m_positionFilter & BASEBALLDRAFT_3B_FILTER && std::find( positions.begin(), positions.end(), wxString( "3B" ) ) != positions.end() ) ||
			    ( m_positionFilter & BASEBALLDRAFT_OF_FILTER && std::find( positions.begin(), positions.end(), wxString( "OF" ) ) != positions.end() ) ||
			    ( m_positionFilter & BASEBALLDRAFT_DH_FILTER && std::find( positions.begin(), positions.end(), wxString( "DH" ) ) != positions.end() ) ||
			    ( m_positionFilter & BASEBALLDRAFT_CI_FILTER && ( std::find( positions.begin(), positions.end(), wxString( "1B" ) ) != positions.end() && std::find( positions.begin(), positions.end(), wxString( "3B" ) ) != positions.end() ) ) ||
			    ( m_positionFilter & BASEBALLDRAFT_MI_FILTER && ( std::find( positions.begin(), positions.end(), wxString( "2B" ) ) != positions.end() && std::find( positions.begin(), positions.end(), wxString( "SS" ) ) != positions.end() ) ) ||
			    ( m_positionFilter & BASEBALLDRAFT_P_FILTER && std::find( positions.begin(), positions.end(), wxString( "P" ) ) != positions.end() ) ||
			    ( m_positionFilter & BASEBALLDRAFT_SP_FILTER && std::find( positions.begin(), positions.end(), wxString( "SP" ) ) != positions.end() ) ||
			    ( m_positionFilter & BASEBALLDRAFT_RP_FILTER && std::find( positions.begin(), positions.end(), wxString( "RP" ) ) == positions.end() ) )
				res.push_back( playerName );
		}
	}
}

std::vector<CPlayer> &CNameCompleter::GetPlayers()
{
	return m_players;
}

void CNameCompleter::SetGeneralFilter(int filter)
{
	m_generalFilter = filter;
}

void CNameCompleter::SetFullPositionFilter(const wxString &filter)
{
	m_positionFilterFull = filter;
}

void CNameCompleter::SetPositionFilter(const int &filter)
{
	m_positionFilter = filter;
}

void CNameCompleter::SetPlayers(const std::vector<CPlayer> &players)
{
	m_players = players;
	PlayerSorter sorter;
	sorter.m_type.push_back( SortObject( SORT_BY_NAME, true ) );
	std::sort( m_players.begin(), m_players.end(), sorter );
}
