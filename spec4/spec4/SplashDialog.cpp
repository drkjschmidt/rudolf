// SplashDialog.cpp : implementation file
//

#include "stdafx.h"
#include "spec4.h"
#include "SplashDialog.h"

#include "MainFrm.h"
#include <limits>

#define WIDTH 390
#define HEIGHT 260
#define BXW 145
#define BXH 20

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
	DDX_Control(pDX, IDC_PROGRESS3, c_splashProgress);
}


BEGIN_MESSAGE_MAP(CSplashDialog, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSplashDialog message handlers

bool CSplashDialog::ShowSplashScreen(CWnd* pParentWnd,double splashTime,double splashStep)
{
	// Can't have multiple splash screens at the same time
	if (c_pSplashDialog != NULL)
		return false;

	c_pSplashDialog = new CSplashDialog;

	if (! c_pSplashDialog->Create(CSplashDialog::IDD, pParentWnd))
	{
		delete c_pSplashDialog;
		return false;
	} 
	else 
	{
		// Need to do setup work in separate function because
		// this is a static function ... 
		c_pSplashDialog->SetupTimerBar(splashTime,splashStep);
		c_pSplashDialog->ShowWindow(SW_SHOW);
		c_pSplashDialog->UpdateWindow();

	}
	return true;
}

bool CSplashDialog::SetupTimerBar(double splashTime,double stepTime)
{
	// Need to have a splash to set this up
	if (c_pSplashDialog == NULL)
		return false;

	bool showSplash=true;

	if (stepTime==0) {
		showSplash=false;
		stepTime=1;
	}

	int splashTimeInt,stepTimeInt;
	if (splashTime < INT_MAX) {
		splashTimeInt=(int)splashTime;
		stepTimeInt=(int)stepTime;
	}
	else {
		// we aim for 1s steps ... 
		stepTimeInt=(int)( (splashTime/INT_MAX) * stepTime + 1);
		splashTimeInt=(int)(splashTime / stepTimeInt);
	}
	if (showSplash)
	{
		c_splashProgress.SetWindowPos(
			&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		c_splashProgress.MoveWindow(WIDTH-BXW-5, HEIGHT-BXH-25,BXW,BXH);
		c_splashProgress.SetRange32(0,splashTimeInt);
		c_splashProgress.SetStep(stepTimeInt);

		c_splashProgress.ShowWindow(SW_SHOW);
		m_counterTimeout=c_pSplashDialog->SetTimer(1,splashTimeInt,NULL);
		m_counterTicks  =c_pSplashDialog->SetTimer(2,stepTimeInt,NULL);
		// m_counterTicks  =c_pSplashDialog->SetTimer(2,1000,NULL);

	}
	else
	{
		c_splashProgress.ShowWindow(SW_HIDE);
		m_counterTimeout=c_pSplashDialog->SetTimer(1,splashTimeInt,NULL);
		m_counterTicks  =c_pSplashDialog->SetTimer(2,splashTimeInt,NULL); // don't need it so make it slow
	}

	
	return true;
}

bool CSplashDialog::HideSplashScreen(void)
{
	// Need to have a splash to close
	if (c_pSplashDialog == NULL)
		return false;

	c_pSplashDialog->KillTimer(1);
	c_pSplashDialog->KillTimer(2);
	// @@@ did this somehow kill the ability to manually kill the window?
	c_pSplashDialog->DestroyWindow();
	AfxGetMainWnd()->UpdateWindow();
	delete c_pSplashDialog;
	c_pSplashDialog = NULL;

	return true;
}

bool CSplashDialog::HideSplashScreenInternal(void)
{
	// Need to have a splash to close
	if (c_pSplashDialog == NULL)
		return false;

	c_pSplashDialog->KillTimer(1);
	c_pSplashDialog->KillTimer(2);
	// @@@ did this somehow kill the ability to manually kill the window?
	DestroyWindow();
	AfxGetMainWnd()->UpdateWindow();
	delete c_pSplashDialog;
	c_pSplashDialog = NULL;

	return true;
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
			c_pSplashDialog->HideSplashScreenInternal();
			// ((CMainFrame *)AfxGetMainWnd())->ChangeStatusText("Still Here?!?");
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
	
	if (nIDEvent == m_counterTimeout) 
	{
		HideSplashScreenInternal();
	}
	if (nIDEvent == m_counterTicks) 
	{
		c_splashProgress.StepIt();
	}
}

BOOL CSplashDialog::OnInitDialog()
{
	RECT myrect,bxrect;

	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
#define AUTOCENTER 0

#if AUTOCENTER == 0
	// This doesn't scale so set up resources ... 
	// CWnd::wndTopMost.GetWindowRect(&myrect);
	AfxGetApp()->m_pMainWnd->GetWindowRect(&myrect);

	// SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	SetWindowPos(&CWnd::wndTopMost,
		(myrect.left + myrect.right  - (myrect.left>myrect.right?-1:1)*WIDTH)/2,
		(myrect.top  + myrect.bottom - (myrect.top>myrect.bottom?-1:1)*HEIGHT)/2,
		WIDTH,
		HEIGHT,
		0);
#else
	SetWindowPos(&CWnd::wndTopMost,0,0,504,360,SWP_NOMOVE);
	// @@@ will this be with respect to the parent?
	// @@@ For some reason calling CenterWindow kills WM messages
	//     and causes crashes?!?
	// CenterWindow();
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
#endif


	// GetWindowRect(&bxrect);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

static CSplashDialog *c_pSplashDialog;
static UINT_PTR m_counterTimeout;
static UINT_PTR m_counterTicks;
