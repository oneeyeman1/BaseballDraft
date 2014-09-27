// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include <map>
#include <utility>
#include "wx/tipdlg.h"
#include "wx/gbsizer.h"
#include "leaguesettings.h"
#include "newleaguescoring.h"

CNewLeagueScoring::CNewLeagueScoring(wxWindow *parent, CLeagueSettings *settings, const std::map<wxString,bool> *displayedHitters, const std::map<wxString,bool> *displayedPitchers) : wxPanel( parent )
{
	int i;
	m_pitchers = const_cast<std::map<wxString,bool> *>( displayedPitchers );
	m_hitters = const_cast<std::map<wxString,bool> *>( displayedHitters );
	const wxString hitScoring[] = 
	{
		wxT( "AVG" ),
		wxT( "HR" ),
		wxT( "Runs" ),
		wxT( "RBI" ),
		wxT( "SB" ),
		wxT( "OBP" ),
		wxT( "SLG" ),
		wxT( "OPS" ),
		wxT( "BB" ),
		wxT( "H" ),
		wxT( "K" ),
		wxT( "1B" ),
		wxT( "2B" ),
		wxT( "3B" ),
		wxT( "TB" ),
		wxT( "2B+3B+HR" ),
		wxT( "2B+3B" ),
		wxT( "SB-CS" ),
		wxT( "CS" ),
		wxT( "R+RBI-HR" ),
		wxT( "H+BB" ),
		wxT( "TB+BB" ),
		wxT( "AB" ),
		wxT( "HBP" ),
		wxT( "GDP" ),
		wxT( "SF" )
	};
	const wxString pitchScoring[] =
	{
		wxT( "Wins" ),
		wxT( "Saves" ),
		wxT( "ERA" ),
		wxT( "WHIP" ),
		wxT( "K" ),
		wxT( "IP" ),
		wxT( "L" ),
		wxT( "H" ),
		wxT( "BB" ),
		wxT( "ER" ),
		wxT( "CG" ),
		wxT( "W-L" ),
		wxT( "QS" ),
		wxT( "Shutouts" ),
		wxT( "BS" ),
		wxT( "S-BS" ),
		wxT( "S+Holds" ),
		wxT( "S+Holds-BS" ),
		wxT( "K/9" ),
		wxT( "H/9" ),
		wxT( "BB/9" ),
		wxT( "K/BB" ),
		wxT( "K-BB" ),
		wxT( "W%" ),
		wxT( "Holds" ),
		wxT( "G" ),
		wxT( "R" ),
		wxT( "HR" )
	};
	wxSizer *mainSizer = new wxBoxSizer( wxVERTICAL );
	wxSizer *secondSizer = new wxBoxSizer( wxHORIZONTAL );
	wxSizer *controls = new wxBoxSizer( wxHORIZONTAL );
	wxStaticBoxSizer *hit = new wxStaticBoxSizer( wxVERTICAL, this, "Select Hitting Scoring Categories" );
	wxStaticBoxSizer *pitch = new wxStaticBoxSizer( wxVERTICAL, this, "Select Pitching Scoring Categories" );
	wxGridBagSizer *hitSizer = new wxGridBagSizer( 15, 5 );
	wxGridBagSizer *pitchSizer = new wxGridBagSizer( 15, 5 );

	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	secondSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	int row = 0, col = 0;
	for( i = 0; i < 26; i++ )
	{
		m_hitScoring[i] = new wxCheckBox( hit->GetStaticBox(), wxID_ANY, hitScoring[i], wxDefaultPosition, wxDefaultSize, wxCHK_2STATE, wxDefaultValidator, "HitCategories" );
		if( displayedHitters )
			m_hitScoring[i]->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CNewLeagueScoring::OnCategoryClicked, this );
		hitSizer->Add( m_hitScoring[i], wxGBPosition( row, col ), wxGBSpan( 1, 1 ) );
		row++;
		if( row == 13 )
		{
			row = 0;
			col = 1;
		}
	}
	if( displayedHitters )
	{
		for( std::map<wxString,bool>::iterator it = const_cast<std::map<wxString,bool> *>( displayedHitters )->begin(); it != displayedHitters->end(); it++ )
		{
			bool found = false;
			for( i = 0; i < 26 && !found; i++ )
				if( hitScoring[i] == (*it).first )
				{
					found = true;
					m_hitScoring[i]->SetValue( (*it).second );
				}
		}
	}
	hit->Add( hitSizer, 0, wxEXPAND, 0 );
	controls->Add( hit, 0, wxEXPAND, 0 );
	controls->Add( 15, 5, 0, wxEXPAND, 0 );
	row = col = 0;
	for( i = 0; i < 28; i++ )
	{
		m_pitchScoring[i] = new wxCheckBox( pitch->GetStaticBox(), wxID_ANY, pitchScoring[i], wxDefaultPosition, wxDefaultSize, wxCHK_2STATE, wxDefaultValidator, "PitchCategories" );
		if( displayedPitchers )
			m_pitchScoring[i]->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CNewLeagueScoring::OnCategoryClicked, this );
		pitchSizer->Add( m_pitchScoring[i], wxGBPosition( row, col ), wxGBSpan( 1, 1 ) );
		row++;
		if( row == 14 )
		{
			if( col == 0 )
			{
				pitchSizer->Add( 5, 5, wxGBPosition( 15, 0 ), wxGBSpan( 1, 1 ) );
				pitchSizer->Add( 5, 5, wxGBPosition( 15, 1 ), wxGBSpan( 1, 1 ) );
			}
			row = 0;
			col = 1;
		}
	}
	if( displayedPitchers )
	{
		for( std::map<wxString,bool>::iterator it = const_cast<std::map<wxString,bool> *>( displayedPitchers )->begin(); it != displayedPitchers->end(); it++ )
		{
			bool found = false;
			for( i = 0; i < 28 && !found; i++ )
				if( pitchScoring[i] == (*it).first )
				{
					found = true;
					m_pitchScoring[i]->SetValue( (*it).second );
				}
		}
	}
	pitch->Add( pitchSizer, 0, wxEXPAND, 0 );
	controls->Add( pitch, 0, wxEXPAND, 0 );
	secondSizer->Add( controls, 0, wxEXPAND, 0 );
	secondSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	mainSizer->Add( secondSizer, 0, wxEXPAND, 0 );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	if( (!displayedHitters && !displayedPitchers ) && settings )
	{
		std::vector<wxString> hits = settings->GetHitScoring();
		std::vector<wxString> pitches = settings->GetPitchScoring();
		std::vector<wxString>::iterator it;
		for( it = hits.begin(); it < hits.end(); it++ )
		{
			for( int i = 0; i < 26; i++ )
				if( hitScoring[i] == *it )
					m_hitScoring[i]->SetValue( true );
		}
		for( it = pitches.begin(); it < pitches.end(); it++ )
		{
			for( int i = 0; i < 26; i++ )
				if( pitchScoring[i] == *it )
					m_pitchScoring[i]->SetValue( true );
		}
	}
	else
	{
		m_hitScoring[0]->SetValue( true );
		m_hitScoring[1]->SetValue( true );
		m_hitScoring[2]->SetValue( true );
		m_hitScoring[3]->SetValue( true );
		m_hitScoring[4]->SetValue( true );
		m_pitchScoring[0]->SetValue( true );
		m_pitchScoring[1]->SetValue( true );
		m_pitchScoring[2]->SetValue( true );
		m_pitchScoring[3]->SetValue( true );
		m_pitchScoring[4]->SetValue( true );
	}
	SetSizer( mainSizer );
	Layout();
}

