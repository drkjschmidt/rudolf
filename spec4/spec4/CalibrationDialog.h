#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "StaticRefreshCalibration.h"
#include "StaticRefreshSpectra.h"

// use this for old version of dialog
// #define CD_USE_THREE 1

// CCalibrationDialog dialog

class CCalibrationDialog : public CDialog
{
	DECLARE_DYNAMIC(CCalibrationDialog)

public:
	CCalibrationDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCalibrationDialog();

// Dialog Data
#ifdef CD_USE_THREE
	enum { IDD = IDD_DIALOG2 };
#else
	// enum { IDD = IDD_DIALOG3 };
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox c_specName;
	CString m_substanceName;
	CComboBox c_integrationTime;

#ifdef CD_USE_THREE
	// removed when changing to list
	CString m_conc_1;
	CString m_conc_2;
	CString m_conc_3;
	CComboBox c_conc_unit_1;
	CComboBox c_conc_unit_2;
	CComboBox c_conc_unit_3;
	
	CEdit c_conc_1;
	CEdit c_conc_2;
	CEdit c_conc_3;
	
	void OnBnClicked_CalButton(int idx);
	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
#else
	CListCtrl c_listControl;
#endif

	CComboBox c_wavelenSelect;
	CComboBox c_calibrationType;

	CStaticRefreshSpectra c_frameSpectrum;
	CStaticRefreshCalibration c_frameCalibration;
	CString m_integrationTime;
	CEdit c_concAVal;
	CComboBox c_concAUnit;
	afx_msg void OnBnClickedButtonAddPoint();
	afx_msg void OnBnClickedButtonDeletePoint();
	afx_msg void OnCbnSelchangeFitType();
	afx_msg void OnEnChangeWavelen();
protected:
	virtual void OnOK();

public:
	CComboBox c_scansToAverage;
	CComboBox c_boxcarWidth;
};
