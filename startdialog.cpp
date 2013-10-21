// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <vector>
#include <algorithm>
#include <string.h>
#include <map>
#include "sqlite3.h"
#include "wx/tipdlg.h"

#include "wx/wfstream.h"
#include "wx/zipstrm.h"

#include "wx/wizard.h"
#include "wx/spinctrl.h"
#include "wx/notebook.h"
#include "wx/hyperlink.h"
#include "leaguesettings.h"
#include "db.h"
#include "dataimporter.h"
#include "newleaguename.h"
#include "createleague.h"
#include "startdialog.h"

CStartDialog::CStartDialog(const wxString &title) : wxWizard( NULL, wxID_ANY, title, wxNullBitmap )
{
	std::vector<std::vector<wxString> > hitters, pitchers;
	m_good = true;
//	CDataImporter importer( "2012_DEMO_DB.6.xlsx" );
//	importer.ReadData( hitters, pitchers );
	wxIconBundle bundle;
	bundle.AddIcon( "Baseball-icon.16.png" );
	bundle.AddIcon( "Baseball-icon.32.png" );
	SetIcons( bundle );
	m_db = new CDb();
	m_db->OpenConnection( "draft.db" );
	m_league = NULL;
	if( !m_db->IsConnected() )
		wxMessageBox( "Cannot establish connection to database" );
	else
	{
//		m_db->InsertPlayers( hitters, pitchers );
		m_db->GetLeagueList( m_leaguesList );
		if( !m_leaguesList.empty() )
		{
			GetPageAreaSizer()->Add( CreateOrSelectLeagueWizard( this, m_leaguesList ) );
			CenterOnScreen();
			Bind( wxEVT_WIZARD_CANCEL, &CStartDialog::OnExit, this );
			m_delete->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CStartDialog::OnLeagueDelete, this );
			m_new->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CStartDialog::OnLeagueCreated, this );
			m_edit->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CStartDialog::OnLeagueCreated, this );
			m_leagues->Bind( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, &CStartDialog::OnSelect, this );
		}
		else
		{
			m_good = false;
			delete m_db;
		}
	}
}

CStartDialog::~CStartDialog(void)
{
}

wxPanel *CStartDialog::CreateOrSelectLeagueWizard(wxWizard *parent, std::vector<std::string> &leagues)
{
	m_panel = new wxWizardPageSimple( parent );
	wxTheColourDatabase->AddColour( "NAVY", wxColour( 0, 0, 58 ) );
	m_label = new wxStaticText( m_panel, wxID_ANY, "Select League" );
	m_label->SetForegroundColour( wxTheColourDatabase->Find( "NAVY" ) );
	m_labelLogo = new wxStaticBitmap( m_panel, wxID_ANY, wxBitmap( "27330AOP1.png", wxBITMAP_TYPE_PNG ) );
	m_label1 = new wxStaticText( m_panel, wxID_ANY, "Mode: Demo" );
	m_label2 = new wxStaticText( m_panel, wxID_ANY, "Version: 1.0" );
	m_url = new wxHyperlinkCtrl( m_panel, wxID_ANY, "www.valuedraft.com", "http://www.valuedraft.com" );
	m_leagues = new wxListBox( m_panel, wxID_ANY );
	m_new = new wxButton( m_panel, wxID_ANY, "Create New League" );
	m_edit = new wxButton( m_panel, wxID_ANY, "Edit Existing League" );
	m_delete = new wxButton( m_panel, wxID_ANY, "Delete League" );
	wxFont font = m_label->GetFont();
	font.SetWeight( wxFONTWEIGHT_BOLD );
	font.SetPointSize( 24 );
	m_label->SetFont( font );
	wxSizer *horz1 = new wxBoxSizer( wxHORIZONTAL );
	wxSizer *vert1 = new wxBoxSizer( wxVERTICAL );
	wxSizer *holder = new wxFlexGridSizer( 3, 2, 5, 5 );
	wxSizer *logo = new wxBoxSizer( wxVERTICAL );
	wxSizer *buttons = new wxBoxSizer( wxVERTICAL );
	horz1->Add( 5, 5, 0, wxEXPAND, 0 );
	vert1->Add( 5, 5, 0, wxEXPAND, 0 );
	holder->Add( m_labelLogo, 0, wxEXPAND, 0 );
	logo->Add( m_label1, 0, wxEXPAND, 0 );
	logo->Add( 5, 5, 0, wxEXPAND, 0 );
	logo->Add( m_label2, 0, wxEXPAND, 0 );
	logo->Add( 5, 5, 0, wxEXPAND, 0 );
	logo->Add( m_url, 0, wxEXPAND, 0 );
	holder->Add( logo, 0, wxEXPAND, 0 );
	holder->Add( m_label, 0, wxEXPAND, 0 );
	holder->Add( 5, 5, 0, wxEXPAND, 0 );
	holder->Add( m_leagues, 0, wxEXPAND, 0 );
	buttons->Add( m_new, 0, wxEXPAND, 0 );
	buttons->Add( 5, 5, 0, wxEXPAND, 0 );
	buttons->Add( m_edit, 0, wxEXPAND, 0 );
	buttons->Add( 5, 5, 0, wxEXPAND, 0 );
	buttons->Add( m_delete, 0, wxEXPAND, 0 );
	holder->Add( buttons, 0, wxEXPAND, 0 );
	vert1->Add( holder, 0, wxEXPAND, 0 );
	vert1->Add( 5, 5, 0, wxEXPAND, 0 );
	horz1->Add( vert1, 0, wxEXPAND, 0 );
	horz1->Add( 5, 5, 0, wxEXPAND, 0 );
	for( std::vector<std::string>::iterator it = leagues.begin(); it < leagues.end(); it++ )
		m_leagues->Append( *it );
	m_panel->SetSizerAndFit( horz1 );
	m_leagues->SetFocus();
	m_leagues->SetSelection( 0 );
	return m_panel;
}

