// spec4.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "time.h"
#include "conf_const.h"
#include "spec4.h"
#include "MainFrm.h"

#include "spec4Doc.h"
#include "spec4View.h"

#include "SetupDialog.h"
#include "CalibrationDialog.h"
#include "RPGarmCom.h"
#include "Cspec4Cfg.h"
#include "VersionNo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cspec4App

BEGIN_MESSAGE_MAP(Cspec4App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &Cspec4App::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_TOOLS_CHANGE, &Cspec4App::OnToolsChange)
	ON_COMMAND(ID_TOOLS_LOAD, &Cspec4App::OnToolsLoad)
	ON_COMMAND(ID_TOOLS_SAVE, &Cspec4App::OnToolsSave)
	ON_COMMAND(ID_VIEW_SELECTDATAFILES, &Cspec4App::OnViewSelectdatafiles)
	ON_COMMAND(ID_VIEW_SAVEGRAPHINFO, &Cspec4App::OnViewSavegraphinfo)
	ON_COMMAND(ID_VIEW_LOADGRAPHINFO, &Cspec4App::OnViewLoadgraphinfo)
	ON_COMMAND(ID_COLLECT_ENTERATL, &Cspec4App::OnCollectEnteratl)
	ON_COMMAND(ID_COLLECT_GPSTOATL, &Cspec4App::OnCollectGpstoatl)
	ON_COMMAND(ID_COLLECT_LOADDEFINITION, &Cspec4App::OnCollectLoaddefinition)
	ON_COMMAND(ID_COLLECT_MEASURE, &Cspec4App::OnCollectMeasure)
	ON_COMMAND(ID_COLLECT_SAVEATL, &Cspec4App::OnCollectSaveatl)
#if 0 // we don't have these meny items anymore
	ON_COMMAND(ID_CALIBRATION_CREATE, &Cspec4App::OnCalibrationCreate)
	ON_COMMAND(ID_CALIBRATION_LOAD, &Cspec4App::OnCalibrationLoad)
	ON_COMMAND(ID_CALIBRATION_SAVE, &Cspec4App::OnCalibrationSave)
#endif

END_MESSAGE_MAP()


// Cspec4App construction

Cspec4App::Cspec4App()
: runmode(RUNMODE_ACQUISITION)
, pSpecControl(0)
//, pGarminPort(0)
, pGPS(0)
, pConfig(0)
, pCustInfoDlg(0)
, pCustInfoData(0)
{
	// TODO: add construction code here,


	// Place all significant initialization in InitInstance
}

Cspec4App::~Cspec4App()
{
	if (pSpecControl  != NULL) delete pSpecControl;
	// if (pGarminPort   != NULL) delete pGarminPort;
	if (pGPS          != NULL) delete pGPS;
	if (pConfig       != NULL) delete pConfig;
	if (pCustInfoDlg  != NULL) delete pCustInfoDlg;
	if (pCustInfoData != NULL) delete pCustInfoData;
}


// The one and only Cspec4App object

Cspec4App theApp;


// Cspec4App initialization

