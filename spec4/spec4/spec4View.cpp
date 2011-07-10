// spec4View.cpp : implementation of the Cspec4View class
//

#include "stdafx.h"
#include "spec4.h"

#include "spec4Doc.h"
#include "spec4View.h"
#include "WavlenCursor.h"
#include "RPGraphHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cspec4View

IMPLEMENT_DYNCREATE(Cspec4View, CView)

BEGIN_MESSAGE_MAP(Cspec4View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_GRAPHTYPE_CURRENTACQUISITION, &Cspec4View::OnGraphtypeCurrentacquisition)
	ON_COMMAND(ID_GRAPHTYPE_CALIBRATIONSPECTRA, &Cspec4View::OnGraphtypeCalibrationspectra)
	ON_COMMAND(ID_GRAPHTYPE_REVSPEC, &Cspec4View::OnGraphtypeRevspec)

	// Graphing options
	ON_COMMAND(ID_DSV_DATA, &Cspec4View::OnDsvData)
	ON_UPDATE_COMMAND_UI(ID_DSV_DATA, &Cspec4View::OnUpdateDsvData)

	ON_COMMAND(ID_DSV_BASELINE, &Cspec4View::OnDsvBaseline)
	ON_UPDATE_COMMAND_UI(ID_DSV_BASELINE, &Cspec4View::OnUpdateDsvBaseline)

	ON_COMMAND(ID_DSV_RAW, &Cspec4View::OnDsvRaw)
	ON_UPDATE_COMMAND_UI(ID_DSV_RAW, &Cspec4View::OnUpdateDsvRaw)
	ON_COMMAND(ID_GRAPHTYPE_REPORT, &Cspec4View::OnGraphtypeReport)
	ON_COMMAND(ID_VIEW_CURSOR, &Cspec4View::OnViewCursor)
END_MESSAGE_MAP()

// Cspec4View construction/destruction

Cspec4View::Cspec4View()
: currentView(VIEW_DATA_ACQ)
{
	// TODO: add construction code here
	if (logoPrint.LoadBitmapA("spec4-plogo-custom.bmp")) {
		theApp.ChangeDebugText(2,"Loading custom logo spec4-plogo-custom.bmp succdeed");
	} else if (logoPrint.LoadBitmapA("spec4-plogo.bmp")) {
		theApp.ChangeDebugText(2,"Loading logo spec4-plogo.bmp succdeed");
	} else {
		theApp.ChangeDebugText(2,"Loading logo spec4-plogo.bmp failed");
	}
}

Cspec4View::~Cspec4View()
{
}

BOOL Cspec4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cspec4View drawing

void Cspec4View::OnDraw(CDC* pDC)
{
	MyDrawAndPrint(pDC,NULL);
}

