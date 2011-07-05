#include "StdAfx.h"
#include <afxpriv.h>
#include "SpectrumData.h"
#include "CalibrationCurve.h"
#include "RPGraphHelper.h"

#include "conf_const.h"
#include "spec4.h"
#include "ugly_text_macros.h"

CSpectrumData::CSpectrumData(void)
: valid_wv(false)
, valid_int(false)
, spec_idx(-1)
, conc(-1)
, wcount(-1)
, wmin(-1)
, wmax(-1)
, imin(-1)
, imax(-1)
, itime(0)
, Customer(_T(""))
, Location(_T(""))
, Comment(_T(""))
, Pipekm(0)
, Dilution(1)
, FF1(_T(""))
, FF2(_T(""))
, specName(_T(""))
, specSerial(_T(""))
{
}

CSpectrumData::~CSpectrumData(void)
{
}

bool 
CSpectrumData::store(int idx, DoubleArray *Intensities, DoubleArray *Wavelengths, double setconc)
{
	int ii;

	spec_idx = idx;

	intensities = *Intensities;

	valid_int = true;

	wcount = intensities.getLength();

	imax = imin = intensities.getDoubleValues()[0];
	for (ii=1; ii<wcount; ii++)
	{
		if (intensities.getDoubleValues()[ii] < imin)
			imin = intensities.getDoubleValues()[ii];
		if (intensities.getDoubleValues()[ii] > imax)
			imax = intensities.getDoubleValues()[ii];
	}

	if (Wavelengths != NULL)
	{
		wavelengths = *Wavelengths;
		if (wavelengths.getLength() != wcount)
			return false;
	}
		
	valid_wv = true;
	wmin = wavelengths.getDoubleValues()[0];
	wmax = wavelengths.getDoubleValues()[wcount - 1];

	if (setconc != -1)
		conc=setconc;

	return true;
}

