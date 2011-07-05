#include "StdAfx.h"
#include <afxpriv.h>
#include "Spectrometer.h"

#include "spec4.h"
#include "Wrapper.h"
#include "RPRainbowColors.h"
#include "CalibrationCurve.h"
#include "RPGraphHelper.h"

CSpectrometer::CSpectrometer(void)
: curSpecName(_T(""))
, curSpecSerial(_T(""))
, parm_integration_time(50)
, parm_boxcar_width(3)
, parm_averaging_num(1)
, parm_electrical_dark_on(true)
, parm_electrical_dark_reset(false)
, wmin(0)
, wmax(0)

{
	if ((pWrapper = new Wrapper()) == NULL) {
		theApp.ChangeStatusText(_T("... loading Ocean Optics Wrapper failed!"));
		MessageBox(NULL,_T("Loading Ocean Optics Wrapper failed!"),_T("Fatal Error"),0);
		exit(1);
	}
	theApp.ChangeStatusText(_T("... loading Ocean Optics Wrapper succeeded"));
	SpecCount = pWrapper->openAllSpectrometers();

	// Set to Illegal value so that a subsequent call to
	// SelectSpectrometer() will set variables.
	curSpectrometer=-1;
}

CSpectrometer::~CSpectrometer(void)
{
	pWrapper->closeAllSpectrometers();
	// @@@ For some reason deleting the wrapper object will blow up ... 
	// delete pWrapper;
}


int 
CSpectrometer::SelectSpectrometer(CString name, CString serial) { 
	int ii;
	CString tval;

	// If we already have the right selection, just return it
	if (serial == curSpecSerial)
		return curSpectrometer;

	if (name == curSpecName)
		return curSpectrometer;

	// First search by serial (this is only a small waste of time if it's empty)
	for (ii=0; ii<SpecCount; ii++)
	{
		tval=pWrapper->getSerialNumber(ii).getASCII();
		if (tval == serial)
			return SelectSpectrometer(ii);
	}

	// If we didn't find it by the serial, select based on name
	for (ii=0; ii<SpecCount; ii++)
	{
		tval=pWrapper->getName(ii).getASCII();
		if (tval == name)
			return SelectSpectrometer(ii);
	}

	return -1;
}

int 
CSpectrometer::SelectSpectrometer(int index) { 
	// If user tries to set an illegal index,
	// return current index and exit
	if (!((0<=index) && (index<SpecCount)))
		return curSpectrometer;

	// only do this if we are actually changing spectometers ...
	if (index!=curSpectrometer)
	{
		curSpectrometer=index; 

		curSpecName=pWrapper->getName(index).getASCII();
		curSpecSerial=pWrapper->getSerialNumber(index).getASCII();

		SpecWavelengths = pWrapper->getWavelengths(index);

		wmin = SpecWavelengths.getDoubleValues()[0];
		wmax = SpecWavelengths.getDoubleValues()[SpecWavelengths.getLength()-1];

	}

	return curSpectrometer;
}

#if 0 // moved this into DlgBarAcq and replaced with inline wrapper ... 
bool 
CSpectrometer::Spectrum_AcquireDataSpectrum() 
{
	DoubleArray tempdata;
	double lat,lon,alt;
	INT32 time,date;
	SYSTEMTIME curtime;

	if (Spectrum_AcquireSpectrum(&tempdata)) {
		LastSpec.storeDatatype(CString(_T("RAW DATA")));
		LastSpec.storeCalInfo(
			CalData.getSubstanceName(),
			CalData.GetCalWavelength(),
			CalData.getFitType(),
			CalData.getFitParms());
		// @@@ more ugly GPS hacking
		theApp.pGarminPort->getLoc(&lat,&lon,&alt);
		LastSpec.storeGpsLoc(lat,lon,alt);

		theApp.pGarminPort->getTimeData(&time,&date);
		LastSpec.storeGpsTime(time,date);

		GetSystemTime(&curtime);
		LastSpec.storeSysTime(curtime);

		LastSpec.storeSpecName(theApp.pSpecControl->GetSpecName(theApp.pSpecControl->GetCurSpec()));
		LastSpec.storeSpecSerial(theApp.pSpecControl->GetSpecSerial(theApp.pSpecControl->GetCurSpec()));

		return LastSpec.store(curSpectrometer,&tempdata,&SpecWavelengths);
	}
	return false;
}

