// menutest.h : main header file for the menutest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CmenutestApp:
// See menutest.cpp for the implementation of this class
//

class CmenutestApp : public CWinApp
{
public:
	CmenutestApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CmenutestApp theApp;