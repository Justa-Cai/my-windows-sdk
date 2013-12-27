/*! \file SerialDlg.cpp 
	\brief CnComm范例程序(VC6) 串口调试器
*/

#include "stdafx.h"
#include "VC6.h"
#include "SerialDlg.h"
#include "DcbDlg.h"
#include "TimeoutsDlg.h"
#include "PropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SerialDlg dialog
int Ascii2Hex(char* ascii, char* hex);
int Hex2Ascii(char* hex, char* ascii);

SerialDlg::SerialDlg(int iInstance, CWnd* pParent /*=NULL*/)
: CDialog(SerialDlg::IDD, pParent), iInstance_(iInstance)
{
	//{{AFX_DATA_INIT(SerialDlg)
	m_sBaudrate = _T("9600");
	m_iBitsSize = 4;
	m_iPort = 0;
	m_iParity = 0;
	m_iStopbits = 0;
	m_uCycle = 1000;
	m_sRx = _T("");
	m_sTx = _T("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	m_bEnOverlapped = TRUE;
	m_bEnRxBuffer = TRUE;
	m_bAutoSend = FALSE;
	m_bEnThread = TRUE;
	m_bEnTxBuffer = TRUE;
	m_bEnRxThread = FALSE;
	m_bEnTxThread = FALSE;
	m_iFlowCtrl = 0;
	m_bRxHex = FALSE;
	m_bTxHex = FALSE;
	m_bBreak = FALSE;
	m_bDtr = FALSE;
	m_bRts = FALSE;
	m_bXon = FALSE;
	m_RxFileName = _T("");
	m_TxFileName = _T("");
	m_bDisplay = TRUE;
	m_bAutoSave = FALSE;
	//}}AFX_DATA_INIT
	
	Comm_.SetOption(Comm_.GetOption() | CnComm::EN_RX_BUFFER);
}


void SerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SerialDlg)
	DDX_Control(pDX, IDC_STATIC_MAIL, m_Mail);
	DDX_Control(pDX, IDC_STATIC_BLOG2, m_Blog2);
	DDX_Control(pDX, IDC_STATIC_BLOG1, m_Blog1);
	DDX_Control(pDX, IDC_STATIC_LAMP, m_Lamp);
	DDX_Control(pDX, IDC_EDIT_TX_QUEUE, m_TxQueue);
	DDX_Control(pDX, IDC_EDIT_TX_BUFFER, m_TxBuffer);
	DDX_Control(pDX, IDC_EDIT_RX_QUEUE, m_RxQueue);
	DDX_Control(pDX, IDC_EDIT_RX_BUFFER, m_RxBuffer);
	DDX_Control(pDX, IDC_EDIT_TX_COUNT, m_TxCount);
	DDX_Control(pDX, IDC_EDIT_RX_COUNT, m_RxCount);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_sBaudrate);
	DDX_CBIndex(pDX, IDC_COMBO_BITSSIZE, m_iBitsSize);
	DDX_CBIndex(pDX, IDC_COMBO_COM_PORT, m_iPort);
	DDX_CBIndex(pDX, IDC_COMBO_PARITY, m_iParity);
	DDX_CBIndex(pDX, IDC_COMBO_STOPBITS, m_iStopbits);
	DDX_Text(pDX, IDC_EDIT_CYCLE, m_uCycle);
	DDX_Text(pDX, IDC_EDIT_RX, m_sRx);
	DDX_Text(pDX, IDC_EDIT_TX, m_sTx);
	DDX_Check(pDX, IDC_CHECK_EN_OVERLAPPED, m_bEnOverlapped);
	DDX_Check(pDX, IDC_CHECK_EN_RX_BUFFER, m_bEnRxBuffer);
	DDX_Check(pDX, IDC_CHECK_AUTO_SEND, m_bAutoSend);
	DDX_Check(pDX, IDC_CHECK_EN_THREAD, m_bEnThread);
	DDX_Check(pDX, IDC_CHECK_EN_TX_BUFFER, m_bEnTxBuffer);
	DDX_Check(pDX, IDC_CHECK_EN_RX_THREAD, m_bEnRxThread);
	DDX_Check(pDX, IDC_CHECK_EN_TX_THREAD, m_bEnTxThread);
	DDX_CBIndex(pDX, IDC_COMBO_FLOW_CTRL, m_iFlowCtrl);
	DDX_Check(pDX, IDC_CHECK_RX_HEX, m_bRxHex);
	DDX_Check(pDX, IDC_CHECK_TX_HEX, m_bTxHex);
	DDX_Check(pDX, IDC_CHECK_BREAK, m_bBreak);
	DDX_Check(pDX, IDC_CHECK_DTR, m_bDtr);
	DDX_Check(pDX, IDC_CHECK_RTS, m_bRts);
	DDX_Check(pDX, IDC_CHECK_XON, m_bXon);
	DDX_Text(pDX, IDC_EDIT_FILE_FOR_RECV, m_RxFileName);
	DDX_Text(pDX, IDC_EDIT_FILE_FOR_SEND, m_TxFileName);
	DDX_Check(pDX, IDC_CHECK_DISP, m_bDisplay);
	DDX_Check(pDX, IDC_CHECK_SAVE, m_bAutoSave);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SerialDlg, CDialog)