#define rpabs(a) ((a<0)?(-a):(a))
#define rpsgn(a) ((a<0)?(-1):(1))
#define rpmin(a,b) ((a<b)?(a):(b))
int Cspec4View::MyDrawAndPrint(CDC* pDC,CPrintInfo *pInfo, bool count)
{
	RECT myrect,trect;
	int pages=0;

	Cspec4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	// TODO: add draw code for native data here
	GetClientRect(&myrect);

	// Canvas size in mm (why is this mm?)
	int horzsize=pDC->GetDeviceCaps(HORZSIZE);
	int vertsize=pDC->GetDeviceCaps(VERTSIZE);

	// Canvas size in pixels
	int horzres=pDC->GetDeviceCaps(HORZRES);
	int vertres=pDC->GetDeviceCaps(VERTRES);

	// Canvas Resolution in pixels/mm
	int hdps=horzres/horzsize;
	int vdps=vertres/vertsize;

	// Canvas resoluton in pixels/in ... not sure if it's better to query or calculate ...  
#if 1
	int hdpi = pDC->GetDeviceCaps(LOGPIXELSX);
	int vdpi = pDC->GetDeviceCaps(LOGPIXELSY);
#else
	int hdpi=(int)(25.4*(double)horzres/(double)horzsize);
	int vdpi=(int)(25.4*(double)vertres/(double)vertsize);
#endif 

	RECT bdrect;
	pDC->GetBoundsRect(&bdrect,0);

	CString tval;
	LONG xcon,ycon; // constraints
	LONG ival; //@@@ should be removed in favor of xcon,ycon
	int tempView=currentView;
#define DEBUG_GRID
#ifdef DEBUG_GRID
	CBrush tempbrush(RGB(0,0,0));
#endif

	tval.Format(_T("(%d,%d,%d,%d) <-> w:%dmm w:%dpx h:%dmm h:%dpx <-> (%d,%d,%d,%d)"),
		myrect.left,myrect.top,myrect.right,myrect.bottom,
		horzsize, horzres, vertsize, vertres,
		bdrect.left,bdrect.top,bdrect.right,bdrect.bottom);
	theApp.ChangeStatusText(tval.GetString());

	if (pDC->IsPrinting()) {
		// get the max page dimensions
		myrect.left=0;
		myrect.top=0;
		myrect.bottom=vertres;
		myrect.right=horzres;

#ifdef DEBUG_GRID
		CRPGraphHelper::drawGridImperial(pDC, myrect, myrect.left,myrect.bottom,0.25,0.25);
#endif

		// crop by margins
		// @@@ there are no sanity checks here so margins could swap things in ugly ways ...
		myrect.left   += rpsgn(myrect.right-myrect.left)*hdpi*theApp.pConfig->getPrintLeftMargin();
		myrect.right  -= rpsgn(myrect.right-myrect.left)*hdpi*theApp.pConfig->getPrintRightMargin();
		myrect.top    += rpsgn(myrect.bottom-myrect.top)*vdpi*theApp.pConfig->getPrintTopMargin();
		myrect.bottom -= rpsgn(myrect.bottom-myrect.top)*vdpi*theApp.pConfig->getPrintBottomMargin();

#ifdef DEBUG_GRID
		pDC->FrameRect(&myrect,&tempbrush);
#endif
		// override view and force report in printing modes
		tempView=VIEW_DATA_REPORT;
	} 
	else 
	{
		// If we are not printing we are, by definition,
		// only creating one page ...
		count=false;
		pages=1;
	}

	if (theApp.pSpecControl != NULL)
	{
		switch (tempView) {
			case VIEW_DATA_ACQ: // View currently acquired spectra
				// We rely on the idea that we can't get here in printing mode
				trect=myrect;
				((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.PaintDSpectra(pDC,&trect);
				break;
			case VIEW_DATA_REV: // Review mode, print multiple spectra
				// We rely on the idea that we can't get here in printing mode
				trect=myrect;
				// @@@ // ugly ... this should move into CDlgBarRev
				theApp.pSpecControl->PaintRSpectra(pDC,pInfo,&trect);
				break;
			case VIEW_PANELS_CAL_CSP: // Configuration mode, show multiple spectra and calibration curve
				// We rely on the idea that we can't get here in printing mode
				trect=myrect;
				trect.bottom=(trect.top+trect.bottom)/2;
				// @@@ // theApp.pSpecControl->PaintCalCurve(pDC, &trect);
				((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl.PaintCalCurve(pDC, pInfo, &trect);

				trect=myrect;
				trect.top=(trect.top+trect.bottom)/2;
				// @@@ // theApp.pSpecControl->PaintCSpectra(pDC, &trect);
				((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl.PaintCalSpectra(pDC,pInfo, &trect);
				break;
#if 0 // I have no idea what this is so take it out for now
			case VIEW_PANELS_CAL_CSP_DATA: // Not sure how we get here?
				if (count) {
					pages=1;
				}
				else {
					trect=myrect;
					trect.bottom=(trect.top+trect.bottom)/2;
					trect.right=(trect.left+trect.right)/2;
					// @@@ // theApp.pSpecControl->PaintCalCurve(pDC, &trect);
					((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl.PaintCalCurve(pDC,&trect);

					trect=myrect;
					trect.bottom=(trect.top+trect.bottom)/2;
					trect.left=(trect.left+trect.right)/2;
					// @@@ // theApp.pSpecControl->PaintCSpectra(pDC, &trect);
					((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl.PaintCalSpectra(pDC,&trect);
					

					trect=myrect;
					trect.top=(trect.top+trect.bottom)/2;
					theApp.pSpecControl->PaintDSpectra(pDC,&trect);
				}
				break;
#endif
			case VIEW_DATA_REPORT: // PRINT (as in printer) a report appropriate for the mode we are in ...
				// If "count" is true, we are being tasked to return the number of pages that we
				// have to print ...
				if (count) 
				{
					switch (theApp.getRunmode()) {
						case RUNMODE_ACQUISITION: // We print a report for the current spectrum
							pages=1;
							break;
						case RUNMODE_CONFIGURATION: // We print a report for the current calibration curve
							pages=1;
							break;
						case RUNMODE_REVIEW: // We print a report for multiple spectra, one page each
							pages=theApp.pSpecControl->RevSpecs.size();
							break;
					}
				}
				// ... otherwise we are supposed to print a page ... if we have a printing context this
				// this is easy because the printing context tells us which page we want. If not ...
				// @@@ Here we assume that we *are* in print mode ... if not, odd things might happen ...
				else 
				{
					switch (theApp.getRunmode()) {
						case RUNMODE_ACQUISITION: // We print a report for the current spectrum
						case RUNMODE_REVIEW: // We print a report for multiple spectra, one page each
							// Print logo into top 1" of screen 
							// ... this should ideally be part of the report generation
							// but since the report itself is such a hardcoded hack ...
							trect=myrect; 
							xcon=(LONG)rpabs(theApp.pConfig->getPrintLogoMaxWidth()*hdpi);
							ycon=(LONG)rpabs(theApp.pConfig->getPrintLogoMaxHeight()*vdpi);
							xcon=rpmin(xcon,rpabs(myrect.right-myrect.left));
							ycon=rpmin(ycon,rpabs(myrect.bottom-myrect.top));
							xcon=xcon*rpsgn(myrect.right-myrect.left);
							ycon=ycon*rpsgn(myrect.bottom-myrect.top);
							trect.right=myrect.left+xcon;
							trect.bottom=myrect.top+ycon;
							logoPrint.ConstBlt(pDC,&trect,
								theApp.pConfig->getPrintLogoMaxWidth(),
								theApp.pConfig->getPrintLogoMaxHeight(),
								theApp.pConfig->getPrintLogoXoff(),
								theApp.pConfig->getPrintLogoYoff(),
								theApp.pConfig->getPrintLogoEscape());
#ifdef DEBUG_GRID
							pDC->FrameRect(&trect,&tempbrush);
#endif

							// Print report header on top of logo. The header will have 
							// the "CONCENTRATION REPORT" title in the first inch but will
							// leave the data beneath that
							//
							// I believe the report header is 4" tall
							//
							trect=myrect;
							ival=(LONG)rpabs(4*vdpi);
							ival=rpmin(ival,rpabs(myrect.bottom-myrect.top));
							ival=ival*rpsgn(myrect.bottom-myrect.top);
							trect.bottom=trect.top+ival;
							if (theApp.getRunmode() == RUNMODE_ACQUISITION) {
								((CMainFrame *)(AfxGetMainWnd()))->m_DlgLeftAcq.PaintDReport(pDC,pInfo,&trect);
							} else {
								theApp.pSpecControl->PaintRReport(pDC, pInfo, &trect);
							}

							// Print the spectrum
							//
							// Make this another 4" tall
							//
							trect.top=trect.bottom;
							trect.bottom=myrect.bottom;
							ival=(LONG)rpabs(4*vdpi);
							ival=rpmin(ival,rpabs(myrect.bottom-myrect.top));
							ival=ival*rpsgn(myrect.bottom-myrect.top);
							trect.bottom=trect.top+ival;
							if (theApp.getRunmode() == RUNMODE_ACQUISITION) {
								((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.PaintDSpectra(pDC,&trect);
							} else {
								theApp.pSpecControl->PaintRSpectra(pDC, pInfo, &trect);
							}

							// Print the calibration spectrum into the remaining space
							trect.top=trect.bottom;
							trect.bottom=myrect.bottom;
							{
								
								if (theApp.getRunmode() == RUNMODE_ACQUISITION) {
									((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.CalDataAcq.PaintCalCurve(pDC,pInfo,&trect);
								} else {
									// @@@ Hack around crash when we aren't actually printing and pInfo is NULL!
									int curPage=(pInfo==NULL)?0:pInfo->m_nCurPage-1;

									if ((0 <= curPage) && (curPage < theApp.pSpecControl->RevSpecs.size()))
										theApp.pSpecControl->RevSpecs[curPage].specDiff.PaintCalCurve(pDC,&trect);
								}
							}

							break;
						case RUNMODE_CONFIGURATION: // We print a report for the current calibration curve
							// Print logo into top 1" of screen 
							// ... this should ideally be part of the report generation
							// but since the report itself is such a hardcoded hack ...
							trect=myrect; 
							xcon=(LONG)rpabs(theApp.pConfig->getPrintLogoMaxWidth()*hdpi);
							ycon=(LONG)rpabs(theApp.pConfig->getPrintLogoMaxHeight()*vdpi);
							xcon=rpmin(xcon,rpabs(myrect.right-myrect.left));
							ycon=rpmin(ycon,rpabs(myrect.bottom-myrect.top));
							xcon=xcon*rpsgn(myrect.right-myrect.left);
							ycon=ycon*rpsgn(myrect.bottom-myrect.top);
							trect.right=myrect.left+xcon;
							trect.bottom=myrect.top+ycon;
							logoPrint.ConstBlt(pDC,&trect,
								theApp.pConfig->getPrintLogoMaxWidth(),
								theApp.pConfig->getPrintLogoMaxHeight(),
								theApp.pConfig->getPrintLogoXoff(),
								theApp.pConfig->getPrintLogoYoff(),
								theApp.pConfig->getPrintLogoEscape());

							// Print report header on top of logo. The header will have 
							// the "CONCENTRATION REPORT" title in the first inch but will
							// leave the data beneath that
							//
							// I believe the report header is 4.7" tall
							//
							trect=myrect;
							ival=(LONG)rpabs(5*vdpi);
							ival=rpmin(ival,rpabs(myrect.bottom-myrect.top));
							ival=ival*rpsgn(myrect.bottom-myrect.top);
							trect.bottom=trect.top+ival;
							((CMainFrame *)(AfxGetMainWnd()))->m_DlgLeftCtrl.CalDataCtrl.PaintReport(pDC,pInfo,&trect);

							// Paint the Calibration Curve on the left 
							trect.top=trect.bottom;
							trect.bottom=myrect.bottom;
							trect.left=myrect.left;
							trect.right=(myrect.left+myrect.right)/2;
							((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl.PaintCalCurve(pDC,pInfo,&trect);

							// Paint the spectra on the right
							trect.left=trect.right;
							trect.right=myrect.right;
							((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl.PaintCalSpectra(pDC,pInfo,&trect);

							break;
					}
				}
				break;
		}
	}
	// On print canvas, add page number ... but we don'w have anything initialized here ...
	if (pDC->IsPrinting() && pInfo!=NULL) {
		int saveDC=pDC->SaveDC();
		CFont pnmFont;
		pnmFont.CreatePointFont((12 * 10),_T("Arial"),pDC);

		tval.Format(_T("%d/%d"),
			pInfo->m_nCurPage,
			pInfo->GetMaxPage());
		pDC->SelectObject(&pnmFont);
		pDC->SetTextAlign(TA_BOTTOM|TA_RIGHT);
		pDC->TextOutA(myrect.right,myrect.bottom, tval);

		pDC->RestoreDC(saveDC);
	}

	return pages;
}


// Cspec4View printing

BOOL Cspec4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	// pInfo->SetMaxPage(4);
	return DoPreparePrinting(pInfo);
}

void Cspec4View::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	pInfo->SetMaxPage(MyDrawAndPrint(pDC,pInfo,true));
}

void Cspec4View::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class

	// CView::OnPrint(pDC, pInfo);
	MyDrawAndPrint(pDC,pInfo);
}

void Cspec4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cspec4View diagnostics

#ifdef _DEBUG
void Cspec4View::AssertValid() const
{
	CView::AssertValid();
}

void Cspec4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cspec4Doc* Cspec4View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cspec4Doc)));
	return (Cspec4Doc*)m_pDocument;
}
#endif //_DEBUG


// Cspec4View message handlers

void Cspec4View::OnGraphtypeCurrentacquisition()
{
	// TODO: Add your command handler code here
	currentView=VIEW_DATA_ACQ;
	Invalidate();
}

void Cspec4View::OnGraphtypeCalibrationspectra()
{
	// TODO: Add your command handler code here
	currentView=VIEW_PANELS_CAL_CSP;
	Invalidate();
}

void Cspec4View::OnGraphtypeRevspec()
{
	// TODO: Add your command handler code here
	currentView=VIEW_DATA_REV;
	Invalidate();
}

void Cspec4View::OnDsvData()
{
	theApp.pSpecControl->PaintOpt_SetDSpecDiff(!theApp.pSpecControl->PaintOpt_GetDSpecDiff());
	Invalidate();
}

void Cspec4View::OnUpdateDsvData(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(theApp.pSpecControl->PaintOpt_GetDSpecDiff());
}

void Cspec4View::OnDsvBaseline()
{
	theApp.pSpecControl->PaintOpt_SetDSpecBaseline(!theApp.pSpecControl->PaintOpt_GetDSpecBaseline());
	Invalidate();
}

void Cspec4View::OnUpdateDsvBaseline(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(theApp.pSpecControl->PaintOpt_GetDSpecBaseline());
}

void Cspec4View::OnDsvRaw()
{
	theApp.pSpecControl->PaintOpt_SetDSpecRaw(!theApp.pSpecControl->PaintOpt_GetDSpecRaw());
	Invalidate();
}

void Cspec4View::OnUpdateDsvRaw(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(theApp.pSpecControl->PaintOpt_GetDSpecRaw());
}

void Cspec4View::OnGraphtypeReport()
{
	// TODO: Add your command handler code here
	currentView=VIEW_DATA_REPORT;
	Invalidate();
}



void Cspec4View::OnViewCursor()
{
	// TODO: Add your command handler code here
	theApp.ChangeStatusText(_T("Open Preferences Dialog"));

	CWavlenCursor *CursorDialog = new CWavlenCursor();
	CursorDialog->DoModal();
	delete CursorDialog;

}
