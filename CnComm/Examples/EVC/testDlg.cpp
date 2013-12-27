// testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTestDlg::IDD, pParent)
, m_Com(CnComm::EN_THREAD | CnComm::EN_TX_BUFFER | CnComm::EN_TX_THREAD | CnComm::EN_RX_BUFFER)
{
	//{{AFX_DATA_INIT(CTestDlg)
	m_Resv = _T("");
	m_Send = _T("CnComm 1.5; http://blog.csdn.net/wujian53");
	m_bHex = FALSE;
	m_Baudrate = _T("9600");
	m_ByteSize = 4;
	m_Port = 0;
	m_Stopbits = 0;
	m_Parity = 2;
	m_bAuto = FALSE;
	m_uSendTimeMs = 500;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_uTimer = 0;
	m_uSendTimer = 0;
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_STATIC_TX, m_Tx);
	DDX_Control(pDX, IDC_STATIC_RX, m_Rx);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_Open);
	DDX_Text(pDX, IDC_EDIT_RECV, m_Resv);
	DDX_Text(pDX, IDC_EDIT_SEND, m_Send);
	DDX_Check(pDX, IDC_CHECK_HEX, m_bHex);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_Baudrate);
	DDX_CBIndex(pDX, IDC_COMBO_BYTESIZE, m_ByteSize);
	DDX_CBIndex(pDX, IDC_COMBO_PORT, m_Port);
	DDX_CBIndex(pDX, IDC_COMBO_STOPBITS, m_Stopbits);
	DDX_CBIndex(pDX, IDC_COMBO_PARITY, m_Parity);
	DDX_Check(pDX, IDC_CHECK_AUTO, m_bAuto);
	DDX_Text(pDX, IDC_EDIT_TIMER, m_uSendTimeMs);
	DDV_MinMaxUInt(pDX, m_uSendTimeMs, 0, 4294967295);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_MESSAGE(ON_COM_RECEIVE, OnComRevc)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_CBN_EDITCHANGE(IDC_COMBO_BAUDRATE, OnEditchangeComboBaudrate)
	ON_CBN_SELCHANGE(IDC_COMBO_BYTESIZE, OnSelchangeComboBytesize)
	ON_CBN_SELCHANGE(IDC_COMBO_PARITY, OnSelchangeComboParity)
	ON_CBN_SELCHANGE(IDC_COMBO_STOPBITS, OnSelchangeComboStopbits)
	ON_BN_CLICKED(IDC_CHECK_AUTO, OnCheckAuto)
	ON_EN_CHANGE(IDC_EDIT_TIMER, OnChangeEditTimer)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_HEX, OnCheckHex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	m_Com.SetWnd(this->m_hWnd);
	m_uTimer = SetTimer(1, 200, NULL);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnButtonOpen() 
{
	UpdateData();
	
	if (!m_Com.IsOpen())
	{
		if (!m_Com.Open(m_Port+1))
		{
			TCHAR szBuf[1024];
			wsprintf(szBuf, _T("Open COM%d fail, Code:%d"), m_Port+1, GetLastError());
			AfxMessageBox(szBuf);
			return;
		}
		else
			m_Open.SetWindowText(_T("Close"));
	}
	else
	{
		m_Com.Close();
		m_Open.SetWindowText(_T("Open"));
	}
	
	SetState(); 
	
}

int Hex2Ascii(const char* hex, char* ascii)
{
	int len = strlen(hex), tlen, i, cnt;
	
	for (i = 0, cnt = 0, tlen = 0; i<len; i++)
	{
		char c = toupper(hex[i]);

		if ((c>='0'&& c<='9') || (c>='A'&& c<='F'))
		{
			BYTE t = (c >= 'A') ? c - 'A' + 10 : c - '0';

			if (cnt)
				ascii[tlen++] += t, cnt = 0;
			else
				ascii[tlen] = t << 4, cnt = 1;
		}
	}

	ascii[tlen] = '\0';
	
	return tlen;
}

