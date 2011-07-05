// menutestView.cpp : implementation of the CmenutestView class
//

#include "stdafx.h"
#include "menutest.h"

#include "menutestDoc.h"
#include "menutestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmenutestView

IMPLEMENT_DYNCREATE(CmenutestView, CView)

BEGIN_MESSAGE_MAP(CmenutestView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CmenutestView construction/destruction

CmenutestView::CmenutestView()
{
	// TODO: add construction code here

}

CmenutestView::~CmenutestView()
{
}

BOOL CmenutestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CmenutestView drawing

void CmenutestView::OnDraw(CDC* /*pDC*/)
{
	CmenutestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CmenutestView printing

BOOL CmenutestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CmenutestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CmenutestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CmenutestView diagnostics

#ifdef _DEBUG
void CmenutestView::AssertValid() const
{
	CView::AssertValid();
}

void CmenutestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmenutestDoc* CmenutestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmenutestDoc)));
	return (CmenutestDoc*)m_pDocument;
}
#endif //_DEBUG


// CmenutestView message handlers
