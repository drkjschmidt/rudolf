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
#define rpmin(a,b) ((a<b)?(a):(b))
BOOL CRPBitmap::ConstBlt(CDC *pDC, LPRECT bbox, 
		double maxw_in, double maxh_in, 
		double xoff_in, double yoff_in,
		bool escape)
{
	if (! valid)
		return FALSE;

	LONG bbxWidth=rpabs(bbox->right   - bbox->left);
	LONG bbxHeight=rpabs(bbox->bottom - bbox->top);

	LONG maxWidth,maxHeight;

	int xdpi=pDC->GetDeviceCaps(LOGPIXELSX);
	int ydpi=pDC->GetDeviceCaps(LOGPIXELSY);
	
	if (maxw_in != 0) {
		maxWidth=xdpi*maxw_in;
		// I am reasoning that we may want to manually override formatting
		// in which case this should no longer be constrained by the bounding
		// box ... 
		// maxWidth=rpmin(bbxWidth,maxWidth);
	}
	else {
		maxWidth=bbxWidth;
	}
	if (maxh_in != 0) {
		maxHeight=ydpi*maxh_in;
		// I am reasoning that we may want to manually override formatting
		// in which case this should no longer be constrained by the bounding
		// box ... 
		// maxHeight=rpmin(bbxHeight,maxHeight);
	}
	else {
		maxHeight=bbxHeight;
	}


	if ((maxWidth==0) || (maxHeight==0))
		return FALSE;

	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);
	dcMemory.SelectObject(this);

	double srcXtoY=rpabs((double)pxWidth  /(double)pxHeight );
	double dstXtoY=rpabs((double)maxWidth /(double)maxHeight);
	LONG dstX, dstY, dstWidth, dstHeight;

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

	// Origin / constraints calculation
	dstX=(bbxWidth -dstWidth )/2 + xoff_in*xdpi;
	dstY=(bbxHeight-dstHeight)/2 + yoff_in*ydpi;
	if (! escape) {
		if (dstX           < 0        ) dstX=0;
		if (dstX+dstWidth  > bbxWidth ) dstX=bbxWidth-dstWidth;
		if (dstY           < 0        ) dstY=0;
		if (dstY+dstHeight > bbxHeight) dstY=bbxHeight-dstHeight;
	}

	return pDC->StretchBlt(
		bbox->left+dstX, bbox->top+dstY, dstWidth, dstHeight, &dcMemory, 
		0,               0,              pxWidth,  pxHeight,  SRCCOPY);
}
