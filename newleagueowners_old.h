#pragma once
class CNewLeagueOwners : public wxPanel
{
public:
	CNewLeagueOwners(wxWindow *parent, CLeagueSettings *settings, bool isUpdate);
	~CNewLeagueOwners(void);
	void SetOwnersNumber(int ownersNumber);
	std::vector<std::string> &GetOwnersNames();
	int GetOwnersNumber();
	virtual bool Validate();
	wxTextCtrl *GetOwner(int pos);
	void SetOwners(const std::vector<std::string> &owners);
protected:
	void CreateLayout(unsigned int owners);
private:
	wxBoxSizer *mainSizer, *vertical, *panelHolder;
	wxStaticBoxSizer *staticbox;
	wxGridBagSizer *container;
	wxPanel *m_holder;
	wxTextCtrl **m_owners;
	wxStaticText **m_numbers;
	int m_ownersNumber;
	bool m_isUpdate;
	std::vector<std::string> m_ownersStr;
};

