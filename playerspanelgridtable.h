#pragma once
class CPlayersPanelGridTable : public wxGridTableBase
{
public:
	enum
	{
		COL_RANGE,
		COL_NAME,
		COL_TEAM,
		COL_POSITION,
		COL_AGE,
		COL_AVG,
		COL_HR,
		COL_RUNS,
		COL_RBI,
		COL_SB,
		COL_OBP,
		COL_SLG,
		COL_OPS,
		COL_bb,
		COL_h,
		COL_Ks,
		COL_1B,
		COL_2B,
		COL_3B,
		COL_TB,
		COL_2B_3B_HR,
		COL_2B_3B,
		COL_SB_CS,
		COL_CS,
		COL_R_RBI_HR,
		COL_H_BB,
		COL_TB_BB,
		COL_AB,
		COL_HBP,
		COL_GDP,
		COL_SF,
		COL_BEGVALUE,
		COL_CURRVALUE,
		COL_AMTPAID,
		COL_PROFIT,
		COL_OWNER,
		COL_WINS,
		COL_SAVES,
		COL_ERA,
		COL_WHIP,
		COL_K,
		COL_IP,
		COL_L,
		COL_H,
		COL_BB,
		COL_ER,
		COL_CG,
		COL_W_L,
		COL_QS,
		COL_SHUTOUTS,
		COL_BS,
		COL_S_BS,
		COL_S_HOLDS,
		COL_S_HOLDS_BS,
		COL_K_9,
		COL_H_9,
		COL_BB_9,
		COL_K_DIV_BB,
		COL_K_BB,
		COL_W_PERCENT,
		COL_HOLDS,
		COL_G,
		COL_R,
		COL_hr,
		COL_NOTES,
		COL_MAX
	};
	CPlayersPanelGridTable(const std::vector<CPlayer> players);
	~CPlayersPanelGridTable(void);
	virtual int GetNumberRows();
	virtual int GetNumberCols();
	virtual wxString GetValue(int row, int col);
	virtual wxString GetColLabelValue(int col);
    virtual void SetValue(int, int, const wxString&);
private:
	std::vector<CPlayer> m_players;
	int *m_sortOrder;
};

