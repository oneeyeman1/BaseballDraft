// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include <set>
#include <map>
#include "sqlite3.h"
#include "wx/tipdlg.h"
#include "wx/grid.h"
#include "wx/tglbtn.h"
#include "wx/renderer.h"
#include "wx/textcompleter.h"
#include "textctrl.h"
#include "leaguesettings.h"
#include "db.h"
#include "namecompleter.h"
#include "playerdraft.h"
#include "teamsroster.h"
#include "playerspanel.h"
#include "teamprojections.h"
#include "draftresultpanel.h"
#include "draftresultsalary.h"
#include "draftresult.h"
#include "gridtotalcell.h"
#include "frame.h"

CGridTotalCell::CGridTotalCell(wxString label)
{
	m_label = label;
}

CGridTotalCell::~CGridTotalCell(void)
{
}

void CGridTotalCell::Draw(wxGrid &grid, wxGridCellAttr &WXUNUSED(attr), wxDC &dc, const wxRect &rect, int WXUNUSED(row), int WXUNUSED(col), bool WXUNUSED(isSelected))
{
	wxRendererNative::Get().DrawPushButton( &grid, dc, rect, wxCONTROL_CURRENT );
	wxFont font = grid.GetFont();
	dc.SetFont( font );
	int x = rect.x + 10;
	int y = rect.y + 1;
	dc.DrawText( m_label, x, y );
}

wxSize CGridTotalCell::GetBestSize(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, int row, int col)
{
	wxString value = grid.GetCellValue( row, col );
	dc.SetFont( attr.GetFont() );
	return dc.GetTextExtent( value );
}

wxGridCellRenderer *CGridTotalCell::Clone() const
{
	return new CGridTotalCell( m_label );
}

IMPLEMENT_DYNAMIC_CLASS(CMyGrid, wxGrid)

CMyGrid::CMyGrid() : wxGrid()
{
}

CMyGrid::CMyGrid(wxWindow *parent) : wxGrid( parent, wxID_ANY )
{
	Bind( wxEVT_GRID_CELL_RIGHT_CLICK, &CFrame::OnContextMenu, dynamic_cast<CFrame *>( parent->GetParent() ) );
}

CMyGrid::~CMyGrid()
{
}

wxSize CMyGrid::DoGetBestSize() const
{
	CMyGrid * const self = const_cast<CMyGrid *>( this );
	wxSize size( self->CalcColumnSizes( true ) - GetRowLabelSize(), // + m_extraWidth
		self->CalcRowSizes( true ) - GetColLabelSize() ); // + m_extraWidth );
    return wxSize( size.x + m_rowLabelWidth, size.y + m_colLabelHeight ) + GetWindowBorderSize();
}

int CMyGrid::CalcColumnSizes(bool calcOnly, bool setAsMin)
{
	int width = GetRowLabelSize();
	wxGridUpdateLocker locker;
	if( !calcOnly )
		locker.Create( this );
	for( int i = 0; i < wxMin( GetNumberCols(), 9 ); i++ )
	{
		if( !calcOnly )
			AutoSizeColumn( i, setAsMin );
		width += GetColWidth( i );
	}
	return width;
}

int CMyGrid::CalcRowSizes(bool calcOnly, bool setAsMin)
{
	int height = GetColLabelSize();
	wxGridUpdateLocker locker;
	if( !calcOnly )
		locker.Create( this );
	for( int i = 0; i < wxMin( GetNumberRows(), 11 ); i++ )
	{
		if( !calcOnly )
			AutoSizeRow( i, setAsMin );
		height += GetRowHeight( i );
	}
	return height;
}
