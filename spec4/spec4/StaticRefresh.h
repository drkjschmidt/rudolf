#pragma once
#include "afxwin.h"

class CStaticRefresh :
	public CStatic
{
public:
	CStaticRefresh(void);
	~CStaticRefresh(void);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};