//{{AFX_MSG_MAP(SerialDlg)
	ON_MESSAGE(ON_COM_RECEIVE, OnReceive)
	ON_MESSAGE(ON_COM_RLSD, OnRLSD)
	ON_MESSAGE(ON_COM_BREAK, OnBreak)
	ON_MESSAGE(ON_COM_RING, OnRing)
	ON_MESSAGE(ON_COM_CTS, OnCTS)
	ON_MESSAGE(ON_COM_DSR, OnDSR)
	ON_BN_CLICKED(IDC_BUTTON_NEW_INSTANCE, OnButtonNewInstance)
	ON_BN_CLICKED(IDC_BUTTON_FREE_INSTANCE, OnButtonFreeInstance)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_RX, OnButtonClearRx)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_TX, OnButtonClearTx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_AUTO_SEND, OnCheckAutoSend)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_DCB, OnButtonDcb)
	ON_BN_CLICKED(IDC_CHECK_EN_RX_BUFFER, OnCheckEnRxBuffer)
	ON_BN_CLICKED(IDC_CHECK_EN_TX_THREAD, OnCheckEnTxThread)
	ON_BN_CLICKED(IDC_CHECK_EN_OVERLAPPED, OnCheckEnOverlapped)
	ON_BN_CLICKED(IDC_CHECK_EN_TX_BUFFER, OnCheckEnTxBuffer)
	ON_BN_CLICKED(IDC_CHECK_EN_RX_THREAD, OnCheckEnRxThread)
	ON_CBN_EDITCHANGE(IDC_COMBO_BAUDRATE, OnEditchangeComboBaudrate)
	ON_CBN_EDITCHANGE(IDC_COMBO_PARITY, OnEditchangeComboParity)
	ON_CBN_EDITCHANGE(IDC_COMBO_BITSSIZE, OnEditchangeComboBitssize)
	ON_CBN_EDITCHANGE(IDC_COMBO_STOPBITS, OnEditchangeComboStopbits)
	ON_CBN_EDITCHANGE(IDC_COMBO_FLOW_CTRL, OnEditchangeComboFlowCtrl)
	ON_BN_CLICKED(IDC_CHECK_RX_HEX, OnCheckRxHex)
	ON_BN_CLICKED(IDC_CHECK_TX_HEX, OnCheckTxHex)
	ON_BN_CLICKED(IDC_BUTTON_TIMEOUTS, OnButtonTimeouts)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FILE, OnButtonLoadFile)
	ON_BN_CLICKED(IDC_CHECK_DTR, OnCheckDtr)
	ON_BN_CLICKED(IDC_CHECK_BREAK, OnCheckBreak)
	ON_BN_CLICKED(IDC_CHECK_RTS, OnCheckRts)
	ON_BN_CLICKED(IDC_CHECK_XON, OnCheckXon)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTIES, OnButtonProperties)
	ON_BN_CLICKED(IDC_BUTTON_SEND_FILE, OnButtonSendFile)
	ON_BN_CLICKED(IDC_BUTTON_BROWER_TX, OnButtonBrowerTx)
	ON_BN_CLICKED(IDC_BUTTON_BROWER_RX, OnButtonBrowerRx)
	ON_EN_CHANGE(IDC_EDIT_CYCLE, OnChangeEditCycle)
	ON_BN_CLICKED(IDC_CHECK_SAVE, OnCheckSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SerialDlg message handlers

BOOL SerialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	Comm_.SetWnd(this->m_hWnd);
	m_uTimer = SetTimer(1, 100, NULL);
	hIcons[0] = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_OFF), IMAGE_ICON, 0, 0, 0);
	hIcons[1] = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_ON), IMAGE_ICON, 0, 0, 0);
	ASSERT(hIcons[0]);
	
	m_Lamp.ModifyStyle(0, SS_ICON);
	m_Lamp.SetIcon(hIcons[0]);
	Rx_.SetMinBlockSize(4096);
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void SerialDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
		UpdateData();

		CString str;
		str.Format(_T("%d"), Comm_.GetCounter());
		m_RxCount.SetWindowText(str);
		str.Format(_T("%d"), Comm_.GetCounter(false));
		m_TxCount.SetWindowText(str);
		
		if (Comm_.IsOpen())
		{
			str.Format(_T("%d"), Comm_.GetQueueCount());
			m_RxQueue.SetWindowText(str);
			str.Format(_T("%d"), Comm_.GetQueueCount(false));
			m_TxQueue.SetWindowText(str);
		}
		else
		{
			m_RxQueue.SetWindowText(_T(""));
			m_TxQueue.SetWindowText(_T(""));
		}
		
		
		str.Format(_T("%d"), Comm_.Input().Size());
		m_RxBuffer.SetWindowText(str);
		str.Format(_T("%d"), Comm_.Output().Size());
		m_TxBuffer.SetWindowText(str);

		str.Format("接受  %d字节", m_sRx.GetLength());
		SendDlgItemMessage(IDC_STATIC_RX, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)str);
		str.Format("发送  %d字节", m_sTx.GetLength());
		SendDlgItemMessage(IDC_STATIC_TX, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)str);
	}
	
	if (nIDEvent == 2)
	{
		if (Comm_.IsOpen())
			OnButtonSend() ;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void SerialDlg::OnDestroy() 
{
	DestroyIcon(hIcons[0]);
	DestroyIcon(hIcons[1]);
	
	KillTimer(1);
	if (m_uSendTimer)
		KillTimer(2);
}
//! 串口接收消息 读取串口数据
LRESULT SerialDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	if (wParam  == Comm_.GetPort())
	{
		UpdateData();
		
		do {
			
			char buffer[1024];
			int len = Comm_.Read(buffer, 1023);
			buffer[len] = _T('\0');
			Rx_.Write(buffer, len);
			
			if (m_bDisplay)
			{
				if (m_bRxHex)
				{
					char hex_buffer[1024*3];
					Ascii2Hex(buffer, hex_buffer);
					m_sRx += hex_buffer;
				}
				else
					m_sRx += buffer;
			}

			if (m_bAutoSave)
			{
				FILE* fp;
				fp = fopen(m_RxFileName, "a+");
				fwrite(buffer, len, 1, fp);
				fclose(fp);
			}
			//! 接收缓冲模式下，要确保把已经在Comm_.Input()缓冲区的数据处理完
			//! 否则如果没有新的数据再来，不会继续通知你
		} while(Comm_.IsRxBufferMode() && Comm_.Input().SafeSize());
		
		UpdateData(FALSE);
	}
	
	return TRUE;
}

