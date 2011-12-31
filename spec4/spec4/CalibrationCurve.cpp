#include "StdAfx.h"
#include <afxpriv.h>
#include "CalibrationCurve.h"
#include "spec4.h"
#include "RPGraphHelper.h"
#include "ugly_text_macros.h"

// we need these to do curve fitting using lapack
// #define SUPPRESSLAPACK
#include <math.h>
#if ! defined SUPPRESSLAPACK
#include <gmd.h>
#include <laslv.h>
#endif

CCalibrationCurve::CCalibrationCurve(void)
: type(CCalibrationCurve::curveType::Linear)
, calWavelength(700)
, calCount(0)
, calRSquare(0)
, calIntegrationTime(50)
, calSpectraToAverage(50)
, calBoxcarWidth(100)
, ppmWarnLow(0)
, ppmWarnHigh(0)
, ppmErrLow(0)
, ppmErrHigh(0)
{
}

CCalibrationCurve::~CCalibrationCurve(void)
{
}


bool CCalibrationCurve::Load(CString *fname)
{
	CString line,buffer;
	CString filename;
	int lpos=0,llpos=0;

	if (fname == NULL) {

		CFileDialog TempLoader(TRUE,_T(".cal"), NULL, 0, "Calibration Files (*.cal)|*.cal|All Files(*.*)|*.*||");
		TempLoader.m_pOFN->lpstrInitialDir=theApp.pConfig->getSavepathCAL();

		if (TempLoader.DoModal() == IDOK)
		{
			// we have a selection
			theApp.ChangeStatusText(_T("Selection OK"));
			filename=TempLoader.GetPathName();
			theApp.pConfig->setSavepathCAL(filename);
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


	((CMainFrame *)theApp.m_pMainWnd)->DlgTop_SetName(substanceName);
	return true;

}

//! Save calibration curve
//
bool CCalibrationCurve::Save(CString *fname)
{
	CString filename;

	if (fname == NULL) {

		CFileDialog TempSaver(FALSE,_T(".cal"), NULL, 0, "Calibration Files (*.cal)|*.cal|All Files(*.*)|*.*||");
		TempSaver.m_pOFN->lpstrInitialDir=theApp.pConfig->getSavepathCAL();

		if (TempSaver.DoModal() == IDOK)
		{
			// we have a selection
			theApp.ChangeStatusText(_T("Selection OK"));
			filename=TempSaver.GetPathName();
			theApp.pConfig->setSavepathCAL(filename);
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

		calfile.WriteString(serializeData(0).GetString());

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

//! Create serialized data for save function
CString CCalibrationCurve::serializeData(unsigned char modes)
{
	int ii;
	CString line;

	// @@@ honor modes flag ...

	line.Format(      _T(TEXT_CAL_FILE_START "\n"));
	line.AppendFormat(_T(TEXT_CAL_VERSION    ",%f\n"),1.0);
	line.AppendFormat(_T(TEXT_CAL_FILE_UFID  ",%s\n"),CRPString().CreateUniqueID());
	// we don't write plaintext any longer ...
	// line.AppendFormat(_T(TEXT_CAL_REQ_SPEC   ",%s,%s\n"),requiredSpectrometer.GetString(),requiredSpecSerial.GetString());
	// ... instead use an encrypted version of the same ... 
	line.AppendFormat(_T(TEXT_CAL_REQ_SPECEN ",%s,%s\n"),
		CRPString(requiredSpectrometer).PseudoHexEncode(),
		CRPString(requiredSpecSerial  ).PseudoHexEncode());
	line.AppendFormat(_T(TEXT_CAL_WAVELEN    ",%f\n"),calWavelength);
	line.AppendFormat(_T(TEXT_CAL_ITIME      ",%f\n"),calIntegrationTime);
	line.AppendFormat(_T(TEXT_CAL_N_AVG      ",%d\n"),calSpectraToAverage);
	line.AppendFormat(_T(TEXT_CAL_BOXCAR_W   ",%d\n"),calBoxcarWidth);
	line.AppendFormat(_T(TEXT_CAL_SUBS_NAME  ",%s\n"),substanceName.GetString());

	line.AppendFormat(_T(TEXT_CAL_PTS_WAVELN));
	for (ii=0; ii<calCount; ii++)
		line.AppendFormat(_T(",%g"),calConcValues[ii]);
	line.AppendFormat(_T("\n"));

	line.AppendFormat(_T(TEXT_CAL_PTS_INTENS));
	for (ii=0; ii<calCount; ii++)
		line.AppendFormat(_T(",%g"),calIntnValues[ii]);
	line.AppendFormat(_T("\n"));

	line.AppendFormat(_T(TEXT_CAL_PTS_RINTEN));
	for (ii=0; ii<calCount; ii++)
		line.AppendFormat(_T(",%g"),calRawIntnValues[ii]);
	line.AppendFormat(_T("\n"));

	line.AppendFormat(_T(TEXT_CAL_PTS_BASEIN ",%g\n"),baselineInt);

	line.AppendFormat(_T(TEXT_CAL_TYPE ",%d\n"),type);

	line.AppendFormat(_T(TEXT_CAL_PARMS));
	for (ii=0; ii<type; ii++)
		line.AppendFormat(_T(",%g"),calParms[ii]);
	line.AppendFormat(_T("\n"));

	line.AppendFormat(_T(TEXT_CAL_RSQUARE ",%f\n"),calRSquare);

	line.AppendFormat(_T(TEXT_CAL_WARN_LOW  ",%f\n"),ppmWarnLow );
	line.AppendFormat(_T(TEXT_CAL_WARN_HIGH ",%f\n"),ppmWarnHigh);
	line.AppendFormat(_T(TEXT_CAL_ERR_LOW   ",%f\n"),ppmErrLow  );
	line.AppendFormat(_T(TEXT_CAL_ERR_HIGH  ",%f\n"),ppmErrHigh );

	// Use CSpectrumData's serialize function. This is not very nice for
	// direct import into excel but it should be trivial to write an
	// excel macro to fix that problem
	for (ii=0; ii<calCount; ii++)
		line += CalSpec[ii].serializeData(CSpectrumData::outputCSV);

	// Make sure we finish the block ... just in case
	line.AppendFormat(_T(TEXT_CAL_FILE_END "\n"));

	return line;
}

bool CCalibrationCurve::deserializeData(CString data, int &lpos, int &llpos)
{
	int ii;
	int lspecs=0,xpos,xxpos;

	int tpos,tcnt;
	CString line;
	CString tokens[MAX_TOKENS];

	double version;

	// so we can tell if it was set ...
	int wcount = -1;
	
	CString tval;

	// removed in favor of passing lpos in and out ... 
	// lpos=0;
	while (lpos > -1)
	{

		llpos=lpos;
		line=data.Tokenize(_T("\n"),lpos);

		for (tcnt=0, tpos=0; (tpos > -1) && (tcnt<MAX_TOKENS); tcnt++)
			tokens[tcnt]=line.Tokenize(_T(","),tpos);

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_FILE_START)))
			;

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_VERSION)))
		{
			version=atof(tokens[1].GetString());
			if (version != 1.0)
				return false;
		}
		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_FILE_UFID)))
			uniqueID=tokens[1];

		if ((! tokens[0].CompareNoCase(_T(TEXT_CAL_REQ_SPEC  ))) ||
			(! tokens[0].CompareNoCase(_T(TEXT_CAL_REQ_SPECEN))) )
		{
			// Plain text version
			if (! tokens[0].CompareNoCase(_T(TEXT_CAL_REQ_SPEC  ))) {
				requiredSpectrometer=tokens[1];
				requiredSpecSerial=tokens[2];
			}
			// pseudocoded version
			else  
			{
				requiredSpectrometer=CRPString(tokens[1]).PseudoHexDecode();
				requiredSpecSerial  =CRPString(tokens[2]).PseudoHexDecode();
			}

			if (theApp.pSpecControl->SelectSpectrometer(requiredSpectrometer,requiredSpecSerial) < 0)
			{
				tval.Format(_T(
					"The spectrometer required for using this\n"
					"Calibration curve:\n"
					"\n"
					"%s (%s)\n"
					"\n"
					"is not available!\n"
					"Continue at own risk!"),
					requiredSpectrometer,
					requiredSpecSerial
				);
				MessageBox(NULL,tval.GetString(),_T("ERROR"),0);
			}
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_WAVELEN)))
			calWavelength=atof(tokens[1]);

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_ITIME)))
			calIntegrationTime=atof(tokens[1]);

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_N_AVG)))
			calSpectraToAverage=atoi(tokens[1]);

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_BOXCAR_W)))
			calBoxcarWidth=atoi(tokens[1]);

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_SUBS_NAME)))
			substanceName=tokens[1];

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_PTS_WAVELN)))
		{
			for (ii=1; ii<tcnt; ii++)
				calConcValues[ii-1]=atof(tokens[ii]);
			calCount=tcnt-2; // need to subtract first column and the extra from the for loop
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_PTS_INTENS)))
		{
			for (ii=1; ii<tcnt; ii++)
				calIntnValues[ii-1]=atof(tokens[ii]);
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_PTS_RINTEN)))
		{
			for (ii=1; ii<tcnt; ii++)
				calRawIntnValues[ii-1]=atof(tokens[ii]);
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_PTS_BASEIN)))
			baselineInt=atof(tokens[1]);

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_TYPE)))
			type=(CCalibrationCurve::curveType)atol(tokens[1]);

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_PARMS)))
		{
			for (ii=1; ii<tcnt; ii++)
				calParms[ii-1]=atof(tokens[ii]);
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_RSQUARE)))
			calRSquare=atof(tokens[1]);

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_WARN_LOW)))
			ppmWarnLow=atof(tokens[1]);
		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_WARN_HIGH)))
			ppmWarnHigh=atof(tokens[1]);
		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_ERR_LOW)))
			ppmErrLow=atof(tokens[1]);
		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_ERR_HIGH)))
			ppmErrHigh=atof(tokens[1]);

		// Use CSpectrumData's deserialize function. 
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_START)))
		{
			xpos=xxpos=0;
			if (! CalSpec[lspecs].deserializeData(data.Mid(llpos),xpos,xxpos))
				return false;

			lspecs++;
			lpos+=xxpos;
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_CAL_FILE_END)))
		{
			// Paranoia ... in one of the older version we had a problem with correctly 
			// selecting the calibration wavelength because the wavelength response of the
			// spectrometer is not linear. These false data then got written into the 
			// Calibration curve ... generate a warning if we are reading one of those!
			if (calCount > 0) {
				double tin=calRawIntnValues[0];
				SetCalWavelength(calWavelength);
				if (tin != calRawIntnValues[0]) {
					theApp.ChangeStatusText(_T("WARNING: abstract calibration parameters don't match data. Fixing!"));
					MessageBox(NULL,_T(
						"Warning: the abstract data in this calibration curve do not match\n"
						"included raw data! To fix this problem, please resave calibration.\n"
						"\n"
						"Proceeding with data updated from raw values.\n"),_T("Warning"),0);

				}
			}

			// This is the OK exit point for loading a calibration curve
			return true;
		}

	}
	return false;
}

