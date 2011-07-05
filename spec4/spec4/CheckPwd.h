#pragma once
#include "afxwin.h"


// CCheckPwd dialog

class CCheckPwd : public CDialog
{
	DECLARE_DYNAMIC(CCheckPwd)

public:
	CCheckPwd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckPwd();

// Dialog Data
	enum { IDD = IDD_DIALOG_PWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit c_pwdBox;
	virtual BOOL OnInitDialog();
	CString getPass(void);
protected:
	virtual void OnOK();
public:
	CString pwdString;
};
