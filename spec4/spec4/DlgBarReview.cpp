// DlgBarReview.cpp : implementation file
//

#include "stdafx.h"
#include "spec4.h"
#include "DlgBarReview.h"

#include <algorithm>
#include <vector>


// CDlgBarReview dialog

IMPLEMENT_DYNAMIC(CDlgBarReview, CDialogBar)

// Define list control arrays ...
#define LIST_NENTRIES	7

#define LIST_IDX_FNAME	0
#define LIST_IDX_GPSLOC	1
#define LIST_IDX_PIPEKM	2
#define LIST_IDX_TSTAMP	3
#define LIST_IDX_CONC	4
#define LIST_IDX_DILUTE	5
#define LIST_IDX_COMMT	6

//! Array defining column names
static char *list_colnames[LIST_NENTRIES] = {
/* 0 */	"File", 
		"GPS",	
		"km",	
		"Time / Date",
/* 4 */	"Conc",
		"Dil.",
		"Comment",
};

//! Array defining the (default) sort order (true=ascending)
bool list_order[LIST_NENTRIES] = {
/* 0 */	false,
		false, 
		true, 
		false, 
/* 4 */	false, 
		false, 
		false,
};

//! Array containing pointer to ascending sort function for each column
//! or null if sorting is not supported
static bool (*fptrA[LIST_NENTRIES])(CDSpectrumData&, CDSpectrumData&) = {
/* 0 */	&(CSpectrometer::RevSpec_SortAFileName),
		NULL,
		&(CSpectrometer::RevSpec_SortAPipeKm),
		&(CSpectrometer::RevSpec_SortATimeStamp),
/* 4 */	&(CSpectrometer::RevSpec_SortAConc),
		&(CSpectrometer::RevSpec_SortADil),
		NULL,
};

//! Array containing pointer to descending sort function for each column
//! or null if sorting is not supported
static bool (*fptrD[LIST_NENTRIES])(CDSpectrumData&, CDSpectrumData&) = {
/* 0 */	&(CSpectrometer::RevSpec_SortDFileName),
		NULL,
		&(CSpectrometer::RevSpec_SortDPipeKm),
		&(CSpectrometer::RevSpec_SortDTimeStamp),
/* 4 */	&(CSpectrometer::RevSpec_SortDConc),
		&(CSpectrometer::RevSpec_SortDDil),
		NULL,
};



CDlgBarReview::CDlgBarReview()
{

}

CDlgBarReview::~CDlgBarReview()
{
}

void CDlgBarReview::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DATALIST, c_displayList);

}


BEGIN_MESSAGE_MAP(CDlgBarReview, CDialogBar)
	// according to http://support.microsoft.com/kb/185672
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )

	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgBarReview::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_REM, &CDlgBarReview::OnBnClickedBtnRem)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDlgBarReview::OnHdnItemclickDatalist)
	ON_BN_CLICKED(IDC_HIGHLIGHT, &CDlgBarReview::OnBnClickedHighlight)
END_MESSAGE_MAP()

LONG CDlgBarReview::OnInitDialog ( UINT wParam, LONG lParam)
{
	
	BOOL bRet = HandleInitDialog(wParam, lParam);

	if (!UpdateData(FALSE))
	{
	   TRACE0("Warning: UpdateData failed during dialog init.\n");
	}

	Populate();

	return bRet;
}

BOOL CDlgBarReview::InitDialog(void)
{
	// UpdateData(FALSE);
	// SetWindowText("Wzd Dialog Bar");
	Populate(true);
	return TRUE;
}


// CDlgBarReview message handlers