bool CCalibrationCurve::acquire(int idx, double set_conc, double integration, long average, long boxwidth) {

	DoubleArray data;
	DoubleArray wavelengths;
	CSpectrumData tspec;

	double lat,lon,alt;
	// INT32 time,date;
	CTime timestamp;
	SYSTEMTIME curtime;

	if (calCount > 0) 
	{
		if ((integration != calIntegrationTime) || 
			(average     != calSpectraToAverage) ||
			(boxwidth    != calBoxcarWidth))
		return false;
	}
	else 
	{
		calIntegrationTime  = integration;
		calSpectraToAverage = average;
		calBoxcarWidth      = boxwidth;
	}

	// pWrapper->setIntegrationTime(curSpectrometer,integration);
	theApp.pSpecControl->SetIntegrationTime(integration);
	theApp.pSpecControl->SetScansToAverage(average);
	theApp.pSpecControl->SetBoxcarWidth(boxwidth);
	// @@@ there is a potential problem in the behavior of 
	// this function *if* we were to switch between control and acquisition modes 
	// because the *default* acquisition parameters get reset here ... 
	// ... I believe this is currently compensated for in DlgBarAcq during the 
	// loading of the calibration curve ... 
	if (theApp.pSpecControl->Spectrum_AcquireSpectrum(&data))
	{
		// CalSpec[idx].store(curSpectrometer,&data,&SpecWavelengths);
		tspec.storeDatatype(CString(_T("CALIBRATION")));

		theApp.pSpecControl->GetWavelengths(&wavelengths);

		// ... as opposed to the other acquire spectrum functions we don't 
		// bother setting calibration information here because it won't be 
		// meaningful until the whole calibration file is saved anyhow.

		// @@@ more ugly GPS hacking
		// @@@ even more ugly now we have CGPSport ... 
		theApp.pGPS->getLoc(&lat,&lon,&alt);
		tspec.storeGpsLoc(lat,lon,alt);

		theApp.pGPS->getTimeData(&timestamp);
		tspec.storeGpsTime(timestamp);

		GetSystemTime(&curtime);
		tspec.storeSysTime(curtime);

		tspec.storeSpecName(theApp.pSpecControl->GetSpecName(theApp.pSpecControl->GetCurSpec()));
		tspec.storeSpecSerial(theApp.pSpecControl->GetSpecSerial(theApp.pSpecControl->GetCurSpec()));

		tspec.store(
			theApp.pSpecControl->GetCurSpec(),
			&data,
			&wavelengths,
			set_conc);

		// @@@ buggy ...
		if ((idx > -1) && (idx < getCount())) { 
			replace(idx, tspec); 
		} else {
			if (add(tspec) < 0)
				return false;
		}
		return true;
	}
	return false;
}

