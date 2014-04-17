// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "sqlite3.h"
#include "wx/tipdlg.h"
#include "wx/grid.h"
#include "wx/tglbtn.h"
#include "wx/textcompleter.h"
#include "textctrl.h"
#include "playerspanelgrid.h"
#include "gridtotalcell.h"
#include "leaguesettings.h"
#include "namecompleter.h"
#include "playerspanel.h"
#include "teamsroster.h"
#include "teamprojections.h"
#include "draftresultsalary.h"
#include "draftresultpanel.h"
#include "draftresult.h"
#include "playerdraft.h"
#include "db.h"
#include "frame.h"


IMPLEMENT_DYNAMIC_CLASS(CPlayersPanel, wxPanel)

CPlayersPanel::CPlayersPanel() : wxPanel()
{
}

CPlayersPanel::CPlayersPanel(wxWindow *parent, const CLeagueData &data) : wxPanel( parent )
{
	m_oldSortingColumn = -1;
	wxString menu = "FULL";
	m_isResizing = false;
	for( std::vector<wxString>::iterator it = data.m_settings->GetHitScoring().begin(); it < data.m_settings->GetHitScoring().end() ; it++ )
		m_hitterColumns[(*it)] = true;
	for( std::vector<wxString>::iterator it = data.m_settings->GetPitchScoring().begin(); it < data.m_settings->GetPitchScoring().end() ; it++ )
		m_pitcherColumns[(*it)] = true;
	m_row = 0;
	m_columns["Notes"] = true;
	m_columns["BegValue"] = true;
	m_columns["CurrValue"] = true;
	m_columns["AmtPaid"] = true;
	m_columns["Profit"] = true;
	m_columns["Owner"] = true;
	m_columns["Age"] = false;
	m_data = &(const_cast<CLeagueData &>( data ));
	m_completer = new CNameCompleter( const_cast<std::vector<CPlayer> *>( m_data->m_players ), false );
	m_pitcherColumn = 0;
	m_filterType = 2;
	m_filterC = new wxToggleButton( this, wxID_ANY, "C", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_filter1B = new wxToggleButton( this, wxID_ANY, "1B", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_filter2B = new wxToggleButton( this, wxID_ANY, "2B", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_filterSS = new wxToggleButton( this, wxID_ANY, "SS", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_filter3B = new wxToggleButton( this, wxID_ANY, "3B", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_filterOF = new wxToggleButton( this, wxID_ANY, "OF", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_filterDH = new wxToggleButton( this, wxID_ANY, "DH", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_filterCI = new wxToggleButton( this, wxID_ANY, "CI", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_filterMI = new wxToggleButton( this, wxID_ANY, "MI", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_allHitters = new wxToggleButton( this, wxID_ANY, "All Hitters", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_allPlayers = new wxToggleButton( this, wxID_ANY, "All Players" );
	m_filterP = new wxToggleButton( this, wxID_ANY, "P", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_filterSP = new wxToggleButton( this, wxID_ANY, "SP", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_filterRP = new wxToggleButton( this, wxID_ANY, "RP", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_allPitchers = new wxToggleButton( this, wxID_ANY, "All Pitchers", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_label1 = new wxStaticText( this, wxID_ANY, "Search:" );
	m_playerSearch = new CTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB );
	m_playerSearch->AutoComplete( m_completer );
	m_label2 = new wxButton( this, wxID_ANY, "Top" );
	const wxString choices[] =
	{
		wxT( "Show Available Players" ),
		wxT( "Show Drafted Players" ),
		wxT( "Show All Players" )
	};
	m_typeFilter = new wxRadioBox( this, wxID_ANY, "Players Filter", wxDefaultPosition, wxDefaultSize, 3, choices, 0, wxRA_SPECIFY_COLS );
	m_edit = new wxButton( this, wxID_ANY, "Edit Columns" );
	m_add = new wxButton( this, wxMENU_FILE_ADD_PLAYER, "Add Player" );
	m_players = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_STATIC );
	m_players->HideRowLabels();
	set_properties();
	do_layout();
	m_filterC->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filter1B->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filter2B->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filterSS->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filter3B->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filterOF->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filterDH->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filterCI->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filterMI->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_allHitters->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterAllHitters, this );
	m_filterP->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterPitchers, this );
	m_filterSP->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterPitchers, this );
	m_filterRP->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterPitchers, this );
	m_allPitchers->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterAllPitchers, this );
	m_label2->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CPlayersPanel::OnFirstPlayer, this );
	m_add->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CFrame::OnAddNewPlayer, dynamic_cast<CFrame *>( GetParent()->GetParent() ) );
	m_players->Bind( wxEVT_GRID_CELL_RIGHT_CLICK, &CFrame::OnContextMenu, dynamic_cast<CFrame *>( GetParent()->GetParent() ) );
	m_players->Bind( wxEVT_GRID_COL_SORT, &CPlayersPanel::OnSortingData, this );
	m_edit->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CFrame::OnEditColumns, dynamic_cast<CFrame *>( GetParent()->GetParent() ) );
	m_playerSearch->Bind( wxEVT_COMMAND_TEXT_UPDATED, &CPlayersPanel::OnNameEnter, this );
	m_typeFilter->Bind( wxEVT_COMMAND_RADIOBOX_SELECTED, &CPlayersPanel::OnPlayersFilter, this );
	m_players->Bind( wxEVT_GRID_CELL_CHANGED, &CPlayersPanel::OnNotesChanged, this );
	m_allPlayers->Bind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnAllPlayers, this );
	m_players->Bind( wxEVT_GRID_COL_AUTO_SIZE, &CPlayersPanel::OnResizingColumn, this );
	m_players->SetRowMinimalAcceptableHeight( m_players->GetRowHeight( 0 ) );
}

CPlayersPanel::~CPlayersPanel(void)
{
	m_filterC->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filter1B->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filter2B->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filterSS->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filter3B->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filterOF->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filterDH->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filterCI->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_filterMI->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterHitters, this );
	m_allHitters->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterAllHitters, this );
	m_filterP->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterPitchers, this );
	m_filterSP->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterPitchers, this );
	m_filterRP->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterPitchers, this );
	m_allPitchers->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnFilterAllPitchers, this );
	m_label2->Unbind( wxEVT_COMMAND_BUTTON_CLICKED, &CPlayersPanel::OnFirstPlayer, this );
	m_players->Unbind( wxEVT_GRID_COL_SORT, &CPlayersPanel::OnSortingData, this );
	m_playerSearch->Unbind( wxEVT_COMMAND_TEXT_UPDATED, &CPlayersPanel::OnNameEnter, this );
	m_typeFilter->Unbind( wxEVT_COMMAND_RADIOBOX_SELECTED, &CPlayersPanel::OnPlayersFilter, this );
	m_players->Unbind( wxEVT_GRID_CELL_CHANGED, &CPlayersPanel::OnNotesChanged, this );
	m_allPlayers->Unbind( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &CPlayersPanel::OnAllPlayers, this );
}

