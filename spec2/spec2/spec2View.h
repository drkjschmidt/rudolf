// spec2View.h : interface of the Cspec2View class
//


#pragma once


class Cspec2View : public CView
{
protected: // create from serialization only
	Cspec2View();
	DECLARE_DYNCREATE(Cspec2View)

// Attributes
public:
	Cspec2Doc* GetDocument() const;

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
	virtual ~Cspec2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in spec2View.cpp
inline Cspec2Doc* Cspec2View::GetDocument() const
   { return reinterpret_cast<Cspec2Doc*>(m_pDocument); }
#endif

