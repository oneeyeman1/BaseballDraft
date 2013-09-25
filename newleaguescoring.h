#pragma once
class CNewLeagueScoring : public wxPanel
{
public:
	CNewLeagueScoring(wxWindow *parent, CLeagueSettings *settings, const std::map<wxString,bool> *displayedHitters, const std::map<wxString,bool> *displayedPitchers);
	~CNewLeagueScoring(void);
	virtual bool Validate();
	void OnCategoryClicked(wxCommandEvent &event);
	std::map<wxString,bool> &GetHitterScoring();
	std::map<wxString,bool> &GetPitcherScoring();
	wxCheckBox *GetHitScore(int pos);
	wxCheckBox *GetPitchScore(int pos);
private:
	wxCheckBox *m_hitScoring[26], *m_pitchScoring[28];
	std::map<wxString,bool> *m_hitters, *m_pitchers;
};

