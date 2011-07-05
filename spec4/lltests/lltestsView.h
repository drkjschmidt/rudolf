// lltestsView.h : interface of the ClltestsView class
//


#pragma once


class ClltestsView : public CView
{
protected: // create from serialization only
	ClltestsView();
	DECLARE_DYNCREATE(ClltestsView)

// Attributes
public:
	ClltestsDoc* GetDocument() const;

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
	virtual ~ClltestsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in lltestsView.cpp
inline ClltestsDoc* ClltestsView::GetDocument() const
   { return reinterpret_cast<ClltestsDoc*>(m_pDocument); }
#endif