bool CSpectrumData::Load(CString *fname)
{
	CString line,buffer;
	CString filename;
	int lpos=0,llpos=0;

	if (fname == NULL) {

		CFileDialog TempLoader(TRUE,_T(".spc"), NULL, 0, "Spectrum Files (*.spc)|*.spc|All Files(*.*)|*.*||");
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

//! Save a single spectrum ... not sure if we even need this
//
bool CSpectrumData::Save(CString *fname=NULL)
{
	CString filename;

	if (fname == NULL) {

		CFileDialog TempSaver(FALSE,_T(".spc"), NULL, 0, "Spectrum Files (*.spc)|*.spc|All Files(*.*)|*.*||");
		TempSaver.m_pOFN->lpstrInitialDir=theApp.pConfig->getSavepathSPM();

		if (TempSaver.DoModal() == IDOK)
		{
			// we have a selection
			theApp.ChangeStatusText(_T("Selection OK"));
			filename=TempSaver.GetPathName();
			theApp.pConfig->setSavepathSPM(filename);
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
CSpectrumData::serializeData(unsigned char mode)
{
	CString tval(_T(""));
	double *xv,*yv;
	int ii;

	if (mode & this->outputCSV)
	{
		if (!(valid_wv && valid_int))
			return tval;

		tval.AppendFormat(_T(TEXT_SPEC_START    ",%s\n"),dtype.GetString());
		tval.AppendFormat(_T(TEXT_SPEC_VERSION  ",%f\n"),1.0);

		// @@@ pack multiline strings!
		tval.AppendFormat(_T(TEXT_SPEC_INF_LOCN ",%s\n"),((CRPString)Location).GetEscaped().GetString());
		tval.AppendFormat(_T(TEXT_SPEC_INF_CUST ",%s\n"),((CRPString)Customer).GetEscaped().GetString());
		tval.AppendFormat(_T(TEXT_SPEC_INF_INFO ",%s\n"),((CRPString)Comment ).GetEscaped().GetString());
		tval.AppendFormat(_T(TEXT_SPEC_INF_GPS  ",%f,%f,%f,%d,%d\n"),
			GPSinfo.getLat(),
			GPSinfo.getLon(),
			GPSinfo.getAlt(),
			GPSinfo.getTime(),
			GPSinfo.getDate()); 
		tval.AppendFormat(_T(TEXT_SPEC_INF_SYST ",%d,%d,%d,%d,%d,%d,%d\n"),
			ClockInfo.wYear,
			ClockInfo.wMonth,
			ClockInfo.wDay,
			ClockInfo.wHour,
			ClockInfo.wMinute,
			ClockInfo.wSecond,
			ClockInfo.wMilliseconds);
		tval.AppendFormat(_T(TEXT_SPEC_INF_KM   ",%f\n"),Pipekm);
		tval.AppendFormat(_T(TEXT_SPEC_INF_DIL  ",%f\n"),Dilution);
		tval.AppendFormat(_T(TEXT_SPEC_INF_FF1  ",%s\n"),((CRPString)FF1).GetEscaped().GetString());


		tval.AppendFormat(_T(TEXT_SPEC_CONC     ",%f\n"),conc);
		tval.AppendFormat(_T(TEXT_SPEC_CAL_NAME ",%s\n"),calName.GetString());
		tval.AppendFormat(_T(TEXT_SPEC_CAL_UFID ",%s\n"),calUFID.GetString());
		tval.AppendFormat(_T(TEXT_SPEC_CAL_WAVE ",%f\n"),calWavelen);
		tval.AppendFormat(_T(TEXT_SPEC_CAL_TYPE ",%d"),calType);
		for (ii=0; ii<calType; ii++)
			tval.AppendFormat(_T(",%f"),calConst[ii]);
		tval.Append(_T("\n"));

		tval.AppendFormat(_T(TEXT_SPEC_CAL_RSQR ",%f\n"), calRSquared);
		tval.AppendFormat(_T(TEXT_SPEC_CAL_CONC ",%d"), calDataCount);
		for (ii=0; ii<calDataCount; ii++)
			tval.AppendFormat(_T(",%f"),calConcns[ii]);
		tval.Append(_T("\n"));
		tval.AppendFormat(_T(TEXT_SPEC_CAL_CNTS ",%d"), calDataCount);
		for (ii=0; ii<calDataCount; ii++)
			tval.AppendFormat(_T(",%f"),calCounts[ii]);
		tval.Append(_T("\n"));

		tval.AppendFormat(_T(TEXT_SPEC_CAL_WLO ",%f\n"), calWL);
		tval.AppendFormat(_T(TEXT_SPEC_CAL_WHI ",%f\n"), calWH);
		tval.AppendFormat(_T(TEXT_SPEC_CAL_ELO ",%f\n"), calEL);
		tval.AppendFormat(_T(TEXT_SPEC_CAL_EHI ",%f\n"), calEH);

		tval.AppendFormat(_T(TEXT_SPEC_SPEC_NAM  ",%s\n"),specName);
		tval.AppendFormat(_T(TEXT_SPEC_SPEC_SER  ",%s\n"),specSerial);

		tval.AppendFormat(_T(TEXT_SPEC_ITIME     ",%f\n"),itime);
		tval.AppendFormat(_T(TEXT_SPEC_AVG       ",%d\n"),avg);
		tval.AppendFormat(_T(TEXT_SPEC_BOXW      ",%d\n"),boxw);
		tval.AppendFormat(_T(TEXT_SPEC_WAVLN_RNG ",%f,%f\n"),wmin,wmax);
		tval.AppendFormat(_T(TEXT_SPEC_INTEN_RNG ",%f,%f\n"),imin,imax);

		tval.AppendFormat(_T(TEXT_SPEC_N_WAVLN   ",%d\n"),wcount);

		tval.Append(      _T(TEXT_SPEC_WAVLN_HDR "\n"));

		xv=wavelengths.getDoubleValues();
		yv=intensities.getDoubleValues();

		for (ii=0; ii<wcount; ii++)
			tval.AppendFormat(_T("%g,%g\n"),xv[ii],yv[ii]);

		tval.AppendFormat(_T(TEXT_SPEC_END ",%s\n"),dtype.GetString());

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
bool CSpectrumData::deserializeData(CString data, int &lpos, int &llpos)
{
	int ii;
	int tpos,tcnt,widx;
	CString line;
	CString tokens[MAX_TOKENS];
	int scnt=0,ecnt=0;
	double version;

	// so we can tell if it was set ...
	wcount = -1;
	double *pwv,*pin;

	// removed this in favor of passing position in and out of the function ... 
	// lpos=0;
	while (lpos > -1)
	{
		llpos=lpos;
		line=data.Tokenize(_T("\n"),lpos);

		for (tcnt=0, tpos=0; (tpos > -1) && (tcnt<MAX_TOKENS); tcnt++)
			tokens[tcnt]=line.Tokenize(_T(","),tpos);

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_START)))
			scnt++;

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_VERSION)))
		{
			version=atof(tokens[1].GetString());
			if (version != 1.0)
				return false;
		}

		// @@@ unpack multiline comments!!!
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_INF_LOCN)))
			Location=((CRPString)(tokens[1])).GetUnEscaped();
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_INF_CUST)))
			Customer=((CRPString)(tokens[1])).GetUnEscaped();
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_INF_INFO)))
			Comment=((CRPString)(tokens[1])).GetUnEscaped();

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_INF_GPS)))
		{
			double lat  = atof(tokens[1].GetString());
			double lon  = atof(tokens[2].GetString());
			double alt  = atof(tokens[3].GetString());

			if ((lat!=0) || (lon!=0) || (alt!=0))
				GPSinfo.setData(&lat,&lon,&alt,NULL,NULL,NULL,NULL,(char *)NULL);

			INT32 time = atol(tokens[4].GetString());
			INT32 date = atol(tokens[5].GetString());

			if ((time!=0) || (date!=0))
				GPSinfo.setData(NULL,NULL,NULL,NULL,NULL,&time,&date,(char *)NULL);
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_INF_SYST)))
		{
			ClockInfo.wYear         = (WORD)atol(tokens[1].GetString());
			ClockInfo.wMonth        = (WORD)atol(tokens[2].GetString());
			ClockInfo.wDay          = (WORD)atol(tokens[3].GetString());
			ClockInfo.wHour         = (WORD)atol(tokens[4].GetString());
			ClockInfo.wMinute       = (WORD)atol(tokens[5].GetString());
			ClockInfo.wSecond       = (WORD)atol(tokens[6].GetString());
			ClockInfo.wMilliseconds = (WORD)atol(tokens[7].GetString());
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_INF_KM)))
			Pipekm=atof(tokens[1].GetString());
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_INF_DIL)))
			Dilution=atof(tokens[1].GetString());
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_INF_FF1)))
			FF1=((CRPString)(tokens[1])).GetUnEscaped();

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CONC)))
			conc=atof(tokens[1].GetString());

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CAL_NAME)))
			calName=tokens[1];

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CAL_UFID)))
			calUFID=tokens[1];

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CAL_WAVE)))
			calWavelen=atof(tokens[1].GetString());

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CAL_TYPE)))
		{
			calType=atol(tokens[1].GetString());
			for (ii=0; ii<calType && ii<4; ii++)
				calConst[ii]=atof(tokens[ii+2]);
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CAL_RSQR)))
			calRSquared=atof(tokens[1].GetString());


		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CAL_CONC)))
		{
			calDataCount=atol(tokens[1].GetString());
			for (ii=0; ii<calDataCount && ii<MAX_CAL_SPEC; ii++)
				calConcns[ii]=atof(tokens[ii+2]);
		}
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CAL_CNTS)))
		{
			calDataCount=atol(tokens[1].GetString());
			for (ii=0; ii<calDataCount && ii<MAX_CAL_SPEC; ii++)
				calCounts[ii]=atof(tokens[ii+2]);
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CAL_WLO)))			
			calWL=atof(tokens[1].GetString());
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CAL_WHI)))			
			calWH=atof(tokens[1].GetString());
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CAL_ELO)))			
			calEL=atof(tokens[1].GetString());
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_CAL_EHI)))			
			calEH=atof(tokens[1].GetString());


		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_SPEC_NAM)))
			specName=tokens[1];

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_SPEC_SER)))
			specSerial=tokens[1];

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_ITIME)))
			itime=atof(tokens[1].GetString());
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_AVG)))
			avg=atol(tokens[1].GetString());
		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_BOXW)))
			boxw=atol(tokens[1].GetString());

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_WAVLN_RNG)))
		{
			wmin=atof(tokens[1].GetString());
			wmax=atof(tokens[2].GetString());
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_INTEN_RNG)))
		{
			imin=atof(tokens[1].GetString());
			imax=atof(tokens[2].GetString());
		}

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_N_WAVLN)))
			wcount=atol(tokens[1].GetString());

		if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_WAVLN_HD1)))
		{
			if (wcount <= 0)
				return false;

			pwv = new double[wcount];
			pin = new double[wcount];

			// NOTE: the '<=' wcount comparison instead of the more obvious '<'
			// approach here is intended to catch the EndSpectrumFile tag ... 
			// ... this should be safe because if the tag is early we can return and 
			// if the tag isn't there the extra line won't make a difference.
			for (widx=0; (lpos > -1) && (widx <= wcount); widx++)
			{
				llpos=lpos;
				line=data.Tokenize(_T("\n"),lpos);

				for (tcnt=0, tpos=0; (tpos > -1) && (tcnt<MAX_TOKENS); tcnt++)
					tokens[tcnt]=line.Tokenize(_T(","),tpos);

				if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_END)))
				{
					wavelengths.setValues(pwv,wcount); valid_wv=true;
					intensities.setValues(pin,wcount); valid_int=true;
					delete pwv;
					delete pin;
					ecnt++;
					if ((scnt>0) && (scnt==ecnt))
						return true; // this is questionable because it is possible we read fewer data points than expected ...
					return false;
				}

				if (tcnt != 3) // for loop counts one more than constituents ... 
				{
					delete pwv;
					delete pin;
					return false;
				}

				pwv[widx]=atof(tokens[0].GetString());
				pin[widx]=atof(tokens[1].GetString());
			}

			// We should have already caught an EndSpectrumFile token at the end of the data ...
			if (! tokens[0].CompareNoCase(_T(TEXT_SPEC_END)))
				return false;
		}
	}

	// we should never get to here ...
	return false;
}

