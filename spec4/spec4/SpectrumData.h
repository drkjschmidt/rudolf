#pragma once

#include "conf_const.h"
#include "wrapper.h"
#include "arraytypes.h"
#include "RPGarmCom.h"
// #include "CalibrationCurve.h"

class CCalibrationCurve;

// static  CString CCalibrationCurve::formatFitInfo(unsigned int infotype,int ctype, double *cparm, int cplen, double crsqr);

//! Class to hold spectrum data
//
//! This class should hold all the data (spectrum, location, concentration, calibration
//! information) for a spectrum
//
class CSpectrumData
{
public:
	CSpectrumData(void);
	~CSpectrumData(void);

	//! Clear spectrum data
	//
	//! This invalidates the data inside the container ...
	inline void clear() { valid_wv=valid_int=false; spec_idx=wcount=0; wmin=wmax=imin=imax=-1; }

	//! Check if this spectrum has any valid data
	inline bool is_valid()    { return valid_int; }
	//! Check if this spectrum has valid wavelength data (in addition to intensities)
	inline bool is_valid_wv() { return valid_wv;  }

	bool store(int idx, DoubleArray *Intensities, DoubleArray *Wavelengths=NULL, double conc=-1);
	inline void storeDatatype(CString stype) { dtype=stype; }
	inline void storeComments(
		CString *slocn, 
		CString *scust, 
		CString *scomm,
		CString *spikm,
		CString *sdilu,
		CString *sffm1,
		CString *sffm2) {
			if (slocn != NULL) { Location = *slocn; }
			if (scust != NULL) { Customer = *scust; }
			if (scomm != NULL) { Comment  = *scomm; }
			if (spikm != NULL) { Pipekm   = atof(spikm->GetString()); }
			if (sdilu != NULL) { Dilution = atof(sdilu->GetString()); }
			if (sffm1 != NULL) { FF1      = *sffm1; }
			if (sffm2 != NULL) { FF2      = *sffm2; }
	}

	inline void storeAcqInfo(double sitime,int savg,int sboxw) {
		itime=sitime;
		avg=savg;
		boxw=sboxw;
	}

	inline void storeConc(double sconc) { conc=sconc; }
	inline void storeCalInfo(CString sname, CString sUFID, double swavelen, 
		int stype, double *sparm, double sRSquared,
		int scc, const double *sccn, const double *scct,
		double swl, double swh, double sel, double seh) 
	{
		int ii;

		calName=sname;
		calUFID=sUFID;
		calWavelen=swavelen;
		calType=stype;
		for (ii=0; ii<calType && ii<4; ii++)
			calConst[ii]=sparm[ii];
		calRSquared=sRSquared;
		calDataCount=min(scc,MAX_CAL_SPEC);
		for (ii=0; ii<calDataCount; ii++)
		{
			calConcns[ii]=sccn[ii];
			calCounts[ii]=scct[ii];
		}
		calWL=swl;
		calWH=swh;
		calEL=sel;
		calEH=seh;
	}

	inline void storeGpsLoc(double slat, double slon, double salt) {
		// lat=slat;
		// lon=slon;
		// alt=salt;
		GPSinfo.setData(&slat, &slon, &salt, NULL, NULL, NULL, NULL, (char *)NULL);
	}
	inline void storeGpsTime(INT32 stime, INT32 sdate) {
		// time=stime;
		// date=sdate;
		GPSinfo.setData(NULL, NULL, NULL, NULL, NULL, &stime, &sdate, (char *)NULL);
	}
	inline void storeGpsTime(CTime timestamp) {
		// time=stime;
		// date=sdate;
		GPSinfo.setData(NULL, NULL, NULL, NULL, NULL, &timestamp, (CString *)NULL);
	}
	inline void storeSysTime(SYSTEMTIME set) { ClockInfo=set; }

	inline void storeSpecName(CString set) { specName=set; }
	inline void storeSpecSerial(CString set) { specSerial=set; }

	inline DoubleArray getWavelengths() { if (valid_wv)  return wavelengths; return DoubleArray(); }
	inline DoubleArray getIntensities() { if (valid_int) return intensities; return DoubleArray(); }

