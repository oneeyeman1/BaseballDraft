#pragma once
class CNewPlayerPositions :	public wxComboCtrl
{
public:
	CNewPlayerPositions(void);
	~CNewPlayerPositions(void);
};

class PositionsPopup : public wxCheckListBox, public wxComboPopup
{
public:
	PositionsPopup();
	~PositionsPopup();
	virtual bool Create(wxWindow *parent);
	virtual wxWindow *GetControl();
	virtual wxString GetStringValue() const;
#if !wxCHECK_VERSION( 2, 9, 5 )
	unsigned int GetCheckedItems(wxArrayInt &checkedItems) const;
#endif
};
