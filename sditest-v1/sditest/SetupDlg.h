#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSetupDlg dialog

class CSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupDlg();

// Dialog Data
	enum { IDD = IDD_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_SetupComPort;
	CComboBox c_SetupComPort;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_baudRate;
	CEdit c_baudRate;
	CString m_baudRateStatic;
//	afx_msg void OnCbnSelchangeCombo2();
	CString m_DataPort;
//	afx_msg void OnCbnEditchangeCombo2();
//	afx_msg void OnCbnEditupdateCombo2();
//	afx_msg void OnCbnCloseupCombo2();
	afx_msg void OnBnClickedButton1();
	CString m_lat;
	CString m_lon;
	CString m_alt;
	CString m_gprmc;
	CString m_gpgga;
	CString m_pgrmt;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl c_listCtrl;
};
