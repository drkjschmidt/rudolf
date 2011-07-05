#include "StdAfx.h"
#include <afxpriv.h>
#include "RPGraphHelper.h"
#include "spec4.h"

CRPGraphHelper::CRPGraphHelper(void)
: sz_oborder( 3.0) // originally  5px ... converts to 72.27[pt/in] *  5 / 120[dpi] =  3.0pt
, sz_ticklno( 2.4) // originally  4px ... converts to 72.27[pt/in] *  4 / 120[dpi] =  2.4pt
, sz_text_lb(30.1) // originally 50px ... converts to 72.27[pt/in] * 50 / 120[dpi] = 30.1pt
, sz_text_rb( 0.0) // originally  0px ... converts to 72.27[pt/in] *  0 / 120[dpi] =  0.0pt
, sz_text_bb( 6.0) // originally 10px ... converts to 72.27[pt/in] * 10 / 120[dpi] =  6.0pt
, sz_text_tb( 0.0) // originally  0px ... converts to 72.27[pt/in] *  0 / 120[dpi] =  0.0pt
, sz_text_ht( 6.0) // originally 10px ... converts to 72.27[pt/in] * 10 / 120[dpi] =  6.0pt
, fm_text_fc(_T("Arial"))
, sz_line_bw( 0.6) // originally  1px ... converts to 72.27[pt/in] *  1 / 120[dpi] =  0.6pt
, sz_glyp_dm( 2.4) // originally  3px ... converts to 72.27[pt/in] *  3 / 120[dpi] =  1.8pt
, fm_xnumber(_T("%.0f"))
, fm_ynumber(_T("%.0f"))
, ct_nticks(7)
, fl_force_xzero(false)
, fl_force_yzero(false)
, fl_plot_number(true)
, linec(RGB(  0,  0,  0))
, backc(RGB(255,255,255))
, drawbg(false)
, drawframe(false)
, mayconv(false)
, knowpreset(false)
{
}

CRPGraphHelper::CRPGraphHelper(CDC *pDC,LPRECT vrect,double vxmin, double vxmax, double vymin, double vymax)
: sz_oborder( 3.0) // originally  5px ... converts to 72.27[pt/in] *  5 / 120[dpi] =  3.0pt
, sz_ticklno( 2.4) // originally  4px ... converts to 72.27[pt/in] *  4 / 120[dpi] =  2.4pt
, sz_text_lb(30.1) // originally 50px ... converts to 72.27[pt/in] * 50 / 120[dpi] = 30.1pt
, sz_text_rb( 0.0) // originally  0px ... converts to 72.27[pt/in] *  0 / 120[dpi] =  0.0pt
, sz_text_bb( 6.0) // originally 10px ... converts to 72.27[pt/in] * 10 / 120[dpi] =  6.0pt
, sz_text_tb( 0.0) // originally  0px ... converts to 72.27[pt/in] *  0 / 120[dpi] =  0.0pt
, sz_text_ht( 6.0) // originally 10px ... converts to 72.27[pt/in] * 10 / 120[dpi] =  6.0pt
, fm_text_fc(_T("Arial"))
, sz_line_bw( 0.6) // originally  1px ... converts to 72.27[pt/in] *  1 / 120[dpi] =  0.6pt
, sz_glyp_dm( 2.4) // originally  3px ... converts to 72.27[pt/in] *  3 / 120[dpi] =  1.8pt
, fm_xnumber(_T("%.0f"))
, fm_ynumber(_T("%.0f"))
, ct_nticks(7)
, fl_force_xzero(false)
, fl_force_yzero(false)
, fl_plot_number(true)
, linec(RGB(  0,  0,  0))
, backc(RGB(255,255,255))
, drawbg(false)
, drawframe(false)
, mayconv(false)
, knowpreset(false)
{
	// @@@ Can't call delegate constructor so make sure initilizations
	// are kept in sync ... 


	xmin=txmin=vxmin;
	xmax=txmax=vxmax;
	ymin=tymin=vymin;
	ymax=tymax=vymax;
	brect=*vrect;
	knowpreset=true;

	// Calculate all conversion factors ... note: this changes vrect!
	int xdpi = pDC->GetDeviceCaps(LOGPIXELSX);
	int ydpi = pDC->GetDeviceCaps(LOGPIXELSY);

	recalcFactors(xdpi,ydpi);
}

