// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include "wx/tipdlg.h"
#include "wx/tglbtn.h"
#include "wx/grid.h"
#include "leaguesettings.h"
#include "teamprojections.h"

CTeamProjections::CTeamProjections(wxWindow *parent, const CLeagueSettings &settings, const std::vector<CPlayer> &draftResult) : wxPanel( parent )
{
	std::vector<wxString> hitterScoring = settings.GetHitScoring();
	std::vector<wxString> pitcherScoring = settings.GetPitchScoring();
	for( int i = 0; i < 53; i++ )
	{
		for( std::vector<std::string>::iterator it = settings.GetOwners().begin(); it < settings.GetOwners().end(); it++ )
			m_projs[i].insert( std::make_pair( (*it), 0 ) );
	}
	m_score.insert( std::make_pair( "AVG", m_projs[0] ) );
	m_score.insert( std::make_pair( "HR1", m_projs[1] ) );
	m_score.insert( std::make_pair( "Runs", m_projs[2] ) );
	m_score.insert( std::make_pair( "RBI", m_projs[3] ) );
	m_score.insert( std::make_pair( "SB", m_projs[4] ) );
	m_score.insert( std::make_pair( "OBP", m_projs[5] ) );
	m_score.insert( std::make_pair( "SLG", m_projs[6]) );
	m_score.insert( std::make_pair( "OPS", m_projs[7] ) );
	m_score.insert( std::make_pair( "BB1", m_projs[8] ) );
	m_score.insert( std::make_pair( "H1", m_projs[9] ) );
	m_score.insert( std::make_pair( "K1", m_projs[10] ) );
	m_score.insert( std::make_pair( "1B", m_projs[11] ) );
	m_score.insert( std::make_pair( "2B", m_projs[12] ) );
	m_score.insert( std::make_pair( "3B", m_projs[13] ) );
	m_score.insert( std::make_pair( "TB", m_projs[14] ) );
	m_score.insert( std::make_pair( "2B+3B+HR", m_projs[15] ) );
	m_score.insert( std::make_pair( "2B+3B", m_projs[16] ) );
	m_score.insert( std::make_pair( "SB-CS", m_projs[17] ) );
	m_score.insert( std::make_pair( "CS", m_projs[18] ) );
	m_score.insert( std::make_pair( "R+RBI-HR", m_projs[19] ) );
	m_score.insert( std::make_pair( "H+BB", m_projs[20] ) );
	m_score.insert( std::make_pair( "TB+BB", m_projs[21] ) );
	m_score.insert( std::make_pair( "AB", m_projs[22] ) );
	m_score.insert( std::make_pair( "HBP", m_projs[23] ) );
	m_score.insert( std::make_pair( "SF", m_projs[24] ) );
	m_score.insert( std::make_pair( "GDP", m_projs[25] ) );
	m_score.insert( std::make_pair( "Wins", m_projs[26] ) );
	m_score.insert( std::make_pair( "Saves", m_projs[27] ) );
	m_score.insert( std::make_pair( "ERA", m_projs[28] ) );
	m_score.insert( std::make_pair( "WHIP", m_projs[29] ) );
	m_score.insert( std::make_pair( "K2", m_projs[30] ) );
	m_score.insert( std::make_pair( "IP", m_projs[31] ) );
	m_score.insert( std::make_pair( "L", m_projs[32] ) );
	m_score.insert( std::make_pair( "H2", m_projs[33] ) );
	m_score.insert( std::make_pair( "BB2", m_projs[34] ) );
	m_score.insert( std::make_pair( "ER", m_projs[35] ) );
	m_score.insert( std::make_pair( "CG", m_projs[36] ) );
	m_score.insert( std::make_pair( "W-L", m_projs[37] ) );
	m_score.insert( std::make_pair( "Shutouts", m_projs[38] ) );
	m_score.insert( std::make_pair( "BS", m_projs[39] ) );
	m_score.insert( std::make_pair( "S-BS", m_projs[40] ) );
	m_score.insert( std::make_pair( "S+Holds", m_projs[41] ) );
	m_score.insert( std::make_pair( "S+Holds-BS", m_projs[42] ) );
	m_score.insert( std::make_pair( "K/9", m_projs[43] ) );
	m_score.insert( std::make_pair( "H/9", m_projs[44] ) );
	m_score.insert( std::make_pair( "BB/9", m_projs[45] ) );
	m_score.insert( std::make_pair( "K/BB", m_projs[46] ) );
	m_score.insert( std::make_pair( "K-BB", m_projs[47] ) );
	m_score.insert( std::make_pair( "W%", m_projs[48] ) );
	m_score.insert( std::make_pair( "Holds", m_projs[49] ) );
	m_score.insert( std::make_pair( "G", m_projs[50] ) );
	m_score.insert( std::make_pair( "R", m_projs[51] ) );
	m_score.insert( std::make_pair( "HR2", m_projs[52] ) );
	wxBoxSizer *mainSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *vertical = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *topSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *bottomSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	int rows = settings.GetOwnersNumber(), row;
	std::vector<std::string>::const_iterator it_row;
	std::vector<wxString>::iterator it_hits, it_pitch;
	m_stats = new wxGrid( this, wxID_ANY );
	m_projections = new wxGrid( this, wxID_ANY );
	m_targets = new wxToggleButton( this, wxID_ANY, "Show Targets", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_stats->CreateGrid( rows, 54 );
	m_projections->CreateGrid( rows, 57 );
	m_stats->BeginBatch();
	row = 0;
	for( it_row = settings.GetOwners().begin(); it_row < settings.GetOwners().end(); it_row++ )
		m_stats->SetRowLabelValue( row++, (*it_row) );
	m_stats->SetColLabelValue( 0, "AVG" );
	m_stats->SetColLabelValue( 1, "HR" );
	m_stats->SetColLabelValue( 2, "Runs" );
	m_stats->SetColLabelValue( 3, "RBI" );
	m_stats->SetColLabelValue( 4, "SB" );
	m_stats->SetColLabelValue( 5, "OBP" );
	m_stats->SetColLabelValue( 6, "SLG" );
	m_stats->SetColLabelValue( 7, "OPS" );
	m_stats->SetColLabelValue( 8, "BB" );
	m_stats->SetColLabelValue( 9, "H" );
	m_stats->SetColLabelValue( 10, "K" );
	m_stats->SetColLabelValue( 11, "1B" );
	m_stats->SetColLabelValue( 12, "2B" );
	m_stats->SetColLabelValue( 13, "3B" );
	m_stats->SetColLabelValue( 14, "TB" );
	m_stats->SetColLabelValue( 15, "2B+3B+HR" );
	m_stats->SetColLabelValue( 16, "2B+3B" );
	m_stats->SetColLabelValue( 17, "SB-CS" );
	m_stats->SetColLabelValue( 18, "CS" );
	m_stats->SetColLabelValue( 19, "R+RBI+HR" );
	m_stats->SetColLabelValue( 20, "H+BB" );
	m_stats->SetColLabelValue( 21, "TB+BB" );
	m_stats->SetColLabelValue( 22, "AB" );
	m_stats->SetColLabelValue( 23, "HBP" );
	m_stats->SetColLabelValue( 24, "GDP" );
	m_stats->SetColLabelValue( 25, "SF" );
	m_stats->SetColLabelValue( 26, "Wins" );
	m_stats->SetColLabelValue( 27, "Saves" );
	m_stats->SetColLabelValue( 28, "ERA" );
	m_stats->SetColLabelValue( 29, "WHIP" );
	m_stats->SetColLabelValue( 30, "K" );
	m_stats->SetColLabelValue( 31, "IP" );
	m_stats->SetColLabelValue( 32, "L" );
	m_stats->SetColLabelValue( 33, "H" );
	m_stats->SetColLabelValue( 34, "BB" );
	m_stats->SetColLabelValue( 35, "ER" );
	m_stats->SetColLabelValue( 36, "CG" );
	m_stats->SetColLabelValue( 37, "W-L" );
	m_stats->SetColLabelValue( 38, "QS" );
	m_stats->SetColLabelValue( 39, "Shutouts" );
	m_stats->SetColLabelValue( 40, "BS" );
	m_stats->SetColLabelValue( 41, "S-BS" );
	m_stats->SetColLabelValue( 42, "S+Holds" );
	m_stats->SetColLabelValue( 43, "S+Holds-BS" );
	m_stats->SetColLabelValue( 44, "K/9" );
	m_stats->SetColLabelValue( 45, "H/9" );
	m_stats->SetColLabelValue( 46, "BB/9" );
	m_stats->SetColLabelValue( 47, "K/BB" );
	m_stats->SetColLabelValue( 48, "K-BB" );
	m_stats->SetColLabelValue( 49, "W%" );
	m_stats->SetColLabelValue( 50, "Holds" );
	m_stats->SetColLabelValue( 51, "G" );
	m_stats->SetColLabelValue( 52, "R" );
	m_stats->SetColLabelValue( 53, "HR" );
	m_stats->EnableEditing( false );
	m_projections->EnableEditing( false );
	for( std::vector<CPlayer>::const_iterator it = draftResult.begin(); it < draftResult.end(); it++ )
		AddPlayer( const_cast<CPlayer &>( (*it) ).GetOwner(), (*it) );
	m_stats->EndBatch();
	m_projections->BeginBatch();
	row = 0;
	for( it_row = settings.GetOwners().begin(); it_row < settings.GetOwners().end(); it_row++ )
		m_projections->SetRowLabelValue( row++, (*it_row) );
	m_projections->SetColLabelValue( 0, "AVG" );
	m_projections->SetColLabelValue( 1, "HR" );
	m_projections->SetColLabelValue( 2, "Runs" );
	m_projections->SetColLabelValue( 3, "RBI" );
	m_projections->SetColLabelValue( 4, "SB" );
	m_projections->SetColLabelValue( 5, "OBP" );
	m_projections->SetColLabelValue( 6, "SLG" );
	m_projections->SetColLabelValue( 7, "OPS" );
	m_projections->SetColLabelValue( 8, "BB" );
	m_projections->SetColLabelValue( 9, "H" );
	m_projections->SetColLabelValue( 10, "K" );
	m_projections->SetColLabelValue( 11, "1B" );
	m_projections->SetColLabelValue( 12, "2B" );
	m_projections->SetColLabelValue( 13, "3B" );
	m_projections->SetColLabelValue( 14, "TB" );
	m_projections->SetColLabelValue( 15, "2B+3B+HR" );
	m_projections->SetColLabelValue( 16, "2B+3B" );
	m_projections->SetColLabelValue( 17, "SB-CS" );
	m_projections->SetColLabelValue( 18, "CS" );
	m_projections->SetColLabelValue( 19, "R+RBI+HR" );
	m_projections->SetColLabelValue( 20, "H+BB" );
	m_projections->SetColLabelValue( 21, "TB+BB" );
	m_projections->SetColLabelValue( 22, "AB" );
	m_projections->SetColLabelValue( 23, "HBP" );
	m_projections->SetColLabelValue( 24, "GDP" );
	m_projections->SetColLabelValue( 25, "SF" );
	m_projections->SetColLabelValue( 26, "Wins" );
	m_projections->SetColLabelValue( 27, "Saves" );
	m_projections->SetColLabelValue( 28, "ERA" );
	m_projections->SetColLabelValue( 29, "WHIP" );
	m_projections->SetColLabelValue( 30, "K" );
	m_projections->SetColLabelValue( 31, "IP" );
	m_projections->SetColLabelValue( 32, "L" );
	m_projections->SetColLabelValue( 33, "H" );
	m_projections->SetColLabelValue( 34, "BB" );
	m_projections->SetColLabelValue( 35, "ER" );
	m_projections->SetColLabelValue( 36, "CG" );
	m_projections->SetColLabelValue( 37, "W-L" );
	m_projections->SetColLabelValue( 38, "QS" );
	m_projections->SetColLabelValue( 39, "Shuitouts" );
	m_projections->SetColLabelValue( 40, "BS" );
	m_projections->SetColLabelValue( 41, "S-BS" );
	m_projections->SetColLabelValue( 42, "S+Holds" );
	m_projections->SetColLabelValue( 43, "S+Holds-BS" );
	m_projections->SetColLabelValue( 44, "K/9" );
	m_projections->SetColLabelValue( 45, "H/9" );
	m_projections->SetColLabelValue( 46, "BB/9" );
	m_projections->SetColLabelValue( 47, "K/BB" );
	m_projections->SetColLabelValue( 48, "K-BB" );
	m_projections->SetColLabelValue( 49, "W%" );
	m_projections->SetColLabelValue( 50, "Holds" );
	m_projections->SetColLabelValue( 51, "G" );
	m_projections->SetColLabelValue( 52, "R" );
	m_projections->SetColLabelValue( 53, "HR" );
	m_projections->EnableEditing( false );
	m_projections->SetColLabelValue( 54, "Hit" );
	m_projections->SetColLabelValue( 55, "Pitch" );
	m_projections->SetColLabelValue( 56, "Total" );
	m_projections->EndBatch();
	m_projections->SetColSize( 54, wxGRID_AUTOSIZE );
	m_projections->SetColSize( 55, wxGRID_AUTOSIZE );
	m_projections->SetColSize( 56, wxGRID_AUTOSIZE );
	m_projections->AutoSizeColumn( 54 );
	m_projections->AutoSizeColumn( 55 );
	m_projections->AutoSizeColumn( 56 );
	ChangeGrids( hitterScoring, pitcherScoring );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	topSizer->Add( m_stats, 1, wxEXPAND, 0 );
	vertical->Add( topSizer, 1, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	bottomSizer->Add( m_projections, 1, wxEXPAND, 0 );
	vertical->Add( bottomSizer, 1, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	buttonSizer->Add( m_targets, 0, wxEXPAND, 0 );
	vertical->Add( buttonSizer, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	mainSizer->Add( vertical, 0, wxEXPAND, 0 );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	SetSizer( mainSizer );
	mainSizer->Fit( this );
//	Layout();
}

CTeamProjections::~CTeamProjections(void)
{
}

void CTeamProjections::AddPlayer(const wxString &owner, const CPlayer &player)
{
	std::map<wxString,double> playerScoring = const_cast<CPlayer &>( player ).GetScoring();
	std::map<wxString,double>::iterator it;
	int row = -1, i;
	bool found = false;
	for( i = 0; i < m_stats->GetNumberRows() && !found; i++ )
	{
		if( owner == m_stats->GetRowLabelValue( i ) )
		{
			row = i;
			found = true;
		}
	}
	m_stats->BeginBatch();
	for( it = playerScoring.begin(); it != playerScoring.end(); it++ )
	{
		int startCol = player.IsHitter() ? 0 : 26, endCol = player.IsHitter() ? 26 : 54;
		for( i = startCol; i < endCol; i++ )
		{
			if( (*it).first == m_stats->GetColLabelValue( i ) )
			{
				double value;
				wxString val = m_stats->GetCellValue( row, i );
				if( val.IsEmpty() )
					value = 0;
				else
					val.ToDouble( &value );
				value += (*it).second;
				m_stats->SetCellValue( row, i, wxString::Format( "%.3f", value ) );
			}
		}
	}
	m_stats->AutoSize();
	m_stats->EndBatch();
	ChangeProjections( owner, player );
}

void CTeamProjections::Resize()
{
	m_stats->AutoSize();
	m_projections->AutoSize();
}

void CTeamProjections::ChangeProjections(const wxString &owner, const CPlayer &player)
{
	for( MapStringToPair::iterator it = m_score.begin(); it != m_score.end(); it++ )
	{
		wxString score = (*it).first;
		bool found = false;
		double value;
		for( SDSetPair::iterator it1 = (*it).second.begin(); it1 != (*it).second.end() && !found; it1++ )
		{
			if( (*it1).first == owner )
			{
				value = (*it1).second;
				found = true;
			}
		}
		found = false;
		(*it).second.erase( SDPair( owner, value ) );
		(*it).second.insert( std::make_pair( owner, value + player.GetAmountPaid() ) );
	}
}

void CTeamProjections::UnAssignPlayer(const CPlayer &player)
{
	std::map<wxString,double> playerScoring = const_cast<CPlayer &>( player ).GetScoring();
	std::map<wxString,double>::iterator it;
	int row = -1, i;
	bool found = false;
	for( i = 0; i < m_stats->GetNumberRows() && !found; i++ )
	{
		if( const_cast<CPlayer &>( player ).GetOwner() == m_stats->GetRowLabelValue( i ) )
		{
			row = i;
			found = true;
		}
	}
	for( it = playerScoring.begin(); it != playerScoring.end(); it++ )
	{
		bool found = false;
		for( i = 0; i < m_stats->GetNumberCols() && !found; i++ )
		{
			if( (*it).first == m_stats->GetColLabelValue( i ) )
			{
				double value;
				wxString val = m_stats->GetCellValue( row, i );
				if( val.IsEmpty() )
					value = 0;
				else
					val.ToDouble( &value );
				value -= (*it).second;
				m_stats->SetCellValue( row, i, value == 0 ? wxString() : wxString::Format( "%.3f", value ) );
				found = true;
			}
		}
	}
	m_stats->AutoSize();
}

void CTeamProjections::ResetProjections()
{
	for( int i = 0; i < m_stats->GetNumberRows(); i++ )
		for( int j = 0; j < m_stats->GetNumberRows(); j++ )
			m_stats->SetCellValue( i, j, wxEmptyString );
	for( int i = 0; i < m_projections->GetNumberRows(); i++ )
		for( int j = 0; j < m_projections->GetNumberCols(); j++ )
			m_projections->SetCellValue( i, j, wxEmptyString );
	m_score.clear();
/*	for( int i = 0; i < 53; i++ )
		m_projs[i].clear();*/
}

void CTeamProjections::ChangeGrids(const std::vector<wxString> &hitterScoring, const std::vector<wxString> &pitcherScoring)
{
	bool found = false;
	m_stats->BeginBatch();
	for( int i = 0; i < 26; i++ )
	{
		for( std::vector<wxString>::const_iterator it = hitterScoring.begin(); it < hitterScoring.end() && !found; it++ )
		{
			if( (*it) == m_stats->GetColLabelValue( i ) )
				found = true;
		}
		if( !found )
			m_stats->HideCol( i );
		else
		{
			m_stats->SetColSize( i, wxGRID_AUTOSIZE );
			m_stats->AutoSizeColumn( i );
		}
		found = false;
	}
	found = false;
	for( int i = 26; i < 54; i++ )
	{
		for( std::vector<wxString>::const_iterator it = pitcherScoring.begin(); it < pitcherScoring.end() && !found; it++ )
		{
			if( (*it) == m_stats->GetColLabelValue( i ) )
				found = true;
		}
		if( !found )
			m_stats->HideCol( i );
		else
		{
			m_stats->SetColSize( i, wxGRID_AUTOSIZE );
			m_stats->AutoSizeColumn( i );
		}
		found = false;
	}
	m_stats->EndBatch();
	found = false;
	m_projections->BeginBatch();
	for( int i = 0; i < 26; i++ )
	{
		for( std::vector<wxString>::const_iterator it = hitterScoring.begin(); it < hitterScoring.end() && !found; it++ )
		{
			if( (*it) == m_projections->GetColLabelValue( i ) )
				found = true;
		}
		if( !found )
			m_projections->HideCol( i );
		else
		{
			m_projections->SetColSize( i, wxGRID_AUTOSIZE );
			m_projections->AutoSizeColumn( i );
		}
		found = false;
	}
	found = false;
	for( int i = 26; i < 54; i++ )
	{
		for( std::vector<wxString>::const_iterator it = pitcherScoring.begin(); it < pitcherScoring.end() && !found; it++ )
		{
			if( (*it) == m_projections->GetColLabelValue( i ) )
				found = true;
		}
		if( !found )
			m_projections->HideCol( i );
		else
		{
			m_projections->SetColSize( i, wxGRID_AUTOSIZE );
			m_projections->AutoSizeColumn( i );
		}
		found = false;
	}
	m_projections->EndBatch();
}