CNewLeagueScoring::~CNewLeagueScoring(void)
{
}

bool CNewLeagueScoring::Validate()
{
	bool validated = true, hits = false, pitches = false;
	int i;
	for( i = 0; i < 25 && !hits; i++ )
	{
		if( m_hitScoring[i]->IsChecked() )
			hits = true;
	}
	for( i = 0; i < 25 && !pitches; i++ )
	{
		if( m_pitchScoring[i]->IsChecked() )
			pitches = true;
	}
	if( !hits && !pitches )
	{
		wxMessageBox( "Please select at least one HIT category and one PITCH category." );
		validated = false;
	}
	return validated;
}

wxCheckBox *CNewLeagueScoring::GetHitScore(int pos)
{
	return m_hitScoring[pos];
}

wxCheckBox *CNewLeagueScoring::GetPitchScore(int pos)
{
	return m_pitchScoring[pos];
}

void CNewLeagueScoring::OnCategoryClicked(wxCommandEvent &event)
{
	wxCheckBox *category = dynamic_cast<wxCheckBox *>( event.GetEventObject() );
	if( category->GetName() == "HitCategories" )
	{
		bool found = false;
		for( std::map<wxString,bool>::iterator it = m_hitters->begin(); it != m_hitters->end() && !found; it++ )
		{
			if( category->GetLabel() == (*it).first )
			{
				found = true;
				if( category->IsChecked() )
					(*it).second = true;
				else
					(*it).second = false;
			}
		}
		if( !found )
			m_hitters->insert( std::make_pair( category->GetLabel(), true ) );
	}
	else
	{
		bool found = false;
		for( std::map<wxString,bool>::iterator it = m_pitchers->begin(); it != m_pitchers->end() && !found; it++ )
		{
			if( category->GetLabel() == (*it).first )
			{
				found = true;
				if( category->IsChecked() )
					(*it).second = true;
				else
					(*it).second = false;
			}
		}
		if( !found )
			m_pitchers->insert( std::make_pair( category->GetLabel(), true ) );
	}
}

std::map<wxString,bool> &CNewLeagueScoring::GetHitterScoring()
{
	return *m_hitters;
}

std::map<wxString,bool> &CNewLeagueScoring::GetPitcherScoring()
{
	return *m_pitchers;
}