CSpectrumData CSpectrumData::operator-(CSpectrumData &right)
{
	CSpectrumData temp;
	int ii;
	double *dl,*dr,*dt;

	// If *either* of the spectra is invalid, return a blank spectrum
	if (!(valid_wv && valid_int && right.valid_wv && right.valid_int))
		return temp;

	// we should reset wavelength boundaries in case spectra are different ...
	// but for now we just consider that an error and return an empty spectrum
	if ((wcount != right.wcount) || 
		(wavelengths.getLength() != wcount) ||
		(right.wavelengths.getLength() != 
		wcount) ||
		(wavelengths.getLength() != wcount) ||
		(wmin != right.wmin) ||
		(wmax != right.wmax))
	{
		return temp;
	}

	// prepopulate result with LEFT parameter. In particular this means all settings such
	// as concentration in the left (data) spectrum will supersede those in the right (baseline).
	temp=*this;

	// Only adjust intensity values ... 
	dl=intensities.getDoubleValues();
	dr=right.intensities.getDoubleValues();
	dt=temp.intensities.getDoubleValues();

	temp.imax = temp.imin = (dl[0]-dr[0]);
	for (ii=0; ii<wcount; ii++) {
		dt[ii]=dl[ii]-dr[ii];

		if (dt[ii] < temp.imin)
			temp.imin = dt[ii];
		if (dt[ii] > temp.imax)
			temp.imax = dt[ii];
	}

	temp.dtype=_T("DIFF DATA");

	return temp;
}

