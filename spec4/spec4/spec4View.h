// spec4View.h : interface of the Cspec4View class
//


#pragma once
#include "RPBitmap.h"

class Cspec4View : public CView
{
protected: // create from serialization only
	Cspec4View();
	DECLARE_DYNCREATE(Cspec4View)

// Attributes
public:
	Cspec4Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	int MyDrawAndPrint(CDC *pDC, CPrintInfo *pInfo, bool count=false);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cspec4View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	int currentView;
public:
	afx_msg void OnGraphtypeCurrentacquisition();
	afx_msg void OnGraphtypeCalibrationspectra();

	enum ViewType {
		VIEW_DATA_ACQ = 0,
		VIEW_PANELS_CAL_CSP = 1,
		VIEW_PANELS_CAL_CSP_DATA = 2,
		VIEW_DATA_REV = 3,
		VIEW_DATA_REPORT = 4,
	};

	inline void setView(int type) { currentView=type; }
	inline int getView() { return currentView; }
	afx_msg void OnGraphtypeRevspec();
	afx_msg void OnDsvData();
	afx_msg void OnUpdateDsvData(CCmdUI *pCmdUI);
	afx_msg void OnDsvBaseline();
	afx_msg void OnUpdateDsvBaseline(CCmdUI *pCmdUI);
	afx_msg void OnDsvRaw();
	afx_msg void OnUpdateDsvRaw(CCmdUI *pCmdUI);
	afx_msg void OnGraphtypeReport();

protected:
	CRPBitmap logoPrint;
public:
	afx_msg void OnViewCursor();
};

#ifndef _DEBUG  // debug version in spec4View.cpp
inline Cspec4Doc* Cspec4View::GetDocument() const
   { return reinterpret_cast<Cspec4Doc*>(m_pDocument); }
#endif

