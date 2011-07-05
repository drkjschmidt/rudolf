// spec2View.cpp : implementation of the Cspec2View class
//

#include "stdafx.h"
#include "spec2.h"

#include "spec2Doc.h"
#include "spec2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cspec2View

IMPLEMENT_DYNCREATE(Cspec2View, CView)

BEGIN_MESSAGE_MAP(Cspec2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Cspec2View construction/destruction

Cspec2View::Cspec2View()
{
	// TODO: add construction code here

}

Cspec2View::~Cspec2View()
{
}

BOOL Cspec2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cspec2View drawing

void Cspec2View::OnDraw(CDC* /*pDC*/)
{
	Cspec2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// Cspec2View printing

BOOL Cspec2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cspec2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cspec2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cspec2View diagnostics

#ifdef _DEBUG
void Cspec2View::AssertValid() const
{
	CView::AssertValid();
}

void Cspec2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cspec2Doc* Cspec2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cspec2Doc)));
	return (Cspec2Doc*)m_pDocument;
}
#endif //_DEBUG


// Cspec2View message handlers