BOOL Cspec4App::InitInstance()
{
	CString sname,tval;
#ifdef PROGTYPE_ACQ
	bool validSpec=false;
	bool validGPS=false;
#endif

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cspec4Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(Cspec4View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Get computer name for logging ...
	TCHAR tmp[32];
	DWORD tpl=32;

	GetComputerName(tmp,&tpl);
	NetBIOSName=tmp;

	// The one and only window has been initialized, so show and update it
	// m_pMainWnd->SetWindowTextA(_T("LightPilot")); // @@@ this breaks e.g. after print preview ... change in MainFrm?
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	// @@@
	// @@RP ... this is where our code starts
	// @@@

#if !defined SUPPRESS_SPLASH
	theApp.ChangeStatusText(_T("Splash Screen and Initializations here"));
	CSplashDialog::ShowSplashScreen(m_pMainWnd,20000,500);
#endif

	// Get working directory ... this is now done by the config file loader ...
	theApp.pConfig=new Cspec4Cfg();

	if (theApp.pConfig->Load())
	{
		theApp.ChangeStatusText(_T("Load default config file SUCCEEDED"));
	}
	else 
	{
		theApp.ChangeStatusText(_T("Load default config file FAILED"));
	}

	// Initialize Spectrometer ...
	theApp.ChangeStatusText(_T("Creating Spectrometer / Wrapper object"));
	theApp.pSpecControl=new CSpectrometer();

	theApp.ChangeStatusText(_T(" ... populating with spectrometers"));
	if (theApp.pSpecControl->GetSpecCount()>0)
	{
		theApp.ChangeStatusText(_T("Found one or more spectrometers"));
		if (theApp.pSpecControl->SelectSpectrometer(theApp.pConfig->getSpectrometer().GetString()) < 0)
		{
			theApp.ChangeStatusText(_T("Did not found preset specrometer ... picking first one"));
			theApp.pSpecControl->SelectSpectrometer(0);
		}
	}
	else
	{
		theApp.ChangeStatusText(_T("No spectrometers found"));
	}

	// Initialize GPS
	theApp.ChangeStatusText(_T("Creating GPS object"));
	// theApp.pGarminPort=new CRPGarmCom();
	// if (theApp.pGarminPort->setGarminPort(theApp.pConfig->getGPS()) &&
	//	theApp.pGarminPort->openGarminPort())
	//	theApp.pGarminPort->updateInfo();
	theApp.pGPS=new CGPSgeneric();
	CGPSport GPSinfo;
	CString GPSportname=theApp.pConfig->getGPSPort();
	CString GPSdevname=theApp.pConfig->getGPSDevice();
	CString GPSdevid=theApp.pConfig->getGPSID();
	
	if (theApp.pGPS->findPort(&GPSinfo,&GPSportname,&GPSdevname,&GPSdevid) &&
		theApp.pGPS->openPort( GPSinfo))
		theApp.pGPS->updateInfo(GPSinfo);
	

	// Create storage object for user / location data
	theApp.ChangeStatusText(_T("Create CustomerDialog object"));
	theApp.pCustInfoDlg=new CCustomerInfoDialog();
	theApp.pCustInfoData=new CCustomerInfoData();

	// TODO: here we would test whether the last known config GPS
	// is still available
/* @@@ This probably belongs into "load calibration"
	theApp.ChangeStatusText(_T("Verifying appropriate Spectrometer for default config"));
	sname=theApp.pConfig->getSpectrometer();
	for (ii=0; ii<theApp.pSpecControl->GetSpecCount(); ii++) {
		tval=theApp.pSpecControl->GetSpecName(ii);
		if (! sname.Compare(tval)) {
			theApp.pSpecControl->SelectSpectrometer(ii);
#ifdef PROGTYPE_ACQ
			validSpec=true;
#endif
		}
	}
*/

	// This should be a safe way to force the window into the run mode we want ... 
	// @@@ for some reason the dialog cannot be killed by mouseclick?!?
#if !defined SUPPRESS_SPLASH
	if (theApp.pConfig->getCfgRunmode() == RUNMODE_CONFIGURATION)
		CSplashDialog::HideSplashScreen();
#endif
	theApp.setRunmode(theApp.pConfig->getCfgRunmode());

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDC_STATIC_ABOUT)->SetWindowTextA(
		STRVERINFO
		"\n"
		BUGFIXINFO 
		NMEAINFO 
		GARXFACEINFO);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void Cspec4App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Cspec4App message handlers


void Cspec4App::OnToolsChange()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Open Preferences Dialog"));

	CSetupDialog *SetupDialog = new CSetupDialog();
	SetupDialog->DoModal();
	delete SetupDialog;

}

void Cspec4App::OnToolsLoad()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Save custom configuration"));
	theApp.pConfig->Load((CString *)NULL);
}

void Cspec4App::OnToolsSave()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Save custom configuration"));
	theApp.pConfig->Save((CString *)NULL);

}

void Cspec4App::OnViewSelectdatafiles()
{
	// TODO: Add your command handler code here
	// @@@ this function should not be used ... the appropriate handler is on spec4Doc ... 
	theApp.ChangeStatusText(_T("Some sort of file selector / accumulator dialog"));
}



