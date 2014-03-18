#pragma once

#define SORT_BY_RANGE			  0
#define SORT_BY_NAME		      1
#define SORT_BY_TEAM		      2
#define SORT_BY_POSITION	      3
#define SORT_BY_AGE               4
#define SORT_BY_VALUE             5
#define SORT_BY_CURRVALUE         6
#define SORT_BY_AMTPAID           7
#define SORT_BY_OWNER             8
#define SORT_BY_NOTES             9
#define SORT_BY_KHITTERS         10
#define SORT_BY_KPITCHERS        11
#define SORT_BY_HHITTERS         12
#define SORT_BY_HPITCHERS        13
#define SORT_BY_RUNSHITTERS      14
#define SORT_BY_RPITCHERS        15
#define SORT_BY_HRHITTERS        16
#define SORT_BY_HRPITCHERS       17
#define SORT_BY_BBHITTERS        18
#define SORT_BY_BBPITCHERS       19
#define SORT_BY_AVGHITTERS       20
#define SORT_BY_OBPHITTERS       21
#define SORT_BY_SLGHITTERS       22
#define SORT_BY_OPSHITTERS       23
#define SORT_BY_ABHITTERS        24
#define SORT_BY_1BHITTERS        25
#define SORT_BY_2BHITTERS        26
#define SORT_BY_3BHITTERS        27
#define SORT_BY_TBHITTERS        28
#define SORT_BY_RBIHITTERS       29
#define SORT_BY_SBHITTERS        30
#define SORT_BY_2B3BHRHITTERS    31
#define SORT_BY_2B3BHITTERS      32
#define SORT_BY_SBCSHITTERS      33
#define SORT_BY_CSHITTERS        34
#define SORT_BY_RRBIHRHITTERS    35
#define SORT_BY_HBBHITTERS       36
#define SORT_BY_TBBBHITTERS      37
#define SORT_BY_HBPHITTERS       38
#define SORT_BY_GDPHITTERS       39
#define SORT_BY_SFHITTERS        40
#define SORT_BY_WPITCHERS        41
#define SORT_BY_SPITCHERS        42
#define SORT_BY_ERAPITCHERS      43
#define SORT_BY_WHIPPITCHERS     44
#define SORT_BY_IPPITCHERS       45
#define SORT_BY_LPITCHERS        46
#define SORT_BY_ERPITCHERS       47
#define SORT_BY_CGPITCHERS       48
#define SORT_BY_WLPITCHERS       49
#define SORT_BY_QSPITCHERS       50
#define SORT_BY_SHUTOUTPITCHERS  51
#define SORT_BY_BSPITCHERS       52
#define SORT_BY_SBSPITCHERS      53
#define SORT_BY_SHOLDSPITCHERS   54
#define SORT_BY_SHOLDSBSPITCHERS 55
#define SORT_BY_K9PITCHERS       56
#define SORT_BY_H9PITCHERS       57
#define SORT_BY_BB9PITCHERS      58
#define SORT_BY_KBBPITCHERS      59
#define SORT_BY_KMINBBPITCHERS   60
#define SORT_BY_W100PITCHERS     61
#define SORT_BY_HOLDSPITCHERS    62
#define SORT_BY_GPITCHERS        63
#define SORT_BY_PROFIT			 64
#define SORT_BY_DRAFTPOSITION	 65

#define  BASEBALLDRAFT_C_FILTER  0x01
#define BASEBALLDRAFT_1B_FILTER  0x02
#define BASEBALLDRAFT_2B_FILTER  0x04
#define BASEBALLDRAFT_SS_FILTER  0x08
#define BASEBALLDRAFT_3B_FILTER  0x10
#define BASEBALLDRAFT_OF_FILTER  0x20
#define BASEBALLDRAFT_DH_FILTER  0x40
#define BASEBALLDRAFT_CI_FILTER  0x80
#define BASEBALLDRAFT_MI_FILTER 0x100
#define  BASEBALLDRAFT_P_FILTER 0x200
#define BASEBALLDRAFT_SP_FILTER 0x400
#define BASEBALLDRAFT_RP_FILTER 0x800