void CStartDialog::OnExit(wxWizardEvent &event)
{
	int style = wxYES_NO;
	if( wxMessageBox( "Are you sure you want to exit?", "Confirmation", style ) == wxYES )
		EndModal( wxID_CANCEL );
	else
		event.Veto();
}

void CStartDialog::OnSelect(wxCommandEvent &WXUNUSED(event))
{
	EndDialog( wxID_OK );
}

wxWizardPage *CStartDialog::GetFirstPage()
{
	return m_panel;
}

void CStartDialog::OnLeagueDelete(wxCommandEvent &WXUNUSED(event))
{
	wxString league = m_leagues->GetStringSelection();
	int responce = wxMessageBox( "Are you sure you want to delete the League: " + league + ". This can't be undone.", "Warning", wxYES_NO | wxNO_DEFAULT );
	if( responce == wxYES )
	{
		if( m_db->DeleteLeague( league, false ) )
		{
			m_leagues->Delete( m_leagues->GetSelection() );
			m_leaguesList.erase( std::remove( m_leaguesList.begin(), m_leaguesList.end(), league ) );
			m_leagues->SetSelection( 0 );
		}
		else
			m_leagues->SetSelection( 0 );
	}
}

void CStartDialog::OnLeagueCreated(wxCommandEvent &event)
{
	int mode;
	wxString title, leagueTitle;
	if( event.GetEventObject() == m_new )
	{
		title = "Create New League";
		mode = 1;
		leagueTitle = wxEmptyString;
	}
	else
	{
		title = "Edit Existing League";
		mode = 2;
		leagueTitle = m_leagues->GetStringSelection();
	}
	CCreateLeague dlg( title, *m_db, mode, leagueTitle, m_leaguesList );
	if( !dlg.IsGood() )
		return;
	int res = dlg.RunWizard( dlg.GetFirstPage() );
	if( res )
	{
		m_league = &dlg.GetLeague();
		m_leagueId = dlg.GetLeagueId();
		dlg.Destroy();
		EndModal( res );
	}
	else
	{
		delete &dlg.GetLeague();
		dlg.Destroy();
	}
}

bool CStartDialog::RunWizard(wxWizardPage *firstPage)
{
	wxCHECK_MSG( firstPage, false, wxT("can't run empty wizard") );
	// can't return false here because there is no old page
	(void)ShowPage(firstPage, true /* forward */);
	wxButton *finish = static_cast<wxButton *>( wxWindow::FindWindowById( wxID_FORWARD ) );
	finish->SetLabel( "Select" );
	m_wasModal = true;
	return ShowModal() == wxID_OK;
}

CLeagueSettings *CStartDialog::GetSelectedLeague()
{
	return m_league;
}

CDb *CStartDialog::GetDatabase()
{
	return m_db;
}

wxString CStartDialog::GetLeagueName()
{
	if( !m_league )
		return m_leagues->GetStringSelection();
	else
		return m_league->GetLeagueName();
}

int CStartDialog::GetLeagueId()
{
	return m_leagueId;
}

bool CStartDialog::IsDialogGood()
{
	return m_good;
}
