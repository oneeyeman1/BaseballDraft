// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "textctrl.h"

CTextCtrl::CTextCtrl(wxWindow *parent, wxWindowID id, const wxString &value, const wxPoint &pos, const wxSize &size, long style) : wxTextCtrl( parent, id, value, pos, size, style )
{
	Bind( wxEVT_LEFT_DOWN, &CTextCtrl::OnMouseDown, this );
}

CTextCtrl::~CTextCtrl(void)
{
	Unbind( wxEVT_LEFT_DOWN, &CTextCtrl::OnMouseDown, this );
}

void CTextCtrl::OnMouseDown(wxMouseEvent &event)
{
	SelectAll();
	event.Skip();
}

wxSize CTextCtrl::DoGetBestSize() const
{
	int cx, cy;
	wxMyGetCharSize( GetHWND(), &cx, &cy, GetFont() );
	int wText = 200;
	int hText = cy;
	if( m_windowStyle & wxTE_MULTILINE )
	{
		hText *= wxMax( wxMin( GetNumberOfLines(), 10 ), 2 );
	}
	if( !HasFlag(wxBORDER_NONE) )
	{
		// we have to add the adjustments for the control height only once, not
		// once per line, so do it after multiplication above
		hText += EDIT_HEIGHT_FROM_CHAR_HEIGHT( cy ) - cy;
	}
	return wxSize(wText, hText);
}

#ifdef __WXMSW__
void wxMyGetCharSize(WXHWND wnd, int *x, int *y, const wxFont& the_font)
{
    TEXTMETRIC tm;
    HDC dc = ::GetDC((HWND) wnd);
    HFONT was = 0;

    //    the_font.UseResource();
    //    the_font.RealizeResource();
    HFONT fnt = (HFONT)the_font.GetResourceHandle(); // const_cast
    if ( fnt )
        was = (HFONT) SelectObject(dc,fnt);

    GetTextMetrics(dc, &tm);
    if ( fnt && was )
    {
        SelectObject(dc,was);
    }
    ReleaseDC((HWND)wnd, dc);

    if ( x )
        *x = tm.tmAveCharWidth;
    if ( y )
        *y = tm.tmHeight + tm.tmExternalLeading;

    //   the_font.ReleaseResource();
}
#endif