// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <map>
#include <vector>
#include <algorithm>
#include "wx/tipdlg.h"
#include "wx/tglbtn.h"
#include "wx/grid.h"
#include "leaguesettings.h"
#include "teamprojections.h"

CTeamProjections::CTeamProjections(wxWindow *parent, const CLeagueSettings &settings, const std::vector<CPlayer> &draftResult) : wxPanel( parent )
{
	for( int i = 0; i < 53; i++ )
	{
		for( std::vector<std::string>::iterator it = settings.GetOwners().begin(); it < settings.GetOwners().end(); it++ )
			m_projs[i].push_back( Projections( (*it) ) );
	}
	m_score["AVG"] = m_projs[0];
	m_score["HR1"] = m_projs[1];
	m_score["Runs"] = m_projs[2];
	m_score["RBI"] = m_projs[3];
	m_score["SB"] = m_projs[4];
	m_score["OBP"] = m_projs[5];
	m_score["SLG"] = m_projs[6];
	m_score["OPS"] = m_projs[7];
	m_score["BB1"] = m_projs[8];
	m_score["H1"] = m_projs[9];
	m_score["K1"] = m_projs[10];
	m_score["1B"] = m_projs[11];
	m_score["2B"] = m_projs[12];
	m_score["3B"] = m_projs[13];
	m_score["TB"] = m_projs[14];
	m_score["2B+3B+HR"] = m_projs[15];
	m_score["2B+3B"] = m_projs[16];
	m_score["SB-CS"] = m_projs[17];
	m_score["CS"] = m_projs[18];
	m_score["R+RBI-HR"] = m_projs[19];
	m_score["H+BB"] = m_projs[20];
	m_score["TB+BB"] = m_projs[21];
	m_score["AB"] = m_projs[22];
	m_score["HBP"] = m_projs[23];
	m_score["SF"] = m_projs[24];
	m_score["GDP"] = m_projs[25];
	m_score["Wins"] = m_projs[26];
	m_score["Saves"] = m_projs[27];
	m_score["ERA"] = m_projs[28];
	m_score["WHIP"] = m_projs[29];
	m_score["K2"] = m_projs[30];
	m_score["IP"] = m_projs[31];
	m_score["L"] = m_projs[32];
	m_score["H2"] = m_projs[33];
	m_score["BB2"] = m_projs[34];
	m_score["ER"] = m_projs[35];
	m_score["CG"] = m_projs[36];
	m_score["W-L"] = m_projs[37];
	m_score["Shutouts"] = m_projs[38];
	m_score["BS"] = m_projs[39];
	m_score["S-BS"] = m_projs[40];
	m_score["S+Holds"] = m_projs[41];
	m_score["S+Holds-BS"] = m_projs[42];
	m_score["K/9"] = m_projs[43];
	m_score["H/9"] = m_projs[44];
	m_score["BB/9"] = m_projs[45];
	m_score["K/BB"] = m_projs[46];
	m_score["K-BB"] = m_projs[47];
	m_score["W%"] = m_projs[48];
	m_score["Holds"] = m_projs[49];
	m_score["G"] = m_projs[50];
	m_score["R"] = m_projs[51];
	m_score["HR2"] = m_projs[52];
	wxBoxSizer *mainSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *vertical = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *topSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *bottomSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	int rows = settings.GetOwnersNumber(), cols = settings.GetHitScoring().size() + settings.GetPitchScoring().size(), row, col, cols1 = cols + 4;
	std::vector<std::string>::const_iterator it_row;
	std::vector<wxString>::iterator it_hits, it_pitch;
	m_stats = new wxGrid( this, wxID_ANY );
	m_projections = new wxGrid( this, wxID_ANY );
	m_targets = new wxToggleButton( this, wxID_ANY, "Show Targets", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_stats->CreateGrid( rows, cols );
	m_projections->CreateGrid( rows, cols1 );
	m_stats->BeginBatch();
	row = 0;
	for( it_row = settings.GetOwners().begin(); it_row < settings.GetOwners().end(); it_row++ )
		m_stats->SetRowLabelValue( row++, (*it_row) );
	col = 0;
	for( it_hits = settings.GetHitScoring().begin(); it_hits < settings.GetHitScoring().end(); it_hits++ )
		m_stats->SetColLabelValue( col++, (*it_hits) );
	for( it_pitch = settings.GetPitchScoring().begin(); it_pitch < settings.GetPitchScoring().end(); it_pitch++ )
		m_stats->SetColLabelValue( col++, (*it_pitch) );
	m_stats->EnableEditing( false );
	m_projections->EnableEditing( false );
	m_stats->AutoSize();
	for( std::vector<CPlayer>::const_iterator it = draftResult.begin(); it < draftResult.end(); it++ )
		AddPlayer( const_cast<CPlayer &>( (*it) ).GetOwner(), (*it) );
	m_stats->EndBatch();
	m_projections->BeginBatch();
	m_projections->HideRowLabels();
	row = 0;
	m_projections->SetColLabelValue( 0, "Team" );
	for( it_row = settings.GetOwners().begin(); it_row < settings.GetOwners().end(); it_row++ )
		m_projections->SetCellValue( row++, 0, (*it_row) );
	col = 1;
	for( it_hits = settings.GetHitScoring().begin(); it_hits < settings.GetHitScoring().end(); it_hits++ )
		m_projections->SetColLabelValue( col++, (*it_hits) );
	for( it_pitch = settings.GetPitchScoring().begin(); it_pitch < settings.GetPitchScoring().end(); it_pitch++ )
		m_projections->SetColLabelValue( col++, (*it_pitch) );
	m_projections->SetColLabelValue( col++, "Hit" );
	m_projections->SetColLabelValue( col++, "Pitch" );
	m_projections->SetColLabelValue( col++, "Total" );
	m_projections->EndBatch();
	m_projections->AutoSize();
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
	SetSizerAndFit( mainSizer );
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
	for( it = playerScoring.begin(); it != playerScoring.end(); it++ )
	{
		for( i = 0; i < m_stats->GetNumberCols(); i++ )
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
	ChangeProjections( owner, player );
}

void CTeamProjections::Resize()
{
	m_stats->AutoSize();
	m_projections->AutoSize();
}

void CTeamProjections::ChangeProjections(const wxString &owner, const CPlayer &player)
{
	bool found = false, found1 = false;
	for( std::map<wxString,std::vector<Projections> >::iterator it = m_score.begin(); it != m_score.end(); it++ )
	{
		for( std::map<wxString,double>::iterator it1 = const_cast<CPlayer &>( player ).GetScoring().begin(); it1 != const_cast<CPlayer &>( player ).GetScoring().end() && !found; it1++ )
		{
			if( (*it).first == (*it1).first )
			{
				found = true;
				for( std::vector<Projections>::iterator it2 = (*it).second.begin(); it2 < (*it).second.end() && !found1; it2++ )
				{
					if( (*it2).GetOwner() == owner )
					{
						found1 = true;
						(*it2) = (*it2) + (*it1).second;
						std::sort( (*it).second.begin(), (*it).second.end() );
					}
				}
			}
		}
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
	for( int i = 0; i < 53; i++ )
		m_projs[i].clear();
}
