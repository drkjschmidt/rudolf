// lltestsView.cpp : implementation of the ClltestsView class
//

#include "stdafx.h"
#include "lltests.h"

#include "lltestsDoc.h"
#include "lltestsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ClltestsView

IMPLEMENT_DYNCREATE(ClltestsView, CView)

BEGIN_MESSAGE_MAP(ClltestsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// ClltestsView construction/destruction

ClltestsView::ClltestsView()
{
	// TODO: add construction code here

}

ClltestsView::~ClltestsView()
{
}

BOOL ClltestsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// ClltestsView drawing

void ClltestsView::OnDraw(CDC* /*pDC*/)
{
	ClltestsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// ClltestsView printing

BOOL ClltestsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void ClltestsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void ClltestsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// ClltestsView diagnostics

#ifdef _DEBUG
void ClltestsView::AssertValid() const
{
	CView::AssertValid();
}

void ClltestsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

ClltestsDoc* ClltestsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ClltestsDoc)));
	return (ClltestsDoc*)m_pDocument;
}
#endif //_DEBUG


// ClltestsView message handlers
