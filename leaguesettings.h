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
private:
	wxString m_name;
	bool m_type, m_benchPlayerIncluded;
	std::vector<std::string> m_owners;
	std::map<wxString,int> m_positions;
	std::map<wxString,double> m_targetHits, m_targetPitches;
	int m_ownersNum, m_draftType, m_scoringType, m_roundingType/*, m_positions[14]*/, m_salary;
	std::vector<int> m_americanleague, m_nationalleague;
	std::vector<wxString> m_hitsScoringName, m_pitchScoringName;
};

class CPlayer
{
public:
	CPlayer()
	{
	}

	CPlayer(const long &id, const wxString &name, const std::vector<wxString> &position, int age, double value, const wxString &team, const wxString &teamAbbreviated, const std::map<wxString,double> &scoring, bool hitter, double currValue, const wxString &notes, double baseValue)
	{
		m_range = 0;
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
	inline void SetPosition(wxString position) { m_positions.push_back( position ); };
	inline void ClearPositions() { m_positions.clear(); };
	inline void SetScore(const std::map<wxString,double> &score) { m_score = score; };
	inline int GetAge() const { return m_age; };
	inline wxString &GetNotes() { return m_notes; };
	inline void SetNotes(const wxString &notes) { m_notes = notes; };
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
	inline void SetRange(int range) { m_range = range; };
	inline int GetRange() { return m_range; };
	inline void SetBaseValue(double value) { m_baseValue = value; };
	inline double GetBaseValue() { return m_baseValue; };
	inline void SetOriginalScore(const std::map<wxString,double> &originalScore)  { m_originalScore = originalScore; };
	inline std::map<wxString,double> &GetOriginalScore() { return m_originalScore; };
private:
	wxString m_name, m_team, m_teamAbbreviated, m_notes, m_draftedPosition, m_owner;
	int m_age, m_amountPaid, m_draftOrder, m_range;
	long m_id;
	double m_currValue, m_baseValue, m_value;
	bool m_isHitter, m_isDrafted, m_isNew;
	std::map<wxString, double> m_score, m_originalScore;
	std::vector<wxString> m_positions;
};

// bool sortPlayersByValue(const CPlayer &player1, const CPlayer &player2);
struct PlayerSorter
{
	bool operator()(const CPlayer &player1, const CPlayer &player2)
	{
		wxString name1 = const_cast<CPlayer &>( player1 ).GetName(), name2 = const_cast<CPlayer &>( player2 ).GetName();
		switch( m_sortType )
		{
		case SORT_BY_RANGE:
			{
				int range1 = const_cast<CPlayer &>( player1 ).GetRange(), range2 = const_cast<CPlayer &>( player2 ).GetRange();
				return m_forward ? range1 < range2 : range1 > range2;
			}
		case SORT_BY_NAME:
			return m_forward ? name1 < name2 : name1 > name2;
			break;
		case SORT_BY_TEAM:
			{
				wxString team1 = const_cast<CPlayer &>( player1 ).GetTeam(), team2 = const_cast<CPlayer &>( player2 ).GetTeam();
				return m_forward ? ( team1 < team2 ? true : team1 == team2 ? name1 < name2 : false ) : ( team1 > team2 ? true : team1 == team2 ? name1 < name2 : false );
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
				return m_forward ? ( position1 < position2 ? true : position1 == position2 ? name1 < name2 : false ) : ( position1 > position2 ? true : position1 == position2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_AGE:
			{
				int age1 = player1.GetAge(), age2 = player2.GetAge();
				return m_forward ? ( age1 < age2 ? true : age1 == age2 ? name1 < name2 : false ) : ( age1 > age2 ? true : age1 == age2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_VALUE:
			{
				double value1 = player1.GetValue(), value2 = player2.GetValue(), currValue1 = player1.GetCurrentValue(), currValue2 = player2.GetCurrentValue();
				if( m_forward )
				{
					if( value1 > value2 )
						return true;
					else if( value1 == value2 )
					{
						if( currValue1 > currValue2 )
							return true;
						else if( currValue1 == currValue2 )
						{
							if( name1 < name2 )
								return true;
							else
								return false;
						}
						else
							return false;
					}
					else
						return false;
				}
				else
				{
					if( value1 < value2 )
						return true;
					else if( value1 == value2 )
					{
						if( currValue1 < currValue2 )
							return true;
						else if( currValue1 == currValue2 )
						{
							if( name1 < name2 )
								return true;
							else
								return false;
						}
						else
							return false;
					}
					else
						return false;
				}
				break;
			}
		case SORT_BY_CURRVALUE:
			{
				double value1 = player1.GetCurrentValue(), value2 = player2.GetCurrentValue();
				return m_forward ? ( value1 > value2 ? true : value1 == value2 ? name1 < name2 : false ) : ( value1 < value2 ? true : value1 == value2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_AMTPAID:
			{
				int value1 = player1.GetAmountPaid(), value2 = player2.GetAmountPaid();
				return m_forward ? ( value1 > value2 ? true : value1 == value2 ? name1 < name2 : false ) : ( value1 < value2 ? true : value1 == value2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_PROFIT:
			{
				double profit1 = player1.GetValue() - player1.GetAmountPaid(), profit2 = player2.GetValue() - player2.GetAmountPaid();
				return m_forward ? ( profit1 > profit2 ? true : profit1 == profit2 ? name1 < name2 : false ) : ( profit1 > profit2 ? true : profit1 == profit2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_OWNER:
			{
				wxString owner1 = const_cast<CPlayer &>( player1 ).GetOwner(), owner2 = const_cast<CPlayer &>( player2 ).GetOwner();
				return m_forward ? ( owner1 > owner2 ? true : owner1 == owner2 ? name1 < name2 : false ) : ( owner1 < owner2 ? true : owner1 == owner2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_NOTES:
			{
				wxString notes1 = const_cast<CPlayer &>( player1 ).GetNotes(), notes2 = const_cast<CPlayer &>( player2 ).GetNotes();
				return m_forward ? ( notes1 > notes2 ? true : notes1 == notes2 ? name1 < name2 : false ) : ( notes1 < notes2 ? true : notes1 == notes2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_KHITTERS:
			{
				double k1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["Ks"] : 0, k2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["Ks"] : 0;
				return m_forward ? ( k1 > k2 ? true : k1 == k2 ? name1 < name2 : false ) : ( k1 < k2 ? true : k1 == k2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_KPITCHERS:
			{
				double k1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["K"] : 0, k2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["K"] : 0;
				return m_forward ? ( k1 > k2 ? true : k1 == k2 ? name1 < name2 : false ) : ( k1 < k2 ? true : k1 == k2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_HHITTERS:
			{
				double h1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["h"] : 0, h2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["h"] : 0;
				return m_forward ? ( h1 > h2 ? true : h1 == h2 ? name1 < name2 : false ) : ( h1 < h2 ? true : h1 == h2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_HPITCHERS:
			{
				double h1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["H"] : 0, h2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["H"] : 0;
				return m_forward ? ( h1 > h2 ? true : h1 == h2 ? name1 < name2 : false ) : ( h1 < h2 ? true : h1 == h2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_RUNSHITTERS:
			{
				double r1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["Runs"] : 0, r2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["Runs"] : 0;
				return m_forward ? ( r1 > r2 ? true : r1 == r2 ? name1 < name2 : false ) : ( r1 < r2 ? true : r1 == r2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_RPITCHERS:
			{
				double r1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["R"] : 0, r2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["R"] : 0;
				return m_forward ? ( r1 > r2 ? true : r1 == r2 ? name1 < name2 : false ) : ( r1 < r2 ? true : r1 == r2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_HRHITTERS:
			{
				double hr1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["HR"] : 0, hr2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["HR"] : 0;
				return m_forward ? ( hr1 > hr2 ? true : hr1 == hr2 ? name1 < name2 : false ) : ( hr1 < hr2 ? true : hr1 == hr2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_HRPITCHERS:
			{
				double hr1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["hr"] : 0, hr2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["hr"] : 0;
				return m_forward ? ( hr1 > hr2 ? true : hr1 == hr2 ? name1 < name2 : false ) : ( hr1 < hr2 ? true : hr1 == hr2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_BBHITTERS:
			{
				double bb1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["bb"] : 0, bb2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["bb"] : 0;
				return m_forward ? ( bb1 > bb2 ? true : bb1 == bb2 ? name1 < name2 : false ) : ( bb1 < bb2 ? true : bb1 == bb2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_BBPITCHERS:
			{
				double bb1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["BB"] : 0, bb2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["BB"] : 0;
				return m_forward ? ( bb1 > bb2 ? true : bb1 == bb2 ? name1 < name2 : false ) : ( bb1 < bb2 ? true : bb1 == bb2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_AVGHITTERS:
			{
				double avg1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["AVG"] : 0, avg2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["AVG"] : 0;
				return m_forward ? ( avg1 > avg2 ? true : avg1 == avg2 ? name1 < name2 : false ) : ( avg1 < avg2 ? true : avg1 == avg2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_OBPHITTERS:
			{
				double obp1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["OBP"] : 0, obp2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["OBP"] : 0;
				return m_forward ? ( obp1 > obp2 ? true : obp1 == obp2 ? name1 < name2 : false ) : ( obp1 < obp2 ? true : obp1 == obp2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_SLGHITTERS:
			{
				double slg1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["SLG"] : 0, slg2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["SLG"] : 0;
				return m_forward ? ( slg1 > slg2 ? true : slg1 == slg2 ? name1 < name2 : false ) : ( slg1 < slg2 ? true : slg1 == slg2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_OPSHITTERS:
			{
				double ops1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["OPS"] : 0, ops2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["OPS"] : 0;
				return m_forward ? ( ops1 > ops2 ? true : ops1 == ops2 ? name1 < name2 : false ) : ( ops1 < ops2 ? true : ops1 == ops2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_ABHITTERS:
			{
				double ab1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["AB"] : 0, ab2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["AB"] : 0;
				return m_forward ? ( ab1 > ab2 ? true : ab1 == ab2 ? name1 < name2 : false ) : ( ab1 < ab2 ? true : ab1 == ab2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_1BHITTERS:
			{
				double s1b1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["1B"] : 0, s1b2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["1B"] : 0;
				return m_forward ? ( s1b1 > s1b2 ? true : s1b1 == s1b2 ? name1 < name2 : false ) : ( s1b1 < s1b2 ? true : s1b1 == s1b2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_2BHITTERS:
			{
				double s2b1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["2B"] : 0, s2b2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["2B"] : 0;
				return m_forward ? ( s2b1 > s2b2 ? true : s2b1 == s2b2 ? name1 < name2 : false ) : ( s2b1 < s2b2 ? true : s2b1 == s2b2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_3BHITTERS:
			{
				double s3b1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["3B"] : 0, s3b2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["3B"] : 0;
				return m_forward ? ( s3b1 > s3b2 ? true : s3b1 == s3b2 ? name1 < name2 : false ) : ( s3b1 < s3b2 ? true : s3b1 == s3b2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_TBHITTERS:
			{
				double tb1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["TB"] : 0, tb2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["TB"] : 0;
				return m_forward ? ( tb1 > tb2 ? true : tb1 == tb2 ? name1 < name2 : false ) : ( tb1 < tb2 ? true : tb1 == tb2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_RBIHITTERS:
			{
				double rbi1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["RBI"] : 0, rbi2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["RBI"] : 0;
				return m_forward ? ( rbi1 > rbi2 ? true : rbi1 == rbi2 ? name1 < name2 : false ) : ( rbi1 < rbi2 ? true : rbi1 == rbi2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_SBHITTERS:
			{
				double sb1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["SB"] : 0, sb2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["SB"] : 0;
				return m_forward ? ( sb1 > sb2 ? true : sb1 == sb2 ? name1 < name2 : false ) : ( sb1 < sb2 ? true : sb1 == sb2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_2B3BHRHITTERS:
			{
				double s2b3bhr1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["2B+3B+HR"] : 0, s2b3bhr2 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["2B+3B+HR"] : 0;
				return m_forward ? ( s2b3bhr1 > s2b3bhr2 ? true : s2b3bhr1 == s2b3bhr2 ? name1 < name2 : false ) : ( s2b3bhr1 < s2b3bhr2 ? true : s2b3bhr1 == s2b3bhr2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_2B3BHITTERS:
			{
				double s2b3b1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["2B+3B"] : 0, s2b3b2 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["2B+3B"] : 0;
				return m_forward ? ( s2b3b1 > s2b3b2 ? true : s2b3b1 == s2b3b2 ? name1 < name2 : false ) : ( s2b3b1 < s2b3b2 ? true : s2b3b1 == s2b3b2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_SBCSHITTERS:
			{
				double sbcs1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["SB-CS"] : 0, sbcs2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["SB-CS"] : 0;
				return m_forward ? ( sbcs1 > sbcs2 ? true : sbcs1 == sbcs2 ? name1 < name2 : false ) : ( sbcs1 < sbcs2 ? true : sbcs1 == sbcs2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_CSHITTERS:
			{
				double cs1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["CS"] : 0, cs2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["CS"] : 0;
				return m_forward ? ( cs1 > cs2 ? true : cs1 == cs2 ? name1 < name2 : false ) : ( cs1 < cs2 ? true : cs1 == cs2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_RRBIHRHITTERS:
			{
				double rrbihr1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["R+RBI-HR"] : 0, rrbihr2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["R+RBI-HR"] : 0;
				return m_forward ? ( rrbihr1 > rrbihr2 ? true : rrbihr1 == rrbihr2 ? name1 < name2 : false ) : ( rrbihr1 < rrbihr2 ? true : rrbihr1 == rrbihr2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_HBBHITTERS:
			{
				double hbb1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["H+BB"] : 0, hbb2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["H+BB"] : 0;
				return m_forward ? ( hbb1 > hbb2 ? true : hbb1 == hbb2 ? name1 < name2 : false ) : ( hbb1 < hbb2 ? true : hbb1 == hbb2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_TBBBHITTERS:
			{
				double tbbb1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["TB+BB"] : 0, tbbb2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["TB+BB"] : 0;
				return m_forward ? ( tbbb1 > tbbb2 ? true : tbbb1 == tbbb2 ? name1 < name2 : false ) : ( tbbb1 < tbbb2 ? true : tbbb1 == tbbb2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_HBPHITTERS:
			{
				double hbp1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["HBP"] : 0, hbp2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["HBP"] : 0;
				return m_forward ? ( hbp1 > hbp2 ? true : hbp1 == hbp2 ? name1 < name2 : false ) : ( hbp1 < hbp2 ? true : hbp1 == hbp2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_GDPHITTERS:
			{
				double gdp1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["GDP"] : 0, gdp2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["GDP"] : 0;
				return m_forward ? ( gdp1 > gdp2 ? true : gdp1 == gdp2 ? name1 < name2 : false ) : ( gdp1 < gdp2 ? true : gdp1 == gdp2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_SFHITTERS:
			{
				double sf1 = player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["SF"] : 0, sf2 = player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["SF"] : 0;
				return m_forward ? ( sf1 > sf2 ? true : sf1 == sf2 ? name1 < name2 : false ) : ( sf1 < sf2 ? true : sf1 == sf2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_WPITCHERS:
			{
				double wins1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["Wins"] : 0, wins2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["Wins"] : 0;
				return m_forward ? ( wins1 > wins2 ? true : wins1 == wins2 ? name1 < name2 : false ) : ( wins1 < wins2 ? true : wins1 == wins2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_SPITCHERS:
			{
				double saves1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["Saves"] : 0, saves2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["Saves"] : 0;
				return m_forward ? ( saves1 > saves2 ? true : saves1 == saves2 ? name1 < name2 : false ) : ( saves1 < saves2 ? true : saves1 == saves2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_ERAPITCHERS:
			{
				double era1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["ERA"] : 0, era2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["ERA"] : 0;
				return m_forward ? ( era1 > era2 ? true : era1 == era2 ? name1 < name2 : false ) : ( era1 < era2 ? true : era1 == era2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_WHIPPITCHERS:
			{
				double whip1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["WHIP"] : 0, whip2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["WHIP"] : 0;
				return m_forward ? ( whip1 > whip2 ? true : whip1 == whip2 ? name1 < name2 : false ) : ( whip1 < whip2 ? true : whip1 == whip2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_IPPITCHERS:
			{
				double ip1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["IP"] : 0, ip2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["IP"] : 0;
				return m_forward ? ( ip1 > ip2 ? true : ip1 == ip2 ? name1 < name2 : false ) : ( ip1 < ip2 ? true : ip1 == ip2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_LPITCHERS:
			{
				double l1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["L"] : 0, l2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["L"] : 0;
				return m_forward ? ( l1 > l2 ? true : l1 == l2 ? name1 < name2 : false ) : ( l1 < l2 ? true : l1 == l2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_ERPITCHERS:
			{
				double er1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["ER"] : 0, er2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["ER"] : 0;
				return m_forward ? ( er1 > er2 ? true : er1 == er2 ? name1 < name2 : false ) : ( er1 < er2 ? true : er1 == er2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_CGPITCHERS:
			{
				double cg1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["CG"] : 0, cg2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["CG"] : 0;
				return m_forward ? ( cg1 > cg2 ? true : cg1 == cg2 ? name1 < name2 : false ) : ( cg1 < cg2 ? true : cg1 == cg2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_WLPITCHERS:
			{
				double wl1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["W-L"] : 0, wl2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["W-L"] : 0;
				return m_forward ? ( wl1 > wl2 ? true : wl1 == wl2 ? name1 < name2 : false ) : ( wl1 < wl2 ? true : wl1 == wl2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_QSPITCHERS:
			{
				double qs1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["QS"] : 0, qs2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["QS"] : 0;
				return m_forward ? ( qs1 > qs2 ? true : qs1 == qs2 ? name1 < name2 : false ) : ( qs1 < qs2 ? true : qs1 == qs2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_SHUTOUTPITCHERS:
			{
				double shuts1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["Shutouts"] : 0, shuts2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["Shutouts"] : 0;
				return m_forward ? ( shuts1 > shuts2 ? true : shuts1 == shuts2 ? name1 < name2 : false ) : ( shuts1 < shuts2 ? true : shuts1 == shuts2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_BSPITCHERS:
			{
				double bs1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["BS"] : 0, bs2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["BS"] : 0;
				return m_forward ? ( bs1 > bs2 ? true : bs1 == bs2 ? name1 < name2 : false ) : ( bs1 < bs2 ? true : bs1 == bs2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_SBSPITCHERS:
			{
				double sbs1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["S-BS"] : 0, sbs2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["S-BS"] : 0;
				return m_forward ? ( sbs1 > sbs2 ? true : sbs1 == sbs2 ? name1 < name2 : false ) : ( sbs1 < sbs2 ? true : sbs1 == sbs2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_SHOLDSPITCHERS:
			{
				double sholds1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["S+Holds"] : 0, sholds2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["S+Holds"] : 0;
				return m_forward ? ( sholds1 > sholds2 ? true : sholds1 == sholds2 ? name1 < name2 : false ) : ( sholds1 < sholds2 ? true : sholds1 == sholds2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_SHOLDSBSPITCHERS:
			{
				double sholdsbs1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["S+Holds-BS"] : 0, sholdsbs2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["S+Holds-BS"] : 0;
				return m_forward ? ( sholdsbs1 > sholdsbs2 ? true : sholdsbs1 == sholdsbs2 ? name1 < name2 : false ) : ( sholdsbs1 < sholdsbs2 ? true : sholdsbs1 == sholdsbs2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_K9PITCHERS:
			{
				double k91 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["K/9"] : 0, k92 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["K/9"] : 0;
				return m_forward ? ( k91 > k92 ? true : k91 == k92 ? name1 < name2 : false ) : ( k91 < k92 ? true : k91 == k92 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_H9PITCHERS:
			{
				double h91 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["H/9"] : 0, h92 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["H/9"] : 0;
				return m_forward ? ( h91 > h92 ? true : h91 == h92 ? name1 < name2 : false ) : ( h91 < h92 ? true : h91 == h92 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_BB9PITCHERS:
			{
				double bb91 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["BB/9"] : 0, bb92 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["BB/9"] : 0;
				return m_forward ? ( bb91 > bb92 ? true : bb91 == bb92 ? name1 < name2 : false ) : ( bb91 < bb92 ? true : bb91 == bb92 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_KBBPITCHERS:
			{
				double kbb1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["K/BB"] : 0, kbb2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["K/BB"] : 0;
				return m_forward ? ( kbb1 > kbb2 ? true : kbb1 == kbb2 ? name1 < name2 : false ) : ( kbb1 < kbb2 ? true : kbb1 == kbb2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_KMINBBPITCHERS:
			{
				double kminbb1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["K-BB"] : 0, kminbb2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["K-BB"] : 0;
				return m_forward ? ( kminbb1 > kminbb2 ? true : kminbb1 == kminbb2 ? name1 < name2 : false ) : ( kminbb1 < kminbb2 ? true : kminbb1 == kminbb2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_W100PITCHERS:
			{
				double w1001 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["W%"] : 0, w1002 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["W%"] : 0;
				return m_forward ? ( w1001 > w1002 ? true : w1001 == w1002 ? name1 < name2 : false ) : ( w1001 < w1002 ? true : w1001 == w1002 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_HOLDSPITCHERS:
			{
				double holds1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["Holds"] : 0, holds2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["Holds"] : 0;
				return m_forward ? ( holds1 > holds2 ? true : holds1 == holds2 ? name1 < name2 : false ) : ( holds1 < holds2 ? true : holds1 == holds2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_GPITCHERS:
			{
				double g1 = !player1.IsHitter() ? const_cast<CPlayer &>( player1 ).GetScoring()["G"] : 0, g2 = !player2.IsHitter() ? const_cast<CPlayer &>( player2 ).GetScoring()["G"] : 0;
				return m_forward ? ( g1 > g2 ? true : g1 == g2 ? name1 < name2 : false ) : ( g1 < g2 ? true : g1 == g2 ? name1 < name2 : false );
				break;
			}
		case SORT_BY_DRAFTPOSITION:
			{
				return player1.GetDraftOrder() < player2.GetDraftOrder();
				break;
			}
		}
		return false;
	}
	int m_sortType;
	bool m_forward;
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