void CPlayersPanel::set_properties()
{
	m_typeFilter->SetSelection( 2 );
	m_allPlayers->SetValue( true );
	if( m_data->m_settings->GetAmericanLeague().size() == 0 && m_data->m_settings->GetPositions()["DH"] == 0 )
		m_filterDH->Hide();
	m_players->CreateGrid( m_data->m_players->size(), 65, wxGrid::wxGridSelectRows );
	m_players->HideRowLabels();
	m_players->BeginBatch();
	m_players->SetColLabelValue( 0, "Rank" );
	m_players->SetColFormatNumber( 0 );
	m_players->SetColLabelValue( 1, "Player" );
	m_players->SetColLabelValue( 2, "Team" );
	m_players->SetColLabelValue( 3, "Position" );
	m_players->SetColLabelValue( 4, "Age" );
	m_players->SetColFormatNumber( 4 );
	m_players->SetColLabelValue( 5, "AVG" );
	m_players->SetColLabelValue( 6, "HR" );
	m_players->SetColFormatNumber( 6 );
	m_players->SetColLabelValue( 7, "Runs" );
	m_players->SetColFormatNumber( 7 );
	m_players->SetColLabelValue( 8, "RBI" );
	m_players->SetColFormatNumber( 8 );
	m_players->SetColLabelValue( 9, "SB" );
	m_players->SetColFormatNumber( 9 );
	m_players->SetColLabelValue( 10, "OBP" );
	m_players->SetColLabelValue( 11, "SLG" );
	m_players->SetColLabelValue( 12, "OPS" );
	m_players->SetColLabelValue( 13, "BB" );
	m_players->SetColFormatNumber( 13 );
	m_players->SetColLabelValue( 14, "H" );
	m_players->SetColFormatNumber( 14 );
	m_players->SetColLabelValue( 15, "K" );
	m_players->SetColFormatNumber( 15 );
	m_players->SetColLabelValue( 16, "1B" );
	m_players->SetColFormatNumber( 16 );
	m_players->SetColLabelValue( 17, "2B" );
	m_players->SetColFormatNumber( 17 );
	m_players->SetColLabelValue( 18, "3B" );
	m_players->SetColFormatNumber( 18 );
	m_players->SetColLabelValue( 19, "TB" );
	m_players->SetColFormatNumber( 19 );
	m_players->SetColLabelValue( 20, "2B+3B+HR" );
	m_players->SetColFormatNumber( 20 );
	m_players->SetColLabelValue( 21, "2B+3B" );
	m_players->SetColFormatNumber( 21 );
	m_players->SetColLabelValue( 22, "SB-CS" );
	m_players->SetColFormatNumber( 22 );
	m_players->SetColLabelValue( 23, "CS" );
	m_players->SetColFormatNumber( 23 );
	m_players->SetColLabelValue( 24, "R+RBI-HR" );
	m_players->SetColFormatNumber( 24 );
	m_players->SetColLabelValue( 25, "H+BB" );
	m_players->SetColFormatNumber( 25 );
	m_players->SetColLabelValue( 26, "TB+BB" );
	m_players->SetColFormatNumber( 26 );
	m_players->SetColLabelValue( 27, "AB" );
	m_players->SetColFormatNumber( 27 );
	m_players->SetColLabelValue( 28, "HBP" );
	m_players->SetColFormatNumber( 28 );
	m_players->SetColLabelValue( 29, "GDP" );
	m_players->SetColFormatNumber( 29 );
	m_players->SetColLabelValue( 30, "SF" );
	m_players->SetColFormatNumber( 30 );
	m_players->SetColLabelValue( 31, "BegValue" );
	m_players->SetColLabelValue( 32, "CurrValue" );
	m_players->SetColLabelValue( 33, "AmtPaid" );
	m_players->SetColLabelValue( 34, "Profit" );
	m_players->SetColLabelValue( 35, "Owner" );
	m_players->SetColLabelValue( 36, "Wins" );
	m_players->SetColFormatNumber( 36 );
	m_players->SetColLabelValue( 37, "Saves" );
	m_players->SetColFormatNumber( 37 );
	m_players->SetColLabelValue( 38, "ERA" );
	m_players->SetColLabelValue( 39, "WHIP" );
	m_players->SetColLabelValue( 40, "K" );
	m_players->SetColFormatNumber( 40 );
	m_players->SetColLabelValue( 41, "IP" );
	m_players->SetColFormatFloat( 41, -1, 1 );
	m_players->SetColLabelValue( 42, "L" );
	m_players->SetColFormatNumber( 42 );
	m_players->SetColLabelValue( 43, "H" );
	m_players->SetColFormatNumber( 43 );
	m_players->SetColLabelValue( 44, "BB" );
	m_players->SetColFormatNumber( 44 );
	m_players->SetColLabelValue( 45, "ER" );
	m_players->SetColFormatNumber( 45 );
	m_players->SetColLabelValue( 46, "CG" );
	m_players->SetColFormatNumber( 46 );
	m_players->SetColLabelValue( 47, "W-L" );
	m_players->SetColFormatNumber( 47 );
	m_players->SetColLabelValue( 48, "QS" );
	m_players->SetColFormatNumber( 48 );
	m_players->SetColLabelValue( 49, "Shutouts" );
	m_players->SetColFormatNumber( 49 );
	m_players->SetColLabelValue( 50, "BS" );
	m_players->SetColFormatNumber( 50 );
	m_players->SetColLabelValue( 51, "S-BS" );
	m_players->SetColFormatNumber( 51 );
	m_players->SetColLabelValue( 52, "S+Holds" );
	m_players->SetColFormatNumber( 52 );
	m_players->SetColLabelValue( 53, "S+Holds-BS" );
	m_players->SetColFormatNumber( 53 );
	m_players->SetColLabelValue( 54, "K/9" );
	m_players->SetColLabelValue( 55, "H/9" );
	m_players->SetColLabelValue( 56, "BB/9" );
	m_players->SetColLabelValue( 57, "K/BB" );
	m_players->SetColLabelValue( 58, "K-BB" );
	m_players->SetColLabelValue( 59, "W%" );
	m_players->SetColLabelValue( 60, "Holds" );
	m_players->SetColLabelValue( 61, "G" );
	m_players->SetColLabelValue( 62, "R" );
	m_players->SetColLabelValue( 63, "HR" );
	m_players->SetColLabelValue( 64, "Notes" );
	m_sort.m_type.push_back( SortObject( SORT_BY_CURRVALUE, true ) );
	m_sort.m_type.push_back( SortObject( SORT_BY_RANGE, true ) );
	m_presorted = true;
	int row = 0;
	for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
	{
		if( (*it).IsPlayerDeleted() )
			continue;
		wxString temp;
		wxFont font = m_players->GetCellFont( row, 0 );
		font.MakeBold();
		m_players->SetCellFont( row, 0, font );
		m_players->SetCellValue( row, 0, wxString::Format( "%d", (*it).GetRange() ) );
		m_players->SetCellValue( row, 1, (*it).GetName() );
		m_players->SetCellValue( row, 2, (*it).GetAbbeviatedTeamName() );
		for( std::vector<wxString>::iterator it_pos = (*it).GetPositions().begin(); it_pos < (*it).GetPositions().end(); it_pos++ )
		{
			temp += *it_pos;
			if( it_pos != (*it).GetPositions().end() - 1 )
				temp += ", ";
		}
		m_players->SetCellValue( row, 3, temp );
		m_players->SetCellValue( row, 4, wxString::Format( "%d", (*it).GetAge() ) );
		for( std::map<wxString,double>::iterator it_hitters = (*it).GetScoring().begin(); it_hitters != (*it).GetScoring().end(); it_hitters++ )
		{
			wxString score;
			if( (*it_hitters).first == "AVG" ||
				(*it_hitters).first == "OBP" ||
				(*it_hitters).first == "SLG" ||
				(*it_hitters).first == "OPS" ||
				(*it_hitters).first == "W%" )
			{
				score = wxString::Format( "%.3f", (*it_hitters).second );
			}
			else if( (*it_hitters).first == "ERA" ||
					 (*it_hitters).first == "WHIP" ||
					 (*it_hitters).first == "K/9" ||
					 (*it_hitters).first == "H/9" ||
					 (*it_hitters).first == "BB/9" ||
					 (*it_hitters).first == "K/BB" )
			{
				score = wxString::Format( "%.2f", (*it_hitters).second );
			}
			else if( (*it_hitters).first == "IP" )
				score = wxString::Format( "%.1f", (*it_hitters).second );
			else
				score << wxString::Format( "%d", (int)(*it_hitters).second );
			if( score.StartsWith( "0." ) )
				score = score.substr( 1 );
			int col = FindColumnByLabel( (*it_hitters).first, (*it).IsHitter() );
			m_players->SetCellValue( row, col, score );
		}
		m_players->SetCellValue( row, 31, wxString::Format( "$%.2f", (*it).GetValue() ) );
		m_players->SetCellValue( row, 32, wxString::Format( "$%.2f", (*it).GetCurrentValue() ) );
		if( (*it).IsPlayerDrafted() )
		{
			int paid = (*it).GetAmountPaid();
			m_players->SetCellValue( row, 33, paid == 0 ? wxString() : wxString::Format( "$%d", paid ) );
			double profit = (*it).GetValue() - (*it).GetAmountPaid();
			m_players->SetCellValue( row, 34, profit == 0 ? wxString() : wxString::Format( "$%d", (int) profit ) );
			m_players->SetCellValue( row, 35, (*it).GetOwner() );
		}
		m_players->SetCellValue( row, 64, (*it).GetNotes() );
		for( int i = 0; i < 64; i++ )
			m_players->SetReadOnly( row, i, true );
		row++;
		(*it).SetRange( row );
	}
	FilterColumns( true, true );
	m_players->DisableDragRowSize();
	m_players->EndBatch();
	for( int i = 0; i < m_players->GetNumberCols(); i++ )
		m_gridWidths[m_players->GetColLabelValue( i )] = m_players->GetColSize( i );
}

