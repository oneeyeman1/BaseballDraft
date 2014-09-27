// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include <map>
#include "wx/gbsizer.h"
#include "leaguesettings.h"
#include "newleagueowners.h"

CNewLeagueOwners::CNewLeagueOwners(wxWindow *parent, CLeagueSettings *settings, bool isUpdate) : wxPanel( parent )
{
	m_isUpdate = isUpdate;
	mainSizer = new wxBoxSizer( wxHORIZONTAL );
	vertical = new wxBoxSizer( wxVERTICAL );
	panelHolder = new wxBoxSizer( wxHORIZONTAL );
	staticbox = new wxStaticBoxSizer( wxVERTICAL, this, "Please enter owner names." );
	container = new wxGridBagSizer( 5, 5 );
	m_holder = new wxPanel( this );
	if( settings )
	{
		std::vector<std::string> owners = settings->GetOwners();
		for( std::vector<std::string>::iterator it = owners.begin(); it < owners.end(); it++ )
			m_ownersStr.push_back( *it );
	}
	m_owners = NULL;
	m_numbers = NULL;
}

CNewLeagueOwners::~CNewLeagueOwners(void)
{
	for( int i = 0; i < m_ownersNumber; i++ )
	{
		delete m_owners[i];
		m_owners[i] = NULL;
		delete m_numbers[i];
		m_numbers[i] = NULL;
	}
	delete m_owners;
	delete m_numbers;
}

void CNewLeagueOwners::CreateLayout(unsigned int owners)
{
	if( m_owners )
	{
		for( int i = 0; i < m_ownersNumber; i++ )
		{
			if( m_owners[i] )
			{
				delete m_owners[i];
				m_owners[i] = NULL;
			}
			if( m_numbers[i] )
			{
				delete m_numbers[i];
				m_numbers[i] = NULL;
			}
		}
		delete[] m_owners;
		delete[] m_numbers;
	}
	m_owners = new wxTextCtrl*[owners];
	m_numbers = new wxStaticText*[owners];
	if( m_ownersStr.size() > 0 && m_ownersStr.size() != owners )
	{
		for( unsigned int i = m_ownersStr.size(); i < owners; i++ )
			m_ownersStr.push_back( "" );
	}
	int row = 0, col = 0;
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	for( unsigned int i = 0; i < owners; i++ )
	{
		wxString value = wxEmptyString;
		if( m_ownersStr.size() > 0 )
			value = m_ownersStr.at( i );
		m_numbers[i] = new wxStaticText( /*staticbox->GetStaticBox()*/m_holder, wxID_ANY, wxString::Format( "%d", i + 1 ) );
		m_numbers[i]->SetFont( m_numbers[i]->GetFont().MakeBold() );
		m_owners[i] = new wxTextCtrl( /*staticbox->GetStaticBox()*/m_holder, wxID_ANY, value );
		container->Add( m_numbers[i], wxGBPosition( row, col++ ), wxGBSpan( 1, 1 ) );
		container->Add( m_owners[i], wxGBPosition( row++, col ), wxGBSpan( 1, 1 ) );
		if( col == 3 )
		{
			container->Add( 15, 15, wxGBPosition( --row, 4 ), wxGBSpan( 1, 1 ) );
			row++;
		}
		if( i <= owners / 2 - 1 )
			col = 0;
		else
			col = 2;
		if( i == owners / 2 - 1 )
		{
			container->Add( 50, 50, wxGBPosition( row++, col ), wxGBSpan( 1, 2 ) );
			row = 0;
			col = 2;
		}
	}
	m_holder->SetSizer( container );
	panelHolder->Add( m_holder, 0, wxEXPAND, 0 );
	staticbox->Add( panelHolder, 0, wxEXPAND, 0 );
	vertical->Add( staticbox, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	mainSizer->Add( vertical, 0, wxEXPAND, 0 );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	SetSizer( mainSizer );
	Layout();
}

void CNewLeagueOwners::SetOwnersNumber(int ownersNumber)
{
	std::vector<wxString> owners;
	unsigned int i;
	for( i = 0; i < container->GetItemCount(); i++ )
		container->Remove( i );
	if( m_ownersNumber != ownersNumber )
	{
		if( m_ownersNumber )
		{
			for( int j = 0; j < m_ownersNumber; j++ )
			{
				if( m_ownersNumber <= ownersNumber )
					owners.push_back( m_owners[j]->GetValue() );
				delete m_owners[j];
				m_owners[j] = NULL;
				delete m_numbers[j];
				m_numbers[j] = NULL;
			}
			delete m_owners;
			m_owners = NULL;
			delete m_numbers;
			m_numbers = NULL;
		}
		m_ownersNumber = ownersNumber;
		CreateLayout( m_ownersNumber );
		if( owners.size() > 0 )
		{
			for( i = 0; i < owners.size(); i++ )
				m_owners[i]->SetValue( owners.at( i ) );
		}
		else if( !m_isUpdate )
		{
			for( i = 0; i < m_ownersNumber; i++ )
				m_owners[i]->SetValue( wxEmptyString );
		}
		if( m_isUpdate )
			m_isUpdate = false;
	}
	m_owners[0]->SetFocus();
}

int CNewLeagueOwners::GetOwnersNumber()
{
	return m_ownersNumber;
}

wxTextCtrl *CNewLeagueOwners::GetOwner(int pos)
{
//	return (*m_owners + pos);
	return m_owners[pos];
}

bool CNewLeagueOwners::Validate()
{
	bool validated = true;
	int validatedNumber = 0;
	for( int i = 0; i < m_ownersNumber; i++ )
	{
		if( !m_owners[i]->GetValue().IsEmpty() )
			validatedNumber++;
	}
	if( validatedNumber != m_ownersNumber )
	{
		wxMessageBox( "Please enter all Owner fields" );
		validated = false;
	}
	return validated;
}

void CNewLeagueOwners::SetOwners(const std::vector<std::string> &owners)
{
	m_ownersStr = owners;
}

std::vector<std::string> &CNewLeagueOwners::GetOwnersNames()
{
	return m_ownersStr;
}
