// CustomerInfo.cpp : implementation file
//

#include "stdafx.h"
#include "spec4.h"
#include "CustomerInfoDialog.h"
#include "CustomerInfoData.h"
#include "GreatCircle.h"
#include "RPString.h"

#include <algorithm>
#include <vector>

// CCustomerInfoDialog dialog

IMPLEMENT_DYNAMIC(CCustomerInfoDialog, CDialog)

CCustomerInfoDialog::CCustomerInfoDialog(CWnd* pParent /*=NULL*/)
: CDialog(CCustomerInfoDialog::IDD, pParent)
, m_gpsLoc(_T(""))
, initMode(CCustomerInfoDialog::locinfo)
{

}

CCustomerInfoDialog::~CCustomerInfoDialog()
{
}

void CCustomerInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GPSLOC, m_gpsLoc);
	DDX_Control(pDX, IDC_COMPNAME, c_compName);
	DDX_Control(pDX, IDC_SAMPLOC, c_sampleLoc);
	DDX_Control(pDX, IDC_SAMPINFO, c_sampleInfo);

	DDX_Control(pDX, IDC_PIPEKM, c_sampleNumPipeKm);
	DDX_Control(pDX, IDC_DILUTION, c_sampleNumDilution);
	DDX_Control(pDX, IDC_USERDEF, c_sampleNumUserdef);
	DDX_Control(pDX, IDOK, c_okMeasureButton);
}


BEGIN_MESSAGE_MAP(CCustomerInfoDialog, CDialog)
	ON_BN_CLICKED(IDC_LOAD, &CCustomerInfoDialog::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_SAVE, &CCustomerInfoDialog::OnBnClickedSave)
	ON_CBN_SELCHANGE(IDC_COMPNAME, &CCustomerInfoDialog::OnCbnSelchangeCompname)
END_MESSAGE_MAP()



// CCustomerInfoDialog message handlers
typedef struct {
	double dist;
	CString fname;
} MYLOCINFO;
bool operator<(const MYLOCINFO &left,const MYLOCINFO &right) {
	if (left.dist<right.dist)
		return true;
	return false;
}

void CCustomerInfoDialog::OnBnClickedLoad()
{
	CString path;
	CString tval;

	path=theApp.pConfig->getSavepathLOC();
	CFileDialog TempLoader(TRUE,_T(".loc"), NULL, 0, "Location Info Files (*.loc)|*.loc|All Files(*.*)|*.*||");
	TempLoader.m_pOFN->lpstrInitialDir=path;

	if (TempLoader.DoModal() == IDOK)
	{
		// we have a selection
		theApp.ChangeStatusText(_T("Selection OK"));
		path=TempLoader.GetPathName();
		theApp.pConfig->setSavepathLOC(path);
	}
	else
	{
		// we don't have a selection
		theApp.ChangeStatusText(_T("Selection Cancel"));
		return;
	}

	CCustomerInfoData *tmpData = new CCustomerInfoData();
	if (tmpData->Load(&path)) {
		c_compName.SetWindowTextA(tmpData->getNameCust().GetString());
		c_sampleLoc.SetWindowTextA(tmpData->getNameLoc().GetString());
		c_sampleInfo.SetWindowTextA(tmpData->getNameSamp().GetString());

		tval.Format(_T("%g"),tmpData->getDataPipeKm());
		c_sampleNumPipeKm.SetWindowTextA(tval.GetString());
		tval.Format(_T("%g"),tmpData->getDataDilution());
		c_sampleNumDilution.SetWindowTextA(tval.GetString());
		c_sampleNumUserdef.SetWindowTextA(tmpData->getDataFF1().GetString());

		// @@@ add extra params (km, freeform, dilution)
	}
	delete tmpData;

	c_compName.UpdateData(FALSE);
	c_sampleLoc.UpdateData(FALSE);
	c_sampleInfo.UpdateData(FALSE);

	c_sampleNumPipeKm.UpdateData(FALSE);
	c_sampleNumDilution.UpdateData(FALSE);
	c_sampleNumUserdef.UpdateData(FALSE);
}

