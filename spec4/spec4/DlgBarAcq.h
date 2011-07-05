#pragma once

// #include "spec4.h"

// CDlgBarAcq dialog

class CDlgBarAcq : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgBarAcq)

public:
	// CDlgBarAcq(CWnd* pParent = NULL);   // standard constructor
	CDlgBarAcq();	
	virtual ~CDlgBarAcq();

// Dialog Data
	enum { IDD = IDD_DBAR_LEFT_ACQ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// according to http://support.microsoft.com/kb/185672
	afx_msg LONG OnInitDialog (UINT, LONG );

	DECLARE_MESSAGE_MAP()
public:
	//! Create the DialogBar
	//
	// Unfortunately we don't necessarily have the global data variables initialized
	// at this point so populating the fields has to happen separately using Populate()
	BOOL InitDialog(void);

	afx_msg void OnBnUpdatedBnAcqCl(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedBnAcqCl();

	afx_msg void OnBnUpdatedBnAcqScal(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedBnAcqScal();

	afx_msg void OnBnUpdatedBnAcqBlank(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedBnAcqBlank();

	afx_msg void OnBnUpdatedBnAcqScan(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedBnAcqScan();

	afx_msg void OnBnUpdatedBnAcqSave(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedBnAcqSave();

	void OnBnClickedBnAcq_Handler(int stage);

	inline bool isSaved() { return saved; }
	inline bool reset(bool force=false) {
		// run implicit save 
		if (! saved) {
			OnBnClickedBnAcq_Handler(DLG_LFT_RESET);
		}
		// check if save was successful
		if ((! saved) && (! force))
			return false;

		// reset dialog item values
		GetDlgItem(IDC_ST_ACQ_CUSTOMER)->SetWindowTextA(_T("[No customer info set]"));
		GetDlgItem(IDC_ST_ACQ_LOC)->SetWindowTextA(_T("[No location info set]"));
		GetDlgItem(IDC_ST_ACQ_GPS)->SetWindowTextA(_T("[No GPS info associated]"));
		GetDlgItem(IDC_ST_ACQ_SI)->SetWindowTextA(_T("[No sample name set]"));
		GetDlgItem(IDC_ST_ACQ_CN)->SetWindowTextA(_T("[No calibration curve loaded]"));
		GetDlgItem(IDC_ST_ACQ_CP)->SetWindowTextA(_T("[No calibration parameters]"));
		GetDlgItem(IDC_ST_ACQ_RES)->SetWindowTextA(_T("[No concentration determined]"));
		GetDlgItem(IDC_ST_ACQ_CN_F)->SetWindowTextA(_T("[No fit formula]"));
		GetDlgItem(IDC_ST_ACQ_CN_R)->SetWindowTextA(_T("[No rsquared]"));
		GetDlgItem(IDC_ST_ACQ_FN)->SetWindowTextA(_T("[No data saved]"));

		saved=true; laststage=DLG_LFT_RESET; 
		return true;
	}

	CString FormatPPM(double undilppm,double dilution);
	CString UpdatePPM();
	bool AcquireDataSpectrum(); 
	bool AcquireBaselineSpectrum();
	bool SaveAllSpectra(CString *fname);

	//! Paint data report ... this is a wrapper for a common function provided 
	//! by CSpectrometer
	//
	//! Currently this is used in an ugly three step wrap so maybe optimize?
	inline void PaintDReport(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect) {
		CSpectrumData DiffSpec;
		DiffSpec=LastSpec - BaselineSpec;

		DiffSpec.PaintReport(pDC,pInfo,myrect);
	}

	//! Paint data spectra
	void PaintDSpectra(CDC *pDC, LPRECT myrect);

	// Set paint properties for data spectra
	inline void paintoptSetBaseline(bool set) { paintopt_D_baseline=set; };
	inline bool paintoptGetBaseline(void) { return paintopt_D_baseline; };

	inline void paintoptSetBaselineOverride(bool set) { paintopt_D_baseline_ovr=set; };
	inline bool paintoptGetBaselineOverride(void) { return paintopt_D_baseline_ovr; };

	inline void paintoptSetRaw(bool set) { paintopt_D_raw=set; };
	inline bool paintoptGetRaw(void) { return paintopt_D_raw; };

	inline void paintoptSetDiff(bool set) { paintopt_D_diff=set; };
	inline bool paintoptGetDiff(void) { return paintopt_D_diff; };

	inline void getCursorInfo(int *nm_id,int wv_id,CString *name, double *wv_val,double *in_val) {
		int idmax=-1;
		if (BaselineSpec.is_valid() && LastSpec.is_valid()) { idmax=2; }
		else if (BaselineSpec.is_valid()) { idmax=0; }

		*nm_id=max(*nm_id,   -1);
		*nm_id=min(*nm_id,idmax);
		switch (*nm_id) {
			case 2:
				*name="Data"; 
				*wv_val=((LastSpec-BaselineSpec).getWavelengths().getDoubleValues())[wv_id];
				*in_val=((LastSpec-BaselineSpec).getIntensities().getDoubleValues())[wv_id];
				break;
			case 1:
				*name="Raw"; 
				*wv_val=(LastSpec.getWavelengths().getDoubleValues())[wv_id];
				*in_val=(LastSpec.getIntensities().getDoubleValues())[wv_id];
				break;
			case 0:
				*name="Baseline"; 
				*wv_val=(BaselineSpec.getWavelengths().getDoubleValues())[wv_id];
				*in_val=(BaselineSpec.getIntensities().getDoubleValues())[wv_id];
				break;
			default:
				*name="No Data";
				*wv_val=0;
				*in_val=0;
				break;
		}
	}



protected:
	// bool s_BnDlgLeftAcqCl;
	// bool s_BnDlgLeftAcqScal;
	// bool s_BnDlgLeftAcqBlank;
	// bool s_BnDlgLeftAcqScan;
	// bool s_BnDlgLeftAcqSave;

	int laststage;
	bool saved;

	// Baseline spectrum and last acquired spectrum
	CSpectrumData BaselineSpec;
	CSpectrumData LastSpec;

	//! Which spectra to draw ...
	bool paintopt_D_baseline;
	bool paintopt_D_baseline_ovr;
	bool paintopt_D_raw;
	bool paintopt_D_diff;

public:
	// Calibration curve used in Acquisition mode only ... 
	// ... we are specifically exposing this calibration curve so
	// other functions can access it!
	CCalibrationCurve CalDataAcq;
};
