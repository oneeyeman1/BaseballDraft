// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <map>
#include <vector>
#include "wx/tipdlg.h"
#include "wx/grid.h"
#include "leaguesettings.h"
#include "draftresultsalary.h"
#include "draftresultpanel.h"
#include "draftresult.h"

CDraftResult::CDraftResult(wxWindow *parent, const CLeagueSettings &settings, std::vector<CPlayer> &draftResult) : wxPanel( parent )
{
	m_results = new wxButton( this, wxID_ANY, "Draft Result" );
	m_salary = new wxButton( this, wxID_ANY, "Salary" );
	m_panel1 = new CDraftResultPanel( this, settings, draftResult );
	m_panel2 = new CDraftResultSalary( this, settings, draftResult );
	m_panelSizer = new wxBoxSizer( wxHORIZONTAL );
	mainSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *vertical = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *top = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *panels = new wxBoxSizer( wxVERTICAL );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
//	top->Add( 5, 5, wxEXPAND, 0 );
	top->Add( m_results, 1, wxEXPAND, 0 );
//	top->Add( 5, 5, 0, wxEXPAND, 0 );
	top->AddStretchSpacer();
	top->Add( m_salary, 1, wxEXPAND, 0 );
	vertical->Add( top, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	m_panelSizer->Add( m_panel1, 1, wxEXPAND, 0 );
	m_panelSizer->Add( m_panel2, 1, wxEXPAND, 0 );
	panels->Add( m_panelSizer, 1, wxEXPAND, 0 );
	vertical->Add( panels, 1, wxEXPAND, 0 );
	mainSizer->Add( vertical, 1, wxEXPAND, 0 );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	SetSizerAndFit( mainSizer );
	m_panel2->Hide();
	m_results->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CDraftResult::OnResults, this );
	m_salary->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CDraftResult::OnSalary, this );
}

CDraftResult::~CDraftResult(void)
{
}

CDraftResultPanel &CDraftResult::GetDraftResultPanel()
{
	return *m_panel1;
}

CDraftResultSalary &CDraftResult::GetDraftResultSalaryPanel()
{
	return *m_panel2;
}

void CDraftResult::OnResults(wxCommandEvent &WXUNUSED(event))
{
	m_panel1->Show();
	m_panel2->Hide();
	m_panelSizer->Layout();
}

void CDraftResult::OnSalary(wxCommandEvent &WXUNUSED(event))
{
	m_panel1->Hide();
	m_panel2->Show();
//	m_panel2->Resize();
	mainSizer->RecalcSizes();
}

void CDraftResult::UnAssignPlayer(const CPlayer &player)
{
	m_panel1->UnAssignPlayer( player );
	m_panel2->UnAssignPlayer( player ); 
}

void CDraftResult::ResetDraftResult()
{
	m_panel1->ResetPanel();
	m_panel2->ResetPanel();
}
