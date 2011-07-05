#pragma once


// CDlgBarReview dialog

class CDlgBarReview : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgBarReview)

public:
	CDlgBarReview();   // standard constructor
	virtual ~CDlgBarReview();

// Dialog Data
	enum { IDD = IDD_DBAR_LEFT_REV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// according to http://support.microsoft.com/kb/185672
	afx_msg LONG OnInitDialog (UINT, LONG );

	DECLARE_MESSAGE_MAP()

public:
	//! Create the DialogBar (following MFC by Example)
	//
	// Unfortunately we don't necessarily have the global data variables initialized
	// at this point so populating the fields has to happen separately using Populate()
	BOOL InitDialog(void);

	void Populate(bool rebuild=false);

	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnRem();

	CListCtrl c_displayList;
	afx_msg void OnHdnItemclickDatalist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedHighlight();

protected:
	CSize CalcFixedLayout(BOOL bStretch,BOOL bHorz);
};
