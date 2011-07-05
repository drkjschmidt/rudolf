#include "StdAfx.h"
#include "RPBitmap.h"

CRPBitmap::CRPBitmap(void)
{
}

CRPBitmap::~CRPBitmap(void)
{
}

BOOL CRPBitmap::LoadBitmap(LPCTSTR szFilename) 
{ 
	BOOL res;

	ASSERT(szFilename);
	DeleteObject();
	valid=false;

	// Conflation of code from http://www.codeguru.com/cpp/g-m/bitmap/article.php/c1707 for loading
	// and from http://www.codeguru.com/forum/showthread.php?t=354024 for getting image size
	HBITMAP hBitmap = NULL; 
	hBitmap = (HBITMAP)LoadImage(NULL, szFilename, IMAGE_BITMAP, 0, 0, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
	res=Attach(hBitmap);
	if (res) {
		BITMAP bitmap;
		GetBitmap(&bitmap);

		pxHeight= bitmap.bmHeight;
		pxWidth = bitmap.bmWidth;
		valid   = true;
	}
	return res;
}

#define rpabs(a) ((a<0)?(-a):(a))
#define rpsgn(a) ((a<0)?(-1):(1))
BOOL CRPBitmap::ConstBlt(CDC *pDC, LPRECT bbox)
{
	if (! valid)
		return FALSE;

    LONG maxWidth=bbox->right-bbox->left;
	LONG maxHeight=bbox->bottom-bbox->top;

	if ((maxWidth==0) || (maxHeight==0))
		return FALSE;

	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);
	dcMemory.SelectObject(this);

	double srcXtoY=rpabs((double)pxWidth  /(double)pxHeight );
	double dstXtoY=rpabs((double)maxWidth /(double)maxHeight);
	LONG dstWidth, dstHeight;

	// Constrained by X dimension of destination
	if (srcXtoY > dstXtoY) {
		dstHeight=(LONG)((rpsgn(maxHeight)*rpsgn(pxHeight)) * rpabs( ((double)maxWidth/(double)pxWidth) * (double)pxHeight ));
		dstWidth =(LONG)((rpsgn(maxWidth )*rpsgn(pxWidth )) * rpabs( ((double)maxWidth/(double)pxWidth) * (double)pxWidth  ));
	} 
	// Constrained by Y dimension of destination
	else {
		dstHeight=(LONG)((rpsgn(maxHeight)*rpsgn(pxHeight)) * rpabs( ((double)maxHeight/(double)pxHeight) * (double)pxHeight ));
		dstWidth =(LONG)((rpsgn(maxWidth )*rpsgn(pxWidth )) * rpabs( ((double)maxHeight/(double)pxHeight) * (double)pxWidth  ));
	}

	return pDC->StretchBlt(
		bbox->left, bbox->top, dstWidth+bbox->left, dstHeight+bbox->top, &dcMemory, 
		0,          0,            pxWidth,             pxHeight,               SRCCOPY);
}
