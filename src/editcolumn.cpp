// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include <map>
#include "wx/tipdlg.h"
#include "leaguesettings.h"
#include "newleaguescoring.h"
#include "editcolumn.h"

CEditColumn::CEditColumn(wxWindow *parent, CLeagueSettings &settings, const std::map<wxString,bool> &displayedHitters, const std::map<wxString,bool> &displayedPitchers, const std::map<wxString,bool> &columnsDisplayed) : wxDialog( parent, wxID_ANY, "Edit Columns" )
{
	m_panel = new wxPanel( this );
	m_displayedHitters = &( const_cast<std::map<wxString,bool> &>( displayedHitters ) );
	m_displayedPitchers = &( const_cast<std::map<wxString,bool> &>( displayedPitchers ) );
	m_displayedColumns = &( const_cast<std::map<wxString,bool> &>( columnsDisplayed ) );
	m_page = new CNewLeagueScoring( m_panel, &settings, m_displayedHitters, m_displayedPitchers );
	m_begValue = new wxCheckBox( m_panel, wxID_ANY, "BegValue" );
	m_curValue = new wxCheckBox( m_panel, wxID_ANY, "CurValue" );
	m_amtPaid = new wxCheckBox( m_panel, wxID_ANY, "AmtPaid" );
	m_profit = new wxCheckBox( m_panel, wxID_ANY, "Profit" );
	m_team = new wxCheckBox( m_panel, wxID_ANY, "Owner" );
	m_notes = new wxCheckBox( m_panel, wxID_ANY, "Notes" );
	m_age = new wxCheckBox( m_panel, wxID_ANY, "Age" );
	m_ok = new wxButton( m_panel, wxID_OK, "OK" );
	m_cancel = new wxButton( m_panel, wxID_CANCEL, "Cancel" );
	if( const_cast<std::map<wxString,bool> &>(columnsDisplayed)["BegValue"] )
		m_begValue->SetValue( true );
	if( const_cast<std::map<wxString,bool> &>(columnsDisplayed)["CurrValue"] )
		m_curValue->SetValue( true );
	if( const_cast<std::map<wxString,bool> &>(columnsDisplayed)["AmtPaid"] )
		m_amtPaid->SetValue( true );
	if( const_cast<std::map<wxString,bool> &>(columnsDisplayed)["Profit"] )
		m_profit->SetValue( true );
	if( const_cast<std::map<wxString,bool> &>(columnsDisplayed)["Owner"] )
		m_team->SetValue( true );
	if( const_cast<std::map<wxString,bool> &>(columnsDisplayed)["Notes"] )
		m_notes->SetValue( true );
	if( const_cast<std::map<wxString,bool> &>(columnsDisplayed)["Age"] )
		m_age->SetValue( true );
	m_begValue->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_curValue->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_amtPaid->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_profit->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_team->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_notes->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_age->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	DoLayout();
}

CEditColumn::~CEditColumn(void)
{
	m_begValue->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_curValue->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_amtPaid->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_profit->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_team->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_notes->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
	m_age->Unbind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CEditColumn::OnColumnSelection, this );
}

void CEditColumn::DoLayout()
{
	wxBoxSizer *main = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *horizontal = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *vertical = new wxBoxSizer( wxVERTICAL );
	wxFlexGridSizer *columns = new wxFlexGridSizer( 3, 3, 5, 5 );
    wxStdDialogButtonSizer *buttons = new wxStdDialogButtonSizer;
    buttons->AddButton( m_ok );
    buttons->AddButton( m_cancel );
    buttons->Realize();
	horizontal->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( m_page, 0, wxEXPAND, 0 );
	columns->Add( m_age, 0, wxEXPAND, 0 );
	columns->Add( m_begValue, 0, wxEXPAND, 0 );
	columns->Add( m_curValue, 0, wxEXPAND, 0 );
	columns->Add( m_amtPaid, 0, wxEXPAND, 0 );
	columns->Add( m_profit, 0, wxEXPAND, 0 );
	columns->Add( m_team, 0, wxEXPAND, 0 );
	columns->Add( m_notes, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( columns, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( buttons, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	horizontal->Add( vertical, 0, wxEXPAND, 0 );
	horizontal->Add( 5, 5, 0, wxEXPAND, 0 );
	m_panel->SetSizerAndFit( horizontal );
	main->Add( m_panel, 0, wxEXPAND, 0 );
	SetSizerAndFit( main );
//	Layout();
}

CNewLeagueScoring &CEditColumn::GetScoringPanel()
{
	return *m_page;
}

void CEditColumn::OnColumnSelection(wxCommandEvent &event)
{
	wxCheckBox *object = dynamic_cast<wxCheckBox *>( event.GetEventObject() );
	if( object->IsChecked() )
	{
		if( object == m_begValue )
			m_displayedColumns->at( "BegValue" ) = true;
		if( object == m_curValue )
			m_displayedColumns->at( "CurValue" ) = true;
		if( object == m_amtPaid )
			m_displayedColumns->at( "AmtPaid" ) = true;
		if( object == m_profit )
			m_displayedColumns->at( "Profit" ) = true;
		if( object == m_team )
			m_displayedColumns->at( "Owner" ) = true;
		if( object == m_notes )
			m_displayedColumns->at( "Notes" ) = true;
		if( object == m_age )
			m_displayedColumns->at( "Age" ) = true;
	}
	else
	{
		if( object == m_begValue )
			m_displayedColumns->at( "BegValue" ) = false;
		if( object == m_curValue )
			m_displayedColumns->at( "CurValue" ) = false;
		if( object == m_amtPaid )
			m_displayedColumns->at( "AmtPaid" ) = false;
		if( object == m_profit )
			m_displayedColumns->at( "Profit" ) = false;
		if( object == m_team )
			m_displayedColumns->at( "Owner" ) = false;
		if( object == m_notes )
			m_displayedColumns->at( "Notes" ) = false;
		if( object == m_age )
			m_displayedColumns->at( "Age" ) = false;
	}
}
