#pragma once
// Configuration constants that didn't fit anywhere else ... this is 
// probably very bad form ...

// Run mode
#define RUNMODE_UNDEF -1
#define RUNMODE_ACQUISITION  0
#define RUNMODE_CONFIGURATION 1
#define RUNMODE_REVIEW 2

// ACQ dialog state 
#define DLG_LFT_RESET  0 // if we have a spectrum this should force a save ... 
#define DLG_LFT_CL     1 // client / location
#define DLG_LFT_SCAL   2 // select calibration
#define DLG_LFT_BLANK  3 // measure water
#define DLG_LFT_SCAN   4 // measure data
#define DLG_LFT_SAVE   5 // save data

// CONTROL dialog state
#define DLG_LFT_CTL_RESET  0 
#define DLG_LFT_CTL_NODAT  1 
#define DLG_LFT_CTL_DATA   2 

// Array constants
#define MAX_CAL_SPEC 20
#define MAX_TOKENS (MAX_CAL_SPEC+2)

// Config file strings
#define TEXT_CNF_DEFGPS     "GPS"
#define TEXT_CNF_DEFSPEC    "Spectrometer"
#define TEXT_CNF_DEFMODE    "Mode"
#define TEXT_CNF_CLRPASS    "Password (clear)"
#define TEXT_CNF_REQ_GPS	"Require GPS"
#define TEXT_CNF_REQ_SPEC	"Require Spec"
#define TEXT_CNF_ACQ_VERB	"Verbose Acquisition"
#define TEXT_CNF_AS_IMMED	"Autosave Immediate"	// Will the measure dialog open a save dialog?
#define TEXT_CNF_AS_ALL		"Autosave All"			// Will the spectra be saved directly to safe location (no used in program)
#define TEXT_CNF_AS_WARN	"Autosave Warnings"		// Will the program warn about unsaved spectra? (not used in program)

// Customer info file strings
#define TEXT_CUST_START     "CustomerFile"
#define TEXT_CUST_VERSION   "Version"
#define TEXT_CUST_GPSLOC    "GPS Location"
#define TEXT_CUST_NAMECUST  "Customer Info"
#define TEXT_CUST_NAMELOC   "Location Info"
#define TEXT_CUST_NAMESAMP  "Sample Info"
#define TEXT_CUST_INF_KM	"Pipe km"
#define TEXT_CUST_INF_DIL	"Pre-Dilution Factor"
#define TEXT_CUST_INF_FF1   "Freeform Info 1"
#define TEXT_CUST_INF_FF2   "Freeform Info 2"
#define TEXT_CUST_END       "EndCustomerFile"

// Calibration file strings
#define TEXT_CAL_FILE_START "CalFile"
#define TEXT_CAL_FILE_END   "EndCalFile"
#define TEXT_CAL_VERSION    "Version"
#define TEXT_CAL_FILE_UFID  "Unique File ID"
#define TEXT_CAL_REQ_SPEC   "Requires Spectrometer"
#define TEXT_CAL_REQ_SPECEN "Requires Spectrometer iString"
#define TEXT_CAL_WAVELEN    "Calibration Wavelength [nm]"
#define TEXT_CAL_ITIME      "Calibration Integration Time [ms]"
#define TEXT_CAL_N_AVG      "Calibration Spectra To Average"
#define TEXT_CAL_BOXCAR_W   "Calibration Boxcar Width"
#define TEXT_CAL_SUBS_NAME  "Substance"
#define TEXT_CAL_PTS_WAVELN "Calibration Points Wavelength [nm]"
#define TEXT_CAL_PTS_INTENS "Calibration Points Intensity [counts]"
#define TEXT_CAL_PTS_RINTEN "Calibration Points Intensity (raw) [counts]"
#define TEXT_CAL_PTS_BASEIN "Calibration Points Baseline [counts]"
#define TEXT_CAL_TYPE       "Calibration Type"
#define TEXT_CAL_PARMS      "Calibration Parameters"
#define TEXT_CAL_RSQUARE    "Calibration Rsquare"
#define TEXT_CAL_WARN_LOW   "Conc. Threshold Low Warning [ppm]"
#define TEXT_CAL_WARN_HIGH  "Conc. Threshold High Warning [ppm]"
#define TEXT_CAL_ERR_LOW    "Conc. Threshold Low Error [ppm]"
#define TEXT_CAL_ERR_HIGH   "Conc. Threshold High Error [ppm]"

// Spectrum data file strings
#define TEXT_SPEC_START     "SpectrumFile"
#define TEXT_SPEC_END       "EndSpectrumFile"
#define TEXT_SPEC_VERSION   "Version"
#define TEXT_SPEC_INF_LOCN  "Location"
#define TEXT_SPEC_INF_CUST  "Customer"
#define TEXT_SPEC_INF_INFO  "SampleInfo"
#define TEXT_SPEC_INF_GPS   "GPSData"
#define TEXT_SPEC_INF_SYST  "SystemTime"
#define TEXT_SPEC_INF_KM	"Pipe km"
#define TEXT_SPEC_INF_DIL	"Pre-Dilution Factor"
#define TEXT_SPEC_INF_FF1   "Freeform Info"
#define TEXT_SPEC_CONC      "Concentration"
#define TEXT_SPEC_CAL_NAME  "CalibrationName"
#define TEXT_SPEC_CAL_UFID  "Unique Calibration File ID"
#define TEXT_SPEC_CAL_TYPE  "CalibrationType"
#define TEXT_SPEC_CAL_WAVE  "CalibrationWavelength"
#  define TEXT_SPEC_CAL_RSQR  "CalibrationRSquared"
#  define TEXT_SPEC_CAL_CONC  "CalibrationConcentrations [ppm]"
#  define TEXT_SPEC_CAL_CNTS  "CalibrationCounts"
#  define TEXT_SPEC_CAL_WLO   "CalibrationWarnLow"
#  define TEXT_SPEC_CAL_WHI   "CalibrationWarnHigh"
#  define TEXT_SPEC_CAL_ELO   "CalibrationErrorLow"
#  define TEXT_SPEC_CAL_EHI   "CalibrationErrorHigh"
#define TEXT_SPEC_SPEC_NAM  "Spectrometer Name"
#define TEXT_SPEC_SPEC_SER  "Spectrometer Serial"
#define TEXT_SPEC_ITIME     "IntegrationTime [ms]"
#define TEXT_SPEC_AVG       "AveragedSpectra" //@@@ implement
#define TEXT_SPEC_BOXW      "BoxcarWidth" //@@@ implement
#define TEXT_SPEC_WAVLN_RNG "WavelengthRange [nm]"
#define TEXT_SPEC_INTEN_RNG "IntensityRange [counts]"
#define TEXT_SPEC_N_WAVLN   "WavelengthCount"
#define TEXT_SPEC_WAVLN_HD1 "Wavelength [nm]"
#define TEXT_SPEC_WAVLN_HD2 "Intensity [counts]"
#define TEXT_SPEC_WAVLN_HDR TEXT_SPEC_WAVLN_HD1 "," TEXT_SPEC_WAVLN_HD2
