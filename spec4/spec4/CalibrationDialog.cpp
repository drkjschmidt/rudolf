// CalibrationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "spec4.h"
#include "CalibrationDialog.h"


// CCalibrationDialog dialog

IMPLEMENT_DYNAMIC(CCalibrationDialog, CDialog)

CCalibrationDialog::CCalibrationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibrationDialog::IDD, pParent)
	, m_substanceName(_T(""))
# ifdef CD_USE_THREE
	, m_conc_1(_T(""))
	, m_conc_2(_T(""))
	, m_conc_3(_T(""))
#else
#endif
	, m_integrationTime(_T(""))
{

}

CCalibrationDialog::~CCalibrationDialog()
{
}

void CCalibrationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, c_specName);
	DDX_Text(pDX, IDC_EDIT2, m_substanceName);
	DDX_Control(pDX, IDC_COMBO3, c_integrationTime);
#ifdef CD_USE_THREE
	DDX_Text(pDX, IDC_EDIT1, m_conc_1);
	DDX_Control(pDX, IDC_EDIT1, c_conc_1);
	DDX_Text(pDX, IDC_EDIT3, m_conc_2);
	DDX_Control(pDX, IDC_EDIT3, c_conc_2);
	DDX_Text(pDX, IDC_EDIT4, m_conc_3);
	DDX_Control(pDX, IDC_EDIT4, c_conc_3);
	DDX_Control(pDX, IDC_COMBO1, c_conc_unit_1);
	DDX_Control(pDX, IDC_COMBO4, c_conc_unit_2);
	DDX_Control(pDX, IDC_COMBO5, c_conc_unit_3);
#else
	DDX_Control(pDX, IDC_LIST2, c_listControl);
	DDX_Control(pDX, IDC_EDIT1, c_concAVal);
	DDX_Control(pDX, IDC_COMBO1, c_concAUnit);
#endif
	DDX_Control(pDX, IDC_COMBO6, c_calibrationType);
	DDX_Control(pDX, IDC_COMBO7, c_wavelenSelect);
	DDX_Control(pDX, ID_FRM_SPEC, c_frameSpectrum);
	DDX_Control(pDX, IDC_FRM_CAL, c_frameCalibration);
	DDX_CBString(pDX, IDC_COMBO3, m_integrationTime);
	DDX_Control(pDX, IDC_COMBO8, c_scansToAverage);
	DDX_Control(pDX, IDC_COMBO9, c_boxcarWidth);
}


BEGIN_MESSAGE_MAP(CCalibrationDialog, CDialog)
#ifdef CD_USE_THREE
	ON_BN_CLICKED(IDC_BUTTON1, &CCalibrationDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalibrationDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalibrationDialog::OnBnClickedButton3)
#else
	ON_BN_CLICKED(IDC_BUTTON2, &CCalibrationDialog::OnBnClickedButtonAddPoint)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalibrationDialog::OnBnClickedButtonDeletePoint)
#endif
	ON_CBN_SELCHANGE(IDC_COMBO6, &CCalibrationDialog::OnCbnSelchangeFitType)
	ON_EN_CHANGE(IDC_COMBO7, &CCalibrationDialog::OnEnChangeWavelen)
END_MESSAGE_MAP()


// CCalibrationDialog message handlers