int CCalibrationCurve::add(CSpectrumData data)
{
	if (calCount >= MAX_CAL_SPEC-1)
		return -1;

	// @@@ Paranoia says: this could go wrong in multithreading
	calCount++;
	return replace(calCount-1, data);
}

int CCalibrationCurve::replace(int idx, CSpectrumData data)
{
	double wavelength,intensity;

	// Allow idx to be calCount (i.e. a *new* spectrum)
	// so we can use this for ADD

	if (idx > calCount-1)
		return -1;

	if (calCount >= MAX_CAL_SPEC)
		return -1;

	wavelength=calWavelength;
	data.getXY(&wavelength,&intensity);
	calConcValues[idx]=data.getConc();
	calRawIntnValues[idx]=intensity;
	calIntnValues[idx]=intensity-calRawIntnValues[0]; // @@@ Added this to deal with list view until we have a baseline ... maybe we should just subtract [0]?
	CalSpec[idx]=data;
	// We return with an error if an attempt is made to
	// *replace* a non-existent spectrum ...
	// calCount++; //@@ what if we are actually replacing?

	CalcBestFit();
	return idx;
}

CSpectrumData *CCalibrationCurve::get(int idx)
{
	if ((idx<0) || (idx>=calCount))
		return NULL;

	return &CalSpec[idx];
}

