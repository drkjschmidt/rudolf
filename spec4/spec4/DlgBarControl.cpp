// DlgBarControl.cpp : implementation file
//

#include "stdafx.h"
#include "spec4.h"
#include "DlgBarControl.h"
#include "SpecNameConvert.h"


// CDlgBarControl dialog

IMPLEMENT_DYNAMIC(CDlgBarControl, CDialogBar)

CDlgBarControl::CDlgBarControl()
: s_BnDlgLeftCtrlAcq0(true)
, s_BnDlgLeftCtrlAcq(false)
, s_BnDlgLeftCtrlDel(false)
, s_BnDlgLeftCtrlLoad(false)
, s_BnDlgLeftCtrlSave(false)
, interlock(false)
{
	pLock = new CDMLock("DlgBarControlUI");
}

CDlgBarControl::~CDlgBarControl()
{
	delete pLock;
	pLock = NULL;
}

void CDlgBarControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	// DDX_Text(pDX, IDC_EDIT_SUBNAME, m_substanceName);
	DDX_Control(pDX, IDC_EDIT_SUBNAME, c_substanceName);

	DDX_Control(pDX, IDC_COMBO2, c_specName);
	DDX_Control(pDX, IDC_COMBO3, c_integrationTime);
	DDX_Control(pDX, IDC_COMBO8, c_scansToAverage);
	DDX_Control(pDX, IDC_COMBO9, c_boxcarWidth);

	DDX_Control(pDX, IDC_LIST2, c_listControl);
	DDX_Control(pDX, IDC_EDIT1, c_concAVal);
	DDX_Control(pDX, IDC_COMBO7, c_wavelenSelect);

	DDX_Text(pDX, ID_TEXT_FORMULA, m_textFormula);
	DDX_Text(pDX, ID_TEXT_RSQUARED, m_textRSquared);

	DDX_Control(pDX, IDC_ERR_LOW  , c_errLow  );
	DDX_Control(pDX, IDC_ERR_HIGH , c_errHigh );
	DDX_Control(pDX, IDC_WARN_LOW , c_warnLow );
	DDX_Control(pDX, IDC_WARN_HIGH, c_warnHigh);

}


BEGIN_MESSAGE_MAP(CDlgBarControl, CDialogBar)
	// according to http://support.microsoft.com/kb/185672
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )

	// according to http://support.microsoft.com/kb/152376
	ON_UPDATE_COMMAND_UI(IDC_DLG_LEFT_CTRL_ACQ0, OnBnUpdatedDlgLeftCtrlAcq0)
	ON_BN_CLICKED(       IDC_DLG_LEFT_CTRL_ACQ0, OnBnClickedDlgLeftCtrlAcq0)
	ON_COMMAND(          ID_DLGLFT_CTRL_F1,      OnBnClickedDlgLeftCtrlAcqAccelF1)

	// according to http://support.microsoft.com/kb/152376
	ON_UPDATE_COMMAND_UI(IDC_DLG_LEFT_CTRL_ACQ, OnBnUpdatedDlgLeftCtrlAcq)
	ON_BN_CLICKED(       IDC_DLG_LEFT_CTRL_ACQ, OnBnClickedDlgLeftCtrlAcq)
	ON_COMMAND(          ID_DLGLFT_CTRL_F2,     OnBnClickedDlgLeftCtrlAcqAccelF2)

	// according to http://support.microsoft.com/kb/152376
	ON_UPDATE_COMMAND_UI(IDC_DLG_LEFT_CTRL_DEL, OnBnUpdatedDlgLeftCtrlDel)
	ON_BN_CLICKED(       IDC_DLG_LEFT_CTRL_DEL, OnBnClickedDlgLeftCtrlDel)

	// according to http://support.microsoft.com/kb/152376
	ON_UPDATE_COMMAND_UI(IDC_DLG_LEFT_CTRL_SAVE, OnBnUpdatedDlgLeftCtrlSave)
	ON_BN_CLICKED(       IDC_DLG_LEFT_CTRL_SAVE, OnBnClickedDlgLeftCtrlSave)




	ON_BN_CLICKED(IDC_DLG_LEFT_CTRL_LOAD, &CDlgBarControl::OnBnClickedDlgLeftCtrlLoad)
	ON_EN_CHANGE(IDC_COMBO7, &CDlgBarControl::OnEnChangeCombo7)
	ON_BN_CLICKED(IDC_DLG_LEFT_CTRL_RESET, &CDlgBarControl::OnBnClickedDlgLeftCtrlReset)
	ON_EN_KILLFOCUS(IDC_EDIT_SUBNAME, &CDlgBarControl::OnEnKillfocusEditSubname)
	ON_EN_CHANGE(IDC_EDIT_SUBNAME, &CDlgBarControl::OnEnChangeEditSubname)
