#pragma once
#include "afxwin.h"
#include "Lock.h"
#include "resource.h"
#include "CalibrationCurve.h"

// CDlgBarControl dialog

class CDlgBarControl : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgBarControl)

public:
	// CDlgBarControl(CWnd* pParent = NULL);   // standard constructor
	CDlgBarControl();   // standard constructor
	virtual ~CDlgBarControl();

// Dialog Data
	enum { IDD = IDD_DBAR_LEFT_CONTROL };

	enum {
		POP_ALL      = 0xFFFF,
		POP_NONE     = 0x0000,
		POP_SUBNAME  = 0x0001,
		POP_SPECNAME = 0x0002,
		POP_ITIME    = 0x0004,
		POP_AVG      = 0x0008,
		POP_BOXW     = 0x0010,
		POP_LISTHDR  = 0x0020,
		POP_LISTDATA = 0x0040,
		POP_WAVELEN  = 0x0080,
		POP_BESTFIT  = 0x0100,
	};

	void Hide();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// according to http://support.microsoft.com/kb/185672
	afx_msg LONG OnInitDialog (UINT, LONG );

	DECLARE_MESSAGE_MAP()
public:
	//! Create the DialogBar
	//
	//! Unfortunately we don't necessarily have the global data variables initialized
	//! at this point so populating the fields has to happen separately using Populate()
	BOOL InitDialog(void);

	//! Populate DialogBar values
	void Populate(int selector=CDlgBarControl::POP_ALL);

	void Readback(void);

	bool reset(bool force=false);
	//! Copy data from dialog bar to calibration curve
	//
	//! This is a helper function that should be called for the purpose of saving data
	//! to the calibration curve data structure
	void saveState();
	//! Make sure the contents of the calibration curve and the display match. 
	//
	//! This is used to verify that we can navigate away / load / save without 
	//! losing data.
	bool isStateChanged();


	afx_msg void OnBnClickedDlgLeftCtrlAcqAccelF1();
	afx_msg void OnBnClickedDlgLeftCtrlAcqAccelF2();
	afx_msg void OnBnClickedDlgLeftCtrlAcqAccel(long selector);
	afx_msg void OnBnUpdatedDlgLeftCtrlAcq0(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedDlgLeftCtrlAcq0();

	afx_msg void OnBnUpdatedDlgLeftCtrlAcq(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedDlgLeftCtrlAcq();

	afx_msg void OnBnUpdatedDlgLeftCtrlDel(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedDlgLeftCtrlDel();

	afx_msg void OnBnUpdatedDlgLeftCtrlSave(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedDlgLeftCtrlSave();

	afx_msg void OnBnClickedDlgLeftCtrlLoad();

	afx_msg void OnBnClickedDlgLeftCtrlReset();

	afx_msg void OnEnChangeCombo7();

protected:
	CEdit c_substanceName;
	// CString m_substanceName;
	CComboBox c_specName;
	CComboBox c_integrationTime;
	CComboBox c_scansToAverage;
	CComboBox c_boxcarWidth;
	CListCtrl c_listControl;
	CEdit c_concAVal;
	CComboBox c_wavelenSelect;

	CComboBox c_errLow;
	CComboBox c_errHigh;
	CComboBox c_warnLow;
	CComboBox c_warnHigh;

	CString m_textFormula;
	CString m_textRSquared;

	bool s_BnDlgLeftCtrlAcq0;
	bool s_BnDlgLeftCtrlAcq;
	bool s_BnDlgLeftCtrlDel;
	bool s_BnDlgLeftCtrlLoad;
	bool s_BnDlgLeftCtrlSave;

	// Keep track of whether we have made relevant changes in the calibration curve
	CString t_substanceName;
	CString t_specName;
	CString t_integrationTime;
	CString t_scansToAverage;
	CString t_boxcarWidth;
	// CString t_concAVal;
	CString t_wavelenSelect;

	int t_numSpecs;

public:
	// Calibration curve used in Control mode only ... 
	// ... we are specifically exposing this calibration curve so
	// other functions can access it!
	CCalibrationCurve CalDataCtrl;

public:
	afx_msg void OnEnKillfocusEditSubname();
	afx_msg void OnEnChangeEditSubname();

private:
	CDMLock *pLock;
	volatile bool interlock;
};