class CLeagueSettings
{
public:
	CLeagueSettings(const wxString &leagueTitle);
	CLeagueSettings(const wxString name, int owners, int draftType, int scoringType, int roundType, int salary, bool benchIncluded);
	~CLeagueSettings();
	void SetLeagueType(bool type);
	bool GetLeagueType() const;
	void SetLeagueName(const wxString &name);
	wxString &GetLeagueName() const;
	void AddPosition(wxString pos, int value);
	void SetPositions(const std::map<wxString,int> &positions);
	std::map<wxString,int> &GetPositions() const;
	void SetDraftType(int type);
	int GetDraftType() const;
	void SetRoundingType(int type);
	int GetRoundingtype() const;
	void SetSalary(int salary);
	int GetSalary() const;
	void SetScoringtype(int type);
	int GetScoringType() const;
	void AddOwner(const wxString &owner);
	void SetOwnersNumber(int num);
	unsigned int GetOwnersNumber() const { return m_ownersNum; }
	void SetOwners(const std::vector<std::string> &owners) { m_owners = const_cast<std::vector<std::string> &>( owners ); };
	std::vector<std::string> &GetOwners() const;
	void SetAmericanLeague(const std::vector<int> &teams) { m_americanleague = const_cast<std::vector<int> &>( teams ); };
	void AddAmericanLeague(int team);
	std::vector<int> &GetAmericanLeague() const;
	void SetNationaLeague(const std::vector<int> &teams) { m_nationalleague = const_cast<std::vector<int> &>( teams ); };
	void AddNationalLeague(int team);
	std::vector<int> &GetNationalLeague() const;
	void SetHitScoring(const std::vector<wxString> &scoring) { m_hitsScoringName = const_cast<std::vector<wxString> &>( scoring ); };
	void AddHitScoring(wxString score);
	std::vector<wxString> &GetHitScoring() const;
	void SetPitchScoring(const std::vector<wxString> &scoring) { m_pitchScoringName = const_cast<std::vector<wxString> &>( scoring ); };
	void AddPitchScoring(wxString score);
	std::vector<wxString> &GetPitchScoring() const;
	bool IsBenchPlayerIncluded() const;
	void SetBenchPlayerIncluded(bool benchIncluded);
	void AddTargetHitter(const wxString &score, double value);
	std::map<wxString,double> &GetTargetsForHitters() const;
	void AddTargetPitchers(const wxString &score, double value);
	std::map<wxString,double> &GetTargetsForPitchers() const;
	void AddTeamAbbreviation(const wxString &name, const wxString &abbreviatedName) { m_teams[abbreviatedName] = name; };
	std::map<wxString,wxString> &GetTeamsName() const;
private:
	wxString m_name;
	bool m_type, m_benchPlayerIncluded;
	std::vector<std::string> m_owners;
	std::map<wxString,int> m_positions;
	std::map<wxString,double> m_targetHits, m_targetPitches;
	int m_ownersNum, m_draftType, m_scoringType, m_roundingType/*, m_positions[14]*/, m_salary;
	std::vector<int> m_americanleague, m_nationalleague;
	std::vector<wxString> m_hitsScoringName, m_pitchScoringName;
	std::map<wxString,wxString> m_teams;
};

class CPlayer
{
public:
	CPlayer()
	{
	}

	CPlayer(const long &id, const wxString &name, const std::vector<wxString> &position, int age, double value, const wxString &team, const wxString &teamAbbreviated, const std::map<wxString,double> &scoring, bool hitter, double currValue, const wxString &notes, double baseValue, bool deleted)
	{
		m_currentRange = 0;
		m_originalRange = 0;
		m_draftOrder = 0;
		m_id = id;
		m_name = name;
		m_positions = position;
		m_age = age;
		m_value = value;
		m_currValue = currValue;
		m_team = team;
		m_teamAbbreviated = teamAbbreviated;
		m_score = scoring;
		m_isHitter = hitter;
		m_isDrafted = false;
		m_notes = notes;
		m_amountPaid = 0;
		m_draftedPosition = m_owner = wxEmptyString;
		m_isNew = false;
		m_baseValue = baseValue;
		m_isDeleted = deleted;
	}
	CPlayer(const long &id, bool isHitter, int age, double value, bool deleted)
	{
		m_id = id;
		m_isHitter = isHitter;
		m_age = age;
		m_value = value;
		m_isDeleted = deleted;
	}
	inline bool operator<(const CPlayer &player) const { return m_name < player.m_name; };
	inline long GetPlayerId() const { return m_id; };
	inline void SetNewPlayerId(const long id) { m_id = id; };
	inline wxString &GetName() { return m_name; };
	inline double GetCurrentValue() const { return m_currValue; };
	inline void SetCurrentValue(double value) { m_currValue = value; };
	inline void SetTeam(const wxString &team) { m_team = team; };
	inline wxString &GetTeam() { return m_team; };
	inline void SetAbbreviatedTeam(const wxString team) { m_teamAbbreviated = team; };
	inline wxString &GetAbbeviatedTeamName() { return m_teamAbbreviated; };
	inline std::vector<wxString> &GetPositions() { return m_positions; };
	inline std::map<wxString,double> &GetScoring() { return m_score; };
	inline double GetValue() const { return m_value; };
	inline void SetValue(double value) { m_value = value; };
	inline void SetAge(int age) { m_age = age; };
	inline void SetPosition(std::vector<wxString> &position) { m_positions = position; };
	inline void ClearPositions() { m_positions.clear(); };
	inline void SetScore(const std::map<wxString,double> &score) { m_score = score; };
	inline int GetAge() const { return m_age; };
	inline wxString &GetNotes() { return m_notes; };
	inline void SetNotes(const wxString &notes) { m_notes = notes; };
	inline bool IsPlayerDeleted() const { return m_isDeleted; };
	inline void DeletePlayer(bool deleted) { m_isDeleted = deleted; };
	inline bool IsHitter() const { return m_isHitter; };
	inline bool IsNewPlayer() const { return m_isNew; };
	inline void SetNewPlayer(bool isNew) { m_isNew = isNew; };
	inline void DraftPlayer(bool draft) { m_isDrafted = draft; };
	inline bool IsPlayerDrafted() const { return m_isDrafted; };
	inline int GetAmountPaid() const {return m_amountPaid; };
	inline void SetAmountPaid(int amount) { m_amountPaid = amount; };
	inline wxString &GetDraftedPosition() { return m_draftedPosition; };
	inline void SetDraftedPosition(const wxString &position) { m_draftedPosition = position; };
	inline wxString &GetOwner() { return m_owner; };
	inline void SetOwner(const wxString &owner) { m_owner = owner; };
	inline int GetDraftOrder() const { return m_draftOrder; };
	inline void SetDraftOrder(int order) { m_draftOrder = order; };
	inline void SetRange(int range) { m_currentRange = range; };
	inline int GetRange() { return m_currentRange; };
	inline void SetOriginalRange(int range) { m_originalRange = range; };
	inline int GetOriginalRange() { return m_originalRange; };
	inline void SetBaseValue(double value) { m_baseValue = value; };
	inline double GetBaseValue() { return m_baseValue; };
	inline void SetOriginalScore(const std::map<wxString,double> &originalScore)  { m_originalScore = originalScore; };
	inline std::map<wxString,double> &GetOriginalScore() { return m_originalScore; };
	inline void SetOriginalPositions(const std::vector<wxString> &pos) { m_originalPositions = pos; };
	std::vector<wxString> &GetOriginalPositions() { return m_originalPositions; };
private:
	wxString m_name, m_team, m_teamAbbreviated, m_notes, m_draftedPosition, m_owner;
	int m_age, m_amountPaid, m_draftOrder, m_currentRange, m_originalRange;
	long m_id;
	double m_currValue, m_baseValue, m_value;
	bool m_isHitter, m_isDrafted, m_isNew, m_isDeleted;
	std::map<wxString, double> m_score, m_originalScore;
	std::vector<wxString> m_positions, m_originalPositions;
};