END_MESSAGE_MAP()


// CDlgBarControl message handlers
LONG CDlgBarControl::OnInitDialog ( UINT wParam, LONG lParam)
{
	
	BOOL bRet = HandleInitDialog(wParam, lParam);

	if (!UpdateData(FALSE))
	{
	   TRACE0("Warning: UpdateData failed during dialog init.\n");
	}

	Populate();

	return bRet;
}

void CDlgBarControl::Readback(void)
{
	CString tval;
	double wavln,itime;
	long avg,boxw;

	// No point trying this if we don't have anywhere to store data
	if (theApp.pSpecControl == NULL)
		return;

	// try to make sure we only run Readback or Populate ...
	DWORD thread = GetCurrentThreadId();
	CString xval;
	xval.Format(_T(__FUNCTION__ " trying for UI lock ... [%lx,%d]"),thread,interlock?1:0);
	theApp.ChangeDebugText(2,xval.GetString());
	if (pLock->Lock()) {
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... succeeded"));
	} else {
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... lock call failed"));
		return;
	}
	if (! interlock) {
		interlock=true;
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... gained interlock"));

		c_substanceName.GetWindowTextA(tval);
		CalDataCtrl.setSubstanceName(tval);

		// @@@ this may not be a good approach in case our spectrometer list ends up being sparse ...
		theApp.pSpecControl->SelectSpectrometer(c_specName.GetCurSel());
		CalDataCtrl.setSpecName(  theApp.pSpecControl->GetSpecName(  theApp.pSpecControl->GetCurSpec()));
		CalDataCtrl.setSpecSerial(theApp.pSpecControl->GetSpecSerial(theApp.pSpecControl->GetCurSpec()));

		c_wavelenSelect.GetWindowTextA(tval);
		wavln=atof(tval.GetString());
		if ((wavln > theApp.pSpecControl->getMinW()) &&
			(wavln < theApp.pSpecControl->getMaxW()))
		{
			CalDataCtrl.SetCalWavelength(wavln);
			CalDataCtrl.CalcBestFit();
		}

		c_integrationTime.GetWindowTextA(tval);
		itime=atof(tval.GetString());
		if (itime<=0)
			itime=50;
		CalDataCtrl.setIntegrationTime(itime);

		c_scansToAverage.GetWindowTextA(tval);
		avg=atol(tval.GetString());
		if (avg<1)
			avg=50;
		CalDataCtrl.setSpectraToAverage(avg);

		c_boxcarWidth.GetWindowTextA(tval);
		boxw=atol(tval.GetString());
		if (boxw<0)
			boxw=100;
		CalDataCtrl.setBoxcarWidth(boxw);

		c_errLow.GetWindowTextA(tval);
		CalDataCtrl.setErrLow(atof(tval.GetString()));
		c_errHigh.GetWindowTextA(tval);
		CalDataCtrl.setErrHigh(atof(tval.GetString()));

		interlock=false;
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... released interlock"));

	}
	else 
	{
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... skipped because of interlock"));
	}

	// unlock
	thread = GetCurrentThreadId();
	xval.Format(_T(__FUNCTION__ " trying to release lock ... [%lx; %d]"),thread,interlock?1:0);
	theApp.ChangeDebugText(2,xval.GetString());
	if (pLock->Unlock()) {
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... succeeded"));
	} else {
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... lock call failed"));
	}
}

