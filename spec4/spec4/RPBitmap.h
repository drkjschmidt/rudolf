#pragma once
#include "afxwin.h"

class CRPBitmap :
	public CBitmap
{
public:
	CRPBitmap(void);
	~CRPBitmap(void);

	//! Load bitmap from file
	//
	//! Simplistic approach that may only work for particular types of 
	//! bitmaps as outlined here http://www.codeguru.com/cpp/g-m/bitmap/article.php/c1707
	//!
	//! For the MS recommended version see this page
	//! http://support.microsoft.com/kb/158898
	//
	BOOL LoadBitmap(LPCTSTR szFilename);

	//! Plot stretched bitmap with constrained aspect ration to not exceed 
	//! the boundaries of provided RECT
	BOOL ConstBlt(CDC *pDC, LPRECT bbox);

	inline bool isValid() { return valid; }
	inline CSize getSize() { CSize tsize(pxWidth,pxHeight); return tsize; }

private:
	LONG pxHeight,pxWidth;
	bool valid;
};
