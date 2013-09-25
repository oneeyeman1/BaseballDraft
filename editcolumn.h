#pragma once
class CEditColumn :	public wxDialog
{
public:
	CEditColumn(wxWindow *parent, CLeagueSettings &settings, const std::map<wxString,bool> &columns_displayedHitters, const std::map<wxString,bool> &columns_displayedPitchers, const std::map<wxString,bool> &columnsDisplayed);
	~CEditColumn(void);
	CNewLeagueScoring &GetScoringPanel();
	void OnColumnSelection(wxCommandEvent &event);
protected:
	void DoLayout();
private:
	wxPanel *m_panel;
	wxCheckBox *m_begValue, *m_curValue, *m_amtPaid, *m_profit, *m_team, *m_notes, *m_age;
	CNewLeagueScoring *m_page;
	wxButton *m_ok, *m_cancel;
	std::map<wxString,bool> *m_displayedColumns, *m_displayedHitters, *m_displayedPitchers;
};

