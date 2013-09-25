// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <map>
#include <vector>
#include "sqlite3.h"
#include "wx/tipdlg.h"
#include "wx/grid.h"
#include "wx/tglbtn.h"
#include "wx/textcompleter.h"
#include "textctrl.h"
#include "leaguesettings.h"
#include "playerspanelgridtable.h"
#include "db.h"
#include "teamsroster.h"
#include "namecompleter.h"
#include "playerspanel.h"
#include "teamprojections.h"
#include "draftresultpanel.h"
#include "draftresultsalary.h"
#include "gridtotalcell.h"
#include "draftresult.h"
#include "playerdraft.h"
#include "frame.h"

IMPLEMENT_DYNAMIC_CLASS(CTeamsRoster, wxPanel)

CTeamsRoster::CTeamsRoster() : wxPanel()
{
}

CTeamsRoster::CTeamsRoster(wxWindow *parent, const CLeagueSettings &settings, const std::vector<CPlayer> &draftResult) : wxPanel( parent )
{
	m_visibleRow = 4;
	int hitters, pitchers, i;
	m_draftresult = draftResult;
	std::map<wxString,int> positions = settings.GetPositions();
	hitters = positions["C"] + positions["1B"] + positions["3B"] + positions["CI"] + positions["2B"] + positions["SS"] + positions["MI"] + positions["OF"] + positions["Utility"];
	if( !settings.GetLeagueType() )
		hitters += positions["DH"];
	pitchers = positions["P"];
	if( !settings.GetLeagueType() )
	{
		pitchers += positions["SP"];
		pitchers += positions["RP"];
	}
	m_roster = new wxGrid( this, wxID_ANY );
	m_roster->CreateGrid( hitters + pitchers + 4, settings.GetOwnersNumber() );
	m_roster->BeginBatch();
	for( unsigned int j = 0; j < settings.GetOwners().size(); j++ )
		m_roster->SetColLabelValue( j, settings.GetOwners().at( j ) );
	m_roster->SetRowLabelValue( 0, "$ Left" );
	m_roster->SetRowLabelValue( 1, "Max Bid" );
	m_roster->SetRowLabelValue( 2, "Average" );
	m_roster->SetRowLabelValue( 3, "Profit" );
	int row = 4;
	for( i = 0; i < positions["C"]; i++ )
		m_roster->SetRowLabelValue( row++, "C" );
	for( i = 0; i < positions["1B"]; i++ )
		m_roster->SetRowLabelValue( row++, "1B" );
	for( i = 0; i < positions["3B"]; i++ )
		m_roster->SetRowLabelValue( row++, "3B" );
	for( i = 0; i < positions["CI"]; i++ )
		m_roster->SetRowLabelValue( row++, "CI" );
	for( i = 0; i < positions["2B"]; i++ )
		m_roster->SetRowLabelValue( row++, "2B" );
	for( i = 0; i < positions["SS"]; i++ )
		m_roster->SetRowLabelValue( row++, "SS" );
	for( i = 0; i < positions["MI"]; i++ )
		m_roster->SetRowLabelValue( row++, "MI" );
	for( i = 0; i < positions["OF"]; i++ )
		m_roster->SetRowLabelValue( row++, "OF" );
	for( i = 0; i < positions["Utility"]; i++ )
		m_roster->SetRowLabelValue( row++, "U" );
	if( !settings.GetLeagueType() )
	{
		for( i = 0; i < positions["DH"]; i++ )
			m_roster->SetRowLabelValue( row++, "DH" );
	}
	for( i = 0; i < positions["P"]; i++ )
		m_roster->SetRowLabelValue( row++, "P" );
	if( !settings.GetLeagueType() )
	{
		for( i = 0; i < positions["SP"]; i++ )
			m_roster->SetRowLabelValue( row++, "SP" );
		for( i = 0; i < positions["RP"]; i++ )
			m_roster->SetRowLabelValue( row++, "RP" );
	}
	m_playersLeft = new int[m_roster->GetNumberCols()];
	m_salary = new int[m_roster->GetNumberCols()];
	m_maxBid = new int[m_roster->GetNumberCols()];
	m_average = new double[m_roster->GetNumberCols()];
	m_profit = new int[m_roster->GetNumberCols()];
	m_value = new int[m_roster->GetNumberCols()];
	m_amtPaid = new int[m_roster->GetNumberCols()];
	for( i = 0; i < m_roster->GetNumberCols(); i++ )
	{
		m_playersLeft[i] = m_roster->GetNumberRows() - 4;
		m_salary[i] = settings.GetSalary();
		m_maxBid[i] = m_salary[i] - ( m_playersLeft[i] - 1 );
		m_average[i] = m_salary[i] / m_playersLeft[i];
		m_roster->SetCellValue( 0, i, wxString::Format( "%d", m_salary[i] ) );
		m_roster->SetReadOnly( 0, i );
		m_roster->SetCellValue( 1, i, wxString::Format( "%d", m_maxBid[i] ) );
		m_roster->SetReadOnly( 1, i );
		m_roster->SetCellValue( 2, i, wxString::Format( "%.3f", m_average[i] ) );
		m_roster->SetReadOnly( 2, i );
		m_value[i] = 0;
		m_amtPaid[i] = 0;
		m_profit[i] = 0;
		m_roster->SetReadOnly( 3, i );
	}
	m_roster->AutoSize();
	for( std::vector<CPlayer>::iterator it = m_draftresult.begin(); it < m_draftresult.end(); it++ )
		PerformDraft( (*it) );
	m_roster->EndBatch();
	m_roster->EnableEditing( false );
	wxBoxSizer *main = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *vertical = new wxBoxSizer( wxVERTICAL );
	main->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( m_roster, 1, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	main->Add( vertical, 1, wxEXPAND, 0 );
	main->Add( 5, 5, 0, wxEXPAND, 0 );
//	SetSizerAndFit( main );
	SetSizer( main );
	m_rowHeight = m_roster->GetRowHeight( 0 );
	m_roster->Bind( wxEVT_SCROLLWIN_LINEUP, &CTeamsRoster::OnScrollUp, this );
	m_roster->Bind( wxEVT_SCROLLWIN_LINEDOWN, &CTeamsRoster::OnScrollDown, this );
	m_roster->Bind( wxEVT_GRID_CELL_RIGHT_CLICK, &CFrame::OnContextMenu, dynamic_cast<CFrame *>( GetParent()->GetParent() ) );
}

CTeamsRoster::~CTeamsRoster(void)
{
	m_roster->Unbind( wxEVT_SCROLLWIN_LINEUP, &CTeamsRoster::OnScrollUp, this );
	m_roster->Unbind( wxEVT_SCROLLWIN_LINEDOWN, &CTeamsRoster::OnScrollDown, this );
//	m_roster->Unbind( wxEVT_GRID_CELL_RIGHT_CLICK, &CFrame::OnContextMenu, dynamic_cast<CFrame *>( GetParent()->GetParent() ) );
	delete m_salary;
	delete m_maxBid;
	delete m_average;
	delete m_playersLeft;
	delete m_value;
	delete m_amtPaid;
	delete m_profit;
}

void CTeamsRoster::PerformDraft(const CPlayer &player)
{
	m_roster->BeginBatch();
	SetPlayer( player, false );
	m_roster->AutoSize();
	m_roster->EndBatch();
}

void CTeamsRoster::OnScrollUp(wxScrollWinEvent &WXUNUSED(event))
{
	if( m_visibleRow > 4 )
	{
		m_visibleRow--;
		m_roster->SetRowHeight( m_visibleRow, m_rowHeight );
	}
}

void CTeamsRoster::OnScrollDown(wxScrollWinEvent &WXUNUSED(event))
{
	m_roster->SetRowHeight( m_visibleRow, 0 );
	m_visibleRow++;
}

wxGrid &CTeamsRoster::GetRosterControl()
{
	return *m_roster;
}

void CTeamsRoster::UnAssignPlayer(const CPlayer &player)
{
	bool found = false;
	int column = 0;
	wxString team = const_cast<CPlayer &>( player ).GetOwner(), position = const_cast<CPlayer &>( player ).GetDraftedPosition(), name = const_cast<CPlayer &>( player ).GetName();
	m_roster->BeginBatch();
	for( int i = 0; i < m_roster->GetNumberCols() && !found; i++ )
	{
		if( m_roster->GetColLabelValue( i ) == team )
		{
			column = i;
			found = true;
		}
	}
	found = false;
	for( int i = 4; i < m_roster->GetNumberRows() && !found; i++ )
	{
		if( m_roster->GetRowLabelValue( i ) == position )
		{
			wxString cell = m_roster->GetCellValue( i, column );
			if( cell.substr( cell.find( ' ' ) + 1 ) == name )
			{
				found = true;
				m_roster->SetCellValue( i, column, wxEmptyString );
			}
		}
	}
	m_playersLeft[column]++;
	m_salary[column] += player.GetAmountPaid();
	m_maxBid[column] = m_salary[column] - ( m_playersLeft[column] - 1 );
	m_average[column] = (double) m_salary[column] / (double) m_playersLeft[column];
	m_roster->SetCellValue( 0, column, wxString::Format( "%d", m_salary[column] ) );
	m_roster->SetCellValue( 1, column, wxString::Format( "%d", m_maxBid[column] ) );
	m_roster->SetCellValue( 2, column, wxString::Format( "%.3f", m_average[column] ) );
	m_value[column] -= player.GetValue();
	m_amtPaid[column] -= player.GetAmountPaid();
	m_profit[column] -= ( m_value[column] - m_amtPaid[column] );
	if( m_profit[column] != 0 )
		m_roster->SetCellValue( 3, column, wxString::Format( "%d", m_profit[column] ) );
	m_roster->AutoSize();
	m_roster->EndBatch();
}

void CTeamsRoster::OnAssignDraftedPlayers(wxCommandEvent &WXUNUSED(event))
{
	if( m_draftresult.size() == 0 )
		return;
	for( std::vector<CPlayer>::iterator it = m_draftresult.begin(); it < m_draftresult.end(); it++ )
		SetPlayer( (*it), true );
}

void CTeamsRoster::SetPlayer(const CPlayer &player, bool justCalculate)
{
	int price = player.GetAmountPaid();
	bool found = false;
	int col = -1;
	for( int i = 0; i < m_roster->GetNumberCols() && !found; i++ )
	{
		if( m_roster->GetColLabelValue( i ) == const_cast<CPlayer &>( player ).GetOwner() )
		{
			found = true;
			col = i;
		}
	}
	found = false;
	for( int i = 0; i < m_roster->GetNumberRows() && !found; i++ )
	{
		if( m_roster->GetRowLabelValue( i ) == const_cast<CPlayer &>( player ).GetDraftedPosition() && m_roster->GetCellValue( i, col ) == wxEmptyString )
		{
			found = true;
			m_roster->SetCellValue( i, col, wxString::Format( "%d %s", price, const_cast<CPlayer &>( player ).GetName() ) );
		}
	}
	m_playersLeft[col]--;
	m_salary[col] -= price;
	m_maxBid[col] = m_salary[col] - ( m_playersLeft[col] - 1 );
	m_average[col] = m_salary[col] / m_playersLeft[col];
	m_value[col] += player.GetValue();
	m_amtPaid[col] += player.GetAmountPaid();
	m_profit[col] += ( m_value[col] - m_amtPaid[col] );
	if( !justCalculate )
		DisplayOwnerData( col );
}

void CTeamsRoster::DisplayOwnerData(int column)
{
	m_roster->SetCellValue( 0, column, wxString::Format( "%d", m_salary[column] ) );
	m_roster->SetCellValue( 1, column, wxString::Format( "%d", m_maxBid[column] ) );
	m_roster->SetCellValue( 2, column, wxString::Format( "%.3f", m_average[column] ) );
	if( m_profit[column] != 0 )
		m_roster->SetCellValue( 3, column, wxString::Format( "%d", m_profit[column] ) );
}

void CTeamsRoster::ResetRoster()
{
	for( int i = 0; i < m_roster->GetNumberRows(); i++ )
		for( int j = 0; j < m_roster->GetNumberCols(); j++ )
			m_roster->SetCellValue( i, j, wxEmptyString );
	m_draftresult.clear();
}
