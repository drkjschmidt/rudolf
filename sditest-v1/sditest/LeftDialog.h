#pragma once

#include "InitDialogBar.h"

// CLeftDialog dialog

class CLeftDialog : public CInitDialogBar
{
	DECLARE_DYNAMIC(CLeftDialog)

public:
	CLeftDialog();   // standard constructor
	virtual ~CLeftDialog();

// Dialog Data
	enum { IDD = IDD_TESTBAR2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialogBar();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAcquire();



};
