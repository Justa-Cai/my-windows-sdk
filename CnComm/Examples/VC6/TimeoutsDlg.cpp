// TimeoutsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VC6.h"
#include "TimeoutsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TimeoutsDlg dialog


TimeoutsDlg::TimeoutsDlg(CnComm *p, CWnd* pParent /*=NULL*/)
	: CDialog(TimeoutsDlg::IDD, pParent), pComm_(p)
{
	//{{AFX_DATA_INIT(TimeoutsDlg)
	m_uRIT	= 0;
	m_uRTTC = 0;
	m_uRTTM = 0;
	m_uWTTC = 0;
	m_uWTTM = 0;
	//}}AFX_DATA_INIT
}


void TimeoutsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TimeoutsDlg)
	DDX_Text(pDX, IDC_EDIT_RIT, m_uRIT);
	DDX_Text(pDX, IDC_EDIT_RTTC, m_uRTTC);
	DDX_Text(pDX, IDC_EDIT_RTTM, m_uRTTM);
	DDX_Text(pDX, IDC_EDIT_WTTC, m_uWTTC);
	DDX_Text(pDX, IDC_EDIT_WTTM, m_uWTTM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TimeoutsDlg, CDialog)
	//{{AFX_MSG_MAP(TimeoutsDlg)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TimeoutsDlg message handlers

BOOL TimeoutsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	pCo_	= pComm_->GetTimeouts();
	m_uRIT	= pCo_->ReadIntervalTimeout;
	m_uRTTC = pCo_->ReadTotalTimeoutConstant;
	m_uRTTM = pCo_->ReadTotalTimeoutMultiplier;
	m_uWTTC = pCo_->WriteTotalTimeoutConstant;
	m_uWTTM = pCo_->WriteTotalTimeoutMultiplier;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void TimeoutsDlg::OnButtonSet() 
{
	UpdateData();

	pCo_->ReadIntervalTimeout			= m_uRIT;
	pCo_->ReadTotalTimeoutConstant		= m_uRTTC;
	pCo_->ReadTotalTimeoutMultiplier	= m_uRTTM;
	pCo_->WriteTotalTimeoutConstant		= m_uWTTC;
	pCo_->WriteTotalTimeoutMultiplier	= m_uWTTM;

	if (pComm_->SetTimeouts())
		AfxMessageBox("设置成功!");
	else
		AfxMessageBox("设置失败!");
}
