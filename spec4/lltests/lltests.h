// lltests.h : main header file for the lltests application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "wrapper.h"

// ClltestsApp:
// See lltests.cpp for the implementation of this class
//

class ClltestsApp : public CWinApp
{
public:
	ClltestsApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern ClltestsApp theApp;