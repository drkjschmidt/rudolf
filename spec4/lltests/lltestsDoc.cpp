// lltestsDoc.cpp : implementation of the ClltestsDoc class
//

#include "stdafx.h"
#include "lltests.h"

#include "lltestsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ClltestsDoc

IMPLEMENT_DYNCREATE(ClltestsDoc, CDocument)

BEGIN_MESSAGE_MAP(ClltestsDoc, CDocument)
END_MESSAGE_MAP()


// ClltestsDoc construction/destruction

ClltestsDoc::ClltestsDoc()
{
	// TODO: add one-time construction code here

}

ClltestsDoc::~ClltestsDoc()
{
}

BOOL ClltestsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// ClltestsDoc serialization

void ClltestsDoc::Serialize(CArchive& ar)
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


// ClltestsDoc diagnostics

#ifdef _DEBUG
void ClltestsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void ClltestsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// ClltestsDoc commands