bool CCalibrationCurve::remove(int idx)
{
	int ii;

	if ((idx<0) || (idx>=calCount))
		return false;

	for (ii=idx; ii<calCount-1; ii++)
	{
		CalSpec[ii]       = CalSpec[ii+1];
		calConcValues[ii]    = calConcValues[ii+1];
		calIntnValues[ii]    = calIntnValues[ii+1];
		calRawIntnValues[ii] = calRawIntnValues[ii+1];
	}
	calCount--;

	CalcBestFit();

	return true;
}

void CCalibrationCurve::PaintCalCurve(CDC *pDC, CPrintInfo *pInfo, LPRECT vrect,
									  int ct, const double *cp,
									  int pc, const double *xv, const double *yv)
{
	// CString tval;
	CString tval=_T("[DEBUG] PaintCalCurve()");
	theApp.ChangeStatusText(tval.GetString());

	CPen *oldpen;
	CPen colors[MAX_CAL_SPEC+1];
	CBrush black(RGB(  0,  0,  0));
	CBrush white(RGB(255,255,255));

	CRect glyploc;

	double xmax=-1e10, ymax=-1e10;
	double xmin= 1e10, ymin= 1e10;

	int ii;
	RECT trect=*vrect;

	// No calibration curves no drawing ... ?
	if (pc<1)
		return;


	// Create color entries for glyphs
	colors[0].CreatePen(PS_SOLID, 1, RGB(  0,  0,  0));
	if (pInfo == NULL) {
		for (ii=1; ii<=MAX_CAL_SPEC; ii++)
			colors[ii].CreatePen(PS_SOLID,1,RPRainbowColors::rainbow_rgb(ii,(int)MAX_CAL_SPEC));
	} 
	else {
		for (ii=1; ii<=MAX_CAL_SPEC; ii++)
			colors[ii].CreatePen(PS_SOLID,1,RGB(0,0,0));
	}

	// Save canvase settings
	int sDC=pDC->SaveDC();

	// Set up graph helper object
	CRPGraphHelper gh;

	// Generic clipping independent of canvas type
	gh.setClipRectangle(pDC, &trect);

// We should not need this any longer ... 
#if 0
	// Clipping ... need to do some ugly things to deal with PrintPreview ... 
	// ... see http://support.microsoft.com/kb/128334
	RECT myrect=*&trect;
	CRgn ClipRgn;
	CRect ClipRect=myrect;
    if (pDC->IsKindOf(RUNTIME_CLASS(CPreviewDC)))
	{
		CPreviewDC *pPrevDC = (CPreviewDC *)pDC;

		pPrevDC->PrinterDPtoScreenDP(&ClipRect.TopLeft());
		pPrevDC->PrinterDPtoScreenDP(&ClipRect.BottomRight());

		// Now offset the result by the viewport origin of
		// the print preview window...

		CPoint ptOrg;
		::GetViewportOrgEx(pDC->m_hDC,&ptOrg);
		ClipRect += ptOrg;
	}
	ClipRgn.CreateRectRgnIndirect(&ClipRect);
	pDC->SelectClipRgn(&ClipRgn);
#endif


	// Calculate graph dimensions ....
	for (ii=0; ii<pc; ii++)
	{
		if (xv[ii]>xmax) xmax=xv[ii];
		if (xv[ii]<xmin) xmin=xv[ii];
		if (yv[ii]>ymax) ymax=yv[ii];
		if (yv[ii]<ymin) ymin=yv[ii];
	}

	// We want to accept a single data point so center it on
	// the graph ...
	if (xmin==xmax) { xmin -= 0.1*xmin; xmax += 0.1*xmax; }
	if (ymin==ymax) { ymin -= 0.1*ymin; ymax += 0.1*ymax; }

	// ... and if the data points were 0 and the above failed ...
	if (xmin==xmax) { xmin = -0.1; xmax = 0.1; }
	if (ymin==ymax) { ymin = -0.1; ymax = 0.1; }


	// ... and create axes
	gh.setFormatYLabel("%.2f");
	// gh.setLineColor(RGB(255,0,0));
	// gh.CreateAxes(pDC,&myrect,min(xmin,0),xmax,min(ymin,0),ymax);
	gh.CreateAxes(pDC,&trect,min(xmin,0),xmax,min(ymin,0),ymax);

#if 0
	// @@@ we probably should not be doing this at all ...
	tval=getFitInfo(FitInfoType::FIT_FORMULA|FitInfoType::FIT_RSQUARED);
	gh.drawTextDataCoord(pDC,xmin,ymax,tval.GetString(),0,"",TA_TOP|TA_LEFT);
#endif

// We should not need this anymore
#if 0
	// Print calibration curve info ...
#define TEXT_H 10
	CFont tmpFont;

	int xdpi = pDC->GetDeviceCaps(LOGPIXELSX);
	int ydpi = pDC->GetDeviceCaps(LOGPIXELSY);

	// This is what the theory says ...
	//
	//   height [pts] = p [px] * 72.27 [pt/in] / ydpi [px/i] 
	//
	// but this is what seems to work in practice ...
	//
	//   height [pts] = p [px] * 10 * 72.27 [pt/in] / ydpi [px/i] 
	//
    tmpFont.CreatePointFont((int)((double)TEXT_H * 10 * 72.27 / ydpi),_T("Arial"),pDC);
	pDC->SelectObject(&tmpFont);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);

	tval=getFitInfo(FitInfoType::FIT_FORMULA|FitInfoType::FIT_RSQUARED);
	pDC->TextOutA(
			(myrect.left + TEXT_H),
	        (myrect.top  + 2*TEXT_H),
			tval.GetString());
