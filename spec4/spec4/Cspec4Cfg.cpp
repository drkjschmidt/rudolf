#include "StdAfx.h"
#include "Cspec4Cfg.h"
#include "spec4.h"
#include "spec4Doc.h"
#include "RegistryHelper.h"

Cspec4Cfg::Cspec4Cfg(void)
: lastSpectrometer(_T(""))
, lastGPS(_T(""))
, savename(_T("spec4.cfg"))
, require_gps(true)
, require_spec(true)
, acq_verb(false)
, specAutoSaveImmediate(false)
, specAutoSaveAll(false)
, specAutoSaveWarn(true)
, cfg_runmode(RUNMODE_ACQUISITION)
, print_lm_in(0.0)
, print_rm_in(0.0)
, print_tm_in(0.0)
, print_bm_in(0.0)
, print_logo_max_h_in(0.0)
, print_logo_max_w_in(0.0)
, print_logo_hc_off_in(-1000)
, print_logo_vc_off_in(-1000)
, print_logo_escape(false)
{
	// _MAX_PATH is defined in a system file:
	// c:\PROGRAM FILES\MICROSOFT VISUAL STUDIO\VC98\INCLUDE\stdlib.h
	CString savepathtmp;
	char strPathName[_MAX_PATH];

// Traditionally we used to log to the application
// folder ... that works well for privileged users,
// not so well for unprivileged. New default is to
// log to My Documents\LightPilot QB\*\...
#if defined LOG_TO_APPFOLDER

	// I am assuming this gives us the application path name
	::GetModuleFileName(NULL, strPathName, _MAX_PATH);

	// Strip the actual application name from the path
	savepathtmp=strPathName;
	int fpos = savepathtmp.ReverseFind('\\');

	if (fpos != -1)
	savepathtmp = savepathtmp.Left(fpos); // trim the backslash ... 
#else
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, strPathName);
	savepathtmp=strPathName;
	savepathtmp.Append("\\LightPilot QB");
#endif

	Init(savepathtmp);
}

Cspec4Cfg::Cspec4Cfg(CString savepathtmp)
: lastSpectrometer(_T(""))
, lastGPS(_T(""))
, savename(_T("spec4.cfg"))
, require_gps(true)
, require_spec(true)
, acq_verb(false)
, specAutoSaveImmediate(false)
, specAutoSaveAll(false)
, specAutoSaveWarn(true)
, cfg_runmode(RUNMODE_ACQUISITION)
, print_lm_in(0.0)
, print_rm_in(0.0)
, print_tm_in(0.0)
, print_bm_in(0.0)
, print_logo_max_h_in(0.0)
, print_logo_max_w_in(0.0)
, print_logo_hc_off_in(-1000)
, print_logo_vc_off_in(-1000)
, print_logo_escape(false)
{
	savepathtmp.TrimRight(_T("\\"));

	Init(savepathtmp);
}

Cspec4Cfg::~Cspec4Cfg(void)
{
}

void
Cspec4Cfg::Init(CString rootpath) {  
	int ii;

	// Populate lookup table
	savenamearray.SetAtGrow(SAVE_PATH_CFG,"confloc");
	savenamearray.SetAtGrow(SAVE_PATH_SPM,"spectra");
	savenamearray.SetAtGrow(SAVE_PATH_CAL,"calfile");
	savenamearray.SetAtGrow(SAVE_PATH_LOC,"userloc");
	savenamearray.SetAtGrow(SAVE_PATH_LOG,"logsloc");
	savenamearray.SetAtGrow(SAVE_PATH_GFG,"graphcf");

	// Preset variables to a default value ...
	savepatharray.SetAtGrow(SAVE_PATH_CFG,rootpath + "\\");
	savepatharray.SetAtGrow(SAVE_PATH_SPM,rootpath + "\\spectra\\");
	savepatharray.SetAtGrow(SAVE_PATH_CAL,rootpath + "\\calfile\\");
	savepatharray.SetAtGrow(SAVE_PATH_LOC,rootpath + "\\userloc\\");
	savepatharray.SetAtGrow(SAVE_PATH_LOG,rootpath + "\\");
	savepatharray.SetAtGrow(SAVE_PATH_GFG,rootpath + "\\graphcf\\");

	// ... then check registry for updated values
	for (ii=0; ii<=SAVE_PATH_MAX; ii++) {
		CString kval,tval;
		kval=_T("SOFTWARE\\Wilson Analytical\\LightPilot QB");
		RegistryHelper regworker;
		if (regworker.Init(kval)) {
			if ((regworker.ReadStringValue(savenamearray[ii],tval)) && (! tval.IsEmpty())) {
				savepatharray[ii]=tval;
			} else {
				regworker.WriteStringValue(savenamearray[ii],savepatharray[ii]);
			}
		}
	}
}

CString 
Cspec4Cfg::getSavepath(int type) {  
	return savepatharray[type];
}

