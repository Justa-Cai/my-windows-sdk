#pragma once


// CDialogWide 对话框

class CDialogWide : public CDialog
{
	DECLARE_DYNAMIC(CDialogWide)

public:
	CDialogWide(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogWide();

// 对话框数据
	enum { IDD = IDD_CNCOMMTEST_DIALOG_WIDE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSend();
};
