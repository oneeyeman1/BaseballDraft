#pragma once
struct NewPlayerStats
{
	double m_value;
	bool m_used;
	NewPlayerStats()
	{
		m_value = 0;
		m_used = false;
	}
	NewPlayerStats(double value, bool used)
	{
		m_value = value;
		m_used = used;
	}
};

class CEnterStats :	public wxDialog
{
public:
	CEnterStats(wxWindow *parent, const CLeagueSettings &settings, const wxString &positions, const CPlayer *player);
	~CEnterStats(void);
	void OnEnterData(wxCommandEvent &event);
	void OnModifyingStats(wxCommandEvent &event);
	void OnCollapsiblePane(wxCollapsiblePaneEvent &event);
	std::map<wxString,NewPlayerStats> &GetStatistics();
	bool IsDirty();
private:
	int m_main, m_extra, m_rows;
	wxPanel *m_panel;
	wxFlexGridSizer *m_paneSizer;
	wxButton *m_enter, *m_cancel;
	wxCollapsiblePane *m_pane;
	wxStaticText **m_labels, **m_extraLabels;
	wxTextCtrl **m_scores, **m_extraScores;
	std::map<wxString,NewPlayerStats> m_stats;
	bool m_dirty;
	int m_count;
};

