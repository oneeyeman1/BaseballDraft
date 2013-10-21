#pragma once
class CAddNewPlayer : public wxDialog
{
public:
	CAddNewPlayer(wxWindow *parent, const wxString &title, const std::map<wxString,wxString> &names, const CLeagueSettings &settings, const CPlayer *player, int rank = 0);
	~CAddNewPlayer(void);
	virtual bool Validate();
	CPlayer &GetNewPlayer();
	void OnAddNewPlayer(wxCommandEvent &event);
	void OnEnterStats(wxCommandEvent &event);
	void PlayerChanging(wxCommandEvent &event);
	double GetChangedCurrentValue();
	long GetChangedAmountPaid();
	int GetChangedRank();
	int GetChangedAge();
	wxString &GetChangedTeam();
	wxString GetChangedAbbreviatedTeam();
	std::map<wxString, double> &GetScoring();
	wxString &GetChangedPositions();
	bool PlayerAddedModified();
protected:
	void SetProperties(const CPlayer *player, const wxString &title);
	bool CheckPlayerPosition();
private:
	bool m_isHitter;
	std::map<wxString,double> m_statistics;
	std::map<wxString,NewPlayerStats> m_newStats;
	std::map<wxString,wxString> m_teams;
	std::vector<wxString> m_pos;
	wxString m_positions;
	wxPanel *m_panel;
	wxStaticText *m_label1, *m_label2, *m_label3, *m_label4, *m_label5, *m_label6, *m_label7;
	wxSpinCtrl *m_rank;
	wxTextCtrl *m_first, *m_last, *m_value, *m_age;
	wxComboBox *m_team;
	wxComboCtrl *m_position;
	wxButton *m_stats, *m_add, *m_cancel;
	CLeagueSettings *m_settings;
	CPlayer *m_player;
	bool m_dirty;
};

