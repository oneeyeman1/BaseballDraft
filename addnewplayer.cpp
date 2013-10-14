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
#include "wx/combo.h"
#include "wx/collpane.h"
#include "wx/valnum.h"
#include "wx/tokenzr.h"
#include "leaguesettings.h"
#include "enterstats.h"
#include "newplayerpositions.h"
#include "addnewplayer.h"

CAddNewPlayer::CAddNewPlayer(wxWindow *parent, const wxString &title, const std::map<wxString,wxString> &names, const CLeagueSettings &settings, const CPlayer *player) : wxDialog( parent, wxID_ANY, title )
{
	m_player = const_cast<CPlayer *>( player );
	m_teams = names;
	m_settings = &( const_cast<CLeagueSettings &>( settings ) );
    m_panel = new wxPanel( this, wxID_ANY );
    m_label1 = new wxStaticText( m_panel, wxID_ANY, "First Name:" );
    m_first = new wxTextCtrl( m_panel, wxID_ANY, wxEmptyString );
    m_label2 = new wxStaticText( m_panel, wxID_ANY, "Last Name:" );
    m_last = new wxTextCtrl( m_panel, wxID_ANY, wxEmptyString );
    m_label3 = new wxStaticText( m_panel, wxID_ANY, "Age:" );
    m_age = new wxTextCtrl( m_panel, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, wxIntegerValidator<int>() );
    m_label4 = new wxStaticText( m_panel, wxID_ANY, "Team:" );
    m_team = new wxComboBox( m_panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY|wxCB_SORT );
	for( std::map<wxString,wxString>::const_iterator it = names.begin(); it != names.end(); it++ )
	{
		int pos = m_team->Append( (*it).first );
		m_team->SetClientData( pos, (void *) &(*it).second );
	}
	m_team->SetSelection( 0 );
    m_label5 = new wxStaticText( m_panel, wxID_ANY, "Position:" );
    m_position = new CNewPlayerPositions();
	m_position->SetTextCtrlStyle( wxTE_RIGHT );
	m_position->Create( m_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCB_READONLY );
	m_position->UseAltPopupWindow();
	PositionsPopup *iface = new PositionsPopup();
	m_position->SetPopupControl( iface );
	if( player && !player->IsPlayerDrafted() )
	{
		m_label6 = new wxStaticText( m_panel, wxID_ANY, "BegValue:" );
		m_value = new wxTextCtrl( m_panel, wxID_ANY, wxString::Format( "%.2f", player->GetValue() ), wxDefaultPosition, wxDefaultSize, 0, wxFloatingPointValidator<double>() );
	}
	else if( !player )
	{
		m_label6 = new wxStaticText( m_panel, wxID_ANY, "BegValue" );
		m_value = new wxTextCtrl( m_panel, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, wxIntegerValidator<unsigned int>() );
	}
	else if( player )
	{
		m_label6 = new wxStaticText( m_panel, wxID_ANY, "AmtPaid:" );
		m_value = new wxTextCtrl( m_panel, wxID_ANY, wxString::Format( "%d", player->GetAmountPaid() ), wxDefaultPosition, wxDefaultSize, 0, wxIntegerValidator<int>() );
	}
    m_stats = new wxButton( m_panel, wxID_ANY, "Enter Stats..." );
    m_add = new wxButton( m_panel, wxID_ANY, player ? "Edit Player" : "Add Player" );
    m_cancel = new wxButton( m_panel, wxID_CANCEL, "Cancel" );
	SetProperties( player, title );
	wxBoxSizer* main_sizer = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer* horizontal = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer* vertical = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* sizer_3 = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer* sizer_4 = new wxBoxSizer( wxHORIZONTAL );
    wxFlexGridSizer* controls = new wxFlexGridSizer( 6, 2, 10, 10 );
    horizontal->Add( 5, 5, 0, wxEXPAND, 0 );
    vertical->Add( 5, 5, 0, wxEXPAND, 0 );
    controls->Add( m_label1, 0, wxALIGN_CENTER_VERTICAL, 0 );
    controls->Add( m_first, 0, wxALIGN_CENTER_VERTICAL, 0 );
    controls->Add( m_label2, 0, wxALIGN_CENTER_VERTICAL, 0 );
    controls->Add( m_last, 0, wxALIGN_CENTER_VERTICAL, 0 );
    controls->Add( m_label3, 0, wxALIGN_CENTER_VERTICAL, 0 );
    controls->Add( m_age, 0, wxALIGN_CENTER_VERTICAL, 0 );
    controls->Add( m_label4, 0, wxALIGN_CENTER_VERTICAL, 0 );
    controls->Add( m_team, 0, wxALIGN_CENTER_VERTICAL, 0 );
    controls->Add( m_label5, 0, wxALIGN_CENTER_VERTICAL, 0 );
    controls->Add( m_position, 0, wxALIGN_CENTER_VERTICAL, 0 );
    controls->Add( m_label6, 0, wxALIGN_CENTER_VERTICAL, 0 );
    controls->Add( m_value, 0, wxALIGN_CENTER_VERTICAL, 0 );
    vertical->Add( controls, 0, wxEXPAND, 0 );
    vertical->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_3->Add( m_stats, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );
    sizer_3->Add( 10, 10, 0, wxEXPAND, 0 );
    sizer_4->Add( m_add, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );
    sizer_4->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_4->Add( m_cancel, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );
    sizer_3->Add( sizer_4, 0, wxEXPAND, 0 );
    vertical->Add( sizer_3, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
    horizontal->Add( vertical, 0, wxEXPAND, 0 );
    horizontal->Add( 5, 5, 0, wxEXPAND, 0 );
    m_panel->SetSizer( horizontal );
    main_sizer->Add( m_panel, 0, wxEXPAND, 0 );
    SetSizerAndFit( main_sizer );
    Centre();
	m_add->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CAddNewPlayer::OnAddNewPlayer, this );
	m_stats->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CAddNewPlayer::OnEnterStats, this );
	m_first->Bind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
	m_last->Bind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
	m_age->Bind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
	m_team->Bind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
	m_position->Bind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
	m_value->Bind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
}

