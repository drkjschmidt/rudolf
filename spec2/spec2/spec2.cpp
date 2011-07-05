// spec2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "spec2.h"
#include "MainFrm.h"
#include "SplashDialog.h"

#include "spec2Doc.h"
#include "spec2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "wrapper.h"

// Cspec2App

BEGIN_MESSAGE_MAP(Cspec2App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &Cspec2App::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// Cspec2App construction

Cspec2App::Cspec2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cspec2App object

Cspec2App theApp;


// Cspec2App initialization

BOOL Cspec2App::InitInstance()
{
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
		RUNTIME_CLASS(Cspec2Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(Cspec2View));
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

	
	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// --- CUT --- MOVE THIS TO CLASS
	CString StatusText;
	CString StatusLog;

	CWnd *pMWnd = AfxGetMainWnd();
	CMainFrame *pMain = (CMainFrame*) pMWnd;
	CDC *pDC = pMWnd->GetDC();

	LPRECT pDCbounds = new RECT;
	// UINT pDCflags=0;
	// pDC->GetBoundsRect(pDCbounds,pDCflags);
	pMWnd->GetClientRect(pDCbounds);

	// SPLASH SCREEN!
	CSplashDialog::ShowSplashScreen(NULL);

	CSize pDCsize = pDC->GetViewportExt();

	int ii,jj;


	StatusText="Start doing stuff here ... \n";
	StatusLog+=StatusText;
	pMain->ChangeStatusText(StatusText);

	JString SpecName;
	int SpecCount;
	DoubleArray SpecWavelengths;
	DoubleArray SpecIntensities;
	DoubleArray SpecDummy;
	double SpecFmin,SpecFmax,SpecFdel;
	int SpecFcount;

	Wrapper *pWrapper = new Wrapper();
	SpecCount = pWrapper->openAllSpectrometers();

	StatusText.Format("Found %d spectrometers ... \n",SpecCount);
	StatusLog+=StatusText;
	pMain->ChangeStatusText(StatusText);

	for (ii=0; ii<SpecCount; ii++) {
		SpecName=pWrapper->getName(ii);
		StatusText=SpecName.getASCII();
		StatusText+=" ...\n";
		StatusLog+=StatusText;
		pMain->ChangeStatusText(StatusText);


		SpecWavelengths = pWrapper->getWavelengths(ii);
		SpecFcount = SpecWavelengths.getLength();
		StatusText.Format("%d wavelengths ...\n",SpecFcount);
		StatusLog+=StatusText;
		pMain->ChangeStatusText(StatusText);


		if (SpecFcount > 0) {
			SpecFmin=SpecWavelengths.getDoubleValues()[0];
			SpecFmax=SpecWavelengths.getDoubleValues()[SpecWavelengths.getLength()-1];
			SpecFdel=(SpecFmax - SpecFmin)/(SpecFcount - 1);

			StatusText.Format("%g - %g delta %g ...\n", SpecFmin, SpecFmax, SpecFdel);
			StatusLog+=StatusText;
			pMain->ChangeStatusText(StatusText);
		}

		// Scan with light ... not sure how to turn light OFF?!?
		pWrapper->setStrobeEnable(ii,1);
		SpecIntensities = pWrapper->getSpectrum(ii);
		pWrapper->setStrobeEnable(ii,0);
		SpecDummy = pWrapper->getSpectrum(ii);

		for (ii=0; ii<SpecFcount-1; ii++)
		{
			pDC->MoveTo(ii,(int)(SpecIntensities.getDoubleValues()[ ii ]));
			pDC->LineTo(ii,(int)(SpecIntensities.getDoubleValues()[ii+1]));
		}

	}
	
	pDC->TextOutA(50,50,StatusLog);
	
	StatusText.Format("Bounds: L%ld R%ld T%ld B%ld ...\n",
		pDCbounds->left,
		pDCbounds->right,
		pDCbounds->top,
		pDCbounds->bottom);
	StatusLog=StatusText;
	pMain->ChangeStatusText(StatusText);

	StatusText.Format("Size: X%d Y%d...\n",
		pDCsize.cx,
		pDCsize.cy);
	StatusLog+=StatusText;
	pMain->ChangeStatusText(StatusText);

	pDC->TextOutA(50,100,StatusLog);

	
	
	// --- CUT ---

	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
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

// App command to run the dialog
void Cspec2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Cspec2App message handlers


BOOL Cspec2App::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (CSplashDialog::PreTranslateAppMessage(pMsg))
		return true;

	return CWinApp::PreTranslateMessage(pMsg);
}
