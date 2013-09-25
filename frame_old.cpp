// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include <map>
#include "sqlite3.h"
#include "leaguesettings.h"
#include "addnewplayer.h"
#include "db.h"
#include "wx/grid.h"
#include "wx/spinctrl.h"
#include "wx/textcompleter.h"
#include "namecompleter.h"
#include "gridtotalcell.h"
#include "frame.h"

BEGIN_EVENT_TABLE(CFrame, wxFrame)
	EVT_MENU(wxMENU_FILE_ADD_PLAYER, CFrame::OnAddNewPlayer)
END_EVENT_TABLE()

CFrame::CFrame(const wxString &title, CLeagueSettings *league, const CDb &db, const wxString &name) : wxFrame( NULL, wxID_ANY, title )
{
	m_rosterDisplayed = true;
	wxMenu *file_menu = new wxMenu();
	wxMenu *edit_menu = new wxMenu();
	wxMenu *activate_menu = new wxMenu();
	wxMenu *help_menu = new wxMenu();
	file_menu->Append( wxMENU_FILE_ADD_PLAYER, "Add New Player\tCtrl+N", "Add New Player" );
	file_menu->Append( wxID_EXIT, "Save and Exit\tAlt-X", "Exit the program" );
	edit_menu->Append( wxMENU_EDIT_LEAGUE_SETTINGS, "League Settings", "League Settings" );
	edit_menu->Append( wxMENU_EDIT_TARGETS, "Targets", "Targets" );
	edit_menu->Append( wxMENU_EDIT_CLEARALL, "Clear All Drafted Players", "Clear All Players" );
	wxMenuBar *bar = new wxMenuBar();
	bar->Append( file_menu, "&File" );
	bar->Append( edit_menu, "&Edit" );
	bar->Append( activate_menu, "&Activate" );
	bar->Append( help_menu, "&Help" );
	SetMenuBar( bar );
	std::map<wxString,int> positions;
	std::vector<wxString> leagueScoringHitters, leagueScoringPitchers;
	m_db = const_cast<CDb *>( &db );
	m_data = new CLeagueData();
	LoadLeagueData( league, db, *m_data, name );
	m_db->GetPositionCount( m_data->m_settings->GetLeagueName(), positions );
	m_db->GetLeagueScoring( m_data->m_settings->GetLeagueName(), leagueScoringHitters, leagueScoringPitchers );
	wxBoxSizer *mainSizer = new wxBoxSizer( wxHORIZONTAL );
	m_panel = new wxPanel( this );
	m_label1 = new wxStaticText( m_panel, wxID_ANY, "Owner" );
	m_teams = new wxComboBox( m_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY );
	m_hideRoster = new wxButton( m_panel, wxID_ANY, "Hide" );
	m_label10 = new wxStaticText( m_panel, wxID_ANY, "Player:" );
	m_player = new wxTextCtrl( m_panel, wxID_ANY );
	m_player->AutoComplete( new CNameCompleter( const_cast<std::vector<CPlayer> *>( m_data->m_players ) ) );
	m_label11 = new wxStaticText( m_panel, wxID_ANY, "Owner:" );
	m_team = new wxComboBox( m_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY | wxCB_DROPDOWN );
	std::vector<std::string>::iterator it;
	for( it = m_data->m_settings->GetOwners().begin(); it < m_data->m_settings->GetOwners().end(); it++ )
		m_team->Append( *it );
	m_label12 = new wxStaticText( m_panel, wxID_ANY, "Amount Paid:" );
	m_amountPaid = new wxSpinCtrl( m_panel, wxID_ANY );
	m_label13 = new wxStaticText( m_panel, wxID_ANY, "Position:" );
	m_position = new wxComboBox( m_panel, wxID_ANY, wxEmptyString );
	m_draft = new wxButton( m_panel, wxID_ANY, "Draft" );
	m_playersDrafted = new CMyGrid( m_panel );
	m_playersDrafted->CreateGrid( 1, 31 );
	m_playersDrafted->HideRowLabels();
	m_playersDrafted->SetReadOnly( 1, 0 );
	m_playersDrafted->SetReadOnly( 1, 1 );
	m_playersDrafted->SetReadOnly( 1, 2 );
	m_playersDrafted->SetColLabelValue( 0, "Player" );
	m_playersDrafted->SetColLabelValue( 1, "Team" );
	m_playersDrafted->SetColLabelValue( 2, "Position" );
	m_playersDrafted->SetColLabelValue( 3, "" );
	m_playersDrafted->SetColFormatFloat( 3, -1, 3 );
	m_playersDrafted->SetColLabelValue( 4, "" );
	m_playersDrafted->SetColFormatFloat( 4, -1, 3 );
	m_playersDrafted->SetColLabelValue( 5, "" );
	m_playersDrafted->SetColFormatFloat( 5, -1, 3 );
	m_playersDrafted->SetColLabelValue( 6, "" );
	m_playersDrafted->SetColFormatFloat( 6, -1, 3 );
	m_playersDrafted->SetColLabelValue( 7, "" );
	m_playersDrafted->SetColFormatFloat( 7, -1, 3 );
	m_playersDrafted->SetColLabelValue( 8, "" );
	m_playersDrafted->SetColFormatFloat( 8, -1, 3 );
	m_playersDrafted->SetColLabelValue( 9, "" );
	m_playersDrafted->SetColFormatFloat( 9, -1, 3 );
	m_playersDrafted->SetColLabelValue( 10, "" );
	m_playersDrafted->SetColFormatFloat( 10, -1, 3 );
	m_playersDrafted->SetColLabelValue( 11, "" );
	m_playersDrafted->SetColFormatFloat( 11, -1, 3 );
	m_playersDrafted->SetColLabelValue( 12, "" );
	m_playersDrafted->SetColFormatFloat( 12, -1, 3 );
	m_playersDrafted->SetColLabelValue( 13, "" );
	m_playersDrafted->SetColFormatFloat( 13, -1, 3 );
	m_playersDrafted->SetColLabelValue( 14, "" );
	m_playersDrafted->SetColFormatFloat( 14, -1, 3 );
	m_playersDrafted->SetColLabelValue( 15, "" );
	m_playersDrafted->SetColFormatFloat( 15, -1, 3 );
	m_playersDrafted->SetColLabelValue( 16, "" );
	m_playersDrafted->SetColFormatFloat( 16, -1, 3 );
	m_playersDrafted->SetColLabelValue( 17, "" );
	m_playersDrafted->SetColFormatFloat( 17, -1, 3 );
	m_playersDrafted->SetColLabelValue( 18, "" );
	m_playersDrafted->SetColFormatFloat( 18, -1, 3 );
	m_playersDrafted->SetColLabelValue( 19, "" );
	m_playersDrafted->SetColFormatFloat( 19, -1, 3 );
	m_playersDrafted->SetColLabelValue( 20, "" );
	m_playersDrafted->SetColFormatFloat( 20, -1, 3 );
	m_playersDrafted->AutoSize();
//	m_hittersDrafted->SetSize( -1, 2 * m_playersDrafted->GetRowMinimalAcceptableHeight() );
	m_label2 = new wxStaticText( m_panel, wxID_ANY, "Hitters" );
	m_label2->SetFont( m_label2->GetFont().Bold() );
	int hitters = positions["C"] + positions["1B"] + positions["3B"] + positions["CI"] + positions["2B"] + positions["SS"] + positions["MI"] + positions["OF"] + positions["Utility"];
	m_hittersDrafted = new CMyGrid( m_panel );
	m_hittersDrafted->CreateGrid( hitters + 2, leagueScoringHitters.size() + 3 );
	m_hittersDrafted->HideRowLabels();
	m_hittersDrafted->SetColLabelValue( 0, "Position" );
	m_hittersDrafted->SetColLabelValue( 1, "Player" );
	m_hittersDrafted->SetColLabelValue( 2, "Salary" );
	int i = 3, row = 0;
	for( std::vector<wxString>::iterator it = leagueScoringHitters.begin(); it < leagueScoringHitters.end(); it++, i++ )
		m_hittersDrafted->SetColLabelValue( i, *it );
	for( i = 0; i < positions["C"]; i++ )
	{
		m_hittersDrafted->SetCellValue( row, 0, "C" );
		m_hittersDrafted->SetReadOnly( row, 0 );
		row++;
	}
	for( i = 0; i < positions["1B"]; i++ )
	{
		m_hittersDrafted->SetCellValue( row, 0, "1B" );
		m_hittersDrafted->SetReadOnly( row, 0 );
		row++;
	}
	for( i = 0; i < positions["3B"]; i++ )
	{
		m_hittersDrafted->SetCellValue( row, 0, "3B" );
		m_hittersDrafted->SetReadOnly( row, 0 );
		row++;
	}
	for( i = 0; i < positions["CI"]; i++ )
	{
		m_hittersDrafted->SetCellValue( row, 0, "CI" );
		m_hittersDrafted->SetReadOnly( row, 0 );
		row++;
	}
	for( i = 0; i < positions["2B"]; i++ )
	{
		m_hittersDrafted->SetCellValue( row, 0, "2B" );
		m_hittersDrafted->SetReadOnly( row, 0 );
		row++;
	}
	for( i = 0; i < positions["SS"]; i++ )
	{
		m_hittersDrafted->SetCellValue( row, 0, "SS" );
		m_hittersDrafted->SetReadOnly( row, 0 );
		row++;
	}
	for( i = 0; i < positions["MI"]; i++ )
	{
		m_hittersDrafted->SetCellValue( row, 0, "MI" );
		m_hittersDrafted->SetReadOnly( row, 0 );
		row++;
	}
	for( i = 0; i < positions["OF"]; i++ )
	{
		m_hittersDrafted->SetCellValue( row, 0, "OF" );
		m_hittersDrafted->SetReadOnly( row, 0 );
		row++;
	}
	for( i = 0; i < positions["Utility"]; i++ )
	{
		m_hittersDrafted->SetCellValue( row, 0, "U" );
		m_hittersDrafted->SetReadOnly( row, 0 );
		row++;
	}
	m_hittersDrafted->SetReadOnly( row, 0 );
	m_hittersDrafted->SetCellRenderer( m_hittersDrafted->GetNumberRows() - 1, 0, new CGridTotalCell( "Edit" ) );
	m_hittersDrafted->SetReadOnly( m_hittersDrafted->GetNumberRows() - 1, 0 );
	m_hittersDrafted->SetCellFont( m_hittersDrafted->GetNumberRows() - 2, 1, m_hittersDrafted->GetCellFont( m_hittersDrafted->GetNumberRows() - 2, 1 ).Bold() );
	m_hittersDrafted->SetCellValue( m_hittersDrafted->GetNumberRows() - 2, 1, "Totals" );
	m_hittersDrafted->SetReadOnly( m_hittersDrafted->GetNumberRows() - 2, 1 );
	m_hittersDrafted->SetCellValue( m_hittersDrafted->GetNumberRows() - 1, 1, "Target" );
	m_hittersDrafted->SetReadOnly( m_hittersDrafted->GetNumberRows() - 1, 1 );
	m_label3 = new wxStaticText( m_panel, wxID_ANY, "Pitchers" );
	m_label3->SetFont( m_label2->GetFont().Bold() );
	int pitchers = positions["P"];
	m_pitchersDrafted = new CMyGrid( m_panel );
	m_pitchersDrafted->CreateGrid( pitchers + 2, leagueScoringPitchers.size() + 3 );
	m_pitchersDrafted->HideRowLabels();
	m_pitchersDrafted->SetColLabelValue( 0, "Position" );
	m_pitchersDrafted->SetColLabelValue( 1, "Player" );
	m_pitchersDrafted->SetColLabelValue( 2, "Salary" );
	i = 3;
	for( std::vector<wxString>::iterator it = leagueScoringPitchers.begin(); it < leagueScoringPitchers.end(); it++, i++ )
		m_pitchersDrafted->SetColLabelValue( i, *it );
	row = 0;
	for( i = 0; i < positions["P"]; i++ )
	{
		m_pitchersDrafted->SetCellValue( row, 0, "P" );
		m_pitchersDrafted->SetReadOnly( row, 0 );
		row++;
	}
	m_pitchersDrafted->SetReadOnly( row, 0 );
	m_pitchersDrafted->SetCellRenderer( m_pitchersDrafted->GetNumberRows() - 1, 0, new CGridTotalCell( "Edit" ) );
	m_pitchersDrafted->SetReadOnly( m_pitchersDrafted->GetNumberRows() - 1, 0 );
	m_pitchersDrafted->SetCellFont( m_pitchersDrafted->GetNumberRows() - 2, 1, m_pitchersDrafted->GetCellFont( m_pitchersDrafted->GetNumberRows() - 2, 1 ).Bold() );
	m_pitchersDrafted->SetCellValue( m_pitchersDrafted->GetNumberRows() - 2, 1, "Totals" );
	m_pitchersDrafted->SetReadOnly( m_pitchersDrafted->GetNumberRows() - 2, 1 );
	m_pitchersDrafted->SetCellValue( m_pitchersDrafted->GetNumberRows() - 1, 1, "Target" );
	m_pitchersDrafted->SetReadOnly( m_pitchersDrafted->GetNumberRows() - 1, 1 );
	m_hittersDrafted->AutoSize();
	m_pitchersDrafted->AutoSize();
	m_hittersDrafted->SetSize( -1, 7 * m_hittersDrafted->GetRowMinimalAcceptableHeight() );
	m_pitchersDrafted->SetSize( -1, 7 * m_pitchersDrafted->GetRowMinimalAcceptableHeight() );
	m_label4 = new wxStaticText( m_panel, wxID_ANY, "Salary Left:" );
	m_salaryLeft = new wxStaticText( m_panel, wxID_ANY, "$0.00" );
	m_label5 = new wxStaticText( m_panel, wxID_ANY, "Players Left:" );
	m_playerLeft = new wxStaticText( m_panel, wxID_ANY, "0" );
	m_playerLeft->SetLabel( wxString::Format( "%d", hitters + pitchers ) );
	m_label6 = new wxStaticText( m_panel, wxID_ANY, "Avg $/player left:" );
	m_average = new wxStaticText( m_panel, wxID_ANY, "0" );
	m_label7 = new wxStaticText( m_panel, wxID_ANY, "Inflation:" );
	m_inflation = new wxStaticText( m_panel, wxID_ANY, "0%" );
	m_label8 = new wxStaticText( m_panel, wxID_ANY, "Profit:" );
	m_profit = new wxStaticText( m_panel, wxID_ANY, "0" );
	m_label9 = new wxStaticText( m_panel, wxID_ANY, "Max Bid:" );
	m_maxBid = new wxStaticText( m_panel, wxID_ANY, "0" );
	m_playersRoster = new wxButton( m_panel, wxID_ANY, "Players" );
	m_roster = new wxButton( m_panel, wxID_ANY, "Roster" );
	m_teamProjection = new wxButton( m_panel, wxID_ANY, "Team Projections" );
	m_draftResults = new wxButton( m_panel, wxID_ANY, "Draft Results" );
	wxBoxSizer *horizontalBoundaries = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *verticalBoundaries = new wxBoxSizer( wxVERTICAL );
	wxFlexGridSizer *sectionSizer = new wxFlexGridSizer( 2, 2, 5, 5 );
	wxBoxSizer *teamRosterControlsTop = new wxBoxSizer( wxHORIZONTAL );
	m_teamRoster = new wxBoxSizer( wxVERTICAL );
	m_teamRosterBottom = new wxFlexGridSizer( 3, 2, 5, 5 );
	wxBoxSizer *teamRosterBottom1 = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *teamRosterBottom2 = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *teamRosterBottom3 = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *teamRosterBottom4 = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *teamRosterBottom5 = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *teamRosterBottom6 = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *playerForDraft = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *draftData = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *dataSection = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *buttons = new wxBoxSizer( wxHORIZONTAL );
	horizontalBoundaries->Add( 5, 5, 0, wxEXPAND, 0 );
	verticalBoundaries->Add( 5, 5, 0, wxEXPAND, 0 );
	teamRosterControlsTop->Add( m_label1, 0, wxEXPAND, 0 );
	teamRosterControlsTop->Add( 5, 5, 0, wxEXPAND, 0 );
	teamRosterControlsTop->Add( m_teams, 0, wxFIXED_MINSIZE, 0 );
	teamRosterControlsTop->Add( 5, 5, 0, wxEXPAND, 0 );
	teamRosterControlsTop->Add( m_hideRoster, 0, wxFIXED_MINSIZE, 0 );
	sectionSizer->Add( teamRosterControlsTop, 0, wxEXPAND, 0 );
	m_teamRoster->Add( m_label2, 0, wxEXPAND, 0 );
	m_teamRoster->Add( 5, 5, 0, wxEXPAND, 0 );
	m_teamRoster->Add( m_hittersDrafted, 0, wxEXPAND, 0 );
	m_teamRoster->Add( 5, 5, 0, wxEXPAND, 0 );
	m_teamRoster->Add( m_label3, 0, wxEXPAND, 0 );
	m_teamRoster->Add( 5, 5, 0, wxEXPAND, 0 );
	m_teamRoster->Add( m_pitchersDrafted, 0, wxEXPAND, 0 );
	teamRosterBottom1->Add( m_label4, 0, wxEXPAND, 0 );
	teamRosterBottom1->Add( 5, 5, 0, wxEXPAND, 0 );
	teamRosterBottom1->Add( m_salaryLeft, 0, wxEXPAND, 0 );
	m_teamRosterBottom->Add( teamRosterBottom1, 0, wxEXPAND, 0 );
	teamRosterBottom4->Add( m_label7, 0, wxEXPAND, 0 );
	teamRosterBottom4->Add( m_profit, 0, wxEXPAND, 0 );
	m_teamRosterBottom->Add( teamRosterBottom4, 0, wxEXPAND, 0 );
	teamRosterBottom2->Add( m_label5, 0, wxEXPAND, 0 );
	teamRosterBottom2->Add( 5, 5, 0, wxEXPAND, 0 );
	teamRosterBottom2->Add( m_inflation, 0, wxEXPAND, 0 );
	m_teamRosterBottom->Add( teamRosterBottom2, 0, wxEXPAND, 0 );
	teamRosterBottom5->Add( m_label8, 0, wxEXPAND, 0 );
	teamRosterBottom5->Add( 5, 5, 0, wxEXPAND, 0 );
	teamRosterBottom5->Add( m_average, 0, wxEXPAND, 0 );
	m_teamRosterBottom->Add( teamRosterBottom5, 0, wxEXPAND, 0 );
	teamRosterBottom3->Add( m_label6, 0, wxEXPAND, 0 );
	teamRosterBottom3->Add( 5, 5, 0, wxEXPAND, 0 );
	teamRosterBottom3->Add( m_playerLeft, 0, wxEXPAND, 0 );
	m_teamRosterBottom->Add( teamRosterBottom3, 0, wxEXPAND, 0 );
	teamRosterBottom6->Add( m_label9, 0, wxEXPAND, 0 );
	teamRosterBottom6->Add( 5, 5, 0, wxEXPAND, 0 );
	teamRosterBottom6->Add( m_maxBid, 0, wxEXPAND, 0 );
	m_teamRosterBottom->Add( teamRosterBottom6, 0, wxEXPAND, 0 );
	m_teamRoster->Add( m_teamRosterBottom, 0, wxEXPAND, 0 );
	playerForDraft->Add( m_label10, 0, wxEXPAND, 0 );
	playerForDraft->Add( 5, 5, 0, wxEXPAND, 0 );
	playerForDraft->Add( m_player, 0, wxEXPAND, 0 );
	playerForDraft->Add( 5, 5, 0, wxEXPAND, 0 );
	playerForDraft->Add( m_label11, 0, wxEXPAND, 0 );
	playerForDraft->Add( 5, 5, 0, wxEXPAND, 0 );
	playerForDraft->Add( m_team, 0, wxEXPAND, 0 );
	playerForDraft->Add( 5, 5, 0, wxEXPAND, 0 );
	playerForDraft->Add( m_label12, 0, wxEXPAND, 0 );
	playerForDraft->Add( 5, 5, 0, wxEXPAND, 0 );
	playerForDraft->Add( m_amountPaid, 0, wxEXPAND, 0 );
	playerForDraft->Add( 5, 5, 0, wxEXPAND, 0 );
	playerForDraft->Add( m_label13, 0, wxEXPAND, 0 );
	playerForDraft->Add( 5, 5, 0, wxEXPAND, 0 );
	playerForDraft->Add( m_position, 0, wxEXPAND, 0 );
	playerForDraft->Add( 5, 5, 0, wxEXPAND );
	playerForDraft->Add( m_draft, 0, wxEXPAND, 0 );
	draftData->Add( playerForDraft, 0, wxEXPAND, 0 );
	draftData->Add( 5, 5, 0, wxEXPAND, 0 );
	draftData->Add( m_playersDrafted, 0, wxEXPAND, 0 );
	sectionSizer->Add( draftData, 0, wxEXPAND, 0 );
	sectionSizer->Add( m_teamRoster, 0, wxEXPAND, 0 );
	buttons->Add( m_playersRoster, 0, wxEXPAND, 0 );
	dataSection->Add( buttons, 0, wxEXPAND, 0 );
	sectionSizer->Add( dataSection, 0, wxEXPAND, 0 );
	verticalBoundaries->Add( sectionSizer, 0, wxEXPAND, 0 );
	verticalBoundaries->Add( 5, 5, 0, wxEXPAND, 0 );
	horizontalBoundaries->Add( verticalBoundaries, 0, wxEXPAND, 0 );
	horizontalBoundaries->Add( 5, 5, 0, wxEXPAND, 0 );
	m_panel->SetSizer( horizontalBoundaries );
	mainSizer->Add( m_panel, 0, wxEXPAND, 0 );
	SetSizer( mainSizer );
	mainSizer->Fit( this );
	m_hittersDrafted->Bind( wxEVT_GRID_CELL_LEFT_CLICK, &CFrame::OnHittersEdit, this );
	m_pitchersDrafted->Bind( wxEVT_GRID_CELL_LEFT_CLICK, &CFrame::OnPitchersEdit, this );
	m_player->Bind( wxEVT_COMMAND_TEXT_UPDATED, &CFrame::OnPlayerNameChange, this );
	m_draft->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CFrame::OnDraftPerformed, this );
	m_hideRoster->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CFrame::OnHideShowteamRoster, this );
	Layout();
}

