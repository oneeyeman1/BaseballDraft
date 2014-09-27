// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include <string.h>
#include <map>
#include "sqlite3.h"
#include "wx/tipdlg.h"
#include "wx/wizard.h"
#include "wx/gbsizer.h"
#include "wx/spinctrl.h"
#include "wx/notebook.h"
#include "wx/propgrid/propgrid.h"
#include "leaguesettings.h"
#include "db.h"
#include "newleaguescoring.h"
#include "newleagueplayers.h"
#include "newleaguepositions.h"
#include "newleaguename.h"
#include "newleagueowners.h"
#include "createleague.h"

CCreateLeague::CCreateLeague(const wxString &title, const CDb &db, int mode, const wxString &leagueTitle, const std::vector<std::string> &leagues) : wxWizard( NULL, wxID_ANY, title, wxNullBitmap )
{
	m_isGood = true;
	SetIcon( wxIcon( "IDI_BASEBALLDRAFT" ) );
	m_mode = mode;
	m_db = const_cast<CDb *>( &db );
	m_page1 = new CNewLeaguePage( this, *m_db, m_mode, leagueTitle, leagues );
	if( !m_page1->IsGood() )
	{
		delete m_page;
		m_isGood = false;
		return;
	}
	GetPageAreaSizer()->Add( m_page1 );
	Layout();
	CenterOnScreen();
}

CCreateLeague::~CCreateLeague(void)
{
}

wxWizardPage *CCreateLeague::GetFirstPage()
{
	return m_page1;
}

bool CCreateLeague::RunWizard(wxWizardPage *firstPage)
{
	wxCHECK_MSG( firstPage, false, wxT("can't run empty wizard") );
	// can't return false here because there is no old page
	(void)ShowPage(firstPage, true /* forward */);
	wxButton *finish = static_cast<wxButton *>( wxWindow::FindWindowById( wxID_FORWARD, this ) );
	finish->SetLabel( "Next >" );
	m_wasModal = true;
	return ShowModal() == wxID_OK;
}

CNewLeaguePage::CNewLeaguePage(wxWizard *parent, const CDb &db, int mode, const wxString &leagueTitle, const std::vector<std::string> &leagues) : wxWizardPageSimple( parent )
{
	m_isGood = true;
	m_mode = mode;
	m_db = const_cast<CDb *>( &db );
	m_selectedPage = 0;
	wxLongLong_t leagueId;
	if( m_mode == 1 )
	{
		m_settings = NULL;
		m_isUpdate = false;
		m_oldLeague = wxEmptyString;
	}
	else
	{
		m_isUpdate = true;
		m_settings = new CLeagueSettings( leagueTitle );
		m_oldLeague = leagueTitle;
		if( m_db->GetLeagueSettings( *m_settings, leagueId ) != SQLITE_OK )
		{
			m_isGood = false;
			return;
		}
	}
	m_book = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP );
	m_book->SetBackgroundColour( GetBackgroundColour() );
	m_panel1 = new CNewLeagueName( m_book, *m_db, m_settings, leagues, m_isUpdate );
	m_panel2 = new CNewLeagueOwners( m_book, m_settings, m_isUpdate );
	m_panel3 = new CNewLeaguePlayers( m_book, m_settings );
	m_panel4 = new CNewLeaguePositions( m_book, m_settings );
	m_panel5 = new CNewLeagueScoring( m_book, m_settings, NULL, NULL );
	m_book->AddPage( m_panel1, "League Details", true );
	m_book->AddPage( m_panel2, "Owners", false );
	m_book->AddPage( m_panel3, "Players", false );
	m_book->AddPage( m_panel4, "Positions", false );
	m_book->AddPage( m_panel5, "Scoring", false );
	wxSizer *mainSizer = new wxBoxSizer( wxVERTICAL );
	mainSizer->Add( m_book, 0, wxEXPAND, 0 );
//	SetSizerAndFit( mainSizer );
	SetSizer( mainSizer );
	Layout();
	m_book->Bind( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING, &CNewLeaguePage::OnPageChanging, this );
	m_book->Bind( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, &CNewLeaguePage::OnPageChanged, this );
	wxButton *forward = dynamic_cast<wxButton *>(wxWindow::FindWindowById( wxID_FORWARD, GetParent() ) );
	forward->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CNewLeaguePage::OnWizardNext, this );
	wxButton *backward = dynamic_cast<wxButton *>(wxWindow::FindWindowById( wxID_BACKWARD, GetParent() ) );
	backward->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CNewLeaguePage::OnWizardNext, this );
	m_initialized = false;
}

