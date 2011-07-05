// spec2Doc.h : interface of the Cspec2Doc class
//


#pragma once


class Cspec2Doc : public CDocument
{
protected: // create from serialization only
	Cspec2Doc();
	DECLARE_DYNCREATE(Cspec2Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~Cspec2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