CString CSpectrumData::formatFitInfo(unsigned int infotype)
{
	return CCalibrationCurve::formatFitInfo(
		infotype,
		calType,calConst,min(4,calType),calRSquared);
}


//! @param pDC - pointer to drawing canvas
//! @param myrect - dimensions of whole canvas (what are the boundary conditions?)
//! @param pen - line color of spectrum
//! @param wmin - minimum wavelength of spectrum
//! @param wmax - maximum wavelength of spectrum
//! @param imin - minimum intensity / count value of spectrum
//! @param imax - maximum intensity / count value of spectrum
//! @param redraw_axis - allow suppressing axis drawing if we are painting multiple spectra
bool 
CSpectrumData::PaintSpectrum(CDC *pDC, LPRECT myrect,CPen *pen,
							 double wmin, double wmax,
							 double imin, double imax, 
							 bool redraw_axis,
							 bool wavlen_line)
{
	theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... entered"));

	if (! valid_wv) {
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... exited: no valid data ..."));
		return false;
	}

	// CBrush blackb(RGB(  0,  0,  0));
	CPen blackp(PS_SOLID,1,RGB(  0,  0,  0));

	int sDC=pDC->SaveDC();
	double xx,yy;

	RECT orect=*myrect;
	CRPGraphHelper gh(pDC,&orect,wmin,wmax,imin,imax);

	gh.setClipRectangle(pDC,&orect);

	if (redraw_axis)
	{
		gh.setFormatXLabel("%.0f");
		gh.setFormatYLabel("%.0f");

		gh.CreateAxes(pDC,&orect,wmin,wmax,imin,imax);

		// draw vertical line for wavelength of interest
		// ... but only if a calibration wavelength is set 
		// and we are instructed to do so (otherwise see glyph
		// below)
		// draw vertical line "cursor" for wavelength of interest
		if (wavlen_line &&  theApp.pSpecControl->PaintOpt_GetCursor()) {
			xx=theApp.pSpecControl->PaintOpt_GetCursorWavelen();
			getXY(&xx,&yy);
			pDC->MoveTo(gh.CalcXConv(xx),orect.bottom-2);
			pDC->LineTo(gh.CalcXConv(xx),orect.top);
		}

//		if ((wavlen_line) && (valid_wv)) {
//			pDC->SelectObject(&blackp);
//			pDC->MoveTo(gh.CalcXConv(calWavelen),orect.bottom);
//			pDC->LineTo(gh.CalcXConv(calWavelen),orect.top);
//		}
	}
	else 
	{
		gh.getGraphRect(&orect);
	}


	// draw spectrum ... on top ...
	pDC->SelectObject(pen);
	gh.drawTraceDataCoord(pDC,
		getCount(),
		getWavelengths().getDoubleValues(),
		getIntensities().getDoubleValues());

#if 0
	// @@@ note that this assumes that the first call to this function will
	// create axes ... if not, gh will not be initialized and the conversions
	// will return 0
	ii=imn;
	pDC->MoveTo(
		gh.CalcXConv(data.getWavelengths().getDoubleValues()[ii]),
		gh.CalcYConv(data.getIntensities().getDoubleValues()[ii]));
	for (ii=imn+1; ii<imx; ii++)
		pDC->LineTo(
			gh.CalcXConv(data.getWavelengths().getDoubleValues()[ii]),
			gh.CalcYConv(data.getIntensities().getDoubleValues()[ii]));
	// draw an individual marker
#endif

	// Mark the calibration wavelength in this spectrum using a glyph. This should 
	// work as long as the valid_wv variable and the calibration wavelength are
	// set correctly ...
	if (valid_wv) {
		double xx=calWavelen;
		double yy;
		if (getXY(&xx,&yy))
		{
			if (wavlen_line) {
				pDC->SelectObject(pen);
				pDC->MoveTo(gh.CalcXConv(xx),orect.bottom-2);
				pDC->LineTo(gh.CalcXConv(xx),orect.top);
			}

			pDC->SelectObject(&blackp);
				gh.drawGlyphDataCoord(pDC,xx,yy,CRPGraphHelper::GLYPH_CROSS);
		}
	}

	pDC->RestoreDC(sDC);

	theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... exited"));
	return true;
}