CRPGraphHelper::~CRPGraphHelper(void)
{
}

#define ABS(a) ((a<0)?(-a):(a))


void CRPGraphHelper::CreateAxes(CDC *pDC, LPRECT vrect,double vxmin, double vxmax, double vymin, double vymax, bool do_clear, bool do_frame)
{
	// Calculate all conversion factors ... note: this changes vrect!

	xmin=txmin=vxmin;
	xmax=txmax=vxmax;
	ymin=tymin=vymin;
	ymax=tymax=vymax;
	brect=*vrect;
	drawbg=do_clear;
	drawframe=do_frame;
	knowpreset=true;


	// @@@ this is ugly but I can't think of a clean way 
	// of doing it better ... we do this in case the recalc
	// inside CreateAxes changes rect ... which it *sh
	CreateAxes(pDC);
	*vrect=crect;
}

void CRPGraphHelper::CreateAxes(CDC *pDC)
{
	int ii;

	// CString tval;
	CString tval=_T("[DEBUG] CreateAxes()");
	theApp.ChangeStatusText(tval.GetString());

	if (! knowpreset)
		return;

	// there is no guarantee we are in the same drawing context so 
	// recalculate pixel values
	int xdpi = pDC->GetDeviceCaps(LOGPIXELSX);
	int ydpi = pDC->GetDeviceCaps(LOGPIXELSY);
	recalcFactors(xdpi,ydpi);
	if (! mayconv)
		return;

	// Save DC status (pens, brushes etc.)
	int sDC=pDC->SaveDC();


	CBrush lineb(linec),backb(backc);
	CPen linep(PS_SOLID, PT_TO_PX(sz_line_bw,xdpi), linec); // @@@ this is questionable if xdpi!=ydpi ...

	// Clear background if requested
	if (drawbg) {
		pDC->FillRect(&brect,&backb);
	}

	pDC->SelectObject(&linep);

	// Draw frame around whole graph ... use lines instead of FrameRect because of printing
	// pDC->FrameRect(&brect,&lineb);
	if (drawframe) {
		pDC->MoveTo(brect.left   ,brect.top     );
		pDC->LineTo(brect.right-1,brect.top     );
		pDC->LineTo(brect.right-1,brect.bottom-1);
		pDC->LineTo(brect.left   ,brect.bottom-1);
		pDC->LineTo(brect.left   ,brect.top     );
	}

	// Draw graph frame ... use lines instead of FrameRect because of printing
	// pDC->FrameRect(&crect,&lineb);
	pDC->MoveTo(crect.left   ,crect.top     );
	pDC->LineTo(crect.right-1,crect.top     );
	pDC->LineTo(crect.right-1,crect.bottom-1);
	pDC->LineTo(crect.left   ,crect.bottom-1);
	pDC->LineTo(crect.left   ,crect.top     );

	// Prepare label font
	CFont tmpFont;
	tmpFont.CreatePointFont((int)(sz_text_ht * 10),fm_text_fc.GetString(),pDC);
	pDC->SelectObject(&tmpFont);
	pDC->SetBkMode(TRANSPARENT);

	// Draw x ticks ... 
	// ... this is *not* pixel optimized!!!
	pDC->SetTextAlign(TA_RIGHT|TA_TOP);
	for (ii=0; ii<=xnticks+xpticks; ii++) {
		pDC->MoveTo(
			CalcXConv(ii*tick_w + txmin),
			crect.bottom
			);
		pDC->LineTo(
			CalcXConv(ii*tick_w + txmin),
			crect.bottom - ysgn*PT_TO_PX(sz_ticklno,ydpi)
			);

		if (fl_plot_number) {
			tval.Format(fm_xnumber,tick_w*ii + txmin);
			pDC->TextOutA(
				CalcXConv(ii*tick_w + txmin),
				crect.bottom - ysgn*PT_TO_PX(sz_ticklno,ydpi),
				tval.GetString());
		}
	}

	// Draw y ticks ... 
	// This is *not* pixel optimized!!!
	pDC->SetTextAlign(TA_RIGHT|TA_BASELINE);
	for (ii=0; ii<=ynticks+ypticks; ii++) {
		pDC->MoveTo(
			crect.left,
			CalcYConv(ii*tick_h + tymin) + ysgn
			);
		pDC->LineTo(
			crect.left   - xsgn*PT_TO_PX(sz_ticklno,xdpi),
			CalcYConv(ii*tick_h + tymin) + ysgn
			);
		if (fl_plot_number) {
			tval.Format(fm_ynumber,tick_h*ii + tymin);
			pDC->TextOutA(
				crect.left   - xsgn*PT_TO_PX(sz_ticklno,xdpi),
				CalcYConv(ii*tick_h + tymin) + ysgn,
				tval.GetString());
		}
	}

	// Restore DC status ... make sure this gets called!
	pDC->RestoreDC(sDC);

}