CNewLeaguePage::~CNewLeaguePage()
{
//	delete m_settings;
}

void CNewLeaguePage::OnPageChanging(wxBookCtrlEvent &event)
{
	int sel = event.GetOldSelection();
	wxPanel *page = dynamic_cast<wxPanel *>( m_book->GetPage( sel ) );
	if( sel == 1 && m_directionForward )
		dynamic_cast<CNewLeagueOwners *>( page )->SetStrictValidation( false );
	else if( sel == 1 )
		dynamic_cast<CNewLeagueOwners *>( page )->SetStrictValidation( true );
	if( !page->Validate() )
	{
		if( m_directionForward )
			m_selectedPage--;
		else
			m_selectedPage++;
		event.Veto();
		return;
	}
	if( !sel )
	{
		CNewLeagueName *pageName = dynamic_cast<CNewLeagueName *>( page );
		if( m_settings && !m_initialized )
		{
			delete m_settings;
			m_settings = NULL;
		}
		if( !m_initialized )
			m_settings = new CLeagueSettings( pageName->GetName(), pageName->GetNumberOfOwners(), pageName->GetDraftType(), pageName->GetScoringType(), pageName->GetRoundType(), pageName->GetSalary(), false );
		else
		{
			m_settings->SetLeagueName( pageName->GetName() );
			m_settings->SetOwnersNumber( pageName->GetNumberOfOwners() );
			m_settings->SetDraftType( pageName->GetDraftType() );
			m_settings->SetScoringtype( pageName->GetScoringType() );
			m_settings->SetRoundingType( pageName->GetRoundType() );
			m_settings->SetSalary( pageName->GetSalary() );
		}
	}
	if( !sel && !m_initialized )
	{
		wxWindow *parent = GetParent();
		wxButton *back = dynamic_cast<wxButton *>( wxWindow::FindWindowById( wxID_BACKWARD, parent ) );
		back->Enable();
		m_initialized = true;
	}
	if( sel == 1 )
	{
		CNewLeagueOwners *panel1 = dynamic_cast<CNewLeagueOwners *>( m_book->GetPage( 1 ) );
		panel1->GetOwnersNames().clear();
		int ownersCount = panel1->GetOwnersNumber();
		std::vector<std::string> owners = m_settings->GetOwners();
		owners.clear();
		for( int i = 0; i < ownersCount; i++ )
		{
			wxTextCtrl *owner = panel1->GetOwner( i );
			std::string value = owner->GetValue().ToStdString();
			if( !value.empty() )
				owners.push_back( value );
		}
		m_settings->SetOwners( owners );
		panel1->SetOwners( owners );
	}
	if( sel == 2 )
	{
		int i;
		CNewLeaguePlayers *panel2 = dynamic_cast<CNewLeaguePlayers *>( m_book->GetPage( 2 ) );
		std::vector<int> *americanLeague = &m_settings->GetAmericanLeague();
		std::vector<int> *nationalLeague = &m_settings->GetNationalLeague();
		americanLeague->clear();
		nationalLeague->clear();
		if( panel2->GetAmericanLeague().IsChecked() )
			americanLeague->push_back( 0 );
		else
		{
			for( i = 0; i < 15; i++ )
			{
				if( panel2->GetAmericanTeams( i )->IsChecked() )
					americanLeague->push_back( i + 1 );
			}
		}
		if( panel2->GetNationalLeague().IsChecked() )
			nationalLeague->push_back( 16 );
		else
		{
			for( i = 0; i < 15; i++ )
			{
				if( panel2->GetNationalTeams( i )->IsChecked() )
					nationalLeague->push_back( i + 17 );
			}
		}
	}
}

