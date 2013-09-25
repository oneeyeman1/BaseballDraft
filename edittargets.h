#pragma once
class CEditTargets : public wxDialog
{
public:
	CEditTargets(wxWindow *parent, const CLeagueData &data);
	~CEditTargets(void);
	double GetHitTarget(const wxString &category);
	double GetPitchTarget(const wxString &category);
private:
	wxStaticText *m_label1, *m_label2;
	wxPropertyGrid *m_hits, *m_pitches;
};

