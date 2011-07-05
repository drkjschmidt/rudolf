#include "StdAfx.h"
#include "StaticRefresh.h"
#include "spec4.h"

CStaticRefresh::CStaticRefresh(void)
{
}

CStaticRefresh::~CStaticRefresh(void)
{
}
BEGIN_MESSAGE_MAP(CStaticRefresh, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CStaticRefresh::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RECT myrect;

	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	GetClientRect(&myrect);
	theApp.pSpecControl->PaintCalCurve(&dc,&myrect);
}
