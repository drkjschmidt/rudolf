// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "spec4.h"

#include "MainFrm.h"
#include "CustomerInfoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Standard MFC toolbar
	// @@@ Suppress toolbar ... how? I think if we don't *create* it will crash
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// Standard MFC status bar, we do want this
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	// Create and HIDE Top dialogbar
	if (!m_DlgTop.Create(this,IDD_DBAR_TOP,CBRS_TOP,IDD_DBAR_TOP) ||
		!m_DlgTop.InitDialog())
	{
		TRACE0("FAILED to create dialog bar\n");
		return -1;
	}
	ShowControlBar(&m_DlgTop,FALSE,FALSE);


	// Create and HIDE Ctrl dialogbar
	if (!m_DlgLeftCtrl.Create(this,IDD_DBAR_LEFT_CONTROL,CBRS_LEFT,IDD_DBAR_LEFT_CONTROL) ||
		!m_DlgLeftCtrl.InitDialog())
	{
		TRACE0("FAILED to create dialog bar\n");
		return -1;
	}
	// if (theApp.getRunmode() != RUNMODE_CONFIGURATION)
		ShowControlBar(&m_DlgLeftCtrl,FALSE,FALSE);

	// For some reason this fails ... http://social.msdn.microsoft.com/Forums/en-US/vclanguage/thread/d47ec490-32f8-4f53-8b17-7dc47cbdab0a
	// suggests this might be a threading issue but I don't see where there is a new thread?
	//
	// ... followed the ASSERT into winfrm.cpp ... looks like MFC doesn't see the class
	//     as related to its parent window?!?
	// m_DlgLeftCtrl.Hide();

	// Following the example in the book this allows undocking the window
	// m_DlgLeftCtrl.EnableDocking(CBRS_ALIGN_ANY);
	// EnableDocking(CBRS_ALIGN_ANY);
	// DockControlBar(&m_DlgLeftCtrl);

	// Create and HIDE Rev dialogbar
	if (!m_DlgLeftRev.Create(this,IDD_DBAR_LEFT_REV,CBRS_TOP,IDD_DBAR_LEFT_REV) ||
		!m_DlgLeftRev.InitDialog())
	{
		TRACE0("FAILED to create dialog bar\n");
		return -1;
	}
	ShowControlBar(&m_DlgLeftRev,FALSE,FALSE);


	// Create and HIDE Acq dialogbar
	if (!m_DlgLeftAcq.Create(this,IDD_DBAR_LEFT_ACQ,CBRS_LEFT,IDD_DBAR_LEFT_ACQ) ||
		!m_DlgLeftAcq.InitDialog())
	{
		TRACE0("FAILED to create dialog bar\n");
		return -1;
	}
	ShowControlBar(&m_DlgLeftAcq,FALSE,FALSE);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::ChangeStatusText(LPCTSTR text)
{
	if (::IsWindow(m_wndStatusBar.m_hWnd))
		m_wndStatusBar.SetWindowText(text);
}


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	
	if ((m_DlgLeftCtrl.m_hWnd != NULL) &&
		 m_DlgLeftCtrl.IsVisible() && 
		 m_DlgLeftCtrl.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return true;

	if ((m_DlgLeftRev.m_hWnd != NULL) &&
		 m_DlgLeftRev.IsVisible() && 
		 m_DlgLeftRev.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return true;

	if ((m_DlgLeftAcq.m_hWnd != NULL) &&
		 m_DlgLeftAcq.IsVisible() && 
		 m_DlgLeftAcq.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return true;

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}