bool 
CSpectrometer::Spectrum_AcquireBaselineSpectrum() 
{
	DoubleArray tempdata;
	double lat,lon,alt;
	INT32 time,date;
	SYSTEMTIME curtime;

	if (Spectrum_AcquireSpectrum(&tempdata)) {
		BaselineSpec.storeDatatype(CString(_T("BASELINE")));
		BaselineSpec.storeCalInfo(
			CalData.getSubstanceName(),
			CalData.GetCalWavelength(),
			CalData.getFitType(),
			CalData.getFitParms());
		// @@@ more ugly GPS hacking
		theApp.pGarminPort->getLoc(&lat,&lon,&alt);
		BaselineSpec.storeGpsLoc(lat,lon,alt);

		theApp.pGarminPort->getTimeData(&time,&date);
		BaselineSpec.storeGpsTime(time,date);

		GetSystemTime(&curtime);
		BaselineSpec.storeSysTime(curtime);

		BaselineSpec.storeSpecName(theApp.pSpecControl->GetSpecName(theApp.pSpecControl->GetCurSpec()));
		BaselineSpec.storeSpecSerial(theApp.pSpecControl->GetSpecSerial(theApp.pSpecControl->GetCurSpec()));

		return BaselineSpec.store(curSpectrometer,&tempdata,&SpecWavelengths);
	}
	return false;
}
#endif

// ### // void 
// ### // CSpectrometer::ForkWrapperGetSpectrum(LPVOID param) {
// ### // 	DoubleArray dummy;
// ### // 	theApp.pSpecControl->pWrapper
// ### // 	dummy = pWrapper->getSpectrum(curSpectrometer);
// ### // }

void MyForkWrapperGetSpectrum(LPVOID param) {
	CString tval;
	int jj;
	double ii;
	for (ii=0,jj=rand(); ii<1e10; ii++) { jj=((jj<<1) | (jj>>31)); } // ### // test busy wait in main thread 
	tval.Format(_T("%d"),jj);
	theApp.ChangeStatusText(tval.GetString());
}

bool 
CSpectrometer::Spectrum_AcquireSpectrum(DoubleArray *datastore) 
{
	// The wrapper class is a bit broken regarding light control ...
	if ((curSpectrometer<0) || (SpecCount<=0))
		return false;

	theApp.ChangeStatusText(_T("Initializing Spectrometer ... "));

	// Turn on light ...
	pWrapper->setStrobeEnable(curSpectrometer,1);

	// Set integration time, boxcar and averaging to current setting
	if ((int)(parm_integration_time*1000) != pWrapper->getIntegrationTime(curSpectrometer))
		pWrapper->setIntegrationTime(curSpectrometer,(int)(parm_integration_time*1000));
	if (parm_boxcar_width != pWrapper->getBoxcarWidth(curSpectrometer))
		pWrapper->setBoxcarWidth(curSpectrometer,parm_boxcar_width);
	if (parm_averaging_num != pWrapper->getScansToAverage(curSpectrometer))
		pWrapper->setScansToAverage(curSpectrometer,parm_averaging_num);

	unsigned char buggy = pWrapper->getCorrectForElectricalDark(curSpectrometer);
	bool bugbool = (buggy==0?false:true);
	if (parm_electrical_dark_on != bugbool)
		pWrapper->setCorrectForElectricalDark(curSpectrometer,parm_electrical_dark_on);

	// Display acquisition splash ...
	// ... multiply by 2 because we have to acquire the extra spectrum to 
	//     turn off the lamp ... 
	// ... divide by 1000 because the resulting time is in ms ...
	//
	double splashTime=2.0*parm_integration_time*parm_averaging_num;
	// CSplashDialog::ShowSplashScreen(theApp.m_pMainWnd,splashTime,splashTime/100);
	// CSplashDialog::ShowSplashScreen(theApp.m_pMainWnd,20000,500);

	// Get Spectrum
	theApp.ChangeStatusText(_T("Acquiring Spectrum ... please wait ... "));
	*datastore = pWrapper->getSpectrum(curSpectrometer);

	// ### // CWinThread *thptr = AfxBeginThread((AFX_THREADPROC)MyForkWrapperGetSpectrum, NULL, THREAD_PRIORITY_TIME_CRITICAL, 0, 0, NULL);
	// ### // while (WaitForSingleObject(thptr->m_hThread,100) == WAIT_TIMEOUT) ; // ### // This seems silly. We are merely doing this to get the splash to work

	// ### // MSG msg;
	// ### // for (int ii=0; ii<20000; ii+=100) {
	// ### // 	Sleep(100);
    // ### //     while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    // ### //         ::TranslateMessage(&msg);
    // ### //         ::DispatchMessage(&msg);
    // ### //     }
	// ### // }


	theApp.ChangeStatusText(_T("... acquisition finished"));

	// Turn off light (won't happen until next spectrum is collected)
	pWrapper->setStrobeEnable(curSpectrometer,0);

	// Collect dummy spectrum to turn off light
	// ... make sure we keep the collection time as short as possible!
	if (parm_electrical_dark_reset)
		pWrapper->setIntegrationTime(curSpectrometer,10);
	pWrapper->setScansToAverage(curSpectrometer,1);
	SpecDummy = pWrapper->getSpectrum(curSpectrometer);

	// Hide splash screen in case we got the timer wrong ... 
	//CSplashDialog::HideSplashScreen();

	return true;
}