CAddNewPlayer::~CAddNewPlayer(void)
{
	m_add->Unbind( wxEVT_COMMAND_BUTTON_CLICKED, &CAddNewPlayer::OnAddNewPlayer, this );
	m_stats->Unbind( wxEVT_COMMAND_BUTTON_CLICKED, &CAddNewPlayer::OnEnterStats, this );
	m_first->Unbind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
	m_last->Unbind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
	m_age->Unbind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
	m_team->Unbind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
	m_position->Unbind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
	m_value->Unbind( wxEVT_COMMAND_TEXT_UPDATED, &CAddNewPlayer::PlayerChanging, this );
}

void CAddNewPlayer::SetProperties(const CPlayer *player, const wxString &title)
{
	SetTitle( title );
	if( player )
	{
		wxString position;
		wxString name = const_cast<CPlayer *>( player )->GetName();
		int pos = name.find( ' ' );
		wxString fname = name.Left( pos );
		wxString lname = name.Right( name.length() - ( pos + 1 ) );
		m_first->SetValue( fname );
		m_first->Enable( false );
		m_last->SetValue( lname );
		m_last->Enable( false );
		m_age->SetValue( wxString::Format( "%d", const_cast<CPlayer *>( player )->GetAge() ) );
		m_team->SetValue( const_cast<CPlayer *>( player )->GetAbbeviatedTeamName() );
		PositionsPopup *popup = dynamic_cast<PositionsPopup *>( m_position->GetPopupControl() );
		if( player->IsPlayerDrafted() )
		{
			position = const_cast<CPlayer *>( player )->GetDraftedPosition();
			m_position->Enable( false );
			popup->Check( dynamic_cast<wxCheckListBox *>( popup )->FindString( position ) );
		}
		else
		{
			for( std::vector<wxString>::iterator it = const_cast<CPlayer *>( player )->GetPositions().begin(); it < const_cast<CPlayer *>( player )->GetPositions().end(); it++ )
			{
				popup->Check( dynamic_cast<wxCheckListBox *>( popup )->FindString( (*it) ) );
				position += (*it);
				if( it != const_cast<CPlayer *>( player )->GetPositions().end() - 1 )
					position += ", ";
			}
		}
		m_position->SetValue( position );
	}
}

