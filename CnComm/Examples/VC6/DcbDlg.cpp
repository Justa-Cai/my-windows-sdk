// DcbDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VC6.h"
#include "DcbDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DcbDlg dialog


DcbDlg::DcbDlg(CnComm *p, CWnd* pParent /*=NULL*/)
	: CDialog(DcbDlg::IDD, pParent), pComm_(p)
{
	//{{AFX_DATA_INIT(DcbDlg)
	m_uBaudrate = 0;
	m_uByteSize = 0;
	m_uDcbLength = 0;
	m_bAbortOnError = FALSE;
	m_bBinary = FALSE;
	m_DsrSensitivity = FALSE;
	m_bParity = FALSE;
	m_bTxContinueOnXoff = FALSE;
	m_btStopBits = 0;
	m_btParity = 0;
	m_bOutxCtsFlow = FALSE;
	m_btRtsControl = 0;
	m_btDtrControl = 0;
	m_bErrorChar = FALSE;
	m_bInx = FALSE;
	m_bNull = FALSE;
	m_bOutX = FALSE;
	m_chXoffChar = 0;
	m_btEofChar = 0;
	m_btErrorChar = 0;
	m_btEvtChar = 0;
	m_btXoffLim = 0;
	m_btXonChar = 0;
	m_btXonLim = 0;
	//}}AFX_DATA_INIT
}


void DcbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DcbDlg)
	DDX_Text(pDX, IDC_EDIT_BaudRate, m_uBaudrate);
	DDX_Text(pDX, IDC_EDIT_ByteSize, m_uByteSize);
	DDX_Text(pDX, IDC_EDIT_DCBlength, m_uDcbLength);
	DDX_Text(pDX, IDC_EDIT_fAbortOnError, m_bAbortOnError);
	DDX_Text(pDX, IDC_EDIT_fBinary, m_bBinary);
	DDX_Text(pDX, IDC_EDIT_fDsrSensitivity, m_DsrSensitivity);
	DDX_Text(pDX, IDC_EDIT_fParity, m_bParity);
	DDX_Text(pDX, IDC_EDIT_fTXContinueOnXoff, m_bTxContinueOnXoff);
	DDX_Text(pDX, IDC_EDIT_StopBits, m_btStopBits);
	DDX_Text(pDX, IDC_EDIT_Parity, m_btParity);
	DDX_Text(pDX, IDC_EDIT_fOutxCtsFlow, m_bOutxCtsFlow);
	DDX_Text(pDX, IDC_EDIT_fRtsControl, m_btRtsControl);
	DDX_Text(pDX, IDC_EDIT_fDtrControl, m_btDtrControl);
	DDX_Text(pDX, IDC_EDIT_fErrorChar, m_bErrorChar);
	DDX_Text(pDX, IDC_EDIT_fInX, m_bInx);
	DDX_Text(pDX, IDC_EDIT_fNull, m_bNull);
	DDX_Text(pDX, IDC_EDIT_fOutX, m_bOutX);
	DDX_Text(pDX, IDC_EDIT_XoffChar, m_chXoffChar);
	DDX_Text(pDX, IDC_EDIT_EofChar, m_btEofChar);
	DDX_Text(pDX, IDC_EDIT_ErrorChar, m_btErrorChar);
	DDX_Text(pDX, IDC_EDIT_EvtChar, m_btEvtChar);
	DDX_Text(pDX, IDC_EDIT_XoffLim, m_btXoffLim);
	DDX_Text(pDX, IDC_EDIT_XonChar, m_btXonChar);
	DDX_Text(pDX, IDC_EDIT_XonLim, m_btXonLim);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DcbDlg, CDialog)
	//{{AFX_MSG_MAP(DcbDlg)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DcbDlg message handlers

BOOL DcbDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pDcb_ = pComm_->GetState();

	m_uDcbLength		= pDcb_->DCBlength			; 
	m_uBaudrate			= pDcb_->BaudRate			; 
	m_bBinary			= pDcb_->fBinary			; 
	m_bParity			= pDcb_->fParity			; 
	m_bOutxCtsFlow		= pDcb_->fOutxCtsFlow		; 
	m_btDtrControl		= pDcb_->fDtrControl		; 
	m_btRtsControl		= pDcb_->fRtsControl		; 
	m_bInx				= pDcb_->fInX				; 
	m_bOutX				= pDcb_->fOutX				; 
	m_DsrSensitivity	= pDcb_->fDsrSensitivity	; 
	m_bTxContinueOnXoff	= pDcb_->fTXContinueOnXoff	; 
	m_btErrorChar		= pDcb_->EofChar			; 
	m_btEvtChar			= pDcb_->EvtChar			; 
	m_bNull				= pDcb_->fNull				; 
	m_bAbortOnError		= pDcb_->fAbortOnError		; 
	m_btXonLim			= pDcb_->XoffLim			; 
	m_btXonChar			= pDcb_->XonChar			; 
	m_btXoffLim			= pDcb_->XoffLim			; 
	m_chXoffChar		= pDcb_->XoffChar			; 
	m_btEofChar			= pDcb_->EofChar			; 
	m_btErrorChar		= pDcb_->ErrorChar			; 
	m_uByteSize			= pDcb_->ByteSize			; 
	m_btStopBits		= pDcb_->StopBits			; 
	m_btParity			= pDcb_->Parity				; 

	UpdateData(FALSE);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DcbDlg::OnButtonSet() 
{
	UpdateData();

	pDcb_->DCBlength		 = m_uDcbLength			;
	pDcb_->BaudRate			 = m_uBaudrate			;
	pDcb_->fBinary			 = m_bBinary			;
	pDcb_->fParity			 = m_bParity			;
	pDcb_->fOutxCtsFlow		 = m_bOutxCtsFlow		;
	pDcb_->fDtrControl		 = m_btDtrControl		;
	pDcb_->fRtsControl		 = m_btRtsControl		;
	pDcb_->fInX				 = m_bInx				;
	pDcb_->fOutX			 = m_bOutX				;
	pDcb_->fDsrSensitivity	 = m_DsrSensitivity		;
	pDcb_->fTXContinueOnXoff = m_bTxContinueOnXoff	;
	pDcb_->EofChar			 = m_btErrorChar		;
	pDcb_->EvtChar			 = m_btEvtChar			;
	pDcb_->fNull			 = m_bNull				;
	pDcb_->fAbortOnError	 = m_bAbortOnError		;
	pDcb_->XoffLim			 = m_btXonLim			;
	pDcb_->XonChar			 = m_btXonChar			;
	pDcb_->XoffLim			 = m_btXoffLim			;
	pDcb_->XoffChar			 = m_chXoffChar			;
	pDcb_->EofChar			 = m_btEofChar			;
	pDcb_->ErrorChar		 = m_btErrorChar		;
	pDcb_->ByteSize			 = m_uByteSize			;
	pDcb_->StopBits			 = m_btStopBits			;
	pDcb_->Parity			 = m_btParity			;

	if (pComm_->SetState(pDcb_))
		AfxMessageBox("设置成功!");
	else
		AfxMessageBox("设置失败!");
}
