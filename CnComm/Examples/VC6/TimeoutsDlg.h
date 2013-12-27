#if !defined(AFX_TIMEOUTSDLG_H__1F04359D_9A4B_489F_B2FC_0D879838037A__INCLUDED_)
#define AFX_TIMEOUTSDLG_H__1F04359D_9A4B_489F_B2FC_0D879838037A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeoutsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TimeoutsDlg dialog

class TimeoutsDlg : public CDialog
{
// Construction
public:
	TimeoutsDlg(CnComm *p, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TimeoutsDlg)
	enum { IDD = IDD_DIALOG_TIMEOUTS };
	UINT	m_uRIT;
	UINT	m_uRTTC;
	UINT	m_uRTTM;
	UINT	m_uWTTC;
	UINT	m_uWTTM;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TimeoutsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CnComm	*pComm_;
	COMMTIMEOUTS* pCo_;

	// Generated message map functions
	//{{AFX_MSG(TimeoutsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMEOUTSDLG_H__1F04359D_9A4B_489F_B2FC_0D879838037A__INCLUDED_)
