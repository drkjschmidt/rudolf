#pragma once
#include "afxwin.h"
#include <vector>
#include "CustomerInfoData.h"
#include "RPString.h"

// CCustomerInfoDialog dialog

class CCustomerInfoDialog : public CDialog
{
	DECLARE_DYNAMIC(CCustomerInfoDialog)

public:
	CCustomerInfoDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCustomerInfoDialog();


// Dialog Data
	enum { IDD = IDD_DIALOG_CUSTINFO };

	enum mode {
		locinfo = 0x01, // Enter data that will be remembered for *all* samples
		sampinfo = 0x02, // modify data PER SAMPLE
	};

	inline void setMode(int smode) { initMode=smode; };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_gpsLoc;
	CComboBox c_compName;
	CComboBox c_sampleLoc;
	CEdit c_sampleInfo;

	CEdit c_sampleNumPipeKm;
	CEdit c_sampleNumDilution;
	CEdit c_sampleNumUserdef;

	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedSave();
	virtual BOOL OnInitDialog();

	inline CString getNameCust() { return nameCust; }
	inline CString getNameLoc()  { return nameLoc; }
	inline CString getNameSamp() { return nameSamp; }

protected:
	// double lat,lon,alt;
	// INT32 time,date;
	CGPSData acqGPSInfo;

	CString nameCust,nameLoc,nameSamp;
	double dataPipeKm,dataDilution;
	CString dataFF1,dataFF2;


	std::vector<CCustomerInfoData> locationArray;
	// vector<int> locationArray;
	virtual void OnOK();
	afx_msg void OnCbnSelchangeCompname();

	int initMode;

public:
	CButton c_okMeasureButton;
};
