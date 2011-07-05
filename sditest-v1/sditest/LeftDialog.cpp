// LeftDialog.cpp : implementation file
//

#include "stdafx.h"
#include "sditest.h"
#include "LeftDialog.h"


// CLeftDialog dialog

IMPLEMENT_DYNAMIC(CLeftDialog, CInitDialogBar)

BEGIN_MESSAGE_MAP(CLeftDialog, CInitDialogBar)
	ON_BN_CLICKED(IDC_ACQUIRE, &CLeftDialog::OnBnClickedAcquire)
END_MESSAGE_MAP()


CLeftDialog::CLeftDialog()
{
	// TODO: custom constructor code
}

CLeftDialog::~CLeftDialog()
{
}



BOOL CLeftDialog::OnInitDialogBar()
{
	// Support for DDX mechanism
	// If you do not want DDX then
	// do not call base class
	CInitDialogBar::OnInitDialogBar();

	// Update any controls NOT supported by DDX
	// CBitmapButton is one
	// m_OKButtton.AutoLoad(IDOK, this);

	return TRUE;
}

void CLeftDialog::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);

	CInitDialogBar::DoDataExchange(pDX);

	// DDX_??? functions to associate control with
	// data or control object
	// Call UpdateData(TRUE) to get data at any time
	// Call UpdateData(FALSE) to set data at any time

	//{{AFX_DATA_MAP(CAboutDlg)
	//DDX_Check(pDX, IDC_CHECK1, m_CheckBox);
	//}}AFX_DATA_MAP
}



// CLeftDialog message handlers

void CLeftDialog::OnBnClickedAcquire()
{
	// TODO: Add your control notification handler code here
	MessageBox(_T("Acq button clicked"));
}

