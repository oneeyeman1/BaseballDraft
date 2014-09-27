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
#include "wx/collpane.h"
#include "wx/valnum.h"
#include "leaguesettings.h"
#include "enterstats.h"

CEnterStats::CEnterStats(wxWindow *parent, const CLeagueSettings &settings, const wxString &positions, const CPlayer *player) : wxDialog( parent, wxID_ANY, "" )
{
	int i;
	m_dirty = false;
	std::vector<wxString>::iterator it;
	std::vector<wxString> scoring;
	int rows, remainder, score = 0;
	SetTitle( "Enter the stats for the player" );
	if( positions.Find( "P" ) != wxNOT_FOUND )
	{
		m_stats["Wins"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["Wins"], false ) : NewPlayerStats( 0, false );
		m_stats["Saves"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["Saves"], false ) : NewPlayerStats( 0, false );
		m_stats["ERA"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["ERA"], false ) : NewPlayerStats( 0, false );
		m_stats["WHIP"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["WHIP"], false ) : NewPlayerStats( 0, false );
		m_stats["K"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["K"], false ) : NewPlayerStats( 0, false );
		m_stats["IP"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["IP"], false ) : NewPlayerStats( 0, false );
		m_stats["L"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["L"], false ) : NewPlayerStats( 0, false );
		m_stats["H"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["H"], false ) : NewPlayerStats( 0, false );
		m_stats["BB"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["BB"], false ) : NewPlayerStats( 0, false );
		m_stats["ER"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["ER"], false ) : NewPlayerStats( 0, false );
		m_stats["CG"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["CG"], false ) : NewPlayerStats( 0, false );
		m_stats["W-L"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["W-L"], false ) : NewPlayerStats( 0, false );
		m_stats["QS"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["QS"], false ) : NewPlayerStats( 0, false );
		m_stats["Shutouts"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["Shotouts"], false ) : NewPlayerStats( 0, false );
		m_stats["BS"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["BS"], false ) : NewPlayerStats( 0, false );
		m_stats["S-BS"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["S-BS"], false ) : NewPlayerStats( 0, false );
		m_stats["S+Holds"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["S+Holds"], false ) : NewPlayerStats( 0, false );
		m_stats["S+Holds-BS"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["S+Holds-BS"], false ) : NewPlayerStats( 0, false );
		m_stats["K/9"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["K/9"], false ) : NewPlayerStats( 0, false );
		m_stats["H/9"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["H/9"], false ) : NewPlayerStats( 0, false );
		m_stats["BB/9"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["BB/9"], false ) : NewPlayerStats( 0, false );
		m_stats["K/BB"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["K/BB"], false ) : NewPlayerStats( 0, false );
		m_stats["K-BB"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["K-BB"], false ) : NewPlayerStats( 0, false );
		m_stats["W%"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["W%"], false ) : NewPlayerStats( 0, false );
		m_stats["Holds"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["Holds"], false ) : NewPlayerStats( 0, false );
		m_stats["G"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["G"], false ) : NewPlayerStats( 0, false );
		m_stats["R"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["R"], false ) : NewPlayerStats( 0, false );
		m_stats["HR"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["HR"], false ) : NewPlayerStats( 0, false );
		rows = settings.GetPitchScoring().size() / 2;
		remainder = settings.GetPitchScoring().size() % 2;
		scoring = settings.GetPitchScoring();
//		*m_valid = new wxValidator*[settings.GetPitchScoring().size()];
	}
	else
	{
		m_stats["AVG"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["AVG"], false ) : NewPlayerStats( 0, false );
		m_stats["HR"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["HR"], false ) : NewPlayerStats( 0, false );
		m_stats["Runs"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["Runs"], false ) : NewPlayerStats( 0, false );
		m_stats["RBI"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["RBI"], false ) : NewPlayerStats( 0, false );
		m_stats["SB"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["SB"], false ) : NewPlayerStats( 0, false );
		m_stats["OBP"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["OBP"], false ) : NewPlayerStats( 0, false );
		m_stats["SLG"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["SLG"], false ) : NewPlayerStats( 0, false );
		m_stats["OPS"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["OPS"], false ) : NewPlayerStats( 0, false );
		m_stats["BB"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["BB"], false ) : NewPlayerStats( 0, false );
		m_stats["H"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["H"], false ) : NewPlayerStats( 0, false );
		m_stats["K"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["K"], false ) : NewPlayerStats( 0, false );
		m_stats["1B"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["1B"], false ) : NewPlayerStats( 0, false );
		m_stats["2B"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["2B"], false ) : NewPlayerStats( 0, false );
		m_stats["3B"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["3B"], false ) : NewPlayerStats( 0, false );
		m_stats["TB"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["TB"], false ) : NewPlayerStats( 0, false );
		m_stats["2B+3B+HR"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["2B+3B+HR"], false ) : NewPlayerStats( 0, false );
		m_stats["2B+3B"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["2B+3B"], false ) : NewPlayerStats( 0, false );
		m_stats["SB-CS"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["SB-CS"], false ) : NewPlayerStats( 0, false );
		m_stats["CS"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["CS"], false ) : NewPlayerStats( 0, false );
		m_stats["R+RBI-HR"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["R+RBI-HR"], false ) : NewPlayerStats( 0, false );
		m_stats["H+BB"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["H+BB"], false ) : NewPlayerStats( 0, false );
		m_stats["TB+BB"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["TB+BB"], false ) : NewPlayerStats( 0, false );
		m_stats["AB"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["AB"], false ) : NewPlayerStats( 0, false );
		m_stats["HBP"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["HBP"], false ) : NewPlayerStats( 0, false );
		m_stats["GDP"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["GDP"], false ) : NewPlayerStats( 0, false );
		m_stats["SF"] = player ? NewPlayerStats( const_cast<CPlayer *>( player )->GetScoring()["SF"], false ) : NewPlayerStats( 0, false );
		rows = settings.GetHitScoring().size() / 2;
		remainder = settings.GetHitScoring().size() % 2;
		scoring = settings.GetHitScoring();
		m_count = 26;
//		*m_valid = new wxValidator*[m_count];
	}
	m_main = scoring.size();
	m_extra = m_stats.size() - scoring.size();
	int extrarows = m_extra / 2;
	int extraremainder = m_extra % 2;
	m_labels = new wxStaticText*[m_main];
	m_scores = new wxTextCtrl*[m_main];
	m_extraLabels = new wxStaticText*[m_extra];
	m_extraScores = new wxTextCtrl*[m_extra];
	m_panel = new wxPanel( this, wxID_ANY );
	m_pane = new wxCollapsiblePane( m_panel, wxID_ANY, "More Categories" );
	m_enter = new wxButton( m_panel, wxID_OK, "Enter" );
	m_cancel = new wxButton( m_panel, wxID_CANCEL, "Cancel" );
	wxBoxSizer *mainSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *horizontal = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *vertical = new wxBoxSizer( wxVERTICAL );
	wxFlexGridSizer *container = new wxFlexGridSizer( remainder == 0 ? rows : rows + 1, 4, 5, 5 );
	wxStdDialogButtonSizer *buttonSizer = new wxStdDialogButtonSizer();
	horizontal->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	unsigned int counter = 0;
	double value = 0.0;
	wxValidator *valid = NULL;
	int val = 0;
	for( i = 0; i <= rows; i++ )
	{
		wxString format;
		wxString label = scoring.at( score );
		bool isFloat;
		if( label == "AVG" || label == "OBP" || label == "SLG" || label == "OPS" || label == "W%" )
		{
			format = "%.3f";
			value = m_stats[scoring.at( score )].m_value;
			valid = new wxFloatingPointValidator<double>( 3, &value, wxNUM_VAL_ZERO_AS_BLANK );
			isFloat = true;
		}
		else if( label == "ERA" || label == "WHIP" || label == "K/9" || label == "H/9" || label == "BB/9" || label == "K/BB" )
		{
			format = "%.2f";
			value = m_stats[scoring.at( score )].m_value;
			valid = new wxFloatingPointValidator<double>( 2, &value, wxNUM_VAL_ZERO_AS_BLANK );
			isFloat = true;
		}
		else if( label == "IP" )
		{
			format = "%.1f";
			value = m_stats[scoring.at( score )].m_value;
			valid = new wxFloatingPointValidator<double>( 1, &value, wxNUM_VAL_ZERO_AS_BLANK );
			isFloat = true;
		}
		else
		{
			format = "%d";
			isFloat = false;
			val = (int)m_stats[scoring.at( score )].m_value;
			valid = new wxIntegerValidator<int>( &val, wxNUM_VAL_ZERO_AS_BLANK );
		}
		m_labels[counter] = new wxStaticText( m_panel, wxID_ANY, scoring.at( score ) );
		m_scores[counter] = new wxTextCtrl( m_panel, wxID_ANY, isFloat ? wxString::Format( format, value ) : wxString::Format( format, val ), wxDefaultPosition, wxDefaultSize, 0, *valid, scoring.at( score ) );
		delete valid;
		valid = NULL;
		m_stats[scoring[score++]].m_used = true;
		container->Add( m_labels[counter], 0, wxEXPAND, 0 );
		container->Add( m_scores[counter], 0, wxEXPAND, 0 );
		counter++;
		if( counter + 1 < scoring.size() )
		{
			wxString format;
			wxString label = scoring.at( score );
			bool isFloat;
			if( label == "AVG" || label == "OBP" || label == "SLG" || label == "OPS" || label == "W%" )
			{
				format = "%.3f";
				value = m_stats[scoring.at( score )].m_value;
				valid = new wxFloatingPointValidator<double>( 3, &value, wxNUM_VAL_ZERO_AS_BLANK );
				isFloat = true;
			}
			else if( label == "ERA" || label == "WHIP" || label == "K/9" || label == "H/9" || label == "BB/9" || label == "K/BB" )
			{
				format = "%.2f";
				value = m_stats[scoring.at( score )].m_value;
				valid = new wxFloatingPointValidator<double>( 2, &value, wxNUM_VAL_ZERO_AS_BLANK );
				isFloat = true;
			}
			else if( label == "IP" )
			{
				format = "%.1f";
				value = m_stats[scoring.at( score )].m_value;
				valid = new wxFloatingPointValidator<double>( 1, &value, wxNUM_VAL_ZERO_AS_BLANK );
				isFloat = true;
			}
			else
			{
				format = "%d";
				isFloat = false;
				val = (int)m_stats[scoring.at( score )].m_value;
				valid = new wxIntegerValidator<int>( &val, wxNUM_VAL_ZERO_AS_BLANK );
			}
			m_labels[counter] = new wxStaticText( m_panel, wxID_ANY, scoring.at( score ) );
			m_scores[counter] = new wxTextCtrl( m_panel, wxID_ANY, isFloat ? wxString::Format( format, value ) : wxString::Format( format, val ), wxDefaultPosition, wxDefaultSize, 0, *valid, scoring.at( score ) );
			delete valid;
			valid = NULL;
			m_stats[scoring[score++]].m_used = true;
			container->Add( m_labels[counter], 0, wxEXPAND, 0 );
			container->Add( m_scores[counter], 0, wxEXPAND, 0 );
			counter++;
		}
	}
	m_count = counter + 1;
	counter = 0;
	wxWindow *win = m_pane->GetPane();
	m_paneSizer = new wxFlexGridSizer( extraremainder == 0 ? extrarows : extrarows + 1, 4, 5, 5 );
	for( std::map<wxString,NewPlayerStats>::iterator it = m_stats.begin(); it != m_stats.end(); it++ )
	{
		if( !(*it).second.m_used )
		{
			wxString format;
			wxString label = (*it).first;
			bool isFloat;
			if( label == "AVG" || label == "OBP" || label == "SLG" || label == "OPS" || label == "W%" )
			{
				format = "%.3f";
				value = (*it).second.m_value;
				valid = new wxFloatingPointValidator<double>( 3, &value, wxNUM_VAL_ZERO_AS_BLANK );
				isFloat = true;
			}
			else if( label == "ERA" || label == "WHIP" || label == "K/9" || label == "H/9" || label == "BB/9" || label == "K/BB" )
			{
				format = "%.2f";
				value = (*it).second.m_value;
				valid = new wxFloatingPointValidator<double>( 2, &value, wxNUM_VAL_ZERO_AS_BLANK );
				isFloat = true;
			}
			else if( label == "IP" )
			{
				format = "%.1f";
				value = (*it).second.m_value;
				valid = new wxFloatingPointValidator<double>( 1, &value, wxNUM_VAL_ZERO_AS_BLANK );
				isFloat = true;
			}
			else
			{
				format = "%d";
				isFloat = false;
				val = (int)(*it).second.m_value;
				valid = new wxIntegerValidator<int>( &val, wxNUM_VAL_ZERO_AS_BLANK );
			}
			m_extraLabels[counter] = new wxStaticText( win, wxID_ANY, (*it).first );
			m_extraScores[counter] = new wxTextCtrl( win, wxID_ANY, isFloat ? wxString::Format( format, value ) : wxString::Format( format, val ), wxDefaultPosition, wxDefaultSize, 0, *valid, (*it).first );
			delete valid;
			valid = NULL;
			m_paneSizer->Add( m_extraLabels[counter], 0, wxEXPAND, 0 );
			m_paneSizer->Add( m_extraScores[counter], 0, wxEXPAND, 0 );
			counter++;
			m_count++;
		}
	}
	win->SetSizer( m_paneSizer );
	m_paneSizer->SetSizeHints( win );
	vertical->Add( container, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( m_pane, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	buttonSizer->AddButton( m_enter );
	buttonSizer->AddButton( m_cancel );
	buttonSizer->Realize();
	vertical->Add( buttonSizer, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	horizontal->Add( vertical, 0, wxEXPAND, 0 );
	horizontal->Add( 5, 5, 0, wxEXPAND, 0 );
	m_panel->SetSizer( horizontal );
	mainSizer->Add( m_panel, 0, wxEXPAND, 0 );
	SetSizer( mainSizer );
	mainSizer->SetSizeHints( this );
	m_scores[0]->SetFocus();
	for( i = 0; i < m_main; i++ )
		m_scores[i]->Bind( wxEVT_COMMAND_TEXT_UPDATED, &CEnterStats::OnModifyingStats, this );
	for( i = 0; i < m_extra; i++ )
		m_extraScores[i]->Bind( wxEVT_COMMAND_TEXT_UPDATED, &CEnterStats::OnModifyingStats, this );
	m_enter->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &CEnterStats::OnEnterData, this );
	m_pane->Bind( wxEVT_COMMAND_COLLPANE_CHANGED, &CEnterStats::OnCollapsiblePane, this );
}

CEnterStats::~CEnterStats(void)
{
	int i;
	for( i = 0; i < m_main; i++ )
		m_scores[i]->Unbind( wxEVT_COMMAND_TEXT_UPDATED, &CEnterStats::OnModifyingStats, this );
	for( i = 0; i < m_extra; i++ )
		m_extraScores[i]->Unbind( wxEVT_COMMAND_TEXT_UPDATED, &CEnterStats::OnModifyingStats, this );
	delete m_labels;
	delete m_scores;
	delete m_extraLabels;
	delete m_extraScores;
}

void CEnterStats::OnEnterData(wxCommandEvent &WXUNUSED(event))
{
	double value = 0.0;
	wxString val;
	int i;
	for( i = 0; i < m_main; i++ )
	{
		val = m_scores[i]->GetValue();
		if( !val.IsEmpty() )
		{
			val.ToDouble( &value );
			m_stats[m_scores[i]->GetName()].m_value = value;
		}
		else
			m_stats[m_scores[i]->GetName()].m_value = 0;
	}
	for( i = 0; i < m_extra; i++ )
	{
		val = m_extraScores[i]->GetValue();
		if( !val.IsEmpty() )
		{
			val.ToDouble( &value );
			m_stats[m_extraScores[i]->GetName()].m_value = value;
		}
		else
			m_stats[m_extraScores[i]->GetName()].m_value = 0;
	}
	EndDialog( wxID_OK );
}

std::map<wxString,NewPlayerStats> &CEnterStats::GetStatistics()
{
	return m_stats;
}

void CEnterStats::OnCollapsiblePane(wxCollapsiblePaneEvent &event)
{
	if( event.GetCollapsed() )
		return;
	else
		Center();
}

void CEnterStats::OnModifyingStats(wxCommandEvent &WXUNUSED(event))
{
	if( !m_dirty )
		m_dirty = true;
}

bool CEnterStats::IsDirty()
{
	return m_dirty;
}
