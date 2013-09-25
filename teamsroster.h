#pragma once
class CTeamsRoster : public wxPanel
{
public:
	CTeamsRoster();
	CTeamsRoster(wxWindow *parent, const CLeagueSettings &settings, const std::vector<CPlayer> &draftResult);
	~CTeamsRoster(void);
	wxGrid &GetRosterControl();
	void PerformDraft(const CPlayer &player);
	void UnAssignPlayer(const CPlayer &player);
	void OnScrollUp(wxScrollWinEvent &event);
	void OnScrollDown(wxScrollWinEvent &event);
	void OnAssignDraftedPlayers(wxCommandEvent &event);
	void ResetRoster();
protected:
	void SetPlayer(const CPlayer &player, bool justCalculate);
	void DisplayOwnerData(int column);
private:
	std::vector<CPlayer> m_draftresult;
	int *m_salary, *m_maxBid, *m_playersLeft, *m_value, *m_amtPaid, *m_profit;
	double *m_average;
	wxGrid *m_roster;
	int m_rowHeight, m_visibleRow;
	DECLARE_DYNAMIC_CLASS(CTeamsRoster)
};