void 
CSpectrumData::PaintReport(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect)
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
	tval=_T("CONCENTRATION REPORT");
	// TEXT_OUT_CT(tval,false);
	TEXT_OUT_BL_C2(tval,false);
	yoff += y_ht+24;

	// Select 12 pt font ... 
	pDC->SelectObject(&rptFont);
	pDC->SetTextAlign(TA_BOTTOM|TA_LEFT);

	y_ht=2+12;
	tval=_T("Date");                 TEXT_OUT_BL_C1(tval,true);
	tval=formatTimeStamp(); TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12;
	tval=_T("GPS Location");   TEXT_OUT_BL_C1(tval,true);
	tval=formatLoc(); TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12;
	tval=_T("Name / Company");   TEXT_OUT_BL_C1(tval,true);
	tval=getCustomer(); TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12;
	tval=_T("Location / Sampling point"); TEXT_OUT_BL_C1(tval,true);
	tval=getLocation();          TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=(2+12*4);
	tval=_T("Sample Information"); TEXT_OUT_TL_C1(tval,true);
	tval=getComment();    TEXT_DRW_TL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12;
	tval=_T("Calibration Curve"); TEXT_OUT_BL_C1(tval,true);
	tval=getCalname();   TEXT_OUT_BL_C2(tval,true);
	yoff += y_ht;

	y_ht=(2+12)*3;
	tval=_T("");	TEXT_OUT_TL_C1(tval,true);
	tval=formatFitInfo(CCalibrationCurve::FIT_FORMULA|CCalibrationCurve::FIT_RSQUARED); 
	tval.AppendFormat(_T("wavelength=%.1f\n"),calWavelen);
					TEXT_DRW_TL_C2(tval,true);
	yoff += y_ht;

	y_ht=2+12 + 2+24;
	tval=_T("Emission / Concentration"); TEXT_OUT_TL_C1(tval,true);

	double calw,cali;
	calw=getCalW();
	getXY(&calw,&cali);
	double dil=getDilution();
	double conc=getConc();

	y_ht=2+12;
	tval.Format(_T("%.0f counts: %.2e ppm * %.1g\n"),cali,conc/dil,dil,conc);
					TEXT_DRW_TL_C2(tval,true);
	yoff += y_ht;
	
	y_ht=2+24;
	pDC->SelectObject(&hdrFont);
	tval=uval.ppxFormat(conc);
					TEXT_DRW_TL_C2(tval,true);
	yoff += y_ht;

	// I believe we are at 15 * 14pt = 3"
	// Print 
	pDC->RestoreDC(sDC);
}


