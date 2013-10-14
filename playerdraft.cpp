//For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <map>
#include <algorithm>
#include <vector>
#include "sqlite3.h"
#include "wx/tipdlg.h"
#include "wx/textcompleter.h"
#include "wx/grid.h"
#include "wx/tglbtn.h"
#include "wx/spinctrl.h"
#include "textctrl.h"
#include "leaguesettings.h"
#include "playerspanelgridtable.h"
#include "db.h"
#include "namecompleter.h"
#include "playerspanel.h"
#include "teamsroster.h"
#include "teamprojections.h"
#include "draftresultpanel.h"
#include "draftresultsalary.h"
#include "draftresult.h"
#include "gridtotalcell.h"
#include "playerdraft.h"
#include "frame.h"

CPlayerDraft::CPlayerDraft(wxWindow *parent, const CLeagueData &settings, int maxBid) : wxPanel( parent )
{
	int i, categoriesSize;
	m_isResizing = false;
	m_data = &( const_cast<CLeagueData &>( settings ) );
	m_completer = new CNameCompleter( const_cast<std::vector<CPlayer> *>( settings.m_players ), true );
	m_label1 = new wxStaticText( this, wxID_ANY, "Player" );
	m_player = new CTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB );
	m_label2 = new wxStaticText( this, wxID_ANY, "Owner" );
	m_owner = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY );
	m_label3 = new wxStaticText( this, wxID_ANY, "AmtPaid" );
	m_amount = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, maxBid );
	m_label4 = new wxStaticText( this, wxID_ANY, "Position" );
	m_position = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY );
	m_draft = new wxButton( this, wxID_ANY, "Draft" );
	std::vector<std::string>::iterator it;
	for( it = settings.m_settings->GetOwners().begin(); it < settings.m_settings->GetOwners().end(); it++ )
		m_owner->Append( *it );
	m_playerInfo = new wxGrid( this, wxID_ANY );
	categoriesSize = settings.m_settings->GetHitScoring().size();
	m_playerInfo->CreateGrid( 1, 8 + categoriesSize );
	m_playerInfo->BeginBatch();
	m_playerInfo->HideRowLabels();
    m_playerInfo->EnableEditing( false );
    m_playerInfo->EnableDragRowSize( false );
    m_playerInfo->EnableDragGridSize( false );
    m_playerInfo->SetColLabelValue( 0, "Player" );
    m_playerInfo->SetColLabelValue( 1, "Team" );
    m_playerInfo->SetColLabelValue( 2, "Position" );
    m_playerInfo->SetColLabelValue( 3, "Age" );
    m_playerInfo->SetColLabelValue( 4, "BegValue" );
    m_playerInfo->SetColLabelValue( 5, "CurrValue" );
	int size = categoriesSize + 6;
	for( i = 6; i < size; i++ )
	{
		m_playerInfo->SetColLabelValue( i, wxEmptyString );
		m_playerInfo->SetCellValue( 0, i, "0" );
	}
    m_playerInfo->SetColLabelValue( i, "Notes" );
	int col = m_playerInfo->GetNumberCols() - 1;
	m_playerInfo->SetColLabelValue( col, wxEmptyString );
	m_playerInfo->SetCellRenderer( 0, col, new CGridTotalCell( "Edit" ) );
 	m_playerInfo->AutoSize();
	int column = m_playerInfo->GetNumberCols() - 2;
	m_playerInfo->SetColSize( column, m_playerInfo->GetColSize( column ) * 4 );
	m_playerInfo->DisableDragRowSize();
	m_playerInfo->EndBatch();
	m_player->AutoComplete( m_completer );
	m_draft->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CFrame::OnDraftPerformed, dynamic_cast<CFrame *>( GetParent()->GetParent() ) );
	m_player->Bind( wxEVT_COMMAND_TEXT_UPDATED, &CPlayerDraft::OnPlayerNameChange, this );
	m_player->Bind( wxEVT_KILL_FOCUS, &CPlayerDraft::OnNameKillFocus, this );
	m_playerInfo->Bind( wxEVT_GRID_CELL_RIGHT_CLICK, &CPlayerDraft::OnContextMenu, this );
	m_playerInfo->Bind( wxEVT_GRID_COL_SIZE, &CPlayerDraft::OnColumnResized, this );
	main = new wxBoxSizer( wxHORIZONTAL );
	vertical = new wxBoxSizer( wxVERTICAL );
	m_draftedPlayer = new wxBoxSizer( wxHORIZONTAL );
	grid = new wxBoxSizer( wxHORIZONTAL );
	main->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
    m_draftedPlayer->Add(m_label1, 0, wxEXPAND, 0);
    m_draftedPlayer->Add(5, 5, 0, wxEXPAND, 0);
    m_draftedPlayer->Add(m_player, 0, wxEXPAND, 0);
	m_draftedPlayer->AddStretchSpacer();
    m_draftedPlayer->Add(m_label2, 0, wxEXPAND, 0);
    m_draftedPlayer->Add(5, 5, 0, wxEXPAND, 0);
    m_draftedPlayer->Add(m_owner, 0, wxEXPAND, 0);
	m_draftedPlayer->AddStretchSpacer();
    m_draftedPlayer->Add(m_label3, 0, wxEXPAND, 0);
    m_draftedPlayer->Add(5, 5, 0, wxEXPAND, 0);
    m_draftedPlayer->Add(m_amount, 0, wxEXPAND, 0);
	m_draftedPlayer->AddStretchSpacer();
    m_draftedPlayer->Add(m_label4, 0, wxEXPAND, 0);
    m_draftedPlayer->Add(5, 5, 0, wxEXPAND, 0);
    m_draftedPlayer->Add(m_position, 0, wxEXPAND, 0);
	m_draftedPlayer->AddStretchSpacer();
    m_draftedPlayer->Add(m_draft, 0, wxEXPAND, 0);
	vertical->Add( m_draftedPlayer, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	grid->Add( m_playerInfo, 1, wxEXPAND, 0 );
	vertical->Add( grid, 1, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	main->Add( vertical, 1, wxEXPAND, 0 );
	main->Add( 5, 5, 0, wxEXPAND, 0 );
	SetSizerAndFit( main );
}

CPlayerDraft::~CPlayerDraft(void)
{
	m_player->Unbind( wxEVT_COMMAND_TEXT_UPDATED, &CPlayerDraft::OnPlayerNameChange, this );
}

wxTextCtrl &CPlayerDraft::GetPlayerNameCtrl()
{
	return *m_player;
}

void CPlayerDraft::GetDraftedPlayer(wxString &name, wxString &position, wxString &owner)
{
	name = m_player->GetValue();
	position = m_position->GetValue();
	owner = m_owner->GetValue();
}

wxComboBox &CPlayerDraft::GetOwnerCtrl()
{
	return *m_owner;
}

int CPlayerDraft::GetAmountPaid()
{
	return m_amount->GetValue();
}

wxComboBox &CPlayerDraft::GetPositionCtrl()
{
	return *m_position;
}

void CPlayerDraft::OnPlayerNameChange(wxCommandEvent &WXUNUSED(event))
{
	wxString name = m_player->GetValue();
	m_position->Clear();
	if( !name.IsEmpty() )
	{
		if( name.length() == 1 )
		{
//			m_player->SetSelection( 0, -1 );
//			m_player->SetValue( name.MakeUpper() );
//			m_player->SetInsertionPointEnd();
		}
		m_playerInfo->Bind( wxEVT_GRID_COL_AUTO_SIZE, &CPlayerDraft::OnAutoResizing, this );
		m_playerInfo->EnableEditing( true );
		name.at( 0 ) = wxToupper( *(name.begin()) );
		std::vector<CPlayer>::iterator it;
		for( it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
		{
			if( it->GetName() == name )
			{
				std::vector<wxString> score;
				if( it->IsHitter() )
					score = m_data->m_settings->GetHitScoring();
				else
					score = m_data->m_settings->GetPitchScoring();
				m_amount->SetValue( it->GetValue() );
				std::vector<wxString> positions = it->GetPositions();
				if( !it->IsHitter() && m_data->m_settings->GetLeagueType() )
					m_position->Append( "P" );
				else
				{
					bool isCIEligible = IsCIEligible( positions );
					bool isMIEligible = IsMIEligible( positions );
					for( std::vector<wxString>::iterator it_pos = positions.begin(); it_pos < positions.end(); it_pos++ )
							m_position->Append( (*it_pos) );
					if( (*it).IsHitter() )
					{
						if( isCIEligible )
							m_position->Append( "CI" );
						if( isMIEligible )
							m_position->Append( "MI" );
						m_position->Append( "U" );
					}
				}
				m_position->SetSelection( 0 );
				m_playerInfo->SetCellValue( 0, 0, name );
				m_playerInfo->SetReadOnly( 0, 0 );
				m_playerInfo->SetCellValue( 0, 1, it->GetAbbeviatedTeamName() );
				m_playerInfo->SetReadOnly( 0, 1 );
				m_playerInfo->SetCellValue( 0, 2, it->GetPositions()[0] );
				m_playerInfo->SetReadOnly( 0, 2 );
				m_playerInfo->SetCellValue( 0, 3, wxString::Format( "%d", it->GetAge() ) );
				m_playerInfo->SetReadOnly( 0, 3 );
				m_playerInfo->SetCellValue( 0, 4, wxString::Format( "$%d", (int) it->GetValue() ) );
				m_playerInfo->SetReadOnly( 0, 4 );
				m_playerInfo->SetCellValue( 0, 5, wxString::Format( "$%.2f", it->GetCurrentValue() ) );
				m_playerInfo->SetReadOnly( 0, 5 );
				int column = 6;
				unsigned int gridSize = m_playerInfo->GetNumberCols() - 8;
				if( gridSize > score.size() )
					m_playerInfo->DeleteCols( 6, gridSize - score.size() );
				else if( gridSize < score.size() )
					m_playerInfo->InsertCols( 6, score.size() - gridSize );
				double value = 0.0;
				int val = 0;
				for( std::vector<wxString>::iterator it_score = score.begin(); it_score < score.end(); it_score++ )
				{
					m_playerInfo->SetColLabelValue( column, (*it_score) );
					wxString format = "%d";
					wxString label = (*it_score);
					bool isFloat = false;
					if( label == "AVG" || label == "OBP" || label == "SLG" || label == "OPS" || label == "K/9" || label == "H/9" || label == "BB/9" ||
						label == "K/BB" || label == "K-BB" || label == "Holds" || label == "G" || label == "R" || label == "hr" )
					{
						format = "%.3f";
						value = it->GetScoring()[(*it_score)];
						isFloat = true;
					}
					else if( label == "ERA" || label == "WHIP" || label == "K" )
					{
						format = "%.2f";
						value = it->GetScoring()[(*it_score)];
						isFloat = true;
					}
					else if( label == "W%" )
					{
						format = "%.2f%%";
						value = it->GetScoring()[(*it_score)];
						isFloat = true;
					}
					else if( label == "IP" )
					{
						format = "%.1f";
						value = it->GetScoring()[(*it_score)];
						isFloat = true;
					}
					else
					{
						isFloat = false;
						val = (int)it->GetScoring()[(*it_score)];
					}
					m_playerInfo->SetCellValue( 0, column, isFloat ? wxString::Format( format, value ) : wxString::Format( format, val ) );
					m_playerInfo->SetReadOnly( 0, column++ );
				}
				int col = m_playerInfo->GetNumberCols() - 2;
				m_playerInfo->SetColLabelValue( col, "Notes" );
				m_playerInfo->SetCellValue( 0, col, (*it).GetNotes() );
				m_playerInfo->SetReadOnly( 0, col, false );
				m_playerInfo->AutoSize();
				m_playerInfo->SetColSize( col, m_playerInfo->GetColSize( col ) * 4 );
				grid->Layout();
				for( int i = 0; i < m_playerInfo->GetNumberCols(); i++ )
					m_gridWidths[m_playerInfo->GetColLabelValue( i )] = m_playerInfo->GetColSize( i );
				break;
			}
		}
	}
	else
	{
		m_playerInfo->Unbind(  wxEVT_GRID_COL_AUTO_SIZE, &CPlayerDraft::OnAutoResizing, this );
		m_amount->SetValue( wxEmptyString );
		m_position->SetValue( wxEmptyString );
		m_playerInfo->SetCellValue( 0, 0, wxEmptyString );
		m_playerInfo->SetCellValue( 0, 1, wxEmptyString );
		m_playerInfo->SetCellValue( 0, 2, wxEmptyString );
		m_playerInfo->SetCellValue( 0, 3, wxEmptyString );
		m_playerInfo->SetCellValue( 0, 4, wxEmptyString );
		m_playerInfo->SetCellValue( 0, 5, wxEmptyString );
		for( int i = 6; i < m_playerInfo->GetNumberCols() - 1; i++ )
		{
			m_playerInfo->SetColLabelValue( i, wxEmptyString );
			m_playerInfo->SetCellValue( 0, i, "0" );
		}
		int col = m_playerInfo->GetNumberCols() - 2;
		m_playerInfo->SetColLabelValue( col, "Notes" );
		m_playerInfo->SetCellValue( 0, col, wxEmptyString );
		m_playerInfo->EnableEditing( false );
		m_playerInfo->AutoSize();
		m_gridWidths.clear();
	}
}

CNameCompleter &CPlayerDraft::GetCompleter()
{
	return *m_completer;
}

void CPlayerDraft::OnNameSetFocus(wxFocusEvent &event)
{
	m_player->SelectAll();
	event.Skip();
}

void CPlayerDraft::OnNameKillFocus(wxFocusEvent &event)
{
	wxString name = m_player->GetValue();
	name.SetChar( 0, wxToupper( name.GetChar( 0 ) ) );
	m_player->SetValue( name );
	event.Skip();
}

void CPlayerDraft::OnColumnResized(wxGridSizeEvent &event)
{
	Layout();
	event.Skip();
}

void CPlayerDraft::DraftCompleted()
{
	m_player->SetValue( "" );
	m_owner->Insert( "", 0 );
	m_owner->SetSelection( 0 );
	m_owner->Delete( 0 );
}

void CPlayerDraft::OnAutoResizing(wxGridSizeEvent &event)
{
	int col = event.GetRowOrCol();
	m_playerInfo->SetColumnWidth( col, m_gridWidths[m_playerInfo->GetColLabelValue( col )] );
}

wxButton &CPlayerDraft::GetDraftButton()
{
	return *m_draft;
}

void CPlayerDraft::OnContextMenu(wxGridEvent &WXUNUSED(event))
{
	wxMenu menu;
	wxString name = m_player->GetValue();
	if( name.IsEmpty() )
		return;
	else
	{
		for( std::vector<CPlayer>::iterator it = m_data->m_players->begin(); it < m_data->m_players->end(); it++ )
			if( (*it).GetName() == name )
				dynamic_cast<CFrame *>( GetParent()->GetParent() )->SetContextMenuPlayer( (*it) );
		menu.Append( wxMENU_EDIT_PLAYER, "Edit Player" );
		PopupMenu( &menu );
	}
}

void CPlayerDraft::UpdatePlayer(const CPlayer &player)
{
	std::map<wxString,double> score = const_cast<CPlayer &>( player ).GetScoring();
	for( int i = 1; i < m_playerInfo->GetNumberCols() - 2; i++ )
	{
		wxString label = m_playerInfo->GetColLabelValue( i );
		if( label == "Team" )
		{
			m_playerInfo->SetCellValue( 0, i, const_cast<CPlayer &>( player ).GetTeam() );
			m_playerInfo->SetReadOnly( 0, i );
		}
		else if( label == "Position" )
		{
			m_playerInfo->SetReadOnly( 0, i );
		}
		else if( label == "Age" )
		{
			m_playerInfo->SetCellValue( 0, i, wxString::Format( "%d", player.GetAge() ) );
			m_playerInfo->SetReadOnly( 0, i );
		}
		else if( label == "BegValue" )
		{
			m_playerInfo->SetCellValue( 0, i, wxString::Format( "$%d", player.GetValue() ) );
			m_playerInfo->SetReadOnly( 0, i );
		}
		else if( label == "CurrValue" )
		{
			m_playerInfo->SetCellValue( 0, i, wxString::Format( "$%.2f", player.GetCurrentValue() ) );
			m_playerInfo->SetReadOnly( 0, i );
		}
		else if( label == "Notes" )
		{
			m_playerInfo->SetCellValue( 0, i, const_cast<CPlayer &>( player ).GetNotes() );
		}
		else
		{
			bool found = false;
			for( std::map<wxString,double>::iterator it = score.begin(); it != score.end() && !found; it++ )
			{
				if( label == (*it).first )
				{
					wxString value;
					if( label == "AVG" || label == "OBP" ||	label == "SLG" || label == "OPS" ||	label == "K/9" || label == "H/9" ||
						label == "BB/9" || label == "K/BB" || label == "K-BB" || label == "Holds" || label == "G" || label == "R" || label == "hr" )
						value = wxString::Format( "%.3f", (*it).second );
					else if( label == "ERA" || label == "WHIP" || label == "K" )
						value = wxString::Format( "%.2f", (*it).second );
					else if( label == "W%" )
						value = wxString::Format( "%.2f%%", (*it).second );
					else if( label == "IP" )
						value = wxString::Format( "%.1f", (*it).second );
					else
						value << (*it).second;
					if( value.StartsWith( "0." ) )
						value = value.substr( 1 );
					m_playerInfo->SetCellValue( 0, i, value );
					m_playerInfo->SetReadOnly( 0, i );
					found = true;
				}
			}
		}
	}
}

bool CPlayerDraft::IsCIEligible(const std::vector<wxString> &positions)
{
	bool eligible = false;
	std::vector<wxString>::iterator it;
	eligible = std::find( positions.begin(), positions.end(), "1B" ) != positions.end() && std::find( positions.begin(), positions.end(), "3B" ) != positions.end();
/*	for( it = const_cast<std::vector<wxString> &>( positions ).begin(); it < positions.end() && !eligible; it++ )
	{
		if( (*it) == "1B" )
			eligible = true;
	}
	if( eligible )
	{
		eligible = false;
		for( it = const_cast<std::vector<wxString> &>( positions ).begin(); it < positions.end() && !eligible; it++ )
		{
			if( (*it) == "3B" )
				eligible = true;
		}
	}*/
	return eligible;
}

bool CPlayerDraft::IsMIEligible(const std::vector<wxString> &positions)
{
	bool eligible = false;
	std::vector<wxString>::iterator it;
	eligible = std::find( positions.begin(), positions.end(), "2B" ) != positions.end() && std::find( positions.begin(), positions.end(), "SS" ) != positions.end();
/*	for( it = const_cast<std::vector<wxString> &>( positions ).begin(); it < positions.end() && !eligible; it++ )
	{
		if( (*it) == "2B" )
			eligible = true;
	}
	if( eligible )
	{
		eligible = false;
		for( it = const_cast<std::vector<wxString> &>( positions ).begin(); it < positions.end() && !eligible; it++ )
		{
			if( (*it) == "SS" )
				eligible = true;
		}
	}*/
	return eligible;
}

void CPlayerDraft::AddPlayer(CPlayer player)
{
	m_completer->GetPlayers().push_back( player );
	PlayerSorter sorter;
	sorter.m_sortType = SORT_BY_NAME;
	sorter.m_forward = true;
	std::sort( m_completer->GetPlayers().begin(), m_completer->GetPlayers().end(), sorter );
}

wxSpinCtrl &CPlayerDraft::GetAmountPaidCtrl()
{
	return *m_amount;
}
