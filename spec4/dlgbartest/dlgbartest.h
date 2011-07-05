// dlgbartest.h : main header file for the dlgbartest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CdlgbartestApp:
// See dlgbartest.cpp for the implementation of this class
//

class CdlgbartestApp : public CWinApp
{
public:
	CdlgbartestApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CdlgbartestApp theApp;