void CRPGraphHelper::recalcFactors(int xdpi,int ydpi)
{
	if (! knowpreset)
		return;

	// Multipliers to deal with different viewport orientations
	xsgn=1; ysgn=1;
	if (brect.right < brect.left)   xsgn = -1;
	if (brect.top   < brect.bottom) ysgn = -1;

	// Recalculate bounding rectangle for axes
	crect.bottom = brect.bottom + ysgn*PT_TO_PX(sz_oborder + sz_ticklno + sz_text_bb,ydpi);
	crect.top    = brect.top    - ysgn*PT_TO_PX(sz_oborder + sz_ticklno + sz_text_tb,ydpi);
	crect.left   = brect.left   + xsgn*PT_TO_PX(sz_oborder + sz_ticklno + sz_text_lb,xdpi);
	crect.right  = brect.right  - xsgn*PT_TO_PX(sz_oborder + sz_ticklno + sz_text_rb,xdpi);

	// calculate x tick parameters
	if (txmin>=0 && txmax>=0) { // both greater 0
		xpticks=ct_nticks;
		xnticks=        0;
		if (fl_force_xzero)
			txmin=0;
		tick_w=(txmax-txmin)/ct_nticks;
	} else if (txmin<0 && txmax<0) { // both smaller 0
		xpticks=0;
		xnticks=ct_nticks;
		if (fl_force_xzero)
			txmax=0;
		tick_w=(txmax-txmin)/ct_nticks;
	} else if (ABS(txmin) < ABS(txmax)) { // more on right
		xpticks=ct_nticks;
		xnticks=(int)(ABS(txmin)*ct_nticks / ABS(txmax));
		tick_w=txmax/ct_nticks;
	} else { // more on left
		xpticks=(int)(ABS(txmax)*ct_nticks / ABS(txmin));
		xnticks=ct_nticks;
		tick_w=ABS(txmin)/ct_nticks;
	}

	// calculate y tick parameters
	if (tymin>=0 && tymax>=0) { // both greater 0
		ypticks=ct_nticks;
		ynticks=        0;
		if (fl_force_yzero)
			tymin=0;
		tick_h=(tymax-tymin)/ct_nticks;
	} else if (tymin<0 && tymax<0) { // both smaller 0
		ypticks=0;
		ynticks=ct_nticks;
		if (fl_force_yzero)
			tymax=0;
		tick_h=(tymax-tymin)/ct_nticks;
	} else if (ABS(tymin) < ABS(tymax)) { // more on top
		ypticks=ct_nticks;
		ynticks=(int)(ABS(tymin)*ct_nticks / ABS(tymax));
		tick_h=tymax/ct_nticks;
	} else { // more on bottom
		ypticks=(int)(ABS(tymax)*ct_nticks / ABS(tymin));
		ynticks=ct_nticks;
		tick_h=ABS(tymin)/ct_nticks;
	}

	// Calculate / set conversion factors
	dtoix_f = (double)(crect.right - crect.left - xsgn) / (txmax - txmin);
	dtoix_vmin = txmin;
	dtoix_wmin = crect.left;

	dtoiy_f = (double)(crect.top - crect.bottom - ysgn) / (tymax - tymin);
	dtoiy_vmin = tymin;
	dtoiy_wmin = crect.bottom;

	glypsz=PT_TO_PX(sz_glyp_dm,xdpi);

	// Prevent problems with empty ranges ... 
	mayconv = ((dtoix_f != 0) && (dtoiy_f != 0));
}

