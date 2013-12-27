#if !defined(AFX_CNHYPERLINK_H__B798794F_763C_46F4_8E6E_7B9414A769AA__INCLUDED_)
#define AFX_CNHYPERLINK_H__B798794F_763C_46F4_8E6E_7B9414A769AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CnHyperlink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CnHyperlink window

class CnHyperlink : public CStatic
{
// Construction
public:
	CnHyperlink();

	enum UnderLineMode{
		ULM_NEVER = 0,	//!< 从不使用下划线
		ULM_ALWAYS,		//!< 始终保持下划线
		ULM_OVER,		//!< 进入窗体范围显示下划线
		ULM_CUSTOM		//!< 用户控制显示下划线
	};

	enum TextColorMode{
		TCM_OVER = 0,	//!< 处于焦点
		TCM_NORMAL,		//!< 正常情况
		TCM_VISITED		//!< 已访问
	};

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CnHyperlink)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	
	virtual ~CnHyperlink();
	//! 设置关联字符串 命令或者统一资源定位字符串
	void SetUrlStr(const CString& str)
	{
		strUrl_ = str;
	}
	//! 获得关联字符串 命令或者统一资源定位字符串
	const CString& GetUrlStr()
	{
		return strUrl_;
	}
	//! 设置焦点状态下的光标 谁分配谁释放
	HCURSOR SetCursor(HCURSOR hOverCursor)
	{
		HCURSOR hOld = hCursor_;
		ASSERT(hOverCursor);

		hCursor_ = hOverCursor;
		return hOld;
	}
	//! 使用命令模式(system)还是统一资源定位模式(url)
	void SetCmdMode(bool b = true)
	{
		bCmdMode_ = b;
	}
	//! 设定字体颜色
	void SetTextColor(TextColorMode eIndex, COLORREF clr)
	{
		clrText_[eIndex] = clr;
	}
	//! 获得字体颜色
	COLORREF GetTextColor(TextColorMode eIndex)
	{
		return clrText_[eIndex];
	}
	//! 设定字体 采用复制方案
	void SetFont(CFont& font)
	{
		LOGFONT lf;
		font.GetLogFont(&lf);
		lf.lfUnderline = bUnderLine_;

		if ((HFONT)Font_)
			Font_.DeleteObject();
		Font_.CreateFontIndirect(&lf);
		bUseParentFont_ = false;
	}
	//! 显示对齐
	void SetTextAlign(UINT uTextAlign)
	{
		uTextAlign_ = uTextAlign;
	}
	//! 设定下划线显示模式
	void SetUnderLineMode(UnderLineMode eMode)
	{
		UnderLineMode_ = eMode;
	}
	//! 设定使用下划线
	void SetUnderLine(bool bUnderLine);

	// Generated message map functions
protected:
	CFont			Font_;				//!< 父窗口字体
	bool			bUseParentFont_;	//!< 是否使用父窗体字体
	bool			bUnderLine_;		//!< 当前是否有下划线
	bool			bVisited_;			//!< 是否访问过
	COLORREF		clrText_[3];		//!< 字体3种状态下的颜色
	CString			strUrl_;			//!< 统一资源定位字符串或者命令字符串
	HCURSOR			hCursor_;			//!< 当前焦点状态光标
	UnderLineMode	UnderLineMode_;		//!< 下划线显示模式
	bool			bCmdMode_;			//!< 是否命令模式
	HCURSOR			hHandCursor_;		//!< 初始转载的手形光标
	UINT			uTextAlign_;

	//{{AFX_MSG(CnHyperlink)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClicked();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNHYPERLINK_H__B798794F_763C_46F4_8E6E_7B9414A769AA__INCLUDED_)
