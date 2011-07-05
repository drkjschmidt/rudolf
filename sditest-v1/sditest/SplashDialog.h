#pragma once

// CSplashDialog dialog

class CSplashDialog : public CDialog
{
	DECLARE_DYNAMIC(CSplashDialog)

public:
	CSplashDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSplashDialog();

// Dialog Data
	enum { IDD = IDD_SPLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	static void ShowSplashScreen(CWnd* pParentWnd);
	void HideSplashScreen(void);
	static bool PreTranslateAppMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();

};

static CSplashDialog *c_pSplashDialog;
