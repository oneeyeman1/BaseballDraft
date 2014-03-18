#pragma once
class CDb
{
public:
	CDb(void);
	~CDb(void);
	void OpenConnection(const wxString &dbName);
	bool IsConnected();
	void StartTransaction();
	void FinishTransaction(bool result);
	void GetLeagueList(std::vector<std::string> &leagues);
	int DeleteLeague(const wxString &league, bool isUpdate);
	int GetLeagueSettings(CLeagueSettings &settings, sqlite3_int64 &leagueId);
	bool CreateLeague(CLeagueSettings &league, bool isUpdate, const wxString oldLeague, sqlite3_int64 &leagueId);
	int InsertPlayers(const std::vector<std::vector<wxString> > &hitters, const std::vector<std::vector<wxString> > &pitchers);
	int LoadLeagueData(CLeagueSettings *league, sqlite3_int64 &leagueId);
	int GetPlayersForLeague(CLeagueData &m_data, const int &leagueId, int &numPlayers, std::vector<CPlayer> &draftResult, int &zeroRanked);
	int GetPositionCount(wxString leagueid, std::map<wxString,int> &positions);
	int GetLeagueScoring(wxString league, std::vector<wxString> &hitters, std::vector<wxString> &pitchers);
	int AddNewPlayer(CPlayer &player, const sqlite3_int64 leagueId, bool skipCheck, wxString &errorMessage);
	bool UpdatePlayer(const CPlayer &player, const sqlite3_int64 leagueId, wxString &errorMessage);
	bool DeleteAllDraft(const sqlite3_int64 leagueId);
	int RemoveNewPlayers(const int leagueId);
protected:
	bool IsPlayerExist(CPlayer &player, bool &errorCheck, wxString &errorMessage);
	bool IsPlayerInLeague(const CPlayer &player, sqlite3_int64 leagueId, bool &errorCheck, wxString &errorMessage);
	sqlite3_int64 GetLeagueId(bool isUpdate, const wxString &name);
	int InsertUpdateOwners(const std::vector<std::string> &owners, sqlite3_int64 leagueId);
	int InsertUpdateTeams(CLeagueSettings &league, sqlite3_int64 leagueId);
	int InsertUpdatePositions(const std::map<wxString,int> &positions, sqlite3_int64 leagueId);
	int InsertUpdatePlayersPosition(sqlite3_int64 leagueId);
	int InsertUpdateHitScoring(const std::vector<wxString> &hits, const std::vector<wxString> &pitch, sqlite3_int64 leagueId);
	int InsertUpdatePlayers(const std::vector<int> &americans, const std::vector<int> &nationals, sqlite3_int64 leagueid);
private:
	sqlite3 *m_handle;
	sqlite3_stmt *stmt, *stmt1;
	bool m_connected;
};