BOOL CCalibrationDialog::OnInitDialog()
{
	int ii;
	CRect myrect;
	CStringArray ports;
	CString tval;
	bool enableControls=(theApp.pSpecControl->GetCalSpecCount()<=0);

	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_substanceName=theApp.pSpecControl->getSubstanceName();

	c_concAUnit.SetCurSel(0);

	c_specName.ResetContent();
	c_specName.AddString(_T("Any"));
	for (ii=0; ii<theApp.pSpecControl->GetSpecCount(); ii++)
	{
		c_specName.AddString(theApp.pSpecControl->GetSpecName(ii).GetString());
	}
	c_specName.SetCurSel(0);
	c_specName.EnableWindow(enableControls);

	c_calibrationType.SetCurSel(0);

	// c_integrationTime.ResetContent();
	// c_integrationTime.AddString(_T("Auto Optimize"));
	// c_integrationTime.AddString(_T("50"));
	// tval.Format(_T("%d"),int(theApp.pSpecControl->GetMaxIntTime()/1000));
	// c_integrationTime.AddString(tval.GetString());
	// c_integrationTime.SetCurSel(0);
	// c_integrationTIme.SetWindowTextA(tval);

	tval.Format(_T("%g"),theApp.pSpecControl->Calibration_getIntegrationTime());
	c_integrationTime.SetWindowTextA(tval);
	c_integrationTime.EnableWindow(enableControls);

	tval.Format(_T("%d"),theApp.pSpecControl->Calibration_getSpectraToAverage());
	c_scansToAverage.SetWindowTextA(tval);
	c_scansToAverage.EnableWindow(enableControls);

	tval.Format(_T("%d"),theApp.pSpecControl->Calibration_getBoxcarWidth());
	c_boxcarWidth.SetWindowTextA(tval);
	c_boxcarWidth.EnableWindow(enableControls);


#ifdef CD_USE_THREE
#else
	/* 
	CRect rect;

	// Make sure we select the "whole" line ... this still does not
	// cover the space not taken up by subitems but it should be 
	// good enough for starters
	c_listControl.SetExtendedStyle(c_listControl.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	// preset column widths ... may not want even thirds later on but it's a start
	c_listControl.GetClientRect(&rect);
	int cw=rect.Width()/3;
	c_listControl.InsertColumn(0,_T("Conc"),0,cw);
	c_listControl.InsertColumn(1,_T("Units"),0,cw);
	c_listControl.InsertColumn(2,_T("Counts"),0,cw);
    */

	tval.Format(_T("%g"),theApp.pSpecControl->getFitWavelength());
	c_wavelenSelect.SetWindowTextA(tval.GetString());

	c_frameSpectrum.GetClientRect(&myrect);
	theApp.pSpecControl->PaintCSpectra(c_frameSpectrum.GetDC(),&myrect);

	c_frameCalibration.GetClientRect(&myrect);
	theApp.pSpecControl->PaintCalCurve(c_frameCalibration.GetDC(),&myrect);

	theApp.pSpecControl->SetupCalCurveList(&c_listControl,true);

#endif

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


#ifdef CD_USE_THREE
void CCalibrationDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	OnBnClicked_CalButton(0);
}

void CCalibrationDialog::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	OnBnClicked_CalButton(1);
}

void CCalibrationDialog::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	OnBnClicked_CalButton(2);
}

void CCalibrationDialog::OnBnClicked_CalButton(int idx)
{
	// TODO: Add your control notification handler code here
	RECT myrect;
	CString tval;
	int cursel;
	int value;

	// TODO: Add your control notification handler code here

	// we should read the integration time here and use it ...
	cursel=c_integrationTime.GetCurSel();
	c_integrationTime.GetWindowTextA(tval);
	if (! tval.CompareNoCase(_T("Auto Optimize")))
	{
		// @@@ a hack for now and a placeholder for later
		//     right now we abuse this to read the concentration
		//     value in place of integration time
		switch(idx) {
			case 0:
				c_conc_1.GetWindowTextA(tval);
				break;
			case 1:
				c_conc_2.GetWindowTextA(tval);
				break;
			case 2:
				c_conc_3.GetWindowTextA(tval);
				break;
		}
	} else {
		if (cursel != -1) {
			c_integrationTime.GetLBText(cursel,tval);
		} 
		// else {
		//	c_integrationTime.GetWindowTextA(tval);
		//}
	}
	value=atol(tval.GetString());


	if ((value<=0) || (value>theApp.pSpecControl->GetMaxIntTime())) {
		return;
	}


	// We use the currently selected spectrometer ... however, we 
	// should probably check whether the currently selected spectrometer
	// is the same as the "required" setting ...
	theApp.ChangeStatusText(_T("Acquiring Spectrum ... Please Wait ..."));
	theApp.pSpecControl->AcquireCalSpec(idx,value);
	theApp.ChangeStatusText(_T("Finished Acquiring Spectrum"));

	c_frameSpectrum.GetClientRect(&myrect);
	theApp.pSpecControl->PaintCSpectra(c_frameSpectrum.GetDC(),&myrect);

}
#else

