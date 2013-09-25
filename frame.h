#pragma once
enum
{
	wxMENU_FILE_ADD_PLAYER,
	wxMENU_FILE_RESET_ALL_PLAYERS,
	wxMENU_FILE_CLEARALL,
	wxMENU_EDIT_LEAGUE_SETTINGS,
	wxMENU_EDIT_TARGETS,
	wxMENU_EDIT_PLAYER,
	wxMENU_DRAFT_PLAYER,
	wxMENU_UNASSIGN_PLAYER
};

class CFrame: public wxFrame
{
public:
    CFrame(const wxString &title, CLeagueSettings *league, const CDb &db, const wxString &name, const int leagueId);
	~CFrame(void);
	void OnContextMenu(wxGridEvent &event);
	void OnAddNewPlayer(wxCommandEvent &event);
	void OnClose(wxCloseEvent &event);
	void OnHittersEdit(wxGridEvent &event);
	void OnPitchersEdit(wxGridEvent &event);
	void OnDraftPerformed(wxCommandEvent &event);
	void OnHideShowteamRoster(wxCommandEvent &event);
	void OnRosterGridCellChange(wxGridEvent &event);
	void OnSelectingOwner(wxCommandEvent &event);
	void OnPrepareDraft(wxCommandEvent &event);
	void OnExit(wxCommandEvent &event);
//	void OnPlayerNameAccepted(wxFocusEvent &event);
	void OnDisplayPlayersData(wxCommandEvent &event);
	void OnDisplayRoster(wxCommandEvent &event);
	void OnDisplayProjections(wxCommandEvent &event);
	void OnClearAllPlayers(wxCommandEvent &event);
	void OnDraftResult(wxCommandEvent &event);
	void OnEditTargets(wxCommandEvent &event);
	void OnUnAssignPlayer(wxCommandEvent &event);
	void OnEditColumns(wxCommandEvent &event);
	std::vector<CPlayer> &GetDraftResult();
	void SetContextMenuPlayer(CPlayer &player);
	bool IsGood();
	DECLARE_EVENT_TABLE()
private:
	int m_budget, m_leagueId;
	std::map<wxString,int> m_positions;
	std::map<wxString, bool> m_columnsDisplayedHitters;
	std::map<wxString, bool> m_columnsDisplayedPitchers;
	std::map<wxString, bool> m_columnsDisplayed;
	std::vector<wxString> m_leagueScoringHitters, m_leagueScoringPitchers;
	std::vector<CPlayer> m_draftResult;
	bool m_rosterDisplayed, m_isGood, m_editPlayerTipDisplayed;
	wxBoxSizer *m_teamRoster, *m_hitters_pitchersRoster, *m_draftedPlayer;
	wxBoxSizer *m_panelSizer;
	wxFlexGridSizer *m_teamRosterBottom;
	CNameCompleter *m_completer;
	int m_availablePlayers, m_totalSpent, m_draftedBeginValue, m_valueLeft;
	wxString m_oldOwner;
	CDb *m_db;
	CLeagueData *m_data;
	CPlayersPanel *m_panel1;
	CTeamsRoster *m_panel2;
	CTeamProjections *m_panel3;
	CDraftResult *m_panel4;
	CPlayer *contextMenuPlayer;
	bool m_dirty;
    void set_properties();
    void do_layout();
	int CalculateHitters();
	int CalculatePitchers();

protected:
	int LoadLeagueData(CLeagueSettings *league, const CDb &db, CLeagueData &data, const wxString &name, const int leagueId, int &numPlayers, std::vector<CPlayer> &draftResult);
	void DoDraftPlayer(const CPlayer &player, const wxString &owner, const wxString &name);
	void DisplayDraftedPlayer(CMyGrid *grid, CPlayer *player, int row);
	void DisplayOwnerParameters(int hitters, int pitchers);
	void DoUnAssignPlayer(bool doErase);
	void DoPerformResetLeague(const int &league);
	void DoEditPlayerFromZero(const wxString name, int droppedValue, int changedValue, CPlayer *player, int style);
    wxStaticText* m_label1;
    wxComboBox* m_teams;
    wxButton* m_hideRoster;
    wxStaticText* m_label2;
    CMyGrid* m_hittersDrafted;
    wxStaticText* m_label3;
    CMyGrid* m_pitchersDrafted;
    wxStaticText* m_label4;
    wxStaticText* m_salaryLeft;
    wxStaticText* m_label7;
    wxStaticText* m_inflation;
    wxStaticText* m_label5;
    wxStaticText* m_playersLeft;
    wxStaticText* m_label8;
    wxStaticText* m_profit;
    wxStaticText* m_label6;
    wxStaticText* m_average;
    wxStaticText* m_label9;
    wxStaticText* m_maxBid;
	CPlayerDraft *m_draftPlayer;
	wxButton *m_playersData, *m_rostersData, *m_teamProjectionsData, *m_draftResultsData;
    wxPanel *m_panel;
};

wxDECLARE_EVENT(EVT_ASSIGN_DRAFTED_PLAYERS, wxCommandEvent);