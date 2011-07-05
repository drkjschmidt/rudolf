#include "StdAfx.h"
#include "StaticRefreshSpectra.h"
#include "spec4.h"

CStaticRefreshSpectra::CStaticRefreshSpectra(void)
{
}

CStaticRefreshSpectra::~CStaticRefreshSpectra(void)
{
}
BEGIN_MESSAGE_MAP(CStaticRefreshSpectra, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CStaticRefreshSpectra::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RECT myrect;

	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	GetClientRect(&myrect);
	theApp.pSpecControl->PaintCSpectra(&dc,&myrect);
}
