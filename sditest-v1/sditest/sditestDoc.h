// sditestDoc.h : interface of the CsditestDoc class
//


#pragma once

#include <afxtempl.h>
struct SBugData {
	float x;
	float y;
};

class CsditestDoc : public CDocument
{
protected: // create from serialization only
	CsditestDoc();
	DECLARE_DYNCREATE(CsditestDoc)

// Attributes
public:
	CArray <SBugData,SBugData> m_BugDataArray;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CsditestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_nBugPosition;
	// afx_msg void OnFileOpen();
};


