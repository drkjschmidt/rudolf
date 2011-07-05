#pragma once
#include "afxcmn.h"

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
	//! Show a splash screen
	//
	//! @param pParentWnd - window with respect to which splash screen shoulg be aligned
	//! @param splashTime - how long to wait until splash screen autohides [ms]
	//! @param splashStep - 0 means not to show a timer bar; >0 show timer bar
	//
	static bool ShowSplashScreen(CWnd* pParentWnd,double splashTime=10000.0,double stepTime=1000.0);

	//! Hide splash screen - from app side
	static bool HideSplashScreen(void);

	//! Windows message handler. Insert this in the message handler of the calling app
	static bool PreTranslateAppMessage(MSG* pMsg);

protected:

	//! Hide splash screen - from inside class
	bool HideSplashScreenInternal(void);

	//! Show/Hide timer bar on splash dialog
	bool SetupTimerBar(double splashTime,double stepTime);

	//! Process timer events (move timer bar, suicide splash)
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// Overloaded creation function for dialog
	virtual BOOL OnInitDialog();

	CProgressCtrl c_splashProgress;
};

extern CSplashDialog *c_pSplashDialog;
extern UINT_PTR m_counterTimeout;
extern UINT_PTR m_counterTicks;
