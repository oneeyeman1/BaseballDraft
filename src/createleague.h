#pragma once
class CNewLeaguePage : public wxWizardPageSimple
{
public:
	CNewLeaguePage(wxWizard *parent, const CDb &db, int mode, const wxString &leagueTitle, const std::vector<std::string> &leagues);
	~CNewLeaguePage();
	wxPanel *GetFirstPage();
	wxLongLong_t GetLeagueId();
	void OnPageChanging(wxBookCtrlEvent &event);
	void OnPageChanged(wxBookCtrlEvent &event);
	void OnWizardNext(wxCommandEvent &event);
	CLeagueSettings &GetNewLeagueSettings();
	bool IsGood();
private:
	wxNotebook *m_book;
	CDb *m_db;
	wxPanel *m_panel1, *m_panel2, *m_panel3, *m_panel4, *m_panel5;
	CLeagueSettings *m_settings;
	int m_selectedPage, m_mode;
	wxLongLong_t m_leagueId;
	bool m_initialized, m_isUpdate, m_directionForward, m_isGood;
	wxString m_oldLeague;
};

class CCreateLeague : public wxWizard
{
public:
	CCreateLeague(const wxString &title, const CDb &db, int mode, const wxString &leagueTitle, const std::vector<std::string> &leagues);
	~CCreateLeague(void);
	virtual bool RunWizard(wxWizardPage *firstPage);
	CLeagueSettings &GetLeague();
	bool IsGood();
	wxLongLong_t GetLeagueId();
	wxWizardPage *GetFirstPage();
private:
	CNewLeaguePage *m_page1;
	bool m_isGood;
	int m_mode;
	CDb *m_db;
};

