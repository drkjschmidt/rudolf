#include "StdAfx.h"
#include "StaticRefreshCalibration.h"
#include "spec4.h"

CStaticRefreshCalibration::CStaticRefreshCalibration(void)
{
}

CStaticRefreshCalibration::~CStaticRefreshCalibration(void)
{
}
BEGIN_MESSAGE_MAP(CStaticRefreshCalibration, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CStaticRefreshCalibration::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RECT myrect;

	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	GetClientRect(&myrect);
	theApp.pSpecControl->PaintCalCurve(&dc,&myrect);
}
