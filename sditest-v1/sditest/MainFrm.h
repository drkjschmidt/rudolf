// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "ColorEdit.h"
#include "InitDialogBar.h"

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
	CDialogBar	m_wndDlgBar;
	CInitDialogBar	m_wndDlgBar2;
	CFont		bigFont;
	CColorEdit	c_colSubst;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	void ChangeStatusText(LPCTSTR text);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void LeftBar_OnBnClickedAcquire();
};