void CCustomerInfoDialog::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
	CString path,file;
	CRPString tval,uval,vval,line;
	DWORD pattr,perr;
	int ii;
	double lat,lon,alt;

	c_compName.GetWindowTextA(tval);
	c_sampleLoc.GetWindowTextA(uval);

	acqGPSInfo.getLat(&lat);
	acqGPSInfo.getLon(&lon);
	acqGPSInfo.getAlt(&alt);
	file.Format(_T("%.6f%1s-%.6f%1s-%-10s-%-10s.loc"),
		abs(lat),((lat<0)?(_T("S")):(_T("N"))),
		abs(lon),((lon<0)?(_T("W")):(_T("E"))),
		tval.GetReplaceWithUnderscores().Left(10),
		uval.GetReplaceWithUnderscores().Left(10));
	path=theApp.pConfig->getSavepathLOC();

	CFileDialog TempSaver(FALSE,_T(".loc"), file.GetString(), 0, "Location Info Files (*.loc)|*.loc|All Files(*.*)|*.*||");
	TempSaver.m_pOFN->lpstrInitialDir=path;
	// TempSaver.m_pOFN->lpstrInitialDir="C:\\";

	if (TempSaver.DoModal() == IDOK)
	{
		// we have a selection
		theApp.ChangeStatusText(_T("Selection OK"));
		path=TempSaver.GetPathName();
		file=TempSaver.GetFileName();
		theApp.pConfig->setSavepathLOC(path);

		ii=path.ReverseFind('\\');
		path=path.Mid(0,ii);
	}
	else
	{
		// we don't have a selection
		theApp.ChangeStatusText(_T("Selection Cancel"));
		return;
	}

	// Create path if needed
	pattr=GetFileAttributesA(path);
	if (pattr == 0xffffffff) 
	{
		perr=GetLastError();
		if ((perr == ERROR_PATH_NOT_FOUND) || 
			(perr == ERROR_FILE_NOT_FOUND))
				CreateDirectory(path.GetString(),NULL);
	}

	// Make sure file does not exist yet ... 
	path.Append(_T("\\"));
	path.Append(file);
	pattr=GetFileAttributesA(path);
	if ((pattr != 0xffffffff) && (pattr & FILE_ATTRIBUTE_DIRECTORY))
	{
		tval.Format(_T("Error: FILE path %s is a DIRECTORY"),path.GetString());
		MessageBox(tval.GetString(),_T("Error"));
		return;
	}
	if (pattr == 0xffffffff) 
	{
		perr=GetLastError();
		if (perr != ERROR_FILE_NOT_FOUND) {
			tval.Format(_T("Error: File exists or other problem (%ld)"), perr);
			MessageBox(tval.GetString(),_T("Error"));
			return;
		}
	}
	else
	{
		// @@@ file exists ... we need an on / cancel box here ... 
		if (MessageBoxA(
			_T("File exits. Do you want to overwrite it?"), 
			_T("Warning"), 
			MB_OKCANCEL | MB_ICONSTOP) != IDOK) 
				return;
	}

	CCustomerInfoData *tmpData = new CCustomerInfoData();

	c_compName.GetWindowTextA(tval);	tmpData->setNameCust(tval);
	c_sampleLoc.GetWindowTextA(tval);	tmpData->setNameLoc(tval);
	c_sampleInfo.GetWindowTextA(tval);	tmpData->setNameSamp(tval);

	c_sampleNumPipeKm.GetWindowTextA(tval);		tmpData->setDataPipeKmStr(tval);
	c_sampleNumDilution.GetWindowTextA(tval);	tmpData->setDataDilutionStr(tval);
	c_sampleNumUserdef.GetWindowTextA(tval);	tmpData->setDataFF1(tval);

	// tmpData->setGpsLoc(lat,lon,alt);
	tmpData->setGpsLoc(acqGPSInfo);
	tmpData->Save(&path);
	delete tmpData;
