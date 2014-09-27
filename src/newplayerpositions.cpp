// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/combo.h"
#include "wx/checklst.h"
#include "newplayerpositions.h"

CNewPlayerPositions::CNewPlayerPositions(void)
{
}

CNewPlayerPositions::~CNewPlayerPositions(void)
{
}

PositionsPopup::PositionsPopup()
{
}

PositionsPopup::~PositionsPopup()
{
}

bool PositionsPopup::Create(wxWindow *parent)
{
	static const wxString positions[] =
	{
		wxT( "C" ),
		wxT( "1B" ),
		wxT( "2B" ),
		wxT( "SS" ),
		wxT( "3B" ),
		wxT( "OF" ),
		wxT( "DH" ),
		wxT( "CI" ),
		wxT( "MI" ),
		wxT( "UI" ),
		wxT( "Utility" ),
		wxT( "SP" ),
		wxT( "RP" ),
		wxT( "P" )
	};
	return wxCheckListBox::Create( parent, wxID_ANY, wxPoint( 0, 0 ), wxDefaultSize, 14, positions, wxLB_MULTIPLE );
}

wxWindow *PositionsPopup::GetControl()
{
	return this;
}

wxString PositionsPopup::GetStringValue() const
{
	wxArrayInt positions;
	wxString result = wxEmptyString;
	unsigned int pos = GetCheckedItems( positions );
	for( unsigned int i = 0; i < pos; i++ )
	{
		result += GetString( positions.Item( i ) );
		if( i != pos - 1 )
			result += ", ";
	}
	return result;
}

#if !wxCHECK_VERSION( 2, 9, 5 )
unsigned int PositionsPopup::GetCheckedItems(wxArrayInt &checkedItems) const
{
	unsigned int const itemsCount = GetCount();
	checkedItems.Empty();
	for( unsigned int i = 0; i < itemsCount; ++i )
	{
		if( IsChecked( i ) )
			checkedItems.Add( i );
	}
	return checkedItems.GetCount();
}
#endif