#pragma once
class CNewLeaguePlayers : public wxPanel
{
public:
	CNewLeaguePlayers(wxWindow *parent, CLeagueSettings *settings);
	~CNewLeaguePlayers(void);
	wxCheckBox &GetAmericanLeague();
	wxCheckBox *GetAmericanTeams(int pos);
	wxCheckBox &GetNationalLeague();
	wxCheckBox *GetNationalTeams(int pos);
	void OnAmericanSelect(wxCommandEvent &event);
	void OnNationalSelect(wxCommandEvent &event);
	void OnTeamSelect(wxCommandEvent &event);
	virtual bool Validate();
private:
	wxCheckBox *m_american, *m_national;
	wxCheckBox *m_americanTeams[15], *m_nationalTeams[15];
};