CFrame::~CFrame(void)
{
	delete m_data;
}

void CFrame::OnAddNewPlayer(wxCommandEvent &WXUNUSED(event))
{
	std::vector<wxString> teamNames;
	m_db->GetSelectedTeams( *(m_data->m_settings), teamNames );
	CAddNewPlayer dlg( this, "Add New Player", teamNames );
	dlg.ShowModal();
}

void CFrame::LoadLeagueData(CLeagueSettings *league, const CDb &db, CLeagueData &data, const wxString &name)
{
	wxBusyCursor cursor;
	if( !league )
	{
		league = new CLeagueSettings( name );
		data.m_settings = league;
		const_cast<CDb &>( db ).LoadLeagueData( league );
	}
	const_cast<CDb &>( db ).GetPlayersForLeague( data, *league );
}

void CFrame::OnHittersEdit(wxGridEvent &event)
{
	if( event.GetRow() == dynamic_cast<wxGrid *>( event.GetEventObject() )->GetNumberRows() - 1 && event.GetCol() == 0 )
		wxMessageBox( "Button was clicked!" );
}

void CFrame::OnPitchersEdit(wxGridEvent &event)
{
	if( event.GetRow() == dynamic_cast<wxGrid *>( event.GetEventObject() )->GetNumberRows() - 1 && event.GetCol() == 0 )
		wxMessageBox( "Button was clicked!" );
}

