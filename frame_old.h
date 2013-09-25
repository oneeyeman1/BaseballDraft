#pragma once
enum
{
	wxMENU_FILE_ADD_PLAYER,
	wxMENU_EDIT_LEAGUE_SETTINGS,
	wxMENU_EDIT_TARGETS,
	wxMENU_EDIT_CLEARALL
};

class CFrame :	public wxFrame
{
public:
	CFrame(const wxString &title, CLeagueSettings *league, const CDb &db, const wxString &name);
	~CFrame(void);
	virtual bool Show(bool show = true);
	void OnAddNewPlayer(wxCommandEvent &event);
	void OnHittersEdit(wxGridEvent &event);
	void OnPitchersEdit(wxGridEvent &event);
	void OnPlayerNameChange(wxCommandEvent &event);
	void OnDraftPerformed(wxCommandEvent &event);
	void OnHideShowteamRoster(wxCommandEvent &event);
	DECLARE_EVENT_TABLE()
protected:
	void LoadLeagueData(CLeagueSettings *league, const CDb &db, CLeagueData &data, const wxString &name);
private:
	bool m_rosterDisplayed;
	wxBoxSizer *m_teamRoster;
	wxFlexGridSizer *m_teamRosterBottom;
	CDb *m_db;
	CLeagueData *m_data;
	wxPanel *m_panel;
	wxComboBox *m_teams, *m_position, *m_team;
	wxStaticText *m_label1, *m_label2, *m_label3, *m_label4, *m_label5, *m_label6, *m_label7, *m_label8, *m_label9, *m_label10, *m_label11, *m_label12, *m_label13;
	wxStaticText *m_salaryLeft, *m_playerLeft, *m_average, *m_inflation, *m_profit, *m_maxBid;
	CMyGrid *m_hittersDrafted, *m_pitchersDrafted, *m_playersDrafted;
	wxButton *m_hideRoster, *m_draft, *m_playersRoster, *m_roster, *m_teamProjection, *m_draftResults;
	wxTextCtrl *m_player;
	wxSpinCtrl *m_amountPaid;
};