void CDlgBarControl::Populate(int selector)
{
	int ii;
	bool enableControls=false;
	CString tval,uval;

	// try to make sure we only run Readback or Populate ...
	DWORD thread = GetCurrentThreadId();
	CString xval;
	xval.Format(_T(__FUNCTION__ " trying for UI lock ... [%lx; %d]"),thread,interlock?1:0);
	theApp.ChangeDebugText(2,xval.GetString());
	if (pLock->Lock()) {
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... succeeded"));
	} else {
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... lock call failed"));
		return;
	}

	if (! interlock) {
		interlock=true;
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... gained interlock"));

		// If we already have controls, check whether we are allowed to edit ...
		enableControls=(CalDataCtrl.getCount()<=0);

		// Changing acquisition settings is only ok if we have NO spectra
		c_specName.EnableWindow(enableControls);
		c_integrationTime.EnableWindow(enableControls);
		c_scansToAverage.EnableWindow(enableControls);
		c_boxcarWidth.EnableWindow(enableControls);

		// Buttons are enabled as follows:
		// 
		// ... Acquire baseline: always
		// ... Acquire spectrum: have baseline
		// ... Delete: have spectrum
		// ... Load: always
		// ... Save: have at least baseline plus 2 spectra plus name plus calibration wavelength
		// ... Reset: always (@@@ not bothering with variable for now)
		s_BnDlgLeftCtrlAcq0=true;

		s_BnDlgLeftCtrlAcq=false;
		if ((theApp.pSpecControl != NULL) && 
			CalDataCtrl.hasBaseline() &&
			CalDataCtrl.getCount() >= 1)
			s_BnDlgLeftCtrlAcq=true;

		s_BnDlgLeftCtrlDel=false;
		if ((theApp.pSpecControl != NULL) && 
			CalDataCtrl.getCount() >= 1)
			s_BnDlgLeftCtrlDel=true;

		s_BnDlgLeftCtrlLoad=true;

		s_BnDlgLeftCtrlSave=false;
		if ((theApp.pSpecControl != NULL) && 
			CalDataCtrl.hasBaseline() &&
			CalDataCtrl.getCount() >= 2 &&
			! CalDataCtrl.getSubstanceName().IsEmpty() &&
			CalDataCtrl.GetCalWavelength() >= theApp.pSpecControl->getMinW() &&
			CalDataCtrl.GetCalWavelength() <= theApp.pSpecControl->getMaxW()
			) 
			s_BnDlgLeftCtrlSave=true;

		// Can't add any data beyond this point unless we are initialized ...
		if (theApp.pSpecControl == NULL)
		{
			interlock=false;
			theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... released interlock"));

			// unlock
			thread = GetCurrentThreadId();
			xval.Format(_T(__FUNCTION__ " trying to release lock ... [1:%lx; %d]"),thread,interlock?1:0);
			theApp.ChangeDebugText(2,xval.GetString());

			if (pLock->Unlock()) {
				theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... succeeded"));
			} else {
				theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... lock call failed"));
			}

			return;
		}

		// Set substance name if one is already defined
		if (selector & POP_SUBNAME)
			c_substanceName.SetWindowTextA(CalDataCtrl.getSubstanceName().GetString());

		// Populate spectrometer choices
		if (selector & POP_SPECNAME)
		{	
			c_specName.ResetContent();
			// c_specName.AddString(_T("Any"));
			for (ii=0; ii<theApp.pSpecControl->GetSpecCount(); ii++)
			{
				uval.Format(_T("%s (%s)"),
			 		theApp.pSpecControl->GetSpecName(ii).GetString(),
					theApp.pSpecControl->GetSpecSerial(ii).GetString());
				tval=CSpecNameConvert::getWilsonSerial(uval);
				c_specName.AddString(tval.GetString());
			}
			// c_specName.SetCurSel(0);
			// c_specName.SetWindowTextA(theApp.pSpecControl->GetSpecName(theApp.pSpecControl->GetCurSpec()));
			// @@@ need to deal with sparse lists ... 
			c_specName.SetCurSel(theApp.pSpecControl->GetCurSpec());
		}
		
		if (selector & POP_ITIME)
		{
			tval.Format(_T("%g"),CalDataCtrl.getIntegrationTime());
			c_integrationTime.SetWindowTextA(tval);
		}

		if (selector & POP_AVG)
		{
			tval.Format(_T("%d"),CalDataCtrl.getSpectraToAverage());
			c_scansToAverage.SetWindowTextA(tval);
		}

		if (selector & POP_BOXW)
		{
			tval.Format(_T("%d"),CalDataCtrl.getBoxcarWidth());
			c_boxcarWidth.SetWindowTextA(tval);
		}

		if (selector & (POP_LISTDATA|POP_LISTHDR))
		{
			CalDataCtrl.SetupCalCurveList(&c_listControl,((selector&POP_LISTHDR)==POP_LISTHDR));
		}

		if (selector & POP_WAVELEN)
		{
			tval.Format(_T("%g"),CalDataCtrl.GetCalWavelength());
			c_wavelenSelect.SetWindowTextA(tval);
		}

		if (selector & POP_BESTFIT)
		{
			CalDataCtrl.CalcBestFit();
			m_textFormula=CalDataCtrl.getFitInfo(CCalibrationCurve::FIT_FORMULA);
			m_textRSquared=CalDataCtrl.getFitInfo(CCalibrationCurve::FIT_RSQUARED);	
		}

		tval.Format(_T("%.1f"),CalDataCtrl.getErrLow());
		c_errLow.SetWindowTextA(tval);
		tval.Format(_T("%.1f"),CalDataCtrl.getErrHigh());
		c_errHigh.SetWindowTextA(tval);

		tval.Format(_T("%.1f"),CalDataCtrl.getWarnLow());
		c_warnLow.SetWindowTextA(tval);
		tval.Format(_T("%.1f"),CalDataCtrl.getWarnHigh());
		c_warnHigh.SetWindowTextA(tval);

		UpdateData(FALSE);

		interlock=false;
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... released interlock"));

	}
	else 
	{
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... skipped because of interlock"));
	}


	// unlock
	thread = GetCurrentThreadId();
	xval.Format(_T(__FUNCTION__ " trying to release lock ... [2:%lx; %d]"),thread,interlock?1:0);
	theApp.ChangeDebugText(2,xval.GetString());
	if (pLock->Unlock()) {
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... succeeded"));
	} else {
		theApp.ChangeDebugText(2,_T(__FUNCTION__ " ... lock call failed"));
	}

}

