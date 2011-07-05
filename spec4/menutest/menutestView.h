// menutestView.h : interface of the CmenutestView class
//


#pragma once


class CmenutestView : public CView
{
protected: // create from serialization only
	CmenutestView();
	DECLARE_DYNCREATE(CmenutestView)

// Attributes
public:
	CmenutestDoc* GetDocument() const;

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
	virtual ~CmenutestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in menutestView.cpp
inline CmenutestDoc* CmenutestView::GetDocument() const
   { return reinterpret_cast<CmenutestDoc*>(m_pDocument); }
#endif