void CDlgBarReview::Populate(bool rebuild) {
	CRect rect;
	LVITEM lvi;
	CString lvs;
	int ii;
	static int lastsize=0;
	int cursize;

	if (rebuild)
	{
		// Make sure we select the "whole" line ... this still does not
		// cover the space not taken up by subitems but it should be 
		// good enough for starters
		c_displayList.SetExtendedStyle(c_displayList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
		while (c_displayList.DeleteColumn(0))
			;

		// preset column widths ... we will still override this later but in case anything goes
		// wrong, at least we have some sort of default.
		c_displayList.GetClientRect(&rect);
		int cw=rect.Width()/4;
		for (ii=0; ii<LIST_NENTRIES; ii++)
			c_displayList.InsertColumn(ii,list_colnames[ii],0,cw);
	}

	if (theApp.pSpecControl == NULL)
		return;

	cursize = theApp.pSpecControl->RevSpecs.size();

	// This is a hack that will likely have rather bad consequences regarding
	// selections etc ... but for now it's easier to just recreate the list
	if (rebuild || (cursize!=lastsize)) {
		c_displayList.DeleteAllItems();
	}
	for (ii=0; ii<(int)(theApp.pSpecControl->RevSpecs.size()); ii++) 
	{
		lvi.mask = LVIF_TEXT;
		lvi.iItem = ii;
		// This does not work ... ?!? On its own ... may work with setitemstate ... 
		// lvi.state = theApp.pSpecControl->RevSpecs[ii].highlight?LVIS_SELECTED:0;
		// c_displayList.SetItemState(ii,&lvi);
		c_displayList.SetItemState(
			ii,
			theApp.pSpecControl->RevSpecs[ii].highlight?LVIS_SELECTED:0,
			LVIS_SELECTED);

		for (lvi.iSubItem=0; lvi.iSubItem<LIST_NENTRIES; lvi.iSubItem++)
		{
			switch (lvi.iSubItem) {
				case LIST_IDX_FNAME: // main entry: file name
					lvs.Format(_T("%s"),theApp.pSpecControl->RevSpecs[ii].specFileName);
					break;
				case LIST_IDX_GPSLOC: // main entry: GPS
					// lvs.Format(_T("%s"),_T("GPS"));
					lvs.Format(_T("%s"),theApp.pSpecControl->RevSpecs[ii].specDiff.formatLoc());
					// theApp.pSpecControl->RevSpecs[ii].specDiff.
					break;
				case LIST_IDX_PIPEKM: // main entry: km
					lvs.Format(_T("%g"),theApp.pSpecControl->RevSpecs[ii].specDiff.getPipeKm());
					break;
				case LIST_IDX_TSTAMP: // main entry: time / date
					lvs.Format(_T("%s"),theApp.pSpecControl->RevSpecs[ii].specDiff.formatTimeStamp());
					// theApp.pSpecControl->RevSpecs[ii].specDiff.
					break;
				case LIST_IDX_CONC: // main entry: concentration
					lvs.Format(_T("%g"),theApp.pSpecControl->RevSpecs[ii].specDiff.getConc());
					break;
				case LIST_IDX_DILUTE: // main entry: dilution
					lvs.Format(_T("%g"),theApp.pSpecControl->RevSpecs[ii].specDiff.getDilution());
					break;
				case LIST_IDX_COMMT: // main entry: comment ... 
					// lvs.Format(_T("%s"),_T("comm"));
					lvs.Format(_T("%s"),theApp.pSpecControl->RevSpecs[ii].specDiff.getComment());
					// theApp.pSpecControl->RevSpecs[ii].specDiff.
					break;
				default: // make sure we don't populate unknown columns
					lvs.Empty();
					break;
			}
			lvi.pszText=(LPTSTR)(LPCTSTR)(lvs);
			switch (lvi.iSubItem) {
				case 0:
					if (lastsize != cursize)
						c_displayList.InsertItem(&lvi);
					else
						c_displayList.SetItem(&lvi);
					break;
				default:
					c_displayList.SetItem(&lvi);
					break;
			}
		}
	}

	// If we just rebuilt / repopulated the list, let's auto-adjust headers ...
	// ... following http://www.codeguru.com/forum/archive/index.php/t-42899.html
	// we should set the width to the maximum of header or content 
	if (rebuild || (lastsize==0 && cursize>0))
	{
		int wc,wh;

		c_displayList.SetRedraw(FALSE);
		for (ii=0; ii<LIST_NENTRIES; ii++) {
			c_displayList.SetColumnWidth(ii,LVSCW_AUTOSIZE);
			wc=c_displayList.GetColumnWidth(ii);

			c_displayList.SetColumnWidth(ii,LVSCW_AUTOSIZE_USEHEADER);
			wh=c_displayList.GetColumnWidth(ii);

			c_displayList.SetColumnWidth(ii,max(wh,wc));
		}
		c_displayList.SetRedraw(TRUE);
	}

	lastsize=cursize;

}

void CDlgBarReview::OnBnClickedBtnAdd()
{
	// TODO: Add your control notification handler code here
	// MessageBox(_T("Add data"));
	// DOC ... OnViewSelectdatafiles()
	((Cspec4Doc *)(((CMainFrame *)AfxGetMainWnd())->GetActiveView()->GetDocument()))->AddDataFiles(false);
	Populate();
	AfxGetMainWnd()->Invalidate();
}

void CDlgBarReview::OnBnClickedBtnRem()
{
	// TODO: Add your control notification handler code here
	// MessageBox(_T("Remove data"));


	UINT nsel;
	int csel;
	int dcnt=0,ii;
	std::vector<int> dlist;

	POSITION pos;
	CString tval;

	tval=_T("Delete ...");

	nsel = c_displayList.GetSelectedCount();
	tval.AppendFormat(_T(" nsel=%d"),nsel);

	tval.Append(_T("items:={"));
	pos = c_displayList.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		csel=c_displayList.GetNextSelectedItem(pos);
		dlist.push_back(csel);
		tval.AppendFormat(_T("%d,"),csel);
	}
	tval.Append(_T("}"));

	for (ii=dlist.size()-1; ii>=0; ii--)
		theApp.pSpecControl->RevSpecs.erase(theApp.pSpecControl->RevSpecs.begin()+dlist[ii]);

	theApp.ChangeStatusText(tval.GetString());

	Populate();

	AfxGetMainWnd()->Invalidate();

}

