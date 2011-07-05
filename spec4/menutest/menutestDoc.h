// menutestDoc.h : interface of the CmenutestDoc class
//


#pragma once


class CmenutestDoc : public CDocument
{
protected: // create from serialization only
	CmenutestDoc();
	DECLARE_DYNCREATE(CmenutestDoc)

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
	virtual ~CmenutestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	HMENU m_hMyMenu,m_hMenu1,m_hMenu2;
	virtual HMENU GetDefaultMenu();
	afx_msg void OnViewOthermenu();
};