void CDlgBarControl::saveState() 
{
	c_substanceName.GetWindowTextA(t_substanceName);
	c_specName.GetWindowTextA(t_specName);
	c_integrationTime.GetWindowTextA(t_integrationTime);
	c_scansToAverage.GetWindowTextA(t_scansToAverage);
	c_boxcarWidth.GetWindowTextA(t_boxcarWidth);
	// c_concAVal.GetWindowTextA(t_concAVal);
	c_wavelenSelect.GetWindowTextA(t_wavelenSelect);

	t_numSpecs=CalDataCtrl.getCount();

}

bool CDlgBarControl::isStateChanged() 
{
	CString tval;

	c_substanceName.GetWindowTextA(tval);
	if (tval.Compare(t_substanceName))
		return true;
		
	c_specName.GetWindowTextA(tval);
	if (tval.Compare(t_specName))
		return true;

	c_integrationTime.GetWindowTextA(tval);
	if (tval.Compare(t_integrationTime))
		return true;

	c_scansToAverage.GetWindowTextA(tval);
	if (tval.Compare(t_scansToAverage))
		return true;

	c_boxcarWidth.GetWindowTextA(tval);
	if (tval.Compare(t_boxcarWidth))
		return true;

	// c_concAVal.GetWindowTextA(tval);
	// if (tval.Compare(t_concAVal))
	// 	return true;

	c_wavelenSelect.GetWindowTextA(tval);
	if (tval.Compare(t_wavelenSelect))
		return true;

	if (t_numSpecs != CalDataCtrl.getCount())
		return true;

	// nothing changed
	return false;

}

bool CDlgBarControl::reset(bool force)
{
	bool changes=false;
	int res;

	// Make sure we saved the calibration curve
	if ((s_BnDlgLeftCtrlSave) && (isStateChanged()))
	{
		changes=true;
		while (changes) {
			res=MessageBoxA(
				_T("You have unsaved modifications to the calibration curve!\nWould you like to save now?"), 
				_T("Calibration Not Saved"), 
				MB_YESNOCANCEL | MB_ICONEXCLAMATION);
			switch (res) {
				case IDYES:
					OnBnClickedDlgLeftCtrlSave();
					changes = isStateChanged();
					break;
				case IDNO:
					changes=false;
					break;
				default:
					return false;
			}
		}
	}

	if ((s_BnDlgLeftCtrlSave) && (changes) && (! force))
		return false;

	// Clear calibration curve
	CCalibrationCurve tmp; 
	CalDataCtrl=tmp; 

	// RePopulate dialogbar
	Populate();

	// Redraw view ...
	AfxGetMainWnd()->Invalidate();

	saveState();
	return true;
}

