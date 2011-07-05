// DlgBarTop.cpp : implementation file
//

#include "stdafx.h"
#include "spec4.h"
#include "DlgBarTop.h"


// CDlgBarTop dialog

IMPLEMENT_DYNAMIC(CDlgBarTop, CDialogBar)

CDlgBarTop::CDlgBarTop()
: warn_low(0)
, warn_high(0)
, err_low(0)
, err_high(0)
, curcol(PPMColGood)
{
}

CDlgBarTop::~CDlgBarTop()
{
}

void CDlgBarTop::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgBarTop, CDialogBar)
	// according to http://support.microsoft.com/kb/185672
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
END_MESSAGE_MAP()


// CDlgBarTop message handlers
LONG CDlgBarTop::OnInitDialog ( UINT wParam, LONG lParam)
{
	
	BOOL bRet = HandleInitDialog(wParam, lParam);

	if (!UpdateData(FALSE))
	{
	   TRACE0("Warning: UpdateData failed during dialog init.\n");
	}

	CFont tmpFont;
	tmpFont.CreatePointFont(200,_T("Arial"));

	c_textConc.SubclassDlgItem(IDC_CONC,(CWnd *)(this));
	c_textConc.SetBkColor(  RGB(  0,  0,  0));
	c_textConc.SetTextColor(RGB(  0,255,  0));
	c_textConc.SetReadOnly(true);
	c_textConc.SetWindowTextA(_T("[no conc msd]"));
	c_textConc.SetFont(&tmpFont);

	c_textSubst.SubclassDlgItem(IDC_SUBSEL,(CWnd *)(this));
	c_textSubst.SetBkColor(  RGB(  0,  0,  0));
	c_textSubst.SetTextColor(RGB(255,255,255));
	c_textSubst.SetReadOnly(true);
	c_textSubst.SetWindowTextA(_T("[no calibration loaded]"));
	c_textSubst.SetFont(&tmpFont);

	// We need to make sure the font definition remains in place ...
	tmpFont.Detach();

	return bRet;
}

BOOL CDlgBarTop::InitDialog(void)
{
	// UpdateData(FALSE);
	// SetWindowText("Wzd Dialog Bar");
	return TRUE;
}

CSize CDlgBarTop::CalcFixedLayout(BOOL bStretch,BOOL bHorz)
{
	// http://support.microsoft.com/kb/143255

	// we ignore the booleans because we know that this dialog can only be horizontal 
	// and docked ... so what do we return?
	RECT twrect,srect;
	CSize mysize;

	AfxGetApp()->m_pMainWnd->GetWindowRect(&twrect);

	mysize.SetSize(twrect.right-twrect.left, m_sizeDefault.cy);

	// @@@ I really don't like this but I can't see anywhere else to put this ...
	// ... I also have no idea *why* this works but apparently it does ;)
	c_textSubst.GetWindowRect(&srect);
	srect.right=twrect.right-16;

	ScreenToClient(&srect);
	c_textSubst.MoveWindow(&srect);

	return mysize;
}
