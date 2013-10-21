// BaseballDraft.cpp : Defines the entry point for the application.
//
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef __WXMSW__
#if defined __LEAK_DETECTOR__
#include "vld.h"
#endif
#endif

#include <vector>
#include <string.h>
#include <map>
#include "sqlite3.h"
#include "wx/wizard.h"
#include "wx/tipdlg.h"
#include "wx/textcompleter.h"
#include "wx/hyperlink.h"
#include "wx/tglbtn.h"
#include "wx/grid.h"
#include "textctrl.h"
#include "leaguesettings.h"
#include "namecompleter.h"
#include "playerdraft.h"
#include "db.h"
#include "playerspanel.h"
#include "teamsroster.h"
#include "teamprojections.h"
#include "draftresultsalary.h"
#include "draftresultpanel.h"
#include "draftresult.h"
#include "gridtotalcell.h"
#include "frame.h"
#include "startdialog.h"
#include "BaseballDraft.h"

IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	bool result = true;
	int res;
	wxString name;
	m_leagueSettings = NULL;
	if( !wxApp::OnInit() )
		result = false;
	else
	{
		wxImage::AddHandler( new wxPNGHandler );
		CStartDialog dlg( "Select Or Create League" );
		if( !dlg.IsDialogGood() )
			result = false;
		else
		{
			dlg.RunWizard( dlg.GetFirstPage() );
			name = dlg.GetLeagueName();
			m_leagueSettings = dlg.GetSelectedLeague();
			m_leagueId = dlg.GetLeagueId();
			m_db = dlg.GetDatabase();
			res = dlg.GetReturnCode();
			dlg.Destroy();
			if( res == wxID_CANCEL )
			{
				delete m_db;
				result = false;
			}
			else
			{
				CFrame *frame = new CFrame( "Player Draft", m_leagueSettings, *m_db, name, m_leagueId );
				if( frame->IsGood() )
				{
					frame->Maximize();
					frame->Show( true );
				}
				else
					return false;
			}
		}
	}
	return result;
}

int MyApp::OnExit()
{
	return 0;
}

