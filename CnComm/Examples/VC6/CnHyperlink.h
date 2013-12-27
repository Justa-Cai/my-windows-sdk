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
		ULM_NEVER = 0,	//!< �Ӳ�ʹ���»���
		ULM_ALWAYS,		//!< ʼ�ձ����»���
		ULM_OVER,		//!< ���봰�巶Χ��ʾ�»���
		ULM_CUSTOM		//!< �û�������ʾ�»���
	};

	enum TextColorMode{
		TCM_OVER = 0,	//!< ���ڽ���
		TCM_NORMAL,		//!< �������
		TCM_VISITED		//!< �ѷ���
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
	//! ���ù����ַ��� �������ͳһ��Դ��λ�ַ���
	void SetUrlStr(const CString& str)
	{
		strUrl_ = str;
	}
	//! ��ù����ַ��� �������ͳһ��Դ��λ�ַ���
	const CString& GetUrlStr()
	{
		return strUrl_;
	}
	//! ���ý���״̬�µĹ�� ˭����˭�ͷ�
	HCURSOR SetCursor(HCURSOR hOverCursor)
	{
		HCURSOR hOld = hCursor_;
		ASSERT(hOverCursor);

		hCursor_ = hOverCursor;
		return hOld;
	}
	//! ʹ������ģʽ(system)����ͳһ��Դ��λģʽ(url)
	void SetCmdMode(bool b = true)
	{
		bCmdMode_ = b;
	}
	//! �趨������ɫ
	void SetTextColor(TextColorMode eIndex, COLORREF clr)
	{
		clrText_[eIndex] = clr;
	}
	//! ���������ɫ
	COLORREF GetTextColor(TextColorMode eIndex)
	{
		return clrText_[eIndex];
	}
	//! �趨���� ���ø��Ʒ���
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
	//! ��ʾ����
	void SetTextAlign(UINT uTextAlign)
	{
		uTextAlign_ = uTextAlign;
	}
	//! �趨�»�����ʾģʽ
	void SetUnderLineMode(UnderLineMode eMode)
	{
		UnderLineMode_ = eMode;
	}
	//! �趨ʹ���»���
	void SetUnderLine(bool bUnderLine);

	// Generated message map functions
protected:
	CFont			Font_;				//!< ����������
	bool			bUseParentFont_;	//!< �Ƿ�ʹ�ø���������
	bool			bUnderLine_;		//!< ��ǰ�Ƿ����»���
	bool			bVisited_;			//!< �Ƿ���ʹ�
	COLORREF		clrText_[3];		//!< ����3��״̬�µ���ɫ
	CString			strUrl_;			//!< ͳһ��Դ��λ�ַ������������ַ���
	HCURSOR			hCursor_;			//!< ��ǰ����״̬���
	UnderLineMode	UnderLineMode_;		//!< �»�����ʾģʽ
	bool			bCmdMode_;			//!< �Ƿ�����ģʽ
	HCURSOR			hHandCursor_;		//!< ��ʼת�ص����ι��
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
