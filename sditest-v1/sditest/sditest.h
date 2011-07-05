// sditest.h : main header file for the sditest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "rpgarmcom.h"


// CsditestApp:
// See sditest.cpp for the implementation of this class
//

class CsditestApp : public CWinApp
{
public:
	CsditestApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	CRPGarmCom garminPort;
};

extern CsditestApp theApp;