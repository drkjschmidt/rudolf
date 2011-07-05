// spec4Doc.h : interface of the Cspec4Doc class
//


#pragma once


class Cspec4Doc : public CDocument
{
protected: // create from serialization only
	Cspec4Doc();
	DECLARE_DYNCREATE(Cspec4Doc)

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
	virtual ~Cspec4Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	HMENU m_hMyMenu,m_hMenu1,m_hMenu2,m_hMenu3;
	virtual HMENU GetDefaultMenu();
	afx_msg void OnViewOthermenu();
	afx_msg void OnViewControlMenu();
	afx_msg void OnViewAcquisitionMenu();
	afx_msg void OnViewReviewMenu();
	void SetProgMode(int mode);

	afx_msg void OnViewSelectdatafiles();
	void AddDataFiles(bool clear=true);
};


