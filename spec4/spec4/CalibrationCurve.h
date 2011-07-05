#pragma once

#include "conf_const.h"
#include "Wrapper.h"
#include "SpectrumData.h"
#include "RPRainbowColors.h"
#include <math.h>

//! Class to hold calibration data and calculate calibration curves
class CCalibrationCurve
{
public:
	CCalibrationCurve(void);
	~CCalibrationCurve(void);
	
	enum curveType {
		Linear = (unsigned char)2,
		Polyn2 = (unsigned char)3,
		Polyn3 = (unsigned char)4,
	};

	//@name I/O functions
	//@{
	//! Save to file
	bool Save(CString *filename=NULL);

	//! Load from file
	bool Load(CString *filename=NULL);

	//! Create serialized data for save function
	CString serializeData(unsigned char modes);

	//! Convert serialized data from load function to calibration curve contents
	bool deserializeData(CString data, int &lpos, int &llpos);
	//@}

	inline void SetCalWavelength(double wavelength) { 
		int ii;
		double wv,in;

		// Store the wavelength ... 
		calWavelength=wavelength;

		// Update values in concentration / counts arrays
		for (ii=0; ii<calCount; ii++) {
			wv=wavelength;
			if (CalSpec[ii].getXY(&wv,&in))
			{
				calRawIntnValues[ii]=in;
				calIntnValues[ii]=calRawIntnValues[ii]-calRawIntnValues[0];
			}
		}

		// Recalculate curve fit and repopulate specrum data
		CalcBestFit(true);
	}
	inline double GetCalWavelength(void) { return calWavelength; }

	bool acquire(int idx, double set_conc, double integration, long average, long boxwidth);
	inline bool acquire (int idx, double set_conc) { 
		return acquire(idx, set_conc, calIntegrationTime, calSpectraToAverage, calBoxcarWidth); 
	}

	int add(CSpectrumData data);
	int replace(int idx, CSpectrumData data);
	CSpectrumData *get(int idx);
	bool remove(int idx);
	inline bool hasBaseline(void) { return hasBaselineVar; } // requires calcBestFit to get set.
	inline int getCount(void) { return calCount; }


	//! Draw a calibration curve based on xy data provided
	//
	//! This is the full customizable function and could conceivable work on any xy data
	static void PaintCalCurve(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect,
		int ct, const double *cp, int pc, const double *xv, const double *yv);

	//! Draw a calibration curve based on stored calibration data
	//
	//! This is a convenience wrapper that will insert the stored calibration data
	//! and draw the calibration curve
	inline void PaintCalCurve(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect) {
		PaintCalCurve(pDC,pInfo,myrect,type,calParms,calCount,calIntnValues,calConcValues);
	}

	//! Draw calibration graph of calibration spectra
	//
	bool PaintCalSpectra(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect);

	//! Create a (print) report
	//
	//! The report contains 
	//!
	//! * a header with all the relevant text data, 
	//! * an optional logo to be printed in the header
	//! * the calibration spectra 
	//! * the best fit calibration curve
	void PaintReport(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect);


	//! Populate a CListCtrl with the data about the stored calibration curves
	//
	//! @param listCtrl - list control pointer to control to be modified
	//! @param init - defines whether to recreate the list headers and set up list options
	void SetupCalCurveList(CListCtrl *listCtrl,bool init=false);


	//! Using the spectra and fit type stored previously, calculate best fit parameters
	//
	//! This function can (and by default will) update the calibration information stored
	//! within each spectrum.
	void CalcBestFit(bool update_spectra=true);

	//! Using best fit parameters stored (make sure you called CalcBestFit first)
	//! calculate coordinates for points of a fit curve
	inline double bestFitXY(double count,double basecount=0) {
		return bestFitXY(count,basecount,type,calParms);
	}

	//! Calculate best fit coordinates based on arbitrary fitting curve
	//! calculate coordinates for points of a fit curve
	static inline double bestFitXY(double count,double basecount,int ct,const double *cp) {
		int ii;
		double res=cp[0];
		for (ii=1; ii<ct; ii++)
			res+=cp[ii]*pow((count-basecount),ii);
		return res;
	}

	//! Return a string with the fit parameters
	enum FitInfoType {
		FIT_FORMULA  = (unsigned int)0x01,
		FIT_RSQUARED = (unsigned int)0x02,
	};

	//! Return contained calibration information
	//
	//! @param infotype - OR-ed selection of values from FitInfoType ENUM (currently FIT_FORMULA, FIT_RSQUARED)
	inline CString getFitInfo(unsigned int infotype) {
		return formatFitInfo(infotype,type,calParms,calCount,calRSquare);
	}