void CAddNewPlayer::OnAddNewPlayer(wxCommandEvent &WXUNUSED(event))
{
	if( Validate() )
	{
		if( m_newStats.size() == 0 && !m_player )
		{
			if( m_position->GetValue().find( "P" ) != wxNOT_FOUND )
			{
				m_statistics["Wins"] = 0;
				m_statistics["Saves"] = 0;
				m_statistics["ERA"] = 0;
				m_statistics["WHIP"] = 0;
				m_statistics["K"] = 0;
				m_statistics["IP"] = 0;
				m_statistics["L"] = 0;
				m_statistics["H"] = 0;
				m_statistics["BB"] = 0;
				m_statistics["ER"] = 0;
				m_statistics["CG"] = 0;
				m_statistics["W-L"] = 0;
				m_statistics["QS"] = 0;
				m_statistics["Shutouts"] = 0;
				m_statistics["BS"] = 0;
				m_statistics["S-BS"] = 0;
				m_statistics["S+Holds"] = 0;
				m_statistics["S+Holds-BS"] = 0;
				m_statistics["K/9"] = 0;
				m_statistics["H/9"] = 0;
				m_statistics["BB/9"] = 0;
				m_statistics["K/BB"] = 0;
				m_statistics["K-BB"] = 0;
				m_statistics["W%"] = 0;
				m_statistics["Holds"] = 0;
				m_statistics["G"] = 0;
				m_statistics["R"] = 0;
				m_statistics["hr"] = 0;
			}
			else
			{
				m_statistics["AVG"] = 0;
				m_statistics["HR"] = 0;
				m_statistics["Runs"] = 0;
				m_statistics["RBI"] = 0;
				m_statistics["SB"] = 0;
				m_statistics["OBP"] = 0;
				m_statistics["SLG"] = 0;
				m_statistics["OPS"] = 0;
				m_statistics["bb"] = 0;
				m_statistics["h"] = 0;
				m_statistics["Ks"] = 0;
				m_statistics["1B"] = 0;
				m_statistics["2B"] = 0;
				m_statistics["3B"] = 0;
				m_statistics["TB"] = 0;
				m_statistics["2B+3B+HR"] = 0;
				m_statistics["2B+3B"] = 0;
				m_statistics["SB-CS"] = 0;
				m_statistics["CS"] = 0;
				m_statistics["R+RBI-HR"] = 0;
				m_statistics["H+BB"] = 0;
				m_statistics["TB+BB"] = 0;
				m_statistics["AB"] = 0;
				m_statistics["HBP"] = 0;
				m_statistics["GDP"] = 0;
				m_statistics["SF"] = 0;
			}
		}
		else if( m_newStats.size() == 0 && m_player )
			m_statistics = m_player->GetScoring();
		else
		{
			for( std::map<wxString,NewPlayerStats>::iterator it = m_newStats.begin(); it != m_newStats.end(); it++ )
				m_statistics[(*it).first] = (*it).second.m_value;
		}
		double value;
		m_value->GetValue().ToDouble( &value );
		if( !m_player )
		{
			wxStringTokenizer tokens( m_position->GetValue(), ", " );
			while( tokens.HasMoreTokens() )
				m_pos.push_back( tokens.GetNextToken() );
			m_player = new CPlayer( 0, m_first->GetValue() + " " + m_last->GetValue(), m_pos, wxAtoi( m_age->GetValue() ), wxAtoi( m_value->GetValue() ), m_teams[m_team->GetValue()], m_team->GetValue(), m_statistics, m_position->GetValue().find( "P" ) == wxNOT_FOUND ? true : false, value, wxEmptyString, wxAtoi( m_value->GetValue() ) );
			m_player->SetNewPlayer( true );
		}
		else
		{
		}
		EndModal( wxID_OK );
	}
}