struct SortObject
{
	int m_type;
	bool m_direction;

	SortObject(int type, bool direction)
	{
		m_type = type;
		m_direction = direction;
	}
};

// bool sortPlayersByValue(const CPlayer &player1, const CPlayer &player2);
struct PlayerSorter
{
	bool operator()(const CPlayer &player1, const CPlayer &player2)
	{
		for( std::vector<SortObject>::iterator it = m_type.begin(); it < m_type.end(); it++ )
		{
			int rank1 = const_cast<CPlayer &>( player1 ).GetRange(), rank2 = const_cast<CPlayer &>( player2 ).GetRange();
			switch( (*it).m_type )
			{
				case SORT_BY_RANGE:
				{
					if( (*it).m_direction )
					{
						if( rank1 < rank2 )
							return true;
						else if( rank1 > rank2 )
							return false;
					}
					else
					{
						if( rank1 > rank2 )
							return true;
						else if( rank1 < rank2 )
							return false;
					}
				}
				break;
				case SORT_BY_NAME:
				{
					wxString name1 = const_cast<CPlayer &>( player1 ).GetName(), name2 = const_cast<CPlayer &>( player2 ).GetName();
					if( (*it).m_direction )
					{
						if( name1 < name2 )
							return true;
						else if( name1 > name2 )
							return false;
					}
					else
					{
						if( name1 > name2 )
							return true;
						else if( name1 < name2 )
							return true;
					}
				}
				break;
				case SORT_BY_TEAM:
				{
					wxString team1 = const_cast<CPlayer &>( player1 ).GetTeam(), team2 = const_cast<CPlayer &>( player2 ).GetTeam();
					if( (*it).m_direction )
					{
						if( team1 < team2 )
							return true;
						else if( team1 > team2 )
							return false;
					}
					else
					{
						if( team1 > team2 )
							return true;
						else if( team1 < team2 )
							return false;
					}
					break;
				}
				case SORT_BY_POSITION:
				{
					wxString position1, position2;
					std::vector<wxString> pos1 = const_cast<CPlayer &>( player1 ).GetPositions(), pos2 = const_cast<CPlayer &>( player2 ).GetPositions();
					for( std::vector<wxString>::iterator it1 = pos1.begin(); it1 < pos1.end(); it1++ )
						position1 += (*it1);
					for( std::vector<wxString>::iterator it2 = pos2.begin(); it2 < pos2.end(); it2++ )
						position2 += (*it2);
					if( (*it).m_direction )
					{
						if( position1 < position2 )
							return true;
						else if( position1 > position2 )
							return false;
					}
					else
					{
						if( position1 > position2 )
							return true;
						else if( position1 < position2 )
							return false;
					}
					break;
				}
				case SORT_BY_AGE:
				{
					int age1 = player1.GetAge(), age2 = player2.GetAge();
					if( (*it).m_direction )
					{
						if( age1 > age2 )
							return true;
						else if( age1 < age2 )
							return false;
					}
					else
					{
						if( age1 < age2 )
							return true;
						else if( age1 > age2 )
							return false;
					}
					break;
				}
				case SORT_BY_VALUE:
				{
					double value1 = player1.GetValue(), value2 = player2.GetValue();
					if( (*it).m_direction )
					{
						if( value1 > value2 )
							return true;
						else if( value1 < value2 )
							return false;
					}
					else
					{
						if( value1 < value2 )
							return true;
						else if( value1 > value2 )
							return false;
					}
					break;
				}
				case SORT_BY_CURRVALUE:
				{
					double value1 = player1.GetCurrentValue(), value2 = player2.GetCurrentValue();
					if( (*it).m_direction )
					{
						if( value1 > value2 )
							return true;
						else if( value1 < value2 )
							return false;
					}
					else
					{
						if( value1 < value2 )
							return true;
						else if( value1 > value2 )
							return false;
					}
					break;
				}
				case SORT_BY_AMTPAID:
				{
					int value1 = player1.GetAmountPaid(), value2 = player2.GetAmountPaid();
					if( (*it).m_direction )
					{
						if( value1 > value2 )
							return true;
						else if( value1 < value2 )
							return false;
					}
					else
					{
						if( value1 < value2 )
							return true;
						else if( value1 > value2 )
							return false;
					}
					break;
				}
				case SORT_BY_PROFIT:
				{
					double profit1 = player1.GetValue() - player1.GetAmountPaid(), profit2 = player2.GetValue() - player2.GetAmountPaid();
					if( (*it).m_direction )
					{
						if( profit1 > profit2 )
							return true;
						else if( profit1 < profit2 )
							return false;
					}
					else
					{
						if( profit1 < profit2 )
							return true;
						else if( profit1 > profit2 )
							return false;
					}
					break;
				}
				case SORT_BY_OWNER:
				{
					wxString owner1 = const_cast<CPlayer &>( player1 ).GetOwner(), owner2 = const_cast<CPlayer &>( player2 ).GetOwner();
					if( (*it).m_direction )
					{
						if( owner1 > owner2 )
							return true;
						else if( owner1 < owner2 )
							return false;
					}
					else
					{
						if( owner1 < owner2 )
							return true;
						else if( owner1 > owner2 )
							return false;
					}
					break;
				}
				case SORT_BY_NOTES:
				{
					wxString notes1 = const_cast<CPlayer &>( player1 ).GetNotes(), notes2 = const_cast<CPlayer &>( player2 ).GetNotes();
					if( (*it).m_direction )
					{
						if( notes1 > notes2 )
							return true;
						else if( notes1 < notes2 )
							return false;
					}
					else
					{
						if( notes1 < notes2 )
							return true;
						else if( notes1 > notes2 )
							return false;
					}
					break;
				}
				case SORT_BY_KHITTERS:
				{
					double k1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["K"] : 0, k2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["K"] : 0;
					if( (*it).m_direction )
					{
						if( k1 > k2 )
							return true;
						else if( k1 < k2 )
							return false;
					}
					else
					{
						if( k1 < k2 )
							return true;
						else if( k1 > k2 )
							return false;
					}
					break;
				}
				case SORT_BY_KPITCHERS:
				{
					double k1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["K"] : 0, k2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["K"] : 0;
					if( (*it).m_direction )
					{
						if( k1 > k2 )
							return true;
						else if( k1 < k2 )
							return false;
					}
					else
					{
						if( k1 < k2 )
							return true;
						else if( k1 > k2 )
							return false;
					}
					break;
				}
				case SORT_BY_HHITTERS:
				{
					double h1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["H"] : 0, h2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["H"] : 0;
					if( (*it).m_direction )
					{
						if( h1 > h2 )
							return true;
						else if( h1 < h2 )
							return false;
					}
					else
					{
						if( h1 < h2 )
							return true;
						else if( h1 > h2 )
							return false;
					}
					break;
				}
				case SORT_BY_HPITCHERS:
				{
					double h1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["H"] : 0, h2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["H"] : 0;
					if( (*it).m_direction)
					{
						if( h1 > h2 )
							return true;
						else if( h1 < h2 )
							return false;
					}
					else
					{
						if( h1 < h2 )
							return true;
						else if( h1 > h2 )
							return false;
					}
					break;
				}
				case SORT_BY_RUNSHITTERS:
				{
					double r1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["Runs"] : 0, r2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["Runs"] : 0;
					if( (*it).m_direction )
					{
						if( r1 > r2 )
							return true;
						else if( r1 < r2 )
							return false;
					}
					else
					{
						if( r1 < r2 )
							return true;
						else if( r1 > r2 )
							return false;
					}
					break;
				}
				case SORT_BY_RPITCHERS:
				{
					double r1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["R"] : 0, r2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["R"] : 0;
					if( (*it).m_direction )
					{
						if( r1 > r2 )
							return true;
						else if( r1 < r2 )
							return false;
					}
					else
					{
						if( r1 < r2 )
							return true;
						else if( r1 > r2 )
							return false;
					}
					break;
				}
				case SORT_BY_HRHITTERS:
				{
					double hr1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["HR"] : 0, hr2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["HR"] : 0;
					if( (*it).m_direction )
					{
						if( hr1 > hr2 )
							return true;
						else if( hr1 < hr2 )
							return false;
					}
					else
					{
						if( hr1 < hr2 )
							return true;
						else if( hr1 > hr2 )
							return false;
					}
					break;
				}
				case SORT_BY_HRPITCHERS:
				{
					double hr1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["HR"] : 0, hr2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["HR"] : 0;
					if( (*it).m_direction )
					{
						if( hr1 > hr2 )
							return true;
						else if( hr1 < hr2 )
							return false;
					}
					else
					{
						if( hr1 < hr2 )
							return true;
						else if( hr1 > hr2 )
							return false;
					}
					break;
				}
				case SORT_BY_BBHITTERS:
				{
					double bb1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["BB"] : 0, bb2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["BB"] : 0;
					if( (*it).m_direction )
					{
						if( bb1 > bb2 )
							return true;
						else if( bb1 < bb2 )
							return false;
					}
					else
					{
						if( bb1 < bb2 )
							return true;
						else if( bb1 > bb2 )
							return false;
					}
					break;
				}
				case SORT_BY_BBPITCHERS:
				{
					double bb1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["BB"] : 0, bb2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["BB"] : 0;
					if( (*it).m_direction )
					{
						if( bb1 > bb2 )
							return true;
						else if( bb1 < bb2 )
							return false;
					}
					else
					{
						if( bb1 < bb2 )
							return true;
						else if( bb1 > bb2 )
							return false;
					}
					break;
				}
				case SORT_BY_AVGHITTERS:
				{
					double avg1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["AVG"] : 0, avg2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["AVG"] : 0;
					if( (*it).m_direction )
					{
						if( avg1 > avg2 )
							return true;
						else if( avg1 < avg2 )
							return false;
					}
					else
					{
						if( avg1 < avg2 )
							return true;
						else if( avg1 > avg2 )
							return false;
					}
					break;
				}
				case SORT_BY_OBPHITTERS:
				{
					double obp1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["OBP"] : 0, obp2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["OBP"] : 0;
					if( (*it).m_direction )
					{
						if( obp1 > obp2 )
							return true;
						else if( obp1 < obp2 )
							return false;
					}
					else
					{
						if( obp1 < obp2 )
							return true;
						else if( obp1 > obp2 )
							return false;
					}
					break;
				}
				case SORT_BY_SLGHITTERS:
				{
					double slg1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["SLG"] : 0, slg2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["SLG"] : 0;
					if( (*it).m_direction )
					{
						if( slg1 > slg2 )
							return true;
						else if( slg1 < slg2 )
							return false;
					}
					else
					{
						if( slg1 < slg2 )
							return true;
						else if( slg1 > slg2 )
							return false;
					}
					break;
				}
				case SORT_BY_OPSHITTERS:
				{
					double ops1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["OPS"] : 0, ops2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["OPS"] : 0;
					if( (*it).m_direction )
					{
						if( ops1 > ops2 )
							return  true;
						else if( ops1 < ops2 )
							return false;
					}
					else
					{
						if( ops1 < ops2 )
							return true;
						else if( ops1 > ops2 )
							return false;
					}
					break;
				}
				case SORT_BY_ABHITTERS:
				{
					double ab1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["AB"] : 0, ab2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["AB"] : 0;
					if( (*it).m_direction )
					{
						if( ab1 > ab2 )
							return true;
						else if( ab1 < ab2 )
							return false;
					}
					else
					{
						if( ab1 < ab2 )
							return true;
						else if( ab1 > ab2 )
							return false;
					}
					break;
				}
				case SORT_BY_1BHITTERS:
				{
					double s1b1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["1B"] : 0, s1b2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["1B"] : 0;
					if( (*it).m_direction )
					{
						if( s1b1 > s1b2 )
							return true;
						else if( s1b1 < s1b2 )
							return false;
					}
					else
					{
						if( s1b1 < s1b2 )
							return true;
						else if( s1b1 > s1b2 )
							return false;
					}
					break;
				}
				case SORT_BY_2BHITTERS:
				{
					double s2b1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["2B"] : 0, s2b2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["2B"] : 0;
					if( (*it).m_direction )
					{
						if( s2b1 > s2b2 )
							return true;
						else if( s2b1 < s2b2 )
							return false;
					}
					else
					{
						if( s2b1 < s2b2 )
							return true;
						else if( s2b1 > s2b2 )
							return false;
					}
					break;
				}
				case SORT_BY_3BHITTERS:
				{
					double s3b1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["3B"] : 0, s3b2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["3B"] : 0;
					if( (*it).m_direction )
					{
						if( s3b1 > s3b2 )
							return true;
						else if( s3b1 < s3b2 )
							return false;
					}
					else
					{
						if( s3b1 < s3b2 )
							return true;
						else if( s3b1 > s3b2 )
							return false;
					}
					break;
				}
				case SORT_BY_TBHITTERS:
				{
					double tb1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["TB"] : 0, tb2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["TB"] : 0;
					if( (*it).m_direction )
					{
						if( tb1 > tb2 )
							return true;
						else if( tb1 < tb2 )
							return false;
					}
					else
					{
						if( tb1 < tb2 )
							return true;
						else if( tb1 > tb2 )
							return false;
					}
					break;
				}
				case SORT_BY_RBIHITTERS:
				{
					double rbi1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["RBI"] : 0, rbi2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["RBI"] : 0;
					if( (*it).m_direction )
					{
						if( rbi1 > rbi2 )
							return true;
						else if( rbi1 < rbi2 )
							return false;
					}
					else
					{
						if( rbi1 < rbi2 )
							return true;
						else if( rbi1 > rbi2 )
							return false;
					}
					break;
				}
				case SORT_BY_SBHITTERS:
				{
					double sb1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["SB"] : 0, sb2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["SB"] : 0;
					if( (*it).m_direction )
					{
						if( sb1 > sb2 )
							return true;
						else if( sb1 < sb2 )
							return false;
					}
					else
					{
						if( sb1 < sb2 )
							return true;
						else if( sb1 > sb2 )
							return false;
					}
					break;
				}
				case SORT_BY_2B3BHRHITTERS:
				{
					double s2b3bhr1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["2B+3B+HR"] : 0, s2b3bhr2 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["2B+3B+HR"] : 0;
					if( (*it).m_direction )
					{
						if( s2b3bhr1 > s2b3bhr2 )
							return true;
						else if( s2b3bhr1 < s2b3bhr2 )
							return false;
					}
					else
					{
						if( s2b3bhr1 < s2b3bhr2 )
							return true;
						else if( s2b3bhr1 > s2b3bhr2 )
							return false;
					}
					break;
				}
				case SORT_BY_2B3BHITTERS:
				{
					double s2b3b1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["2B+3B"] : 0, s2b3b2 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["2B+3B"] : 0;
					if( (*it).m_direction )
					{
						if( s2b3b1 > s2b3b2 )
							return true;
						else if( s2b3b1 < s2b3b2 )
							return false;
					}
					else
					{
						if( s2b3b1 < s2b3b2 )
							return true;
						else if( s2b3b1 > s2b3b2 )
							return false;
					}
					break;
				}
				case SORT_BY_SBCSHITTERS:
				{
					double sbcs1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["SB-CS"] : 0, sbcs2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["SB-CS"] : 0;
					if( (*it).m_direction )
					{
						if( sbcs1 > sbcs2 )
							return true;
						else if( sbcs1 < sbcs2 )
							return false;
					}
					else
					{
						if( sbcs1 < sbcs2 )
							return true;
						else if( sbcs1 > sbcs2 )
							return false;
					}
					break;
				}
				case SORT_BY_CSHITTERS:
				{
					double cs1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["CS"] : 0, cs2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["CS"] : 0;
					if( (*it).m_direction )
					{
						if( cs1 > cs2 )
							return true;
						else if( cs1 < cs2 )
							return false;
					}
					else
					{
						if( cs1 < cs2 )
							return true;
						else if( cs1 > cs2 )
							return false;
					}
					break;
				}
				case SORT_BY_RRBIHRHITTERS:
				{
					double rrbihr1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["R+RBI-HR"] : 0, rrbihr2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["R+RBI-HR"] : 0;
					if( (*it).m_direction )
					{
						if( rrbihr1 > rrbihr2 )
							return true;
						else if( rrbihr1 < rrbihr2 )
							return false;
					}
					else
					{
						if( rrbihr1 < rrbihr2 )
							return true;
						else if( rrbihr1 > rrbihr2 )
							return false;
					}
					break;
				}
				case SORT_BY_HBBHITTERS:
				{
					double hbb1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["H+BB"] : 0, hbb2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["H+BB"] : 0;
					if( (*it).m_direction )
					{
						if( hbb1 > hbb2 )
							return true;
						else if( hbb1 < hbb2 )
							return false;
					}
					else
					{
						if( hbb1 < hbb2 )
							return true;
						else if( hbb1 > hbb2 )
							return false;
					}
					break;
				}
				case SORT_BY_TBBBHITTERS:
				{
					double tbbb1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["TB+BB"] : 0, tbbb2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["TB+BB"] : 0;
					if( (*it).m_direction )
					{
						if( tbbb1 > tbbb2 )
							return true;
						else if( tbbb1 < tbbb2 )
							return false;
					}
					else
					{
						if( tbbb1 < tbbb2 )
							return true;
						else if( tbbb1 > tbbb2 )
							return false;
					}
					break;
				}
				case SORT_BY_HBPHITTERS:
				{
					double hbp1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["HBP"] : 0, hbp2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["HBP"] : 0;
					if( (*it).m_direction )
					{
						if( hbp1 > hbp2 )
							return true;
						else if( hbp1 < hbp2 )
							return false;
					}
					else
					{
						if( hbp1 < hbp2 )
							return true;
						else if( hbp1 > hbp2 )
							return false;
					}
					break;
				}
				case SORT_BY_GDPHITTERS:
				{
					double gdp1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["GDP"] : 0, gdp2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["GDP"] : 0;
					if( (*it).m_direction )
					{
						if( gdp1 > gdp2 )
							return true;
						else if( gdp1 < gdp2 )
							return false;
					}
					else
					{
						if( gdp1 < gdp2 )
							return true;
						else if( gdp1 > gdp2 )
							return false;
					}
					break;
				}
				case SORT_BY_SFHITTERS:
				{
					double sf1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["SF"] : 0, sf2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["SF"] : 0;
					if( (*it).m_direction )
					{
						if( sf1 > sf2 )
							return true;
						else if( sf1 < sf2 )
							return false;
					}
					else
					{
						if( sf1 < sf2 )
							return true;
						else if( sf1 > sf2 )
							return false;
					}
					break;
				}
				case SORT_BY_WPITCHERS:
				{
					double wins1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["Wins"] : 0, wins2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["Wins"] : 0;
					if( (*it).m_direction )
					{
						if( wins1 > wins2 )
							return true;
						else if( wins1 < wins2 )
							return false;
					}
					else
					{
						if( wins1 < wins2 )
							return true;
						else if( wins1 > wins2 )
							return false;
					}
					break;
				}
				case SORT_BY_SPITCHERS:
				{
					double saves1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["Saves"] : 0, saves2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["Saves"] : 0;
					if( (*it).m_direction )
					{
						if( saves1 > saves2 )
							return true;
						else if( saves1 < saves2 )
							return false;
					}
					else
					{
						if( saves1 < saves2 )
							return true;
						else if( saves1 > saves2 )
							return false;
					}
					break;
				}
				case SORT_BY_ERAPITCHERS:
				{
					double era1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["ERA"] : 0, era2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["ERA"] : 0;
					if( (*it).m_direction )
					{
						if( era1 > era2 )
							return true;
						else if( era1 < era2 )
							return false;
					}
					else
					{
						if( era1 < era2 )
							return true;
						else if( era1 > era2 )
							return false;
					}
					break;
				}
				case SORT_BY_WHIPPITCHERS:
				{
					double whip1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["WHIP"] : 0, whip2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["WHIP"] : 0;
					if( (*it).m_direction )
					{
						if( whip1 > whip2 )
							return true;
						else if( whip1 < whip2 )
							return false;
					}
					else
					{
						if( whip1 < whip2 )
							return true;
						else if( whip1 > whip2 )
							return false;
					}
					break;
				}
				case SORT_BY_IPPITCHERS:
				{
					double ip1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["IP"] : 0, ip2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["IP"] : 0;
					if( (*it).m_direction )
					{
						if( ip1 > ip2 )
							return true;
						else if( ip1 < ip2 )
							return false;
					}
					else
					{
						if( ip1 < ip2 )
							return true;
						else if( ip1 > ip2 )
							return false;
					}
					break;
				}
				case SORT_BY_LPITCHERS:
				{
					double l1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["L"] : 0, l2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["L"] : 0;
					if( (*it).m_direction )
					{
						if( l1 > l2 )
							return true;
						else if( l1 < l2 )
							return false;
					}
					else
					{
						if( l1 < l2 )
							return true;
						else if( l1 > l2 )
							return false;
					}
					break;
				}
				case SORT_BY_ERPITCHERS:
				{
					double er1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["ER"] : 0, er2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["ER"] : 0;
					if( (*it).m_direction )
					{
						if( er1 > er2 )
							return true;
						else if( er1 < er2 )
							return false;
					}
					else
					{
						if( er1 < er2 )
							return true;
						else if( er1 > er1 )
							return false;
					}
					break;
				}
				case SORT_BY_CGPITCHERS:
				{
					double cg1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["CG"] : 0, cg2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["CG"] : 0;
					if( (*it).m_direction )
					{
						if( cg1 > cg2 )
							return true;
						else if( cg1 < cg2 )
							return false;
					}
					else
					{
						if( cg1 < cg2 )
							return true;
						else if( cg1 > cg2 )
							return false;
					}
					break;
				}
				case SORT_BY_WLPITCHERS:
				{
					double wl1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["W-L"] : 0, wl2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["W-L"] : 0;
					if( (*it).m_direction )
					{
						if( wl1 > wl2 )
							return true;
						else if( wl1 < wl2 )
							return false;
					}
					else
					{
						if( wl1 < wl2 )
							return true;
						else if( wl1 > wl2 )
							return false;
					}
					break;
				}
				case SORT_BY_QSPITCHERS:
				{
					double qs1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["QS"] : 0, qs2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["QS"] : 0;
					if( (*it).m_direction )
					{
						if( qs1 > qs2 )
							return true;
						else if( qs1 < qs2 )
							return false;
					}
					else
					{
						if( qs1 < qs2 )
							return true;
						else if( qs1 > qs2 )
							return false;
					}
					break;
				}
				case SORT_BY_SHUTOUTPITCHERS:
				{
					double shuts1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["Shutouts"] : 0, shuts2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["Shutouts"] : 0;
					if( (*it).m_direction )
					{
						if( shuts1 > shuts2 )
							return true;
						else if( shuts1 < shuts2 )
							return false;
					}
					else
					{
						if( shuts1 < shuts2 )
							return true;
						else if( shuts1 > shuts2 )
							return false;
					}
					break;
				}
				case SORT_BY_BSPITCHERS:
				{
					double bs1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["BS"] : 0, bs2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["BS"] : 0;
					if( (*it).m_direction )
					{
						if( bs1 > bs2 )
							return true;
						else if( bs1 < bs2 )
							return false;
					}
					else
					{
						if( bs1 < bs2 )
							return true;
						else if( bs1 > bs2 )
							return false;
					}
					break;
				}
				case SORT_BY_SBSPITCHERS:
				{
					double sbs1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["S-BS"] : 0, sbs2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["S-BS"] : 0;
					if( (*it).m_direction )
					{
						if( sbs1 > sbs2 )
							return true;
						else if( sbs1 < sbs2 )
							return false;
					}
					else
					{
						if( sbs1 < sbs2 )
							return true;
						else if( sbs1 > sbs2 )
							return false;
					}
					break;
				}
				case SORT_BY_SHOLDSPITCHERS:
				{
					double sholds1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["S+Holds"] : 0, sholds2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["S+Holds"] : 0;
					if( (*it).m_direction )
					{
						if( sholds1 > sholds2 )
							return true;
						else if( sholds1 < sholds2 )
							return false;
					}
					else
					{
						if( sholds1 < sholds2 )
							return true;
						else if( sholds1 > sholds2 )
							return false;
					}
					break;
				}
				case SORT_BY_SHOLDSBSPITCHERS:
				{
					double sholdsbs1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["S+Holds-BS"] : 0, sholdsbs2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["S+Holds-BS"] : 0;
					if( (*it).m_direction )
					{
						if( sholdsbs1 > sholdsbs2 )
							return true;
						else if( sholdsbs1 < sholdsbs2 )
							return false;
					}
					else
					{
						if( sholdsbs1 < sholdsbs2 )
							return true;
						else if( sholdsbs1 > sholdsbs2 )
							return false;
					}
					break;
				}
				case SORT_BY_K9PITCHERS:
				{
					double k91 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["K/9"] : 0, k92 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["K/9"] : 0;
					if( (*it).m_direction )
					{
						if( k91 > k92 )
							return true;
						else if( k91 < k91 )
							return false;
					}
					else
					{
						if( k91 < k91 )
							return true;
						else if( k91 > k91 )
							return false;
					}
					break;
				}
				case SORT_BY_H9PITCHERS:
				{
					double h91 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["H/9"] : 0, h92 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["H/9"] : 0;
					if( (*it).m_direction )
					{
						if( h91 > h92 )
							return true;
						else if( h91 < h92 )
							return false;
					}
					else
					{
						if( h91 < h92 )
							return true;
						else if( h91 > h92 )
							return false;
					}
					break;
				}
				case SORT_BY_BB9PITCHERS:
				{
					double bb91 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["BB/9"] : 0, bb92 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["BB/9"] : 0;
					if( (*it).m_direction )
					{
						if( bb91 > bb92 )
							return true;
						else if( bb91 < 992 )
							return false;
					}
					else
					{
						if( bb91 < bb92 )
							return true;
						else if( bb91 > bb92 )
							return false;
					}
					break;
				}
				case SORT_BY_KBBPITCHERS:
				{
					double kbb1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["K/BB"] : 0, kbb2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["K/BB"] : 0;
					if( (*it).m_direction )
					{
						if( kbb1 > kbb2 )
							return true;
						else if( kbb1 < kbb2 )
							return false;
					}
					else
					{
						if( kbb1 < kbb2 )
							return true;
						else if( kbb1 > kbb2 )
							return false;
					}
					break;
				}
				case SORT_BY_KMINBBPITCHERS:
				{
					double kminbb1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["K-BB"] : 0, kminbb2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["K-BB"] : 0;
					if( (*it).m_direction )
					{
						if( kminbb1 > kminbb2 )
							return true;
						else if( kminbb1 < kminbb2 )
							return false;
					}
					else
					{
						if( kminbb1 < kminbb2 )
							return true;
						else if( kminbb1 > kminbb2 )
							return false;
					}
					break;
				}
				case SORT_BY_W100PITCHERS:
				{
					double w1001 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["W%"] : 0, w1002 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["W%"] : 0;
					if( (*it).m_direction )
					{
						if( w1001 > w1002 )
							return true;
						else if( w1001 < w1002 )
							return false;
					}
					else
					{
						if( w1001 < w1002 )
							return true;
						else if( w1001 > w1002 )
							return false;
					}
					break;
				}
				case SORT_BY_HOLDSPITCHERS:
				{
					double holds1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["Holds"] : 0, holds2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["Holds"] : 0;
					if( (*it).m_direction )
					{
						if( holds1 > holds2 )
							return true;
						else if( holds1 < holds2 )
							return false;
					}
					else
					{
						if( holds1 < holds2 )
							return true;
						else if( holds1 > holds2 )
							return false;
					}
					break;
				}
				case SORT_BY_GPITCHERS:
				{
					double g1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["G"] : 0, g2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["G"] : 0;
					if( (*it).m_direction )
					{
						if( g1 > g2 )
							return true;
						else if( g1 < g2 )
							return false;
					}
					else
					{
						if( g1 < g2 )
							return true;
						else if( g1 > g2 )
							return false;
					}
					break;
				}
				case SORT_BY_DRAFTPOSITION:
				{
					wxString dp1 = player1.IsPlayerDrafted() ? const_cast<CPlayer &>( player1 ).GetDraftedPosition() : wxT( "" ), dp2 = player2.IsPlayerDrafted() ? const_cast<CPlayer &>( player2 ).GetDraftedPosition() : wxT( "" );
					if( (*it).m_direction )
					{
						if( dp1 > dp2 )
							return true;
						else if( dp1 < dp2 )
							return  false;
					}
					else
					{
						if( dp1 < dp2 )
							return true;
						else if( dp1 > dp2 )
							return false;
					}
					break;
				}
			}
		}
		return false;
	}
	std::vector<SortObject> m_type;
};

struct Remover
{
public:
	bool operator()(const CPlayer &player) const
	{
		return player.IsNewPlayer();
	}
};

class CLeagueData
{
public:
	CLeagueData();
	~CLeagueData();
	CLeagueData &operator=(const CLeagueData &data);
	CLeagueSettings *m_settings;
	std::vector<CPlayer> *m_players;
};

#define ADDPLAYER			1
#define CHANGEPLAYERFROM0	2
#define CHANGEPLAYERTO0		3

#define POSITIVE_DIFF 1
#define NEGATIVE_DIFF 0