/*
	// now write data ...
	try {
		CStdioFile locfile(path.GetString(),CFile::modeCreate|CFile::modeWrite|CFile::typeText);

		c_compName.GetWindowTextA(tval);
		c_sampleLoc.GetWindowTextA(uval);
		c_sampleInfo.GetWindowTextA(vval);

		line=tval.GetEscaped();
		line.Append(_T("\n"));
		locfile.WriteString(line.GetString());

		line=uval.GetEscaped();
		line.Append(_T("\n"));
		locfile.WriteString(line.GetString());

		 
//		for (ii=0; ii<vval.GetLength(); ii++) {
//			cc=vval.GetAt(ii);
//			if ((('A'<=cc) && (cc<='Z')) ||
//				(('a'<=cc) && (cc<='z')) ||
//				(('0'<=cc) && (cc<='9')) ||
//				('-'==cc) ||
//				('_'==cc) ||
//				('.'==cc) ||
//				(' '==cc))
//			{
//			}
//			else {
//				vval.SetAt(ii,'_');
//			}
//		}
		

		// locfile.WriteString(vval.GetReplaceWithUnderscores().GetString());

		line=vval.GetEscaped();
		line.Append(_T("\n"));
		locfile.WriteString(line.GetString());

		// @@@ TEST
		c_sampleInfo.GetWindowTextA(vval);
		line=vval.GetEscaped();
		line=vval.GetUnEscaped();
		vval.Append((CString)line);
		c_sampleInfo.SetWindowTextA(vval);

		locfile.Close();
	}
	catch(CFileException* pe)
	{
		char szMsg[256];
		pe->GetErrorMessage(szMsg, sizeof(szMsg));
		MessageBox(szMsg, NULL, 0);
		pe->Delete();
	}
	*/
}




