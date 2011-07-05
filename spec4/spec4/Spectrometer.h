#pragma once

#include "MainFrm.h"
#include "conf_const.h"
#include "wrapper.h"
#include "SpectrumData.h"
#include "CalibrationCurve.h"
#include <vector>

// #define min(a,b) ((a<b)?(a):(b))
// #define max(a,b) ((a>b)?(a):(b))

class CSpectrometer
{
public:
	CSpectrometer(void);
	~CSpectrometer(void);

	inline int GetCurSpec() { return curSpectrometer; }
	inline int GetSpecCount() { return SpecCount; };
//	inline JString `(int index) {
//		// Not sure if this should check for legal index
//		// but also not sure how to create an empty JString in 
//		// case of failure ... so let's home the library function
//		// handles this ...
//		return pWrapper->getName(index);		
//	}
	inline CString GetSpecName(int index) {
		CString specname;
		if (index>=0 && index<SpecCount)
			specname=pWrapper->getName(index).getASCII();
		return specname;
	}
	inline CString GetSpecSerial(int index) {
		CString serial;
		if (index>=0 && index<SpecCount)
			serial=pWrapper->getSerialNumber(index).getASCII();
		return serial;
	}
	inline void GetWavelengths(DoubleArray *wavelengths) { *wavelengths=SpecWavelengths; }

	int SelectSpectrometer(CString name,CString serial=CString(_T("")));
	int SelectSpectrometer(int index);

	inline void SetIntegrationTime(double time) { parm_integration_time=time; }
	inline void SetBoxcarWidth(int width) { parm_boxcar_width=width; }
	inline void SetScansToAverage(int num) { parm_averaging_num=num; }
	inline void EnableElecticalDark(bool enable, bool reset) { 
		parm_electrical_dark_on=enable;
		parm_electrical_dark_reset=reset;
	}

	void ForkWrapperGetSpectrum(LPVOID param);

#if 0 // move this to DlgBarAcq
	bool Spectrum_AcquireDataSpectrum(); 
	bool Spectrum_AcquireBaselineSpectrum(); 
#endif

	bool Spectrum_AcquireSpectrum(DoubleArray *datastore); 

#if 0 // looks like we aren't using this anywhere ... 
	//! Force I/O on data spectrum
	inline bool Spectrum_LoadBaseline(CString *filename=NULL) { return BaselineSpec.Load(filename); }
	inline bool Spectrum_SaveBaseline(CString *filename=NULL) { return BaselineSpec.Save(filename); }

	//! Force I/O on data spectrum
	inline bool Spectrum_LoadSpectrum(CString *filename=NULL) { return LastSpec.Load(filename); }
	inline bool Spectrum_SaveSpectrum(CString *filename=NULL) { return LastSpec.Save(filename); }
#endif

	//! Save Blank / Data / Difference spectrum
	bool Spectrum_LoadAllSpectra(CString *filename=NULL);
	bool Spectrum_SaveAllSpectra(CString *filename=NULL);

#if 0
	inline void Spectrum_StoreComments(	
		CString *slocn, 
		CString *scust, 
		CString *scomm,
		CString *spikm,
		CString *sdilu,
		CString *sffm1,
		CString *sffm2) 
	{ 
		LastSpec.storeComments(slocn,scust,scomm,spikm,sdilu,sffm1,sffm2); 
	}
#endif

	inline double getMinW(void) { return wmin; }
	inline double getMaxW(void) { return wmax; }

	//! Draw a text report for current acquisition
	inline void PaintDReport(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect) { 
		((CMainFrame *)(AfxGetMainWnd()))->m_DlgLeftAcq.PaintDReport(pDC,pInfo,myrect);
	}

	//! Draw a text report for review spectrum
	void PaintRReport(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect);

	//! Draw text report for provided spectrum
	void PaintReport(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect,CSpectrumData ReportSpec);

	//! Draw all the calibration spectra 
	void PaintCSpectra(CDC *pDC, LPRECT myrect);

