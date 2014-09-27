#pragma once
class CPlayerDraft : public wxPanel
{
public:
	CPlayerDraft(wxWindow *parent, const CLeagueData &settings);
	~CPlayerDraft(void);
	void OnPlayerNameChange(wxCommandEvent &event);
	void OnContextMenu(wxGridEvent &event);
	void OnAutoResizing(wxGridSizeEvent &event);
	void OnNameSetFocus(wxFocusEvent &event);
	void OnNameKillFocus(wxFocusEvent &event);
	void OnColumnResized(wxGridSizeEvent &event);
	void GetDraftedPlayer(wxString &name, wxString &position, wxString &owner, int &value);
	void DraftCompleted();
	void UpdatePlayer(const CPlayer &player);
	void AddPlayer(CPlayer player);
	wxButton &GetDraftButton();
	wxTextCtrl &GetPlayerNameCtrl();
	wxComboBox &GetPositionCtrl();
	wxComboBox &GetOwnerCtrl();
	wxSpinCtrl &GetAmountPaidCtrl();
	wxString &GetTeam();
	int GetAmountPaid();
	CNameCompleter &GetCompleter();
protected:
	bool IsCIEligible(const std::vector<wxString> &positions);
	bool IsMIEligible(const std::vector<wxString> &positions);
private:
	std::map<wxString,int> m_gridWidths;
	CNameCompleter *m_completer;
	bool m_isResizing;
	wxBoxSizer *main, *vertical, *m_draftedPlayer, *grid;
	CLeagueData *m_data;
	wxStaticText *m_label1, *m_label2, *m_label3, *m_label4;
	wxButton *m_draft;
	CTextCtrl *m_player;
	wxComboBox *m_owner, *m_position;
	wxSpinCtrl *m_amount;
	wxGrid *m_playerInfo;
};

