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
#include "wx/gbsizer.h"
#include "leaguesettings.h"
#include "newleagueplayers.h"

CNewLeaguePlayers::CNewLeaguePlayers(wxWindow *parent, CLeagueSettings *settings) : wxPanel( parent )
{
	wxStaticBoxSizer *americanLeague = new wxStaticBoxSizer( wxVERTICAL, this, "American League Teams" );
	wxStaticBoxSizer *nationalLeague = new wxStaticBoxSizer( wxVERTICAL, this, "National League Teams" );
	m_american = new wxCheckBox( americanLeague->GetStaticBox(), wxID_ANY, "All AL Teams", wxDefaultPosition, wxDefaultSize );
	m_national = new wxCheckBox( nationalLeague->GetStaticBox(), wxID_ANY, "All NL Teams", wxDefaultPosition, wxDefaultSize );
	int i;
	wxSizer *mainSizer = new wxBoxSizer( wxVERTICAL );
	wxSizer *secondSizer = new wxBoxSizer( wxHORIZONTAL );
	wxSizer *controls = new wxBoxSizer( wxHORIZONTAL );
	wxGridBagSizer *americanSizer = new wxGridBagSizer( 5, 5 );
	wxGridBagSizer *nationalSizer = new wxGridBagSizer( 5, 5 );

	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	secondSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	americanSizer->Add( m_american, wxGBPosition( 0, 0 ), wxGBSpan( 1, 2 ) );
	const wxString americanTeamName[] = 
	{
		wxT( "Baltimore Orioles" ),
		wxT( "Boston Red Sox" ),
		wxT( "Chicago White Sox" ),
		wxT( "Cleveland Indians" ),
		wxT( "Detroit Tigers" ),
		wxT( "Houston Astros" ),
		wxT( "Kansas City Royals" ),
		wxT( "Los Angeles Angels" ),
		wxT( "Minnesota Twins" ),
		wxT( "New York Yankees" ),
		wxT( "Oakland A’s" ),
		wxT( "Seattle Mariners" ),
		wxT( "Tampa Bay Rays" ),
		wxT( "Texas Rangers" ),
		wxT( "Toronto Blue Jays" )
	};
	for( i = 0; i < 15; i++ )
	{
		m_americanTeams[i] = new wxCheckBox( americanLeague->GetStaticBox(), wxID_ANY, americanTeamName[i], wxDefaultPosition, wxDefaultSize, 0L, wxDefaultValidator, "AmericanLeague" );
		m_americanTeams[i]->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CNewLeaguePlayers::OnTeamSelect, this );
		americanSizer->Add( 15, 5, wxGBPosition( i + 1, 0 ), wxGBSpan( 1, 1 ) );
		americanSizer->Add( m_americanTeams[i], wxGBPosition( i + 1, 1 ), wxGBSpan( 1, 1 ) );
		americanSizer->Add( 10, 10, wxGBPosition( i + 1, 2 ), wxGBSpan( 1, 1 ) );
	}
	americanSizer->Add( 15, 5, wxGBPosition( 16, 0 ), wxGBSpan( 1, 1 ) );
	americanSizer->Add( 20, 20, wxGBPosition( 16, 1 ), wxGBSpan( 1, 1 ) );
	americanLeague->Add( americanSizer, 0, wxEXPAND, 0 );
	controls->Add( americanLeague, 0, wxEXPAND, 0 );
	controls->Add( 5, 5, 0, wxEXPAND, 0 );
	nationalSizer->Add( m_national, wxGBPosition( 0, 0 ), wxGBSpan( 1, 2 ) );
	const wxString nationalTeamName[] =
	{
		wxT( "Arizona Diamondbacks" ),
		wxT( "Atlanta Braves" ),
		wxT( "Chicago Cubs" ),
		wxT( "Cincinnati Reds" ),
		wxT( "Colorado Rockies" ),
		wxT( "Florida Marlins" ),
		wxT( "Los Angeles Dodgers" ),
		wxT( "Milwaukee Brewers" ),
		wxT( "New York Mets" ),
		wxT( "Philadelphia Phillies" ),
		wxT( "Pittsburgh Pirates" ),
		wxT( "San Diego Padres" ),
		wxT( "San Francisco Giants" ),
		wxT( "St. Louis Cardinals" ),
		wxT( "Washington Nationals" )
	};
	for( i = 0; i < 15; i++ )
	{
		m_nationalTeams[i] = new wxCheckBox( nationalLeague->GetStaticBox(), wxID_ANY, nationalTeamName[i], wxDefaultPosition, wxDefaultSize, 0L, wxDefaultValidator, "NationalLeague" );
		m_nationalTeams[i]->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CNewLeaguePlayers::OnTeamSelect, this );
		nationalSizer->Add( 15, 5, wxGBPosition( i + 1, 0 ), wxGBSpan( 1, 1 ) );
		nationalSizer->Add( m_nationalTeams[i], wxGBPosition( i + 1, 1 ), wxGBSpan( 1, 1 ) );
		nationalSizer->Add( 10, 10, wxGBPosition( i + 1, 2 ), wxGBSpan( 1, 1 ) );
	}
	if( settings )
	{
		std::vector<int> americanleague = settings->GetAmericanLeague();
		std::vector<int> nationalleague = settings->GetNationalLeague();
		std::vector<int>::iterator it;
		if( americanleague.size() == 1 && americanleague.at( 0 ) == 0 )
		{
			m_american->SetValue( true );
			for( int i = 0; i < 15; i++ )
				m_americanTeams[i-1]->SetValue( true );
		}
		else if( americanleague.size() > 0 )
		{
			for( it = americanleague.begin(); it < americanleague.end(); it++ )
				m_americanTeams[*it-1]->SetValue( true );
		}
		if( nationalleague.size() == 1 && nationalleague.at( 0 ) == 16 )
		{
			m_national->SetValue( true );
			for( int i = 1; i < 16; i++ )
				m_nationalTeams[i-1]->SetValue( true );
		}
		else if( nationalleague.size() > 0 )
		{
			for( it = nationalleague.begin(); it < nationalleague.end(); it++ )
				m_nationalTeams[(*it-1) - 16]->SetValue( true );
		}
	}
	nationalLeague->Add( nationalSizer, 0, wxEXPAND, 0 );
	controls->Add( nationalLeague, 0, wxEXPAND, 0 );
	secondSizer->Add( controls, 0, wxEXPAND, 0 );
	secondSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	mainSizer->Add( secondSizer, 0, wxEXPAND, 0 );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	SetSizer( mainSizer );
	Layout();
	m_american->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CNewLeaguePlayers::OnAmericanSelect, this );
	m_national->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &CNewLeaguePlayers::OnNationalSelect, this );
}