#if 0 // moved to CSpectrumData with the PaintSpectrum method
// Helper macro to print text with bottom left alignment within box
// The macro relies on yoff, y_ht, and xoff being set and will
// update yoff.	
#define TEXT_OUT_BL_C1(str,outline) \
	trect.left   = (int)(myrect->left +(xoff     )*xdpi/72.27)  ; \
	trect.right  = (int)(myrect->left +(xoff+x2ff)*xdpi/72.27)+1; \
	trect.top    = (int)(myrect->top  +(yoff     )*ydpi/72.27)  ; \
	trect.bottom = (int)(myrect->top  +(yoff+y_ht)*ydpi/72.27)+1; \
	\
	pDC->SetTextAlign(TA_BOTTOM|TA_LEFT); \
	pDC->TextOutA(trect.left,trect.bottom, str); \
	if (outline) pDC->FrameRect(&trect,&blackb);

// Helper macro to print text with top left alignment within box
// The macro relies on yoff, y_ht, and xoff being set and will
// update yoff.	
#define TEXT_OUT_TL_C1(str,outline) \
	trect.left   = (int)(myrect->left +(xoff     )*xdpi/72.27)  ; \
	trect.right  = (int)(myrect->left +(xoff+x2ff)*xdpi/72.27)+1; \
	trect.top    = (int)(myrect->top  +(yoff     )*ydpi/72.27)  ; \
	trect.bottom = (int)(myrect->top  +(yoff+y_ht)*ydpi/72.27)+1; \
	\
	pDC->SetTextAlign(TA_TOP|TA_LEFT); \
	pDC->TextOutA(trect.left,trect.top, str); \
	if (outline) pDC->FrameRect(&trect,&blackb);

// Helper macro to print MULTILINE text with top left alignment within box
// The macro relies on yoff, y_ht, and xoff being set and will
// update yoff.	
#define TEXT_DRW_TL_C2(str,outline) \
	trect.left   = (int)(myrect->left +(xoff+x2ff)*xdpi/72.27);   \
	trect.right  = (int)(myrect->right-(xoff     )*xdpi/72.27)+1; \
	trect.top    = (int)(myrect->top  +(yoff     )*ydpi/72.27);   \
	trect.bottom = (int)(myrect->top  +(yoff+y_ht)*ydpi/72.27)+1; \
	\
	pDC->DrawText(str,&trect,DT_TOP|DT_LEFT); \
	if (outline) pDC->FrameRect(&trect,&blackb);

	

