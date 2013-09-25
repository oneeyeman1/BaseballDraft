// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/frame.h"
#include "mainfrane.h"

CMainFrane::CMainFrane(void) : wxFrame( NULL, wxID_ANY, "Fantasy Draft" )
{
}

CMainFrane::~CMainFrane(void)
{
}
