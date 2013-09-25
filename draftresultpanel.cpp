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
#include "wx/grid.h"
#include "leaguesettings.h"
#include "draftresultpanel.h"

CDraftResultPanel::CDraftResultPanel(wxWindow *parent, const CLeagueSettings &settings, std::vector<CPlayer> &draftResult) : wxPanel( parent )
{
	PlayerSorter sorter;
	sorter.m_forward = true;
	sorter.m_sortType = SORT_BY_DRAFTPOSITION;
	std::sort( draftResult.begin(), draftResult.end(), sorter );
	m_label1 = new wxStaticText( this, wxID_ANY, "Search" );
	m_label2 = new wxStaticText( this, wxID_ANY, "Team" );
	m_search = new wxTextCtrl( this, wxID_ANY );
	m_team = new wxComboBox( this, wxID_ANY );
	for( unsigned int i = 0; i < settings.GetOwnersNumber(); i++ )
		m_team->Append( settings.GetOwners().at( i ) );
	m_results = new wxGrid( this, wxID_ANY );
	m_results->CreateGrid( 0, 5 );
	m_results->BeginBatch();
	m_results->HideRowLabels();
	m_results->SetColLabelValue( 0, "Draft Order" );
	m_results->SetColLabelValue( 1, "Player" );
	m_results->SetColLabelValue( 2, "Team" );
	m_results->SetColLabelValue( 3, "AmtPaid" );
	m_results->SetColLabelValue( 4, "Profit" );
	for( std::vector<CPlayer>::iterator it = draftResult.begin(); it < draftResult.end(); it++ )
	{
		m_results->AppendRows();
		int row = m_results->GetNumberRows() - 1;
		m_results->SetCellValue( row, 0, wxString::Format( "%d", (*it).GetDraftOrder() ) );
		m_results->SetCellValue( row, 1, (*it).GetName() );
		m_results->SetCellValue( row, 2, (*it).GetAbbeviatedTeamName() );
		m_results->SetCellValue( row, 3, wxString::Format( "%d", (*it).GetAmountPaid() ) );
		m_results->SetCellValue( row, 4, wxString::Format( "%d", (*it).GetValue() - (*it).GetAmountPaid() ) );
	}
	m_results->EndBatch();
	wxBoxSizer *mainSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *vertical = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *top = new wxBoxSizer( wxHORIZONTAL );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	top->Add( m_label1, 0, wxEXPAND, 0 );
	top->Add( 5, 5, 0, wxEXPAND, 0 );
	top->Add( m_search, 0, wxEXPAND, 0 );
	top->AddStretchSpacer();
	top->Add( m_label2, 0, wxEXPAND, 0 );
	top->Add( 5, 5, 0, wxEXPAND, 0 );
	top->Add( m_team, 0, wxEXPAND, 0 );
	vertical->Add( top, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( m_results, 1, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	mainSizer->Add( vertical, 1, wxEXPAND, 0 );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	SetSizerAndFit( mainSizer );
//	Layout();
	m_team->Bind( wxEVT_COMMAND_COMBOBOX_SELECTED, &CDraftResultPanel::OnTeamSelection, this );
}

CDraftResultPanel::~CDraftResultPanel(void)
{
	m_team->Unbind( wxEVT_COMMAND_COMBOBOX_SELECTED, &CDraftResultPanel::OnTeamSelection, this );
}

void CDraftResultPanel::AddDraftedPlayer(const CPlayer &player, bool isDrafted)
{
	if( isDrafted )
		m_draftedPlayers.push_back( player );
	m_results->AppendRows();
	int row = m_results->GetNumberRows();
	m_results->SetCellValue( row - 1, 0, wxString::Format( "%d", player.GetDraftOrder() ) );
	m_results->SetCellValue( row - 1, 1, const_cast<CPlayer &>( player ).GetName() );
	m_results->SetCellValue( row - 1, 2, const_cast<CPlayer &>( player ).GetOwner() );
	m_results->SetCellValue( row - 1, 3, wxString::Format( "$%d", const_cast<CPlayer &>( player ).GetAmountPaid() ) );
	if( player.GetValue() - player.GetAmountPaid() != 0 )
		m_results->SetCellValue( row - 1, 4, wxString::Format( "$%d", player.GetValue() - player.GetAmountPaid() ) );
	m_results->AutoSize();
}

void CDraftResultPanel::OnTeamSelection(wxCommandEvent &WXUNUSED(event))
{
	wxString owner = m_team->GetValue();
	m_results->BeginBatch();
	m_results->DeleteRows( 0, m_results->GetNumberRows() );
	for( std::vector<CPlayer>::iterator it = m_draftedPlayers.begin(); it < m_draftedPlayers.end(); it++ )
	{
		if( (*it).GetOwner() == owner )
			AddDraftedPlayer( (*it), false );
	}
	m_results->EndBatch();
}

void CDraftResultPanel::UnAssignPlayer(const CPlayer &player)
{
	bool found = false;
	int row = 0;
	wxString order;
	m_results->BeginBatch();
	for( int i = 0; i < m_results->GetNumberRows() && !found; i++ )
	{
		if( const_cast<CPlayer &>( player ).GetName() == m_results->GetCellValue( i, 1 ) && const_cast<CPlayer &>( player ).GetOwner() == m_results->GetCellValue( i, 2 ) )
		{
			found = true;
			row = i;
			order = m_results->GetCellValue( i, 0 );
		}
	}
	for( int i = row + 1; i < m_results->GetNumberRows(); i++ )
	{
		m_draftedPlayers.at( i ).SetDraftOrder( m_draftedPlayers.at( i ).GetDraftOrder() );
		m_results->SetCellValue( i, 0, wxString::Format( "%d", m_draftedPlayers.at( i ).GetDraftOrder() ) );
	}
	m_results->DeleteRows( row );
	m_results->EndBatch();
}

void CDraftResultPanel::ResetPanel()
{
	for( int i = 0; i < m_results->GetNumberRows(); i++ )
		for( int j = 0; j < m_results->GetNumberRows(); j++ )
			m_results->SetCellValue( i, j, wxEmptyString );
	m_draftedPlayers.clear();
}
