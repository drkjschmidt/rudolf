// sditestView.cpp : implementation of the CsditestView class
//

#include "stdafx.h"
#include "sditest.h"

#include "sditestDoc.h"
#include "sditestView.h"

#include "MainFrm.h"
#include "SetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsditestView

IMPLEMENT_DYNCREATE(CsditestView, CView)

BEGIN_MESSAGE_MAP(CsditestView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_TRACK_ANIMATE, &CsditestView::OnTrackAnimate)
	ON_WM_TIMER()
	ON_COMMAND(ID_TRACK_LOADCONFIG, &CsditestView::OnTrackLoadconfig)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CsditestView::OnToolsOptions)
END_MESSAGE_MAP()

// CsditestView construction/destruction

CsditestView::CsditestView()
{
	// TODO: add construction code here

}

CsditestView::~CsditestView()
{
}

BOOL CsditestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CsditestView drawing

void CsditestView::OnDraw(CDC* pDC)
{
	int N,ii;
	int start;

	CsditestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPen RedPen(PS_SOLID, 2, RGB(255,0,0));
	CPen BluePen(PS_SOLID, 2, RGB(0,0, 255));
	CPen *pOldPen = pDC->SelectObject(&RedPen);

	// Draw lines ...
	// HWND hWnd = WindowFromDC(*pDC);
	RECT wsize;
	GetClientRect(&wsize);

	pDC->SelectObject(&BluePen);
	pDC->MoveTo(wsize.left,wsize.top);
	pDC->LineTo(wsize.right,wsize.top);
	pDC->LineTo(wsize.right,wsize.bottom);
	pDC->LineTo(wsize.left,wsize.bottom);
	pDC->LineTo(wsize.left,wsize.top);

	pDC->SelectObject(&RedPen);
	pDC->MoveTo(wsize.left  +10,wsize.top    +10);
	pDC->LineTo(wsize.right -10,wsize.top    +10);
	pDC->LineTo(wsize.right -10,wsize.bottom -10);
	pDC->LineTo(wsize.left  +10,wsize.bottom -10);
	pDC->LineTo(wsize.left  +10,wsize.top    +10);

	int xdpi = pDC->GetDeviceCaps(LOGPIXELSX);
	int ydpi = pDC->GetDeviceCaps(LOGPIXELSY);
	CString tval;
	CFont tmpFont;

	// height [pts] = p [px] * 72.27 [pt/in] / ydpi [px/i] 
	int pxht = 50;
	int height = (int)((double)pxht * 10 * 72.27 / ydpi);

    tmpFont.CreatePointFont(height,_T("Arial"));
	tval.Format(_T("|Height|=%d xdpi=%d ydpi=%d"),height,xdpi,ydpi);
	pDC->SelectObject(&tmpFont);
	pDC->SetTextAlign(TA_CENTER|TA_TOP);
	pDC->TextOutA(
		(wsize.left+wsize.right)/2,
		(wsize.bottom+wsize.top)/2,
		tval.GetString());

	CPen blackp(PS_SOLID,1,RGB(  0,  0,  0));
	pDC->SelectObject(&blackp);

	pDC->MoveTo(wsize.left, (wsize.bottom+wsize.top)/2         );
	pDC->LineTo(wsize.right,(wsize.bottom+wsize.top)/2         );

	pDC->MoveTo(wsize.left, (wsize.bottom+wsize.top)/2 + pxht  );
	pDC->LineTo(wsize.right,(wsize.bottom+wsize.top)/2 + pxht  );

	// TODO: add draw code for native data here
	N=pDoc->m_BugDataArray.GetSize();

	for (ii=0; (ii < pDoc->m_nBugPosition-1) && (ii < N-2); ii++) {
		pDC->MoveTo(pDoc->m_BugDataArray[ ii ].x, pDoc->m_BugDataArray[ ii ].y);
		pDC->LineTo(pDoc->m_BugDataArray[ii+1].x, pDoc->m_BugDataArray[ii+1].y);
	}

	pDC->SelectObject(&BluePen);
	start=pDoc->m_nBugPosition;
	if (start < 0)
		start=0;
	for (ii=start; ii<N-2; ii++) {
		pDC->MoveTo(pDoc->m_BugDataArray[ ii ].x, pDoc->m_BugDataArray[ ii ].y);
		pDC->LineTo(pDoc->m_BugDataArray[ii+1].x, pDoc->m_BugDataArray[ii+1].y);
	}
	pDC->SelectObject(pOldPen);
	RedPen.DeleteObject();
	BluePen.DeleteObject();

	if(pDoc->m_nBugPosition!=-1) pDoc->m_nBugPosition++; 
	if(pDoc->m_nBugPosition>=N) 
	{ 
		pDoc->m_nBugPosition=-1; 

		// stop timer 1 
		KillTimer(1); 

		// redraw and erase so all lines are in initial state (blue) 
		InvalidateRect(NULL); 
	}


}


// CsditestView printing

BOOL CsditestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CsditestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CsditestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CsditestView diagnostics

#ifdef _DEBUG
void CsditestView::AssertValid() const
{
	CView::AssertValid();
}

void CsditestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CsditestDoc* CsditestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CsditestDoc)));
	return (CsditestDoc*)m_pDocument;
}
#endif //_DEBUG


// CsditestView message handlers

void CsditestView::OnTrackAnimate()
{
	// TODO: Add your command handler code here
	CsditestDoc *pDoc = GetDocument();
	pDoc->m_nBugPosition=0;
	SetTimer(1,20,NULL);


	CString strStatus;
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

	strStatus.Format("Starting animation");
	pMain->ChangeStatusText(strStatus);
}

void CsditestView::OnTimer(UINT_PTR nIDEvent)
{
	static int counter=0;
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent==1) {
		CString strStatus;
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
		CsditestDoc *pDoc = GetDocument();

		strStatus.Format("Redraw count %d : path count %d",counter++,pDoc->m_nBugPosition);
		pMain->ChangeStatusText(strStatus);

		// redraw whole window ... ouch
		InvalidateRect(NULL,FALSE);
	}
	CView::OnTimer(nIDEvent);
}

void CsditestView::OnTrackLoadconfig()
{
	// TODO: Add your command handler code here
	CFileDialog l_SampleDlg(
		TRUE,
		NULL,
		NULL,
		OFN_OVERWRITEPROMPT,
		"Text Files (*.txt)|*.txt|Comma Separated Values(*.csv)|*.csv|All Files (*.*)|*.*||");

	int iRet = l_SampleDlg.DoModal();
	CString l_strFileName;
    l_strFileName = l_SampleDlg.GetPathName();

    if (iRet == IDOK)
		MessageBox(l_strFileName);
	else
		MessageBox("No File Selected!");
}

void CsditestView::OnToolsOptions()
{
	// TODO: Add your command handler code here
	CSetupDlg SetupDlg = new CSetupDlg();

	SetupDlg.DoModal();
	MessageBox(SetupDlg.m_SetupComPort.GetString());
}
