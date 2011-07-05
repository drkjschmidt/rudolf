// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "conf_const.h"
#include "ColorEdit.h"
#include "DlgBarControl.h"
#include "DlgBarReview.h"
#include "DlgBarAcq.h"
#include "DlgBarTop.h"


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
#if 0 // Not needed after moving to classes
	CDialogBar	c_wndDlgBarTop;  // control for top  dialog bar
	CDialogBar	c_wndDlgBarLeft; // control for left dialog bar
#endif
	CColorEdit	c_colSubst; // control for CColorEdit substance name
	CColorEdit	c_colConc;  // control for CColorEdit substance name

public:
	// Exposing these specifically so we can get access to calibration curve data ... 
	CDlgBarControl m_DlgLeftCtrl;
	CDlgBarReview  m_DlgLeftRev;
	CDlgBarAcq     m_DlgLeftAcq;
	CDlgBarTop     m_DlgTop;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	void ChangeStatusText(LPCTSTR text);

	inline void DlgTop_SetName(CString name) { 	
		// c_wndDlgBarTop.GetDlgItem(IDC_SUBSEL)->SetWindowTextA(name.GetString());
		m_DlgTop.SetName(name);
	}

	inline void DlgTop_SetPPM(double ppm) {
		// CString tval;
		// tval.Format(_T("%.1f ppm"),ppm);
		// c_wndDlgBarTop.GetDlgItem(IDC_CONC)->SetWindowTextA(tval.GetString());
		CString tval(_T(__FUNCTION__ "oops"));
		m_DlgTop.SetPPM(tval,ppm);
	}

	inline void DlgTop_SetPPM(CString ppmstr) {
		// c_wndDlgBarTop.GetDlgItem(IDC_CONC)->SetWindowTextA(ppmstr.GetString());
		CString tval(_T(__FUNCTION__ "oops"));
		m_DlgTop.SetPPM(tval,-123456789);
	}

	inline void DlgTop_SetPPMOK() {
		// c_colConc.SetTextColor(RGB(  0,255,  0));
		m_DlgTop.SetPPMOK();
	}
	inline void DlgTop_SetPPMFail() {
		// c_colConc.SetTextColor(RGB(255,  0,  0));
		m_DlgTop.SetPPMFail();
	}
#if 0 // We need to either fix the wrapper or remove it ... 
	inline void DlgLeft_Hide() {
		ShowControlBar(&c_wndDlgBarLeft,FALSE,FALSE);
	}
	inline void DlgLeft_Show() {
		ShowControlBar(&c_wndDlgBarLeft,TRUE,FALSE);
	}
#endif

	inline void DlgTop_Hide() {
		// ShowControlBar(&c_wndDlgBarTop,FALSE,FALSE);
		ShowControlBar(&m_DlgTop,FALSE,FALSE);
	}
	inline void DlgTop_Show() {
		// ShowControlBar(&c_wndDlgBarTop,TRUE,FALSE);
		ShowControlBar(&m_DlgTop,TRUE,FALSE);
	}

	inline void DlgLeftCtrl_Show() {
		m_DlgLeftCtrl.Populate();
		ShowControlBar(&m_DlgLeftCtrl,TRUE,FALSE);
	}
	inline void DlgLeftCtrl_Hide() {
		ShowControlBar(&m_DlgLeftCtrl,FALSE,FALSE);
	}
	inline bool DlgLeftCtrl_Reset() {
		return m_DlgLeftCtrl.reset();
	}

	inline void DlgLeftRev_Show() {
		m_DlgLeftRev.Populate();
		ShowControlBar(&m_DlgLeftRev,TRUE,FALSE);
	}
	inline void DlgLeftRev_Hide() {
		ShowControlBar(&m_DlgLeftRev,FALSE,FALSE);
	}

	inline void DlgLeftAcq_Show() {
		// m_DlgLeftAcq.Populate();
		ShowControlBar(&m_DlgLeftAcq,TRUE,FALSE);
	}
	inline void DlgLeftAcq_Hide() {
		ShowControlBar(&m_DlgLeftAcq,FALSE,FALSE);
	}
	inline bool DlgLeftAcq_Reset() {
		return m_DlgLeftAcq.reset();
	}

#if 0 // now in class
	afx_msg void DlgLeft_OnBnClickedBnAcq();
	afx_msg void DlgLeft_MockupRed();
	afx_msg void DlgLeft_MockupGreen();
	afx_msg void DlgLeft_OnBnClickedBnAcqCl();
	afx_msg void DlgLeft_OnBnClickedBnAcqScal();
	afx_msg void DlgLeft_OnBnClickedBnAcqBlank();
	afx_msg void DlgLeft_OnBnClickedBnAcqScan();
	afx_msg void DlgLeft_OnBnClickedBnAcqSave();
	afx_msg void DlgLeft_OnBnClickedBnAcq_Handler(int stage);
#endif
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};


