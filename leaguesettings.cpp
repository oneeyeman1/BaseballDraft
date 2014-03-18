// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include <map>
#include "wx/tipdlg.h"
#include "leaguesettings.h"

/*bool sortPlayersByValue(const CPlayer &player1, const CPlayer &player2)
{
	return player1.GetValue() > player2.GetValue() ? true : player1.GetValue() == player2.GetValue() ? const_cast<CPlayer &>( player1 ).GetName() < const_cast<CPlayer &>( player2 ).GetName() : false;
}*/

CLeagueSettings::CLeagueSettings(const wxString &leagueTitle)
{
	m_name = leagueTitle;
/*	m_owners = new std::vector<std::string>();
	m_americanleague = new std::vector<int>();
	m_nationalleague = new std::vector<int>();
	m_hitsScoringName = new std::vector<wxString>();
	m_pitchScoringName = new std::vector<wxString>();*/
	m_salary = 0;
	m_benchPlayerIncluded = false;
}

CLeagueSettings::CLeagueSettings(const wxString name, int owners, int draftType, int scoringType, int roundType, int salary, bool benchIncluded)
{
	m_name = name;
	m_ownersNum = owners;
	m_draftType = draftType;
	m_scoringType = scoringType;
	m_roundingType = roundType;
	m_salary = salary;
	m_benchPlayerIncluded = benchIncluded;
/*	m_owners = new std::vector<std::string>();
	m_americanleague = new std::vector<int>();
	m_nationalleague = new std::vector<int>();
	m_hitsScoringName = new std::vector<wxString>();
	m_pitchScoringName = new std::vector<wxString>();*/
}

CLeagueSettings::~CLeagueSettings(void)
{
/*	delete m_owners;
	delete m_americanleague;
	delete m_nationalleague;
	delete m_hitsScoringName;
	delete m_pitchScoringName;*/
}

wxString &CLeagueSettings::GetLeagueName() const
{
	return const_cast<wxString &>( m_name );
}

void CLeagueSettings::SetDraftType(int type)
{
	m_draftType = type;
}

int CLeagueSettings::GetDraftType() const
{
	return m_draftType;
}

void CLeagueSettings::SetScoringtype(int type)
{
	m_scoringType = type;
}

int CLeagueSettings::GetScoringType() const
{
	return m_scoringType;
}

std::vector<std::string> &CLeagueSettings::GetOwners() const
{
	return const_cast<std::vector<std::string> &>( m_owners );
}

std::map<wxString,wxString> &CLeagueSettings::GetTeamsName() const
{
	 return const_cast<std::map<wxString,wxString> &>( m_teams );
}

std::vector<int> &CLeagueSettings::GetAmericanLeague() const
{
	return const_cast<std::vector<int> &>( m_americanleague );
}

std::vector<int> &CLeagueSettings::GetNationalLeague() const
{
	return const_cast<std::vector<int> &>( m_nationalleague );
}

std::vector<wxString> &CLeagueSettings::GetHitScoring() const
{
	return const_cast<std::vector<wxString> &>( m_hitsScoringName );
}

std::vector<wxString> &CLeagueSettings::GetPitchScoring() const
{
	return const_cast<std::vector<wxString> &>( m_pitchScoringName );
}

int CLeagueSettings::GetRoundingtype() const
{
	return m_roundingType;
}

std::map<wxString,int> &CLeagueSettings::GetPositions() const
{
	return const_cast<std::map<wxString,int> &>( m_positions );
}

void CLeagueSettings::SetPositions(const std::map<wxString,int> &positions)
{
	m_positions = positions;
}

void CLeagueSettings::AddOwner(const wxString &owner)
{
	m_owners.push_back( owner.ToStdString() );
}

void CLeagueSettings::AddPosition(wxString pos, int value)
{
	m_positions[pos] = value;
}

void CLeagueSettings::AddAmericanLeague(int team)
{
	m_americanleague.push_back( team );
}

int CLeagueSettings::GetSalary() const
{
	return m_salary;
}

void CLeagueSettings::SetSalary(int salary)
{
	m_salary = salary;
}

void CLeagueSettings::AddNationalLeague(int team)
{
	m_nationalleague.push_back( team );
}

void CLeagueSettings::AddHitScoring(wxString score)
{
	m_hitsScoringName.push_back( score );
}

void CLeagueSettings::AddPitchScoring(wxString score)
{
	m_pitchScoringName.push_back( score );
}

void CLeagueSettings::SetRoundingType(int type)
{
	m_roundingType = type;
}

void CLeagueSettings::SetOwnersNumber(int num)
{
	m_ownersNum = num;
}

CLeagueData::CLeagueData()
{
	m_players = new std::vector<CPlayer>();
}

CLeagueData::~CLeagueData()
{
	delete m_settings;
	delete m_players;
	m_settings = NULL;
	m_players = NULL;
}

void CLeagueSettings::SetLeagueName(const wxString &name)
{
	m_name = name;
}

void CLeagueSettings::SetLeagueType(bool type)
{
	m_type = type;
}

bool CLeagueSettings::GetLeagueType() const
{
	return m_type;
}

CLeagueData &CLeagueData::operator=(const CLeagueData &data)
{
	if( this != &data )
	{
		m_settings = new CLeagueSettings( data.m_settings->GetLeagueName(), data.m_settings->GetOwnersNumber(), data.m_settings->GetDraftType(), data.m_settings->GetScoringType(), data.m_settings->GetRoundingtype(), data.m_settings->GetSalary(), data.m_settings->IsBenchPlayerIncluded() );
		m_settings->SetOwners( data.m_settings->GetOwners() );
		m_settings->SetAmericanLeague( data.m_settings->GetAmericanLeague() );
		m_settings->SetNationaLeague( data.m_settings->GetNationalLeague() );
		m_settings->SetHitScoring( data.m_settings->GetHitScoring() );
		m_settings->SetPitchScoring( data.m_settings->GetPitchScoring() );
		*m_players = *data.m_players;
	}
	return *this;
}

bool CLeagueSettings::IsBenchPlayerIncluded() const
{
	return m_benchPlayerIncluded;
}

void CLeagueSettings::SetBenchPlayerIncluded(bool includeBench)
{
	m_benchPlayerIncluded = includeBench;
}

std::map<wxString,double> &CLeagueSettings::GetTargetsForHitters() const
{
	return const_cast<std::map<wxString,double> &>( m_targetHits );
}

std::map<wxString,double> &CLeagueSettings::GetTargetsForPitchers() const
{
	return const_cast<std::map<wxString,double> &>( m_targetPitches );
}

void CLeagueSettings::AddTargetHitter(const wxString &score, double value)
{
	m_targetHits[score] = value;
}

void CLeagueSettings::AddTargetPitchers(const wxString &score, double value)
{
	m_targetPitches[score] = value;
}
