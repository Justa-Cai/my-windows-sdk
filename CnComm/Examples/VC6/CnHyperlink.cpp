// CnHyperlink.cpp : implementation file
//

#include "stdafx.h"
#include "CnHyperlink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CnHyperlink

CnHyperlink::CnHyperlink()
{
	bUnderLine_ = false;
	bVisited_ = false;
	bCmdMode_ = false;
	bUseParentFont_ = true;
	UnderLineMode_ = ULM_OVER;//ULM_ALWAYS;
	uTextAlign_ = 0;

	clrText_[TCM_OVER] = RGB(0, 0, 255);
	clrText_[TCM_NORMAL] = RGB(0, 0, 0);
	clrText_[TCM_VISITED] = RGB(128, 128, 128);

	hHandCursor_ = hCursor_ = LoadCursor(NULL, MAKEINTRESOURCE(32649));
	ASSERT(hHandCursor_);
}

CnHyperlink::~CnHyperlink()
{
	DestroyCursor(hHandCursor_);
	Font_.DeleteObject();
}

BEGIN_MESSAGE_MAP(CnHyperlink, CStatic)
	//{{AFX_MSG_MAP(CnHyperlink)
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CnHyperlink message handlers

void CnHyperlink::PreSubclassWindow() 
{
	ModifyStyle(0, SS_OWNERDRAW | SS_NOTIFY);

	if (bUseParentFont_)
	{
		CWnd* pParent = GetParent();

		if (pParent)
		{
			LOGFONT lf;
			pParent->GetFont()->GetLogFont(&lf);
			lf.lfUnderline = UnderLineMode_ == ULM_ALWAYS ? TRUE : FALSE;
			Font_.DeleteObject();
			Font_.CreateFontIndirect(&lf);
		}
	}

	CStatic::PreSubclassWindow();
}

void CnHyperlink::OnMouseMove(UINT nFlags, CPoint point) 
{
	::SetCursor(hCursor_);

	if (GetCapture() == this)
	{
		CRect rect;
        GetClientRect(rect);

		if (UnderLineMode_ == ULM_OVER)
			SetUnderLine(false);

        if (!rect.PtInRect(point))
		{
			ReleaseCapture();
			Invalidate(TRUE);
		}
	}
	else
	{
		if (UnderLineMode_ == ULM_OVER)
			SetUnderLine(true);

		SetCapture();
		Invalidate(TRUE);
	}
	
	CStatic::OnMouseMove(nFlags, point);
}

void CnHyperlink::OnClicked() 
{
	bVisited_ = true;

	CString str;
	
	if (strUrl_.IsEmpty())
		GetWindowText(str);
	else
		str = strUrl_;

	if (bCmdMode_)
		_tsystem(str);
	else
		ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOW);

	Invalidate();
}

void CnHyperlink::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CString str;
	GetWindowText(str);

	CRect rc;
	GetClientRect(&rc);

	dc.SetBkMode(TRANSPARENT);

	dc.SetTextColor(
		clrText_[GetCapture() == this ? TCM_OVER : bVisited_ ? TCM_VISITED : TCM_NORMAL]
		);

	HGDIOBJ hOldFont = NULL;

	hOldFont = dc.SelectObject(Font_);

	if (!uTextAlign_)
	{
		DWORD dwStyle = GetStyle();
		UINT uFormat;

		if (dwStyle & SS_CENTER)
			uFormat |= DT_CENTER;

		if (dwStyle & SS_LEFT)
			uFormat |= DT_LEFT;

		if (dwStyle & SS_RIGHT)
			uFormat |= DT_RIGHT;
			
		dc.DrawText(str, &rc, uFormat);
	}
	else
		dc.DrawText(str, &rc, uTextAlign_);

	if (hOldFont)
		dc.SelectObject(hOldFont);
}

BOOL CnHyperlink::OnEraseBkgnd(CDC* pDC) 
{
	HBRUSH hBrush;
	CRect rc;
	GetClientRect(&rc);

	CWnd * pParent = GetParent();

	if (pParent)
	{
		hBrush = (HBRUSH)GetParent()->SendMessage(
			WM_CTLCOLORSTATIC, 
			(WPARAM)pDC->m_hDC, 
			(LPARAM)m_hWnd
			);

		FillRect(pDC->m_hDC, &rc, hBrush);
	}
	
	return TRUE;
}

void CnHyperlink::SetUnderLine(bool bUnderLine)
{
	if (bUnderLine_ != bUnderLine)
	{
		LOGFONT lf;

		if (bUseParentFont_)
		{
			CWnd* pParent = GetParent();

			if (pParent)
				pParent->GetFont()->GetLogFont(&lf);
			else
				Font_.GetLogFont(&lf);
		}
		else
			Font_.GetLogFont(&lf);
		
		
		lf.lfUnderline = bUnderLine;
		
		if ((HFONT)Font_)
			Font_.DeleteObject();
		Font_.CreateFontIndirect(&lf);

		bUnderLine_ = bUnderLine;
	}
}


