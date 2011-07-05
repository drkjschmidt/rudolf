// DlgBarAcq.cpp : implementation file
//

#include "stdafx.h"
#include "spec4.h"
#include "DlgBarAcq.h"


// CDlgBarAcq dialog

IMPLEMENT_DYNAMIC(CDlgBarAcq, CDialogBar)

CDlgBarAcq::CDlgBarAcq()
: laststage(DLG_LFT_RESET)
, saved(true)
, paintopt_D_baseline(false)
, paintopt_D_baseline_ovr(false)
, paintopt_D_raw(false)
, paintopt_D_diff(true)
// , s_BnDlgLeftAcqCl(true)
// , s_BnDlgLeftAcqScal(false)
// , s_BnDlgLeftAcqBlank(false)
// , s_BnDlgLeftAcqScan(false)
// , s_BnDlgLeftAcqSave(false)
{

}

CDlgBarAcq::~CDlgBarAcq()
{
}

void CDlgBarAcq::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgBarAcq, CDialogBar)
	// according to http://support.microsoft.com/kb/185672
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )

	// according to http://support.microsoft.com/kb/152376
	ON_UPDATE_COMMAND_UI(IDC_BN_ACQ_CL,          &CDlgBarAcq::OnBnUpdatedBnAcqCl)
	ON_BN_CLICKED(       IDC_BN_ACQ_CL,          &CDlgBarAcq::OnBnClickedBnAcqCl)
	ON_COMMAND(          ID_DLGLFT_CTRL_F1,      &CDlgBarAcq::OnBnClickedBnAcqCl)

	// according to http://support.microsoft.com/kb/152376
	ON_UPDATE_COMMAND_UI(IDC_BN_ACQ_SCAL,        &CDlgBarAcq::OnBnUpdatedBnAcqScal)
	ON_BN_CLICKED(       IDC_BN_ACQ_SCAL,        &CDlgBarAcq::OnBnClickedBnAcqScal)
	ON_COMMAND(          ID_DLGLFT_CTRL_F2,      &CDlgBarAcq::OnBnClickedBnAcqScal)

	// according to http://support.microsoft.com/kb/152376
	ON_UPDATE_COMMAND_UI(IDC_BN_ACQ_BLANK,       &CDlgBarAcq::OnBnUpdatedBnAcqBlank)
	ON_BN_CLICKED(       IDC_BN_ACQ_BLANK,       &CDlgBarAcq::OnBnClickedBnAcqBlank)
	ON_COMMAND(          ID_DLGLFT_CTRL_F3,      &CDlgBarAcq::OnBnClickedBnAcqBlank)

	// according to http://support.microsoft.com/kb/152376
	ON_UPDATE_COMMAND_UI(IDC_BN_ACQ_SCAN,        &CDlgBarAcq::OnBnUpdatedBnAcqScan)
	ON_BN_CLICKED(       IDC_BN_ACQ_SCAN,        &CDlgBarAcq::OnBnClickedBnAcqScan)
	ON_COMMAND(          ID_DLGLFT_CTRL_F4,      &CDlgBarAcq::OnBnClickedBnAcqScan)

	// according to http://support.microsoft.com/kb/152376
	ON_UPDATE_COMMAND_UI(IDC_BN_ACQ_SAVE,        &CDlgBarAcq::OnBnUpdatedBnAcqSave)
	ON_BN_CLICKED(       IDC_BN_ACQ_SAVE,        &CDlgBarAcq::OnBnClickedBnAcqSave)
	ON_COMMAND(          ID_DLGLFT_CTRL_F5,      &CDlgBarAcq::OnBnClickedBnAcqSave)

END_MESSAGE_MAP()


// CDlgBarAcq message handlers
LONG CDlgBarAcq::OnInitDialog ( UINT wParam, LONG lParam)
{
	
	BOOL bRet = HandleInitDialog(wParam, lParam);

	if (!UpdateData(FALSE))
	{
	   TRACE0("Warning: UpdateData failed during dialog init.\n");
	}

	return bRet;
}

BOOL CDlgBarAcq::InitDialog(void)
{
	// UpdateData(FALSE);
	// SetWindowText("Wzd Dialog Bar");
	return TRUE;
}