#endif


	// Draw line ... 
	// ... if we don't have a fit yet (only for higher order fits) ... connect the dots ...
	if ((pc>1) && (pc<ct))
	{
		oldpen = pDC->SelectObject(&(colors[0]));
		//pDC->MoveTo(myrect.left,myrect.bottom);
		//for (ii=0; ii<pc; ii++)
		//	pDC->LineTo((int)(xv[ii]*xscale+myrect.left),(int)(yv[ii]*yscale+myrect.bottom));
		pDC->MoveTo(gh.CalcXConv(xv[0]),gh.CalcYConv(yv[0]));
		for (ii=1; ii<pc; ii++)
			pDC->LineTo(gh.CalcXConv(xv[ii]),gh.CalcYConv(yv[ii]));

	}
	// ... if we have a fit already, draw best fit curve ... 
	else 
	{
		//pDC->SelectObject(&(colors[0]));
		//pDC->MoveTo((int)(xmin*xscale + myrect.left), (int)(bestFitXY(xmin)*yscale+myrect.bottom));
		//for (ii=(int)(xmin+1); ii<=(int)xmax; ii++)
		//{
		//	pDC->LineTo((int)(ii*xscale + myrect.left), (int)(bestFitXY(ii)*yscale+myrect.bottom));
		//}

		// If we assume we only support linear calibrations then we can simplify the code to drawing
		// a simple line. For higher order calibrations, it is necessary to step through the size
		// of the canvas ...
		pDC->SelectObject(&(colors[0]));
		if (ct != CCalibrationCurve::Linear) { 
			pDC->MoveTo(gh.CalcXConv(xmin),gh.CalcYConv(bestFitXY(xmin,0,ct,cp)));
			for (ii=(int)(xmin+1); ii<=(int)xmax; ii++)
				pDC->LineTo(gh.CalcXConv(ii),gh.CalcYConv(bestFitXY(ii,0,ct,cp)));
		} 
		else 
		{
			pDC->MoveTo(gh.CalcXConv(xmin),gh.CalcYConv(bestFitXY(xmin,0,ct,cp)));
			pDC->LineTo(gh.CalcXConv(xmax),gh.CalcYConv(bestFitXY(xmax,0,ct,cp)));
		}
	}

	// Draw glyphs ... we do this after the line to make sure they end up on top
	for (ii=0; ii<pc; ii++)
	{
#if 0
		glyploc.left   = (int)(xv[ii]*xscale + myrect.left   - 3);
		glyploc.right  = (int)(xv[ii]*xscale + myrect.left   + 3);
		glyploc.bottom = (int)(yv[ii]*yscale + myrect.bottom - 3);
		glyploc.top    = (int)(yv[ii]*yscale + myrect.bottom + 3);
		pDC->SelectObject(&(colors[ii+1]));
		pDC->Ellipse(glyploc);
#else
		pDC->SelectObject(&(colors[ii+1]));
		gh.drawGlyphDataCoord(pDC,xv[ii],yv[ii],CRPGraphHelper::GLYPH_CIRCLE);
#endif
	}

	pDC->RestoreDC(sDC);
}

