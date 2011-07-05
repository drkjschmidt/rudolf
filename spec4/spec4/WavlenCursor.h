#pragma once
#include "afxwin.h"
#include "AutoRepeatButton.h"


// CWavlenCursor dialog

class CWavlenCursor : public CDialog
{
	DECLARE_DYNAMIC(CWavlenCursor)

public:
	CWavlenCursor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWavlenCursor();

// Dialog Data
	enum { IDD = IDD_CURSOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CAutoRepeatButton curUp;
	CAutoRepeatButton curDown;
	CAutoRepeatButton curLeft;
	CAutoRepeatButton curRight;
	CEdit curName;
	CEdit curWavelen;
	CEdit curIntensity;
	afx_msg void OnBnClickedNameUp();
	afx_msg void OnBnClickedNameDn();
	afx_msg void OnBnClickedWavlenDn();
	afx_msg void OnBnClickedWavlenUp();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	void Handler();

private:
	int cur_wv_min,cur_wv_index,cur_wv_max;
	int cur_nm_min,cur_nm_index,cur_nm_max;
	double cur_wavelen;
	double cur_intensity;

public:
	CScrollBar curHScroll;
	CScrollBar curVScroll;

	afx_msg void OnEnChangeCurCounts();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	virtual void OnOK();
};
