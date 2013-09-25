#pragma once
class CGridTotalCell :	public wxGridCellRenderer
{
public:
	CGridTotalCell(wxString label);
	~CGridTotalCell();
	virtual void Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, const wxRect &rect, int row, int col, bool isSelected);
	virtual wxSize GetBestSize(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, int row, int col);
	virtual wxGridCellRenderer *Clone() const;
private:
	wxString m_label;
};

class CMyGrid : public wxGrid
{
public:
	CMyGrid();
	~CMyGrid();
	CMyGrid(wxWindow *parent);
protected:
	virtual wxSize DoGetBestSize() const;
    int CalcColumnSizes(bool calcOnly, bool setAsMin = true);
    int CalcRowSizes(bool calcOnly, bool setAsMin = true);
	DECLARE_DYNAMIC_CLASS(CMyGrid)
};
