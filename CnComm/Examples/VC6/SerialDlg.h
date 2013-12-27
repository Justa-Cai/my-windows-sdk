#if !defined(AFX_SERIALDLG_H__C93FF80C_17E3_42E7_BC90_CB5850D497ED__INCLUDED_)
#define AFX_SERIALDLG_H__C93FF80C_17E3_42E7_BC90_CB5850D497ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SerialDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SerialDlg dialog
#include "CnHyperlink.h"
#define ON_NEW_INSTANCE		WM_USER + 1
#define ON_FREE_INSTANCE	WM_USER + 2
#define	ON_COM_OPEN			WM_USER + 3	
#define	ON_COM_CLOSE		WM_USER + 4	

class SerialDlg : public CDialog
{
// Construction
public:
	void SetState();
	SerialDlg(int iInstance, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SerialDlg)
	enum { IDD = IDD_DIALOG_CN_COMM };
	CnHyperlink	m_Mail;
	CnHyperlink	m_Blog2;
	CnHyperlink	m_Blog1;
	CStatic	m_Lamp;
	CStatic	m_Flag;
	CEdit	m_TxQueue;
	CEdit	m_TxBuffer;
	CEdit	m_RxQueue;
	CEdit	m_RxBuffer;
	CEdit	m_TxCount;
	CEdit	m_RxCount;
	CString	m_sBaudrate;
	int		m_iBitsSize;
	int		m_iPort;
	int		m_iParity;
	int		m_iStopbits;
	UINT	m_uCycle;
	CString	m_sRx;
	CString	m_sTx;
	BOOL	m_bEnOverlapped;
	BOOL	m_bEnRxBuffer;
	BOOL	m_bAutoSend;
	BOOL	m_bEnThread;
	BOOL	m_bEnTxBuffer;
	BOOL	m_bEnRxThread;
	BOOL	m_bEnTxThread;
	int		m_iFlowCtrl;
	BOOL	m_bRxHex;
	BOOL	m_bTxHex;
	BOOL	m_bBreak;
	BOOL	m_bDtr;
	BOOL	m_bRts;
	BOOL	m_bXon;
	CString	m_RxFileName;
	CString	m_TxFileName;
	BOOL	m_bDisplay;
	BOOL	m_bAutoSave;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SerialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CnComm Comm_;
	CnComm::BlockBuffer Rx_;
	UINT m_uTimer;
	UINT m_uSendTimer;
	int iInstance_;
	HICON hIcons[2];

	// Generated message map functions
	//{{AFX_MSG(SerialDlg)
	afx_msg LRESULT OnReceive(WPARAM, LPARAM);
	afx_msg LRESULT OnRLSD(WPARAM, LPARAM);
	afx_msg LRESULT OnBreak(WPARAM, LPARAM);
	afx_msg LRESULT OnRing(WPARAM, LPARAM);
	afx_msg LRESULT OnCTS(WPARAM, LPARAM);
	afx_msg LRESULT OnDSR(WPARAM, LPARAM);
	afx_msg void OnButtonNewInstance();
	afx_msg void OnButtonFreeInstance();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonClearRx();
	afx_msg void OnButtonClearTx();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnCheckAutoSend();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonDcb();
	afx_msg void OnCheckEnRxBuffer();
	afx_msg void OnCheckEnTxThread();
	afx_msg void OnCheckEnOverlapped();
	afx_msg void OnCheckEnTxBuffer();
	afx_msg void OnCheckEnRxThread();
	afx_msg void OnEditchangeComboBaudrate();
	afx_msg void OnEditchangeComboParity();
	afx_msg void OnEditchangeComboBitssize();
	afx_msg void OnEditchangeComboStopbits();
	afx_msg void OnEditchangeComboFlowCtrl();
	afx_msg void OnCheckRxHex();
	afx_msg void OnCheckTxHex();
	afx_msg void OnButtonTimeouts();
	afx_msg void OnButtonLoadFile();
	afx_msg void OnCheckDtr();
	afx_msg void OnCheckBreak();
	afx_msg void OnCheckRts();
	afx_msg void OnCheckXon();
	afx_msg void OnButtonProperties();
	afx_msg void OnButtonSendFile();
	afx_msg void OnButtonBrowerTx();
	afx_msg void OnButtonBrowerRx();
	afx_msg void OnChangeEditCycle();
	afx_msg void OnCheckSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALDLG_H__C93FF80C_17E3_42E7_BC90_CB5850D497ED__INCLUDED_)
