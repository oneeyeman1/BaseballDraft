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
#include "wx/propgrid/propgrid.h"
#include "leaguesettings.h"
#include "edittargets.h"

CEditTargets::CEditTargets(wxWindow *parent, const CLeagueData &data) : wxDialog( parent, wxID_ANY, "Enter Targets" )
{
	m_label1 = new wxStaticText( this, wxID_ANY, "Hitting Targets" );
	m_label2 = new wxStaticText( this, wxID_ANY, "Pitching Targets" );
	std::map<wxString,double> hitTargets = data.m_settings->GetTargetsForHitters();
	std::map<wxString,double>pitchTargets = data.m_settings->GetTargetsForPitchers();
	std::map<wxString,double>::iterator it;
	m_hits = new wxPropertyGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED );
	m_pitches = new wxPropertyGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED );
	for( std::vector<wxString>::iterator it1 = data.m_settings->GetHitScoring().begin(); it1 < data.m_settings->GetHitScoring().end(); it1++ )
	{
		m_hits->Append( new wxFloatProperty( (*it1), wxPG_LABEL, hitTargets.size() == 0 || hitTargets.find( (*it1) ) == hitTargets.end() ? 0 : hitTargets[(*it1)] ) );
	}
	for( std::vector<wxString>::iterator it2 = data.m_settings->GetPitchScoring().begin(); it2 < data.m_settings->GetPitchScoring().end(); it2++ )
	{
		m_pitches->Append( new wxFloatProperty( (*it2), wxPG_LABEL, pitchTargets.size() == 0 || pitchTargets.find( (*it2) ) == pitchTargets.end() ? 0 : pitchTargets[(*it2)] ) );
	}
	wxBoxSizer *mainSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *vertical = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *controls = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *hits = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *pitches = new wxBoxSizer( wxVERTICAL );
	wxStdDialogButtonSizer *buttons = new wxStdDialogButtonSizer();
	buttons->AddButton( new wxButton( this, wxID_OK ) );
	buttons->AddButton( new wxButton( this, wxID_CANCEL ) );
	buttons->Realize();
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	hits->Add( m_label1, 0, wxEXPAND, 0 );
	hits->Add( 5, 5, 0, wxEXPAND, 0 );
	hits->Add( m_hits, 0, wxEXPAND, 0 );
	controls->Add( hits, 0, wxEXPAND, 0 );
	controls->Add( 15, 15, 0, wxEXPAND, 0 );
	pitches->Add( m_label2, 0, wxEXPAND, 0 );
	pitches->Add( 5, 5, 0, wxEXPAND, 0 );
	pitches->Add( m_pitches, 0, wxEXPAND, 0 );
	controls->Add( pitches, 0, wxEXPAND, 0 );
	vertical->Add( controls, 0, wxEXPAND, 0 );
	vertical->Add( 10, 10, 0, wxEXPAND, 0 );
	vertical->Add( buttons, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	mainSizer->Add( vertical, 0, wxEXPAND, 0 );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	SetSizerAndFit( mainSizer );
	Layout();
}

CEditTargets::~CEditTargets(void)
{
}

double CEditTargets::GetHitTarget(const wxString &category)
{
	wxPropertyGridIterator it;
	for( it = m_hits->GetIterator(); !it.AtEnd(); it++ )
	{
		wxPGProperty *prop = *it;
		wxAny value = prop->GetValue();
		if( prop->GetName() == category )
			return value.As<double>();
	}
	return 0;
}

double CEditTargets::GetPitchTarget(const wxString &category)
{
	wxPropertyGridIterator it;
	for( it = m_pitches->GetIterator(); !it.AtEnd(); it++ )
	{
		wxPGProperty *prop = *it;
		wxAny value = prop->GetValue();
		if( prop->GetName() == category )
			return value.As<double>();
	}
	return 0;
}
