// menutestDoc.cpp : implementation of the CmenutestDoc class
//

#include "stdafx.h"
#include "menutest.h"

#include "menutestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmenutestDoc

IMPLEMENT_DYNCREATE(CmenutestDoc, CDocument)

BEGIN_MESSAGE_MAP(CmenutestDoc, CDocument)
	ON_COMMAND(ID_VIEW_OTHERMENU, &CmenutestDoc::OnViewOthermenu)
END_MESSAGE_MAP()


// CmenutestDoc construction/destruction

CmenutestDoc::CmenutestDoc()
: m_hMyMenu(0)
, m_hMenu1(0)
, m_hMenu2(0)
{
	// TODO: add one-time construction code here

}

CmenutestDoc::~CmenutestDoc()
{
	::DestroyMenu(m_hMenu1);
	::DestroyMenu(m_hMenu2);
}

BOOL CmenutestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CmenutestDoc serialization

void CmenutestDoc::Serialize(CArchive& ar)
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


// CmenutestDoc diagnostics

#ifdef _DEBUG
void CmenutestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CmenutestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CmenutestDoc commands
HMENU CmenutestDoc::GetDefaultMenu()
{
  return m_hMyMenu;    // just use original default
}

void CmenutestDoc::OnViewOthermenu()
{
	static int curMenu=0;

	// TODO: Add your command handler code here
	if (curMenu==0) {
		if (m_hMenu2 == NULL) {
			// m_hMenu1 = (HMENU)AfxGetMainWnd()->GetMenu();
			m_hMenu2 = ::LoadMenu(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME1));
		}
		m_hMyMenu = m_hMenu2;
		curMenu=1;
	}
	else {
		if (m_hMenu1 == NULL) {
			m_hMenu1 = ::LoadMenu(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
		}
		m_hMyMenu = m_hMenu1;
		curMenu=0;
	}
	((CFrameWnd*)AfxGetMainWnd())->OnUpdateFrameMenu(NULL);
	AfxGetMainWnd()->DrawMenuBar();
}
