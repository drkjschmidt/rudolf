#include "StdAfx.h"
#include "spec4.h"
#include "CustomerInfoData.h"

CCustomerInfoData::CCustomerInfoData(void)
: isvalid(false)
, nameCust(_T(""))
, nameLoc(_T(""))
, nameSamp(_T(""))
, dataPipeKm(0)
, dataDilution(1)
, dataFF1(_T(""))
, dataFF2(_T(""))
{
}

CCustomerInfoData::~CCustomerInfoData(void)
{
}

bool CCustomerInfoData::Load(CString *fname)
{
	CString line,buffer;
	CString filename;
	int lpos=0,llpos=0;

	if (fname == NULL) {

		CFileDialog TempLoader(TRUE,_T(".loc"), NULL, 0, "Customer Info Files (*.loc)|*.loc|All Files(*.*)|*.*||");
		TempLoader.m_pOFN->lpstrInitialDir=theApp.pConfig->getSavepathLOC();

		if (TempLoader.DoModal() == IDOK)
		{
			// we have a selection
			theApp.ChangeStatusText(_T("Selection OK"));
			filename=TempLoader.GetPathName();
			theApp.pConfig->setSavepathLOC(filename);
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
		//if (! calfile.Open(filename.GetString(),CFile::modeRead|CFile::typeText))
		//	return false;

		// Implicit open using constructor. This is not pretty
		// but makes the try / catch error handling much prettier
		CStdioFile calfile(filename.GetString(),CFile::modeRead|CFile::typeText);

		while (calfile.ReadString(line))
			buffer.AppendFormat(_T("%s\n"), line);

		deserializeData(buffer,lpos,llpos);

		calfile.Close();
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

bool CCustomerInfoData::Save(CString *fname=NULL)
{
	CString filename;

	if (fname == NULL) {

		CFileDialog TempSaver(FALSE,_T(".loc"), NULL, 0, "Customer Information Files (*.loc)|*.loc|All Files(*.*)|*.*||");
		TempSaver.m_pOFN->lpstrInitialDir=theApp.pConfig->getSavepathLOC();

		if (TempSaver.DoModal() == IDOK)
		{
			// we have a selection
			theApp.ChangeStatusText(_T("Selection OK"));
			filename=TempSaver.GetPathName();
			theApp.pConfig->setSavepathLOC(filename);
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
		//if (! calfile.Open(filename.GetString(),calfile.modeCreate|calfile.modeWrite|calfile.typeText))
		//	return false;

		// Implicit open using constructor. This is not pretty
		// but makes the try / catch error handling much prettier
		CStdioFile calfile(filename.GetString(),CFile::modeCreate|CFile::modeWrite|CFile::typeText);
		

		// @@@ we don't have any sanity checking here for whether the calibration curve is valid

		calfile.WriteString(serializeData(CSpectrumData::outputCSV).GetString());

		calfile.Close();
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

// this will generate multi-line output!
CString 
CCustomerInfoData::serializeData(unsigned char mode)
{
	CString tval(_T(""));
	double lat,lon,alt;

	if (mode & this->outputCSV)
	{
		tval.Append(      _T(TEXT_CUST_START    "\n"));
		tval.AppendFormat(_T(TEXT_CUST_VERSION  ",%f\n"),1.0);

		// tval.AppendFormat(_T(TEXT_CUST_GPSLOC   ",%f,%f,%f\n"),lat,lon,alt);
		acqGPSInfo.getLat(&lat);
		acqGPSInfo.getLon(&lon);
		acqGPSInfo.getAlt(&alt);
		tval.AppendFormat(_T(TEXT_CUST_GPSLOC   ",%f,%f,%f\n"),lat,lon,alt);

		tval.AppendFormat(_T(TEXT_CUST_NAMECUST ",%s\n"),((CRPString)nameCust).GetEscaped());
		tval.AppendFormat(_T(TEXT_CUST_NAMELOC  ",%s\n"),((CRPString)nameLoc ).GetEscaped());
		tval.AppendFormat(_T(TEXT_CUST_NAMESAMP ",%s\n"),((CRPString)nameSamp).GetEscaped());

		tval.AppendFormat(_T(TEXT_CUST_INF_KM   ",%f\n"),dataPipeKm);
		tval.AppendFormat(_T(TEXT_CUST_INF_DIL  ",%f\n"),dataDilution);
		tval.AppendFormat(_T(TEXT_CUST_INF_FF1  ",%s\n"),((CRPString)dataFF1).GetEscaped());
		tval.AppendFormat(_T(TEXT_CUST_INF_FF2  ",%s\n"),((CRPString)dataFF2).GetEscaped());

		tval.Append(_T(TEXT_CUST_END "\n"));

	} 
	else if (mode & this->outputXML)
	{
		// currently not supported
	}

	return tval;
}

// TODO: I suspect I will have to read data files in some sort of 
// blocks ... also there is the issue of returning the end of the 
// spectrum for the next spectrum ... and probably this should actually
// return a spectrum data instance instead of overwriting the global data ...
bool CCustomerInfoData::deserializeData(CString data, int &lpos, int &llpos)
{
	int tpos,tcnt;
	CString line;
	CString tokens[MAX_TOKENS];
	double lat,lon,alt;

	double version;

	// removed this in favor of passing position in and out of the function ... 
	// lpos=0;
	while (lpos > -1)
	{
		llpos=lpos;
		line=data.Tokenize(_T("\n"),lpos);

		for (tcnt=0, tpos=0; (tpos > -1) && (tcnt<MAX_TOKENS); tcnt++)
			tokens[tcnt]=line.Tokenize(_T(","),tpos);

		if (! tokens[0].CompareNoCase(_T(TEXT_CUST_START)))
			;

		if (! tokens[0].CompareNoCase(_T(TEXT_CUST_VERSION)))
		{
			version=atof(tokens[1].GetString());
			if (version != 1.0)
				return false;
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_CUST_GPSLOC)))
		{
			lat=atof(tokens[1].GetString());
			lon=atof(tokens[2].GetString());
			alt=atof(tokens[3].GetString());
			acqGPSInfo.setData(&lat,&lon,&alt,NULL,NULL,NULL,NULL,(char *)NULL);

		}

		if (! tokens[0].CompareNoCase(_T(TEXT_CUST_NAMECUST)))
			nameCust=((CRPString)tokens[1]).GetUnEscaped();

		if (! tokens[0].CompareNoCase(_T(TEXT_CUST_NAMELOC )))
			nameLoc=((CRPString)tokens[1]).GetUnEscaped();

		if (! tokens[0].CompareNoCase(_T(TEXT_CUST_NAMESAMP)))
			nameSamp=((CRPString)tokens[1]).GetUnEscaped();

		if (! tokens[0].CompareNoCase(_T(TEXT_CUST_INF_KM)))
			dataPipeKm=atof(tokens[1].GetString());

		if (! tokens[0].CompareNoCase(_T(TEXT_CUST_INF_DIL)))
			dataDilution=atof(tokens[1].GetString());

		if (! tokens[0].CompareNoCase(_T(TEXT_CUST_INF_FF1 )))
			dataFF1=((CRPString)tokens[1]).GetUnEscaped();

		if (! tokens[0].CompareNoCase(_T(TEXT_CUST_INF_FF2 )))
			dataFF2=((CRPString)tokens[1]).GetUnEscaped();

		if (! tokens[0].CompareNoCase(_T(TEXT_CUST_END)))
			return true;

	}

	// we should never get to here ...
	return false;
}