void CRPGraphHelper::drawGlyphDataCoord(CDC *pDC, double x, double y,int type) {

	if (! knowpreset)
		return;

	int xdc=CalcXConv(x);
	int ydc=CalcYConv(y);

	if (type&GLYPH_CIRCLE) {
		CRect glyploc(xdc-glypsz,ydc-glypsz,xdc+glypsz,ydc+glypsz);
		// pDC->SelectObject(&(colors[ii+1]));
		pDC->Ellipse(glyploc);
	}
	if (type&GLYPH_VLINE) {
		// pDC->SelectObject(&(colors[ii+1]));
		pDC->MoveTo(xdc,ydc-max(glypsz  ,4));
		pDC->LineTo(xdc,ydc-max(glypsz/3,2));
		pDC->MoveTo(xdc,ydc+max(glypsz/3,2));
		pDC->LineTo(xdc,ydc+max(glypsz  ,4));
	}
	if (type&GLYPH_HLINE) {
		// pDC->SelectObject(&(colors[ii+1]));
		pDC->MoveTo(xdc-max(glypsz  ,4),ydc);
		pDC->LineTo(xdc-max(glypsz/3,2),ydc);
		pDC->MoveTo(xdc+max(glypsz/3,2),ydc);
		pDC->LineTo(xdc+max(glypsz  ,4),ydc);
	}
}

void CRPGraphHelper::drawTextDataCoord(CDC *pDC, double x, double y,CString text,double ptht, CString face, UINT align) {

	if (! knowpreset)
		return;

	CFont tmpFont;

	CString useface=face;
	if (useface.IsEmpty()) {
		useface=fm_text_fc;
	}

	int useht=(int)(ptht*10);
	if (ptht == 0) {
		useht=(int)(sz_text_ht*10);
	}

	int xdc=CalcXConv(x);
	int ydc=CalcYConv(y);

	tmpFont.CreatePointFont(useht,useface,pDC);
	pDC->SelectObject(&tmpFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextAlign(align);

	pDC->TextOutA(xdc,ydc,text.GetString());
}

void CRPGraphHelper::drawTraceDataCoord(CDC *pDC, int npts, double *x, double *y) {

	if (! knowpreset)
		return;

	if (npts<=0)
		return;

#define DRAW_PLINE 0
#if DRAW_PLINE == 1
	// Use single polyline drawing operation ... surprisingly this does not appear to be 
	// faster ... ?!?
	LPPOINT plpts;
	if (plpts=(LPPOINT)calloc(imx-imn,sizeof(POINT))) {
		for (ii=imn; ii<imx; ii++) {
			plpts[ii].x=gh.CalcXConv(data.getWavelengths().getDoubleValues()[ii]);
			plpts[ii].y=gh.CalcYConv(data.getIntensities().getDoubleValues()[ii]);
		}
		pDC->Polyline(plpts,imx-imn);
		free(plpts);
	}

#else
	int ii;

	pDC->MoveTo(CalcXConv(x[0]),CalcYConv(y[0]));
	for (ii=1; ii<npts; ii++)
		pDC->LineTo(CalcXConv(x[ii]),CalcYConv(y[ii]));
#endif

}


void CRPGraphHelper::setClipRectangle(CDC *pDC, LPRECT clrect) {
	CRgn ClipRgn;
	CRect ClipRect=*clrect;

	// If the canvas is a view or print canvas, we are fine, but if it's a
	// preview canvas everything goes to hell in a handbasket, so work
	// around that ... 
	if (pDC->IsKindOf(RUNTIME_CLASS(CPreviewDC)))
	{
		CPreviewDC *pPrevDC = (CPreviewDC *)pDC;

		pPrevDC->PrinterDPtoScreenDP(&ClipRect.TopLeft());
		pPrevDC->PrinterDPtoScreenDP(&ClipRect.BottomRight());

		// Now offset the result by the viewport origin of
		// the print preview window...

		CPoint ptOrg;
		::GetViewportOrgEx(pDC->m_hDC,&ptOrg);
		ClipRect += ptOrg;
	}

	ClipRgn.CreateRectRgnIndirect(&ClipRect);
	pDC->SelectClipRgn(&ClipRgn);
}
