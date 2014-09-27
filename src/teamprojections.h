#pragma once
typedef std::pair<wxString, double> SDPair;
class Comparator
{
public:
	bool operator()(const SDPair &lhs, const SDPair &rhs) const
	{
		if( lhs.second < rhs.second )
			return true;
		else if( lhs.second > rhs.second )
			return false;
		else
			return lhs.first < rhs.first;
	}
};

typedef std::set<SDPair, Comparator> SDSetPair;
typedef std::map<wxString, SDSetPair> MapStringToPair;

class CTeamProjections : public wxPanel
{
public:
	CTeamProjections(wxWindow *parent, const CLeagueSettings &settings, const std::vector<CPlayer> &draftResult);
	~CTeamProjections(void);
	void AddPlayer(const wxString &owner, const CPlayer &player);
	void UnAssignPlayer(const CPlayer &player);
	void Resize();
	void ResetProjections();
	void ChangeGrids(const std::vector<wxString> &hitterScoring, const std::vector<wxString> &pitcherScoring);
protected:
	void ChangeProjections(const wxString &owner, const CPlayer &player);
private:
	MapStringToPair m_score;
	SDSetPair m_projs[53];
	wxGrid *m_stats, *m_projections;
	wxToggleButton *m_targets;
};

