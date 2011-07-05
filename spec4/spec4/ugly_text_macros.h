#pragma once

// Helper macro to print text with bottom left alignment within box
// The macro relies on yoff, y_ht, and xoff being set and will
// update yoff.	
#define TEXT_OUT_BL_C1(str,outline) \
	trect.left   = (int)(myrect->left +(xoff     )*xdpi/72.27)  ; \
	trect.right  = (int)(myrect->left +(xoff+x2ff)*xdpi/72.27)+1; \
	trect.top    = (int)(myrect->top  +(yoff     )*ydpi/72.27)  ; \
	trect.bottom = (int)(myrect->top  +(yoff+y_ht)*ydpi/72.27)+1; \
	\
	pDC->SetTextAlign(TA_BOTTOM|TA_LEFT); \
	pDC->TextOutA(trect.left,trect.bottom, str); \
	if (outline) pDC->FrameRect(&trect,&blackb);

// Helper macro to print text with top left alignment within box
// The macro relies on yoff, y_ht, and xoff being set and will
// update yoff.	
#define TEXT_OUT_TL_C1(str,outline) \
	trect.left   = (int)(myrect->left +(xoff     )*xdpi/72.27)  ; \
	trect.right  = (int)(myrect->left +(xoff+x2ff)*xdpi/72.27)+1; \
	trect.top    = (int)(myrect->top  +(yoff     )*ydpi/72.27)  ; \
	trect.bottom = (int)(myrect->top  +(yoff+y_ht)*ydpi/72.27)+1; \
	\
	pDC->SetTextAlign(TA_TOP|TA_LEFT); \
	pDC->TextOutA(trect.left,trect.top, str); \
	if (outline) pDC->FrameRect(&trect,&blackb);

// Helper macro to print MULTILINE text with top left alignment within box
// The macro relies on yoff, y_ht, and xoff being set and will
// update yoff.	
#define TEXT_DRW_TL_C2(str,outline) \
	trect.left   = (int)(myrect->left +(xoff+x2ff)*xdpi/72.27);   \
	trect.right  = (int)(myrect->right-(xoff     )*xdpi/72.27)+1; \
	trect.top    = (int)(myrect->top  +(yoff     )*ydpi/72.27);   \
	trect.bottom = (int)(myrect->top  +(yoff+y_ht)*ydpi/72.27)+1; \
	\
	pDC->DrawText(str,&trect,DT_TOP|DT_LEFT); \
	if (outline) pDC->FrameRect(&trect,&blackb);

	

// Helper macro to print text with bottom left alignment within box
// The macro relies on yoff, y_ht, and xoff being set and will
// update yoff.	
#define TEXT_OUT_BL_C2(str,outline) \
	trect.left   = (int)(myrect->left +(xoff+x2ff)*xdpi/72.27)  ; \
	trect.right  = (int)(myrect->right-(xoff     )*xdpi/72.27)+1; \
	trect.top    = (int)(myrect->top  +(yoff     )*ydpi/72.27)  ; \
	trect.bottom = (int)(myrect->top  +(yoff+y_ht)*ydpi/72.27)+1; \
	\
	pDC->SetTextAlign(TA_BOTTOM|TA_LEFT); \
	pDC->TextOutA(trect.left,trect.bottom, str); \
	if (outline) pDC->FrameRect(&trect,&blackb);

// Helper macro to print text with bottom left alignment within box
// The macro relies on yoff, y_ht, xoff, and x2ff being set and will
// update yoff.	
#define TEXT_OUT_CT(str,outline) \
	trect.left   = (int)(myrect->left +(xoff     )*xdpi/72.27)  ; \
	trect.right  = (int)(myrect->right-(xoff     )*xdpi/72.27)+1; \
	trect.top    = (int)(myrect->top  +(yoff     )*ydpi/72.27)  ; \
	trect.bottom = (int)(myrect->top  +(yoff+y_ht)*ydpi/72.27)+1; \
	\
	pDC->SetTextAlign(TA_BOTTOM|TA_CENTER); \
	pDC->TextOutA((trect.left+trect.right)/2,trect.bottom, str); \
	if (outline) pDC->FrameRect(&trect,&blackb);