void 
Cspec4Cfg::setSavepath(int type,CString path,bool trim) { 
	savepatharray[type]=(trim?TrimFileName(path):path); 

	CString kval,tval;
	kval=_T("SOFTWARE\\Wilson Analytical\\LightPilot QB");
	RegistryHelper regworker;
	if (regworker.Init(kval))
		regworker.WriteStringValue(savenamearray[type],savepatharray[type]);
}


bool 
Cspec4Cfg::Save(CString *fname)
{
	CString line;
	CString filename;

	if (fname == NULL) 
	{
		CFileDialog TempLoader(FALSE,_T(".cfg"), NULL, 0, "Config Files (*.cfg)|*.cfg|All Files(*.*)|*.*||");
		TempLoader.m_pOFN->lpstrInitialDir=savepath_cfg;

		if (TempLoader.DoModal() == IDOK)
		{
			// we have a selection
			theApp.ChangeStatusText(_T("Selection OK"));
			// savepath_cfg=TempLoader.GetFolderPath();
			// savepath_cfg=savepath_cfg.TrimRight(_T("\\"));
			filename=TempLoader.GetPathName();
			savepath_cfg=filename;
			savepath_cfg.Truncate(savepath_cfg.ReverseFind('\\'));
		}
		else
		{
			// we don't have a selection
			theApp.ChangeStatusText(_T("Selection Cancel"));
			return false;
		}
	}
	else {
		filename=*fname;
	}


	try {
		// Implicit open using constructor. This is not pretty
		// but makes the try / catch error handling much prettier
		
		//if (! cfgfile.Open(filename.GetString(),CFile::modeCreate|CFile::modeWrite|CFile::typeText))
		//	return false;
		CStdioFile cfgfile(filename.GetString(),CFile::modeCreate|CFile::modeWrite|CFile::typeText);

		line.Format(_T(TEXT_CNF_DEFSPEC ",%s\n"),lastSpectrometer.GetString());
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_DEFGPS ",%s\n"),lastGPS.GetString());
		cfgfile.WriteString(line.GetString());

		// @@@ HACK around problem of *loading* with mode=1
		// line.Format(_T(TEXT_CNF_DEFMODE ",%d\n"),theApp.getRunmode());
		line.Format(_T(TEXT_CNF_DEFMODE ",%d\n"),0);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_CLRPASS ",%s\n"),passwd.GetString());
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_REQ_GPS ",%d\n"),require_gps?1:0);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_REQ_SPEC ",%d\n"),require_spec?1:0);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_ACQ_VERB ",%d\n"),acq_verb?1:0);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_AS_IMMED ",%d\n"),specAutoSaveImmediate?1:0);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_AS_ALL ",%d\n"),specAutoSaveAll?1:0);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_AS_WARN ",%d\n"),specAutoSaveWarn?1:0);
		cfgfile.WriteString(line.GetString());

		// --- bug fix branch ---

		line.Format(_T(TEXT_CNF_PRINT_LM ",%f\n"),print_lm_in);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_PRINT_RM ",%f\n"),print_rm_in);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_PRINT_TM ",%f\n"),print_tm_in);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_PRINT_BM ",%f\n"),print_bm_in);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_PRINT_LOGO_MAXW ",%f\n"),print_logo_max_w_in);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_PRINT_LOGO_MAXH ",%f\n"),print_logo_max_h_in);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_PRINT_LOGO_XOFF ",%f\n"),print_logo_hc_off_in);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_PRINT_LOGO_YOFF ",%f\n"),print_logo_vc_off_in);
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_PRINT_LOGO_EXIT ",%d\n"),print_logo_escape?1:0);
		cfgfile.WriteString(line.GetString());





		cfgfile.Close();
	}
	catch(CFileException* pe)
	{
		char szMsg[256];
		pe->GetErrorMessage(szMsg, sizeof(szMsg));
		MessageBox(NULL, szMsg, NULL, 0);
		pe->Delete();

		return false;
	}

	return true;
}