int Ascii2Hex(const char* ascii, char* hex)
{
	int i, len = strlen(ascii);
	char chHex[] = "0123456789ABCDEF";
					
	for (i = 0; i<len; i++)
	{
		hex[i*3]	= chHex[((BYTE)ascii[i]) >> 4];
		hex[i*3 +1]	= chHex[((BYTE)ascii[i]) & 0xf];
		hex[i*3 +2]	= ' ';
	}

	hex[len * 3] = '\0';

	return len * 3;
}

LRESULT CTestDlg::OnComRevc(WPARAM, LPARAM)
{
	do {

		char buffer[128];
		char hex_buffer[128*3];
		TCHAR tbuffer[128*3];
		
		int len = m_Com.Read(buffer, 127);
		buffer[len] = '\0';

		if (m_bHex)
		{
			Ascii2Hex(buffer, hex_buffer);
			mbstowcs(tbuffer, hex_buffer, 128*3);
		}
		else
			mbstowcs(tbuffer, buffer, 128);//ansi --> unicode

		UpdateData();
		m_Resv += tbuffer;
		UpdateData(FALSE);
		
	} while(m_Com.IsRxBufferMode() &&  m_Com.Input().SafeSize());
	

	return TRUE;
}


void CTestDlg::OnButtonSend() 
{
	char buffer[1024];
	char btBuf[512];
	int HexCnt = 0;
	//unicode --> ansi
	int len = wcstombs(buffer, (LPCTSTR)m_Send, 1024);
	buffer[len]= 0;
	UpdateData();
	if (m_bHex)
	{
		 HexCnt = Hex2Ascii(buffer, btBuf);
		 m_Com.Write(btBuf, HexCnt);
	}
	else
		m_Com.Write(buffer);
}

void CTestDlg::OnButtonClear() 
{
	m_Resv = _T("");
	UpdateData(FALSE);
}


void CTestDlg::OnEditchangeComboBaudrate() 
{
	SetState(); 
}

void CTestDlg::OnSelchangeComboBytesize() 
{
	SetState(); 
}

void CTestDlg::OnSelchangeComboParity() 
{
	SetState(); 
}

void CTestDlg::OnSelchangeComboStopbits() 
{
	SetState(); 
}

void CTestDlg::SetState() 
{
	if (!m_Com.IsOpen())
		return;

	struct Parity{
		BYTE dwParity;
		TCHAR sParity[24];
	} Paritys[] = 
	{
		{EVENPARITY  ,_T("EVEN")},
		{MARKPARITY  ,_T("MARK")},
		{NOPARITY    ,_T("NO")},
		{ODDPARITY   ,_T("ODD")},
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
	temp[wcstombs(temp, (LPCTSTR)m_Baudrate, m_Baudrate.GetLength())] = 0;

	DWORD dwBaudrate = atoi(temp);
	if (!m_Com.SetState(dwBaudrate, Paritys[m_Parity].dwParity, ByteSizes[m_ByteSize], StopBits[m_Stopbits].dwStopBit))
	{
		wsprintf(szBuf, _T("SetState(%d, %d, %d, %d), Fail. Code:%d"), 
			dwBaudrate, 
			Paritys[m_Parity].dwParity, 
			ByteSizes[m_ByteSize], 
			StopBits[m_Stopbits].dwStopBit,
			GetLastError());
		AfxMessageBox(szBuf);
		return;
	}
}

void CTestDlg::OnCheckAuto() 
{
	UpdateData();
	if (m_bAuto)
	{
		if (m_uSendTimer)
			KillTimer(2);
		m_uSendTimer = SetTimer(2, m_uSendTimeMs, NULL);
	}
	else
	{
		KillTimer(2);
		m_uSendTimer = 0;
	}
}

void CTestDlg::OnChangeEditTimer() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	OnCheckAuto() ;

	
}

void CTestDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		CString str;
		str.Format(_T("%d"), m_Com.GetCounter());
		m_Rx.SetWindowText(str);
		str.Format(_T("%d"), m_Com.GetCounter(false));
		m_Tx.SetWindowText(str);
	}
	

	if (nIDEvent == 2)
	{
		OnButtonSend() ;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CTestDlg::OnButtonReset() 
{
	m_Com.ResetCounter();
}

void CTestDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	KillTimer(1);
	if (m_uSendTimer)
		KillTimer(2);
}

void CTestDlg::OnCheckHex() 
{
	UpdateData();
	
}