LRESULT SerialDlg::OnRLSD(WPARAM wParam, LPARAM)
{
	if (wParam  == Comm_.GetPort())
		SendDlgItemMessage(IDC_CHECK_RLSD, BM_SETCHECK, Comm_.CheckRLSD());
	return TRUE;
}

LRESULT SerialDlg::OnBreak(WPARAM wParam, LPARAM)
{
	CString str;
	str.Format("COM%d be placed in a break state.", (int)wParam);
	MessageBox(str);
	return TRUE;
}

LRESULT SerialDlg::OnRing(WPARAM wParam, LPARAM)
{
	if (wParam  == Comm_.GetPort())
		SendDlgItemMessage(IDC_CHECK_RING, BM_SETCHECK, Comm_.CheckRING());
	return TRUE;
}

LRESULT SerialDlg::OnCTS(WPARAM wParam, LPARAM)
{
	if (wParam  == Comm_.GetPort())
		SendDlgItemMessage(IDC_CHECK_CTS, BM_SETCHECK, Comm_.CheckCTS());
	
	return TRUE;
}

LRESULT SerialDlg::OnDSR(WPARAM wParam, LPARAM)
{
	if (wParam  == Comm_.GetPort())
		SendDlgItemMessage(IDC_CHECK_DSR, BM_SETCHECK, Comm_.CheckDSR());
	return TRUE;
}

