// DialogWide.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CnCommTest.h"
#include "DialogWide.h"


// CDialogWide �Ի���

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


// CDialogWide ��Ϣ�������

void CDialogWide::OnBnClickedButtonSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
