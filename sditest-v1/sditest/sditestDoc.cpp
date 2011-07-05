// sditestDoc.cpp : implementation of the CsditestDoc class
//

#include "stdafx.h"
#include "sditest.h"

#include "sditestDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsditestDoc

IMPLEMENT_DYNCREATE(CsditestDoc, CDocument)

BEGIN_MESSAGE_MAP(CsditestDoc, CDocument)
//	ON_COMMAND(ID_FILE_OPEN, &CsditestDoc::OnFileOpen)
END_MESSAGE_MAP()


// CsditestDoc construction/destruction

CsditestDoc::CsditestDoc()
: m_nBugPosition(0)
{
	// TODO: add one-time construction code here

}

CsditestDoc::~CsditestDoc()
{
}

BOOL CsditestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_BugDataArray.RemoveAll();
	m_nBugPosition=-1;

	return TRUE;
}




// CsditestDoc serialization

void CsditestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		SBugData Data;
		CString strOneLine;
		CString strStatus;
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

		m_BugDataArray.RemoveAll();
		while (ar.ReadString(strOneLine)) {
			sscanf_s(strOneLine,"%g %g\n", &Data.x, &Data.y);
			m_BugDataArray.Add(Data);			

			strStatus.Format("Reading point %d.", m_BugDataArray.GetSize());
			pMain->ChangeStatusText(strStatus);
		}
		strStatus.Format("Loaded %d points.", m_BugDataArray.GetSize());
		pMain->ChangeStatusText(strStatus);

	}
}


// CsditestDoc diagnostics

#ifdef _DEBUG
void CsditestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CsditestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CsditestDoc commands

//void CsditestDoc::OnFileOpen()
//{
//	// TODO: Add your command handler code here
//}