	inline double getMinW() { if (valid_wv)  return wmin; return -1; }
	inline double getCalW() { if (valid_wv)  return calWavelen; return -1; }
	inline double getMaxW() { if (valid_wv)  return wmax; return -1; }
	inline double getMinI() { if (valid_int) return imin; return -1; }
	inline double getMaxI() { if (valid_int) return imax; return -1; }
	inline double getConc() {                return conc;            }

	//! Get the closest wavelength and intensity available based on 
	//! wavelength submitted. 
	//
	inline bool getXY(double *wv, double *in) {
		int ii;
		double tw;

		if ((! valid_wv) || (wv==NULL) || (in==NULL))
			return false;

		if ((*wv<wmin) || (*wv>wmax))
			return false;

		// Guess approximate index ... unfortunately the scale is not linear ...
		ii=(int)((wcount-1)*((*wv-wmin)/(wmax-wmin)));
		tw = wavelengths.getDoubleValues()[ii];

		// ... so now we need to search ...
		if (wavelengths.getDoubleValues()[ii] > *wv) {
			for ( ; ii>=0; ii--)
				if (wavelengths.getDoubleValues()[ii] <= *wv)
					break;
		} else {
			for ( ; ii<=wcount; ii++)
				if (wavelengths.getDoubleValues()[ii] > *wv)
					break;
			ii--;
		}

		// ... make sure we limit to legal indexes ...
		ii=min(ii,wcount-1);
		ii=max(ii,       0);

		// ... finally check which one is closer ...
		if ((ii<wcount-1) && ((*wv - wavelengths.getDoubleValues()[ii]) > (wavelengths.getDoubleValues()[ii+1] - *wv)))
			ii++;

		*wv = wavelengths.getDoubleValues()[ii];
		*in = intensities.getDoubleValues()[ii];
		return true;
	}



	inline int getSpec() { if (valid_int) return spec_idx; return -1; }

	inline int getCount() { if (valid_int) return wcount; return 0; }

	//! Define output style of serialize
	//
	//! If used with outputCSV, the data header (horizontal) and the acutal data
	//! (vertical) can be generated separately
	enum SerializeModes {
		outputCSV =   (unsigned char) 0x01,
		outputXML =   (unsigned char) 0x02,
	};

	//@name I/O functions
	//@{
	//! Save to file
	bool Save(CString *filename);

	//! Load from file
	bool Load(CString *filename);

	//! Create serialized data for save function
	CString serializeData(unsigned char modes);

	//! Convert serialized data from load function to calibration curve contents
	bool deserializeData(CString data, int &lpos, int &llpos);
	//@}

	//! Subtraction operator for programming convenience
	CSpectrumData operator-(CSpectrumData &right);

	const inline double getPipeKm() { return Pipekm; }
	const inline CString getPipeKmStr() { CString tval; tval.Format(_T("%g"),Pipekm); return tval; }
	const inline double getDilution() { return Dilution; }
	const inline CString getDilutionStr() { CString tval; tval.Format(_T("%g"),Dilution); return tval; }
	inline CString formatLoc() { return GPSinfo.formatLoc(); }
	inline CString formatTimeStamp() { 
		CString tval; 

		// Use GPS time if possible
		if (GPSinfo.getTime(NULL))
		{
			tval=GPSinfo.formatTime();
			tval.Append(" ");
			tval.Append(GPSinfo.formatDate());
		}
		// ... if we don't have GPS time, fall back to system time ...
		else
		{
			tval.Format(_T("%02d:%02d:%02d UTC  "),
				ClockInfo.wHour,
				ClockInfo.wMinute,
				ClockInfo.wSecond);
			tval.AppendFormat(_T("%04d-%02d-%02d "),
				ClockInfo.wYear,
				ClockInfo.wMonth,
				ClockInfo.wDay);
		}
		return tval;
	}
	inline CString getComment() { return Comment; }
	inline CString getCustomer() { return Customer; }
	inline CString getLocation() { return Location; }
	inline CString getCalname() { return calName; }
	inline CGPSData getGPSData() { return GPSinfo; }

	// not inlined to avoid cross-references ... 
	CString formatFitInfo(unsigned int infotype);