// ------------------------------------------------------------------------------------------




CDSpectrumData::CDSpectrumData(void)
: highlight(false)
, specFileName("not set")
{
}

CDSpectrumData::~CDSpectrumData(void)
{
}

//! Save triple spectrum (baseline / raw / diff)
bool CDSpectrumData::Save(CString *fname=NULL)
{
	CString filename;

	if ((fname == NULL) || (fname->IsEmpty())) {

		CFileDialog TempSaver(FALSE,_T(".spm"), NULL, 0, "Spectrum Files (*.spm)|*.spm|All Files(*.*)|*.*||");
		TempSaver.m_pOFN->lpstrInitialDir=theApp.pConfig->getSavepathSPM();

		if (TempSaver.DoModal() == IDOK)
		{
			// we have a selection
			theApp.ChangeStatusText(_T("Selection OK"));
			filename=TempSaver.GetPathName();
			theApp.pConfig->setSavepathSPM(filename);
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

		calfile.WriteString(specBaseline.serializeData(CSpectrumData::outputCSV).GetString());
		calfile.WriteString(specRaw.serializeData(CSpectrumData::outputCSV).GetString());
		calfile.WriteString(specDiff.serializeData(CSpectrumData::outputCSV).GetString());

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

	if (fname!=NULL)
		*fname=filename;
	return true;
}

bool CDSpectrumData::Load(CString *fname)
{
	CString line,buffer;
	CString filename;
	int lpos=0,llpos=0;

	if (fname == NULL) {

		CFileDialog TempLoader(TRUE,_T(".spm"), NULL, 0, "Spectrum Files (*.spm)|*.spm|All Files(*.*)|*.*||");
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

		calfile.Close();

		if (specBaseline.deserializeData(buffer,lpos,llpos) &&
			specRaw.deserializeData(buffer,lpos,llpos) && 
			specDiff.deserializeData(buffer,lpos,llpos))
		{
			specFileName=filename;
			return true;
		}
		return false;

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

void CSpectrumData::PaintCalCurve(CDC *pDC, LPRECT myrect) {
	CCalibrationCurve::PaintCalCurve(pDC,NULL,myrect,calType,calConst,
		calDataCount,calCounts,calConcns);
}



