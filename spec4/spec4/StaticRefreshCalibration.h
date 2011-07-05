#pragma once
#include "afxwin.h"

class CStaticRefreshCalibration :
	public CStatic
{
public:
	CStaticRefreshCalibration(void);
	~CStaticRefreshCalibration(void);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};