// Helper macro to print text with bottom left alignment within box
// The macro relies on yoff, y_ht, and xoff being set and will
// update yoff.	
#define TEXT_OUT_BL_C2(str,outline) \
	trect.left   = (int)(myrect->left +(xoff+x2ff)*xdpi/72.27)  ; \
	trect.right  = (int)(myrect->right-(xoff     )*xdpi/72.27)+1; \
	trect.top    = (int)(myrect->top  +(yoff     )*ydpi/72.27)  ; \
	trect.bottom = (int)(myrect->top  +(yoff+y_ht)*ydpi/72.27)+1; \
	\
	pDC->SetTextAlign(TA_BOTTOM|TA_LEFT); \
	pDC->TextOutA(trect.left,trect.bottom, str); \
	if (outline) pDC->FrameRect(&trect,&blackb);

// Helper macro to print text with bottom left alignment within box
// The macro relies on yoff, y_ht, xoff, and x2ff being set and will
// update yoff.	
#define TEXT_OUT_CT(str,outline) \
	trect.left   = (int)(myrect->left +(xoff     )*xdpi/72.27)  ; \
	trect.right  = (int)(myrect->right-(xoff     )*xdpi/72.27)+1; \
	trect.top    = (int)(myrect->top  +(yoff     )*ydpi/72.27)  ; \
	trect.bottom = (int)(myrect->top  +(yoff+y_ht)*ydpi/72.27)+1; \
	\
	pDC->SetTextAlign(TA_BOTTOM|TA_CENTER); \
	pDC->TextOutA((trect.left+trect.right)/2,trect.bottom, str); \
	if (outline) pDC->FrameRect(&trect,&blackb);
#endif

#if 0 // code moved to DlgBarAcq and wrapper became inline ...
void 
CSpectrometer::PaintDReport(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect)
{
	CSpectrumData DiffSpec;
	DiffSpec=LastSpec - BaselineSpec;

	PaintReport(pDC,pInfo,myrect,DiffSpec);
}
#endif

void 
CSpectrometer::PaintRReport(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect)
{
	// By default generate report for first spectrum
	int curPage=0;

	// If we are in print mode, create report for appropriate spectrum
	if (pInfo != NULL)
		curPage=pInfo->m_nCurPage-1;

	if ((0 <= curPage) && (curPage < theApp.pSpecControl->RevSpecs.size()))
		theApp.pSpecControl->RevSpecs[curPage].specDiff.PaintReport(pDC,pInfo,myrect);
}