void CCalibrationDialog::OnBnClickedButtonDeletePoint()
{
	UINT nsel;
	int csel;
	int dspecs[MAX_CAL_SPEC],dcnt=0,ii;

	RECT myrect;
	POSITION pos;
	CString tval;

	tval=_T("Delete ...");


	nsel = c_listControl.GetSelectedCount();
	tval.AppendFormat(_T(" nsel=%d"),nsel);

	tval.Append(_T("items:={"));
	pos = c_listControl.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		csel=c_listControl.GetNextSelectedItem(pos);
		dspecs[dcnt++]=csel;
		tval.AppendFormat(_T("%d,"),csel);
	}
	tval.Append(_T("}"));

	for (ii=dcnt-1; ii>-1; ii--)
		theApp.pSpecControl->RemoveCalSpec(dspecs[ii]);

	theApp.ChangeStatusText(tval.GetString());

	// (Re)draw calibration spectra
	c_frameSpectrum.GetClientRect(&myrect);
	theApp.pSpecControl->PaintCSpectra(c_frameSpectrum.GetDC(),&myrect);

	// (Re)draw calibration curve
	c_frameCalibration.GetClientRect(&myrect);
	theApp.pSpecControl->PaintCalCurve(c_frameCalibration.GetDC(),&myrect);

	// Repopulate and redraw information list
	theApp.pSpecControl->SetupCalCurveList(&c_listControl);

	bool enableControls=(theApp.pSpecControl->GetCalSpecCount()<=0);
	c_specName.EnableWindow(enableControls);
	c_integrationTime.EnableWindow(enableControls);
	c_scansToAverage.EnableWindow(enableControls);
	c_boxcarWidth.EnableWindow(enableControls);

}

void CCalibrationDialog::OnBnClickedButtonAddPoint()
{
	// TODO: Add your control notification handler code here
	// Add
	RECT myrect;
	CString tval;

	double conc;
	long itime,avg,boxw;
	double cwvl;

//	cursel=c_integrationTime.GetCurSel();
	c_integrationTime.GetWindowTextA(tval);
//	if (! tval.CompareNoCase(_T("Auto Optimize")))
//	{
//		// this needs some sort of coding ... not sure how yet?
//		itime=1000;
//	} else {
//		// this could code for time or selection
//		// but currently we don't pre-populate ... 
//		if (cursel != -1) {
//			c_integrationTime.GetLBText(cursel,tval);
//		} 
//		else {
			itime=(long)(atof(tval.GetString()));
//		}
//	}

	c_scansToAverage.GetWindowTextA(tval);
	avg=atol(tval.GetString());
	if (avg<1)
		avg=1;

	c_boxcarWidth.GetWindowTextA(tval);
	boxw=atol(tval.GetString());
	if (boxw<0)
		boxw=0;

	
	c_concAVal.GetWindowTextA(tval);
	conc=atof(tval.GetString());

	// c_concAUnit.GetWindowTextA(tval);
	// c_concAUnit.GetCurSel();

	// We use the currently selected spectrometer ... however, we 
	// should probably check whether the currently selected spectrometer
	// is the same as the "required" setting ...

	// Check that we still have space for more calibration spectra ...
	if (theApp.pSpecControl->GetCalSpecCount() >= MAX_CAL_SPEC)
	{
		MessageBox(_T("Maximum number of calibration spectra exceeded!"));
		return;
	}


	theApp.ChangeStatusText(_T("Acquiring Spectrum ... Please Wait ..."));
	theApp.pSpecControl->AcquireCalSpec(-1,conc,itime,avg,boxw);
	theApp.ChangeStatusText(_T("Finished Acquiring Spectrum"));

	// Set calibration curve wavelength
	c_wavelenSelect.GetWindowTextA(tval);
	cwvl=atoi(tval.GetString());
	if ((cwvl>=theApp.pSpecControl->getMinW()) && (cwvl<=theApp.pSpecControl->getMaxW()))
		theApp.pSpecControl->setFitWavelength(cwvl);
	// Set calibration curve type
	theApp.pSpecControl->setFitType((CCalibrationCurve::curveType)(c_calibrationType.GetCurSel()+2));

	// (Re)draw calibration spectra
	c_frameSpectrum.GetClientRect(&myrect);
	theApp.pSpecControl->PaintCSpectra(c_frameSpectrum.GetDC(),&myrect);

	// (Re)draw calibration curve
	c_frameCalibration.GetClientRect(&myrect);
	theApp.pSpecControl->PaintCalCurve(c_frameCalibration.GetDC(),&myrect);

	// Repopulate and redraw information list
	theApp.pSpecControl->SetupCalCurveList(&c_listControl);

	bool enableControls=(theApp.pSpecControl->GetCalSpecCount()<=0);
	c_specName.EnableWindow(enableControls);
	c_integrationTime.EnableWindow(enableControls);
	c_scansToAverage.EnableWindow(enableControls);
	c_boxcarWidth.EnableWindow(enableControls);
}

