// spec4Doc.cpp : implementation of the Cspec4Doc class
//

#include "stdafx.h"
#include "conf_const.h"
#include "spec4.h"
#include "spec4View.h"
#include "spec4Doc.h"
#include "CheckPwd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cspec4Doc

IMPLEMENT_DYNCREATE(Cspec4Doc, CDocument)

BEGIN_MESSAGE_MAP(Cspec4Doc, CDocument)
//	ON_COMMAND(ID_CONTROLMODE_SWITCH, &Cspec4Doc::OnViewOthermenu)
	ON_COMMAND(ID_MODESET_CONTROL, &Cspec4Doc::OnViewControlMenu)
	ON_COMMAND(ID_MODESET_ACQ,     &Cspec4Doc::OnViewAcquisitionMenu)
	ON_COMMAND(ID_MODESET_REV,     &Cspec4Doc::OnViewReviewMenu)
	ON_COMMAND(ID_VIEW_SELECTDATAFILES, &Cspec4Doc::OnViewSelectdatafiles)
END_MESSAGE_MAP()


// Cspec4Doc construction/destruction

Cspec4Doc::Cspec4Doc()
: m_hMyMenu(0)
, m_hMenu1(0)
, m_hMenu2(0)
, m_hMenu3(0)
{
	// TODO: add one-time construction code here

}

Cspec4Doc::~Cspec4Doc()
{
	::DestroyMenu(m_hMenu1);
	::DestroyMenu(m_hMenu2);
}

BOOL Cspec4Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}




// Cspec4Doc serialization

void Cspec4Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// Cspec4Doc diagnostics

#ifdef _DEBUG
void Cspec4Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cspec4Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cspec4Doc commands
HMENU Cspec4Doc::GetDefaultMenu()
{
  return m_hMyMenu;    // just use original default
}

// We really should have no use for this anymore ...
void Cspec4Doc::OnViewOthermenu() {
	if (theApp.getRunmode() == RUNMODE_CONFIGURATION) 
		SetProgMode(RUNMODE_ACQUISITION);
	else
		SetProgMode(RUNMODE_CONFIGURATION);
}

void Cspec4Doc::OnViewControlMenu() {
	// @@@ should we put some sort of check here whether that switch is possible / legitimate?
	if (theApp.getRunmode() != RUNMODE_CONFIGURATION) 
		SetProgMode(RUNMODE_CONFIGURATION);
}

void Cspec4Doc::OnViewAcquisitionMenu() {
	// @@@ should we put some sort of check here whether that switch is possible / legitimate?
	if (theApp.getRunmode() != RUNMODE_ACQUISITION) 
		SetProgMode(RUNMODE_ACQUISITION);
}

void Cspec4Doc::OnViewReviewMenu() {
	// @@@ should we put some sort of check here whether that switch is possible / legitimate?
	if (theApp.getRunmode() != RUNMODE_REVIEW) 
		SetProgMode(RUNMODE_REVIEW);
}