void CDlgBarReview::OnHdnItemclickDatalist(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString tval;

	//! Pointer to the current sort function (populated from arrays below)
	static bool (*fptr)(CDSpectrumData&, CDSpectrumData&);

	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	// tval.Format(_T("Clicked column %d"),phdr->iItem);
	// MessageBox(tval.GetString());

	if (phdr->iItem < LIST_NENTRIES)
	{
		fptr = (list_order[phdr->iItem])?(fptrA[phdr->iItem]):(fptrD[phdr->iItem]);

		if (fptr != NULL) {
			std::sort(
				theApp.pSpecControl->RevSpecs.begin(),
				theApp.pSpecControl->RevSpecs.end(),
				fptr);

			list_order[phdr->iItem] = !list_order[phdr->iItem];

			Populate();
		}
	}


	*pResult = 0;
}


void CDlgBarReview::OnBnClickedHighlight()
{

	int dcnt=0,ii;

	bool oldval,newval,redraw=false;
	CString tval;

	tval=_T("Highlight ...");
	theApp.ChangeStatusText(tval.GetString());

	dcnt=c_displayList.GetItemCount();
	for (ii=0; ii<dcnt; ii++) {
		oldval=theApp.pSpecControl->RevSpecs[ii].highlight;
		newval=(c_displayList.GetItemState(ii,LVIS_SELECTED)==LVIS_SELECTED);

		theApp.pSpecControl->RevSpecs[ii].highlight=newval;

		if (newval != oldval)
			redraw=true;
	}

	if (redraw)
		AfxGetMainWnd()->Invalidate();
	c_displayList.SetFocus();
}

CSize CDlgBarReview::CalcFixedLayout(BOOL bStretch,BOOL bHorz)
{
	// http://support.microsoft.com/kb/143255

	// we ignore the booleans because we know that this dialog can only be horizontal 
	// and docked ... so what do we return?
	RECT twrect,srect;
	CSize mysize;

	AfxGetApp()->m_pMainWnd->GetWindowRect(&twrect);

	mysize.SetSize(twrect.right-twrect.left, m_sizeDefault.cy);

	// @@@ I really don't like this but I can't see anywhere else to put this ...
	// ... I also have no idea *why* this works but apparently it does ;)
	c_displayList.GetWindowRect(&srect);
	srect.right=twrect.right-16;

	ScreenToClient(&srect);
	c_displayList.MoveWindow(&srect);

	return mysize;
}
