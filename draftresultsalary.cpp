// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <map>
#include <vector>
#include "wx/tipdlg.h"
#include "wx/grid.h"
#include "leaguesettings.h"
#include "draftresultsalary.h"

CDraftResultSalary::CDraftResultSalary(wxWindow *parent, const CLeagueSettings &settings, const std::vector<CPlayer> &draftResult) : wxPanel( parent )
{
	int row = 0;
	m_totalSalary = settings.GetSalary();
	std::map<wxString, int> positions = settings.GetPositions();
	m_players = positions["C"] + positions["1B"] + positions["3B"] + positions["CI"] + positions["2B"] + positions["SS"] + positions["MI"] + positions["OF"] + positions["Utility"] + positions["P"];
	if( !settings.GetLeagueType() )
		m_players += positions["DH"] + positions["SP"] + positions["RP"];
	m_salaryInfo = new wxGrid( this, wxID_ANY );
	m_salaryInfo->CreateGrid( settings.GetOwnersNumber(), 7, wxGrid::wxGridSelectRows );
	m_salaryInfo->BeginBatch();
	m_salaryInfo->EnableEditing( false );
	for( std::vector<std::string>::iterator it = settings.GetOwners().begin(); it < settings.GetOwners().end(); it++ )
	{
		m_salaryInfo->SetRowLabelValue( row, (*it) );
		m_salaryInfo->SetCellValue( row++, 0, wxString::Format( "%d", m_totalSalary ) );
	}
	m_salaryInfo->SetColLabelValue( 0, "Salary" );
	m_salaryInfo->SetColLabelValue( 1, "Spent" );
	m_salaryInfo->SetColLabelValue( 2, "$ Left" );
	m_salaryInfo->SetColLabelValue( 3, "# Players Left" );
	m_salaryInfo->SetColLabelValue( 4, "$ / Players Left" );
	m_salaryInfo->SetColLabelValue( 5, "Max Bid" );
	m_salaryInfo->SetColLabelValue( 6, "Profit" );
	m_salaryInfo->AutoSize();
	for( std::vector<CPlayer>::const_iterator it = draftResult.begin(); it < draftResult.end(); it++ )
		PerformDraft( (*it) );
	m_salaryInfo->EndBatch();
	wxBoxSizer *mainSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *vertical = new wxBoxSizer( wxVERTICAL );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	vertical->Add( m_salaryInfo, 1, wxEXPAND, 0 );
	vertical->Add( 5, 5, 0, wxEXPAND, 0 );
	mainSizer->Add( vertical, 1, wxEXPAND, 0 );
	mainSizer->Add( 5, 5, 0, wxEXPAND, 0 );
	SetSizerAndFit( mainSizer );
}

CDraftResultSalary::~CDraftResultSalary(void)
{
}

void CDraftResultSalary::Resize()
{
	m_salaryInfo->AutoSize();
}

void CDraftResultSalary::PerformDraft(const CPlayer &player)
{
	int row;
	bool found = false;
	wxString totalAmountPaid, totalPlayersLeft, totalProfit;
	int totalAmtPaid = 0, total$Left = 0, playersLeft = 0, profit = 0;
	for( row = 0; row < m_salaryInfo->GetNumberRows() && !found; row++ )
	{
		if( m_salaryInfo->GetRowLabelValue( row ) == const_cast<CPlayer &>( player ).GetOwner() )
		{
			totalAmountPaid = m_salaryInfo->GetCellValue( row, 1 );
			if( !totalAmountPaid.IsEmpty() )
			{
				totalAmtPaid = wxAtoi( totalAmountPaid );
				totalAmtPaid += player.GetAmountPaid();
			}
			else
				totalAmtPaid = player.GetAmountPaid();
			total$Left = m_totalSalary - totalAmtPaid;
			totalPlayersLeft = m_salaryInfo->GetCellValue( row, 3 );
			if( !totalPlayersLeft.IsEmpty() )
			{
				playersLeft = wxAtoi( totalPlayersLeft );
				playersLeft--;
			}
			else
				playersLeft = m_players - 1;
			totalProfit = m_salaryInfo->GetCellValue( row, 6 );
			if( !totalProfit.IsEmpty() )
			{
				profit = wxAtoi( totalProfit );
				profit += ( player.GetValue() - player.GetAmountPaid() );
			}
			else
				profit = player.GetValue() - player.GetAmountPaid();
			found = true;
		}
	}
	row--;
	m_salaryInfo->SetCellValue( row, 1, wxString::Format( "%d", totalAmtPaid ) );
	m_salaryInfo->SetCellValue( row, 2, wxString::Format( "%d", total$Left) );
	m_salaryInfo->SetCellValue( row, 3, wxString::Format( "%d", playersLeft ) );
	m_salaryInfo->SetCellValue( row, 4, wxString::Format( "%.3f", (double) total$Left / playersLeft  ) );
	m_salaryInfo->SetCellValue( row, 5, wxString::Format( "%d", total$Left - (playersLeft - 1 ) ) );
	m_salaryInfo->SetCellValue( row, 6, wxString::Format( "%d", profit ) );
}

void CDraftResultSalary::UnAssignPlayer(const CPlayer &player)
{
	bool found = false;
	int row = 0;
	for( int i = 0; i < m_salaryInfo->GetNumberRows() && !found; i++ )
	{
		if( m_salaryInfo->GetRowLabelValue( i ) == const_cast<CPlayer &>( player ).GetOwner() )
		{
			found = true;
			row = i;
		}
	}
	wxString totalAmountPaid = m_salaryInfo->GetCellValue( row, 1 );
	wxString total$Left = m_salaryInfo->GetCellValue( row, 2 );
	wxString playersLeft = m_salaryInfo->GetCellValue( row, 3 );
	wxString totalByPlayersLeft = m_salaryInfo->GetCellValue( row, 4 );
	wxString maxBid = m_salaryInfo->GetCellValue( row, 5 );
	wxString profit = m_salaryInfo->GetCellValue( row, 6 );
	int totalAmount = wxAtoi( totalAmountPaid ) - player.GetAmountPaid();
	m_salaryInfo->SetCellValue( row, 1, totalAmount == 0 ? wxEmptyString : wxString::Format( "%d", totalAmount ) );
	int total$ = m_totalSalary - totalAmount;
	m_salaryInfo->SetCellValue( row, 2, total$ == 0 ? wxEmptyString : wxString::Format( "%d", total$ ) );
	int totalPlayersLeft = wxAtoi( playersLeft ) + 1;
	m_salaryInfo->SetCellValue( row, 3, wxString::Format( "%d", totalPlayersLeft ) );
	m_salaryInfo->SetCellValue( row, 4, totalPlayersLeft == 0 ? wxEmptyString : wxString::Format( "%.3f", (double) total$ / totalPlayersLeft ) );
	int maximumBid = total$ - (totalPlayersLeft - 1 );
	m_salaryInfo->SetCellValue( row, 5, maximumBid == 0 ? wxEmptyString : wxString::Format( "%d", maximumBid ) );
	int currentProfit = player.GetValue() - player.GetAmountPaid();
	m_salaryInfo->SetCellValue( row, 6, wxString::Format( "%d", wxAtoi( profit ) - currentProfit ) );
}

void CDraftResultSalary::ResetPanel()
{
	for( int i = 0; i < m_salaryInfo->GetNumberRows(); i++ )
		for( int j = 0; j < m_salaryInfo->GetNumberCols(); j++ )
			m_salaryInfo->SetCellValue( i, j, wxEmptyString );
}