void SerialDlg::OnButtonNewInstance() 
{
	AfxGetMainWnd()->SendMessage(ON_NEW_INSTANCE);
}

void SerialDlg::OnButtonFreeInstance() 
{
	AfxGetMainWnd()->SendMessage(ON_FREE_INSTANCE, 0, (LPARAM)this);
}

void SerialDlg::OnButtonReset() 
{
	Comm_.ResetCounter();
	m_RxCount.SetWindowText(_T("0"));
	m_TxCount.SetWindowText(_T("0"));
}

void SerialDlg::OnButtonOpen() 
{
	UpdateData();
	
	WORD item_id[] = {
		IDC_BUTTON_DCB ,
		IDC_CHECK_EN_OVERLAPPED ,
		IDC_CHECK_EN_TX_BUFFER ,
		IDC_CHECK_EN_RX_BUFFER ,
		IDC_CHECK_EN_TX_THREAD ,
		IDC_CHECK_EN_RX_THREAD ,
		IDC_BUTTON_SEND_FILE,
		IDC_BUTTON_TIMEOUTS,
		IDC_BUTTON_PROPERTIES,
		IDC_CHECK_DTR,
		IDC_CHECK_BREAK,
		IDC_CHECK_RTS,
		IDC_CHECK_XON,
		IDC_BUTTON_SEND
	};
	
	if (!Comm_.IsOpen())
	{
		if (!Comm_.Open(m_iPort+1))
		{
			TCHAR szBuf[1024];
			wsprintf(szBuf, _T("打开 COM%d 失败, 代码:%d"), m_iPort+1, GetLastError());
			MessageBox(szBuf);
			return;
		}
		else
		{
			AfxGetMainWnd()->SendMessage(ON_COM_OPEN, (WPARAM)Comm_.GetPort());
			SendDlgItemMessage(IDC_BUTTON_OPEN, WM_SETTEXT, 0, (LPARAM)_T("关闭"));
			m_Lamp.SetIcon(hIcons[1]);
			
			for (int i = 0; i<sizeof(item_id)/sizeof(WORD); i++) 
				GetDlgItem(item_id[i])->EnableWindow(!GetDlgItem(item_id[i])->IsWindowEnabled());
		}

		SetState(); 
	}
	else
	{
		Comm_.Close();
		AfxGetMainWnd()->SendMessage(ON_COM_CLOSE, (WPARAM)iInstance_);
		SendDlgItemMessage(IDC_BUTTON_OPEN, WM_SETTEXT, 0, (LPARAM)_T("打开"));
		m_Lamp.SetIcon(hIcons[0]);
		
		for (int i = 0; i<sizeof(item_id)/sizeof(WORD); i++) 
			GetDlgItem(item_id[i])->EnableWindow(!GetDlgItem(item_id[i])->IsWindowEnabled());
	}
	
	
	UpdateData(FALSE);
	
	Invalidate();
	
}

