#pragma once
class CDraftResultPanel : public wxPanel
{
public:
	CDraftResultPanel(wxWindow *parent, const CLeagueSettings &settings, std::vector<CPlayer> &draftReult);
	~CDraftResultPanel(void);
	void AddDraftedPlayer(const CPlayer &player, bool isDrafted);
	void OnTeamSelection(wxCommandEvent &event);
	void UnAssignPlayer(const CPlayer &player);
	void ResetPanel();
private:
	std::vector<CPlayer> m_draftedPlayers;
	wxStaticText *m_label1, *m_label2;
	wxTextCtrl *m_search;
	wxComboBox *m_team;
	wxGrid *m_results;
};

