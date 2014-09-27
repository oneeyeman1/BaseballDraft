#pragma once
class CDraftResult : public wxPanel
{
public:
	CDraftResult(wxWindow *parent, const CLeagueSettings &settings, std::vector<CPlayer> &draftResult);
	~CDraftResult(void);
	void OnResults(wxCommandEvent &event);
	void OnSalary(wxCommandEvent &event);
	void UnAssignPlayer(const CPlayer &player);
	CDraftResultPanel &GetDraftResultPanel();
	CDraftResultSalary &GetDraftResultSalaryPanel();
	void ResetDraftResult();
private:
	wxButton *m_results, *m_salary;
	wxBoxSizer *m_panelSizer, *mainSizer;
	CDraftResultPanel *m_panel1;
	CDraftResultSalary *m_panel2;
};

