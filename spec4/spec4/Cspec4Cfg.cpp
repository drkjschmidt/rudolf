#include "StdAfx.h"
#include "Cspec4Cfg.h"
#include "spec4.h"
#include "spec4Doc.h"

Cspec4Cfg::Cspec4Cfg(void)
: lastSpectrometer(_T(""))
, lastGPSPortType(_T(""))
, lastGPSPort(_T(""))
, lastGPSDevice(_T(""))
, lastGPSID(_T(""))
, savename(_T("spec4.cfg"))
, require_gps(true)
, require_spec(true)
, acq_verb(false)
, specAutoSaveImmediate(false)
, specAutoSaveAll(false)
, specAutoSaveWarn(true)
, cfg_runmode(RUNMODE_ACQUISITION)
{
	// _MAX_PATH is defined in a system file:
	// c:\PROGRAM FILES\MICROSOFT VISUAL STUDIO\VC98\INCLUDE\stdlib.h
	CString savepath;
	char strPathName[_MAX_PATH];

	// I am assuming this gives us the application path name
	::GetModuleFileName(NULL, strPathName, _MAX_PATH);

	// Strip the actual application name from the path
	savepath=strPathName;
	int fpos = savepath.ReverseFind('\\');

	if (fpos != -1)
	savepath = savepath.Left(fpos); // trim the backslash ... 

	savepath_cfg=savepath + "\\";
	savepath_log=savepath + "\\";
	savepath_loc=savepath + "\\userloc\\";
	savepath_spm=savepath + "\\spectra\\";
	savepath_cal=savepath + "\\calfile\\";
	savepath_gfg=savepath + "\\graphcf\\";
}

Cspec4Cfg::Cspec4Cfg(CString savepath)
: lastSpectrometer(_T(""))
, lastGPSPortType(_T(""))
, lastGPSPort(_T(""))
, lastGPSDevice(_T(""))
, lastGPSID(_T(""))
, savename(_T("spec4.cfg"))
, require_gps(true)
, require_spec(true)
, acq_verb(false)
, specAutoSaveImmediate(false)
, specAutoSaveAll(false)
, specAutoSaveWarn(true)
, cfg_runmode(RUNMODE_ACQUISITION)
{
	savepath.TrimRight(_T("\\"));

	savepath_cfg=savepath + "\\";
	savepath_log=savepath + "\\";
	savepath_loc=savepath + "\\userloc\\";
	savepath_spm=savepath + "\\spectra\\";
	savepath_cal=savepath + "\\calfile\\";
	savepath_gfg=savepath + "\\graphcf\\";
}

Cspec4Cfg::~Cspec4Cfg(void)
{
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

		// line.Format(_T(TEXT_CNF_DEFGPS ",%s\n"),lastGPS.GetString());
		// cfgfile.WriteString(line.GetString());
		line.Format(_T(TEXT_CNF_DEFPTP ",%s\n"),lastGPSPortType.GetString());
		cfgfile.WriteString(line.GetString());

		line.Format(_T(TEXT_CNF_DEFPNM ",%s\n"),lastGPSPort.GetString());
		cfgfile.WriteString(line.GetString());
		
		line.Format(_T(TEXT_CNF_DEFDNM ",%s\n"),lastGPSDevice.GetString());
		cfgfile.WriteString(line.GetString());
		
		line.Format(_T(TEXT_CNF_DEFDID ",%s\n"),lastGPSID.GetString());
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
			// savepath_cfg=TempLoader.GetFolderPath().TrimRight(_T("\\"));
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

			// This is an old config file but we still need to do something sensible
			if (! token.CompareNoCase(_T(TEXT_CNF_DEFGPS))) {
				lastGPSPortType="NMEA"; // @@@ This is a hack but there were no other choices at the time
				lastGPSPort=value;
				lastGPSDevice="";
				lastGPSID="";
			}

			// The 4 new values replacing it ... 
			if (! token.CompareNoCase(_T(TEXT_CNF_DEFPTP)))
				lastGPSPortType=value;
			if (! token.CompareNoCase(_T(TEXT_CNF_DEFPNM)))
				lastGPSPort=value;
			if (! token.CompareNoCase(_T(TEXT_CNF_DEFDNM)))
				lastGPSDevice=value;
			if (! token.CompareNoCase(_T(TEXT_CNF_DEFDID)))
				lastGPSID=value;

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