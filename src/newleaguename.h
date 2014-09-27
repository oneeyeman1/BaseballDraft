#pragma once
class CNewLeagueName :	public wxPanel
{
public:
	CNewLeagueName(wxWindow *parent, const CDb &db, CLeagueSettings *settings, const std::vector<std::string> &leagues, bool isUpdate);
	~CNewLeagueName(void);
	virtual bool Validate();
//	virtual bool Show(bool show = true);
	void OnDraftChange(wxCommandEvent &event);
	wxString GetName();
	wxTextCtrl *GetNameCtrl();
	wxSpinCtrl *GetOwnersNumberCtrl();
	int GetNumberOfOwners();
	int GetSalary();
	int GetDraftType();
	int GetScoringType();
	int GetRoundType();
	bool IsNameEmpty();
private:
	std::vector<std::string> m_leagues;
	wxStaticText *m_label1, *m_label2, *m_label3;
	bool m_isUpdate;
	wxString *m_strName;
	wxTextCtrl *m_name;
	wxSpinCtrl *m_owners, *m_salary;
	wxRadioBox *m_draftType, *m_scoringtype, *m_round;
	CDb *m_db;
};
