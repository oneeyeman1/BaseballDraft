#pragma once
class Projections
{
public:
	Projections(wxString team)
	{
		m_team = team;
		m_value = 0;
	}
	wxString &GetOwner() { return m_team; };
	bool operator<(const Projections &projection) const { return m_value < projection.m_value; };
	Projections operator+(const double &projection)
	{
		this->m_value += projection;
		return *this;
	};
private:
	wxString m_team;
	double m_value;
};

class CTeamProjections : public wxPanel
{
public:
	CTeamProjections(wxWindow *parent, const CLeagueSettings &settings, const std::vector<CPlayer> &draftResult);
	~CTeamProjections(void);
	void AddPlayer(const wxString &owner, const CPlayer &player);
	void UnAssignPlayer(const CPlayer &player);
	void Resize();
	void ResetProjections();
protected:
	void ChangeProjections(const wxString &owner, const CPlayer &player);
private:
	std::map<wxString,std::vector<Projections>> m_score;
	std::vector<Projections> m_projs[53];
	wxGrid *m_stats, *m_projections;
	wxToggleButton *m_targets;
};

