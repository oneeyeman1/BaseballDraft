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
#include "newleagueowners.h"

CNewLeagueOwners::CNewLeagueOwners(wxWindow *parent, CLeagueSettings *settings, bool isUpdate) : wxPanel( parent )
{
	m_oldNumber = 30;
	m_strict = true;
	if( !isUpdate )
		m_oldNumber = 10;
	else
		m_oldNumber = settings->GetOwnersNumber();
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
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	int row = 0, col = 0;
	for( unsigned int i = 0; i < m_oldNumber; i++ )
	{
		m_owners[i] = new wxTextCtrl( m_holder, wxID_ANY );
		m_numbers[i] = new wxStaticText( m_holder, wxID_ANY, wxString::Format( "%d", i + 1 ) );
		m_numbers[i]->SetFont( m_numbers[i]->GetFont().MakeBold() );
		container->Add( m_numbers[i], wxGBPosition( row, col++ ), wxGBSpan( 1, 1 ) );
		container->Add( m_owners[i], wxGBPosition( row++, col ), wxGBSpan( 1, 1 ) );
		if( col == 3 )
		{
			container->Add( 15, 15, wxGBPosition( --row, 4 ), wxGBSpan( 1, 1 ) );
			row++;
		}
		if( i <= 14 )
			col = 0;
		else
			col = 2;
		if( i == 14 )
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
	SetSizerAndFit( mainSizer );
	if( m_isUpdate )
	{
		for( unsigned int i = 0; i < m_oldNumber; i++ )
		{
			if( i < settings->GetOwnersNumber() )
			{
				m_owners[i]->SetValue( settings->GetOwners().at( i ) );
			}
			else
			{
				m_owners[i]->Hide();
				m_numbers[i]->Hide();
			}
		}
	}
}

CNewLeagueOwners::~CNewLeagueOwners(void)
{
/*	for( int i = 0; i < m_ownersNumber; i++ )
	{
		delete m_owners[i];
		m_owners[i] = NULL;
		delete m_numbers[i];
		m_numbers[i] = NULL;
	}
	delete m_owners;
	delete m_numbers;*/
}

void CNewLeagueOwners::CreateLayout(unsigned int owners)
{
	unsigned int i;
	int row = m_oldNumber, col = 0;
	if( m_oldNumber < owners )
	{
		for( i = 0; i < m_oldNumber; i++ )
		{
			m_owners[i]->SetValue( wxEmptyString );
		}
		for( i = m_oldNumber; i < owners; i++ )
		{
			m_owners[i] = new wxTextCtrl( m_holder, wxID_ANY );
			m_numbers[i] = new wxStaticText( m_holder, wxID_ANY, wxString::Format( "%d", i + 1 ) );
			m_numbers[i]->SetFont( m_numbers[i]->GetFont().MakeBold() );
			container->Add( m_numbers[i], wxGBPosition( row, col++ ), wxGBSpan( 1, 1 ) );
			container->Add( m_owners[i], wxGBPosition( row++, col ), wxGBSpan( 1, 1 ) );
			if( col == 3 )
			{
				container->Add( 15, 15, wxGBPosition( --row, 4 ), wxGBSpan( 1, 1 ) );
				row++;
			}
			if( i <= 14 )
				col = 0;
			else
				col = 2;
			if( i == 14 )
			{
				container->Add( 50, 50, wxGBPosition( row++, col ), wxGBSpan( 1, 2 ) );
				row = 0;
				col = 2;
			}
		}
	}
	else
	{
		for( i = m_oldNumber; i > owners; i-- )
		{
			delete m_owners[i - 1];
			delete m_numbers[i - 1];
			m_owners[i - 1] = NULL;
			m_numbers[i - 1] = NULL;
			if( m_ownersStr.size() > 0 )
				m_ownersStr.pop_back();
		}
	}
	Layout();
	if( m_isUpdate )
		m_isUpdate = false;
}

void CNewLeagueOwners::SetOwnersNumber(unsigned int ownersNumber)
{
	std::vector<wxString> owners;
	unsigned int i;
	m_ownersNumber = ownersNumber;
	Freeze();
	CreateLayout( m_ownersNumber );
	Thaw();
	if( owners.size() > 0 )
	{
		for( i = 0; i < owners.size(); i++ )
			m_owners[i]->SetValue( owners.at( i ) );
	}
	else if( m_ownersStr.size() > 0 )
	{
		for( i = 0; i < m_ownersStr.size(); i++ )
			m_owners[i]->SetValue( m_ownersStr.at( i ) );
	}
	else if( !m_isUpdate )
	{
		for( int i = 0; i < m_ownersNumber; i++ )
			m_owners[i]->SetValue( wxEmptyString );
	}
	if( m_isUpdate )
		m_isUpdate = false;
	m_owners[0]->SetFocus();
	m_oldNumber = ownersNumber;
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
	if( validatedNumber == 0 && !m_strict )
	{
		wxMessageBox( "Please enter all Owner fields" );
		validated = false;
	}
	if( validatedNumber != m_ownersNumber && validatedNumber > 0 )
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

void CNewLeagueOwners::SetStrictValidation(bool strict)
{
	m_strict = strict;
}