BOOL CCustomerInfoDialog::OnInitDialog()
{
	CString path,fsel;
	CFileFind finder;
	double curLat,curLon,curAlt;
	double fileLat,fileLon;
	CString tval,tlat,tlon;
	int tpos;
	double dist;
	int ii;


	MYLOCINFO myLocTmp;
	std::vector<MYLOCINFO> myLocArray;


	CDialog::OnInitDialog();

	// BEEP so the user knows to do something
	MessageBeep(MB_ICONEXCLAMATION);

	if (initMode == mode::locinfo) {
		// Get *current* GPS location ... so first clear all old info ... wait for at most 60s
		UINT uTimerId;
		MSG msg;
#define CUSTOMER_GPS_WAIT 60
#define CUSTOMER_GPS_TIME 100
		theApp.pGarminPort->updateInfo();
		if (theApp.pGarminPort->isValid()) {
			for (ii=0; ii<CUSTOMER_GPS_WAIT*(1000/CUSTOMER_GPS_TIME); ii++) 
			{
				tval.Format(_T("Optimizing GPS info quality ... (%.1fs)"),
					(double)(CUSTOMER_GPS_WAIT*(1000/CUSTOMER_GPS_TIME) - ii)/(1000/CUSTOMER_GPS_TIME)
					);
				theApp.ChangeStatusText(tval.GetString());

				theApp.pGarminPort->updateInfo(ii==0);

				if (theApp.pGarminPort->getName(NULL) && 
					theApp.pGarminPort->getLoc(NULL,NULL,NULL) &&
					theApp.pGarminPort->getTimeData(NULL,NULL)) {
					theApp.ChangeStatusText(_T("All GPS data up to date"));
					break;
				}

#if 0	// for some reason the timer doesn't sleep?
			// XSleep function from here: http://www.codeguru.com/Cpp/misc/misc/article.php/c211/
				#define UNIQUE_TIMER_ID 1

				uTimerId = this->SetTimer(UNIQUE_TIMER_ID, 1000, NULL);
				while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.hwnd == NULL && msg.message == WM_TIMER && msg.wParam == UNIQUE_TIMER_ID)
					break;
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				this->KillTimer(uTimerId); 
#else
				Sleep(100);
#endif
			}
		}
	}
	else
	{
		theApp.pGarminPort->updateInfo();
	}

	acqGPSInfo=theApp.pGarminPort->getGPSInfo();
	m_gpsLoc = acqGPSInfo.formatLoc() + acqGPSInfo.formatDate() + acqGPSInfo.formatTime();
	// theApp.pGarminPort->getLoc(&lat, &lon, &alt);
	// theApp.pGarminPort->getTimeData(&time,&date);
	// @@@ we should use some generic GPS function here !!!
	// m_gpsLoc.Format(_T("%.6fN %.6fW %.2fm %d %d"),lat,-lon,alt,time,date);



	acqGPSInfo.getLoc(&curLat,&curLon,&curAlt);
	// theApp.pGarminPort->updateInfo();
	// theApp.pGarminPort->getLoc(&curLat,&curLon,&curAlt);

	path=theApp.pConfig->getSavepathLOC();
	fsel=path;
	fsel.Append(_T("\\*.loc"));

	// Find all definition files ... 
	BOOL bWorking = finder.FindFile(fsel.GetString());;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		tval=finder.GetFileName();

		tpos=0;
		tlat=tval.Tokenize(_T("-"),tpos);
		tlon=tval.Tokenize(_T("-"),tpos); // @@@ I wonder if this will crash if tpos < 0?

		fileLat=atof(tlat.Left(tlat.GetLength()-1));
		if (! tlat.Right(1).CompareNoCase(_T("S")))
			fileLat=-fileLat;

		fileLon=atof(tlon.Left(tlon.GetLength()-1));
		if (! tlon.Right(1).CompareNoCase(_T("W")))
			fileLon=-fileLon;

		dist=CGreatCircle::distGPSHaversine(curLat,curLon,fileLat,fileLon);

		myLocTmp.dist=dist;
		myLocTmp.fname=tval;
		myLocArray.push_back(myLocTmp);

		// tlat.Format(_T("%f -- %s"),dist,tval.GetString());
		// c_compName.AddString(tlat.GetString());
	}

	// Extract information from the first 10 files we found ... 
	std::sort(myLocArray.begin(),myLocArray.end());
	locationArray.empty();
	CCustomerInfoData *tmpData=new CCustomerInfoData();
	for (ii=0; ii<10 && ii<(int)myLocArray.size(); ii++) {
		tval=path;
		tval.Append(_T("\\"));
		tval.Append(myLocArray[ii].fname);
		tmpData->Load(&(tval));

		c_compName.AddString(tmpData->getNameCust().GetString());
		c_sampleLoc.AddString(tmpData->getNameLoc().GetString());
	}
	delete tmpData;

	// If we already have an initialized dataset, populate the dialog with it ... 
	if (theApp.pCustInfoData->isValid()) {
		c_compName.SetWindowTextA(theApp.pCustInfoData->getNameCust());
		c_sampleLoc.SetWindowTextA(theApp.pCustInfoData->getNameLoc());
		c_sampleInfo.SetWindowTextA(theApp.pCustInfoData->getNameSamp());

		c_sampleNumPipeKm.SetWindowTextA(theApp.pCustInfoData->getDataPipeKmStr());
		c_sampleNumDilution.SetWindowTextA(theApp.pCustInfoData->getDataDilutionStr());
		c_sampleNumUserdef.SetWindowTextA(theApp.pCustInfoData->getDataFF1());
	}
	// ... otherwise set sane default values, particularly for dilution!
	else
	{
		c_sampleNumPipeKm.SetWindowTextA(CString(_T("0")));
		c_sampleNumDilution.SetWindowTextA(CString(_T("1")));
	}


	if (initMode == mode::locinfo) {
		this->SetWindowTextA(_T("Customer Info: Enter or Load presets"));

		c_compName.EnableWindow(TRUE);
		c_sampleLoc.EnableWindow(TRUE);
		c_sampleInfo.EnableWindow(TRUE);

		c_sampleNumPipeKm.EnableWindow(TRUE);
		c_sampleNumDilution.EnableWindow(FALSE);
		c_sampleNumUserdef.EnableWindow(TRUE);

		c_okMeasureButton.SetWindowTextA(_T("OK"));
	}
	if (initMode == mode::sampinfo) {
		this->SetWindowTextA(_T("Customer Info: Modify sample specific info"));

		c_compName.EnableWindow(FALSE);
		c_sampleLoc.EnableWindow(FALSE);
		c_sampleInfo.EnableWindow(TRUE);

		c_sampleNumPipeKm.EnableWindow(FALSE);
		c_sampleNumDilution.EnableWindow(TRUE);
		c_sampleNumUserdef.EnableWindow(TRUE);

		c_okMeasureButton.SetWindowTextA(_T("Measure"));
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomerInfoDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CString tval;

	CDialog::OnOK();

	c_compName.GetWindowTextA(nameCust);
	c_sampleLoc.GetWindowTextA(nameLoc);
	c_sampleInfo.GetWindowTextA(nameSamp);

	c_sampleNumPipeKm.GetWindowTextA(tval); dataPipeKm=atof(tval.GetString());
	c_sampleNumDilution.GetWindowTextA(tval); dataDilution=atof(tval.GetString());
	c_sampleNumUserdef.GetWindowTextA(dataFF1); 

	theApp.pCustInfoData->setNameCust(nameCust);
	theApp.pCustInfoData->setNameLoc(nameLoc);
	theApp.pCustInfoData->setNameSamp(nameSamp);
	theApp.pCustInfoData->setDataPipeKm(dataPipeKm);
	theApp.pCustInfoData->setDataDilution(dataDilution);
	theApp.pCustInfoData->setDataFF1(dataFF1);

	// @@@ another horrible GPS hack ...
	//double lat,lon,alt;
	//INT32 time,date;
	//theApp.pGarminPort->getLoc(&lat,&lon,&alt);
	//theApp.pGarminPort->getTimeData(&time,&date);
	//
	//theApp.pCustInfoData->setGpsLoc(lat,lon,alt);
	theApp.pCustInfoData->setGpsLoc(acqGPSInfo);

	theApp.pCustInfoData->setValid();
}

