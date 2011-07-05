// CheckPwd.cpp : implementation file
//

#include "stdafx.h"
#include "spec4.h"
#include "CheckPwd.h"


// CCheckPwd dialog

IMPLEMENT_DYNAMIC(CCheckPwd, CDialog)

CCheckPwd::CCheckPwd(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckPwd::IDD, pParent)
	, pwdString(_T(""))
{

}

CCheckPwd::~CCheckPwd()
{
}

void CCheckPwd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PWD, c_pwdBox);
}


BEGIN_MESSAGE_MAP(CCheckPwd, CDialog)
END_MESSAGE_MAP()


// CCheckPwd message handlers

BOOL CCheckPwd::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// c_pwdBox.SetFocus();
	// SendMessage(WM_NEXTDLGCTL,(WPARAM)c_pwdBox.m_hWnd,TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CString CCheckPwd::getPass(void)
{
	return pwdString;
}

void CCheckPwd::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::OnOK();

	c_pwdBox.GetWindowTextA(pwdString);
}