void CPlayersPanel::do_layout()
{
    wxBoxSizer* sizer_1 = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer* sizer_2 = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* sizer_3 = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* sizer_5 = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer* sizer_4 = new wxBoxSizer( wxHORIZONTAL );
    sizer_1->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_2->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_filterC, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_filter1B, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_filter2B, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_filterSS, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_filter3B, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_filterOF, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_filterDH, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_filterCI, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 20, 0, wxEXPAND, 0 );
    sizer_4->Add( m_filterMI, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_allHitters, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 1, wxEXPAND, 0 );
    sizer_4->Add( m_allPlayers, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 1, wxEXPAND, 0 );
    sizer_4->Add( m_filterP, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_filterSP, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_filterRP, 0, wxEXPAND, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_allPitchers, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL, 0 );
    sizer_3->Add( sizer_4, 0, wxEXPAND, 0);
    sizer_3->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_5->Add( m_label1, 0, wxALIGN_CENTER_VERTICAL, 0 );
    sizer_5->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_5->Add( m_playerSearch, 0, wxALIGN_CENTER_VERTICAL, 0 );
    sizer_5->Add( 5, 5, 1, wxEXPAND, 0 );
    sizer_5->Add( m_label2, 0, wxALIGN_CENTER_VERTICAL, 0 );
    sizer_5->Add( 5, 5, 1, wxEXPAND, 0 );
    sizer_5->Add( m_typeFilter, 0, wxALIGN_CENTER_VERTICAL, 0 );
    sizer_5->Add( 5, 5, 1, wxEXPAND, 0 );
    sizer_5->Add( m_edit, 0, wxALIGN_CENTER_VERTICAL, 0 );
    sizer_5->Add( 5, 5, 1, wxEXPAND, 0 );
    sizer_5->Add( m_add, 0, wxALIGN_CENTER_VERTICAL, 0 );
    sizer_3->Add( sizer_5, 0, wxEXPAND, 0 );
    sizer_3->Add( 5, 5, 0, wxEXPAND, 0 );

	wxBoxSizer *sizer_6 = new wxBoxSizer( wxHORIZONTAL );
	sizer_6->Add( m_players, 2, wxEXPAND, 0 );
	sizer_3->Add( sizer_6, 2, wxEXPAND, 0 );
//    sizer_3->Add( m_players, 1, wxEXPAND, 0 );


    sizer_2->Add( sizer_3, 0, wxEXPAND, 0 );
    sizer_2->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_1->Add( sizer_2, 0, wxEXPAND, 0 );
    sizer_1->Add( 5, 5, 0, wxEXPAND, 0 );
    SetSizer( sizer_1 );
    sizer_1->Fit( this );
}

void CPlayersPanel::OnFilterHitters(wxCommandEvent &event)
{
	int filter = 0;
	m_gridWidths.clear();
	std::vector<wxString> hitScoring = m_data->m_settings->GetHitScoring();
	std::vector<wxString>::iterator it_hitters, it_pitchers;
	if( !wxGetKeyState( WXK_CONTROL ) )
	{
		m_filterP->SetValue( false );
		m_filterSP->SetValue( false );
		m_filterRP->SetValue( false );
		m_filterC->SetValue( false );
		m_filter1B->SetValue( false );
		m_filter2B->SetValue( false );
		m_filterSS->SetValue( false );
		m_filter3B->SetValue( false );
		m_filterOF->SetValue( false );
		m_filterDH->SetValue( false );
		m_filterCI->SetValue( false );
		m_filterMI->SetValue( false );
		m_allHitters->SetValue( false );
		dynamic_cast<wxToggleButton *>( event.GetEventObject() )->SetValue( true );
	}
	if( m_filterC->GetValue() )
		filter |= BASEBALLDRAFT_C_FILTER;
	if( m_filter1B->GetValue() )
		filter |= BASEBALLDRAFT_1B_FILTER;
	if( m_filter2B->GetValue() )
		filter |= BASEBALLDRAFT_2B_FILTER;
	if( m_filterSS->GetValue() )
		filter |= BASEBALLDRAFT_SS_FILTER;
	if( m_filter3B->GetValue() )
		filter |= BASEBALLDRAFT_3B_FILTER;
	if( m_filterOF->GetValue() )
		filter |= BASEBALLDRAFT_OF_FILTER;
	if( m_filterDH->GetValue() )
		filter |= BASEBALLDRAFT_DH_FILTER;
	if( m_filterCI->GetValue() )
		filter |= BASEBALLDRAFT_CI_FILTER;
	if( m_filterMI->GetValue() )
		filter |= BASEBALLDRAFT_MI_FILTER;
	m_completer->SetPositionFilter( filter );
	m_players->BeginBatch();
	int row = 0;
	for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
	{
		if( (*it).IsPlayerDeleted() )
			continue;
		std::vector<wxString> positions = (*it).GetPositions();
		if( ( m_filterC->GetValue() && std::find( positions.begin(), positions.end(), "C" ) != positions.end() ) ||
			( m_filter1B->GetValue() && std::find( positions.begin(), positions.end(), "1B" ) != positions.end() ) ||
			( m_filter2B->GetValue() && std::find( positions.begin(), positions.end(), "2B" ) != positions.end() ) ||
			( m_filterSS->GetValue() && std::find( positions.begin(), positions.end(), "SS" ) != positions.end() ) ||
			( m_filter3B->GetValue() && std::find( positions.begin(), positions.end(), "3B" ) != positions.end() ) ||
			( m_filterOF->GetValue() && std::find( positions.begin(), positions.end(), "OF" ) != positions.end() ) ||
			( m_filterDH->GetValue() && std::find( positions.begin(), positions.end(), "DH" ) != positions.end() ) ||
			( m_filterCI->GetValue() && ( ( std::find( positions.begin(), positions.end(), "1B" ) != positions.end() || std::find( positions.begin(), positions.end(), "3B" ) != positions.end() ) ) ) ||
			( m_filterMI->GetValue() && ( ( std::find( positions.begin(), positions.end(), "2B" ) != positions.end() || std::find( positions.begin(), positions.end(), "SS" ) != positions.end() ) ) ) )
		{
			m_players->SetRowHeight( row, wxGRID_AUTOSIZE );
			m_players->AutoSizeRow( row );
		}
		else
			m_players->SetRowSize( row, 0 );
		row++;
	}
	FilterColumns( true, false );
	m_players->EndBatch();
	for( int i = 0; i < m_players->GetNumberCols(); i++ )
		m_gridWidths[m_players->GetColLabelValue( i )] = m_players->GetColSize( i );
}

wxGrid *CPlayersPanel::GetPlayersGrid() const
{
	return m_players;
}

void CPlayersPanel::OnFilterPitchers(wxCommandEvent &event)
{
	int filter;
	m_gridWidths.clear();
	std::vector<wxString> pitchScoring = m_data->m_settings->GetPitchScoring();
	std::vector<wxString>::iterator it_pitchers;
	if( !wxGetKeyState( WXK_CONTROL ) )
	{
		m_filterP->SetValue( false );
		m_filterSP->SetValue( false );
		m_filterRP->SetValue( false );
		m_filterC->SetValue( false );
		m_filter1B->SetValue( false );
		m_filter2B->SetValue( false );
		m_filterSS->SetValue( false );
		m_filter3B->SetValue( false );
		m_filterOF->SetValue( false );
		m_filterDH->SetValue( false );
		m_filterCI->SetValue( false );
		m_filterMI->SetValue( false );
		m_allHitters->SetValue( false );
		dynamic_cast<wxToggleButton *>( event.GetEventObject() )->SetValue( true );
	}
	if( m_filterP->GetValue() )
		filter |= BASEBALLDRAFT_P_FILTER;
	if( m_filterSP->GetValue() )
		filter |= BASEBALLDRAFT_SP_FILTER;
	if( m_filterRP->GetValue() )
		filter |= BASEBALLDRAFT_RP_FILTER;
	m_completer->SetPositionFilter( filter );
	m_players->BeginBatch();
	for( unsigned int i = 0; i < m_data->m_players->size(); i++ )
	{
		if( m_data->m_players->at( i ).IsPlayerDeleted() )
			continue;
		std::vector<wxString> positions = m_data->m_players->at( i ).GetPositions();
		if( ( m_filterP->GetValue() && std::find( positions.begin(), positions.end(), "P" ) != positions.end() ) ||
			( m_filterSP->GetValue() && std::find( positions.begin(), positions.end(), "SP" ) != positions.end() ) ||
			( m_filterRP->GetValue() && std::find( positions.begin(), positions.end(), "RP" ) != positions.end() ) )
		{
			m_players->SetRowHeight( i, wxGRID_AUTOSIZE );
			m_players->AutoSizeRow( i );
		}
		else
			m_players->SetRowSize( i, 0 );
	}
	FilterColumns( false, true );
	m_players->EndBatch();
	for( int i = 0; i < m_players->GetNumberCols(); i++ )
		m_gridWidths[m_players->GetColLabelValue( i )] = m_players->GetColSize( i );
}

