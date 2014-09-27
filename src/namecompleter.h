#pragma once
class CNameCompleter : public wxTextCompleterSimple
{
public:
	CNameCompleter(const std::vector<CPlayer> *players, bool strict);
	~CNameCompleter(void);
	virtual void GetCompletions(const wxString& prefix, wxArrayString& res);
	std::vector<CPlayer> &GetPlayers();
	void SetFullPositionFilter(const wxString &filter);
	void SetPositionFilter(const int &filter);
	void SetGeneralFilter(int filter);
	void SetPlayers(const std::vector<CPlayer> &players);
private:
	bool m_strict;
	std::vector<CPlayer> m_players;
	wxString m_positionFilterFull;
	int m_generalFilter, m_positionFilter;
};

