#pragma once

#define SHOW_AVAILABLE_PLAYERS 0
#define SHOW_DRAFTED_PLAYERS   1
#define SHOW_ALL_PLAYERS       2

class CPlayersPanel : public wxPanel
{
public:
	CPlayersPanel();
	CPlayersPanel(wxWindow *parent, const CLeagueData &data);
	~CPlayersPanel(void);
	wxTextCtrl &GetPlayerNameCtrl();
	void PerformDraft(const CPlayer &player, double inflation, const wxString &name);
	void SetPlayerToZero(const wxString &name);
	void UnAssignPlayer(const CPlayer &player, const double &inflation);
	void SetSelectionToRow(int row);
	wxButton &GetAddPlayerButton();
	wxButton &GetEditColumnsButton();
	int GetSortingType();
	CNameCompleter &GetCompleter();
	void OnFilterHitters(wxCommandEvent &event);
	void OnFilterAllHitters(wxCommandEvent &event);
	void OnFilterAllPitchers(wxCommandEvent &event);
	void OnFilterPitchers(wxCommandEvent &event);
	void OnFirstPlayer(wxCommandEvent &event);
	void OnAllPlayers(wxCommandEvent &event);
	void OnNameEnter(wxCommandEvent &event);
	void OnPlayersFilter(wxCommandEvent &event);
	void OnNotesChanged(wxGridEvent &event);
	wxGrid *GetPlayersGrid() const;
	int GetFilterType();
	void ColumnsChanged(const std::map<wxString,bool> &hitterColumns, const std::map<wxString,bool> &pitcherColumns, const std::map<wxString,bool> &columns);
	void OnSortingData(wxGridEvent &event);
	void RecalculatePlayersValue(double value, bool isEdit, const CPlayer *player, const wxString &playerDropped, double val);
	void OnResizingColumn(wxGridSizeEvent &event);
	void DisplayPlayer(const CPlayer &player, bool addRow);
	void SetCurrentPlayerRow(int row);
protected:
	bool m_isResizing;
	void GeneralPlayerFilter(const CPlayer &player, bool addRow);
	void PositionPlayerFilter(const CPlayer &player, bool addRow);
	int FindColumnByLabel(const wxString &label);
	void FilterColumns(bool displayHitters, bool displayPitchers);
	void CapitalizeName(wxString &name);
	void set_properties();
	void do_layout();
	int m_filterType;
private:
	std::map<wxString,bool> m_hitterColumns, m_pitcherColumns, m_columns;
	std::map<wxString,int> m_gridWidths;
	int m_pitcherColumn, m_row, m_selection;
	CNameCompleter *m_completer;
	bool m_filter;
	wxToggleButton* m_filterC;
	wxToggleButton* m_filter1B;
	wxToggleButton* m_filter2B;
	wxToggleButton* m_filterSS;
	wxToggleButton* m_filter3B;
	wxToggleButton* m_filterOF;
	wxToggleButton* m_filterDH;
	wxToggleButton* m_filterCI;
	wxToggleButton* m_filterMI;
	wxToggleButton* m_allHitters;
	wxToggleButton *m_allPlayers;
	wxToggleButton* m_filterP;
	wxToggleButton* m_filterSP;
	wxToggleButton* m_filterRP;
	wxToggleButton* m_allPitchers;
	wxStaticText* m_label1;
	CTextCtrl* m_playerSearch;
	wxButton* m_label2;
	wxRadioBox *m_typeFilter;
	wxButton* m_edit;
	wxButton* m_add;
	wxGrid* m_players;
	CLeagueData *m_data;
	wxString m_sorter;
	int m_sortColumn;
	PlayerSorter m_sort;
	DECLARE_DYNAMIC_CLASS(CPlayersPanel)
};

