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
#include "wx/grid.h"
#include "leaguesettings.h"
#include "playerspanelgridtable.h"

CPlayersPanelGridTable::CPlayersPanelGridTable(const std::vector<CPlayer> players) : wxGridTableBase()
{
	m_sortOrder = NULL;
	m_players = players;
}

CPlayersPanelGridTable::~CPlayersPanelGridTable(void)
{
}

int CPlayersPanelGridTable::GetNumberRows()
{
	return m_players.size();
}

int CPlayersPanelGridTable::GetNumberCols()
{
	return COL_MAX;
}

wxString CPlayersPanelGridTable::GetValue(int row, int col)
{
	wxString value, temp;
	CPlayer player = m_players.at( row );
	if( m_sortOrder )
		row = m_sortOrder[row];
	switch( col )
	{
		case COL_RANGE:
			temp = wxString::Format( "%d", player.GetRange() );
			break;
		case COL_NAME:
			temp = player.GetName();
			break;
		case COL_TEAM:
			temp = player.GetAbbeviatedTeamName();
			break;
		case COL_POSITION:
			{
				for( std::vector<wxString>::iterator it_pos = player.GetPositions().begin(); it_pos < player.GetPositions().end(); it_pos++ )
				{
					temp += *it_pos;
					if( it_pos != player.GetPositions().end() - 1 )
						temp += ", ";
				}
				break;
			}
		case COL_AGE:
			temp = wxString::Format( "%d", player.GetAge() );
			break;
		case COL_AVG:
			value = wxString::Format( "%.3f", player.GetScoring()["AVG"] );
			if( !player.IsHitter() )
				temp = wxEmptyString;
			else if( value == "0.000" )
				temp = "0";
			else if( value.StartsWith( "0." ) )
				temp = value.substr( 1 );
			else
				temp = value;
			break;
		case COL_HR:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["HR"] ) : wxEmptyString;
			break;
		case COL_RUNS:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["HR"] ) : wxEmptyString;
			break;
		case COL_RBI:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["RBI"] ) : wxEmptyString;
			break;
		case COL_SB:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["SB"] ) : wxEmptyString;
			break;
		case COL_OBP:
			value = wxString::Format( "%.3f", player.GetScoring()["OBP"] );
			if( !player.IsHitter() )
				temp = wxEmptyString;
			else if( value == "0.000" )
				temp = "0";
			else if( value.StartsWith( "0." ) )
				temp = value.substr( 1 );
			else
				temp = value;
			break;
		case COL_SLG:
			value = wxString::Format( "%.3f", player.GetScoring()["SLG"] );
			if( !player.IsHitter() )
				temp = wxEmptyString;
			else if( value == "0.000" )
				temp = "0";
			else if( value.StartsWith( "0." ) )
				temp = value.substr( 1 );
			else
				temp = value;
			break;
		case COL_OPS:
			value = wxString::Format( "%.3f", player.GetScoring()["OBS"] );
			if( !player.IsHitter() )
				temp = wxEmptyString;
			else if( value == "0.000" )
				temp = "0";
			else if( value.StartsWith( "0." ) )
				temp = value.substr( 1 );
			else
				temp = value;
			break;
		case COL_bb:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["bb"] ) : wxEmptyString;
			break;
		case COL_h:
			temp = m_players.at( row ).IsHitter() ? wxString::Format( "%d", (int) m_players.at( row ).GetScoring()["h"] ) : wxEmptyString;
			break;
		case COL_Ks:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["Ks"] ) : wxEmptyString;
			break;
		case COL_1B:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["1B"] ) : wxEmptyString;
			break;
		case COL_2B:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["2B"] ) : wxEmptyString;
			break;
		case COL_3B:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["3B"] ) : wxEmptyString;
			break;
		case COL_TB:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["TB"] ) : wxEmptyString;
			break;
		case COL_2B_3B_HR:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["2B+3B+HR"] ) : wxEmptyString;
			break;
		case COL_2B_3B:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["2B+3B"] ) : wxEmptyString;
			break;
		case COL_SB_CS:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["SB-CS"] ) : wxEmptyString;
			break;
		case COL_CS:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["CS"] ) : wxEmptyString;
			break;
		case COL_R_RBI_HR:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["R+RBI-HR"] ) : wxEmptyString;
			break;
		case COL_H_BB:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["H+BB"] ) : wxEmptyString;
			break;
		case COL_TB_BB:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["TB+BB"] ) : wxEmptyString;
			break;
		case COL_AB:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["AB"] ) : wxEmptyString;
			break;
		case COL_HBP:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["HBP"] ) : wxEmptyString;
			break;
		case COL_GDP:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["GDP"] ) : wxEmptyString;
			break;
		case COL_SF:
			temp = player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["SF"] ) : wxEmptyString;
			break;
		case COL_BEGVALUE:
			temp = wxString::Format( "$%d", (int) player.GetValue() );
			break;
		case COL_CURRVALUE:
			temp = wxString::Format( "$%.2f", player.GetCurrentValue() );
			break;
		case COL_AMTPAID:
			temp = player.IsPlayerDrafted() ? wxString::Format( "$%d", player.GetAmountPaid() ) : wxEmptyString;
			break;
		case COL_PROFIT:
			{
				int profit = player.GetValue() - player.GetAmountPaid();
				temp = profit == 0 ? wxEmptyString : !player.IsPlayerDrafted() ? wxEmptyString : wxString::Format( "$%d", profit );
				break;
			}
		case COL_OWNER:
			temp = player.IsPlayerDrafted() ? player.GetOwner() : wxEmptyString;
			break;
		case COL_WINS:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["Wins"] ) : wxEmptyString;
			break;
		case COL_SAVES:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["Saves"] ) : wxEmptyString;
			break;
		case COL_ERA:
			temp = !player.IsHitter() ? wxString::Format( "%.2f", player.GetScoring()["ERA"] ) : wxEmptyString;
			break;
		case COL_WHIP:
			temp = !player.IsHitter() ? wxString::Format( "%.2f", player.GetScoring()["WHIP"] ) : wxEmptyString;
			break;
		case COL_K:
			temp = !player.IsHitter() ? wxString::Format( "%.2f", player.GetScoring()["K"] ) : wxEmptyString;
			break;
		case COL_IP:
			temp = !player.IsHitter() ? wxString::Format( "%.1f", player.GetScoring()["IP"] ) : wxEmptyString;
			break;
		case COL_L:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["L"] ) : wxEmptyString;
			break;
		case COL_H:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["H"] ) : wxEmptyString;
			break;
		case COL_BB:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["BB"] ) : wxEmptyString;
			break;
		case COL_ER:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["ER"] ) : wxEmptyString;
			break;
		case COL_CG:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["CG"] ) : wxEmptyString;
			break;
		case COL_W_L:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["W-L"] ) : wxEmptyString;
			break;
		case COL_QS:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["QS"] ) : wxEmptyString;
			break;
		case COL_SHUTOUTS:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["Shutouts"] ) : wxEmptyString;
			break;
		case COL_BS:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["BS"] ) : wxEmptyString;
			break;
		case COL_S_BS:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["S-BS"] ) : wxEmptyString;
			break;
		case COL_S_HOLDS:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["S+HOLDS"] ) : wxEmptyString;
			break;
		case COL_S_HOLDS_BS:
			temp = !player.IsHitter() ? wxString::Format( "%d", (int) player.GetScoring()["S+HOLDS-BS"] ) : wxEmptyString;
			break;
		case COL_K_9:
			temp = !player.IsHitter() ? wxString::Format( "%.3f", (double) player.GetScoring()["K/9"] ) : wxEmptyString;
			break;
		case COL_H_9:
			temp = !player.IsHitter() ? wxString::Format( "%.3f", (double) player.GetScoring()["H/9"] ) : wxEmptyString;
			break;
		case COL_BB_9:
			temp = !player.IsHitter() ? wxString::Format( "%.3f", (double) player.GetScoring()["BB/9"] ) : wxEmptyString;
			break;
		case COL_K_DIV_BB:
			temp = !player.IsHitter() ? wxString::Format( "%.3f", (double) player.GetScoring()["K/BB"] ) : wxEmptyString;
			break;
		case COL_K_BB:
			temp = !player.IsHitter() ? wxString::Format( "%.3f", (double) player.GetScoring()["K-BB"] ) : wxEmptyString;
			break;
		case COL_W_PERCENT:
			temp = !player.IsHitter() ? wxString::Format( "%.2f%%", (double) player.GetScoring()["W%"] ) : wxEmptyString;
			break;
		case COL_HOLDS:
			temp = !player.IsHitter() ? wxString::Format( "%.3f", (double) player.GetScoring()["Holds"] ) : wxEmptyString;
			break;
		case COL_G:
			temp = !player.IsHitter() ? wxString::Format( "%.3f", (double) player.GetScoring()["G"] ) : wxEmptyString;
			break;
		case COL_R:
			temp = !player.IsHitter() ? wxString::Format( "%.3f", (double) player.GetScoring()["R"] ) : wxEmptyString;
			break;
		case COL_hr:
			temp = !player.IsHitter() ? wxString::Format( "%.3f", (double) player.GetScoring()["hr"] ) : wxEmptyString;
			break;
		case COL_NOTES:
			temp = player.GetNotes();
			break;
	}
	wxGrid *grid = GetView();
	grid->SetReadOnly( row, col );
	return temp;
}