bool 
Cspec4Cfg::Load(CString *fname)
{
	CString line,token,value;
	CString filename;
	int tpos;

	if (fname == NULL) {

		CFileDialog TempLoader(TRUE,_T(".cfg"), NULL, 0, "Config Files (*.cfg)|*.cfg|All Files(*.*)|*.*||");
		TempLoader.m_pOFN->lpstrInitialDir=savepath_cfg;

		if (TempLoader.DoModal() == IDOK)
		{
			// we have a selection
			theApp.ChangeStatusText(_T("Selection OK"));
			savepath_cfg=TempLoader.GetFolderPath().TrimRight(_T("\\"));
			filename=TempLoader.GetPathName();
		}
		else
		{
			// we don't have a selection
			theApp.ChangeStatusText(_T("Selection Cancel"));
			return false;
		}
	}
	else {
		filename=*fname;
	}


	try {
		//if (! cfgfile.Open(filename.GetString(),CFile::modeRead|CFile::typeText))
		//	return false;

		// Implicit open using constructor. This is not pretty
		// but makes the try / catch error handling much prettier
		CStdioFile cfgfile(filename.GetString(),CFile::modeRead|CFile::typeText);

		while (cfgfile.ReadString(line)) {
			tpos=0;
			token=line.Tokenize(_T(","),tpos);
			value=line.Tokenize(_T(","),tpos);

			if (! token.CompareNoCase(_T(TEXT_CNF_DEFSPEC)))
				lastSpectrometer=value;

			if (! token.CompareNoCase(_T(TEXT_CNF_DEFGPS)))
				lastGPS=value;

			if (! token.CompareNoCase(_T(TEXT_CNF_DEFMODE)))
			{
				cfg_runmode=atoi(value);
				// @@@ we set this here in case this gets called in program
				// but we should make sure we override this on startup ...
				theApp.setRunmode(cfg_runmode);

				// ((Cspec4Doc *)(AfxGetMainWnd()->GetActiveView()->GetDocument())->SetProgMode(theApp.runmode);
			}

			if (! token.CompareNoCase(_T(TEXT_CNF_CLRPASS)))
				passwd=value;

			if (! token.CompareNoCase(_T(TEXT_CNF_REQ_GPS)))
			{
				require_gps=true;
				if (! value.CompareNoCase(_T("false")))
					require_gps=false;
				if (! value.CompareNoCase(_T("0")))
					require_gps=false;
			}

			if (! token.CompareNoCase(_T(TEXT_CNF_REQ_SPEC)))
			{
				require_spec=true;
				if (! value.CompareNoCase(_T("false")))
					require_spec=false;
				if (! value.CompareNoCase(_T("0")))
					require_spec=false;
			}

			if (! token.CompareNoCase(_T(TEXT_CNF_ACQ_VERB)))
			{
				acq_verb=true;
				if (! value.CompareNoCase(_T("false")))
					acq_verb=false;
				if (! value.CompareNoCase(_T("0")))
					acq_verb=false;
			}

			if (! token.CompareNoCase(_T(TEXT_CNF_AS_ALL)))
			{
				specAutoSaveAll=true;
				if (! value.CompareNoCase(_T("false")))
					specAutoSaveAll=false;
				if (! value.CompareNoCase(_T("0")))
					specAutoSaveAll=false;
			}

			if (! token.CompareNoCase(_T(TEXT_CNF_AS_WARN)))
			{
				specAutoSaveWarn=true;
				if (! value.CompareNoCase(_T("false")))
					specAutoSaveWarn=false;
				if (! value.CompareNoCase(_T("0")))
					specAutoSaveWarn=false;
			}

			if (! token.CompareNoCase(_T(TEXT_CNF_AS_IMMED)))
			{
				specAutoSaveImmediate=true;
				if (! value.CompareNoCase(_T("false")))
					specAutoSaveImmediate=false;
				if (! value.CompareNoCase(_T("0")))
					specAutoSaveImmediate=false;
			}

			// --- bug fix branch ---

			if (! token.CompareNoCase(_T(TEXT_CNF_PRINT_LM)))
				print_lm_in=atof(value);

			if (! token.CompareNoCase(_T(TEXT_CNF_PRINT_RM)))
				print_rm_in=atof(value);

			if (! token.CompareNoCase(_T(TEXT_CNF_PRINT_TM)))
				print_tm_in=atof(value);

			if (! token.CompareNoCase(_T(TEXT_CNF_PRINT_BM)))
				print_bm_in=atof(value);

			if (! token.CompareNoCase(_T(TEXT_CNF_PRINT_LOGO_MAXW)))
				print_logo_max_w_in=atof(value);

			if (! token.CompareNoCase(_T(TEXT_CNF_PRINT_LOGO_MAXH)))
				print_logo_max_h_in=atof(value);

			if (! token.CompareNoCase(_T(TEXT_CNF_PRINT_LOGO_XOFF)))
				print_logo_hc_off_in=atof(value);

			if (! token.CompareNoCase(_T(TEXT_CNF_PRINT_LOGO_YOFF)))
				print_logo_vc_off_in=atof(value);

			if (! token.CompareNoCase(_T(TEXT_CNF_PRINT_LOGO_EXIT)))
			{
				print_logo_escape=false;
				if (! value.CompareNoCase(_T("true")))
					print_logo_escape=true;
				if (! value.CompareNoCase(_T("1")))
					print_logo_escape=true;
			}

		}

		cfgfile.Close();
	}
	catch(CFileException* pe)
	{
		char szMsg[256];
		pe->GetErrorMessage(szMsg, sizeof(szMsg));
		MessageBox(NULL, szMsg, NULL, 0);
		pe->Delete();

		return false;
	}

	return true;
}