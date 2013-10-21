// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vector>
#include "wx/wfstream.h"
#include "wx/xml/xml.h"
#include "wx/zipstrm.h"
#include "dataimporter.h"

CDataImporter::CDataImporter(const wxString &name)
{
	m_badFile = false;
	ZipCatalog::iterator it;
	wxZipEntry *entry;

	m_in = new wxFFileInputStream( name );
	m_in1 = new wxFFileInputStream( name );
	m_zin = new wxZipInputStream( m_in );
	m_zin1 = new wxZipInputStream( m_in1 );

	while( ( entry = m_zin->GetNextEntry() ) != NULL )
	{
		wxZipEntry *&current = m_catalog[entry->GetInternalName()];
		delete current;
		current = entry;
	}
	if( ( it = m_catalog.find( wxZipEntry::GetInternalName( "docProps/app.xml" ) ) ) != m_catalog.end() )
	{
		wxXmlDocument doc;
		m_zin->OpenEntry( *it->second );
		if( !doc.Load( *m_zin ) )
		{
			wxMessageBox( "Can't load Excel file" );
		}
		if( doc.GetRoot()->GetName() != "Properties" )
			wxMessageBox( "Incorrect file" );
		wxXmlNode *child = doc.GetRoot()->GetChildren();
		while( child )
		{
			if( child->GetName() == "HeadingPairs" )
			{
				wxXmlNode *attribute = child->GetChildren();
//				wxString content = child->GetNodeContent();
				wxString pages = attribute->GetAttribute( "size", "3" );
				m_pages = wxAtoi( pages );
			}
			if( child->GetName() == "TitlesOfParts" )
			{
				wxXmlNode *attribute = child->GetChildren()->GetChildren();
				for( int i = 0; i < m_pages; i++ )
				{
					m_names.push_back( attribute->GetChildren()->GetContent() );
					attribute = attribute->GetNext();
				}
			}
			child = child->GetNext();
		}
		m_zin->CloseEntry();
	}
}

CDataImporter::~CDataImporter(void)
{
//	delete m_in1;
//	m_in1 = NULL;
//	delete m_in;
//	m_in = NULL;
	delete m_zin1;
	m_zin1 = NULL;
	delete m_zin;
	m_zin = NULL;
}

