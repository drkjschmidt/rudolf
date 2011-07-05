// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "sditest.h"
#include "ColorEdit.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_ACQUIRE, &CMainFrame::LeftBar_OnBnClickedAcquire)
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
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

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

	// @@RP: added test based on ctrlbars to see if we can get a 
	// form control sort of thing to work ...
	if (!m_wndDlgBar.Create(this,IDD_TESTBAR,
		CBRS_TOP,IDD_TESTBAR))
	{
		TRACE0("Failed to create test bar\n");
		return -1;      // fail to create
	}
	// Simple solution ... 
	// m_wndDlgBar.GetDlgItem(IDC_SUBSEL)->SetWindowTextA(_T("Testing substance XYZ"));

	// Solution according to: http://www.functionx.com/visualc/applications/chart1.htm
	//	CEdit *pED = reinterpret_cast<CEdit *>(m_wndDlgBar.GetDlgItem(IDC_SUBSEL));
	//	pED->SetWindowTextA(_T("CCCCrash"));

	// Need to define in class so it's static ... 
// m_myButton.SubclassDlgItem(IDC_BUTTON1, this);
	c_colSubst.SubclassDlgItem(IDC_SUBSEL,(CWnd *)(&m_wndDlgBar));
	c_colSubst.SetBkColor(RGB(255,0,0));
	c_colSubst.SetTextColor(RGB(255,255,255));
	c_colSubst.SetReadOnly(true);
	CFont tmpFont;
	tmpFont.CreatePointFont(200,_T("Arial"));
	m_wndDlgBar.GetDlgItem(IDC_SUBSEL)->SetWindowTextA(_T("Testing substance XYZ"));
	m_wndDlgBar.GetDlgItem(IDC_SUBSEL)->SetFont(&tmpFont);
	tmpFont.Detach();

	// @@RP: added test based on ctrlbars to see if we can get a 
	// form control sort of thing to work ...
	if (!m_wndDlgBar2.Create(this,IDD_TESTBAR2,
		CBRS_LEFT,IDD_TESTBAR2))
	{
		TRACE0("Failed to create test bar 2\n");
		return -1;      // fail to create
	}


//	bigFont.CreatePointFont(200,_T("Arial"));
//	bigFont.CreatePointFont(200,_T("Arial"));
//	m_wndDlgBar.GetDlgItem(IDC_SUBSEL)->SetWindowTextA(_T("Testing substance XYZ"));
//	m_wndDlgBar.GetDlgItem(IDC_SUBSEL)->SetFont(&bigFont);



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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
	m_wndStatusBar.SetWindowTextA(text);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	switch (pMsg->message) {
		case WM_SIZE:
		case WM_SIZING:
		case WM_MOVE:
		case WM_MOVING:
		case WM_WINDOWPOSCHANGING:
		case WM_WINDOWPOSCHANGED:
			((CMainFrame *)AfxGetMainWnd())->ChangeStatusText("MainFrame: Window resized");
			break;	
		default:
			break;
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::LeftBar_OnBnClickedAcquire()
{
	// TODO: Add your control notification handler code here
	CString tval;
	m_wndDlgBar2.GetDlgItem(IDC_ETIME)->GetWindowTextA(tval);

	MessageBox(tval.GetString());
}