#endif



void CCalibrationDialog::OnCbnSelchangeFitType()
{
	RECT myrect;

	// Set calibration curve type
	theApp.pSpecControl->setFitType((CCalibrationCurve::curveType)(c_calibrationType.GetCurSel()+2));

	// Redraw calibration curve
	c_frameCalibration.GetClientRect(&myrect);
	theApp.pSpecControl->PaintCalCurve(c_frameCalibration.GetDC(),&myrect);

}

void CCalibrationDialog::OnEnChangeWavelen()
{
	RECT myrect;
	CString tval;
	double cwvl;

	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	// Set calibration curve wavelength
	c_wavelenSelect.GetWindowTextA(tval);
	cwvl=atoi(tval.GetString());
	if ((cwvl>=theApp.pSpecControl->getMinW()) && (cwvl<=theApp.pSpecControl->getMaxW()))
		theApp.pSpecControl->setFitWavelength(cwvl);
	// Set calibration curve type
	theApp.pSpecControl->setFitType((CCalibrationCurve::curveType)(c_calibrationType.GetCurSel()+2));

	// (Re)draw calibration spectra
	c_frameSpectrum.GetClientRect(&myrect);
	theApp.pSpecControl->PaintCSpectra(c_frameSpectrum.GetDC(),&myrect);

	// (Re)draw calibration curve
	c_frameCalibration.GetClientRect(&myrect);
	theApp.pSpecControl->PaintCalCurve(c_frameCalibration.GetDC(),&myrect);

	// Repopulate and redraw information list
	theApp.pSpecControl->SetupCalCurveList(&c_listControl);

}

void CCalibrationDialog::OnOK()
{
	CString tval;

	// ultimately we want to use a copy of the Calibration curve while
	// the dialog is open and in that case we would copy the data here
	//
	// as it stands we currently just update all values that would not
	// otherwise be updated already
	CDialog::OnOK();

	theApp.pSpecControl->setSubstanceName(m_substanceName);

	((CMainFrame *)theApp.m_pMainWnd)->DlgTop_SetName(m_substanceName);

	// This is a terrible hack because all spectra should be collected with
	// the same settings. However it has practical advantages not to enforce
	// this ... ;)
	// @@@ paranoia? 
	c_integrationTime.GetWindowTextA(tval);
	theApp.pSpecControl->Calibration_setIntegrationTime(atof(tval.GetString()));

	c_scansToAverage.GetWindowTextA(tval);
	theApp.pSpecControl->Calibration_setSpectraToAverage(atoi(tval.GetString()));

	c_boxcarWidth.GetWindowTextA(tval);
	theApp.pSpecControl->Calibration_setBoxcarWidth(atoi(tval.GetString()));

}


