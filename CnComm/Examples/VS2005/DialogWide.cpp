// DialogWide.cpp : 实现文件
//

#include "stdafx.h"
#include "CnCommTest.h"
#include "DialogWide.h"


// CDialogWide 对话框

IMPLEMENT_DYNAMIC(CDialogWide, CDialog)

CDialogWide::CDialogWide(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWide::IDD, pParent)
{

}

CDialogWide::~CDialogWide()
{
}

void CDialogWide::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogWide, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CDialogWide::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CDialogWide 消息处理程序

void CDialogWide::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
}
