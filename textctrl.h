#pragma once
class CTextCtrl :	public wxTextCtrl
{
public:
	CTextCtrl(wxWindow *parent, wxWindowID id, const wxString &value=wxEmptyString, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=0);
	~CTextCtrl(void);
	void OnMouseDown(wxMouseEvent &event);
protected:
	virtual wxSize DoGetBestSize() const;
};

#ifdef __WXMSW__
void wxMyGetCharSize(WXHWND wnd, int *x, int *y, const wxFont& the_font);
#define EDIT_HEIGHT_FROM_CHAR_HEIGHT(cy)    (cy+8)
#endif
