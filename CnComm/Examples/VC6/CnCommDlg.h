// CnCommDlg.h : header file
//

#if !defined(AFX_CNCOMMDLG_H__F41A159E_136D_40C1_8EEC_322CAFCBDB76__INCLUDED_)
#define AFX_CNCOMMDLG_H__F41A159E_136D_40C1_8EEC_322CAFCBDB76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CnCommDlg dialog

class SerialDlg;


class CnCommDlg : public CDialog
{
// Construction
public:
	void FreeInstance(SerialDlg * iIndex);
	void NewInstance();
	CnCommDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CnCommDlg)
	enum { IDD = IDD_VC6_DIALOG };
	CTabCtrl	m_TabCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CnCommDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	UINT uInstanceId_;
	UINT uLastSel_, uCurSel_;
	vector<SerialDlg*> SerialDlgArray_;

	// Generated message map functions
	//{{AFX_MSG(CnCommDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg	LRESULT OnNewInstance(WPARAM, LPARAM);
	afx_msg	LRESULT OnFreeInstance(WPARAM, LPARAM);
	afx_msg	LRESULT OnComOpen(WPARAM, LPARAM);
	afx_msg	LRESULT OnComClose(WPARAM, LPARAM);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNCOMMDLG_H__F41A159E_136D_40C1_8EEC_322CAFCBDB76__INCLUDED_)
