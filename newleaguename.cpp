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
#include "wx/tipdlg.h"
#include "wx/wizard.h"
#include "wx/spinctrl.h"
#include "leaguesettings.h"
#include "db.h"
#include "newleaguename.h"

CNewLeagueName::CNewLeagueName(wxWindow *parent, const CDb &db, CLeagueSettings *settings, const std::vector<std::string> &leagues, bool isUpdate) : wxPanel( parent )
{
	m_isUpdate = isUpdate;
	m_leagues = leagues;
	m_db = const_cast<CDb *>( &db );
	wxBoxSizer *verticalBoundaries = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *horizontalBoundaries = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *controls = new wxBoxSizer( wxVERTICAL );
	wxSizer *nameSizer = new wxBoxSizer( wxHORIZONTAL );
	wxSizer *ownersSizer = new wxBoxSizer( wxHORIZONTAL );
	wxSizer *salaryTypeSizer = new wxBoxSizer( wxHORIZONTAL );
	wxSizer *salarySizer = new wxBoxSizer( wxVERTICAL );
	m_label1 = new wxStaticText( this, wxID_ANY, "Enter League Name: " );
	m_name = new wxTextCtrl( this, wxID_ANY, !settings ? wxEmptyString : settings->GetLeagueName(), wxDefaultPosition, wxDefaultSize, 0L );
	m_label2 = new wxStaticText( this, wxID_ANY, "Number of Owners" );
	m_owners = new wxSpinCtrl( this, wxID_ANY, !settings ? "10" : wxString::Format( "%d", settings->GetOwnersNumber() ), wxDefaultPosition, wxSize( 50, -1 ), wxSP_ARROW_KEYS, 2, 30, 10 );
	m_label3 = new wxStaticText( this, wxID_ANY, "Salary:" );
	m_salary = new wxSpinCtrl( this, wxID_ANY, "260", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 999, 260 );
	if( settings )
		m_salary->SetValue( settings->GetSalary() );
	static const wxString draftTypes[] = 
	{
		wxT( "Auction" ),
		wxT( "Straight Draft" ),
		wxT( "Snake Draft" )
	};
	m_draftType = new wxRadioBox( this, wxID_ANY, "Draft Type", wxDefaultPosition, wxDefaultSize, 3, draftTypes, 0, wxRA_SPECIFY_ROWS );
	if( settings )
		m_draftType->SetSelection( settings->GetDraftType() - 1 );
	static const wxString scoringType[] =
	{
		wxT( "Roto" ),
		wxT( "Points" )
	};
	m_scoringtype = new wxRadioBox( this, wxID_ANY, "Scoring Type", wxDefaultPosition, wxDefaultSize, 2, scoringType );
	if( settings )
		m_scoringtype->SetSelection( settings->GetScoringType() - 1 );
	static const wxString roundValues[] =
	{
		wxT( "$.10" ),
		wxT( "$.25" ),
		wxT( "$.50" ),
		wxT( "$1.00" )
	};
	m_round = new wxRadioBox( this, wxID_ANY, "Round Values To Nearest", wxDefaultPosition, wxDefaultSize, 4, roundValues );
	if( settings && settings->GetDraftType() == 1 )
		m_round->SetSelection( settings->GetRoundingtype() - 1 );
	verticalBoundaries->Add( 5, 5, 0, wxEXPAND, 0 );
	nameSizer->Add( m_label1, 0, wxEXPAND, 0 );
	nameSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	nameSizer->Add( m_name, 0, wxEXPAND, 0 );
	controls->Add( nameSizer, 0, wxEXPAND, 0 );
	controls->Add( 5, 5, 0, wxEXPAND, 0 );
	ownersSizer->Add( m_label2, 0, wxEXPAND, 0 );
	ownersSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	ownersSizer->Add( m_owners, 0, wxEXPAND, 0 );
	controls->Add( ownersSizer, 0, wxEXPAND, 0 );
	controls->Add( 5, 5, 0, wxEXPAND, 0 );
	salaryTypeSizer->Add( m_draftType, 0, wxEXPAND, 0 );
	salaryTypeSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	salarySizer->Add( m_label3, 0, wxEXPAND, 0 );
	salarySizer->Add( 5, 5, 0, wxEXPAND, 0 );
	salarySizer->Add( m_salary, 0, wxEXPAND, 0 );
	salaryTypeSizer->Add( salarySizer, 0, wxEXPAND, 0 );
	controls->Add( salaryTypeSizer, 0, wxEXPAND, 0 );
	controls->Add( 5, 5, 0, wxEXPAND, 0 );
	controls->Add( m_scoringtype, 0, wxEXPAND, 0 );
	controls->Add( 5, 5, 0, wxEXPAND, 0 );
	controls->Add( m_round, 0, wxEXPAND, 0 );
	horizontalBoundaries->Add( controls, 0, wxEXPAND, 0 );
	verticalBoundaries->Add( horizontalBoundaries, 0, wxEXPAND, 0 );
	verticalBoundaries->Add( 5, 5, 0, wxEXPAND, 0 );
	SetSizerAndFit( verticalBoundaries );
	m_name->SetFocus();
}

CNewLeagueName::~CNewLeagueName(void)
{
}

bool CNewLeagueName::IsNameEmpty()
{
	return m_name->GetLineLength( 0 ) == 0;
}

/*bool CNewLeagueName::Show(bool show)
{
	bool result = wxPanel::Show( show );
	m_name->SetFocus();
	return result;
}
*/
wxString CNewLeagueName::GetName()
{
	return m_name->GetLineText( 0 );
}

int CNewLeagueName::GetNumberOfOwners()
{
	return m_owners->GetValue();
}

int CNewLeagueName::GetDraftType()
{
	return m_draftType->GetSelection();
}

int CNewLeagueName::GetScoringType()
{
	return m_scoringtype->GetSelection();
}

wxTextCtrl *CNewLeagueName::GetNameCtrl()
{
	return m_name;
}

int CNewLeagueName::GetRoundType()
{
	return m_round->GetSelection();
}

void CNewLeagueName::OnDraftChange(wxCommandEvent &event)
{
	if( dynamic_cast<wxRadioBox *>( event.GetEventObject() )->GetSelection() == 0 )
		m_round->Show();
	else
		m_round->Show( false );
}

bool CNewLeagueName::Validate()
{
	bool validated = true, found = false;
	wxString name = m_name->GetValue();
	if( name.IsEmpty() )
	{
		wxMessageBox( "Please enter League Name" );
		m_name->SetFocus();
		validated = false;
	}
	for( std::vector<std::string>::iterator it = m_leagues.begin(); it < m_leagues.end() && !found; it++ )
	{
		if( name == (*it) && !m_isUpdate )
			found = true;
	}
	if( found )
	{
		wxMessageBox( "League " + name + " already exists! Please choose a different name" );
		m_name->SetFocus();
		validated = false;
	}
	if( m_owners->GetValue() < 2 )
	{
		wxMessageBox( "At least 2 owners is required" );
		m_owners->SetFocus();
		validated = false;
	}
	return validated;
}

wxSpinCtrl *CNewLeagueName::GetOwnersNumberCtrl()
{
	return m_owners;
}

int CNewLeagueName::GetSalary()
{
	return m_salary->GetValue();
}