void CPlayersPanel::DisplayPlayer(const CPlayer &player, bool addRow)
{
	bool isFirst = true;
	m_players->BeginBatch();
	int row;
	if( addRow )
	{
		m_players->AppendRows();
		row = m_players->GetNumberRows() - 1;
	}
	else
	{
		row = m_row;
	}
	wxString temp;
	wxFont font = m_players->GetCellFont( row, 0 );
	font.MakeBold();
	m_players->SetCellFont( row, 0, font );
	m_players->SetCellValue( row, 0, wxString::Format( "%d", const_cast<CPlayer &>( player ).GetRange() ) );
	m_players->SetCellValue( row, 1, const_cast<CPlayer &>( player ).GetName() );
	m_players->SetCellValue( row, 2, const_cast<CPlayer &>( player ).GetAbbeviatedTeamName() );
	if( player.IsPlayerDrafted() )
		m_players->SetCellValue( row, 3, const_cast<CPlayer &>( player ).GetDraftedPosition() );
	else
	{
		for( std::vector<wxString>::iterator it_pos = const_cast<CPlayer &>( player ).GetPositions().begin(); it_pos < const_cast<CPlayer &>( player ).GetPositions().end(); it_pos++ )
		{
			temp += *it_pos;
			if( it_pos != const_cast<CPlayer &>( player ).GetPositions().end() - 1 )
				temp += ", ";
		}
		m_players->SetCellValue( row, 3, temp );
	}
	m_players->SetCellValue( row, 4, wxString::Format( "%d", player.GetAge() ) );
	for( std::map<wxString,double>::iterator it_hitters = const_cast<CPlayer &>( player ).GetScoring().begin(); it_hitters != const_cast<CPlayer &>( player ).GetScoring().end(); it_hitters++ )
	{
		if( !player.IsHitter() && isFirst )
		{
			for( int col = 5; col < 31; col++ )
				m_players->SetCellValue( row, col, wxEmptyString );
		}
		else if( isFirst )
		{
			for( int col = 36; col < 64; col++ )
				m_players->SetCellValue( row, col, wxEmptyString );
		}
		isFirst = false;
		wxString score;
		if( (*it_hitters).first == "AVG" ||
			(*it_hitters).first == "OBP" ||
			(*it_hitters).first == "SLG" ||
			(*it_hitters).first == "OPS" ||
			(*it_hitters).first == "W%" )
		{
			score = wxString::Format( "%.3f", (*it_hitters).second );
		}
		else if( (*it_hitters).first == "ERA" ||
				 (*it_hitters).first == "WHIP" ||
				 (*it_hitters).first == "K/9" ||
				 (*it_hitters).first == "H/9" ||
				 (*it_hitters).first == "BB/9" ||
				 (*it_hitters).first == "K/BB" )
		{
			score = wxString::Format( "%.2f", (*it_hitters).second );
		}
		else if( (*it_hitters).first == "IP" )
			score = wxString::Format( "%.1f", (*it_hitters).second );
		else
			score = wxString::Format( "%d", (int) (*it_hitters).second );
		if( score.StartsWith( "0." ) )
			score = score.substr( 1 );
		int col = FindColumnByLabel( (*it_hitters).first, player.IsHitter() );
		m_players->SetCellValue( row, col, score );
	}
	double value = player.GetValue();
	double currValue = player.GetCurrentValue();
	m_players->SetCellValue( row, 31, wxString::Format( "$%.2f", value ) );
	m_players->SetCellValue( row, 32, wxString::Format( "$%.2f", floor( currValue * 100 + 0.5 ) / 100 ) );
	if( player.IsPlayerDrafted() )
	{
		int paid = player.GetAmountPaid();
		m_players->SetCellValue( row, 33, paid == 0 ? wxString() : wxString::Format( "$%d", paid ) );
		m_players->SetCellValue( row, 35, const_cast<CPlayer &>( player ).GetOwner() );
		double profit = player.GetValue() - player.GetAmountPaid();
		m_players->SetCellValue( row, 34, profit == 0 ? wxString() : wxString::Format( "$%.2f", profit ) );
	}
	else
	{
		m_players->SetCellValue( row, 33, wxEmptyString );
		m_players->SetCellValue( row, 34, wxEmptyString );
		m_players->SetCellValue( row, 35, wxEmptyString );
	}
	m_players->SetCellValue( row, 64, const_cast<CPlayer &>( player ).GetNotes() );
	for( int i = 0; i < 64; i++ )
		m_players->SetReadOnly( row, i, true );
	m_row++;
	m_players->EndBatch();
}

void CPlayersPanel::OnFirstPlayer(wxCommandEvent &WXUNUSED(event))
{
	m_players->MakeCellVisible( 0, 0 );
}

wxTextCtrl &CPlayersPanel::GetPlayerNameCtrl()
{
	return *m_playerSearch;
}

void CPlayersPanel::OnFilterAllHitters(wxCommandEvent &WXUNUSED(event))
{
	wxBeginBusyCursor();
	m_gridWidths.clear();
	m_filterC->SetValue( false );
	m_filter1B->SetValue( false );
	m_filter2B->SetValue( false );
	m_filterSS->SetValue( false );
	m_filter3B->SetValue( false );
	m_filterOF->SetValue( false );
	m_filterDH->SetValue( false );
	m_filterCI->SetValue( false );
	m_filterMI->SetValue( false );
	m_filterP->SetValue( false );
	m_filterSP->SetValue( false );
	m_filterRP->SetValue( false );
	m_allPlayers->SetValue( false );
	m_allPitchers->SetValue( false );
	m_completer->SetFullPositionFilter( "All Hitters" );
	m_completer->SetPositionFilter( 0 );
	m_completer->SetPositionFilter( 0 );
	m_players->BeginBatch();
	for( unsigned int i = 0; i < m_data->m_players->size(); i++ )
	{
		if( m_data->m_players->at( i ).IsPlayerDeleted() )
			continue;
		if( !m_data->m_players->at( i ).IsHitter() )
			m_players->HideRow( i );
		else
		{
			m_players->SetRowHeight( i, wxGRID_AUTOSIZE );
			m_players->AutoSizeRow( i );
		}
	}
	m_players->EndBatch();
	for( int i = 0; i < m_players->GetNumberCols(); i++ )
		m_gridWidths[m_players->GetColLabelValue( i )] = m_players->GetColSize( i );
	wxEndBusyCursor();
}

void CPlayersPanel::OnFilterAllPitchers(wxCommandEvent &WXUNUSED(event))
{
	wxBeginBusyCursor();
	m_gridWidths.clear();
	m_filterP->SetValue( false );
	m_filterSP->SetValue( false );
	m_filterRP->SetValue( false );
	m_filterC->SetValue( false );
	m_filter1B->SetValue( false );
	m_filter2B->SetValue( false );
	m_filterSS->SetValue( false );
	m_filter3B->SetValue( false );
	m_filterOF->SetValue( false );
	m_filterDH->SetValue( false );
	m_filterCI->SetValue( false );
	m_filterMI->SetValue( false );
	m_allHitters->SetValue( false );
	m_completer->SetFullPositionFilter( "All Pitchers" );
	m_completer->SetPositionFilter( 0 );
	m_players->BeginBatch();
	for( unsigned int i = 0; i < m_data->m_players->size(); i++ )
	{
		if( m_data->m_players->at( i ).IsPlayerDeleted() )
			continue;
		if( m_data->m_players->at( i ).IsHitter() )
			m_players->HideRow( i );
		else
		{
			m_players->SetRowHeight( i, wxGRID_AUTOSIZE );
			m_players->AutoSizeRow( i );
		}
	}
	m_players->EndBatch();
	for( int i = 0; i < m_players->GetNumberCols(); i++ )
		m_gridWidths[m_players->GetColLabelValue( i )] = m_players->GetColSize( i );
	wxEndBusyCursor();
}