void CFrame::OnPlayerNameChange(wxCommandEvent &event)
{
	wxString name = m_player->GetValue();
	name.at( 0 ) = wxToupper( *(name.begin()) );
	std::vector<CPlayer>::iterator it;
	std::map<wxString,double>::iterator itmap;
	for( it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
	{
		if( it->GetName() == name )
		{
			m_amountPaid->SetValue( it->GetValue() );
			m_position->SetValue( it->GetPositions().at( 0 ) );
			m_playersDrafted->SetCellValue( 0, 0, name );
			m_playersDrafted->SetCellValue( 0, 1, it->GetTeam() );
			m_playersDrafted->SetCellValue( 0, 2, it->GetPositions()[0] );
			int column = 3;
			for( itmap = it->GetScoring().begin(); itmap != it->GetScoring().end(); itmap++ )
			{
				m_playersDrafted->SetColLabelValue( column, (*itmap).first );
				m_playersDrafted->SetCellValue( 0, column++, wxString::Format( "%.3f", (*itmap).second ) );
			}
			m_playersDrafted->AutoSize();
		}
	}
}

void CFrame::OnDraftPerformed(wxCommandEvent &event)
{
	if( m_player->GetValue().IsEmpty() )
	{
		wxMessageBox( "Please enter the player name for draft" );
		m_player->SetFocus();
		return;
	}
	if( m_team->GetValue().IsEmpty() )
	{
		wxMessageBox( "Please select the owner for the drafted player" );
		m_team->SetFocus();
		return;
	}
}

void CFrame::OnHideShowteamRoster(wxCommandEvent &event)
{
	if( m_rosterDisplayed )
	{
		m_teamRoster->Hide( m_hittersDrafted );
		m_teamRoster->Hide( m_pitchersDrafted );
		m_teamRoster->Hide( m_label2 );
		m_teamRoster->Hide( m_label3 );
/*		m_teamRoster->Hide( m_label4 );
		m_teamRoster->Hide( m_label5 );
		m_teamRoster->Hide( m_label6 );
		m_teamRoster->Hide( m_label7 );
		m_teamRoster->Hide( m_label8 );
		m_teamRoster->Hide( m_label9 );
		m_teamRoster->Hide( m_salaryLeft );
		m_teamRoster->Hide( m_playerLeft );
		m_teamRoster->Hide( m_average );
		m_teamRoster->Hide( m_inflation );
		m_teamRoster->Hide( m_profit );*/
		m_teamRoster->Hide( m_teamRosterBottom, true );
		m_hideRoster->SetLabel( "Show" );
	}
	else
	{
		m_teamRoster->Show( m_hittersDrafted );
		m_teamRoster->Show( m_pitchersDrafted );
		m_teamRoster->Show( m_label2 );
		m_teamRoster->Show( m_label3 );
/*		m_teamRoster->Show( m_label4 );
		m_teamRoster->Show( m_label5 );
		m_teamRoster->Show( m_label6 );
		m_teamRoster->Show( m_label7 );
		m_teamRoster->Show( m_label8 );
		m_teamRoster->Show( m_label9 );
		m_teamRoster->Show( m_salaryLeft );
		m_teamRoster->Show(m_playerLeft );
		m_teamRoster->Show( m_average );
		m_teamRoster->Show( m_inflation );
		m_teamRoster->Show( m_profit );*/
		m_teamRoster->Show( m_teamRosterBottom, true );
		m_hideRoster->SetLabel( "Hide" );
	}
	Layout();
	m_rosterDisplayed = !m_rosterDisplayed;
}

bool CFrame::Show(bool show)
{
	if( show && !IsShown() )
	{
		Layout();
	}
	return wxWindow::Show( show );
}