CNewLeaguePlayers::~CNewLeaguePlayers(void)
{
}

void CNewLeaguePlayers::OnAmericanSelect(wxCommandEvent &WXUNUSED(event))
{
	int i;
	if( m_american->IsChecked() )
	{
		for( i = 0; i < 15; i++ )
			m_americanTeams[i]->SetValue( true );
	}
	else
	{
		for( i = 0; i < 15; i++ )
			m_americanTeams[i]->SetValue( false );
	}
}

void CNewLeaguePlayers::OnNationalSelect(wxCommandEvent &WXUNUSED(event))
{
	int i;
	if( m_national->IsChecked() )
	{
		for( i = 0; i < 15; i++ )
			m_nationalTeams[i]->SetValue( true );
	}
	else
	{
		for( i = 0; i < 15; i++ )
			m_nationalTeams[i]->SetValue( false );
	}
}

void CNewLeaguePlayers::OnTeamSelect(wxCommandEvent &event)
{
	wxCheckBox *object = dynamic_cast<wxCheckBox *>( event.GetEventObject() );
	if( object->GetName() == "AmericanLeague" )
//	if( data == (wxObject *) 1 )
	{
		if( !object->IsChecked() )
		{
			m_american->SetValue( false );
		}
		else
		{
			bool found = false;
			for( int i = 0; i < 15 && !found; i++ )
			{
				if( !m_americanTeams[i]->IsChecked() )
					found = true;
			}
			if( !found )
				m_american->SetValue( true );
		}
	}
	else
	{
		if( !object->IsChecked() )
		{
			m_national->SetValue( false );
		}
		else
		{
			bool found = false;
			for( int i = 0; i < 15 && !found; i++ )
			{
				if( !m_nationalTeams[i]->IsChecked() )
					found = true;
			}
			if( !found )
				m_national->SetValue( true );
		}
	}
}

bool CNewLeaguePlayers::Validate()
{
	bool validated = true;
	int count = 0, i;
	for( i = 0; i < 15 && !count; i++ )
	{
		if( m_americanTeams[i]->IsChecked() )
			count++;
	}
	for( i = 0; i < 15 && !count; i++ )
	{
		if( m_nationalTeams[i]->IsChecked() )
			count++;
	}
	if( !count )
	{
		wxMessageBox( "Please select Teams." );
		validated = false;
	}
	return validated;
}

wxCheckBox &CNewLeaguePlayers::GetAmericanLeague()
{
	return *m_american;
}

wxCheckBox &CNewLeaguePlayers::GetNationalLeague()
{
	return *m_national;
}

wxCheckBox *CNewLeaguePlayers::GetAmericanTeams(int pos)
{
	wxASSERT_MSG( pos > 0 || pos < 15, "Only 15 teams in the American League" );
	return m_americanTeams[pos];
}

wxCheckBox *CNewLeaguePlayers::GetNationalTeams(int pos)
{
	wxASSERT_MSG( pos > 0 || pos < 15, "Only 15 teams in the National League" );
	return m_nationalTeams[pos];
}
