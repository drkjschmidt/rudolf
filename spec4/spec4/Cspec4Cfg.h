#pragma once

#define DEF_CFG_NAME "spec4.cfg"

class Cspec4Cfg
{
public:
	Cspec4Cfg(void);
	Cspec4Cfg(CString);
	~Cspec4Cfg(void);

protected:
	void Init(CString savepath);

	CString TrimFileName(CString fullpath) {
		CString trimpath(fullpath);
		int trimfpos = trimpath.ReverseFind('\\');

		if (trimfpos != -1)
			trimpath = trimpath.Left(trimfpos + 1); // keep the backslash ... 
		return trimpath;
	}

public:
	bool Save(CString *filename);
	bool Load(CString *filename);

	inline bool Save() { return Save(&(savepatharray[SAVE_PATH_CFG]+savename)); }
	inline bool Load() { return Load(&(savepatharray[SAVE_PATH_CFG]+savename)); }

	inline CString getSpectrometer(void) {  return lastSpectrometer; }
	inline void setSpectrometer(CString spec) { lastSpectrometer=spec; }

	inline CString getGPS(void) {  return lastGPS; }
	inline void setGPS(CString GPS) { lastGPS=GPS; }

	enum savePaths {
		SAVE_PATH_CFG = 0,
		SAVE_PATH_SPM = 1,
		SAVE_PATH_CAL = 2,
		SAVE_PATH_LOC = 3,
		SAVE_PATH_LOG = 4,
		SAVE_PATH_GFG = 5,
		SAVE_PATH_MAX = 5,
	};

	CString getSavepath(int type);
	void setSavepath(int type,CString path,bool trim=true);

	inline CString getSavepathCFG(void) {  return getSavepath(SAVE_PATH_CFG); }
	inline CString getSavepathSPM(void) {  return getSavepath(SAVE_PATH_SPM); }
	inline CString getSavepathCAL(void) {  return getSavepath(SAVE_PATH_CAL); }
	inline CString getSavepathLOC(void) {  return getSavepath(SAVE_PATH_LOC); }
	inline CString getSavepathLOG(void) {  return getSavepath(SAVE_PATH_LOG); }
	inline CString getSavepathGFG(void) {  return getSavepath(SAVE_PATH_GFG); }

	inline void setSavepathCFG(CString path,bool trim=true) { setSavepath(SAVE_PATH_CFG,path,trim); }
	inline void setSavepathSPM(CString path,bool trim=true) { setSavepath(SAVE_PATH_SPM,path,trim); }
	inline void setSavepathCAL(CString path,bool trim=true) { setSavepath(SAVE_PATH_CAL,path,trim); }
	inline void setSavepathLOC(CString path,bool trim=true) { setSavepath(SAVE_PATH_LOC,path,trim); }
	inline void setSavepathLOG(CString path,bool trim=true) { setSavepath(SAVE_PATH_LOG,path,trim); }
	inline void setSavepathGFG(CString path,bool trim=true) { setSavepath(SAVE_PATH_GFG,path,trim); }

	inline CString getSavename(void) {  return savename; }
	inline void setSavename(CString name) { savename=name; }

	inline CString getPasswd(void) { return passwd; }
	inline void setPasswd(CString pwd) { passwd=pwd; }

	//inline CString getUserLocPath(void) { 
	//	CString tval; 
	//	tval=savepath_cfg; 
	//	tval.TrimRight(_T("\\"));
	//	tval.Append(_T("\\userloc\\")); return tval; }

	inline int getCfgRunmode() { return cfg_runmode; }
	inline void setCfgRunmode(int set) { cfg_runmode=set; }

	inline bool getReqGPS() { return require_gps; }
	inline void setReqGPS(bool set) { require_gps=set; }

	inline bool getReqSpec() { return require_spec; }
	inline void setReqSpec(bool set) { require_spec=set; }

	inline bool getAcqVerb() { return acq_verb; }
	inline void setAcqVerb(bool set) { acq_verb=set; }

	inline void setAutosaveImmediate(bool set) { specAutoSaveImmediate=set; }
	inline bool getAutosaveImmediate() { return specAutoSaveImmediate; }

	inline void setAutosaveAll(bool set) { specAutoSaveAll=set; }
	inline bool getAutosaveWarn() { return specAutoSaveWarn; }

	inline void setAutosaveWarn(bool set) { specAutoSaveWarn=set; }
	inline bool getAutosaveAll() { return specAutoSaveAll; }

	inline void  setPrintLeftMargin(float set) { print_lm_in=set; }
	inline float getPrintLeftMargin() { return print_lm_in; }

	inline void  setPrintRightMargin(float set) { print_rm_in=set; }
	inline float getPrintRightMargin() { return print_rm_in; }

	inline void  setPrintTopMargin(float set) { print_tm_in=set; }
	inline float getPrintTopMargin() { return print_tm_in; }

	inline void  setPrintBottomMargin(float set) { print_bm_in=set; }
	inline float getPrintBottomMargin() { return print_bm_in; }

	inline void  setPrintLogoMaxHeight(float set) { print_logo_max_h_in=set; }
	inline float getPrintLogoMaxHeight() { return print_logo_max_h_in; }

	inline void  setPrintLogoMaxWidth(float set) { print_logo_max_w_in=set; }
	inline float getPrintLogoMaxWidth() { return print_logo_max_w_in; }

	inline void  setPrintLogoXoff(float set) { print_logo_hc_off_in=set; }
	inline float getPrintLogoXoff() { return print_logo_hc_off_in; }

	inline void  setPrintLogoYoff(float set) { print_logo_vc_off_in=set; }
	inline float getPrintLogoYoff() { return print_logo_vc_off_in; }

	inline void  setPrintLogoEscape(bool set) { print_logo_escape=set; }
	inline bool getPrintLogoEscape() { return print_logo_escape; }

protected:
	CString lastSpectrometer;
	CString lastGPS;

	// now in array
	// CString savepath_cfg; // save path for the config file (this class)
	// CString savepath_spm; // save path for spectrum data files
	// CString savepath_cal; // save path for calibration curves
	// CString savepath_loc; // save path for user location data
	// CString savepath_gfg; // save path for graph config info (@@@ not implemented yet ... dropped from meny around Dec 4, 2009)
	// CString savepath_log; // save path for debug / status log

	CString savename;
	CString passwd; // currently just cleartext
	int cfg_runmode;
	bool require_gps; // we need this override in case someone wants to test ...
	bool require_spec; // not sure what this override would be good for
	bool acq_verb; // Ask for sample change before sample custom dialog

	bool specAutoSaveAll; // Save all acquired spectra in safe location
	bool specAutoSaveImmediate; // Immediately after measuring show save dialog
	bool specAutoSaveWarn; // Warn before leaving acquisition without saving spectrum

	// hard margins, nothing goes outside here
	float print_lm_in; // left
	float print_rm_in; // right
	float print_tm_in; // top
	float print_bm_in; // bottom

	// constraints on logo
	float print_logo_max_w_in; // max width (default 2")
	float print_logo_max_h_in; // max height (default 1")
	float print_logo_hc_off_in; // how to position logo horizontally: offset from center, can't go past edge
	float print_logo_vc_off_in; // how to position logo veritcally: offset from center, can't go past edge
	bool  print_logo_escape; // whether the logo may escape the bounding box

protected:
	// Save names, i.e. the search indexes in the registry initialized by constructor
	CStringArray savenamearray;

	// Save paths (initialized by constructor)
	CStringArray savepatharray;
};
