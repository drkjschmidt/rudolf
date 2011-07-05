// WavlenCursor.cpp : implementation file
//

#include "stdafx.h"
#include "spec4.h"
#include "WavlenCursor.h"


// CWavlenCursor dialog

IMPLEMENT_DYNAMIC(CWavlenCursor, CDialog)

CWavlenCursor::CWavlenCursor(CWnd* pParent /*=NULL*/)
: CDialog(CWavlenCursor::IDD, pParent)
, cur_wavelen(0)
, cur_intensity(0)
, cur_wv_min(0)
, cur_wv_index(0)
, cur_wv_max(4096)
, cur_nm_min(0)
, cur_nm_index(0)
, cur_nm_max(0)
{

}

CWavlenCursor::~CWavlenCursor()
{
}

void CWavlenCursor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUR_NAME, curName);
	DDX_Control(pDX, IDC_CUR_WAVLEN, curWavelen);
	DDX_Control(pDX, IDC_CUR_COUNTS, curIntensity);
	DDX_Control(pDX, IDC_SCROLLBAR1, curHScroll);
	DDX_Control(pDX, IDC_SCROLLBAR2, curVScroll);
}


BEGIN_MESSAGE_MAP(CWavlenCursor, CDialog)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CWavlenCursor message handlers

void CWavlenCursor::OnBnClickedNameUp()
{
	// TODO: Add your control notification handler code here
}

void CWavlenCursor::OnBnClickedNameDn()
{
	// TODO: Add your control notification handler code here
}

void CWavlenCursor::OnBnClickedWavlenDn()
{
	// TODO: Add your control notification handler code here
	cur_wv_index--;
	Handler();
}

void CWavlenCursor::OnBnClickedWavlenUp()
{
	// TODO: Add your control notification handler code here
	cur_wv_index++;
	Handler();
}


