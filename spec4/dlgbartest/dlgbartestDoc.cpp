// dlgbartestDoc.cpp : implementation of the CdlgbartestDoc class
//

#include "stdafx.h"
#include "dlgbartest.h"

#include "dlgbartestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdlgbartestDoc

IMPLEMENT_DYNCREATE(CdlgbartestDoc, CDocument)

BEGIN_MESSAGE_MAP(CdlgbartestDoc, CDocument)
END_MESSAGE_MAP()


// CdlgbartestDoc construction/destruction

CdlgbartestDoc::CdlgbartestDoc()
{
	// TODO: add one-time construction code here

}

CdlgbartestDoc::~CdlgbartestDoc()
{
}

BOOL CdlgbartestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CdlgbartestDoc serialization

void CdlgbartestDoc::Serialize(CArchive& ar)
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


// CdlgbartestDoc diagnostics

#ifdef _DEBUG
void CdlgbartestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CdlgbartestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CdlgbartestDoc commands