void 
CSpectrometer::PaintCSpectra(CDC *pDC, LPRECT myrect)
{
#if 0 // we want to move all this into the Calibration curve itself
	CString tval=_T("[DEBUG] PaintCSpectra()");
	theApp.ChangeStatusText(tval.GetString());

	CPen colors[MAX_CAL_SPEC+1];
	CBrush black(RGB(  0,  0,  0));
	CBrush white(RGB(255,255,255));
	CSpectrumData *tcal;

	// @@@ // Changed CalData. to CalDataPtr->
	CCalibrationCurve *CalDataPtr=&(((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl);

	double wmin=1e10,wmax=-1e10,imin=1e10,imax=-1e10;

	int ii;

	colors[0].CreatePen(PS_SOLID, 1, RGB(  0,  0,  0));
	for (ii=1; ii<=MAX_CAL_SPEC; ii++)
		colors[ii].CreatePen(PS_SOLID,1,RPRainbowColors::rainbow_rgb(ii,MAX_CAL_SPEC));

	for (ii=0; ii<CalDataPtr->getCount(); ii++) {
		tcal=CalDataPtr->get(ii);
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
		
	// @@@ I think this should go into CreateAxes ... define whether to clear background
	// and frame graph ... 
	// pDC->FillRect(myrect,&white);
	// pDC->FrameRect(myrect,&black);

	for (ii=0; ii<CalDataPtr->getCount(); ii++)
		PaintSpectrum(pDC,myrect,
			&(colors[ii+1]),*(CalDataPtr->get(ii)),
			wmin,wmax,imin,imax,
			(ii==0)?true:false);
#else
	// @@@ // There should be a proper selection of calibration curve
	// @@@ // in fact we should not be using this function at all
	// @@@ // Changed CalData. to CalDataPtr->
	CCalibrationCurve *CalDataPtr=&(((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl);

	CalDataPtr->PaintCalSpectra(pDC,NULL,myrect);
#endif

}

#if 0 // moved to DlgBarAcq
void 
CSpectrometer::PaintDSpectra(CDC *pDC, LPRECT myrect) 
{
	CPen black(PS_SOLID, 1, RGB(  0,  0,  0));
	CPen gray( PS_SOLID, 1, RGB(128,128,128));
	CPen red(  PS_SOLID, 3, RGB(255,  0,  0));
	bool redraw_axes=true;
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
		PaintSpectrum(pDC,myrect,&gray,
			BaselineSpec,
			wmin,wmax,imin,imax,
			redraw_axes);
		redraw_axes=false;
	}

	if (LastSpec.is_valid() && paintopt_D_raw) {
		PaintSpectrum(pDC,myrect,&black,
			LastSpec,
			wmin,wmax,imin,imax,
			redraw_axes);
		redraw_axes=false;
	}

	if ((BaselineSpec.is_valid()) && (LastSpec.is_valid()) && paintopt_D_diff) {
		// @@@ This should really be a difference ... create appropriate
		// operator for CSpectrumData ...
		PaintSpectrum(pDC,myrect,&red,
			DiffSpec,
			wmin,wmax,imin,imax,
			redraw_axes);
		redraw_axes=false;
	}

}
#endif

void 
CSpectrometer::PaintRSpectra(CDC *pDC, CPrintInfo *pInfo, LPRECT myrect) 
{
	CPen *colors;
	CBrush black(RGB(  0,  0,  0));
	CBrush white(RGB(255,255,255));

	int ii,scnt,smin,smax;
	double wmin=1e10,wmax=-1e10,imin=1e10,imax=-1e10;
	CSpectrumData *tcal;

	if (RevSpecs.empty())
		return;

	scnt=RevSpecs.size();

	// If we are in print mode, create report for appropriate spectrum
	if (pInfo != NULL) {
		smin=pInfo->m_nCurPage-1;
		smax=min(smin+1,scnt);

		colors=(CPen *)calloc(scnt+1,sizeof(CPen));
		colors[0].CreatePen(PS_SOLID, 1, RGB(  0,  0,  0));
		for (ii=0; ii<scnt; ii++)
			colors[ii + 1].CreatePen(
				PS_SOLID,
				RevSpecs[ii].highlight?2:1,
				RGB(0,0,0));
	}
	// in view mode, print all
	else {
		smin=0;
		smax=scnt;

		colors=(CPen *)calloc(scnt+1,sizeof(CPen));
		colors[0].CreatePen(PS_SOLID, 1, RGB(  0,  0,  0));
		for (ii=0; ii<scnt; ii++)
			colors[ii + 1].CreatePen(
				PS_SOLID,
				RevSpecs[ii].highlight?2:1,
				RPRainbowColors::rainbow_rgb(ii,scnt));
	}


	// @@@ we should probably add functionality to paint baseline / raw data as well?


	for (ii=smin; ii<smax; ii++) {
		tcal=&(RevSpecs[ii].specDiff);
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

	// @@@ should be part of CreateAxes
	// pDC->FillRect(myrect,&white);
	// pDC->FrameRect(myrect,&black);



	for (ii=smin; ii<smax; ii++)
		PaintSpectrum(pDC,myrect,
			&(colors[ii+1]),RevSpecs[ii].specDiff,
			wmin,wmax,imin,imax,
			((ii==0)||(pInfo != NULL))?true:false);

	// I believe we need to explicitly free the pens ... 
	for (ii=0; ii<scnt; ii++)
		colors[ii + 1].DeleteObject();
	free(colors);
}

//! @param pDC - pointer to drawing canvas
//! @param myrect - dimensions of whole canvas (what are the boundary conditions?)
//! @param pen - line color of spectrum
//! @param data - wavelengths and intensities
//! @param wmin - minimum wavelength of spectrum
//! @param wmax - maximum wavelength of spectrum
//! @param imin - minimum intensity / count value of spectrum
//! @param imax - maximum intensity / count value of spectrum
//! @param redraw_axis - allow suppressing axis drawing if we are painting multiple spectra
bool 
CSpectrometer::PaintSpectrum(CDC *pDC, LPRECT myrect,CPen *pen,
							 CSpectrumData &data,
							 double wmin, double wmax,
							 double imin, double imax, 
							 bool redraw_axis,
							 bool wavlen_line)
{
#if 1 // We should be using the spectrum's own draw function!
	return data.PaintSpectrum(pDC,myrect,pen,wmin,wmax,imin,imax,redraw_axis,wavlen_line);
#else
	double xx,yy;

	CString tval=_T("[DEBUG] PaintSpectrum()");
	theApp.ChangeStatusText(tval.GetString());

	int ii,imn,imx;
//	double maxint;
//	double xscale;
//	double yscale;
//	int xsgn=1,ysgn=1;
//	double wmn,wmx
	double wcl;

//#define BORDER 5
//#define N_TICS 7
//#define TICKLN 4
//#define TEXT_H 10
//#define TEXT_W 50

	CBrush blackb(RGB(  0,  0,  0));
	CPen blackp(PS_SOLID,1,RGB(  0,  0,  0));

	RECT orect=*myrect;
	CRPGraphHelper gh(pDC,&orect,wmin,wmax,imin,imax);

	if (! data.is_valid_wv())
		return false;

	int sDC=pDC->SaveDC();

	gh.setClipRectangle(pDC,&orect);

#if 0
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
#endif

#if 0 // this optmization is bad because the spectrum response is not linear ...
	// We might not be drawing the whole spectrum ... so determine indexes
	imn=(int)(data.getCount() * ( (wmin-data.getMinW()) / (data.getMaxW()-data.getMinW())));
	imx=(int)(data.getCount() * ( (wmax-data.getMinW()) / (data.getMaxW()-data.getMinW())));

	// @@@ This will likely lead to really odd results if there is some data ...
	//     the way out of this is probably to plot the data using the wavelengths ...
	if (imn<0)
		imn=0;
	if (imx>data.getCount())
		imx=data.getCount();

//	// @@@ in the special case of no data this should crash ...
//	wmn=data.getWavelengths().getDoubleValues()[imn  ];
//	wmx=data.getWavelengths().getDoubleValues()[imx-1];

//	if (myrect->right < myrect->left)   xsgn = -1;
//	if (myrect->top   < myrect->bottom) ysgn = -1;

//	// use indexes for xscale ... 
//	xscale = ((double)(myrect->right - myrect->left   - xsgn*(2*BORDER + 2*TICKLN + 2 + TEXT_W)))/(imx -imn );
//	yscale = ((double)(myrect->top   - myrect->bottom - ysgn*(2*BORDER + 2*TICKLN + 2 + TEXT_H)))/(imax-imin);

//	char tempstr[256];
//	sprintf(tempstr,"xs=%g ys=%g",xscale,yscale);
//	theApp.ChangeStatusText(tempstr);
#endif

	if (redraw_axis)
	{
		// @@@ Axes testing

		gh.setFormatXLabel("%.0f");
		gh.setFormatYLabel("%.0f");

		gh.CreateAxes(pDC,&orect,wmin,wmax,imin,imax);

		// draw vertical line "cursor" for wavelength of interest
		if (wavlen_line && cursor_wavelen>=0) {
			// @@@ // hypothetically all spectra should have their
			// @@@ // calibration wavelength edited properly ... so
			// wcl=CalData.GetCalWavelength();
			// wcl=data.getCalW();
			xx=cursor_wavelen;
			data.getXY(&xx,&yy);
			pDC->MoveTo(gh.CalcXConv(xx),orect.bottom);
			pDC->LineTo(gh.CalcXConv(xx),orect.top);
		}
	}


	// draw ... on top ...
	pDC->SelectObject(pen);
	gh.drawTraceDataCoord(pDC,
		data.getCount(),
		data.getWavelengths().getDoubleValues(),
		data.getIntensities().getDoubleValues());

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

	// @@@ // this should be ok, I think ... err ... well ... this only works if the appropriate calibration wavelength was written into the spectrum ... so no, this is bad ...
	xx=data.getCalW();
	yy;
	if (data.getXY(&xx,&yy))
	{
		if (wavlen_line) {
			pDC->SelectObject(pen);
			pDC->MoveTo(gh.CalcXConv(xx),orect.bottom);
			pDC->LineTo(gh.CalcXConv(xx),orect.top);
		}

		pDC->SelectObject(&blackp);
		gh.drawGlyphDataCoord(pDC,xx,yy,CRPGraphHelper::GLYPH_CROSS);
	}

	pDC->RestoreDC(sDC);

	return true;
#endif
}


/*
bool CSpectrometer::AcquireCalSpec(int idx, double set_conc, double integration, long average, long boxwidth)
{
	DoubleArray data;
	CSpectrumData tspec;

	// pWrapper->setIntegrationTime(curSpectrometer,integration);
	SetIntegrationTime(integration);
	SetScansToAverage(average);
	SetBoxcarWidth(boxwidth);
	// @@@ there is a potential problem in the behavior of 
	// this function *if* we were to switch between control and acquisition modes 
	// because the *default* acquisition parameters get reset here ... 
	// ... I believe this is currently compensated for in DlgBarAcq during the 
	// loading of the calibration curve ... 
	Spectrum_AcquireSpectrum(&data);
	// CalSpec[idx].store(curSpectrometer,&data,&SpecWavelengths);
	tspec.store(curSpectrometer,&data,&SpecWavelengths,set_conc);

	tspec.storeSpecName(theApp.pSpecControl->GetSpecName(theApp.pSpecControl->GetCurSpec()));
	tspec.storeSpecSerial(theApp.pSpecControl->GetSpecSerial(theApp.pSpecControl->GetCurSpec()));


	
	if ((idx > -1) && (idx<CalData.getCount())) { 
		CalData.replace(idx, tspec); 
	} else {
		if (CalData.add(tspec) < 0)
			return false;
	}
	return true;
}
*/

/*
void 
CSpectrometer::PaintCalCurve(CDC *pDC, LPRECT myrect,CPen *pen,
								  double *xv, double *yv, int pc)
{
	int ii;
	CPen *oldpen;

	CBrush black(RGB(  0,  0,  0));
	CBrush white(RGB(255,255,255));

	double xmin,xmax,ymin,ymax;
	double xscale,yscale;

	if ((pc<=0) || (xv == NULL) || (yv == NULL))
		return;

	xmin=xmax=xv[0];
	ymin=ymax=yv[0];
	for (ii=1; ii<pc; ii++) 
	{
		if (xv[ii] > xmax) xmax=xv[ii];
		if (xv[ii] < xmin) xmin=xv[ii];
		if (yv[ii] > ymax) ymax=yv[ii];
		if (yv[ii] < ymin) ymin=yv[ii];
	}

	// this is not really good ... we should probably root this at (0,0)
	xscale = ((double)(myrect->right - myrect->left  ))/(xmax - xmin);
	yscale = ((double)(myrect->top   - myrect->bottom))/(ymax - ymin);

	// Clear first ...
	pDC->FillRect(myrect,&white);
	pDC->FrameRect(myrect,&black);

	// draw ... on top ... note that for a single point this will draw nothing ...
	oldpen = pDC->SelectObject(pen);
	for (ii=0; ii<pc-1; ii++)
	{
		pDC->MoveTo(
			(int)((xv[ ii ] - xmin)*xscale + myrect->left),
			(int)((yv[ ii ] - ymin)*yscale + myrect->bottom));
		pDC->LineTo(
			(int)((xv[ii+1] - xmin)*xscale + myrect->left),
			(int)((yv[ii+1] - ymin)*yscale + myrect->bottom));
	}
	pDC->SelectObject(oldpen);
}
*/

#if 0 // moved into DlgBarAcq
double CSpectrometer::UpdatePPM()
{
	double in;
	double wv=CalData.GetCalWavelength();
	double in2;
	double wv2=CalData.GetCalWavelength();
	double ppm,dil;
	CString tval;
	
	LastSpec.getXY(&wv,&in);
	BaselineSpec.getXY(&wv2,&in2);
	BaselineSpec.storeConc(0);

	ppm=CalData.bestFitXY(in,in2);

	// @@@ Brutal hack ... totally the wrong place ... compare with code
	// in MainFrm ... both of these should really be the same ...
	dil=atof(theApp.pCustInfoData->getDataDilutionStr().GetString());
	tval.Empty();
	if (dil!=1)
		tval.AppendFormat(_T("%.1f * %.1f = "),ppm,dil);
	tval.AppendFormat(_T("%.1f ppm"),ppm*dil);

	((CMainFrame *)(theApp.m_pMainWnd))->DlgTop_SetPPM(tval);

	// @@@ Is this the right place?
	LastSpec.storeConc(ppm);
	return ppm;
}
#endif

bool CSpectrometer::Spectrum_LoadAllSpectra(CString *filename) 
{
	// @@@ ADD CODE
	return false;
}

#if 0 // moved to DlgBarAcq
bool CSpectrometer::Spectrum_SaveAllSpectra(CString *fname)
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

	// @@@ This should be implicitly populated from the raw data in the '-' operation
	//tSpec.specDiff.storeCalInfo(
	//		CalData.getSubstanceName(),
	//		CalData.GetCalWavelength(),
	//		CalData.getFitType(),
	//		CalData.getFitParms());

	return tSpec.Save(fname);

	/*
	if (fname == NULL) {

		// TODO: Add your command handler code here
		CFileDialog TempSaver(FALSE,_T(".csv"), NULL, 0, "Spectrum Files (*.csv)|*.csv|All Files(*.*)|*.*||");

		TempSaver.m_pOFN->lpstrInitialDir=theApp.pConfig->getSavepath();
		if (TempSaver.DoModal() == IDOK)
		{
			// we have a selection
			theApp.ChangeStatusText(_T("Selection OK"));
			filename=TempSaver.GetPathName();
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
		calfile.WriteString(BaselineSpec.serializeData(CSpectrumData::outputCSV).GetString());
		calfile.WriteString(LastSpec.serializeData(CSpectrumData::outputCSV).GetString());
		calfile.WriteString(DiffSpec.serializeData(CSpectrumData::outputCSV).GetString());

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
	*/
}
#endif

bool CSpectrometer::RevSpec_SortAFileName(CDSpectrumData& d1, CDSpectrumData& d2)
{
	return d1.specFileName < d2.specFileName;
}

bool CSpectrometer::RevSpec_SortDFileName(CDSpectrumData& d1, CDSpectrumData& d2)
{
	return d1.specFileName > d2.specFileName;
}

bool CSpectrometer::RevSpec_SortATimeStamp(CDSpectrumData& d1, CDSpectrumData& d2)
{
	return 
		( d1.specDiff.getGPSData().getGPSDate() <  d2.specDiff.getGPSData().getGPSDate()) ||
		((d1.specDiff.getGPSData().getGPSDate() == d2.specDiff.getGPSData().getGPSDate()) &&
		 (d1.specDiff.getGPSData().getGPSTime() <  d2.specDiff.getGPSData().getGPSTime()));
}

bool CSpectrometer::RevSpec_SortDTimeStamp(CDSpectrumData& d1, CDSpectrumData& d2)
{
	return
		( d1.specDiff.getGPSData().getGPSDate() >  d2.specDiff.getGPSData().getGPSDate()) ||
		((d1.specDiff.getGPSData().getGPSDate() == d2.specDiff.getGPSData().getGPSDate()) &&
		 (d1.specDiff.getGPSData().getGPSTime() >  d2.specDiff.getGPSData().getGPSTime()));

}

bool CSpectrometer::RevSpec_SortAPipeKm(CDSpectrumData& d1, CDSpectrumData& d2)
{
	return d1.specDiff.getPipeKm() < d2.specDiff.getPipeKm();
}

bool CSpectrometer::RevSpec_SortDPipeKm(CDSpectrumData& d1, CDSpectrumData& d2)
{
	return d1.specDiff.getPipeKm() > d2.specDiff.getPipeKm();
}

bool CSpectrometer::RevSpec_SortAConc(CDSpectrumData& d1, CDSpectrumData& d2)
{
	return d1.specDiff.getConc() < d2.specDiff.getConc();
}

bool CSpectrometer::RevSpec_SortDConc(CDSpectrumData& d1, CDSpectrumData& d2)
{
	return d1.specDiff.getConc() > d2.specDiff.getConc();
}

bool CSpectrometer::RevSpec_SortADil(CDSpectrumData& d1, CDSpectrumData& d2)
{
	return d1.specDiff.getDilution() < d2.specDiff.getDilution();
}

bool CSpectrometer::RevSpec_SortDDil(CDSpectrumData& d1, CDSpectrumData& d2)
{
	return d1.specDiff.getDilution() > d2.specDiff.getDilution();
}