	//! Format arbitrary calibration information
	//
	//! @param infotype - OR-ed selection of values from FitInfoType ENUM (currently FIT_FORMULA, FIT_RSQUARED)
	//! @param ctype    - one of the values from curveTypefit ENUM (currently linear, Polyn2, Polyn3) ... this defines the number of necessary fit parameters
	//! @param cparm    - array of the fit coefficients in ascending power order 
	//! @param crsqr    - the rsquared value
	static inline CString formatFitInfo(unsigned int infotype,int ctype, double *cparm, int cplen, double crsqr) {
		CString tval;
		int ii;

		// fit function
		if (infotype & FitInfoType::FIT_FORMULA) {
			tval.Format(_T("ppm = "));
			if (cplen >= ctype)
			{
				for (ii=ctype-1; ii>1; ii--)
					tval.AppendFormat(_T("%.3g * x^%d + "),cparm[ii],ii);
				for (          ; ii>0; ii--)
					tval.AppendFormat(_T("%.3g * x + "),cparm[ii],ii);
				tval.AppendFormat(_T("%.3g\n"),cparm[0]);
			}
			else 
			{
				tval.Append(_T(" ..."));
			}

		}

		// TODO: rsquared ...
		if (infotype & FitInfoType::FIT_RSQUARED) {
			if (cplen >= ctype)
			{
				tval.AppendFormat(_T("Rsquared = %.4f\n"),crsqr);
			}
			else 
			{
				tval.Append(_T("Rsquared = ...\n"));
			}
		}

		return tval;
	}



	inline void setFitType(curveType stype) { type=stype; }
	inline curveType getFitType() { return type; }
	inline double *getFitParms() { return calParms; }

	inline CString getSubstanceName() { return substanceName; }
	inline void setSubstanceName(CString name) { substanceName = name; }

	inline double getIntegrationTime() { return calIntegrationTime; }
	inline void setIntegrationTime(double time) { calIntegrationTime=time; }
	inline int getSpectraToAverage() { return calSpectraToAverage; }
	inline void setSpectraToAverage(int avg) { calSpectraToAverage=avg; }
	inline int getBoxcarWidth() { return calBoxcarWidth; }
	inline void setBoxcarWidth(int width) { calBoxcarWidth=width; }
	inline CString getSpecName() { return requiredSpectrometer; }
	inline void setSpecName(CString spec) { requiredSpectrometer=spec; }
	inline CString getSpecSerial() { return requiredSpecSerial; }
	inline void setSpecSerial(CString serial) { requiredSpecSerial=serial; }
	inline bool isValid() { return calCount>=type; }

	inline double getRSquare() { return calRSquare; }
	inline const double *getCalConcArray() { return (const double *)calConcValues; }
	inline const double *getCalIntnArray() { return (const double *)calIntnValues; }

	// inline void   setWarnLow(double set) {        warn_low=set; } // autocalculated
	inline double getWarnLow()           { return ppmWarnLow;     }
	// inline void   setWarnHigh(double set) {        warn_high=set; } // autocalculated
	inline double getWarnHigh()           { return ppmWarnHigh;     }

	inline void   setErrLow(double set) {        ppmErrLow=set; }
	inline double getErrLow()           { return ppmErrLow;     }
	inline void   setErrHigh(double set) {        ppmErrHigh=set; }
	inline double getErrHigh()           { return ppmErrHigh;     }

	inline bool getXY(int idx, double *x, double *y) { 
		if ((0 <= idx) && (idx < calCount)) {
			return CalSpec[idx].getXY(x,y);
		}
		return false;
	}

	inline CString getUFID() { return uniqueID; }

private:

	curveType type;

	// Unique calibration curve identifier ... this is semi-ephemeral: it will be loaded with
	// a calibration curve, but a new one will be created and written to file every time 
	// a calibration curve is saved.
	CString uniqueID;

	// TODO
	CString requiredSpectrometer,requiredSpecSerial;
	CString substanceName;

	// at which wavelength was calibration done
	double calWavelength;

	// what integration time do we want to use ...
	// ... note that the spectra themselves will carry the information about the 
	//     integration time they were created at but that does not guarantee that
	//     they are all the same ... 
	// TODO: actually use the parameter ...
	// Acquisition parameters ... we need to save these because they will
	// define acquisition in BASIC mode
	double calIntegrationTime; // ms
	int calSpectraToAverage;
	int calBoxcarWidth;

	// number of calibration spectra
	int calCount;

	// calibration points (x=conc, y=counts)
	double calConcValues[MAX_CAL_SPEC];
	double calIntnValues[MAX_CAL_SPEC],calRawIntnValues[MAX_CAL_SPEC];
	
	// Currently we need up to 4 calibration parameters (3rd order polynomial)
	double calParms[4];

	// quality of fit: true if a "0 concentration" baseline spectrum is available
	double calRSquare;

	// for convenience ...
	bool hasBaselineVar;
	double baselineInt;

	// Saved dark curve?
	CSpectrumData calDark;

	// Saved calibration spectra
	CSpectrumData CalSpec[MAX_CAL_SPEC];

	// Warning / error reporting points for ppm
	double ppmWarnLow,ppmWarnHigh;
	double ppmErrLow,ppmErrHigh;

};