void CPlayersPanel::OnNameEnter(wxCommandEvent &WXUNUSED(event))
{
	bool found = false;
	wxString name = m_playerSearch->GetValue();
	if( !name.IsEmpty() )
	{
		CapitalizeName( name );
		int rows = m_players->GetNumberRows();
		for( int i = 0; i < rows && !found; i++ )
		{
			if( m_players->GetCellValue( i, 1 ) == name )
			{
				found = true;
				m_players->MakeCellVisible( i < 4 ? 0 : i + 3 > rows ? i : m_selection < i ? i + 3 : i - 3, 0 );
				m_players->SelectRow( i );
				m_selection = i;
				CFrame *parent = (CFrame *) GetParent()->GetParent();
				CPlayerDraft *draftPanel = parent->GetPlayerDraft();
				draftPanel->GetPlayerNameCtrl().SetValue( name );
			}
		}
	}
}

void CPlayersPanel::OnPlayersFilter(wxCommandEvent &WXUNUSED(event))
{
	wxBeginBusyCursor();
	m_filterType = m_typeFilter->GetSelection();
	m_completer->SetGeneralFilter( m_filterType );
	int row = 0;
	for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
	{
		if( (*it).IsPlayerDeleted() )
			continue;
		if( ( m_filterType == 0 || m_filterType == 2 ) && !(*it).IsPlayerDrafted() )
			m_players->SetRowSize( row, wxGRID_AUTOSIZE );
		else if( ( m_filterType == 1 || m_filterType == 2 ) && (*it).IsPlayerDrafted() )
			m_players->SetRowSize( row, wxGRID_AUTOSIZE );
		else
			m_players->SetRowSize( row, 0 );
		row++;
	}
	wxEndBusyCursor();
}

void CPlayersPanel::ColumnsChanged(const std::map<wxString,bool> &hitterColumns, const std::map<wxString,bool> &pitcherColumns, const std::map<wxString,bool> &columns)
{
	wxBeginBusyCursor();
	m_gridWidths.clear();
	m_hitterColumns = hitterColumns;
	m_pitcherColumns = pitcherColumns;
	m_columns = columns;
	m_players->BeginBatch();
	FilterColumns( true, true );
	m_players->EndBatch();
	for( int i = 0; i < m_players->GetNumberCols(); i++ )
		m_gridWidths[m_players->GetColLabelValue( i )] = m_players->GetColSize( i );
	wxEndBusyCursor();
}

void CPlayersPanel::OnSortingData(wxGridEvent &event)
{
	std::vector<SortObject>::iterator it = m_sort.m_type.end();
	int col = event.GetCol();
	if( col == 32 && m_presorted )
	{
		m_presorted = false;
		m_oldSortingColumn = 31;
		return;
	}
	if( m_presorted )
	{
		m_presorted = false;
		m_sort.m_type.clear();
	}
	if( m_isResizing )
	{
		m_isResizing = false;
		return;
	}
	wxBeginBusyCursor();
	wxString label = m_players->GetColLabelValue( event.GetCol() );
	if( !event.ControlDown() && m_oldSortingColumn != col )
		m_sort.m_type.clear();
	if( m_oldSortingColumn == col )
		if( m_sort.m_type.size() == 1 )
			m_sort.m_type.at( 0 ).m_direction = !m_sort.m_type.at( 0 ).m_direction;
		else
			m_sort.m_type.at( m_sort.m_type.size() - 2 ).m_direction = !m_sort.m_type.at( m_sort.m_type.size() - 2 ).m_direction;
	else
	{
		if( label == "Rank" )
		{
			if( m_sort.m_type.size() == 0 )
			{
				m_sort.m_type.push_back( SortObject( SORT_BY_RANGE, true ) );
			}
			else
				return;
		}
		else if( label == "Player" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_NAME, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_NAME, true ) );
		}
		else if( label == "Team" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_TEAM, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_TEAM, true ) );
		}
		else if( label == "Position" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_POSITION, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_POSITION, true ) );
		}
		else if( label == "Notes" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_NOTES, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_NOTES, true ) );
		}
		else if( label == "K" )
		{
			if( event.GetCol() == 15 )
			{
				if( m_sort.m_type.size() == 0 )
					m_sort.m_type.push_back( SortObject( SORT_BY_KHITTERS, true ) );
				else
					m_sort.m_type.insert( it - 1, SortObject( SORT_BY_KHITTERS, true ) );
			}
			else
			{
				if( m_sort.m_type.size() == 0 )
					m_sort.m_type.push_back( SortObject( SORT_BY_KPITCHERS, true ) );
				else
					m_sort.m_type.insert( it - 1, SortObject( SORT_BY_KPITCHERS, true ) );
			}
		}
		else if( label == "H" )
		{
			if( event.GetCol() == 14 )
			{
				if( m_sort.m_type.size() == 0 )
					m_sort.m_type.push_back( SortObject( SORT_BY_HHITTERS, true ) );
				else
					m_sort.m_type.insert( it - 1, SortObject( SORT_BY_HHITTERS, true ) );
			}
			else
			{
				if( m_sort.m_type.size() == 0 )
					m_sort.m_type.push_back( SortObject( SORT_BY_HPITCHERS, true ) );
				else
					m_sort.m_type.insert( it - 1, SortObject( SORT_BY_HPITCHERS, true ) );
			}
		}
		else if( label == "Runs" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_RUNSHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_RUNSHITTERS, true ) );
		}
		else if( label == "R" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_RPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_RPITCHERS, true ) );
		}
		else if( label == "HR" )
		{
			if( event.GetCol() == 6 )
			{
				if( m_sort.m_type.size() == 0 )
					m_sort.m_type.push_back( SortObject( SORT_BY_HRHITTERS, true ) );
				else
					m_sort.m_type.insert( it - 1, SortObject( SORT_BY_HRHITTERS, true ) );
			}
			else
			{
				if( m_sort.m_type.size() == 0 )
					m_sort.m_type.push_back( SortObject( SORT_BY_HRPITCHERS, true ) );
				else
					m_sort.m_type.insert( it - 1, SortObject( SORT_BY_HRPITCHERS, true ) );
			}
		}
		else if( label == "BB" )
		{
			if( event.GetCol() == 13 )
			{
				if( m_sort.m_type.size() == 0 )
					m_sort.m_type.push_back( SortObject( SORT_BY_BBHITTERS, true ) );
				else
					m_sort.m_type.insert( it - 1, SortObject( SORT_BY_BBHITTERS, true ) );
			}
			else
			{
				if( m_sort.m_type.size() == 0 )
					m_sort.m_type.push_back( SortObject( SORT_BY_BBPITCHERS, true ) );
				else
					m_sort.m_type.insert( it - 1, SortObject( SORT_BY_BBPITCHERS, true ) );
			}
		}
		else if( label == "AVG" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_AVGHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_AVGHITTERS, true ) );
		}
		else if( label == "OBP" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_OBPHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_OBPHITTERS, true ) );
		}
		else if( label == "SLG" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_SLGHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_SLGHITTERS, true ) );
		}
		else if( label == "OPS" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_OPSHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_OPSHITTERS, true ) );
		}
		else if( label == "AB" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_ABHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_ABHITTERS, true ) );
		}
		else if( label == "1B" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_1BHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_1BHITTERS, true ) );
		}
		else if( label == "2B" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_2BHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_2BHITTERS, true ) );
		}
		else if( label == "3B" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_3BHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_3BHITTERS, true ) );
		}
		else if( label == "TB" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_TBHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_TBHITTERS, true ) );
		}
		else if( label == "RBI" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_RBIHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_RBIHITTERS, true ) );
		}
		else if( label == "SB" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_SBHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_SBHITTERS, true ) );
		}
		else if( label == "2B+3B+HR" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_2B3BHRHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_2B3BHRHITTERS, true ) );
		}
		else if( label == "2B+3B" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_2B3BHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_2B3BHITTERS, true ) );
		}
		else if( label == "SB-CS" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_SBCSHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_SBCSHITTERS, true ) );
		}
		else if( label == "CS" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_CSHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_CSHITTERS, true ) );
		}
		else if( label == "R+RBI-HR" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_RRBIHRHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_RRBIHRHITTERS, true ) );
		}
		else if( label == "H+BB" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_HBBHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_HBBHITTERS, true ) );
		}
		else if( label == "TB+BB" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_TBBBHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_TBBBHITTERS, true ) );
		}
		else if( label == "HBP" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_HBPHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_HBPHITTERS, true ) );
		}
		else if( label == "GDP" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_GDPHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_GDPHITTERS, true ) );
		}
		else if( label == "SF" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_SFHITTERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_SFHITTERS, true ) );
		}
		else if( label == "BegValue" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_VALUE, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_VALUE, true ) );
		}
		else if( label == "CurrValue" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_CURRVALUE, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_CURRVALUE, true ) );
		}
		else if( label == "AmtPaid" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_AMTPAID, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_AMTPAID, true ) );
		}
		else if( label == "Profit" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_PROFIT, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_PROFIT, true ) );
		}
		else if( label == "Owner" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_OWNER, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_OWNER, true ) );
		}
		else if( label == "Wins" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_WPITCHERS, true ) );
			else
				m_sort.m_type.insert( it -1, SortObject( SORT_BY_WPITCHERS, true ) );
		}
		else if( label == "Saves" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_SPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_SPITCHERS, true ) );
		}
		else if( label == "ERA" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_ERAPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_ERAPITCHERS, true ) );
		}
		else if( label == "WHIP" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_WHIPPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_WHIPPITCHERS, true ) );
		}
		else if( label == "IP" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_IPPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_IPPITCHERS, true ) );
		}
		else if( label == "L" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_LPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_LPITCHERS, true ) );
		}
		else if( label == "ER" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_ERPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_ERPITCHERS, true ) );
		}
		else if( label == "CG" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_CGPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_CGPITCHERS, true ) );
		}
		else if( label == "W-L" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_WLPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_WLPITCHERS, true ) );
		}
		else if( label == "QS" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_QSPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_QSPITCHERS, true ) );
		}
		else if( label == "Shoutouts" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_SHUTOUTPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_SHUTOUTPITCHERS, true ) );
		}
		else if( label == "BS" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_BSPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_BSPITCHERS, true ) );
		}
		else if( label == "S-BS" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_SBSPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_SBSPITCHERS, true ) );
		}
		else if( label == "S+Holds" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_SHOLDSPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_SHOLDSPITCHERS, true ) );
		}
		else if( label == "S+Holds-BS" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_SHOLDSBSPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_SHOLDSBSPITCHERS, true ) );
		}
		else if( label == "K/9" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_K9PITCHERS, true ) );
			else
				m_sort.m_type.insert( it -1, SortObject( SORT_BY_K9PITCHERS, true ) );
		}
		else if( label == "H/9" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_H9PITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_H9PITCHERS, true ) );
		}
		else if( label == "BB/9" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_BB9PITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_BB9PITCHERS, true ) );
		}
		else if( label == "K/BB" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_KBBPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_KBBPITCHERS, true ) );
		}
		else if( label == "K-BB" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_KMINBBPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_KMINBBPITCHERS, true ) );
		}
		else if( label == "W%" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_W100PITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_W100PITCHERS, true ) );
		}
		else if( label == "Holds" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_HOLDSPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_HOLDSPITCHERS, true ) );
		}
		else if( label == "G" )
		{
			if( m_sort.m_type.size() == 0 )
				m_sort.m_type.push_back( SortObject( SORT_BY_GPITCHERS, true ) );
			else
				m_sort.m_type.insert( it - 1, SortObject( SORT_BY_GPITCHERS, true ) );
		}
	}
	if( m_sort.m_type.at( m_sort.m_type.size() - 1 ).m_type != SORT_BY_RANGE )
		m_sort.m_type.push_back( SortObject( SORT_BY_RANGE, true ) );
	std::sort( m_data->m_players->begin(), m_data->m_players->end(), m_sort );
	m_players->BeginBatch();
	m_row = 0;
	for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
		GeneralPlayerFilter( (*it), false );
	FilterColumns( true, true );
	Layout();
	m_players->EndBatch();
	m_oldSortingColumn = col;
	m_row = 0;
	wxEndBusyCursor();
}

