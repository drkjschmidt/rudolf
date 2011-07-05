// SetupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "spec4.h"
#include "SetupDialog.h"
#include "RPGarmCom.h"
#include "Cspec4Cfg.h"

// CSetupDialog dialog

IMPLEMENT_DYNAMIC(CSetupDialog, CDialog)

CSetupDialog::CSetupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDialog::IDD, pParent)
	, m_GPSName(_T(""))
	, m_GPSlat(_T(""))
	, m_GPSlon(_T(""))
	, m_GPSalt(_T(""))
	, m_SelectSpec(_T(""))
{

}

CSetupDialog::~CSetupDialog()
{
}

void CSetupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELECT_GPS, c_SetupGPS);
	DDX_Control(pDX, IDC_TEST_GPS, c_TestGPS);
	DDX_Text(pDX, IDC_GPS_NAME, m_GPSName);
	DDX_Text(pDX, IDC_LATITUDE, m_GPSlat);
	DDX_Text(pDX, IDC_LONGITUDE, m_GPSlon);
	DDX_Text(pDX, IDC_ALTITUDE, m_GPSalt);
	DDX_Control(pDX, IDC_SELECT_SPECTROMETER, c_SelectSpec);
	DDX_Control(pDX, IDC_SPECTEST, c_SpecFrame);
	DDX_CBString(pDX, IDC_SELECT_SPECTROMETER, m_SelectSpec);

	DDX_Control(pDX, IDC_CHK_GPS,     c_requireGPS);
	DDX_Control(pDX, IDC_CHK_SPEC,    c_requireSpec);
	DDX_Control(pDX, IDC_CHK_AUTOSV1, c_autosaveSpec);
	DDX_Control(pDX, IDC_EDIT_PASSWD, c_passwordText);
}


BEGIN_MESSAGE_MAP(CSetupDialog, CDialog)
	ON_BN_CLICKED(IDC_TEST_SPECTROMETER, &CSetupDialog::OnBnClickedTestSpectrometer)
	ON_CBN_SELCHANGE(IDC_SELECT_SPECTROMETER, &CSetupDialog::OnCbnSelchangeSelectSpectrometer)
	ON_BN_CLICKED(IDC_TEST_GPS, &CSetupDialog::OnBnClickedTestGps)
END_MESSAGE_MAP()


// CSetupDialog message handlers

