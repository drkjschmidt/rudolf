// DlgBarControl.cpp : implementation file
//

#include "stdafx.h"
#include "dlgbartest.h"
#include "DlgBarControl.h"


// CDlgBarControl dialog

IMPLEMENT_DYNAMIC(CDlgBarControl, CDialogBar)

CDlgBarControl::CDlgBarControl(CWnd* pParent /*=NULL*/)
//	: CDialogBar(CDlgBarControl::IDD, pParent)
{

}

CDlgBarControl::~CDlgBarControl()
{
}

void CDlgBarControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgBarControl, CDialogBar)
	ON_BN_CLICKED(IDC_DLG_LEFT_CTRL_ACQ, &CDlgBarControl::OnBnClickedDlgLeftCtrlAcq)
	ON_COMMAND(ID_ACCELERATOR32772, &CDlgBarControl::OnBnClickedDlgLeftCtrlAcq)
END_MESSAGE_MAP()


// CDlgBarControl message handlers

void CDlgBarControl::OnBnClickedDlgLeftCtrlAcq()
{
	CString tval;

	// TODO: Add your control notification handler code here
	this->GetDlgItem(IDC_EDIT2)->GetWindowTextA(tval);
	MessageBox(_T("Clicked acquisition button"),tval.GetString());
}

BOOL CDlgBarControl::InitDialog(void)
{
	UpdateData(FALSE);
	SetWindowText("Wzd Dialog Bar");
	return TRUE;
}

