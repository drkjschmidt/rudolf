#pragma once
#include "ColorEdit.h"

// CDlgBarTop dialog

class CDlgBarTop : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgBarTop)

public:
	// CDlgBarTop(CWnd* pParent = NULL);   // standard constructor
	CDlgBarTop();
	virtual ~CDlgBarTop();

// Dialog Data
	enum { IDD = IDD_DBAR_TOP };

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

	CColorEdit c_textConc;
	CColorEdit c_textSubst;

	enum PPMColors {
		PPMColGood = RGB(  0,255,  0),
		PPMColWarn = RGB(255,150,  0),
		PPMColErr  = RGB(255,  0,  0),
	};

	inline void SetPPMOK() {
			c_textConc.SetTextColor(RGB(  0,255,  0));
	}

	inline void SetPPMFail() {
			c_textConc.SetTextColor(RGB(255,  0,  0));
	}

	inline void SetName(CString name) { 	
		c_textSubst.SetWindowTextA(name.GetString());
	}

	//! Set the concentration value string.
	//
	//! It would seem simpler to just pass the ppm value and do 
	//! the string formatting here, but then we would be duplicating
	//! functionality from CDlgBarAcq and that's a recipe for trouble
	//
	//! 2011-12 Ken pointed out that the coloring for being outside the sensitivity 
	//! range should be done on the measured value, not on the pre-dilution
	//! value!
	inline void SetPPM(CString ppmstr,double ppm_msd) {
		CString tval;
		COLORREF setcol;

		setcol=PPMColGood;
		if ((warn_low!=warn_high) && ((ppm_msd<warn_low) || (ppm_msd>warn_high)))
			setcol=PPMColWarn;
		if ((err_low!=err_high) && ((ppm_msd<err_low) || (ppm_msd>err_high)))
			setcol=PPMColErr;
		if (setcol != curcol) {
			curcol=setcol;
			c_textConc.SetTextColor(setcol);
		}

		c_textConc.SetWindowTextA(ppmstr.GetString());
	}

	inline void   setWarnLow(double set) {        warn_low=set; }
	inline double getWarnLow()           { return warn_low;     }
	inline void   setWarnHigh(double set) {        warn_high=set; }
	inline double getWarnHigh()           { return warn_high;     }

	inline void   setErrLow(double set) {        err_low=set; }
	inline double getErrLow()           { return err_low;     }
	inline void   setErrHigh(double set) {        err_high=set; }
	inline double getErrHigh()           { return err_high;     }


protected:
	CSize CalcFixedLayout(BOOL bStretch,BOOL bHorz);
	double warn_low, warn_high;
	double err_low, err_high;
	COLORREF curcol;

};