void CDlgBarControl::OnBnClickedDlgLeftCtrlAcqAccelF1() { OnBnClickedDlgLeftCtrlAcqAccel(ID_DLGLFT_CTRL_F1); }
void CDlgBarControl::OnBnClickedDlgLeftCtrlAcqAccelF2() { OnBnClickedDlgLeftCtrlAcqAccel(ID_DLGLFT_CTRL_F2); }

void CDlgBarControl::OnBnClickedDlgLeftCtrlAcqAccel(long selector)
{
	if ((selector == ID_DLGLFT_CTRL_F2) && (s_BnDlgLeftCtrlAcq))
	{
		OnBnClickedDlgLeftCtrlAcq();
		return;
	}
	if ((selector == ID_DLGLFT_CTRL_F1) && (s_BnDlgLeftCtrlAcq0))
	{
		OnBnClickedDlgLeftCtrlAcq0();
	}

}

void CDlgBarControl::OnBnUpdatedDlgLeftCtrlAcq0(CCmdUI* pCmdUI)
{
   pCmdUI->Enable( s_BnDlgLeftCtrlAcq0 );
}

void CDlgBarControl::OnBnClickedDlgLeftCtrlAcq0()
{
	// CString tval;
	// double itime;
	// long avg,boxw;
	// double conc;
	// double wavln;


	// UpdateData(TRUE);

	/*
	c_integrationTime.GetWindowTextA(tval);
	itime=atof(tval.GetString());

	c_scansToAverage.GetWindowTextA(tval);
	avg=atol(tval.GetString());
	if (avg<1)
		avg=1;

	c_boxcarWidth.GetWindowTextA(tval);
	boxw=atol(tval.GetString());
	if (boxw<0)
		boxw=0;

	conc=0;
	*/

	if (CalDataCtrl.getCount() >= MAX_CAL_SPEC)
	{
		MessageBox(_T("Maximum number of calibration spectra exceeded!"));
		return;
	}


	// Transfer values back to Calibration curve data ... 
	Readback();

	// Acquire baseline spectrum ... only if safe ... 
	if ((CalDataCtrl.getCount() == 0) ||
		((CalDataCtrl.getCount() > 0) &&
		 (MessageBox(_T("Are you sure you want to replace\nexisting baseline?"),
		_T("Warning"),
		MB_OKCANCEL | MB_ICONSTOP) == IDOK)))
	{
		theApp.ChangeStatusText(_T("Acquiring Spectrum ... Please Wait ..."));
		// theApp.pSpecControl->AcquireCalSpec(0,conc,itime,avg,boxw);
		CalDataCtrl.acquire(
			0 /* force insertion as first spectrum */,
			0 /* force concentration to be 0 ... it's a baseline */,
			CalDataCtrl.getIntegrationTime() /* @@@ this works because Readback() sets it up */,
			CalDataCtrl.getSpectraToAverage(),
			CalDataCtrl.getBoxcarWidth());
		theApp.ChangeStatusText(_T("Finished Acquiring Spectrum"));

		// RePopulate dialogbar
		Populate();

		// Redraw view ...
		AfxGetMainWnd()->Invalidate();

		// Mark calibration as modified
	}

}

void CDlgBarControl::OnBnUpdatedDlgLeftCtrlAcq(CCmdUI* pCmdUI)
{
   pCmdUI->Enable( s_BnDlgLeftCtrlAcq );
}

void CDlgBarControl::OnBnClickedDlgLeftCtrlAcq()
{
	CString tval;
	double conc;

	c_concAVal.GetWindowTextA(tval);
	conc=atof(tval.GetString());

	conc=max(conc,0);
	if (conc == 0)
	{
		MessageBox(_T("Cannot acquire spectrum for ZERO concentration!\nPlease use Baseline for this purpose"),_T("ERROR"));
	} 
	else 
	{
		Readback();

		theApp.ChangeStatusText(_T("Acquiring Spectrum ... Please Wait ..."));
		CalDataCtrl.acquire(
			  -1 /* append spectrum */,
			conc /* selected concentration */,
			CalDataCtrl.getIntegrationTime() /* @@@ this relies on input fields being disabled! */,
			CalDataCtrl.getSpectraToAverage(),
			CalDataCtrl.getBoxcarWidth());
		theApp.ChangeStatusText(_T("Finished Acquiring Spectrum"));

		Populate();

		AfxGetMainWnd()->Invalidate();

	}
}

