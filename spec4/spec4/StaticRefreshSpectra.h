#pragma once
#include "afxwin.h"

class CStaticRefreshSpectra :
	public CStatic
{
public:
	CStaticRefreshSpectra(void);
	~CStaticRefreshSpectra(void);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};