	//! Draw data and baseline spectra for current acquisition ... this is a wrapper for CDlgBarAcq
	void PaintDSpectra(CDC *pDC, LPRECT myrect) { ((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.PaintDSpectra(pDC,myrect); }

	//! Draw data spectra in review list
	void PaintRSpectra(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect);

	//! Draw a spectrum.
	//
	//! This is a helper function for all the other spectrum painters
	//!
	//! @param wmin - set the axis minimum wavelength
	//! @param wmax - set the axis maximum wavelength
	//! @param imin - set the axis minimum intensity
	//! @param imax - set the axis maximum intensity
	bool PaintSpectrum(CDC *pDC, LPRECT myrect,CPen *pen,
					CSpectrumData &data,
					double wmin, double wmax,
					double imin, double imax,
					bool redraw_axis=true,
					bool wavlen_line=true);

	// void PaintCalCurve(CDC *pDC, LPRECT myrect,CPen *pen,
	// 				double *xv, double *yv, int pc);


#if 0 // should call this directly
	//! Draw a calibration curve based on stored calibration data
	//
	//! This is a wrapper for the function defined with the calibration data
	inline void PaintCalCurve(CDC *pDC, LPRECT myrect) {
		CalData.PaintCalCurve(pDC,myrect);
	}
#endif

#if 0 // should call this directly
	//! Populate a CListCtrl control with information about available calibration data.
	//
	//! This is a wrapper for the function defined with the calibration data
	inline void SetupCalCurveList(CListCtrl *listCtrl,bool init=false) {
		CalData.SetupCalCurveList(listCtrl,init);
	}
#endif

	inline int GetMaxIntTime() { 
		return pWrapper->getMaximumIntegrationTime(curSpectrometer); 
	}

#if 0 // should call this directly
	inline bool AcquireCalSpec(int idx, double set_conc, double integration, long average, long boxwidth) {
		return CalData.acquire(idx,set_conc,integration,average,boxwidth);
	}
	inline bool RemoveCalSpec(int idx) { return CalData.remove(idx); } 
#endif

#if 0 // should call this directly
	//! Indicate whether spectrum is valid and return data is non NULL pointer provided
	inline bool GetCalSpec(int idx, CSpectrumData *spec) {
		// if (idx>=MAX_CAL_SPEC)
		// 	return false;
		// if (! CalSpec[idx].is_valid())
		//	return false;
		// if (spec != NULL)
		// 	spec=&(CalSpec[idx]);
		if (spec != NULL)
			spec=CalData.get(idx);
		// @@@ huh? I thought this was supposed to tell me if the spectrum is valid?!?
		return true;
	}
	inline int GetCalSpecCount() { return CalData.getCount(); };
	inline void setFitType(CCalibrationCurve::curveType stype) { 
		CalData.setFitType(stype); 
		CalData.CalcBestFit();
	}
	inline CCalibrationCurve::curveType getFitType() { return CalData.getFitType(); }

	inline void setFitWavelength(double wavelength) { CalData.SetCalWavelength(wavelength); }
	inline double getFitWavelength(void) { return CalData.GetCalWavelength(); }

	inline CString getSubstanceName() { return CalData.getSubstanceName(); }
	inline void setSubstanceName(CString name) { CalData.setSubstanceName(name); }

	inline CString getFitInfo(unsigned int infotype) { return CalData.getFitInfo(infotype); };

	// This should likely be in theApp ... ?
	// !!! // moved this to DlgBarAcq
	//double UpdatePPM();

	inline bool Calibration_Save(CString *filename=NULL) { return CalData.Save(filename); }
	inline bool Calibration_Load(CString *filename=NULL) { return CalData.Load(filename); }
	inline void Calibration_Reset() { CCalibrationCurve tmp; CalData=tmp; }

	inline double  Calibration_getIntegrationTime() { return CalData.getIntegrationTime(); }
	inline void    Calibration_setIntegrationTime(double time) { CalData.setIntegrationTime(time); }
	inline int     Calibration_getSpectraToAverage() { return CalData.getSpectraToAverage(); }
	inline void    Calibration_setSpectraToAverage(int avg) { CalData.setSpectraToAverage(avg); }
	inline int     Calibration_getBoxcarWidth() { return CalData.getBoxcarWidth(); }
	inline void    Calibration_setBoxcarWidth(int width) { CalData.setBoxcarWidth(width); }
	inline CString Calibration_getSpecName() { return CalData.getSpecName(); }
	inline void    Calibration_setSpecName(CString spec) { CalData.setSpecName(spec); }
	inline CString Calibration_getSpecSerial() { return CalData.getSpecSerial(); }
	inline void    Calibration_setSpecSerial(CString serial) { CalData.setSpecSerial(serial); }

	inline void   Calibration_calcBestFit(void) { CalData.CalcBestFit(); }
	inline bool   Calibration_hasBaseline(void) { return CalData.hasBaseline(); }
#endif

	static bool RevSpec_SortAFileName(CDSpectrumData& d1, CDSpectrumData& d2);
	static bool RevSpec_SortDFileName(CDSpectrumData& d1, CDSpectrumData& d2);
	static bool RevSpec_SortATimeStamp(CDSpectrumData& d1, CDSpectrumData& d2);
	static bool RevSpec_SortDTimeStamp(CDSpectrumData& d1, CDSpectrumData& d2);
	static bool RevSpec_SortAPipeKm(CDSpectrumData& d1, CDSpectrumData& d2);
	static bool RevSpec_SortDPipeKm(CDSpectrumData& d1, CDSpectrumData& d2);
	static bool RevSpec_SortAConc(CDSpectrumData& d1, CDSpectrumData& d2);
	static bool RevSpec_SortDConc(CDSpectrumData& d1, CDSpectrumData& d2);
	static bool RevSpec_SortADil(CDSpectrumData& d1, CDSpectrumData& d2);
	static bool RevSpec_SortDDil(CDSpectrumData& d1, CDSpectrumData& d2);

	inline void PaintOpt_SetDSpecBaseline(bool set) {    ((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.paintoptSetBaseline(set); };
	inline bool PaintOpt_GetDSpecBaseline(void) { return ((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.paintoptGetBaseline();    };

	inline void PaintOpt_SetDSpecBaselineOverride(bool set) {    ((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.paintoptSetBaselineOverride(set); };
	inline bool PaintOpt_GetDSpecBaselineOverride(void) { return ((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.paintoptGetBaselineOverride();    };

	inline void PaintOpt_SetDSpecRaw(bool set) {    ((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.paintoptSetRaw(set); };
	inline bool PaintOpt_GetDSpecRaw(void) { return ((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.paintoptGetRaw();    };

	inline void PaintOpt_SetDSpecDiff(bool set) {    ((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.paintoptSetDiff(set); };
	inline bool PaintOpt_GetDSpecDiff(void) { return ((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.paintoptGetDiff();    };

	inline void PaintOpt_ShowCursor(double wavelen) { cursor_wavelen=wavelen; }
	inline void PaintOpt_HideCursor() { cursor_wavelen=-1; }
	inline bool PaintOpt_GetCursor() { return cursor_wavelen>=0; }
	inline double PaintOpt_GetCursorWavelen() { return cursor_wavelen; }

protected:
	// Pointer to the OO driver wrapper class
	Wrapper *pWrapper;

	// Number of spectrometers found
	int SpecCount;

	// Current Spectrometer on which to do actions
	int curSpectrometer;
	CString curSpecName,curSpecSerial;

	// Array containing the wavelength corresponding ot each index
	// @@@ this was conceived when I thought all spectrometers were created equal
	// ... really we need one of these for *each* spectrometer! Currently this is
	// compensated for by having to call SelecttSpectrometer() before acquiring a 
	// spectrum ... 
	DoubleArray SpecWavelengths;
	double wmin,wmax;

	// Setup parameters for acquiring a spectrum
	double parm_integration_time; // ms
	int parm_boxcar_width; // pixels
	int parm_averaging_num; // number
	bool parm_electrical_dark_on;
	bool parm_electrical_dark_reset;

	// wavelength at which to draw cursor (-1 for off)
	double cursor_wavelen;

#if 0 // Now owned by DlgBarAcq
	// Baseline spectrum and last acquired spectrum
	CSpectrumData BaselineSpec;
	CSpectrumData LastSpec;
#endif

	// Saved calibration spectra
	// CSpectrumData CalSpec[MAX_CAL_SPEC];
#if 0 // this is now located in ((CMainFrame *)(AfxGetMainWnd()))->m_DlgLeftAcq.CalData...
	CCalibrationCurve CalData;
#endif

	// Sometimes we need to collect extra dummy spectra ... 
	DoubleArray SpecDummy;



public:
	// Review Spectra
	std::vector<CDSpectrumData> RevSpecs;

};
