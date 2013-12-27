// testDlg.h : header file
//

#if !defined(AFX_TESTDLG_H__77DD88BB_CE39_4A5F_9103_D384A26A9487__INCLUDED_)
#define AFX_TESTDLG_H__77DD88BB_CE39_4A5F_9103_D384A26A9487__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog
#include "..\..\src\cncomm.h"

class CTestDlg : public CDialog
{
// Construction
public:
	void SetState();
	CTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_TEST_DIALOG };
	CStatic	m_Tx;
	CStatic	m_Rx;
	CButton	m_Open;
	CString	m_Resv;
	CString	m_Send;
	BOOL	m_bHex;
	CString	m_Baudrate;
	int		m_ByteSize;
	int		m_Port;
	int		m_Stopbits;
	int		m_Parity;
	BOOL	m_bAuto;
	UINT	m_uSendTimeMs;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CnComm m_Com;
	UINT m_uTimer;
	UINT m_uSendTimer;

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnComRevc(WPARAM, LPARAM);
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonClear();
	afx_msg void OnEditchangeComboBaudrate();
	afx_msg void OnSelchangeComboBytesize();
	afx_msg void OnSelchangeComboParity();
	afx_msg void OnSelchangeComboStopbits();
	afx_msg void OnCheckAuto();
	afx_msg void OnChangeEditTimer();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonReset();
	afx_msg void OnDestroy();
	afx_msg void OnCheckHex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__77DD88BB_CE39_4A5F_9103_D384A26A9487__INCLUDED_)