void Cspec4App::OnViewSavegraphinfo()
{
	// TODO: Add your command handler code here

	CFileDialog TempLoader(FALSE,_T(".gfg"), NULL, 0, "Graph Config Files (*.gfg)|*.gfg|All Files(*.*)|*.*||");
	TempLoader.m_pOFN->lpstrInitialDir=theApp.pConfig->getSavepathGFG();

	if (TempLoader.DoModal() == IDOK)
	{
		// we have a selection
		theApp.ChangeStatusText(_T("Selection OK"));
		theApp.pConfig->setSavepathGFG(TempLoader.GetPathName());
	}
	else
	{
		// we don't have a selection
		theApp.ChangeStatusText(_T("Selection Cancel"));
	}
}

void Cspec4App::OnViewLoadgraphinfo()
{
	CFileDialog TempLoader(TRUE,_T(".gfg"), NULL, 0, "Graph Config Files (*.gfg)|*.gfg|All Files(*.*)|*.*||");
	TempLoader.m_pOFN->lpstrInitialDir=theApp.pConfig->getSavepathGFG();

	if (TempLoader.DoModal() == IDOK)
	{
		// we have a selection
		theApp.ChangeStatusText(_T("Selection OK"));
		theApp.pConfig->setSavepathGFG(TempLoader.GetPathName());
	}
	else
	{
		// we don't have a selection
		theApp.ChangeStatusText(_T("Selection Cancel"));
	}
}

void Cspec4App::OnCollectEnteratl()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Dialog to enter ATL location"));
}

void Cspec4App::OnCollectGpstoatl()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Acquire GPS coordinates and convert to ATL"));
}

void Cspec4App::OnCollectLoaddefinition()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Load definitions for a substance of interest"));
}

void Cspec4App::OnCollectMeasure()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Acquire Spectrum"));
}

void Cspec4App::OnCollectSaveatl()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Save the ATL location of this sampling point (we will be back, right?"));
}

BOOL Cspec4App::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (CSplashDialog::PreTranslateAppMessage(pMsg))
		return true;

	return CWinApp::PreTranslateMessage(pMsg);
}

#if 0 // we don't have these menu items any longer ...
void Cspec4App::OnCalibrationCreate()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Acquire data for new calibration curve"));

	CCalibrationDialog *CalibrationDialog = new CCalibrationDialog();
	CalibrationDialog->DoModal();
	delete CalibrationDialog;
}

void Cspec4App::OnCalibrationLoad()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Prepopulate calibration info with pre-existing data"));
	theApp.pSpecControl->Calibration_Load();

}

void Cspec4App::OnCalibrationSave()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Save a *new* calibration file"));
	theApp.pSpecControl->Calibration_Save();
}
#endif

void Cspec4App::LogToFile(LPCTSTR text)
{
	SYSTEMTIME sysTime;
	CString tval;

	if (logpath.IsEmpty())
		SetStatusLogPath(NULL);

	GetLocalTime(&sysTime);
	tval.Format(_T("[%s] %04d-%02d-%02d %02d:%02d:%02d.%03d %s\n"),
		NetBIOSName.GetString(),
		sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds,
		text);

	try {
		logfile.Open(
			logpath.GetString(),
			CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::typeText);
		logfile.SeekToEnd();
		logfile.WriteString(tval);
		logfile.Close();
	}
	catch(CFileException* pe)
	{
		char szMsg[256];
		pe->GetErrorMessage(szMsg, sizeof(szMsg));
		MessageBox(NULL, szMsg, NULL, 0);
		pe->Delete();
	}
}

void Cspec4App::SetStatusLogPath(CString *path) 
{
	char strPathName[_MAX_PATH];

	if (path == NULL) {
// Traditionally we used to log to the application
// folder ... that works well for privileged users,
// not so well for unprivileged. New default is to
// log to My Documents/lightpilot_qb_runlog.txt
#if defined LOG_TO_APPFOLDER
		::GetModuleFileName(NULL, strPathName, _MAX_PATH);

		// The following code will allow you to get the path.
		logpath=strPathName;
		int fpos = logpath.ReverseFind('\\');

		if (fpos != -1)
		logpath = logpath.Left(fpos + 1);

		logpath.Append("runlog.txt");
#else
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, strPathName);
		logpath=strPathName;
		logpath.Append("\\lightpilot_qb_runlog.txt");
#endif
	} 
	else {
		logpath=*path;
	}

}