void SerialDlg::SetState()
{
	if (!Comm_.IsOpen())
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
#ifdef UNICODE
	temp[wcstombs(temp, (LPCTSTR)m_sBaudrate, m_sBaudrate.GetLength())] = 0;
#else
	strcpy(temp, (LPCTSTR)m_sBaudrate);
#endif
	
	DCB* pDcb = Comm_.GetState();
	pDcb->BaudRate = atoi(temp);
	pDcb->ByteSize = ByteSizes[m_iBitsSize];
	pDcb->StopBits = StopBits[m_iStopbits].dwStopBit;
	pDcb->Parity = Paritys[m_iParity].dwParity;
	//pDcb->
	
	DWORD dwBaudrate = atoi(temp);
	if (!Comm_.SetState(pDcb))
	{
		wsprintf(szBuf, _T("SetState(%d, %d, %d, %d), Fail. Code:%d"), 
			dwBaudrate, 
			Paritys[m_iParity].dwParity, 
			ByteSizes[m_iBitsSize], 
			StopBits[m_iStopbits].dwStopBit,
			GetLastError());
		MessageBox(szBuf);
		return;
	}
}

int Hex2Ascii(char* hex, char* ascii)
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
	
	return tlen;
}

int Ascii2Hex(char* ascii, char* hex)
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
//! 发送数据
void SerialDlg::OnButtonSend() 
{
	UpdateData();
	
	char* buffer = new char[m_sTx.GetLength()*2+1];
	
#ifdef UNICODE
	int len = wcstombs(buffer, (LPCTSTR)m_sTx, m_sTx.GetLength());
	buffer[len]= 0;
#else
	strcpy(buffer, (LPCTSTR)m_sTx);
	int len = m_sTx.GetLength();
#endif
	
	if (m_bTxHex)
	{
		int HexCnt = Hex2Ascii(buffer, buffer);
		
		if (Comm_.IsOverlappedMode() || Comm_.IsTxBufferMode())
			Comm_.Write(buffer, HexCnt);
		else
		{
			for (int i = 0; i<HexCnt; i++ )
				i += Comm_.Write(buffer + i, HexCnt - i);
		}
	}
	else
	{	//! 重叠模式下或者缓冲区模式下 可以不检查输出返回值，并直接输出
		if (Comm_.IsOverlappedMode() || Comm_.IsTxBufferMode())
			Comm_.Write(buffer);
		else
		{//! 阻塞非缓冲区模式 必须检查返回值，确保数据完全发送出
			for (int i = 0; i<len; i++ )
				i += Comm_.Write(buffer + i, len - i);
		}
	}
	
	delete[] buffer;
}

void SerialDlg::OnButtonClearRx() 
{
	UpdateData();
	
	Comm_.Input().SafeClear();
	m_sRx = _T("");
	Rx_.Clear();
	UpdateData(FALSE);
}

void SerialDlg::OnButtonClearTx() 
{
	UpdateData();
	m_sTx = _T("");
	UpdateData(FALSE);
}

void SerialDlg::OnCheckAutoSend() 
{
	UpdateData();
	
	if (m_bAutoSend)
	{
		if (m_uSendTimer)
			KillTimer(2);

		m_uSendTimer = SetTimer(2, m_uCycle, NULL);
	}
	else
	{
		KillTimer(2);
		m_uSendTimer = 0;
	}
}

void SerialDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect rc;
	GetParent()->GetClientRect(&rc);
	rc.top += 25, rc.bottom -= 3, rc.right -= 3, rc.left += 3;
	MoveWindow(&rc, TRUE);
}

void SerialDlg::OnButtonDcb() 
{
	DcbDlg Dlg(&Comm_, this);
	Dlg.DoModal();
}

void SerialDlg::OnCheckEnRxBuffer() 
{
	Comm_.SetOption(Comm_.GetOption() | CnComm::EN_RX_BUFFER);
}

void SerialDlg::OnCheckEnTxThread() 
{
	Comm_.SetOption(Comm_.GetOption() | CnComm::EN_TX_THREAD);
}

void SerialDlg::OnCheckEnOverlapped() 
{
	Comm_.SetOption(Comm_.GetOption() | CnComm::EN_OVERLAPPED);
}