void CDlgBarAcq::OnBnUpdatedBnAcqCl(CCmdUI* pCmdUI)
{
	// pCmdUI->Enable( s_BnDlgLeftAcqCl );
	pCmdUI->Enable( laststage >= DLG_LFT_RESET );
}

void CDlgBarAcq::OnBnClickedBnAcqCl()
{
	// For now call the function located in MainFrm.cpp
	// ((CMainFrame *)AfxGetMainWnd())->DlgLeft_OnBnClickedBnAcq_Handler(DLG_LFT_CL);
	OnBnClickedBnAcq_Handler(DLG_LFT_CL);
}


void CDlgBarAcq::OnBnUpdatedBnAcqScal(CCmdUI* pCmdUI)
{
	// pCmdUI->Enable( s_BnDlgLeftAcqScal );
	pCmdUI->Enable( laststage >= DLG_LFT_CL );
}

void CDlgBarAcq::OnBnClickedBnAcqScal()
{
	// For now call the function located in MainFrm.cpp
	// ((CMainFrame *)AfxGetMainWnd())->DlgLeft_OnBnClickedBnAcq_Handler(DLG_LFT_SCAL);
	OnBnClickedBnAcq_Handler(DLG_LFT_SCAL);
}


void CDlgBarAcq::OnBnUpdatedBnAcqBlank(CCmdUI* pCmdUI)
{
	// pCmdUI->Enable( s_BnDlgLeftAcqBlank );
	pCmdUI->Enable( laststage >= DLG_LFT_SCAL );
}

void CDlgBarAcq::OnBnClickedBnAcqBlank()
{
	// For now call the function located in MainFrm.cpp
	// ((CMainFrame *)AfxGetMainWnd())->DlgLeft_OnBnClickedBnAcq_Handler(DLG_LFT_BLANK);
	OnBnClickedBnAcq_Handler(DLG_LFT_BLANK);
}


void CDlgBarAcq::OnBnUpdatedBnAcqScan(CCmdUI* pCmdUI)
{
	// pCmdUI->Enable( s_BnDlgLeftAcqScan );
	pCmdUI->Enable( laststage >= DLG_LFT_BLANK );
}

void CDlgBarAcq::OnBnClickedBnAcqScan()
{
	// For now call the function located in MainFrm.cpp
	// ((CMainFrame *)AfxGetMainWnd())->DlgLeft_OnBnClickedBnAcq_Handler(DLG_LFT_SCAN);
	OnBnClickedBnAcq_Handler(DLG_LFT_SCAN);
}


void CDlgBarAcq::OnBnUpdatedBnAcqSave(CCmdUI* pCmdUI)
{
	// pCmdUI->Enable( s_BnDlgLeftAcqSave );
	pCmdUI->Enable( laststage >= DLG_LFT_SCAN );
}

void CDlgBarAcq::OnBnClickedBnAcqSave()
{
	// For now call the function located in MainFrm.cpp
	// ((CMainFrame *)AfxGetMainWnd())->DlgLeft_OnBnClickedBnAcq_Handler(DLG_LFT_SAVE);
	OnBnClickedBnAcq_Handler(DLG_LFT_SAVE);
}