BOOL CSetupDialog::OnInitDialog()
{
	int ii;
	CDialog::OnInitDialog();
	//CStringArray ports;
	// CGPSportList ports; // moved to class definiton
	CString tval;

	// TODO:  Add extra initialization here
	// theApp.pGarminPort->getGarminPorts(ports);
	ports=theApp.pGPS->getPorts();
	for (ii=0; ii<ports.size(); ii++)
	{
		switch(ports[ii].getType()) {
		case CGPSPortTypesSerialNMEA:
			tval=ports[ii].getPortName();
			break;
		case CGPSPortTypesSerialGarXface:
			tval=_T("Serial");
			break;
		case CGPSPortTypesUSBGarXface:
			tval=_T("USB");
			break;
		default:
			break;
		}
		tval=tval + _T(": ") + ports[ii].getDeviceName();
		c_SetupGPS.AddString(tval);
	}
	// @@@ c_SetupGPS.SetCurSel(0);
	// c_SetupGPS.SetWindowTextA(theApp.pConfig->getGPS());
	c_SetupGPS.SetCurSel(0);

	for (ii=0; ii<theApp.pSpecControl->GetSpecCount(); ii++)
	{
		c_SelectSpec.AddString(theApp.pSpecControl->GetSpecName(ii).GetString());
	}
	c_SelectSpec.SetCurSel(0);

	c_passwordText.SetWindowTextA(theApp.pConfig->getPasswd().GetString());
	c_requireGPS.SetCheck(theApp.pConfig->getReqGPS()?BST_CHECKED:BST_UNCHECKED);
	c_requireSpec.SetCheck(theApp.pConfig->getReqSpec()?BST_CHECKED:BST_UNCHECKED);
	c_autosaveSpec.SetCheck(theApp.pConfig->getAutosaveImmediate()?BST_CHECKED:BST_UNCHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupDialog::OnBnClickedTestSpectrometer()
{
	RECT myrect;
	CString tval;
	int cursel;
	CBrush white(RGB(255,255,255));

	// TODO: Add your control notification handler code here

	//c_SelectSpec.UpdateData(false);
	cursel=c_SelectSpec.GetCurSel();
	if (cursel != -1) 
	{
		c_SelectSpec.GetLBText(cursel,tval);
		//c_SelectSpec.GetDlgItemTextA(IDC_SELECT_SPECTROMETER,tval);
		theApp.pSpecControl->SelectSpectrometer(tval);
	}

	// This should be unnecessary paranoia ... 
	if (theApp.pSpecControl->GetCurSpec() >= 0)
	{
		c_SpecFrame.GetClientRect(&myrect);
		// c_SpecFrame.GetDC()->FillRect(&myrect,&white);

		// !!! // // @@@ // Rewrite as part of splitting calibration curves into dialog bars etc.
		// theApp.pSpecControl->Spectrum_AcquireDataSpectrum();
		// theApp.pSpecControl->PaintDSpectra(c_SpecFrame.GetDC(),&myrect);

		// This could be done directly but using the wrapper is probably easier .. 
		CSpectrumData tspec;
		

		DoubleArray wavelengths,intensities;
		CPen blackp(PS_SOLID,1,RGB(  0,  0,  0));

		theApp.pSpecControl->Spectrum_AcquireSpectrum(&intensities);
		theApp.pSpecControl->GetWavelengths(&wavelengths);
		tspec.store(-1,&intensities,&wavelengths,0);

		tspec.PaintSpectrum(c_SpecFrame.GetDC(),&myrect,&blackp,false);
	}

}

void CSetupDialog::OnCbnSelchangeSelectSpectrometer()
{
	// TODO: Add your control notification handler code here
}

void CSetupDialog::OnBnClickedTestGps()
{
	CString tval,sname("[No GPS found]");
	char wait[]="Waiting for GPS to return name. This may take a minute ...";
	char loop[]=".oOo";
	int cursel;
	double lat=0, lon=0, alt=0;
	int ii;

	// TODO: Add your control notification handler code here

	//c_SelectSpec.UpdateData(false);
	//cursel=c_SetupGPS.GetCurSel();
	//c_SetupGPS.GetLBText(cursel,tval);
	m_GPSlat=_T("---");
	m_GPSlon=_T("---");
	m_GPSalt=_T("---");
	m_GPSName=_T("---");
	// c_SetupGPS.GetWindowTextA(tval);
	cursel=c_SetupGPS.GetCurSel();
	if ((cursel<0)||(cursel>=ports.size()))
		return;

	UpdateData(false);

	CGPSport GPSinfo=ports[cursel];
	if (theApp.pGPS->openPort(GPSinfo))
	{
		// theApp.pGPS->updateInfo(GPSinfo);
	
		theApp.ChangeStatusText(wait);
		for (ii=0; ; ii=(ii+1)%4) { // Loop variable for status text

			// Try re-reading GPS information ... quit loop if that fails!
			if (! theApp.pGPS->updateInfo(GPSinfo)) {
				theApp.ChangeStatusText(_T("No GPS found"));
				break;
			}

			// Quit the loop if we can read GPS name
			if (theApp.pGPS->getDeviceName(GPSinfo,&sname))
			{
				theApp.ChangeStatusText(_T("Found GPS"));

				theApp.pGPS->getLoc(GPSinfo, &lat, &lon, &alt);
				theApp.pGPS->getDeviceName(GPSinfo, &tval);

				m_GPSlat.Format("%f",lat);
				m_GPSlon.Format("%f",lon);
				m_GPSalt.Format("%f",alt);
				m_GPSName=sname;

				break;
			}

			// Change status text ...
			tval.Format(_T("%s %c"),wait,loop[ii]);
			theApp.ChangeStatusText(tval.GetString());
		}

		theApp.pGPS->closePort();
	}

	UpdateData(false);

}

void CSetupDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CString tval;
	int cursel;

	// c_SelectSpec.GetLBText(c_SelectSpec.GetCurSel(),tval);
	c_SelectSpec.GetWindowTextA(tval);
	theApp.pConfig->setSpectrometer(tval);

	// c_SetupGPS.GetLBText(c_SetupGPS.GetCurSel(),tval);
	// c_SetupGPS.GetWindowTextA(tval);
	cursel=c_SetupGPS.GetCurSel();
	if ((cursel>=0)&&(cursel<ports.size())) {
		theApp.pConfig->setGPSInfo(
			ports[cursel].getPortName(),
			ports[cursel].getDeviceName(),
			ports[cursel].getDeviceID());
	}

	// Password ...
	c_passwordText.GetWindowTextA(tval);
	theApp.pConfig->setPasswd(tval);

	// Read back checkboxes 
	// ... the code errs on the paranoid side, i.e. undefined == checked!
	theApp.pConfig->setReqGPS(c_requireGPS.GetCheck()!=BST_UNCHECKED);
	theApp.pConfig->setReqSpec(c_requireSpec.GetCheck()!=BST_UNCHECKED);
	theApp.pConfig->setAutosaveImmediate(c_autosaveSpec.GetCheck()!=BST_UNCHECKED);

	theApp.pConfig->Save();

	CDialog::OnOK();
}