void SerialDlg::OnCheckEnTxBuffer() 
{
	Comm_.SetOption(Comm_.GetOption() | CnComm::EN_TX_BUFFER);
}

void SerialDlg::OnCheckEnRxThread() 
{
	Comm_.SetOption(Comm_.GetOption() | CnComm::EN_RX_THREAD);
}

void SerialDlg::OnEditchangeComboBaudrate() 
{
	if (Comm_.IsOpen())
		SetState();
}

void SerialDlg::OnEditchangeComboParity() 
{
	if (Comm_.IsOpen())
		SetState();
}

void SerialDlg::OnEditchangeComboBitssize() 
{
	if (Comm_.IsOpen())
		SetState();
}

void SerialDlg::OnEditchangeComboStopbits() 
{
	if (Comm_.IsOpen())
		SetState();
}

void SerialDlg::OnEditchangeComboFlowCtrl() 
{
	if (Comm_.IsOpen())
		SetState();
}

void SerialDlg::OnCheckRxHex() 
{
	// TODO: Add your control notification handler code here
	
}

void SerialDlg::OnCheckTxHex() 
{
	// TODO: Add your control notification handler code here
	
}

void SerialDlg::OnButtonTimeouts() 
{
	TimeoutsDlg dlg(&Comm_, this);
	dlg.DoModal();
}

void SerialDlg::OnButtonLoadFile() 
{
	UpdateData();
	
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		CFile file;
		if (file.Open(dlg.GetFileName(), CFile::modeRead))
		{
			m_sTx = "";
			
			CFileStatus fs;
			file.GetStatus(fs);
			file.Read(m_sTx.GetBufferSetLength(fs.m_size+1), fs.m_size);
			m_sTx.ReleaseBuffer();
			UpdateData(FALSE);
		}
		else
		{
			CString str = "无法打开文件:";
			str += dlg.GetFileName();
			MessageBox(str);
		}
	}
}

void SerialDlg::OnCheckDtr() 
{
	UpdateData();
	Comm_.SetDTR(m_bDtr);
}

void SerialDlg::OnCheckBreak() 
{
	UpdateData();
	Comm_.SetBreak(m_bBreak);
}

void SerialDlg::OnCheckRts() 
{
	UpdateData();
	Comm_.SetRTS(m_bRts);
}

void SerialDlg::OnCheckXon() 
{
	UpdateData();
	Comm_.SetX(m_bXon);	
}

void SerialDlg::OnButtonProperties() 
{
	PropertiesDlg dlg(&Comm_, this);
	dlg.DoModal();
}

void SerialDlg::OnButtonSendFile() 
{
	UpdateData(TRUE);
	
	if (m_TxFileName == "")
	{
		CFileDialog dlg(TRUE);
		if (dlg.DoModal() == IDOK)
		{
			m_TxFileName = dlg.GetPathName();
			UpdateData(FALSE);
		}
	}
	
	if (m_TxFileName != "")
	{
		char buffer[1024];
		CFile file;
		if (file.Open(m_TxFileName, CFile::modeRead))
		{
			UINT uLen;
			do 
			{
				uLen = file.Read(buffer, 1024);
				if (uLen)
				{
					if (Comm_.IsOverlappedMode() || Comm_.IsTxBufferMode())
						Comm_.Write(buffer, uLen);
					else
					{
						for (int i = 0; i<uLen; i++ )
							i += Comm_.Write(buffer + i, uLen - i);
					}
				}
			} while(uLen == 1024);
		}
		else
		{
			CString str = "无法打开文件:";
			str += m_TxFileName;
			MessageBox(str);
		}
	}
	
}

void SerialDlg::OnButtonBrowerTx() 
{
	UpdateData();

	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		m_TxFileName = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void SerialDlg::OnButtonBrowerRx() 
{
	UpdateData();
	
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		m_RxFileName = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void SerialDlg::OnChangeEditCycle() 
{
	UpdateData();

	OnCheckAutoSend();
}

void SerialDlg::OnCheckSave() 
{
	UpdateData();

	if (m_bAutoSave && m_RxFileName == "")
		OnButtonBrowerRx();
}