void CPlayersPanel::GeneralPlayerFilter(const CPlayer &player, bool addRow)
{
	if( ( m_typeFilter->GetSelection() == SHOW_AVAILABLE_PLAYERS && player.IsPlayerDrafted() ) || ( m_typeFilter->GetSelection() == SHOW_DRAFTED_PLAYERS && !player.IsPlayerDrafted() ) )
		return;
	PositionPlayerFilter( player, addRow );
}

void CPlayersPanel::PositionPlayerFilter(const CPlayer &player, bool addRow)
{
	if( player.IsHitter() &&
		( m_filterP->GetValue() ||
		  m_filterSP->GetValue() ||
		  m_filterRP->GetValue() ||
		  m_allPitchers->GetValue() ) )
		return;
	if( !player.IsHitter() && 
	  ( m_filterC->GetValue() ||
	    m_filter1B->GetValue() ||
		m_filter2B->GetValue() ||
		m_filterSS->GetValue() ||
		m_filter3B->GetValue() ||
		m_filterOF->GetValue() ||
		m_filterDH->GetValue() ||
		m_filterCI->GetValue() ||
		m_filterMI->GetValue() ||
		m_allHitters->GetValue() ) )
		return;
	if( player.IsHitter() )
	{
		if( !m_filterC->GetValue() &&
			!m_filter1B->GetValue() &&
			!m_filter2B->GetValue() &&
			!m_filterSS->GetValue() &&
			!m_filter3B->GetValue() &&
			!m_filterOF->GetValue() &&
			!m_filterDH->GetValue() &&
			!m_filterCI->GetValue() &&
			!m_filterMI->GetValue() )
			m_filter = true;
		else
		{
			m_filter = m_filterC->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "C" : false;
			bool temp = m_filter1B->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "1B" : false;
			m_filter = m_filter || temp;
			temp = m_filter2B->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "2B" : false;
			m_filter = m_filter || temp;
			temp = m_filterSS->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "SS" : false;
			m_filter = m_filter || temp;
			temp = m_filter3B->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "3B" : false;
			m_filter = m_filter || temp;
			temp = m_filterOF->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "OF" : false;
			m_filter = m_filter || temp;
			temp = m_filterDH->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "DH" : false;
			m_filter = m_filter || temp;
			temp = m_filterCI->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "1B" || const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "3B" : false;
			m_filter = m_filter || temp;
			temp = m_filterMI->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "2B" || const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "SS" : false;
			m_filter = m_filter || temp;
		}
	}
	else
	{
		if( !m_filterP->GetValue() &&
			!m_filterSP->GetValue() &&
			!m_filterRP->GetValue() )
			m_filter = true;
		else
		{
			m_filter = m_filterP->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "P" : false;
			bool temp = m_filterSP->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "SP" : false;
			m_filter = m_filter || temp;
			temp = m_filterRP->GetValue() ? const_cast<CPlayer &>( player ).GetPositions().at( 0 ) == "RP" : false;
			m_filter = m_filter || temp;
		}
	}
	if( !m_filter )
		return;
	DisplayPlayer( player, addRow );
}

