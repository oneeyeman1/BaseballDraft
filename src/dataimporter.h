#pragma once
WX_DECLARE_STRING_HASH_MAP( wxZipEntry *, ZipCatalog );
class CDataImporter
{
public:
	CDataImporter(const wxString &name);
	~CDataImporter(void);
	void ReadData(std::vector<std::vector<wxString> > &hitters, std::vector<std::vector<wxString> > &pitchers);
protected:
	void ParsePositions(const wxString &positions, std::vector<wxString> &position);
private:
	wxFFileInputStream *m_in, *m_in1;
	wxZipInputStream *m_zin, *m_zin1;
	int m_pages;
	std::vector<wxString> m_names;
	std::vector<wxString> queries;
	std::vector<std::vector<wxString> > players;
	bool m_badFile;
	ZipCatalog m_catalog;
};

