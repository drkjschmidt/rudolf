#pragma once
#include "afxwin.h"


// CDlgBarControl dialog

class CDlgBarControl : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgBarControl)

public:
	CDlgBarControl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBarControl();

// Dialog Data
	enum { IDD = ID_DLGBAR_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDlgLeftCtrlAcq();
	BOOL InitDialog(void);
};
