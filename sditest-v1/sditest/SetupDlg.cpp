// SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sditest.h"
#include "SetupDlg.h"

// Make sure you add this to include path ...
#include <enumser.h>

// CSetupDlg dialog

IMPLEMENT_DYNAMIC(CSetupDlg, CDialog)

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
	, m_SetupComPort(_T(""))
	, m_baudRate(_T(""))
	, m_baudRateStatic(_T(""))
	, m_DataPort(_T(""))
	, m_lat(_T(""))
	, m_lon(_T(""))
	, m_alt(_T(""))
	, m_gprmc(_T(""))
	, m_gpgga(_T(""))
	, m_pgrmt(_T(""))
{

}

CSetupDlg::~CSetupDlg()
{
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO2, m_SetupComPort);
	DDV_MaxChars(pDX, m_SetupComPort, 20);
	DDX_Control(pDX, IDC_COMBO2, c_SetupComPort);
	DDX_Text(pDX, IDC_EDIT1, m_baudRate);
	DDV_MaxChars(pDX, m_baudRate, 20);
	DDX_Control(pDX, IDC_EDIT1, c_baudRate);
	DDX_Text(pDX, IDC_STEXT1, m_baudRateStatic);
	DDX_Text(pDX, IDC_STEXT3, m_DataPort);
	DDX_Text(pDX, IDC_SLAT, m_lat);
	DDX_Text(pDX, IDC_SLON, m_lon);
	DDX_Text(pDX, IDC_SALT, m_alt);
	DDX_Text(pDX, IDC_GPRMC, m_gprmc);
	DDX_Text(pDX, IDC_GPGGA, m_gpgga);
	DDX_Text(pDX, IDC_PGRMT, m_pgrmt);
	DDX_Control(pDX, IDC_LIST1, c_listCtrl);
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CSetupDlg::OnEnChangeEdit1)
//	ON_CBN_SELCHANGE(IDC_COMBO2, &CSetupDlg::OnCbnSelchangeCombo2)
//	ON_CBN_EDITCHANGE(IDC_COMBO2, &CSetupDlg::OnCbnEditchangeCombo2)
//ON_CBN_EDITUPDATE(IDC_COMBO2, &CSetupDlg::OnCbnEditupdateCombo2)
//ON_CBN_CLOSEUP(IDC_COMBO2, &CSetupDlg::OnCbnCloseupCombo2)
ON_BN_CLICKED(IDC_BUTTON1, &CSetupDlg::OnBnClickedButton1)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CSetupDlg::OnLvnItemchangedList1)
END_MESSAGE_MAP()



// CSetupDlg message handlers

BOOL CSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_baudRate.Format("9600");
	c_SetupComPort.ResetContent();
	m_baudRateStatic="9600";
	CUIntArray ports;
	int	ii;
	char buf[20];

	if (CEnumerateSerial::UsingGetDefaultCommConfig(ports))
	{
		for (ii=0; ii<ports.GetSize(); ii++)
		{
			_snprintf(buf,20,"COM%d",ports[ii]);
			c_SetupComPort.AddString(buf);
		}
      
	}

	CRect rect;
	c_listCtrl.GetClientRect(&rect);
	int cw=rect.Width()/3;
	c_listCtrl.InsertColumn(0,_T("Col 1"),0,cw);
	c_listCtrl.InsertColumn(1,_T("Col 2"),0,cw);
	c_listCtrl.InsertColumn(2,_T("Col 3"),0,cw);

	LVITEM lvi;
	CString lvs;

	int jj;

	for (ii=0; ii<3; ii++) {

		lvi.mask = LVIF_TEXT;
		lvi.iItem = ii;

		for (jj=0; jj<3; jj++) {

			// lvs.Format(_T("ii %d jj %d"),ii,jj);
			lvs.Format(_T("%d ii %d jj %d"),rand(),ii,jj);
			lvi.iSubItem = jj;
			lvi.pszText=(LPTSTR)(LPCTSTR)(lvs);
			if (jj==0)
				c_listCtrl.InsertItem(&lvi);
			else
				c_listCtrl.SetItem(&lvi);
		}
	}

	c_listCtrl.SetExtendedStyle(c_listCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


//void CSetupDlg::OnCbnSelchangeCombo2()
//{
//	CString tempdata;
//
//	
//	tempdata=this->m_SetupComPort;
//	UpdateData();
//	tempdata=this->m_SetupComPort;
//
//	theApp.garminPort.setComPort(m_SetupComPort);
//	theApp.garminPort.setComPort("COM1");
//	// TODO: Add your control notification handler code here
//	if (theApp.garminPort.openComPort())
//	{
//		theApp.garminPort.readComPort(&tempdata);
//		m_DataPort=tempdata;
//		UpdateData(false);
//	}
//}





//void CSetupDlg::OnCbnCloseupCombo2()
//{
//	// TODO: Add your control notification handler code here
//	CString tempdata;
//
//	
//	tempdata=this->m_SetupComPort;
//	UpdateData();
//	tempdata=this->m_SetupComPort;
//
//	theApp.garminPort.setComPort(m_SetupComPort);
//	theApp.garminPort.setComPort("COM1");
//	// TODO: Add your control notification handler code here
//	if (theApp.garminPort.openComPort())
//	{
//		theApp.garminPort.readComPort(&tempdata);
//		m_DataPort=tempdata;
//		UpdateData(false);
//	}
//}

void CSetupDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString tempdata;

	
	tempdata=this->m_SetupComPort;
	UpdateData();
	tempdata=this->m_SetupComPort;

	theApp.garminPort.setComPort(m_SetupComPort);
	// theApp.garminPort.setComPort("COM1");
	// TODO: Add your control notification handler code here
	if (theApp.garminPort.openComPort())
	{
		double lat,lon,alt;

		theApp.garminPort.updateInfo();
		m_DataPort=theApp.garminPort.getIObuf();\

		theApp.garminPort.getloc(&lat,&lon,&alt);
		m_lat.Format("%f",lat);
		m_lon.Format("%f",lon);
		m_alt.Format("%f",alt);

		m_gprmc=theApp.garminPort.lastGPRMC.Format(_T("%H:%M:%S"));
		m_gpgga=theApp.garminPort.lastGPGGA.Format(_T("%H:%M:%S"));
		m_pgrmt=theApp.garminPort.lastPGRMT.Format(_T("%H:%M:%S"));

		UpdateData(false);

		
	}
}


void CSetupDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
