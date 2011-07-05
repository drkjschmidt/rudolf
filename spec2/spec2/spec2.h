// spec2.h : main header file for the spec2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Cspec2App:
// See spec2.cpp for the implementation of this class
//

class Cspec2App : public CWinApp
{
public:
	Cspec2App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern Cspec2App theApp;