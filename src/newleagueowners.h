#pragma once
class CNewLeagueOwners : public wxPanel
{
public:
	CNewLeagueOwners(wxWindow *parent, CLeagueSettings *settings, bool isUpdate);
	~CNewLeagueOwners(void);
	void SetOwnersNumber(unsigned int ownersNumber);
	std::vector<std::string> &GetOwnersNames();
	int GetOwnersNumber();
	virtual bool Validate();
	wxTextCtrl *GetOwner(int pos);
	void SetOwners(const std::vector<std::string> &owners);
	void SetStrictValidation(bool strict);
protected:
	void CreateLayout(unsigned int owners);
private:
	unsigned int m_oldNumber;
	wxBoxSizer *mainSizer, *vertical, *panelHolder;
	wxStaticBoxSizer *staticbox;
	wxGridBagSizer *container;
	wxPanel *m_holder;
	wxTextCtrl *m_owners[30];
	wxStaticText *m_numbers[30];
	int m_ownersNumber;
	bool m_isUpdate, m_strict;
	std::vector<std::string> m_ownersStr;
};