bool CAddNewPlayer::Validate()
{
	bool validated = true;
	wxString positions = m_position->GetValue();
	if( m_first->GetValue().IsEmpty() || m_last->GetValue().IsEmpty() )
	{
		wxMessageBox( "First name or last name is required." );
		m_first->SetFocus();
		validated = false;
	}
	if( validated && m_age->GetValue().IsEmpty() )
	{
		wxMessageBox( "Age is required" );
		m_age->SetFocus();
		validated = false;
	}
	if( validated && m_value->GetValue().IsEmpty() )
	{
		wxMessageBox( "Begin Value is required for player." );
		m_value->SetFocus();
		validated = false;
	}
	if( validated && !CheckPlayerPosition() )
		validated = false;
	return validated;
}

void CAddNewPlayer::OnEnterStats(wxCommandEvent &WXUNUSED(event))
{
	if( CheckPlayerPosition() )
	{
		CEnterStats dlg( this, *m_settings, m_position->GetValue(), m_player );
		dlg.Center();
		if( dlg.ShowModal() == wxID_OK )
		{
			m_newStats = dlg.GetStatistics();
			if( dlg.IsDirty() )
				m_dirty = true;
		}
	}
}

bool CAddNewPlayer::CheckPlayerPosition()
{
	bool good = true, isHitter = false, isPitcher = false;
	wxString position = m_position->GetValue();
	m_positions.clear();
	if( position.IsEmpty() )
	{
		wxMessageBox( "Please select player position to enter the statistics" );
		m_position->SetFocus();
		good = false;
	}
	wxStringTokenizer tokens( position, ", " );
	while( tokens.HasMoreTokens() && good )
	{
		wxString token = tokens.GetNextToken();
		if( token.IsEmpty() )
			continue;
		if( token == "P" || token == "RP" || token == "SP" )
			isPitcher = true;
		else
			isHitter = true;
		if( isPitcher && isHitter )
		{
			wxMessageBox( "Player cannot be hitter and pitcher" );
			m_position->SetFocus();
			m_positions.clear();
			good = false;
		}
		else
			m_pos.push_back( token );
	}
	return good;
}

CPlayer &CAddNewPlayer::GetNewPlayer()
{
	return *m_player;
}

double CAddNewPlayer::GetChangedCurrentValue()
{
	double value;
	wxString temp = m_value->GetValue();
	temp.ToDouble( &value );
	return value;
}

long CAddNewPlayer::GetChangedAmountPaid()
{
	long value;
	wxString temp = m_value->GetValue();
	temp.ToLong( &value );
	return value;
}

bool CAddNewPlayer::PlayerAddedModified()
{
	return m_dirty;
}

void CAddNewPlayer::PlayerChanging(wxCommandEvent &WXUNUSED(event))
{
	if( !m_dirty )
		m_dirty = true;
}

int CAddNewPlayer::GetChangedAge()
{
	return wxAtoi( m_age->GetValue() );
}

wxString &CAddNewPlayer::GetChangedTeam()
{
	return *(wxString *) m_team->GetClientData( m_team->GetSelection() );
}

wxString CAddNewPlayer::GetChangedAbbreviatedTeam()
{
	return m_team->GetValue();
}

std::map<wxString, double> &CAddNewPlayer::GetScoring()
{
	return m_statistics;
}

wxString &CAddNewPlayer::GetChangedPositions()
{
	wxStringTokenizer tokens( m_position->GetValue(), ", " );
	while( tokens.HasMoreTokens() )
	{
		m_positions += tokens.GetNextToken();
	}
	return m_positions;
}
