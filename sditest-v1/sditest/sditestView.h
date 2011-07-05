// sditestView.h : interface of the CsditestView class
//


#pragma once


class CsditestView : public CView
{
protected: // create from serialization only
	CsditestView();
	DECLARE_DYNCREATE(CsditestView)

// Attributes
public:
	CsditestDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CsditestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTrackAnimate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTrackLoadconfig();
	afx_msg void OnToolsOptions();
};

#ifndef _DEBUG  // debug version in sditestView.cpp
inline CsditestDoc* CsditestView::GetDocument() const
   { return reinterpret_cast<CsditestDoc*>(m_pDocument); }
#endif

