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
	int GetLeagueSettings(CLeagueSettings &settings, int &leagueId);
	bool CreateLeague(const CLeagueSettings &league, bool isUpdate, const wxString oldLeague, int &leagueId);
	int InsertPlayers(const std::vector<std::vector<wxString>> &hitters, const std::vector<std::vector<wxString>> &pitchers);
	int LoadLeagueData(CLeagueSettings *league, int &leagueId);
	int GetPlayersForLeague(CLeagueData &m_data, const CLeagueSettings &settings, const int &leagueId, int &numPlayers, std::vector<CPlayer> &draftResult);
	int GetSelectedTeams(const CLeagueSettings &settings, std::map<wxString,wxString> &teams);
	int GetPositionCount(wxString leagueid, std::map<wxString,int> &positions);
	int GetLeagueScoring(wxString league, std::vector<wxString> &hitters, std::vector<wxString> &pitchers);
	int AddNewPlayer(CPlayer &player, const int leagueId, bool skipCheck);
	int AddPlayerToLeague(const CPlayer &player, const int &leagueId);
	bool UpdatePlayer(const CPlayer &player, const int leagueId);
	bool DeleteAllDraft(const int leagueId);
	int RemoveNewPlayers(const int leagueId);
protected:
	bool IsPlayerExist(CPlayer &player);
	bool IsPlayerInLeague(const CPlayer &player, int leagueId);
	int GetLeagueId(bool isUpdate, const wxString &name);
	int InsertUpdateOwners(const std::vector<std::string> &owners, int leagueId);
	int InsertUpdateTeams(const std::vector<int> &americans, const std::vector<int> &nationals, int leagueId);
	int InsertUpdatePositions(const std::map<wxString,int> &positions, int leagueId);
	int InsertUpdatePlayersPosition(int leagueId);
	int InsertUpdateHitScoring(const std::vector<wxString> &hits, const std::vector<wxString> &pitch, int leagueId);
	int InsertUpdatePlayers(const std::vector<int> &americans, const std::vector<int> &nationals, int leagueid);
private:
	sqlite3 *m_handle;
	sqlite3_stmt *stmt, *stmt1;
	bool m_connected;
};

