// AutoRepeatButton.cpp : implementation file
//

#include "stdafx.h"
// #include "AutoRepeat.h"
#include "AutoRepeatButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoRepeatButton

CAutoRepeatButton::CAutoRepeatButton()
{
}

CAutoRepeatButton::~CAutoRepeatButton()
{
}


BEGIN_MESSAGE_MAP(CAutoRepeatButton, CButton)
	//{{AFX_MSG_MAP(CAutoRepeatButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoRepeatButton message handlers
#define INITIAL_DELAY 50
#define MAX_DELAY 200
#define MIN_DELAY  10
#define DEC_DELAY  10
#define IDT_TIMER 1

void CAutoRepeatButton::OnLButtonDown(UINT nFlags, CPoint point) 
   {
    sent = 0;
	delay=MAX_DELAY;
    SetTimer(IDT_TIMER, INITIAL_DELAY, NULL);
    CButton::OnLButtonDown(nFlags, point);
   }

void CAutoRepeatButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
    KillTimer(IDT_TIMER);
	
	if(GetCapture() != NULL)
	{ /* release capture */
		ReleaseCapture();
	if(sent == 0 && (GetState() & BST_PUSHED) != 0)
	   GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
	} /* release capture */
       
	   
    
    //CButton::OnLButtonUp(nFlags, point);
}

void CAutoRepeatButton::OnTimer(UINT nIDEvent) 
   {
    if( (GetState() & BST_PUSHED) == 0)
       return;
	delay-=DEC_DELAY;
	delay=max(delay,MIN_DELAY);
    SetTimer(IDT_TIMER, delay, NULL);
    GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
    sent++;
	
    CButton::OnTimer(nIDEvent);
   }
