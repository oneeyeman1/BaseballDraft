// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include <map>
#include "wx/propgrid/propgrid.h"
#include "wx/propgrid/advprops.h"
#include "wx/spinctrl.h"
#include "leaguesettings.h"
#include "newleaguepositions.h"

CNewLeaguePositions::CNewLeaguePositions(wxWindow *parent, const CLeagueSettings *settings) : wxPanel( parent )
{
	m_positions = new CPropertyGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED );
	m_positions->Append( new wxIntProperty( "C", wxPG_LABEL, !settings ? 2 : settings->GetPositions()["C"] ) );
	m_positions->SetPropertyEditor( "C", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "C", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "C", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "1B", wxPG_LABEL, !settings ? 1 : settings->GetPositions()["1B"] ) );
	m_positions->SetPropertyEditor( "1B", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "1B", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "1B", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "2B", wxPG_LABEL, !settings ? 1 : settings->GetPositions()["2B"] ) );
	m_positions->SetPropertyEditor( "2B", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "2B", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "2B", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "SS", wxPG_LABEL, !settings ? 1 : settings->GetPositions()["SS"] ) );
	m_positions->SetPropertyEditor( "SS", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "SS", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "SS", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "3B", wxPG_LABEL, !settings ? 1 : settings->GetPositions()["3B"] ) );
	m_positions->SetPropertyEditor( "3B", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "3B", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "3B", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "MI", wxPG_LABEL, !settings ? 1 : settings->GetPositions()["MI"] ) );
	m_positions->SetPropertyEditor( "MI", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "MI", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "MI", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "CI", wxPG_LABEL, !settings ? 1 : settings->GetPositions()["CI"] ) );
	m_positions->SetPropertyEditor( "CI", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "CI", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "CI", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "OF", wxPG_LABEL, !settings ? 5 : settings->GetPositions()["OF"] ) );
	m_positions->SetPropertyEditor( "OF", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "OF", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "OF", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "DH", wxPG_LABEL, !settings ? 1 : settings->GetPositions()["DH"] ) );
	m_positions->SetPropertyEditor( "DH", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "DH", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "DH", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "Utility", wxPG_LABEL, !settings ? 1 : settings->GetPositions()["Utility"] ) );
	m_positions->SetPropertyEditor( "Utility", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "Utility", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "Utility", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "SP", wxPG_LABEL, !settings ? 0 : settings->GetPositions()["SP"] ) );
	m_positions->SetPropertyEditor( "SP", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "SP", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "SP", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "RP", wxPG_LABEL, !settings ? 0 : settings->GetPositions()["RP"] ) );
	m_positions->SetPropertyEditor( "RP", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "RP", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "RP", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "P", wxPG_LABEL, !settings ? 9 : settings->GetPositions()["P"] ) );
	m_positions->SetPropertyEditor( "P", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "P", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "P", wxPG_ATTR_MAX, (int) 99 );
	m_positions->Append( new wxIntProperty( "Bench", wxPG_LABEL, !settings ? 10 : settings->GetPositions()["Bench"] ) );
	m_positions->SetPropertyEditor( "Bench", wxPGEditor_SpinCtrl );
	m_positions->SetPropertyAttribute( "Bench", wxPG_ATTR_MIN, (int) 0 );
	m_positions->SetPropertyAttribute( "Bench", wxPG_ATTR_MAX, (int) 99 );
	m_benchPlayers = new wxCheckBox( this, wxID_ANY, "Include Bench Players in salary cap" );
	if( settings )
		m_benchPlayers->SetValue( settings->IsBenchPlayerIncluded() );
	m_label1 = new wxStaticText( this, wxID_ANY, "Positions" );
	wxSizer *mainSizer = new wxBoxSizer( wxVERTICAL );
	wxSizer *secondSizer = new wxBoxSizer( wxHORIZONTAL );
	wxSizer *controls = new wxBoxSizer( wxVERTICAL );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	secondSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	controls->Add( m_label1, 0, wxEXPAND, 0 );
	controls->Add( 5, 5, 0, wxEXPAND, 0 );
	controls->Add( m_positions, 1, wxEXPAND, 0 );
	controls->Add( 5, 5, 0, wxEXPAND, 0 );
	controls->Add( m_benchPlayers, 0, wxEXPAND, 0 );
	secondSizer->Add( controls, 0, wxEXPAND, 0 );
	secondSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	mainSizer->Add( secondSizer, 0, wxEXPAND, 0 );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	SetSizer( mainSizer );
	Layout();
}

CNewLeaguePositions::~CNewLeaguePositions(void)
{
}

/*bool CNewLeaguePositions::Show(bool show)
{
	bool result = wxPanel::Show( show );
	m_positions->SetFocus();
	return result;
}
*/
bool CNewLeaguePositions::Validate()
{
	bool validated = true;
	wxPropertyGridIterator it;
	int goodValue = 0;
	for( it = m_positions->GetIterator(); !it.AtEnd() && !goodValue; it++ )
	{
		wxPGProperty *prop = *it;
		int value = ((wxAny) prop->GetValue()).As<int>();
		if( value > 0 && value <= 20 )
			goodValue++;
	}
	if( !goodValue )
	{
		wxMessageBox( "At least one value should be in range 1-20" );
		validated = false;
	}
	return validated;
}

std::map<wxString,int> &CNewLeaguePositions::GetPositions()
{
	wxPropertyGridIterator it;
	for( it = m_positions->GetIterator(); !it.AtEnd(); it++ )
	{
		wxPGProperty *prop = *it;
		wxAny value = prop->GetValue();
		if( prop->GetName() == "C" )
			m_properties["C"] = value.As<int>();
		else if( prop->GetName() == "1B" )
			m_properties["1B"] = value.As<int>();
		else if( prop->GetName() == "2B" )
			m_properties["2B"] = value.As<int>();
		else if( prop->GetName() == "SS" )
			m_properties["SS"] = value.As<int>();
		else if( prop->GetName() == "3B" )
			m_properties["3B"] = value.As<int>();
		else if( prop->GetName() == "MI" )
			m_properties["MI"] = value.As<int>();
		else if( prop->GetName() == "CI" )
			m_properties["CI"] = value.As<int>();
		else if( prop->GetName() == "OF" )
			m_properties["OF"] = value.As<int>();
		else if( prop->GetName() == "DH" )
			m_properties["DH"] = value.As<int>();
		else if( prop->GetName() == "Utility" )
			m_properties["Utility"] = value.As<int>();
		else if( prop->GetName() == "SP" )
			m_properties["SP"] = value.As<int>();
		else if( prop->GetName() == "RP" )
			m_properties["RP"] = value.As<int>();
		else if( prop->GetName() == "P" )
			m_properties["P"] = value.As<int>();
		else if( prop->GetName() == "Bench" )
			m_properties["Bench"] = value.As<int>();
	}
	return m_properties;
}

bool CNewLeaguePositions::IncludeBenchPlayers()
{
	return m_benchPlayers->IsChecked();
}

void CNewLeaguePositions::SetDHValue()
{
	m_positions->SetPropertyValue( "DH", 0 );
}