/*
If drafting payer:
diff - inflation ratio
isEdit - true
player - NULL
playerDropped - ""
val - 0.0
*/
void CPlayersPanel::RecalculatePlayersValue(double diff, bool isEdit, const CPlayer *player, const wxString &playerDropped, double val)
{
	int row = 0;
	if( !player && isEdit )
	{
		double lessThanOneDollar = 0.0;
		int countLessThanOneDollar = 0;
		for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
		{
			if( (*it).IsPlayerDeleted() || (*it).IsPlayerDrafted() )
				continue;
			(*it).SetCurrentValue( diff * (*it).GetValue() );
			double currValue = (*it).GetCurrentValue();
			if( currValue > 0 && currValue < 1 )
			{
				lessThanOneDollar += currValue;
				countLessThanOneDollar++;
			}
		}
		if( countLessThanOneDollar > 0 )
		{
			double newdiff = lessThanOneDollar / (double) ( dynamic_cast<CFrame *>(GetParent()->GetParent())->GetAvailablePlayers() - countLessThanOneDollar - 1 );
			for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
			{
				if( (*it).IsPlayerDeleted() || (*it).IsPlayerDrafted() || (*it).GetValue() == 0 )
					continue;
				if( (*it).GetCurrentValue() > 1 )
					(*it).SetCurrentValue( (*it).GetCurrentValue() - newdiff );
				if( (*it).GetCurrentValue() < 1 )
					(*it).SetCurrentValue( 1.0 );
			}
		}
		PlayerSorter sorter;
		sorter.m_type.push_back( SortObject( SORT_BY_CURRVALUE, true ) );
		sorter.m_type.push_back( SortObject( SORT_BY_RANGE, true ) );
		std::sort( m_data->m_players->begin(), m_data->m_players->end(), sorter );
		int range = 1;
		for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
			(*it).SetRange( range++ );
		std::sort( m_data->m_players->begin(), m_data->m_players->end(), m_sort );
		SetCurrentPlayerRow( 0 );
		std::sort( m_data->m_players->begin(), m_data->m_players->end(), m_sort );
		for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++, row++ )
			DisplayPlayer( (*it), false );
		return;
	}
	if( isEdit )
	{
		SetCurrentPlayerRow( 0 );
		std::sort( m_data->m_players->begin(), m_data->m_players->end(), m_sort );
		for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++, row++ )
			DisplayPlayer( (*it), false );
	}
	else if( !isEdit )
	{
		CPlayer newPlayer( const_cast<CPlayer *>( player )->GetPlayerId(), const_cast<CPlayer *>( player )->GetName(), const_cast<CPlayer *>( player )->GetPositions(), player->GetAge(), player->GetValue(), const_cast<CPlayer *>( player )->GetTeam(), const_cast<CPlayer *>( player )->GetAbbeviatedTeamName(), const_cast<CPlayer *>( player )->GetScoring(), player->IsHitter(), player->GetCurrentValue(), const_cast<CPlayer *>( player )->GetNotes(), player->GetValue(), player->IsPlayerDeleted() );
/*		newPlayer.SetRange( const_cast<CPlayer *>( player )->GetRange() );
		newPlayer.SetOriginalRange( const_cast<CPlayer *>( player )->GetOriginalRange() );*/
		newPlayer.SetNewPlayer( true );
/*		int newPlayerRank = const_cast<CPlayer *>( player )->GetRange();
		m_data->m_players->push_back( newPlayer );*/
		int changedRank;
		if( newPlayer.GetValue() == 0 )
		{
			int pos = m_data->m_players->size() + 1;
			for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
			{
				if( (*it).IsPlayerDeleted() )
					pos--;
			}
			changedRank = pos;
			newPlayer.SetRange( changedRank );
			newPlayer.SetOriginalRange( changedRank );
			int newPlayerRank = const_cast<CPlayer *>( player )->GetRange();
			m_data->m_players->push_back( newPlayer );
			m_players->BeginBatch();
			m_players->AppendRows();
			m_row = m_players->GetNumberRows() - 1;
			DisplayPlayer( newPlayer, false );
			m_players->MakeCellVisible( m_row - 2, 0 );
			m_players->SelectRow( m_row - 1 );
			m_players->EndBatch();
			m_row = 0;
		}
		else
		{
			double amount = 0.0;
			int counter = 0;
			for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
			{
				int playerRange = (*it).GetRange();
				if( (*it).IsPlayerDeleted() )
					continue;
				if( !playerDropped.IsEmpty() && (*it).GetName() == playerDropped )
				{
					(*it).SetCurrentValue( 0.0 );
					(*it).SetValue( 0 );
				}
//				if( (*it).GetName() == const_cast<CPlayer *>( player )->GetName() && playerRange == newPlayerRank )
//					continue;
				if( !(*it).IsPlayerDrafted() && val > 1 && (*it).GetCurrentValue() > 0 )
				{
					(*it).SetCurrentValue( (*it).GetCurrentValue() + diff );
					double currValue = (*it).GetCurrentValue();
					if( currValue < 1.0 )
					{
						amount += ( 1 - currValue );
						counter++;
					}
				}
			}
			double newdiff = amount / (double) ( dynamic_cast<CFrame *>(GetParent()->GetParent())->GetAvailablePlayers() - counter - 1 );
			for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
			{
				if( (*it).IsPlayerDeleted() || (*it).GetValue() == 0 )
					continue;
				if( (*it).GetCurrentValue() > 1 && ( (*it).GetName() != const_cast<CPlayer *>( player )->GetName() && ( (*it).GetAge() != player->GetAge() ) ) )
					(*it).SetCurrentValue( (*it).GetCurrentValue() - newdiff );
				if( (*it).GetCurrentValue() < 1 )
					(*it).SetCurrentValue( 1.0 );
			}
			bool found = false;
			for( std::vector<CPlayer>::reverse_iterator it = m_data->m_players->rbegin(); it != m_data->m_players->rend() && !found; it++ )
			{
				if( (*it).IsPlayerDeleted() )
					continue;
				if( (*it).GetCurrentValue() == val )
				{
					changedRank = (*it).GetRange() + 1;
					found = true;
				}
				if( (*it).GetCurrentValue() > val )
				{
					changedRank = (*it).GetRange() + 1;
					found = true;
				}
			}
			newPlayer.SetRange( changedRank );
			newPlayer.SetOriginalRange( changedRank );
			int currentRank = m_data->m_players->size() + 1/*, changedRank = dlg.GetChangedRank()*/;
			if( currentRank > changedRank )
			{
				for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
				{
					if( (*it).IsPlayerDeleted() )
					{
						currentRank--;
						continue;
					}
					int range = (*it).GetRange();
					if( range >= changedRank && range < currentRank )
						(*it).SetRange( range + 1 );
				}
			}
			m_data->m_players->push_back( newPlayer );
			found = false;
			std::sort( m_data->m_players->begin(), m_data->m_players->end(), m_sort );
			m_players->BeginBatch();
			m_players->AppendRows();
			int row = 0;
			for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
			{
				if( (*it).IsPlayerDeleted() )
					continue;
				GeneralPlayerFilter( (*it), false );
				if( (*it).IsPlayerDrafted() )
					m_players->HideRow( row );
				row++;
			}
			found = false;
			int count = 0;
			for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end() && !found; it++ )
			{
				if( (*it).IsNewPlayer() && !(*it).IsPlayerDeleted() && (*it).GetName() == const_cast<CPlayer *>( player )->GetName() && (*it).GetAge() == player->GetAge() && (*it).GetTeam() == const_cast<CPlayer *>( player )->GetTeam() )
					found = true;
				else
					count++;
			}
			m_players->MakeCellVisible( count, 0 );
			m_players->SelectRow( count );
			m_row = 0;
			m_players->EndBatch();
		}
		PlayerSorter sorter;
		GetCompleter().GetPlayers().push_back( newPlayer );
		sorter.m_type.push_back( SortObject( SORT_BY_NAME, true ) );
		std::sort( GetCompleter().GetPlayers().begin(), GetCompleter().GetPlayers().end(), sorter );
	}
}

void CPlayersPanel::OnNotesChanged(wxGridEvent &event)
{
	if( m_players->GetColLabelValue( event.GetCol() ) == "Notes" )
	{
		m_data->m_players->at( event.GetRow() ).SetNotes( m_players->GetCellValue( event.GetRow(), event.GetCol() ) );
	}
}

void CPlayersPanel::OnAllPlayers(wxCommandEvent &WXUNUSED(event))
{
	if( !m_filterC->GetValue() &&
	    !m_filter1B->GetValue() &&
		!m_filter2B->GetValue() &&
		!m_filter3B->GetValue() &&
		!m_filterSS->GetValue() &&
		!m_filterOF->GetValue() &&
		!m_filterDH->GetValue() &&
		!m_filterCI->GetValue() &&
		!m_filterMI->GetValue() &&
		!m_allHitters->GetValue() &&
		!m_filterP->GetValue() &&
		!m_filterSP->GetValue() &&
		!m_filterRP->GetValue() &&
		!m_allPitchers->GetValue() )
	{
		m_allPlayers->SetValue( true );
		return;
	}
	else
	{
		wxBeginBusyCursor();
		m_gridWidths.clear();
		m_filterC->SetValue( false );
	    m_filter1B->SetValue( false );
		m_filter2B->SetValue( false );
		m_filter3B->SetValue( false );
		m_filterSS->SetValue( false );
		m_filterOF->SetValue( false );
		m_filterDH->SetValue( false );
		m_filterCI->SetValue( false );
		m_filterMI->SetValue( false );
		m_allHitters->SetValue( false );
		m_filterP->SetValue( false );
		m_filterSP->SetValue( false );
		m_filterRP->SetValue( false );
		m_allPitchers->SetValue( false );
		m_completer->SetFullPositionFilter( wxEmptyString );
		m_players->BeginBatch();
		for( unsigned int i = 0; i < m_data->m_players->size(); i++ )
		{
			if( m_data->m_players->at( i ).IsPlayerDeleted() )
				continue;
			m_players->SetRowSize( i, wxGRID_AUTOSIZE );
			m_players->AutoSizeRow( i );
		}
		m_players->EndBatch();
	}
	for( int i = 0; i < m_players->GetNumberCols(); i++ )
		m_gridWidths[m_players->GetColLabelValue( i )] = m_players->GetColSize( i );
	wxEndBusyCursor();
}

