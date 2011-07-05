// spec4.h : main header file for the spec4 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "conf_const.h"
#include "resource.h"       // main symbols
#include "MainFrm.h"
#include "SplashDialog.h"
#include "Spectrometer.h"
// #include "RPGarmCom.h"
#include "GPSgeneric.h"
#include "Cspec4Cfg.h"
#include "spec4Doc.h"
#include "CustomerInfoDialog.h"

// Cspec4App:
// See spec4.cpp for the implementation of this class
//

class Cspec4App : public CWinApp
{
public:
	Cspec4App();
	~Cspec4App();


public:
	// Overrides
	virtual BOOL InitInstance();

	// Utility functions

	//! Set run mode variable and update screen
	inline void setRunmode(int mode,bool update=true) { 
		runmode=mode; 
		if (update)
			// ((CFrameWnd *)(AfxGetMainWnd()))->GetActiveView()->GetDocument()->setProgMode(mode);
			((Cspec4Doc *)(((CMainFrame *)(AfxGetMainWnd()))->GetActiveView()->GetDocument()))->SetProgMode(mode);
	}

	//! Get the password variable from the config class
	inline CString getRunmodePass() {

	}

	inline int  getRunmode() { return runmode; }

	//! Change text in status bar and log to file for forensic analysis
	inline void ChangeStatusText(LPCTSTR text) { 
		LogToFile(text);
		((CMainFrame *)m_pMainWnd)->ChangeStatusText(text); 
	}

	inline void ChangeDebugText(int level,LPCTSTR text) { 
// #ifdef DEBUG
		CString tval;
		tval.Format(_T("[DBG %d] %s"),level,text);
		LogToFile(tval.GetString());
// #endif
	}

	//! Log dated string for forensic analysis
	void LogToFile(LPCTSTR test);

	//! Set path for logfile
	void SetStatusLogPath(CString *path);


// Implementation
	afx_msg void OnAppAbout();
	afx_msg void OnToolsChange();
	afx_msg void OnToolsLoad();
	afx_msg void OnToolsSave();
	afx_msg void OnViewSelectdatafiles();
	afx_msg void OnViewSavegraphinfo();
	afx_msg void OnViewLoadgraphinfo();
	afx_msg void OnCollectEnteratl();
	afx_msg void OnCollectGpstoatl();
	afx_msg void OnCollectLoaddefinition();
	afx_msg void OnCollectMeasure();
	afx_msg void OnCollectSaveatl();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
#if 0 // we don't have these menu items anymore
	afx_msg void OnCalibrationCreate();
	afx_msg void OnCalibrationLoad();
	afx_msg void OnCalibrationSave();
#endif

	DECLARE_MESSAGE_MAP()

public:
    // Global variables ...
	CSpectrometer *pSpecControl;
	// CRPGarmCom *pGarminPort;
    CGPSgeneric *pGPS;
	Cspec4Cfg *pConfig;
	CCustomerInfoDialog *pCustInfoDlg;
	CCustomerInfoData *pCustInfoData;


protected:
	int runmode;
	CString logpath;
	CStdioFile logfile;
	CString NetBIOSName;
};

extern Cspec4App theApp;