bool CCalibrationCurve::PaintCalSpectra(CDC *pDC, CPrintInfo *pInfo, LPRECT vrect) {

	theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... entered"));

	if (calCount <= 0) {
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... exited: no valid data ..."));
		return false;
	}

	int ii;
	bool retval=true;

	// Preset colors for graphs
	// @@@ might want to adjust this for printing etc ... 
	CBrush black(RGB(  0,  0,  0));
	CBrush white(RGB(255,255,255));
	CPen colors[MAX_CAL_SPEC+1];

	colors[0].CreatePen(PS_SOLID, 1, RGB(  0,  0,  0));
	if (pInfo == NULL) {
		for (ii=1; ii<=MAX_CAL_SPEC; ii++)
			colors[ii].CreatePen(PS_SOLID,1,RPRainbowColors::rainbow_rgb(ii,(int)MAX_CAL_SPEC));
	} 
	else {
		for (ii=1; ii<=MAX_CAL_SPEC; ii++)
			colors[ii].CreatePen(PS_SOLID,1,RGB(0,0,0));
	}

	// Get min/max x/y values for all calibration spectra
	CSpectrumData *tcal;
	double wmin=1e10,wmax=-1e10,imin=1e10,imax=-1e10;
	for (ii=0; ii<calCount; ii++) {
		tcal=&(CalSpec[ii]);
		if (tcal->is_valid_wv()) {
			if (wmin > tcal->getMinW())
				wmin = tcal->getMinW();
			if (wmax < tcal->getMaxW())
				wmax = tcal->getMaxW();
			if (imin > tcal->getMinI())
				imin = tcal->getMinI();
			if (imax < tcal->getMaxI())
				imax = tcal->getMaxI();
		}
	}

	// Paint the individual spectra
	for (ii=0; ii<calCount; ii++) 
	{
		retval=retval && CalSpec[ii].PaintSpectrum(pDC,vrect,
			&(colors[ii+1]),
			wmin,wmax,imin,imax,
			(ii==0)?true:false,true);
	}

	theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... exited"));
	return retval;
}

void 
CCalibrationCurve::PaintReport(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect)
{
	CString tval;
	CRPString uval;
	CFont hdrFont,rptFont;
	CRect trect;
	CBrush blackb(RGB(0,0,0));

	int yoff=24; // minimum vertical cursor position / top margin (pt)
	int y_ht= 0; // set height of text box
	int xoff=12; // minimum left offset / left margin (pt)
	int x2ff=25*6; // x-offset of second column (pt)

	int xdpi = pDC->GetDeviceCaps(LOGPIXELSX);
	int ydpi = pDC->GetDeviceCaps(LOGPIXELSY);

	int ii;

	int sDC=pDC->SaveDC();

	// Clipping ... need to do some ugly things to deal with PrintPreview ... 
	// ... see http://support.microsoft.com/kb/128334
	CRgn ClipRgn;
	CRect ClipRect=*myrect;
    if (pDC->IsKindOf(RUNTIME_CLASS(CPreviewDC)))
	{
		CPreviewDC *pPrevDC = (CPreviewDC *)pDC;

		pPrevDC->PrinterDPtoScreenDP(&ClipRect.TopLeft());
		pPrevDC->PrinterDPtoScreenDP(&ClipRect.BottomRight());

		// Now offset the result by the viewport origin of
		// the print preview window...

		CPoint ptOrg;
		::GetViewportOrgEx(pDC->m_hDC,&ptOrg);
		ClipRect += ptOrg;
	}
	ClipRgn.CreateRectRgnIndirect(&ClipRect);
	pDC->SelectClipRgn(&ClipRgn);

	hdrFont.CreatePointFont((24 * 10),_T("Arial"),pDC);
	rptFont.CreatePointFont((12 * 10),_T("Arial"),pDC);

	// Select 24pt font
	pDC->SelectObject(&hdrFont);
	pDC->SetTextAlign(TA_BOTTOM|TA_CENTER);
	pDC->SetBkMode(TRANSPARENT);

	// Print header 48pt down, 24pt high for a total of 72pt = 1"
	y_ht  =   24;
	tval=_T("CALIBRATION CURVE REPORT");
	// TEXT_OUT_CT(tval,false);
	TEXT_OUT_BL_C2(tval,false);
	yoff += y_ht+24;

	// Select 12 pt font ... 
	pDC->SelectObject(&rptFont);
	pDC->SetTextAlign(TA_BOTTOM|TA_LEFT);

	y_ht=2+12;
	tval=_T("Calibration name");                 TEXT_OUT_BL_C1(tval,true);
	tval=getSubstanceName(); TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12;
	tval=_T("Quatbox Information");   TEXT_OUT_BL_C1(tval,true);
	// @@@ // Look in DlgBarCtrl ... CSpecNameConvert::getWilsonSerial(uval); ... 
	tval=_T(""); TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12;
	tval=_T("Integration Time [ms]");   TEXT_OUT_BL_C1(tval,true);
	tval.Format(_T("%.1f"),getIntegrationTime()); TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12;
	tval=_T("Spectra to Average"); TEXT_OUT_BL_C1(tval,true);
	tval.Format(_T("%d"),getSpectraToAverage());          TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12;
	tval=_T("Boxcar Width"); TEXT_OUT_TL_C1(tval,true);
	tval.Format(_T("%d"),getBoxcarWidth());    TEXT_DRW_TL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12;
	tval=_T("Wavelength [nm]"); TEXT_OUT_BL_C1(tval,true);
	tval.Format(_T("%.1f"),GetCalWavelength());   TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12;
	tval=_T("Warning Thresholds (auto)"); TEXT_OUT_BL_C1(tval,true);
	tval.Empty();
	tval.Append(uval.ppxFormat(getWarnLow()).GetString());
	tval.Append(" < ");
	tval.Append(uval.ppxFormat(getWarnHigh()).GetString());
	TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12;
	tval=_T("Error Thresholds (manual)"); TEXT_OUT_BL_C1(tval,true);
	tval.Empty();
	tval.Append(uval.ppxFormat(getErrLow()).GetString());
	tval.Append(" < ");
	tval.Append(uval.ppxFormat(getErrHigh()).GetString());
	TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=(2+12)*2;
	tval=_T("Fit Information");	TEXT_OUT_TL_C1(tval,true);
	tval=getFitInfo(CCalibrationCurve::FIT_FORMULA|CCalibrationCurve::FIT_RSQUARED); 
					TEXT_DRW_TL_C2(tval,true);
	yoff += y_ht;


	y_ht=(2+12)*(MAX_CAL_SPEC/2);
	tval=_T("Data Points"); TEXT_OUT_TL_C1(tval,true);

	tval.Empty();
	for (ii=0; ii<MAX_CAL_SPEC/2; ii++) {
		if (ii<calCount) {
			tval.Append(uval.ppxFormat(calConcValues[ii]));
			tval.AppendFormat(_T(": %.0f counts    "),
				calIntnValues[ii]);
		}
		if (ii+MAX_CAL_SPEC/2 <calCount) {
			tval.Append(uval.ppxFormat(calConcValues[ii+MAX_CAL_SPEC/2]));
			tval.AppendFormat(_T(": %.0f counts    "),
				calIntnValues[ii+MAX_CAL_SPEC/2]);
		}
		tval.Append(_T("\n"));
	}
					TEXT_DRW_TL_C2(tval,true);
	yoff += y_ht;

	// I believe we are at 19 * 14pt = 3.7"
	// Print 
	pDC->RestoreDC(sDC);
}


