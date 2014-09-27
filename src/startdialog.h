#pragma once
class CStartDialog : public wxWizard
{
public:
	CStartDialog(const wxString &title);
	~CStartDialog(void);
	CLeagueSettings *GetSelectedLeague();
	bool IsDialogGood();
	wxString GetLeagueName();
	wxLongLong_t GetLeagueId();
	CDb *GetDatabase();
	wxWizardPage *GetFirstPage();
	virtual bool RunWizard(wxWizardPage *firstPage);
	void OnExit(wxWizardEvent &event);
	void OnLeagueDelete(wxCommandEvent &event);
	void OnLeagueCreated(wxCommandEvent &event);
	void OnSelect(wxCommandEvent &event);
protected:
	wxPanel *CreateOrSelectLeagueWizard(wxWizard *parent, std::vector<std::string> &leagues);
private:
	CDb *m_db;
	bool m_good;
	wxLongLong_t m_leagueId;
	std::vector<std::string> m_leaguesList;
	CLeagueSettings *m_league;
	wxWizardPageSimple *m_panel;
	wxStaticBitmap *m_labelLogo;
	wxStaticText *m_label, *m_label1, *m_label2;
	wxButton *m_new, *m_edit, *m_delete;
	wxListBox *m_leagues;
	wxHyperlinkCtrl *m_url;
};