void CDlgBarAcq::OnBnClickedBnAcq_Handler(int stage)
{
	CString tval;
	int curstage=stage;
	CString fname;
	int res;

	if (curstage > laststage + 1)
		curstage = laststage + 1;

	// Make sure we don't throw away last spectrum ...
	// ... hoping the recursive call here will do ...
	if ((! saved) && (curstage!=DLG_LFT_SAVE)) {
		while (! saved) {
			res=MessageBoxA(
				_T("Last spectrum not saved. Do you want to save now?"), 
				_T("Sample Not Saved"), 
				MB_YESNOCANCEL | MB_ICONEXCLAMATION);
			switch (res) {
				case IDYES:
					OnBnClickedBnAcq_Handler(DLG_LFT_SAVE);
					break;
				case IDNO:
					saved=true;
					break;
				default:
					return;
			}
		}
	}

	// Set drawing override for baseline spectrum during acquisition ...
	// this could be done below but it more obvious like this
	//
	// @@@ this is ugly because data spectrum does not get erased ...
	theApp.pSpecControl->PaintOpt_SetDSpecBaselineOverride(curstage == DLG_LFT_BLANK);

	switch (curstage) {
		case DLG_LFT_RESET:
			break;
		case DLG_LFT_CL: // Get customer information
			theApp.pCustInfoDlg->setMode(CCustomerInfoDialog::locinfo);
			if (theApp.pCustInfoDlg->DoModal() != IDOK)
				return;

			// c_wndDlgBarLeft.GetDlgItem(IDC_ST_ACQ_CUSTOMER)->SetWindowTextA(_T("yes yes yes"));
			// c_wndDlgBarLeft.GetDlgItem(IDC_ST_ACQ_LOC)->SetWindowTextA(_T("no no no"));
			// c_wndDlgBarLeft.GetDlgItem(IDC_ST_ACQ_GPS)->SetWindowTextA(_T("xx N yy W"));
			GetDlgItem(IDC_ST_ACQ_CUSTOMER)->SetWindowTextA(theApp.pCustInfoData->getNameCust());
			GetDlgItem(IDC_ST_ACQ_LOC)->SetWindowTextA(theApp.pCustInfoData->getNameLoc());
			GetDlgItem(IDC_ST_ACQ_GPS)->SetWindowTextA(theApp.pCustInfoData->getGpsString());
			GetDlgItem(IDC_ST_ACQ_SI)->SetWindowTextA(theApp.pCustInfoData->getNameSamp());
			break;
		case DLG_LFT_SCAL: // Select calibration curve.
			// !!! // replaced with local call to load function ...
			// theApp.OnCalibrationLoad();
			// !!! // I think this only tested for valid cal curve loaded ... 
			// if (! theApp.pSpecControl->GetCalSpec(theApp.pSpecControl->GetCurSpec(),NULL))
			if (!(CalDataAcq.Load() && CalDataAcq.isValid()))
				return;

			// @@@ not sure this is appropriate ... 
			CalDataAcq.CalcBestFit();
			((CMainFrame *)theApp.m_pMainWnd)->m_DlgTop.setWarnLow(CalDataAcq.getWarnLow());
			((CMainFrame *)theApp.m_pMainWnd)->m_DlgTop.setWarnHigh(CalDataAcq.getWarnHigh());
			((CMainFrame *)theApp.m_pMainWnd)->m_DlgTop.setErrLow(CalDataAcq.getErrLow());
			((CMainFrame *)theApp.m_pMainWnd)->m_DlgTop.setErrHigh(CalDataAcq.getErrHigh());
			tval.Format("%gms / %dx / [%d]",
				CalDataAcq.getIntegrationTime(),
				CalDataAcq.getSpectraToAverage(),
				CalDataAcq.getBoxcarWidth());
			GetDlgItem(IDC_ST_ACQ_CP)->SetWindowTextA(
				tval.GetString());
			// @@@ this is another terrible hack ... this should probably happen
			// in the calibration load function !!!
			// !!! // setting these values here means we have to reset acquisition after mode change!
			theApp.pSpecControl->SetIntegrationTime(CalDataAcq.getIntegrationTime());
			theApp.pSpecControl->SetScansToAverage(CalDataAcq.getSpectraToAverage());
			theApp.pSpecControl->SetBoxcarWidth(CalDataAcq.getBoxcarWidth());

			GetDlgItem(IDC_ST_ACQ_CN)->SetWindowTextA(
				CalDataAcq.getSubstanceName().GetString());
			GetDlgItem(IDC_ST_ACQ_CN_F)->SetWindowTextA(
				CalDataAcq.getFitInfo(CCalibrationCurve::FIT_FORMULA).GetString());
			GetDlgItem(IDC_ST_ACQ_CN_R)->SetWindowTextA(
				CalDataAcq.getFitInfo(CCalibrationCurve::FIT_RSQUARED).GetString());
			break;
		case DLG_LFT_BLANK: // Acquire blank spectrum
			if (MessageBoxA(
				_T("Please insert ZERO CONCENTRATION sample"), 
				_T("Sample Change Operation"), 
				MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK) {
					// Acquire blank spectrum
					AcquireBaselineSpectrum();
					AfxGetMainWnd()->Invalidate();
			}
			else {
				// don't increment stage because we didn't succeed ...
				return;
			}
			break;
		case DLG_LFT_SCAN:
			// If we want to be paranoid about the stupidity of the user,
			// Display a dialog box asking for sample change
			if (theApp.pConfig->getAcqVerb()) {
				if (MessageBoxA(
					_T("Please insert MEASUREMENT sample"), 
					_T("Sample Change Operation"), 
					MB_OKCANCEL | MB_ICONEXCLAMATION) != IDOK)
					return;
			} 

			// Allow user to customize information for this sample
			theApp.pCustInfoDlg->setMode(CCustomerInfoDialog::sampinfo);
			if (theApp.pCustInfoDlg->DoModal() != IDOK)
				return;
			// @@@ what happens if we make changes and then hit cancel?!?
			GetDlgItem(IDC_ST_ACQ_CUSTOMER)->SetWindowTextA(theApp.pCustInfoData->getNameCust());
			GetDlgItem(IDC_ST_ACQ_LOC)->SetWindowTextA(theApp.pCustInfoData->getNameLoc());
			GetDlgItem(IDC_ST_ACQ_GPS)->SetWindowTextA(theApp.pCustInfoData->getGpsString());
			GetDlgItem(IDC_ST_ACQ_SI)->SetWindowTextA(theApp.pCustInfoData->getNameSamp());

			// Acquire blank spectrum
			AcquireDataSpectrum();

			// theApp.pSpecControl->Spectrum_StoreComments(
			LastSpec.storeComments(
				&(theApp.pCustInfoData->getNameLoc()),
				&(theApp.pCustInfoData->getNameCust()),
				&(theApp.pCustInfoData->getNameSamp()),
				&(theApp.pCustInfoData->getDataPipeKmStr()),
				&(theApp.pCustInfoData->getDataDilutionStr()),
				&(theApp.pCustInfoData->getDataFF1()),
				&(theApp.pCustInfoData->getDataFF2()));
			AfxGetMainWnd()->Invalidate();

			// Set PPM information in top and side bars
			GetDlgItem(IDC_ST_ACQ_RES)->SetWindowTextA(UpdatePPM());

			// Memorize that we have acquired but not saved yet
			saved=false;

			// Customization: if we break here, the user has to manually save the just-acquired
			// spectrum. If don't, then the user is automatically presented with a save dialog.
			//
			if (! theApp.pConfig->getAutosaveImmediate())
				break;
		case DLG_LFT_SAVE:
			// c_wndDlgBarLeft.GetDlgItem(IDC_ST_ACQ_FN)->SetWindowTextA(_T("SavNam"));
			if (SaveAllSpectra(&fname)) {
				saved=true;
				GetDlgItem(IDC_ST_ACQ_FN)->SetWindowTextA(fname.GetString());
			}
			break;
		default:
			break;
	}

	laststage = curstage;
}


bool 
CDlgBarAcq::AcquireDataSpectrum() 
{
	DoubleArray tempdata;
	double lat,lon,alt;
	//INT32 time,date;
	CTime timestamp;
	SYSTEMTIME curtime;

	if (theApp.pSpecControl->Spectrum_AcquireSpectrum(&tempdata)) {
		LastSpec.storeDatatype(CString(_T("RAW DATA")));
		LastSpec.storeAcqInfo(
			CalDataAcq.getIntegrationTime(),
			CalDataAcq.getSpectraToAverage(),
			CalDataAcq.getBoxcarWidth());
		LastSpec.storeCalInfo(
			CalDataAcq.getSubstanceName(),
			CalDataAcq.getUFID(),
			CalDataAcq.GetCalWavelength(),
			CalDataAcq.getFitType(),
			CalDataAcq.getFitParms(),
			CalDataAcq.getRSquare(),
			CalDataAcq.getCount(),
			CalDataAcq.getCalConcArray(),
			CalDataAcq.getCalIntnArray(),
			CalDataAcq.getWarnLow(),
			CalDataAcq.getWarnHigh(),
			CalDataAcq.getErrLow(),
			CalDataAcq.getErrHigh());

		// @@@ more ugly GPS hacking
		theApp.pGPS->getLoc(&lat,&lon,&alt);
		LastSpec.storeGpsLoc(lat,lon,alt);

		theApp.pGPS->getTimeData(&timestamp);
		LastSpec.storeGpsTime(timestamp);

		GetSystemTime(&curtime);
		LastSpec.storeSysTime(curtime);

		LastSpec.storeSpecName(theApp.pSpecControl->GetSpecName(theApp.pSpecControl->GetCurSpec()));
		LastSpec.storeSpecSerial(theApp.pSpecControl->GetSpecSerial(theApp.pSpecControl->GetCurSpec()));

		DoubleArray wavelengths;
		theApp.pSpecControl->GetWavelengths(&wavelengths);
		return LastSpec.store(theApp.pSpecControl->GetCurSpec(),&tempdata,&wavelengths);
	}
	return false;
}

bool 
CDlgBarAcq::AcquireBaselineSpectrum() 
{
	DoubleArray tempdata;
	double lat,lon,alt;
	// INT32 time,date;
	CTime timestamp;
	SYSTEMTIME curtime;

	if (theApp.pSpecControl->Spectrum_AcquireSpectrum(&tempdata)) {
		BaselineSpec.storeDatatype(CString(_T("BASELINE")));
		BaselineSpec.storeAcqInfo(
			CalDataAcq.getIntegrationTime(),
			CalDataAcq.getSpectraToAverage(),
			CalDataAcq.getBoxcarWidth());
		BaselineSpec.storeCalInfo(
			CalDataAcq.getSubstanceName(),
			CalDataAcq.getUFID(),
			CalDataAcq.GetCalWavelength(),
			CalDataAcq.getFitType(),
			CalDataAcq.getFitParms(),
			CalDataAcq.getRSquare(),
			CalDataAcq.getCount(),
			CalDataAcq.getCalConcArray(),
			CalDataAcq.getCalIntnArray(),
			CalDataAcq.getWarnLow(),
			CalDataAcq.getWarnHigh(),
			CalDataAcq.getErrLow(),
			CalDataAcq.getErrHigh());
		// @@@ more ugly GPS hacking
		theApp.pGPS->getLoc(&lat,&lon,&alt);
		BaselineSpec.storeGpsLoc(lat,lon,alt);

		theApp.pGPS->getTimeData(&timestamp);
		BaselineSpec.storeGpsTime(timestamp);

		GetSystemTime(&curtime);
		BaselineSpec.storeSysTime(curtime);

		BaselineSpec.storeSpecName(theApp.pSpecControl->GetSpecName(theApp.pSpecControl->GetCurSpec()));
		BaselineSpec.storeSpecSerial(theApp.pSpecControl->GetSpecSerial(theApp.pSpecControl->GetCurSpec()));

		DoubleArray wavelengths;
		theApp.pSpecControl->GetWavelengths(&wavelengths);
		return BaselineSpec.store(theApp.pSpecControl->GetCurSpec(),&tempdata,&wavelengths);
	}
	return false;
}

bool CDlgBarAcq::SaveAllSpectra(CString *fname)
{
	CString filename;
	// CSpectrumData DiffSpec;
	CDSpectrumData tSpec;

	if (!(BaselineSpec.is_valid()&&LastSpec.is_valid()))
		return false;

	// DiffSpec=LastSpec - BaselineSpec;
	tSpec.specBaseline = BaselineSpec;
	tSpec.specRaw      = LastSpec;
	tSpec.specDiff     = LastSpec - BaselineSpec;
	tSpec.specDiff.storeDatatype(CString(_T("DIFF DATA")));

	return tSpec.Save(fname);
}

// Note that this functionality *is* logically duplicated in
// DlgBarTop. This is ugly, but 
CString CDlgBarAcq::FormatPPM(double undilppm,double dilution) {
	CString tval;
	CRPString uval;

	// This is an error condition and should never happen, 
	// but the only reasonable interpretation of '0'
	// dilution would be NO dilution ...
	if (dilution==0)
		dilution=1;

	// If we have a dilution factor, prepend the (calculated)
	// diluted concentration
	if (dilution!=1) {
		tval.Append(uval.ppxFormat(undilppm/dilution));
		tval.AppendFormat(_T(" * %.1f = "),dilution);
	}

	// Append the measured concentration
	tval.AppendFormat(uval.ppxFormat(undilppm));

	return tval;
}

CString CDlgBarAcq::UpdatePPM()
{
	double in;
	double wv=CalDataAcq.GetCalWavelength();
	double in2;
	double wv2=CalDataAcq.GetCalWavelength();
	double ppm,dil;
	CString tval;

	LastSpec.getXY(&wv,&in);
	BaselineSpec.getXY(&wv2,&in2);
	BaselineSpec.storeConc(0);

	// Get the dilution information from the spectrum data
	dil=LastSpec.getDilution();

	// Calculate the concentratio based on calibration curve
	// and multiply by dilution factor.
	ppm=CalDataAcq.bestFitXY(in,in2) * dil;

	// Create ppm string ... 
	// @@@ we still need to add some sort of mechanism for warnings ... 
	tval=FormatPPM(ppm,dil);

	// @@@ Ugly wrapping ... I wonder if this should point directly to the DialogBar method ...
	((CMainFrame *)(theApp.m_pMainWnd))->m_DlgTop.SetPPM(tval,ppm);

	// @@@ Is this the right place?
	LastSpec.storeConc(ppm);

	return tval;
}

void 
CDlgBarAcq::PaintDSpectra(CDC *pDC, LPRECT myrect) 
{
	CPen black(PS_SOLID, 1, RGB(  0,  0,  0));
	CPen gray( PS_SOLID, 1, RGB(128,128,128));
	CPen red(  PS_SOLID, 3, RGB(255,  0,  0));
	bool redraw_axes=true;
	bool draw_vlines=true;

	CSpectrumData DiffSpec;
	double wmin=1e10,wmax=-1e10,imin=1e10,imax=-1e10;


	// Set up scale parameters ...
	if (BaselineSpec.is_valid() && (paintopt_D_baseline || paintopt_D_baseline_ovr) ) {
		wmin = min(wmin,BaselineSpec.getMinW());
		wmax = max(wmax,BaselineSpec.getMaxW());
		imin = min(imin,BaselineSpec.getMinI());
		imax = max(imax,BaselineSpec.getMaxI());
	}
	if (LastSpec.is_valid() && paintopt_D_raw) {
		wmin = min(wmin,LastSpec.getMinW());
		wmax = max(wmax,LastSpec.getMaxW());
		imin = min(imin,LastSpec.getMinI());
		imax = max(imax,LastSpec.getMaxI());
	}
	if ((BaselineSpec.is_valid()) && (LastSpec.is_valid()) && paintopt_D_diff) {
		DiffSpec=LastSpec - BaselineSpec;

		wmin = min(wmin,DiffSpec.getMinW());
		wmax = max(wmax,DiffSpec.getMaxW());
		imin = min(imin,DiffSpec.getMinI());
		imax = max(imax,DiffSpec.getMaxI());
	}

	if (BaselineSpec.is_valid() && (paintopt_D_baseline || paintopt_D_baseline_ovr) ) {
		BaselineSpec.PaintSpectrum(
			pDC,myrect,&gray,
			wmin,wmax,imin,imax,
			redraw_axes,draw_vlines);
		redraw_axes=false;
	}

	if (LastSpec.is_valid() && paintopt_D_raw) {
		LastSpec.PaintSpectrum(
			pDC,myrect,&black,
			wmin,wmax,imin,imax,
			redraw_axes,draw_vlines);
		redraw_axes=false;
	}

	if ((BaselineSpec.is_valid()) && (LastSpec.is_valid()) && paintopt_D_diff) {
		// @@@ This should really be a difference ... create appropriate
		// operator for CSpectrumData ...
		DiffSpec.PaintSpectrum(
			pDC,myrect,&red,
			wmin,wmax,imin,imax,
			redraw_axes,draw_vlines);
		redraw_axes=false;
	}

}