wxString CPlayersPanelGridTable::GetColLabelValue(int col)
{
	static const char *labels[] =
	{
		"Rank",
		"Player",
		"Team",
		"Position",
		"Age",
		"AVG",
		"HR",
		"Runs",
		"RBI",
		"SB",
		"OBP",
		"SLG",
		"OPS",
		"bb",
		"h",
		"Ks",
		"1B",
		"2B",
		"3B",
		"TB",
		"2B+3B+HR",
		"2B+3B",
		"SB-CS",
		"CS",
		"R+RBI-HR",
		"H+BB",
		"TB+BB",
		"AB",
		"HBP",
		"GDP",
		"SF",
		"BegValue",
		"CurrValue",
		"AmtPaid",
		"Profit",
		"Team",
		"Wins",
		"Saves",
		"ERA",
		"WHIP",
		"K",
		"IP",
		"L",
		"H",
		"BB",
		"ER",
		"CG",
		"W-L",
		"QS",
		"Shutouts",
		"BS",
		"S-BS",
		"S+Holds",
		"S+Holds-BS",
		"K/9",
		"H/9",
		"BB/9",
		"K/BB",
		"K-BB",
		"W%",
		"Holds",
		"G",
		"R",
		"hr",
		"Notes"
	};
	wxCOMPILE_TIME_ASSERT( WXSIZEOF( labels ) == COL_MAX, LabelsMismatch );
	return labels[col];
}

void CPlayersPanelGridTable::SetValue(int, int, const wxString&)
{
    wxFAIL_MSG( "shouldn't be called" );
}