void CDlgBarControl::OnBnUpdatedDlgLeftCtrlDel(CCmdUI* pCmdUI)
{
   pCmdUI->Enable( s_BnDlgLeftCtrlDel );
}

void CDlgBarControl::OnBnClickedDlgLeftCtrlDel()
{
	UINT nsel;
	int csel;
	int dspecs[MAX_CAL_SPEC],dcnt=0,ii;

	POSITION pos;
	CString tval;

	tval=_T("Delete ...");


	nsel = c_listControl.GetSelectedCount();
	tval.AppendFormat(_T(" nsel=%d"),nsel);

	tval.Append(_T("items:={"));
	pos = c_listControl.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		csel=c_listControl.GetNextSelectedItem(pos);
		dspecs[dcnt++]=csel;
		tval.AppendFormat(_T("%d,"),csel);
	}
	tval.Append(_T("}"));

	for (ii=dcnt-1; ii>-1; ii--)
		CalDataCtrl.remove(dspecs[ii]);

	theApp.ChangeStatusText(tval.GetString());

	Populate();

	AfxGetMainWnd()->Invalidate();

}

void CDlgBarControl::OnBnUpdatedDlgLeftCtrlSave(CCmdUI* pCmdUI)
{
   pCmdUI->Enable( s_BnDlgLeftCtrlSave );
}

void CDlgBarControl::OnBnClickedDlgLeftCtrlSave()
{
	// Make sure we don't run if the button is disabled ... 
	if (! s_BnDlgLeftCtrlSave)
		return;

	// Make sure calibration curve data are up to date
	Readback();

	theApp.ChangeStatusText(_T("Save a *new* calibration file"));
	// if (theApp.pSpecControl->Calibration_Save())
	if (CalDataCtrl.Save())
	{
		saveState();
	}
}

BOOL CDlgBarControl::InitDialog(void)
{
	// UpdateData(FALSE);
	// SetWindowText("Wzd Dialog Bar");
	return TRUE;
}

void CDlgBarControl::Hide() { 
	((CMainFrame *)AfxGetMainWnd())->ShowControlBar(this,FALSE,FALSE); 
}

void CDlgBarControl::OnBnClickedDlgLeftCtrlLoad()
{
	// We are destroying an old calibration curve here ... 
	if (reset()) {

		// TODO: Add your control notification handler code here
		theApp.ChangeStatusText(_T("Populate calibration info with pre-existing data"));
		// theApp.pSpecControl->Calibration_Load();
		CalDataCtrl.Load();

		Populate();

		// A freshly loaded calibration curve has no changes ... 
		saveState();

		AfxGetMainWnd()->Invalidate();
	}
}

void CDlgBarControl::OnBnClickedDlgLeftCtrlReset()
{
	// Call the generic reset function that's also used for mode
	// switching
	reset();

}

void CDlgBarControl::OnEnChangeCombo7()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	double wavln;
	CString tval;

	// This *will* break unless you can find a solution for threading ... 
	//
	//   Readback();
	//
	// Instead *only* read back the thing that changed ...
	c_wavelenSelect.GetWindowTextA(tval);
	wavln=atof(tval.GetString());
	if ((wavln > theApp.pSpecControl->getMinW()) &&
		(wavln < theApp.pSpecControl->getMaxW()))
	{
		CalDataCtrl.SetCalWavelength(wavln);
		CalDataCtrl.CalcBestFit();

		// And only update the thing we recalculated
		Populate(POP_BESTFIT|POP_LISTDATA);
		AfxGetMainWnd()->Invalidate();
	}
}

void CDlgBarControl::OnEnKillfocusEditSubname()
{
	// TODO: Add your control notification handler code here
	Readback();
}

void CDlgBarControl::OnEnChangeEditSubname()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	Readback();

	s_BnDlgLeftCtrlSave=false;
	if ((theApp.pSpecControl != NULL) && 
		CalDataCtrl.hasBaseline() &&
		CalDataCtrl.getCount() >= 2 &&
		! CalDataCtrl.getSubstanceName().IsEmpty() &&
		CalDataCtrl.GetCalWavelength() >= theApp.pSpecControl->getMinW() &&
		CalDataCtrl.GetCalWavelength() <= theApp.pSpecControl->getMaxW()
		) 
		s_BnDlgLeftCtrlSave=true;

}