void CDataImporter::ReadData(std::vector<std::vector<wxString> > &hitters, std::vector<std::vector<wxString> > &pitchers)
{
	ZipCatalog::iterator it;
	wxXmlDocument strings;
	wxXmlNode *stringsRoot = NULL, *root = NULL;
	std::vector<wxString> position;
	wxString name, team, positions;
	int age = 0, value = 0;
	double avg_score = 0.0, obp_score = 0.0, slg_score = 0.0, ops_score = 0.0, ab_score = 0.0, r_score = 0.0, h_score = 0.0, score_1b = 0.0, score_2b = 0.0, score_3b = 0.0, hr_score = 0.0, tb_score = 0.0, rbi_score = 0.0, bb_score = 0.0, k_score = 0.0, hbp_score = 0.0, sf_score = 0.0, sb_score = 0.0, cs_score = 0.0, gdp_score = 0.0;
	double w_score = 0.0, l_score = 0.0, era_score = 0.0, whip_score = 0.0, g_score = 0.0, gs_score = 0.0, wpercent_score = 0.0, ip_score = 0.0, er_score = 0.0, cg_score = 0.0, sho_score = 0.0, s_score = 0.0, hld_score = 0.0, bs_score = 0.0;
	if( ( it = m_catalog.find( wxZipEntry::GetInternalName( "xl/sharedStrings.xml" ) ) ) != m_catalog.end() )
	{
		m_zin1->OpenEntry( *it->second );
		if( !strings.Load( *m_zin1 ) )
		{
			wxMessageBox( "Incorrect Excel format" );
			return;
		}
		stringsRoot = root = strings.GetRoot()->GetChildren();
	}
	if( ( it = m_catalog.find( wxZipEntry::GetInternalName( "xl/worksheets/sheet1.xml" ) ) ) != m_catalog.end() )
	{
		wxXmlDocument doc;
		m_zin->OpenEntry( *it->second );
		if( !doc.Load( *m_zin ) )
		{
			wxMessageBox( "Incorrect Excel file" );
			return;
		}
		if( doc.GetRoot()->GetName() != "worksheet" )
		{
			wxMessageBox( "Incorrect Excel file" );
			return;
		}
		wxXmlNode *prolog = doc.GetDocumentNode()->GetChildren();
		while( prolog )
		{
			if( prolog->GetType() == wxXML_PI_NODE && prolog->GetName() == "" )
			{
				wxString content = prolog->GetContent();
			}
			prolog = prolog->GetNext();
		}
		wxXmlNode *child = doc.GetRoot()->GetChildren();
		while( child )
		{
			if( child->GetName() == "sheetData" )
			{
				wxXmlNode *row = child->GetChildren();
				while( row )
				{
					if( row->GetAttribute( "r" ) != "1" )
					{
						wxXmlNode *cell = row->GetChildren();
						while( cell )
						{
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'A' && ( cell->GetAttribute( "r" ).GetChar( 1 ) >= '0' && cell->GetAttribute( "r" ).GetChar( 1 ) <= '9' ) )
							{
								int value = wxAtoi( cell->GetChildren()->GetChildren()->GetContent() );
								for( int i = 0; i < value; i++ )
								{
									stringsRoot = stringsRoot->GetNext();
								}
								name = stringsRoot->GetChildren()->GetChildren()->GetContent();
								stringsRoot = root;
							}
							if( cell->GetAttribute( "r").GetChar( 0 ) == 'B' )
							{
								int value = wxAtoi( cell->GetChildren()->GetChildren()->GetContent() );
								for( int i = 0; i < value; i++ )
									stringsRoot = stringsRoot->GetNext();
								team = stringsRoot->GetChildren()->GetChildren()->GetContent();
								stringsRoot = root;
							}
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'C' )
								age = wxAtoi( cell->GetChildren()->GetChildren()->GetContent() );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'D' )
							{
								int value = wxAtoi( cell->GetChildren()->GetChildren()->GetContent() );
								for( int i = 0; i < value; i++ )
									stringsRoot = stringsRoot->GetNext();
								positions = stringsRoot->GetChildren()->GetChildren()->GetContent();
								ParsePositions( positions, position );
								stringsRoot = root;
							}
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'E' )
								avg_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'F' )
								obp_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'G' )
								slg_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'H' )
								ops_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'I' )
								ab_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'J' )
								r_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'K' )
								h_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'L' )
								score_1b = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'M' )
								score_2b = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'N' )
								score_3b = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'O' )
								hr_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'P' )
								tb_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'Q' )
								rbi_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'R' )
								bb_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'S' )
								k_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'T' )
								hbp_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'U' )
								sf_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'V' )
								sb_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'W' )
								cs_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'X' )
								gdp_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'Y' )
								value = wxAtoi( cell->GetChildren()->GetChildren()->GetContent() );
							cell = cell->GetNext();
						}
						queries.push_back( wxString::Format( "INSERT INTO players(playerid, name, ishitter, age, value, teamid, currvalue, isdrafted, isnew) VALUES( NULL, \"%s\", \"1\", %d, %.2f, (SELECT teamid FROM teams WHERE teams.shortname = \"%s\"), %.2f, \"0\", \"0\" );", name, age, value, team, (double) value ) );
						for( size_t i = 0; i < position.size(); i++ )
							queries.push_back( wxString::Format( "INSERT INTO playerposition VALUES( NULL, (SELECT positionid FROM positions WHERE positions.positionname = \"%s\") );", position.at( i ) ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"AVG\"), %.3f );", avg_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"HR\"), %.3f);", hr_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"Runs\"), %.3f);", r_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"RBI\"), %.3f);", rbi_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"SB\"), %.3f);", sb_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"OBP\"), %.3f );", obp_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"SLG\"), %.3f );", slg_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"OPS\"), %.3f );", ops_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"BB\"), %.3f);", bb_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"H\"), %.3f);", h_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"K\"), %.3f);", k_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"1B\"), %.3f);", score_1b ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"2B\"), %.3f);", score_2b ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"3B\"), %.3f);", score_3b ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"TB\"), %.3f);", tb_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"2B+3B+HR\"), %.3f);", score_2b + score_3b + hr_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"2B+3B\"), %.3f);", score_2b + score_3b ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"SB-CS\"), %.3f);", sb_score - cs_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"CS\"), %.3f);", cs_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"R+RBI-HR\"), %.3f);", r_score + rbi_score - hr_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"H+BB\"), %.3f);", h_score + bb_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"TB+BB\"), %.3f);", tb_score + bb_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"AB\"), %.3f );", ab_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"HBP\"), %.3f);", hbp_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"SF\"), %.3f);", sf_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorehits VALUES( NULL, (SELECT scoreid FROM scorehits WHERE scorename = \"GDP\"), %.3f);", gdp_score ) );
						hitters.push_back( queries );
						position.clear();
						queries.clear();
						row = row->GetNext();
					}
					else
						row = row->GetNext();
				}
			}
			child = child->GetNext();
		}
	}
	if( ( it = m_catalog.find( wxZipEntry::GetInternalName( "xl/worksheets/sheet2.xml" ) ) ) != m_catalog.end() )
	{
		wxXmlDocument doc;
		m_zin->OpenEntry( *it->second );
		if( !doc.Load( *m_zin ) )
		{
			wxMessageBox( "Incorrect Excel file" );
			return;
		}
		if( doc.GetRoot()->GetName() != "worksheet" )
		{
			wxMessageBox( "Incorrect Excel file" );
			return;
		}
		wxXmlNode *prolog = doc.GetDocumentNode()->GetChildren();
		while( prolog )
		{
			if( prolog->GetType() == wxXML_PI_NODE && prolog->GetName() == "" )
			{
				wxString content = prolog->GetContent();
			}
			prolog = prolog->GetNext();
		}
		wxXmlNode *child = doc.GetRoot()->GetChildren();
		while( child )
		{
			if( child->GetName() == "sheetData" )
			{
				wxXmlNode *row = child->GetChildren();
				while( row )
				{
					if( row->GetAttribute( "r" ) != "1" )
					{
						wxXmlNode *cell = row->GetChildren();
						while( cell )
						{
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'A' && ( cell->GetAttribute( "r" ).GetChar( 1 ) >= '0' && cell->GetAttribute( "r" ).GetChar( 1 ) <= '9' ) )
							{
								int value = wxAtoi( cell->GetChildren()->GetChildren()->GetContent() );
								for( int i = 0; i < value; i++ )
								{
									stringsRoot = stringsRoot->GetNext();
								}
								name = stringsRoot->GetChildren()->GetChildren()->GetContent();
								stringsRoot = root;
							}
							if( cell->GetAttribute( "r").GetChar( 0 ) == 'B' )
							{
								int value = wxAtoi( cell->GetChildren()->GetChildren()->GetContent() );
								for( int i = 0; i < value; i++ )
									stringsRoot = stringsRoot->GetNext();
								team = stringsRoot->GetChildren()->GetChildren()->GetContent();
								stringsRoot = root;
							}
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'C' )
								age = wxAtoi( cell->GetChildren()->GetChildren()->GetContent() );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'D' )
							{
								int value = wxAtoi( cell->GetChildren()->GetChildren()->GetContent() );
								for( int i = 0; i < value; i++ )
									stringsRoot = stringsRoot->GetNext();
								positions = stringsRoot->GetChildren()->GetChildren()->GetContent();
								ParsePositions( positions, position );
								stringsRoot = root;
							}
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'E' )
								w_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'F' )
								l_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'G' )
								era_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'H' )
								whip_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'I' )
								g_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'J' )
								gs_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'K' )
								wpercent_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'L' )
								ip_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'M' )
								h_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'N' )
								r_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'O' )
								er_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'P' )
								hr_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'Q' )
								bb_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'R' )
								k_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'S' )
								cg_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'T' )
								sho_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'U' )
								s_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'V' )
								bs_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'W' )
								hld_score = wxAtof( cell->GetChildren()->GetChildren()->GetContent().SubString( 0, 5 ) );
							if( cell->GetAttribute( "r" ).GetChar( 0 ) == 'X' )
								value = wxAtoi( cell->GetChildren()->GetChildren()->GetContent() );
							cell = cell->GetNext();
						}
						queries.push_back( wxString::Format( "INSERT INTO players(playerid, name, ishitter, age, value, teamid, currvalue, isdrafted, isnew) VALUES( NULL, \"%s\", \"0\", %d, %.2f, (SELECT teamid FROM teams WHERE teams.shortname = \"%s\"), %.2f, \"0\", \"0\" );", name, age, value, team, (double) value ) );
						for( size_t i = 0; i < position.size(); i++ )
							queries.push_back( wxString::Format( "INSERT INTO playerposition VALUES( NULL, (SELECT positionid FROM positions WHERE positions.positionname = \"%s\") );", position.at( i ) ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"Wins\"), %.3f );", w_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"Saves\"), %.3f);", s_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"ERA\"), %.3f);", era_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"WHIP\"), %.3f);", whip_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"K\"), %.3f);", k_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"IP\"), %.3f );", ip_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"L\"), %.3f );", l_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"H\"), %.3f );", h_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"BB\"), %.3f);", bb_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"ER\"), %.3f);", er_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"CG\"), %.3f);", cg_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"W-L\"), %.3f);", w_score - l_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"Shutouts\"), %.3f);", sho_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"BS\"), %.3f);", bs_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"S-BS\"), %.3f);", s_score - bs_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"S+Holds\"), %.3f);", s_score + hld_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"S+Holds-BS\"), %.3f);", s_score + hld_score - bs_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"K/9\"), %.3f);", k_score / 9 ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"H/9\"), %.3f);", h_score / 9 ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"BB/9\"), %.3f);", bb_score / 9 ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"K/BB\"), %.3f);", k_score / bb_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"K-BB\"), %.3f );", k_score - bb_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"W%%\"), %.3f);", wpercent_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"Holds\"), %.3f);", hld_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"G\"), %.3f);", g_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"R\"), %.3f);", r_score ) );
						queries.push_back( wxString::Format( "INSERT INTO playerscorepitches VALUES( NULL, (SELECT scoreid FROM scorepitch WHERE scorename = \"HR\"), %.3f);", hr_score ) );
						pitchers.push_back( queries );
						position.clear();
						queries.clear();
						row = row->GetNext();
					}
					else
						row = row->GetNext();
				}
			}
			child = child->GetNext();
		}
	}
}

void CDataImporter::ParsePositions(const wxString &positions, std::vector<wxString> &position)
{
	wxString rest, temp = positions;
	do
	{
		position.push_back( temp.BeforeFirst( ',', &rest ) );
		temp = rest;
	} 	while( ( rest = temp.BeforeFirst( ',', &rest ) ) != wxEmptyString );
}
