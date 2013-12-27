#if !defined(AFX_DCBDLG_H__1721D990_C370_4656_8E90_30B62D6B8AD2__INCLUDED_)
#define AFX_DCBDLG_H__1721D990_C370_4656_8E90_30B62D6B8AD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DcbDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DcbDlg dialog
class CnComm;

class DcbDlg : public CDialog
{
// Construction
public:
	DcbDlg(CnComm *p, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DcbDlg)
	enum { IDD = IDD_DIALOG_DCB };
	UINT	m_uBaudrate;
	UINT	m_uByteSize;
	UINT	m_uDcbLength;
	BOOL	m_bAbortOnError;
	BOOL	m_bBinary;
	BOOL	m_DsrSensitivity;
	BOOL	m_bParity;
	BOOL	m_bTxContinueOnXoff;
	BYTE	m_btStopBits;
	BYTE	m_btParity;
	BOOL	m_bOutxCtsFlow;
	BYTE	m_btRtsControl;
	BYTE	m_btDtrControl;
	BOOL	m_bErrorChar;
	BOOL	m_bInx;
	BOOL	m_bNull;
	BOOL	m_bOutX;
	BYTE	m_chXoffChar;
	BYTE	m_btEofChar;
	BYTE	m_btErrorChar;
	BYTE	m_btEvtChar;
	BYTE	m_btXoffLim;
	BYTE	m_btXonChar;
	BYTE	m_btXonLim;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DcbDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CnComm	*pComm_;
	DCB		*pDcb_;

	// Generated message map functions
	//{{AFX_MSG(DcbDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCBDLG_H__1721D990_C370_4656_8E90_30B62D6B8AD2__INCLUDED_)
