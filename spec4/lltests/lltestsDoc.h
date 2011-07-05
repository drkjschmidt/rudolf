// lltestsDoc.h : interface of the ClltestsDoc class
//


#pragma once


class ClltestsDoc : public CDocument
{
protected: // create from serialization only
	ClltestsDoc();
	DECLARE_DYNCREATE(ClltestsDoc)

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
	virtual ~ClltestsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