BOOL CWavlenCursor::OnInitDialog()
{
	CDialog::OnInitDialog();

	DoubleArray wavelens;

	cur_wv_index=0;
	cur_nm_index=0;

	switch (theApp.getRunmode()) {
		case RUNMODE_REVIEW:
			// If we are in review mode we might not have a spectrometer
			// so use the wavelengths from the spectra ... for now
			// assume they are all the same because I am not sure
			// how to deal with spectra that have different wavelengths ...
			//
			// Only initialize if we in fact have data ...
			//
			cur_wv_min=0;
			cur_wv_max=0;
			if (theApp.pSpecControl->RevSpecs.size() > 0)
			{
				wavelens=theApp.pSpecControl->RevSpecs[0].specDiff.getWavelengths();
				cur_wv_max=wavelens.getLength()-1;
			}
			break;
		case RUNMODE_CONFIGURATION:
		case RUNMODE_ACQUISITION:
		default:
			// If we have a spectrometer use the wavelengths from the spectrometer
			theApp.pSpecControl->GetWavelengths(&wavelens);
			cur_wv_min=0;
			cur_wv_max=wavelens.getLength()-1;
			break;
	}

	// @@@ // this should probably start at the current wavelength?
	curHScroll.SetScrollRange(cur_wv_min,cur_wv_max);

	Handler();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWavlenCursor::Handler()
{
	CString tval;
	DoubleArray wavelens;
	DoubleArray intensities;


	cur_nm_min=0;
	cur_nm_max=0;
	switch (theApp.getRunmode()) {
		case RUNMODE_REVIEW:
			// I don't see a good way of handling NO review spectra so just quit here
			// @@@ should we UNdraw the cursor somehow?
			if (theApp.pSpecControl->RevSpecs.size() <= 0)
				return;

			wavelens=theApp.pSpecControl->RevSpecs[0].specDiff.getWavelengths();

			cur_wv_index=max(cur_wv_index,cur_wv_min);
			cur_wv_index=min(cur_wv_index,cur_wv_max);
			cur_wavelen=(wavelens.getDoubleValues())[cur_wv_index];
			cur_nm_max=theApp.pSpecControl->RevSpecs.size()-1;

			if (cur_nm_max >= 0) {
				cur_nm_index=max(cur_nm_index,cur_nm_min);
				cur_nm_index=min(cur_nm_index,cur_nm_max);

				tval=theApp.pSpecControl->RevSpecs[cur_nm_index].specFileName;
				curName.SetWindowTextA(tval);

				cur_wavelen=(theApp.pSpecControl->RevSpecs[cur_nm_index].specDiff.getWavelengths().getDoubleValues())[cur_wv_index];
				tval.Format(_T("%.1f"),cur_wavelen);
				curWavelen.SetWindowTextA(tval);

				cur_intensity=(theApp.pSpecControl->RevSpecs[cur_nm_index].specDiff.getIntensities().getDoubleValues())[cur_wv_index];
				tval.Format(_T("%.1f"),cur_intensity);
				curIntensity.SetWindowTextA(tval);
			}
			else {
				curName.SetWindowTextA("No Data");
				curWavelen.SetWindowTextA("--");
				curIntensity.SetWindowTextA("--");
			}

			break;
		case RUNMODE_CONFIGURATION:
			theApp.pSpecControl->GetWavelengths(&wavelens);

			cur_wv_index=max(cur_wv_index,cur_wv_min);
			cur_wv_index=min(cur_wv_index,cur_wv_max);
			cur_wavelen=(wavelens.getDoubleValues())[cur_wv_index];
			cur_nm_max=theApp.pSpecControl->RevSpecs.size()-1;

			cur_wavelen=(wavelens.getDoubleValues())[cur_wv_index];

			cur_nm_max=((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl.getCount()-1;
			if (cur_nm_max >= 0) {
				cur_nm_index=max(cur_nm_index,cur_nm_min);
				cur_nm_index=min(cur_nm_index,cur_nm_max);

				tval.Format(
					_T("%d: %.1f nm"),
					cur_nm_index,
					(((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl.getCalConcArray())[cur_nm_index]);
				curName.SetWindowTextA(tval);

				cur_wavelen=(((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl.get(cur_nm_index)->getWavelengths().getDoubleValues())[cur_wv_index];
				tval.Format(_T("%.1f"),cur_wavelen);
				curWavelen.SetWindowTextA(tval);

				cur_intensity=(((CMainFrame *)AfxGetMainWnd())->m_DlgLeftCtrl.CalDataCtrl.get(cur_nm_index)->getIntensities().getDoubleValues())[cur_wv_index];
				tval.Format(_T("%.1f"),cur_intensity);
				curIntensity.SetWindowTextA(tval);
			}
			else {
				curName.SetWindowTextA("No Data");
				curWavelen.SetWindowTextA("--");
				curIntensity.SetWindowTextA("--");
			}

			break;
		case RUNMODE_ACQUISITION:
			theApp.pSpecControl->GetWavelengths(&wavelens);

			cur_wv_index=max(cur_wv_index,cur_wv_min);
			cur_wv_index=min(cur_wv_index,cur_wv_max);
			cur_wavelen=(wavelens.getDoubleValues())[cur_wv_index];
			cur_nm_max=theApp.pSpecControl->RevSpecs.size()-1;

			cur_wavelen=(wavelens.getDoubleValues())[cur_wv_index];

			cur_nm_min=0;
			cur_nm_max=2;
			// @@@ // Hacked custom solution because no time to do better ...
			((CMainFrame *)AfxGetMainWnd())->m_DlgLeftAcq.getCursorInfo(&cur_nm_index,cur_wv_index,&tval,&cur_wavelen,&cur_intensity);

			if (cur_nm_index >= 0) {
				curName.SetWindowTextA(tval);

				tval.Format(_T("%.1f"),cur_wavelen);
				curWavelen.SetWindowTextA(tval);

				tval.Format(_T("%.1f"),cur_intensity);
				curIntensity.SetWindowTextA(tval);
			}
			else {
				curName.SetWindowTextA("No Data");
				curWavelen.SetWindowTextA("--");
				curIntensity.SetWindowTextA("--");
			}
			break;
		default:
			curName.SetWindowTextA("No Data");
			curWavelen.SetWindowTextA("--");
			curIntensity.SetWindowTextA("--");
			break;
	}

	// redraw ...
	theApp.pSpecControl->PaintOpt_ShowCursor(cur_wavelen);
	AfxGetMainWnd()->Invalidate();
}


void CWavlenCursor::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	switch (nSBCode)
	{
		case SB_LEFT:      // Scroll to far left.
			cur_wv_index = cur_wv_min;
			break;

		case SB_RIGHT:      // Scroll to far right.
			cur_wv_index = cur_wv_max;
			break;

		case SB_ENDSCROLL:   // End scroll.
			break;

		case SB_LINELEFT:      // Scroll left.
			cur_wv_index=(max(cur_wv_min,cur_wv_index-1));
			break;

		case SB_LINERIGHT:   // Scroll right.
			cur_wv_index=(min(cur_wv_max,cur_wv_index+1));
			break;

		case SB_PAGELEFT:    // Scroll one page left.
			cur_wv_index=(max(cur_wv_min,cur_wv_index-50));
			break;

		case SB_PAGERIGHT:      // Scroll one page right
			cur_wv_index=(min(cur_wv_max,cur_wv_index+50));
			break;

		case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
			cur_wv_index = nPos;      // of the scroll box at the end of the drag operation.
			break;

		case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
			cur_wv_index = nPos;     // position that the scroll box has been dragged to.
			break;
	}
	curHScroll.SetScrollPos(cur_wv_index);
	Handler();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CWavlenCursor::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	switch (nSBCode)
	{
		case SB_TOP:      // Scroll to far top.
			cur_nm_index = cur_nm_min;
			break;

		case SB_BOTTOM:      // Scroll to far right.
			cur_nm_index = cur_nm_max;
			break;

		case SB_ENDSCROLL:   // End scroll.
			break;

		case SB_LINEUP:      // Scroll left.
			cur_nm_index=(max(cur_nm_min,cur_nm_index-1));
			break;

		case SB_LINEDOWN:   // Scroll right.
			cur_nm_index=(min(cur_nm_max,cur_nm_index+1));
			break;

		case SB_PAGEUP:    // Scroll one page left.
			cur_nm_index=(max(cur_nm_min,cur_nm_index-5));
			break;

		case SB_PAGEDOWN:      // Scroll one page right
			cur_nm_index=(min(cur_nm_max,cur_nm_index+5));
			break;

		case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
			cur_nm_index = nPos;      // of the scroll box at the end of the drag operation.
			break;

		case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
			cur_nm_index = nPos;     // position that the scroll box has been dragged to.
			break;
	}
	curVScroll.SetScrollPos(cur_nm_index);
	Handler();

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CWavlenCursor::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	theApp.pSpecControl->PaintOpt_HideCursor();
	AfxGetMainWnd()->Invalidate();
	CDialog::OnOK();
}
