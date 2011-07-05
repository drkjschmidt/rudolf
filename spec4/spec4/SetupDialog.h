#pragma once
#include "afxwin.h"


// CSetupDialog dialog

class CSetupDialog : public CDialog
{
	DECLARE_DYNAMIC(CSetupDialog)

public:
	CSetupDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox c_SetupGPS;
	CButton c_TestGPS;
	CString m_GPSName;
	CString m_GPSlat;
	CString m_GPSlon;
	CString m_GPSalt;
	CComboBox c_SelectSpec;
	CStatic c_SpecFrame;
	afx_msg void OnBnClickedTestSpectrometer();
	afx_msg void OnCbnSelchangeSelectSpectrometer();
	CString m_SelectSpec;
	afx_msg void OnBnClickedTestGps();
protected:
	virtual void OnOK();
public:
	CButton c_requireGPS;
	CButton c_requireSpec;
	CButton c_autosaveSpec;
	CEdit c_passwordText;
};
