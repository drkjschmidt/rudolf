// dlgbartestView.h : interface of the CdlgbartestView class
//


#pragma once


class CdlgbartestView : public CView
{
protected: // create from serialization only
	CdlgbartestView();
	DECLARE_DYNCREATE(CdlgbartestView)

// Attributes
public:
	CdlgbartestDoc* GetDocument() const;

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
	virtual ~CdlgbartestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in dlgbartestView.cpp
inline CdlgbartestDoc* CdlgbartestView::GetDocument() const
   { return reinterpret_cast<CdlgbartestDoc*>(m_pDocument); }
#endif

