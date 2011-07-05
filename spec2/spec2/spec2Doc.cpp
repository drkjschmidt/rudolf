// spec2Doc.cpp : implementation of the Cspec2Doc class
//

#include "stdafx.h"
#include "spec2.h"

#include "spec2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cspec2Doc

IMPLEMENT_DYNCREATE(Cspec2Doc, CDocument)

BEGIN_MESSAGE_MAP(Cspec2Doc, CDocument)
END_MESSAGE_MAP()


// Cspec2Doc construction/destruction

Cspec2Doc::Cspec2Doc()
{
	// TODO: add one-time construction code here

}

Cspec2Doc::~Cspec2Doc()
{
}

BOOL Cspec2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// Cspec2Doc serialization

void Cspec2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// Cspec2Doc diagnostics

#ifdef _DEBUG
void Cspec2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cspec2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cspec2Doc commands
