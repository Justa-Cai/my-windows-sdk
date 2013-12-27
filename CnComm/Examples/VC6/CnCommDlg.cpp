/*! \file CnCommDlg.cpp 
	\brief CnComm范例程序(VC6) 串口调试器
*/

#include "stdafx.h"
#include "VC6.h"
#include "CnCommDlg.h"
#include "SerialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CnCommDlg dialog

CnCommDlg::CnCommDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CnCommDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CnCommDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	uInstanceId_ = 0;
	uLastSel_ = 0;
	uCurSel_ = 0;
}

void CnCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CnCommDlg)
	DDX_Control(pDX, IDC_TAB, m_TabCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CnCommDlg, CDialog)
	//{{AFX_MSG_MAP(CnCommDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_MESSAGE(ON_NEW_INSTANCE, OnNewInstance)
	ON_MESSAGE(ON_FREE_INSTANCE, OnFreeInstance)
	ON_MESSAGE(ON_COM_OPEN, OnComOpen)
	ON_MESSAGE(ON_COM_CLOSE, OnComClose)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_SIZING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CnCommDlg message handlers

BOOL CnCommDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	NewInstance();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CnCommDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CnCommDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CnCommDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CnCommDlg::NewInstance()
{
	CString str;
	str.Format(_T("实例%d"), uInstanceId_ + 1);
	m_TabCtrl.InsertItem(m_TabCtrl.GetItemCount(), str);
	
	SerialDlg *pDlg = new SerialDlg(uInstanceId_);
	SerialDlgArray_.push_back(pDlg);

	pDlg->Create(IDD_DIALOG_CN_COMM, &m_TabCtrl);

	CRect rc;
	m_TabCtrl.GetClientRect(&rc);
	rc.top += 0, rc.bottom -= 3, rc.right -= 3, rc.left += 3;
	pDlg->MoveWindow(&rc, FALSE);
	pDlg->ShowWindow(SW_SHOW);

	m_TabCtrl.SetCurSel(m_TabCtrl.GetItemCount() - 1);

	uCurSel_ = m_TabCtrl.GetCurSel();
	for (int i = 0; i<SerialDlgArray_.size(); i++)
		SerialDlgArray_[i]->ShowWindow(SW_HIDE );
	SerialDlgArray_[uCurSel_]->ShowWindow(SW_SHOW);

	uInstanceId_ ++;
}

void CnCommDlg::FreeInstance(SerialDlg * pDlg)
{
	if (m_TabCtrl.GetItemCount() == 1)
	{
		AfxMessageBox("至少保留一个实例!");
		return;
	}

	vector<SerialDlg* >::iterator iter;
	iter = find(SerialDlgArray_.begin(), SerialDlgArray_.end(), pDlg);
	SerialDlgArray_.erase(iter);
	int iCur = m_TabCtrl.GetCurSel();
	m_TabCtrl.SetCurSel(uLastSel_);
	m_TabCtrl.DeleteItem(iCur);
	if (m_TabCtrl.GetCurSel() == -1)
		m_TabCtrl.SetCurSel(0);

	uCurSel_ = m_TabCtrl.GetCurSel();
	for (int i = 0; i<SerialDlgArray_.size(); i++)
		SerialDlgArray_[i]->ShowWindow(SW_HIDE );
	SerialDlgArray_[uCurSel_]->ShowWindow(SW_SHOW);

	pDlg->DestroyWindow();
	delete pDlg;
}

void CnCommDlg::OnDestroy() 
{
	for (int i = 0; i<SerialDlgArray_.size(); i++)
	{
		SerialDlgArray_[i]->DestroyWindow();
		delete SerialDlgArray_[i];
	}

	CDialog::OnDestroy();
}

LRESULT CnCommDlg::OnNewInstance(WPARAM, LPARAM)
{
	NewInstance();
	return TRUE;
}

LRESULT CnCommDlg::OnFreeInstance(WPARAM wParam, LPARAM lParam)
{
	FreeInstance((SerialDlg *)lParam);
	return TRUE;
}

void CnCommDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(&rc);
	
	rc.top += 5, rc.bottom -= 5, rc.right -= 5, rc.left += 5;
	m_TabCtrl.MoveWindow(&rc);

	m_TabCtrl.GetClientRect(&rc);
	rc.top += 0, rc.bottom -= 3, rc.right -= 3, rc.left += 3;
	for (int i = 0; i<SerialDlgArray_.size(); i++)
		SerialDlgArray_[i]->MoveWindow(&rc, TRUE);
}

void CnCommDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	uLastSel_ = uCurSel_;
	uCurSel_ = m_TabCtrl.GetCurSel();

	for (int i = 0; i<SerialDlgArray_.size(); i++)
		SerialDlgArray_[i]->ShowWindow(SW_HIDE );
	SerialDlgArray_[uCurSel_]->ShowWindow(SW_SHOW);

	*pResult = 0;
}

LRESULT CnCommDlg::OnComOpen(WPARAM wParam, LPARAM lParam)
{
	TCHAR szText[128];
	TCITEM item = {0};
	int iSel;

	iSel = m_TabCtrl.GetCurSel(); 
	item.mask |= TCIF_TEXT ;  
	item.pszText = szText;
	item.cchTextMax = 128;
	wsprintf(szText, _T("COM%d"), (int)wParam);
	m_TabCtrl.SetItem(iSel, &item);

	return TRUE;
}

LRESULT CnCommDlg::OnComClose(WPARAM wParam, LPARAM)
{
	TCHAR szText[128];
	TCITEM item = {0};
	int iSel;

	iSel = m_TabCtrl.GetCurSel(); 
	item.mask |= TCIF_TEXT ;  
	item.pszText = szText;
	item.cchTextMax = 128;
	wsprintf(szText, _T("实例%d"), (int)wParam + 1);
	m_TabCtrl.SetItem(iSel, &item);

	return TRUE;
}


void CnCommDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CDialog::OnSizing(fwSide, pRect);
	
	// TODO: Add your message handler code here
	
}
