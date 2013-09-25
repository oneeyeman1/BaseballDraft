#pragma once
class CPropertyGrid : public wxPropertyGrid
{
public:
	CPropertyGrid(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style ) : wxPropertyGrid( parent, id, pos, size, style ) {}
protected:
	virtual wxSize DoGetBestSize() const
	{
//		wxSize size = wxPropertyGrid::GetBestSize();
		int lineHeight = wxMax( 15, m_lineHeight );
		// don't make the grid too tall (limit height to 10 items) but don't
		// make it too small neither
		int numLines = 14;
		const wxSize sz = wxSize( -1, lineHeight * numLines + 5 );
	    CacheBestSize(sz);
		return sz;
	}
};

class CNewLeaguePositions :	public wxPanel
{
public:
	CNewLeaguePositions(wxWindow *parent, const CLeagueSettings *settings);
	~CNewLeaguePositions(void);
	virtual bool Validate();
	std::map<wxString,int> &GetPositions();
	void SetDHValue();
	bool IncludeBenchPlayers();
//	virtual bool Show(bool show = true);
private:
	std::map<wxString,int> m_properties;
	CPropertyGrid *m_positions;
	wxStaticText *m_label1;
	wxCheckBox *m_benchPlayers;
};
