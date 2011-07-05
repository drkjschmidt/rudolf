#pragma once

#define PT_TO_PX(pt,dpi) (int)((double)(pt)*(double)(dpi)/72.27)
#define PX_TO_PT(px,dpi) (int)((double)(px)*72.27/(double)(dpi))

class CRPGraphHelper
{
public:
	CRPGraphHelper(void);
	CRPGraphHelper(CDC *pDC,LPRECT vrect,double xmin, double xmax, double ymin, double ymax);
	~CRPGraphHelper(void);

	//! Draw axes based on current format settings
	//
	void CreateAxes(CDC *pDC, LPRECT vrect,double xmin, double xmax, double ymin, double ymax, bool do_clear=true, bool do_frame=false);
	void CreateAxes(CDC *pDC);

	//! Get the inner canvas rectangle
	//! This way we don't need to redraw axes to set that boundary
	//
	inline void getGraphRect(LPRECT vrect) { *vrect=crect; }

	//! Calculate integer location x value within canvas / pDC rect
	inline int CalcXConv(double x) {
		if (mayconv)
			return int(dtoix_f*(x - dtoix_vmin) + dtoix_wmin);
		return 0;
	}

	//! Calculate integer location x value within canvas / pDC rect
	inline int CalcYConv(double y) {
		if (mayconv)
			return int(dtoiy_f*(y - dtoiy_vmin) + dtoiy_wmin);
		return 0;
	}

	inline void setBorder(double w) { sz_oborder=w; }
	inline void setTickLength(double l) { sz_ticklno=l; }
	inline void setBorderTextLeft(double w)   { sz_text_lb=w; }
	inline void setBorderTextRight(double w)  { sz_text_rb=w; }
	inline void setBorderTextTop(double w)    { sz_text_tb=w; }
	inline void setBorderTextBottom(double w) { sz_text_bb=w; }

	inline void setGlyphSize(double d) { sz_glyp_dm=d; }

	inline void setTextHeight(double h) { sz_text_ht=h; }
	inline void setTextFace(CString f) { fm_text_fc=f; }

	inline void setFormatXLabel(CString s) { fm_xnumber=s; }
	inline void setFormatYLabel(CString s) { fm_ynumber=s; }

	inline void setForceXZero(bool f) { fl_force_xzero=f; }
	inline void setForceYZero(bool f) { fl_force_yzero=f; }

	inline void setPlotLabels(bool f) { fl_plot_number=f; }

	inline void setLineColor(COLORREF c) { linec=c; }
	inline void setBgColor(COLORREF c) { backc=c; }

	inline void setDoClear(bool s) { drawbg=s; }
	inline void setDoFrame(bool s) { drawframe=s; }

	enum {
		GLYPH_CIRCLE = (int)0x0001,
		GLYPH_HLINE  = (int)0x0002,
		GLYPH_VLINE  = (int)0x0004,
		GLYPH_CROSS  = (int)0x0006,
	};

	//! Draw a glyph given data coordinates
	//
	//! This function will convert to drawing canvas coordinates before drawing the glyph.
	//! Drawing will be done with canvas presets so you can set them before calling this.
	void drawGlyphDataCoord(CDC *pDC, double x, double y,int type);

	//! Draw a string at given data coordinates
	//
	//! This function will convert to drawing canvas coordinates before drawing the text.
	//! Drawing will be done with canvas presets so you can set them before calling this.
	void drawTextDataCoord(CDC *pDC, double x, double y,CString text,double ptht=0, CString face="", UINT align=0);

	//! Draw a trace given data coordinates
	//
	//! This functions will convert to drawing canvas coordinates before drawing the trace.
	//! Drawing will be done with canvas presets so you can set them before calling this.
	void drawTraceDataCoord(CDC *pDC, int npts, double *x, double *y);

	//! Set clip rectangle
	//
	//! This is a helper function to deal with the idiosyncracies of drawing vs.
	//! printing canvases and the different clip rectangles they need
	//
	//! You probably want to wrap any call to this into a SaveDC / RestoreDC pair ... 
	void setClipRectangle(CDC *pDC, LPRECT clrect);




protected:
//#define BORDER 5
//#define N_TICS 7
//#define TICKLN 4
//#define TEXT_H 10
//#define TEXT_W 50

	void recalcFactors(int xdpi,int ydpi);

//	int xdpi,ydpi; // DPI conversion factors. These should be reset every time we draw!

	double sz_oborder; // width of white border surrounding everything (pt)
	double sz_ticklno; // length of ticks on outside of axes (pt)
	double sz_text_lb; // space to leave for text on left side (pt)
	double sz_text_rb; // space to leave for text on right side (pt)
	double sz_text_bb; // space to leave for text on bottom side (pt)
	double sz_text_tb; // space to leave for text on top side (pt)

	double sz_text_ht; // Text height (pt)
	CString fm_text_fc; // Font face (name)

	double sz_line_bw; // Border line width (pt)

	double sz_glyp_dm;   // Glyph size (pt)

	CString fm_xnumber; // What number format to use for x axis labels (%.0f)
	CString fm_ynumber; // What number format to use for y axis labels (%.0f)

	int ct_nticks; // Number of ticks to draw (on one end of axis)

	bool fl_force_xzero; // include x==0 even if it's not part of data?
	bool fl_force_yzero; // include y==0 even if it's not part of data?

	bool fl_plot_number; // plot tick labels

	COLORREF linec;
	COLORREF backc;

	bool knowpreset;
	double txmin,xmin,txmax,xmax,tymin,ymin,tymax,ymax;
	int xnticks,xpticks;
	int ynticks,ypticks;
	double tick_w,tick_h;
	RECT brect,crect; // boundary and graph canvas respectively

	bool drawbg,drawframe; // whether to clear background / draw frame around whole graph
	bool mayconv;
	int xsgn,ysgn;
	double dtoix_f,dtoix_vmin,dtoix_wmin;
	double dtoiy_f,dtoiy_vmin,dtoiy_wmin;
	int glypsz; // glyph size in pixels (calculated in recalc factors)

};
