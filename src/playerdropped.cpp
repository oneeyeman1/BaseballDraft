// -*- C++ -*- generated by wxGlade 0.6.5 (standalone edition) on Wed Apr 03 12:02:22 2013
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
#include "wx/spinctrl.h"
#include "leaguesettings.h"
#include "playerdropped.h"

CDroppedValuePlayer::CDroppedValuePlayer(wxWindow* parent, const std::vector<CPlayer> *player, int condition) : wxDialog(parent, wxID_ANY, wxEmptyString)
{
	m_players = *player;
	double sum = 0;
	int count = 0;
	m_panel = new wxPanel( this );
	m_condition = condition;
    const wxString *m_playersList_choices = NULL;
	wxString labelText;
	if( m_condition == ADDPLAYER || m_condition == CHANGEPLAYERFROM0 )
		labelText = "Select a player to be valued at 0";
	if( m_condition == CHANGEPLAYERTO0 )
		labelText = "Select player whose value will increase";
	m_label1 = new wxStaticText( m_panel, wxID_ANY, labelText );
    m_playersList = new wxListBox( m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, m_playersList_choices, wxLB_SINGLE);
	if( m_condition == CHANGEPLAYERTO0 )
	{
		for( std::vector<CPlayer>::const_iterator it = player->begin(); it < player->end(); it++ )
			if( !(*it).IsPlayerDrafted() && (*it).GetValue() > 0 )
			{
				sum += (*it).GetValue();
				count++;
			}
		m_label2 = new wxStaticText( m_panel, wxID_ANY, "Enter new value for a player:" );
		m_newValue = new wxSpinCtrl( m_panel, wxID_ANY, "1", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxSP_WRAP, 1, sum - ( count - 1 ), 1 );
	}
	m_ok = new wxButton( m_panel, wxID_OK, "OK" );
    set_properties();
    do_layout();
	m_ok->Bind( wxEVT_UPDATE_UI, &CDroppedValuePlayer::OnOKUIUpdate, this );
}

void CDroppedValuePlayer::set_properties()
{
    SetTitle( "Select player to change" );
	if( m_condition == ADDPLAYER || m_condition == CHANGEPLAYERFROM0 )
	{
		for( std::vector<CPlayer>::iterator it = m_players.begin(); it < m_players.end(); it++ )
		{
			if( (*it).GetValue() == 1 && !(*it).IsPlayerDrafted() )
				m_playersList->Append( (*it).GetName() );
		}
	}
	if( m_condition == CHANGEPLAYERTO0 )
	{
		for( std::vector<CPlayer>::iterator it = m_players.begin(); it < m_players.end(); it++ )
			if( (*it).GetValue() == 0 )
				m_playersList->Append( (*it).GetName() );
	}
}

void CDroppedValuePlayer::do_layout()
{
	wxBoxSizer *main = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer *sizer_1 = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer *sizer_2 = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *sizer_3 = new wxBoxSizer( wxVERTICAL );
    wxStdDialogButtonSizer *sizerBtns = new wxStdDialogButtonSizer;
	sizerBtns->AddButton( m_ok );
	sizerBtns->AddButton( new wxButton( m_panel, wxID_CANCEL ) );
	sizerBtns->Realize();
    sizer_1->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_2->Add( 5, 5, 0, wxEXPAND, 0 );
	sizer_2->Add( m_label1, 0, wxEXPAND, 0 );
    sizer_3->Add( m_playersList, 0, 0, 0 );
    sizer_3->Add( 5, 20, 0, wxEXPAND, 0 );
	if( m_condition == CHANGEPLAYERTO0 )
	{
		wxBoxSizer *sizer_4 = new wxBoxSizer( wxHORIZONTAL );
		sizer_4->Add( m_label2, 0, wxEXPAND, 0 );
		sizer_4->Add( m_newValue, 0, wxEXPAND, 0 );
		sizer_3->Add( sizer_4, 0, wxEXPAND, 0 );
		sizer_3->Add( 5, 5, 0, wxEXPAND, 0 );
	}
    sizer_3->Add( sizerBtns, 1, wxEXPAND, 0 );
    sizer_2->Add( sizer_3, 0, wxEXPAND, 0 );
    sizer_2->Add( 5, 5, 0, wxEXPAND, 0 );
    sizer_1->Add( sizer_2, 0, wxEXPAND, 0 );
    sizer_1->Add( 5, 5, 0, wxEXPAND, 0 );
	m_panel->SetSizer( sizer_1 );
	main->Add( m_panel, 0, wxEXPAND, 0 );
    SetSizerAndFit( main );
}

void CDroppedValuePlayer::OnOKUIUpdate(wxUpdateUIEvent &event)
{
	event.Enable( m_playersList->GetSelection() != wxNOT_FOUND );
}

wxString CDroppedValuePlayer::GetDroppedPlayerName()
{
	return m_playersList->GetStringSelection();
}

int CDroppedValuePlayer::GetDroppedPlayerValue()
{
	return m_newValue->GetValue();
}
