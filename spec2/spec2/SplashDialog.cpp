// SplashDialog.cpp : implementation file
//

#include "stdafx.h"
#include "spec2.h"
#include "SplashDialog.h"

#include "MainFrm.h"

// CSplashDialog dialog

IMPLEMENT_DYNAMIC(CSplashDialog, CDialog)

CSplashDialog::CSplashDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDialog::IDD, pParent)
{

}

CSplashDialog::~CSplashDialog()
{
}

void CSplashDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplashDialog, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSplashDialog message handlers

void CSplashDialog::ShowSplashScreen(CWnd* pParentWnd)
{
	c_pSplashDialog = new CSplashDialog;

	if (! c_pSplashDialog->Create(CSplashDialog::IDD, pParentWnd))
	{
		delete c_pSplashDialog;
	} 
	else 
	{
		c_pSplashDialog->ShowWindow(SW_SHOW);
		c_pSplashDialog->UpdateWindow();

		c_pSplashDialog->SetTimer(1,10000,NULL);
	}
}

void CSplashDialog::HideSplashScreen(void)
{
	c_pSplashDialog->KillTimer(1);
	DestroyWindow();
	AfxGetMainWnd()->UpdateWindow();
	delete c_pSplashDialog;
	c_pSplashDialog = NULL;
}

bool CSplashDialog::PreTranslateAppMessage(MSG* pMsg)
{
	
	if (c_pSplashDialog == NULL)
		return false;

	if (c_pSplashDialog->m_hWnd == NULL)
		return false;

	switch (pMsg->message) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
			c_pSplashDialog->HideSplashScreen();
			((CMainFrame *)AfxGetMainWnd())->ChangeStatusText("Still Here?!?");
			return true;
			break;		
		default:
			return false;
			break;
	}

	return false;
}

void CSplashDialog::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
	
	// TODO: Add your message handler code here and/or call default
	HideSplashScreen();

	
}

BOOL CSplashDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CenterWindow();
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