void CNewLeaguePage::OnPageChanged(wxBookCtrlEvent &event)
{
	int sel = event.GetSelection();
	wxButton *forward = dynamic_cast<wxButton *>( wxWindow::FindWindowById( wxID_FORWARD, GetParent() ) );
	m_selectedPage = sel;
	if( !sel )
	{
		wxWindow::FindWindowById( wxID_BACKWARD, GetParent() )->Enable( false );
//		delete m_settings;
	}
	if( sel )
	{
		wxWindow *parent = GetParent();
		wxButton *back = dynamic_cast<wxButton *>( wxWindow::FindWindowById( wxID_BACKWARD, parent ) );
		back->Enable();
	}
	if( sel == 1 )
	{
		CNewLeagueOwners *panel2 = dynamic_cast<CNewLeagueOwners *>( m_book->GetPage( 1 ) );
		panel2->SetOwnersNumber( m_settings->GetOwnersNumber() );
	}
	if( sel == 3 && m_settings->GetAmericanLeague().size() == 0 )
	{
		CNewLeaguePositions *panel3 = dynamic_cast<CNewLeaguePositions *>( m_book->GetPage( 3 ) );
		panel3->SetDHValue();
	}
	if( sel == 4 )
		forward->SetLabel( "Save" );
	else
		forward->SetLabel( "Next" );
}

CLeagueSettings &CNewLeaguePage::GetNewLeagueSettings()
{
	return *m_settings;
}

void CNewLeaguePage::OnWizardNext(wxCommandEvent &event)
{
	if( m_selectedPage == 4 && event.GetEventObject() == dynamic_cast<wxButton *>( wxWindow::FindWindowById( wxID_FORWARD, GetParent() ) ) )
	{
		for( unsigned int i = 0; i < m_book->GetPageCount(); i++ )
		{
			wxPanel *page = dynamic_cast<wxPanel *>( m_book->GetPage( i ) );
			if( !page->Validate() )
			{
				m_book->ChangeSelection( i );
				m_selectedPage = i;
				return;
			}
		}
		int i;
		CNewLeaguePositions *panel3 = dynamic_cast<CNewLeaguePositions *>( m_book->GetPage( 3 ) );
		CNewLeagueScoring *panel4 = dynamic_cast<CNewLeagueScoring *>( m_book->GetPage( 4 ) );
		m_settings->SetPositions( panel3->GetPositions() );
		m_settings->SetPlayersInLeague( panel3->GetPositionsCount() );
		m_settings->SetBenchPlayerIncluded( panel3->IncludeBenchPlayers() );
		std::vector<wxString> *hits = &m_settings->GetHitScoring();
		std::vector<wxString> *pitches = &m_settings->GetPitchScoring();
		for( i = 0; i < 25; i++ )
		{
			if( panel4->GetHitScore( i )->IsChecked() )
				hits->push_back( panel4->GetHitScore( i )->GetLabel() );
			if( panel4->GetPitchScore( i )->IsChecked() )
				pitches->push_back( panel4->GetPitchScore( i )->GetLabel() );
		}
		if( m_db->CreateLeague( *m_settings, m_isUpdate, m_oldLeague, m_leagueId ) )
			dynamic_cast<wxWizard *>( GetParent() )->EndModal( wxID_OK );
		else
			dynamic_cast<wxWizard *>( GetParent() )->EndModal( wxID_CANCEL );
	}
	else
	{
		wxButton *button = dynamic_cast<wxButton *>( event.GetEventObject() );
		if( button == dynamic_cast<wxButton *>( wxWindow::FindWindowById( wxID_FORWARD, GetParent() ) ) )
		{
			m_directionForward = true;
			m_selectedPage++;
		}
		else
		{
			m_directionForward = false;
			m_selectedPage--;
		}
		m_book->SetSelection( m_selectedPage );
	}
}

wxPanel *CNewLeaguePage::GetFirstPage()
{
	return m_panel1;
}

CLeagueSettings &CCreateLeague::GetLeague()
{
	return dynamic_cast<CNewLeaguePage *>( GetFirstPage() )->GetNewLeagueSettings();
}

wxLongLong_t CCreateLeague::GetLeagueId()
{
	return dynamic_cast<CNewLeaguePage *>( GetFirstPage() )->GetLeagueId();
}

wxLongLong_t CNewLeaguePage::GetLeagueId()
{
	return m_leagueId;
}

bool CNewLeaguePage::IsGood()
{
	return m_isGood;
}

bool CCreateLeague::IsGood()
{
	return m_isGood;
}