void Cspec4Doc::SetProgMode(int askmode)
{
	int mode=RUNMODE_UNDEF;
	int curmode=theApp.getRunmode();
	bool warn_spec=false;
	bool warn_gps =false;
	CString tval;

	// TODO: Add your command handler code here

	// Clean up the mode we are leaving
	switch (curmode) {
		case RUNMODE_ACQUISITION:
			if (! ((CMainFrame *)AfxGetMainWnd())->DlgLeftAcq_Reset())
				return;
			break;
		case RUNMODE_CONFIGURATION:
			if (! ((CMainFrame *)AfxGetMainWnd())->DlgLeftCtrl_Reset())
				return;
			break;
	}

	// We do the sanity checking first to allow for downgrading mode
	// on startup and based on accessories we have (Spec, GPS)

	if (askmode != RUNMODE_REVIEW) {

		// Spectrometer is required for all modes other than review
		if (theApp.pConfig->getReqSpec()) {
			// nothing initialized yet ... have to play it save
			if (theApp.pSpecControl == NULL) {
				mode=RUNMODE_REVIEW;
			}
			// no spectrometers ... back to review mode
			else if (theApp.pSpecControl->GetSpecCount()<=0) {
				mode=RUNMODE_REVIEW;
				warn_spec=true;
			}
		}

		// GPS is only *required* in acquisition mode for documentation purposes
		if ((askmode == RUNMODE_ACQUISITION) && (theApp.pConfig->getReqGPS())) {
			if (theApp.pGPS == NULL) {
				mode=RUNMODE_REVIEW;
			} 
			// @@@ this is a bad hack and might not work so well
			// we should really allow some sort of override here ...
			// or implement a proper check for presence of GPS ... 
			else {
				// @@@ this used to have an isValid() check ... 
				if (theApp.pGPS->updateInfo()) {
					mode=RUNMODE_REVIEW;
					warn_gps=true;
				}
			}
		}

		tval.Empty();
		if (warn_spec)
			tval.Append(_T(
				"No spectrometers found! If a spectrometer\n"
				"is connected, try stopping application, unplugging\n"
				"spectrometer and plugging it back in before proceeding.\n"
				"\n"));

		if (warn_gps)
			tval.Append(_T(
				"No GPS found! Please ensure a GPS is connected\n"
				"and that Garmin Spanner software is running.\n"
				"Then restart application.\n"
				"\n"));

		if (! tval.IsEmpty()) {
			CSplashDialog::HideSplashScreen();
			tval.Append(_T("Meanwhile, only REVIEW mode will be available.\n"));
			MessageBox(NULL,tval.GetString(),_T("Error"),0);
		}
	}

	if ((mode == RUNMODE_UNDEF) && (askmode == RUNMODE_CONFIGURATION)) {
		// @@@ Password protection coming ...
		CCheckPwd *pwdDialog = new CCheckPwd();

		if ((pwdDialog->DoModal() == IDOK) && 
			(pwdDialog->getPass().Compare(theApp.pConfig->getPasswd()) == 0))
		{
			mode=askmode;
		}
		else {
			mode=RUNMODE_ACQUISITION;
		}

		delete pwdDialog;
	}
	else if (mode == RUNMODE_UNDEF) {
		mode=askmode;
	}

	// now load / select menus and dialog bars
	switch (mode) {
		case RUNMODE_ACQUISITION:
			if (m_hMenu2 == NULL) {
				// m_hMenu1 = (HMENU)AfxGetMainWnd()->GetMenu();
				m_hMenu2 = ::LoadMenu(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME_ACQ));
			}
			m_hMyMenu = m_hMenu2;

			theApp.setRunmode(mode,false);
			((CMainFrame *)AfxGetMainWnd())->DlgTop_Show();
			// ((CMainFrame *)AfxGetMainWnd())->DlgLeft_Show();
			((CMainFrame *)AfxGetMainWnd())->DlgLeftAcq_Show();
			((CMainFrame *)AfxGetMainWnd())->DlgLeftCtrl_Hide();
			((CMainFrame *)AfxGetMainWnd())->DlgLeftRev_Hide();
			((Cspec4View *)(((CMainFrame *)AfxGetMainWnd())->GetActiveView()))->setView(Cspec4View::VIEW_DATA_ACQ);
			break;
		case RUNMODE_CONFIGURATION:
			if (m_hMenu1 == NULL) {
				m_hMenu1 = ::LoadMenu(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
			}
			m_hMyMenu = m_hMenu1;
			theApp.setRunmode(mode,false);
			((CMainFrame *)AfxGetMainWnd())->DlgTop_Hide();
			// ((CMainFrame *)AfxGetMainWnd())->DlgLeft_Hide();
			((CMainFrame *)AfxGetMainWnd())->DlgLeftAcq_Hide();
			((CMainFrame *)AfxGetMainWnd())->DlgLeftCtrl_Show();
			((CMainFrame *)AfxGetMainWnd())->DlgLeftRev_Hide();
			((Cspec4View *)(((CMainFrame *)AfxGetMainWnd())->GetActiveView()))->setView(Cspec4View::VIEW_PANELS_CAL_CSP);
			break;
		case RUNMODE_REVIEW:
			if (m_hMenu3 == NULL) {
				m_hMenu3 = ::LoadMenu(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME_REV));
			}
			m_hMyMenu = m_hMenu3;
			theApp.setRunmode(mode,false);
			((CMainFrame *)AfxGetMainWnd())->DlgTop_Hide();
			// ((CMainFrame *)AfxGetMainWnd())->DlgLeft_Hide();
			((CMainFrame *)AfxGetMainWnd())->DlgLeftAcq_Hide();
			((CMainFrame *)AfxGetMainWnd())->DlgLeftCtrl_Hide();
			((CMainFrame *)AfxGetMainWnd())->DlgLeftRev_Show();
			((Cspec4View *)(((CMainFrame *)AfxGetMainWnd())->GetActiveView()))->setView(Cspec4View::VIEW_DATA_REV);
			break;
	}
	((CFrameWnd*)AfxGetMainWnd())->OnUpdateFrameMenu(NULL);
	AfxGetMainWnd()->DrawMenuBar();
}