	//! Draw the spectrum
	//
	//! @param pDC - pointer to drawing canvas
	//! @param myrect - dimensions of whole canvas (what are the boundary conditions?)
	//! @param pen - line color of spectrum
	//! @param wmin - minimum wavelength of spectrum
	//! @param wmax - maximum wavelength of spectrum
	//! @param imin - minimum intensity / count value of spectrum
	//! @param imax - maximum intensity / count value of spectrum
	//! @param redraw_axis - allow suppressing axis drawing if we are painting multiple spectra
	bool PaintSpectrum(CDC *pDC, LPRECT myrect,CPen *pen,
						double wmin, double wmax,
						double imin, double imax, 
						bool redraw_axis,
						bool wavlen_line);

	//! Draw only a single spectrum
	//
	//! This is a wrapper to use if only a single spectrum is needed and there is no 
	//! need to pass extra limit information or to suppress the redrawing of axes
	inline bool PaintSpectrum(CDC *pDC, LPRECT myrect,CPen *pen,
		bool wavlen_line) 
	{
		return PaintSpectrum(pDC,myrect,pen,getMinW(),getMaxW(),getMinI(),getMaxI(),true,wavlen_line);
	}

	//! Create a (print) report
	//
	//! The report contains 
	//!
	//! * a header with all the relevant text data, 
	//! * an optional logo to be printed in the header
	//! * the spectrum itself
	//! * a calibration curve based on the data contained in the spectrum
	void PaintReport(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect);

	//! Draw the *included* calibration curve
	void PaintCalCurve(CDC *pDC, LPRECT myrect);

private:
	
	//! a comment line to identify spectrum sections
	CString dtype;

	//! Actual wavelengths [nm] at which spectrum was measured
	DoubleArray wavelengths;
	//! Associated intensities [counts]
	DoubleArray intensities;

	bool valid_wv,valid_int;
	int spec_idx;

	//! Informational variables
	//
	CString Customer;
	CString Location;
	CString Comment;
	double Pipekm;
	double Dilution;
	CString FF1,FF2;

	CGPSData GPSinfo;
	SYSTEMTIME ClockInfo;

	// double lat,lon,alt;
	// INT32 time,date;

	//! Name and Serial of spectrometer
	CString specName, specSerial;


	//! Name of calibration curve on which concentration is based
	//
	CString calName;
	CString calUFID;

	//! Wavelength at which concentration was determined
	double calWavelen;

	//! Calibration type to go with concentration
	//
	//! Currently defined values are:
	//!  0 - none/undefined
	//!  2 - linear
	//!  3 - square
	//!  4 - cubic
	int calType;

	//! Array of calibration constants ... contains calType values
	double calConst[4];

	//! calibration quality factor rqsuared
	double calRSquared;

	//! actual calibration datapoints so we don't need the calibration curve itself ...
	int calDataCount;
	double calConcns[MAX_CAL_SPEC];
	double calCounts[MAX_CAL_SPEC];

	//! Warning levels in calibration
	double calEL, calEH, calWL, calWH;


	//! Concentration value (measured or userdefined)
	//
	//! For normal spectra this is the measured concentration ...
	//! ... for calibration spectra this is the set concentration
	double conc; 

	// number of datapoints in spectrum
	int wcount;

	// minimum and maximum wavelengths
	double wmin,wmax;

	// minimum and maximum intensity values
	double imin,imax;

	// @@@ implement acquisition parameters ...
	// integration time used
	double itime; //ms
	int avg;
	int boxw;

};

//! Class to hold a "data" spectrum 
//
//! This is essentially a wrapper for a spectrum data file consisting of *three* spectra:
//! the baseline, the raw data, and the difference data
class CDSpectrumData
{
public:
	CDSpectrumData(void);
	~CDSpectrumData(void);

	//@name I/O functions
	//@{
	//! Save to file
	bool Save(CString *filename);

	//! Load from file
	bool Load(CString *filename);

	//! Data ... these are intentionally public for now ... maybe later cleanup
	//! can add appropriate cleanup / wrapper functions
	CSpectrumData specBaseline;
	CSpectrumData specRaw;
	CSpectrumData specDiff;

	//! Is this spectrum highlighted / bold
	bool highlight;

	//! Filename for use in review dialog bar
	CString specFileName;



//	static inline double getPipeKm() { return specDiff.getPipeKm(); }
//	static inline CSpectrumData getDiffSpecConst() { return static_cast<const CSpectrumData>(specDiff); }
};
