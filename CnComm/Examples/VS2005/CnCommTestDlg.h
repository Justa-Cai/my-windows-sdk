// CnCommTestDlg.h : ͷ�ļ�
//

#pragma once

// CCnCommTestDlg �Ի���
class CCnCommTestDlg : public CDialog
{
// ����
public:
	CCnCommTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CNCOMMTEST_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	CnComm Com_;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
	afx_msg void OnDestroy();
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSend();
	afx_msg LRESULT OnComRecv(WPARAM, LPARAM);
	CString RxStr;
	CString TxStr;
	void SetState();
	afx_msg void OnCbnSelchangeComboBaudrate();
	CString m_sBaudrate;
	afx_msg void OnBnClickedButtonOpen();
	int m_uByteSize;
	int m_uStopbits;
	int m_uParity;
	int m_uPort;
	HICON hIcons[2];
	CStatic m_Lamp;
};