void CCustomerInfoDialog::OnCbnSelchangeCompname()
{
	// For now brute force this ... we can optimize for speed later ...
	CFileFind finder;

	int tpos;
	CString tval,uval;
	CString tloc;
	CString path,fsel;

	CCustomerInfoData *tmpData=new CCustomerInfoData();

	c_compName.GetLBText(c_compName.GetCurSel(),tloc);
	tloc.Truncate(min(tloc.GetLength(),10));
	tloc.MakeLower();

	c_sampleLoc.ResetContent();

	path=theApp.pConfig->getSavepathLOC();
	fsel=path;
	fsel.AppendFormat(_T("\\*.loc"),tloc);
	BOOL bWorking = finder.FindFile(fsel.GetString());;
	while (bWorking)
	{

		bWorking = finder.FindNextFile();
		if (! finder.IsDirectory()) {
			tval=finder.GetFileName();

			tpos=0;
			uval=tval.Tokenize(_T("-"),tpos); // lat
			uval=tval.Tokenize(_T("-"),tpos); // lon
			uval=tval.Tokenize(_T("-"),tpos); // cust

			uval.MakeLower();
			if (! strncmp(tloc.GetString(),uval.GetString(),min(tloc.GetLength(),uval.GetLength())))
			{
				uval=path;
				uval.Append(_T("\\"));
				uval.Append(tval);
				tmpData->Load(&(uval));

				c_sampleLoc.AddString(tmpData->getNameLoc());
			}
		}
	}

	c_sampleLoc.SetCurSel(0);
	UpdateData(FALSE);

	delete tmpData;
}