void Cspec4Doc::OnViewSelectdatafiles()
{
	AddDataFiles(true);
}

// @@@ UGLY ... not sure who should own what but right now *adding* files
// is done here but removing in done in review dialog ...
void Cspec4Doc::AddDataFiles(bool clear)
{
	CString line,buffer;
	CString filename;
	int lpos=0,llpos=0;
	CString tval,success,fail;
	CDSpectrumData tspec;

	// @@@ we really need a wrapper here ...
	if (clear)
		theApp.pSpecControl->RevSpecs.clear();

	CFileDialog TempLoader(TRUE,_T(".csv"), NULL, OFN_ALLOWMULTISELECT,
		"Spectrum Files (*.spm)|*.spm|"
		"Spectrum Files (*.spc)|*.spc|"
		"Spectrum Files (*.csv)|*.csv|"
		"All Files(*.*)|*.*|"
		"|");
	TempLoader.m_pOFN->lpstrInitialDir=theApp.pConfig->getSavepathSPM();

	if (TempLoader.DoModal() == IDOK)
	{
		// we have a selection
		theApp.ChangeStatusText(_T("Selection OK"));
		filename=TempLoader.GetPathName();
		theApp.pConfig->setSavepathSPM(filename);
	}
	else
	{
		// we don't have a selection
		theApp.ChangeStatusText(_T("Selection Cancel"));
		return;
	}


	POSITION pos (TempLoader.GetStartPosition());
	while(pos)
	{
		filename=TempLoader.GetNextPathName(pos);
		buffer.Empty();

		tval.Format(_T("Loading spectrum %s"),filename.GetString());
		theApp.ChangeStatusText(tval);

		// tspec = new CDSpectrumData();
		if (tspec.Load(&filename)) {
			theApp.pSpecControl->RevSpecs.push_back(tspec);
			success.AppendFormat(_T("%s\n"),filename.GetString());
		} 
		else {
			fail.AppendFormat(_T("%s\n"),filename.GetString());
		}
		// delete tspec;

		/* 
		try {

			//if (! calfile.Open(filename.GetString(),CFile::modeRead|CFile::typeText))
			//	return false;

			// Implicit open using constructor. This is not pretty
			// but makes the try / catch error handling much prettier
			CStdioFile calfile(filename.GetString(),CFile::modeRead|CFile::typeText);


			while (calfile.ReadString(line))
				buffer.AppendFormat(_T("%s\n"), line);

			calfile.Close();

			lpos=0; llpos=0;
			if (tspec.deserializeData(buffer,lpos,llpos))
			{
				// @@@ add the spectrum to array here ...
				theApp.pSpecControl->RevSpecs.push_back(tspec);
				success.AppendFormat(_T("%s\n"),filename.GetString());
			}
			else {
				fail.AppendFormat(_T("%s\n"),filename.GetString());
			}
		}
		catch(CFileException* pe)
		{
			char szMsg[256];
			pe->GetErrorMessage(szMsg, sizeof(szMsg));
			MessageBox(NULL, szMsg, NULL, 0);
			pe->Delete();

			fail.AppendFormat(_T("%s\n"),filename.GetString());
		}
		*/
	}

	// ((CMainFrame *)theApp.m_pMainWnd)->DlgLeft_ChangeName(substanceName);
	if (! fail.IsEmpty()) 
	{
		tval.Format(_T("Successfully loaded:\n\n%s\nFailed to load:\n\n%s"),success.GetString(), fail.GetString());
		MessageBox(NULL,tval.GetString(),_T("INFO"),0);
	}
}
