#pragma once
class CMyTipDialog : public wxDialog
{
public:
	CMyTipDialog(wxWindow *parent,/* const CMyTipProvider &tipProvider,*/ bool showTip);
	~CMyTipDialog(void);
	bool ShowTipDialog();
private:
//	CMyTipProvider *m_tipProvider;
	wxTextCtrl *m_tip;
	wxCheckBox *m_showTip;
};