void CPlayersPanel::OnResizingColumn(wxGridSizeEvent &event)
{
	m_isResizing = true;
	int col = event.GetRowOrCol();
	m_players->SetColumnWidth( col, m_gridWidths[m_players->GetColLabelValue( col )] );
}

void CPlayersPanel::UnAssignPlayer(const CPlayer &player, const double &inflation)
{
	int i = 0;
	m_players->BeginBatch();
	for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++, i++ )
	{
		if( (*it).GetName() == const_cast<CPlayer &>( player ).GetName() )
		{
			(*it).SetOwner( wxEmptyString );
			(*it).SetAmountPaid( 0 );
			(*it).SetDraftedPosition( wxEmptyString );
			(*it).DraftPlayer( false );
			(*it).SetDraftOrder( 0 );
		}
		if( !(*it).IsPlayerDrafted() )
		{
			(*it).SetCurrentValue( (*it).GetValue() * inflation );
			m_players->SetCellValue( i, 32, wxString::Format( "$%.2f", (*it).GetCurrentValue() ) );
			m_players->SetCellValue( i, 33, wxEmptyString );
			m_players->SetCellValue( i, 34, wxEmptyString );
			m_players->SetCellValue( i, 35, wxEmptyString );
		}
	}
	m_players->EndBatch();
}

void CPlayersPanel::FilterColumns(bool displayHitters, bool displayPitchers)
{
	unsigned int i;
	m_players->AutoSizeColumn( 0 );
	m_players->AutoSizeColumn( 1 );
	m_players->AutoSizeColumn( 2 );
	m_players->AutoSizeColumn( 3 );
	if( !m_hitterColumns["Age"] )
		m_players->HideCol( 4 );
	else
	{
		m_players->SetColSize( 4, wxGRID_AUTOSIZE );
		m_players->AutoSizeColumn( 4 );
	}
	for( i = 5; i < 31; i++ )
	{
		if( m_hitterColumns[m_players->GetColLabelValue( i )] && displayHitters )
		{
			m_players->SetColSize( i, wxGRID_AUTOSIZE );
			m_players->AutoSizeColumn( i );
		}
		else
			m_players->HideCol( i );
	}
	if( !m_columns["BegValue"] )
		m_players->HideCol( 31 );
	else
	{
		m_players->SetColSize( 31, wxGRID_AUTOSIZE );
		m_players->AutoSizeColumn( 31 );
	}
	if( !m_columns["CurrValue"] )
		m_players->HideCol( 32 );
	else
	{
		m_players->SetColSize( 32, wxGRID_AUTOSIZE );
		m_players->AutoSizeColumn( 32 );
	}
	if( !m_columns["AmtPaid"] )
		m_players->HideCol( 33 );
	else
	{
		m_players->SetColSize( 33, wxGRID_AUTOSIZE );
		m_players->AutoSizeColumn( 33 );
	}
	if( !m_columns["Profit"] )
		m_players->HideCol( 34 );
	else
	{
		m_players->SetColSize( 34, wxGRID_AUTOSIZE );
		m_players->AutoSizeColumn( 34 );
	}
	if( !m_columns["Owner"] )
		m_players->HideCol( 35 );
	else
	{
		m_players->SetColSize( 35, wxGRID_AUTOSIZE );
		m_players->AutoSizeColumn( 35 );
	}
	for( i = 36; i < 64; i++ )
	{
		if( m_pitcherColumns[m_players->GetColLabelValue( i )] && displayPitchers )
		{
			m_players->SetColSize( i, wxGRID_AUTOSIZE );
			m_players->AutoSizeColumn( i );
		}
		else
			m_players->HideCol( i );
	}
	if( !m_columns["Notes"] )
		m_players->HideCol( 64 );
	else
	{
		int col = m_players->GetNumberCols() - 1;
		m_players->SetColSize( col, wxGRID_AUTOSIZE );
		m_players->SetColSize( col, m_players->GetColSize( col ) * 4 );
	}
}

int CPlayersPanel::FindColumnByLabel(const wxString &label, bool isHitter)
{
	int result = wxNOT_FOUND, startColumn = isHitter ? 5 : 36, endColumn = isHitter ? 31 : 64;
	bool found = false;
	for( int i = startColumn; i < endColumn && !found; i++ )
	{
		if( m_players->GetColLabelValue( i ) == label )
		{
			found = true;
			result = i;
		}
	}
	return result;
}

wxButton &CPlayersPanel::GetAddPlayerButton()
{
	return *m_add;
}

wxButton &CPlayersPanel::GetEditColumnsButton()
{
	return *m_edit;
}

void CPlayersPanel::CapitalizeName(wxString &name)
{
	const wxString::iterator en = name.end();
	wxString::iterator it = name.begin();
	if( it != en )
		*it = (wxChar) wxToupper(*it);
}

int CPlayersPanel::GetFilterType()
{
	return m_filterType;
}

CNameCompleter &CPlayersPanel::GetCompleter()
{
	return *m_completer;
}

void CPlayersPanel::SetSelectionToRow(int row)
{
	m_selection = row;
}

void CPlayersPanel::SetPlayerToZero(const wxString &name)
{
	int i;
	bool found = false;
	for( i = 0; i < m_players->GetNumberRows() && !found; i++ )
	{
		if( m_players->GetCellValue( i, 1 ) == name )
		{
			found = true;
			m_players->SetCellValue( i, 31, "0" );
			m_players->SetCellValue( i, 32, "0.0" );
		}
	}
}

void CPlayersPanel::PerformDraft(const CPlayer &player, double inflation, const wxString &name)
{
	bool ok = false;
	for( int row = 0; row < m_players->GetNumberRows() && !ok; row++ )
	{
		if( m_players->GetCellValue( row, 1 ) == name )
		{
			wxGridUpdateLocker locker( m_players );
			m_players->BeginBatch();
			if( GetFilterType() == 0 )
				m_players->SetRowSize( row, 0 );
			else
			{
				m_players->SetCellValue( row, 32, wxString::Format( "$%.2f", player.GetCurrentValue() ) );
				m_players->SetCellValue( row, 33, wxString::Format( "$%d", player.GetAmountPaid() ) );
				m_players->SetCellValue( row, 34, wxString::Format( "$%d", (int) player.GetValue() - player.GetAmountPaid() ) );
				m_players->SetCellValue( row, 35, const_cast<CPlayer &>( player ).GetOwner() );
			}
			RecalculatePlayersValue( inflation, true, NULL, wxEmptyString, 0 );
			m_players->EndBatch();
			ok = true;
		}
	}
}

void CPlayersPanel::SetCurrentPlayerRow(int row)
{
	m_row = row;
}

void CPlayersPanel::DeletePlayer(const CPlayer &player)
{
	int deletedRank = const_cast<CPlayer &>( player ).GetRange();
	for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
	{
		if( (*it).GetRange() == deletedRank )
			(*it).DeletePlayer( true );
		else if( (*it).GetRange() > deletedRank )
			(*it).SetRange( (*it).GetRange() - 1 );
	}
	m_players->BeginBatch();
	for( int i = 0; i < m_players->GetNumberRows(); i++ )
	{
		int rank = wxAtoi( m_players->GetCellValue( i, 0 ) );
		if( rank == deletedRank )
			m_players->DeleteRows( i );
	}
	for( int i = 0; i < m_players->GetNumberRows(); i++ )
	{
		int rank = wxAtoi( m_players->GetCellValue( i, 0 ) );
		if( rank > deletedRank )
			m_players->SetCellValue( i, 0, wxString::Format( "%d", rank - 1 ) );
	}
	m_players->EndBatch();
}
