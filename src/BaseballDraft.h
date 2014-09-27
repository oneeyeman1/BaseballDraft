#pragma once
class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
private:
	CDb *m_db;
	wxLongLong_t m_leagueId;
	CLeagueSettings *m_leagueSettings;
};

wxDECLARE_APP( MyApp );