void CCalibrationCurve::SetupCalCurveList(CListCtrl *listCtrl,bool init)
{

#ifdef CD_USE_THREE
#else
	CRect rect;
	LVITEM lvi;
	CString lvs;

	int ii;

	if (init) 
	{
		// Make sure we select the "whole" line ... this still does not
		// cover the space not taken up by subitems but it should be 
		// good enough for starters
		listCtrl->SetExtendedStyle(listCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
		while (listCtrl->DeleteColumn(0))
			;

		// preset column widths ... may not want even thirds later on but it's a start
		listCtrl->GetClientRect(&rect);
		int cw=rect.Width()/3;
		listCtrl->InsertColumn(0,_T("Conc [ppm]"),0,cw);
		listCtrl->InsertColumn(1,_T("Counts"),0,cw);
		listCtrl->InsertColumn(2,_T("Raw Counts"),0,cw);
	}

	// This is a hack that will likely have rather bad consequences regarding
	// selections etc ... but for now it's easier to just recreate the list
	listCtrl->DeleteAllItems();
	for (ii=0; ii<calCount; ii++) 
	{
		lvi.mask = LVIF_TEXT;
		lvi.iItem = ii;

		// main entry ... conc
		lvs.Format(_T("%g"),calConcValues[ii]);
		lvi.iSubItem = 0;
		lvi.pszText=(LPTSTR)(LPCTSTR)(lvs);
		listCtrl->InsertItem(&lvi);

		// subentry 1 ... intensity counts
		lvs.Format(_T("%g"), calIntnValues[ii]);
		lvi.iSubItem = 1;
		lvi.pszText=(LPTSTR)(LPCTSTR)(lvs);
		listCtrl->SetItem(&lvi);

		// subentry 2 ... intensity counts
		lvs.Format(_T("%g"), calRawIntnValues[ii]);
		lvi.iSubItem = 2;
		lvi.pszText=(LPTSTR)(LPCTSTR)(lvs);
		listCtrl->SetItem(&lvi);
	}
#endif

}


void CCalibrationCurve::CalcBestFit(bool update_spectra)
{
#if defined SUPPRESSLAPACK
	return;
#else
	if (calCount < (int)type)
		return;

	int ii,jj;
	int li;
	double lcnt;
	int fi,fcnt;

	// Introduce baseline corrections. The ppm values stay the same
	// because they are give to us by the user, but the count values
	// need to have a "baseline" subtracted because we will be 
	// subtracting baselines in actual measurements as well.
	//
	// * find count value associated with lowest ppm value to 
	//   extrapolate a "baseline" if one is not available
	//
	// * while we are at it find higest and lowest ppm values 
	//   to set warning thresholds
	//
	ppmWarnLow=calConcValues[0]; 
	ppmWarnHigh=calConcValues[0]; 
	li=0; lcnt=calRawIntnValues[0]; hasBaselineVar=false;
	for (ii=0; ii<calCount; ii++) {

		if (calConcValues[ii] == 0)
			hasBaselineVar=true;

		if (calConcValues[ii]<ppmWarnLow) {
			ppmWarnLow=calConcValues[ii];
			li=ii; lcnt=calRawIntnValues[ii];
		}

		if (calConcValues[ii]>ppmWarnHigh)
			ppmWarnHigh=calConcValues[ii];
	}

	// If we have a baseline all is well and we just subtract it
	if (hasBaselineVar) {
		baselineInt=lcnt;
		for (ii=0; ii<calCount; ii++)
			calIntnValues[ii]=calRawIntnValues[ii]-baselineInt;
		fcnt=1;
	}
	// ... otherwise we have a hack for LINEAR
	else {
		for (ii=0; ii<calCount; ii++)
			calIntnValues[ii]=calRawIntnValues[ii];
		fcnt=2;
	}

	// XValues = ppm
	// YValues = counts
	// rows=npts, columns=degree+1
	// XV/YV(NPTS,DEG+1);
	LaGenMatDouble XV(calCount,type);
	LaGenMatDouble YV(calCount,1);

	// AV(DEG+1,1)
	LaGenMatDouble AV(type,1);

	// If we didn't have a baseline, we run the fit twice, the first
	// time to get the baseline, the second time to get the fit on the
	// subtracted data ...
	for (fi=0; fi<fcnt; fi++)
	{


		// populate X data matrix
		//
		//                 jj
		//               0 ...   degree
		//       0  x[0]^0 ... x[0]^deg
		//       :  x[1]^0 ... x[1]^deg
		// ii    :
		//       :
		//    npts  x[n]^0 ... x[n]^deg
		//
		for (jj=0; jj<type; jj++)
		{
			for (ii=0; ii<calCount; ii++)
			{
				// "x" values is counts
				XV(ii,jj)=pow(calIntnValues[ii],jj);
			}
		}
		// populate Y data vector
		for (ii=0; ii<calCount; ii++)
		{
			// "y" values is ppm
				YV(ii,0)=calConcValues[ii];
		}
		// Paranoia: clear result vector
		for (ii=0; ii<type; ii++)
		{
			AV(ii,0)=0;
		}

		//CString tval;
		//for (ii=0; ii<calCount; ii++) 
		//{
		//	tval.AppendFormat("| ");
		//	for (jj=0; jj<type; jj++)
		//		tval.AppendFormat("%4.1f ",(const double)XV(ii,jj));
		//	tval.AppendFormat("| ");
		// 	tval.AppendFormat("| %4.1f |\n",(const double)YV(ii,0));
		//}

		// Solve for AV
		LaLinearSolve(XV,AV,YV);

		// In case we are running this twice ...
		if ((fcnt==2)&&(fi==0)) {
			if (AV(1,0)!=0)
			{
				double a0=AV(0,0);
				double a1=AV(1,0);
				baselineInt= -a0/a1; // @@@ this is only right for linear!
			}
			else
				baselineInt=0;
			for (ii=0; ii<calCount; ii++)
				calIntnValues[ii]=calRawIntnValues[ii]-baselineInt;
		}
	}

	// Transfer fit coefficients into array
	for (ii=0; ii<type; ii++) {
		calParms[ii]=AV(ii,0);
	}

	// Calculate Rsquared (PEARSON product moment correlation as in Excel) value ... use pseudocode from
	// Wikipedia because it also allows us to deal with division / 0 ... note that this is only good
	// for *linear* fits
	if ((int)type == 2) {
		double sum_sq_x = 0;
		double sum_sq_y = 0;
		double sum_coproduct = 0;
		double mean_x = calConcValues[0];
		double mean_y = calIntnValues[0];

		double sweep;
		double delta_x, delta_y;
		double pop_sd_x, pop_sd_y;
		double cov_x_y;

		for (ii=1; ii<calCount; ii++)
		{
			sweep = (double)(ii) / (ii+1.0);

			delta_x = calConcValues[ii] - mean_x;
			delta_y = calIntnValues[ii] - mean_y;
			sum_sq_x += delta_x * delta_x * sweep;
			sum_sq_y += delta_y * delta_y * sweep;
			sum_coproduct += delta_x * delta_y * sweep;
			mean_x += delta_x / (ii+1);
			mean_y += delta_y / (ii+1); 
		}
		pop_sd_x = sqrt( sum_sq_x/calCount );
		pop_sd_y = sqrt( sum_sq_y/calCount );
		cov_x_y = sum_coproduct/calCount;

		if (pop_sd_x * pop_sd_y != 0) {
			calRSquare = cov_x_y / (pop_sd_x * pop_sd_y);
			calRSquare = calRSquare * calRSquare;
		} else {
			calRSquare = 0;
		}
	}
	else {
		calRSquare=0;
	}

	// Change the calibration information stored within each spectrum
	if (update_spectra) {
		for (ii=0; ii<calCount; ii++) {	
			CalSpec[ii].storeCalInfo(
				substanceName,"",calWavelength,
				type,calParms,calRSquare,
				calCount,calConcValues,calIntnValues,
				ppmWarnLow,ppmWarnHigh,ppmErrLow,ppmErrHigh);
		}
	}

	// CString tval;
	// tval.Format("%g %g %g %g",calParms[0],calParms[1],calParms[2],calParms[3]);
	// theApp.ChangeStatusText(tval.GetString());

#endif
}