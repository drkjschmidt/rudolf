// dlgbartestView.cpp : implementation of the CdlgbartestView class
//

#include "stdafx.h"
#include "dlgbartest.h"

#include "dlgbartestDoc.h"
#include "dlgbartestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdlgbartestView

IMPLEMENT_DYNCREATE(CdlgbartestView, CView)

BEGIN_MESSAGE_MAP(CdlgbartestView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CdlgbartestView construction/destruction

CdlgbartestView::CdlgbartestView()
{
	// TODO: add construction code here

}

CdlgbartestView::~CdlgbartestView()
{
}

BOOL CdlgbartestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CdlgbartestView drawing

void CdlgbartestView::OnDraw(CDC* /*pDC*/)
{
	CdlgbartestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CdlgbartestView printing

BOOL CdlgbartestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CdlgbartestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CdlgbartestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CdlgbartestView diagnostics

#ifdef _DEBUG
void CdlgbartestView::AssertValid() const
{
	CView::AssertValid();
}

void CdlgbartestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdlgbartestDoc* CdlgbartestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdlgbartestDoc)));
	return (CdlgbartestDoc*)m_pDocument;
}
#endif //_DEBUG


// CdlgbartestView message handlers
