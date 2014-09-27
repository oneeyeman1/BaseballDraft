#pragma once
class CDraftResultSalary :	public wxPanel
{
public:
	CDraftResultSalary(wxWindow *parent, const CLeagueSettings &settings, const std::vector<CPlayer> &draftResult);
	~CDraftResultSalary(void);
	void Resize();
	void PerformDraft(const CPlayer &player);
	void UnAssignPlayer(const CPlayer &player);
	void ResetPanel();
private:
	int m_totalSalary, m_players;
	wxGrid *m_salaryInfo;
};

