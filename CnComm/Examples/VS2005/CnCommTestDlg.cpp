// CnCommTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <vector>
using namespace std;
#include "CnCommTest.h"
#include "CnCommTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCnCommTestDlg 对话框

CCnCommTestDlg::CCnCommTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCnCommTestDlg::IDD, pParent)
	, RxStr(_T(""))
	, TxStr(_T("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
	, Com_(CnComm::EN_THREAD | CnComm::EN_TX_BUFFER | CnComm::EN_TX_THREAD | CnComm::EN_RX_BUFFER)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_sBaudrate = _T("9600");
	m_uPort = 0;
	m_uStopbits = 0;
	m_uParity = 0;
	m_uByteSize = 4;
}

void CCnCommTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RX, RxStr);
	DDX_Text(pDX, IDC_EDIT_TX, TxStr);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_sBaudrate);
	DDX_CBIndex(pDX, IDC_COMBO_BYTESIZE, m_uByteSize);
	DDX_CBIndex(pDX, IDC_COMBO_PARITY, m_uParity);
	DDX_CBIndex(pDX, IDC_COMBO_STOPBITS, m_uStopbits);
	DDX_CBIndex(pDX, IDC_COMBO_PORT, m_uPort);
	DDX_Control(pDX, IDC_STATIC_LAMP, m_Lamp);
}

BEGIN_MESSAGE_MAP(CCnCommTestDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CCnCommTestDlg::OnBnClickedButtonSend)
	ON_MESSAGE(ON_COM_RECEIVE, OnComRecv)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CCnCommTestDlg::OnBnClickedButtonOpen)
END_MESSAGE_MAP()


// CCnCommTestDlg 消息处理程序

BOOL CCnCommTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	hIcons[0] = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_OFF), IMAGE_ICON, 0, 0, 0);
	hIcons[1] = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ON), IMAGE_ICON, 0, 0, 0);

	m_Lamp.ModifyStyle(0, SS_ICON);
	m_Lamp.SetIcon(hIcons[0], FALSE);

	Com_.SetWnd(m_hWnd);

		return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCnCommTestDlg::OnDestroy() 
{
	DestroyIcon(hIcons[0]);
	DestroyIcon(hIcons[1]);
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CCnCommTestDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_CNCOMMTEST_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_CNCOMMTEST_DIALOG));
	}
}
#endif


void CCnCommTestDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!Com_.IsOpen())
		return ;

	UpdateData();
	char buf[1024];
	buf[wcstombs(buf, TxStr, 1023)] = 0;
	
	DWORD dwSend = Com_.Write(buf);
	
}

LRESULT CCnCommTestDlg::OnComRecv(WPARAM, LPARAM)
{
	char buf[1024];
	TCHAR sbuf[1024];
	int len;

	len = Com_.Read(buf, 1024);

	mbstowcs(sbuf, buf, len);
	RxStr += sbuf;
	RxStr += _T("\r\n");

	UpdateData(0);

	return 1;
}

void CCnCommTestDlg::OnBnClickedButtonOpen()
{
	if (!Com_.IsOpen())
	{
		if (!Com_.Open(m_uPort+1))
		{
			TCHAR szBuf[1024];
			wsprintf(szBuf, _T("打开 COM%d 失败, 代码:%d"), m_uPort+1, GetLastError());
			MessageBox(szBuf);
			return;
		}
		else
		{
			SendDlgItemMessage(IDC_BUTTON_OPEN, WM_SETTEXT, 0, (LPARAM)_T("关闭"));
			m_Lamp.SetIcon(hIcons[1], FALSE);
		}
	}
	else
	{
		Com_.Close();
		SendDlgItemMessage(IDC_BUTTON_OPEN, WM_SETTEXT, 0, (LPARAM)_T("打开"));
		m_Lamp.SetIcon(hIcons[0], FALSE);
	}
}

void CCnCommTestDlg::SetState()
{
	if (!Com_.IsOpen())
		return;

	struct Parity{
		BYTE dwParity;
		TCHAR sParity[24];
	} Paritys[] = 
	{
		{NOPARITY    ,_T("NO")},
		{ODDPARITY   ,_T("ODD")},
		{EVENPARITY  ,_T("EVEN")},
		{MARKPARITY  ,_T("MARK")},
		{SPACEPARITY ,_T("SPACE")}
	};

	BYTE ByteSizes[] = {4, 5, 6, 7, 8};

	struct StopBit {
		BYTE dwStopBit;
		TCHAR sStopBit[24];
	} StopBits [] = 
	{
		{ONESTOPBIT   ,_T("ONESTOPBIT")},
		{ONE5STOPBITS ,_T("ONE5STOPBITS")},
		{TWOSTOPBITS  ,_T("TWOSTOPBITS")},
	};

	TCHAR szBuf[1024];

	UpdateData();
	char temp[120];
	temp[wcstombs(temp, (LPCTSTR)m_sBaudrate, m_sBaudrate.GetLength())] = 0;

	DCB* pDcb = Com_.GetState();
	pDcb->BaudRate = atoi(temp);
	pDcb->ByteSize = ByteSizes[m_uByteSize];
	pDcb->StopBits = StopBits[m_uStopbits].dwStopBit;
	pDcb->Parity = Paritys[m_uParity].dwParity;
	//pDcb->

	DWORD dwBaudrate = atoi(temp);
	if (!Com_.SetState(pDcb))
	{
		wsprintf(szBuf, _T("SetState(%d, %d, %d, %d), Fail. Code:%d"), 
			dwBaudrate, 
			Paritys[m_uParity].dwParity, 
			ByteSizes[m_uByteSize], 
			StopBits[m_uStopbits].dwStopBit,
			GetLastError());
		MessageBox(szBuf);
		return;
	}
}
