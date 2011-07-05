// dlgbartestDoc.h : interface of the CdlgbartestDoc class
//


#pragma once


class CdlgbartestDoc : public CDocument
{
protected: // create from serialization only
	CdlgbartestDoc();
	DECLARE_DYNCREATE(CdlgbartestDoc)

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
	virtual ~CdlgbartestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


