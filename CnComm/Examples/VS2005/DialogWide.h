#pragma once


// CDialogWide �Ի���

class CDialogWide : public CDialog
{
	DECLARE_DYNAMIC(CDialogWide)

public:
	CDialogWide(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogWide();

// �Ի�������
	enum { IDD = IDD_CNCOMMTEST_DIALOG_WIDE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSend();
};
