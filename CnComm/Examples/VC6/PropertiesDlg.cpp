// PropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VC6.h"
#include "PropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropertiesDlg dialog


PropertiesDlg::PropertiesDlg(CnComm *p, CWnd* pParent /*=NULL*/)
	: CDialog(PropertiesDlg::IDD, pParent), pComm_(p)
{
	//{{AFX_DATA_INIT(PropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void PropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropertiesDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_EDIT_wPacketLength,				dwPacketLength				);
	DDX_Text(pDX, IDC_EDIT_wPacketVersion, 				dwPacketVersion				);
	DDX_Text(pDX, IDC_EDIT_dwServiceMask, 				dwServiceMask				);
	DDX_Text(pDX, IDC_EDIT_dwMaxTxQueue, 				dwMaxTxQueue				);
	DDX_Text(pDX, IDC_EDIT_dwMaxRxQueue, 				dwMaxRxQueue				);
	DDX_Text(pDX, IDC_EDIT_dwMaxBaud, 					dwMaxBaud					);
	DDX_Text(pDX, IDC_EDIT_dwProvSubType, 				dwProvSubType				);
	DDX_Text(pDX, IDC_EDIT_dwProvCapabilities, 			dwProvCapabilities			);
	DDX_Text(pDX, IDC_EDIT_dwSettableParams, 			dwSettableParams			);
	DDX_Text(pDX, IDC_EDIT_dwSettableBaud, 				dwSettableBaud				);
	DDX_Text(pDX, IDC_EDIT_wSettableData, 				dwSettableData				);
	DDX_Text(pDX, IDC_EDIT_wSettableStopParity, 		dwSettableStopParity		);
	DDX_Text(pDX, IDC_EDIT_dwCurrentTxQueue, 			dwCurrentTxQueue			);
	DDX_Text(pDX, IDC_EDIT_dwCurrentRxQueue, 			dwCurrentRxQueue			);
	DDX_Text(pDX, IDC_EDIT_dwProvSpec1, 				dwProvSpec1					);
	DDX_Text(pDX, IDC_EDIT_dwProvSpec2, 				dwProvSpec2					);
	DDX_Text(pDX, IDC_EDIT_dwRequiredSize, 				dwRequiredSize				);
	DDX_Text(pDX, IDC_EDIT_dwDevSpecificOffset, 		dwDevSpecificOffset			);
	DDX_Text(pDX, IDC_EDIT_dwDevSpecificSize, 			dwDevSpecificSize			);
	DDX_Text(pDX, IDC_EDIT_dwModemProviderVersion, 		dwModemProviderVersion		);
	DDX_Text(pDX, IDC_EDIT_dwModemManufacturerOffset, 	dwModemManufacturerOffset	);
	DDX_Text(pDX, IDC_EDIT_dwModemManufacturerSize, 	dwModemManufacturerSize		);
	DDX_Text(pDX, IDC_EDIT_dwModemModelOffset, 			dwModemModelOffset			);
	DDX_Text(pDX, IDC_EDIT_dwModemModelSize, 			dwModemModelSize			);
	DDX_Text(pDX, IDC_EDIT_dwModemVersionOffset, 		dwModemVersionOffset		);
	DDX_Text(pDX, IDC_EDIT_dwModemVersionSize, 			dwModemVersionSize			);
	DDX_Text(pDX, IDC_EDIT_dwDialOptions, 				dwDialOptions				);
	DDX_Text(pDX, IDC_EDIT_dwCallSetupFailTimer, 		dwCallSetupFailTimer		);
	DDX_Text(pDX, IDC_EDIT_dwInactivityTimeout, 		dwInactivityTimeout			);
	DDX_Text(pDX, IDC_EDIT_dwSpeakerVolume, 			dwSpeakerVolume				);
	DDX_Text(pDX, IDC_EDIT_dwSpeakerMode, 				dwSpeakerMode				);
	DDX_Text(pDX, IDC_EDIT_dwModemOptions, 				dwModemOptions				);
	DDX_Text(pDX, IDC_EDIT_dwMaxDTERate, 				dwMaxDTERate				);
	DDX_Text(pDX, IDC_EDIT_dwMaxDCERate, 				dwMaxDCERate				);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(PropertiesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropertiesDlg message handlers

BOOL PropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LPCOMMPROP lpCP = pComm_->GetProperties();
	if (lpCP)
	{

		dwPacketLength			=	lpCP->wPacketLength              	  ;
		dwPacketVersion			=	lpCP->wPacketVersion				  ;  
		dwServiceMask			=	lpCP->dwServiceMask                   ;    
		dwMaxTxQueue      		=	lpCP->dwMaxTxQueue                    ;
		dwMaxRxQueue			=	lpCP->dwMaxRxQueue					  ; 
		dwMaxBaud         		=	lpCP->dwMaxBaud                       ; 
		dwProvSubType     		=	lpCP->dwProvSubType                   ; 
		dwProvCapabilities		=	lpCP->dwProvCapabilities              ; 
		dwSettableParams		=	lpCP->dwSettableParams                ;  
		dwSettableBaud    		=	lpCP->dwSettableBaud                  ; 
		dwSettableData     		=	lpCP->wSettableData                   ; 
		dwSettableStopParity 	=	lpCP->wSettableStopParity             ; 
		dwCurrentTxQueue  		=	lpCP->dwCurrentTxQueue                ; 
		dwCurrentRxQueue  		=	lpCP->dwCurrentRxQueue                ; 
		dwProvSpec1       		=	lpCP->dwProvSpec1                     ; 
		dwProvSpec2       		=	lpCP->dwProvSpec2                     ; 

		MODEMDEVCAPS* lp = ((MODEMDEVCAPS *)(lpCP->wcProvChar));
		dwRequiredSize				 = 	lp->dwRequiredSize				  ;			
		dwDevSpecificOffset			 = 	lp->dwDevSpecificOffset			  ;
		dwDevSpecificSize			 = 	lp->dwDevSpecificSize			  ;
		dwModemProviderVersion 		 = 	lp->dwModemProviderVersion        ;
		dwModemManufacturerOffset	 = 	lp->dwModemManufacturerOffset	  ;
		dwModemManufacturerSize		 = 	lp->dwModemManufacturerSize       ;
		dwModemModelOffset     		 = 	lp->dwModemModelOffset            ;
		dwModemModelSize       		 = 	lp->dwModemModelSize              ;
		dwModemVersionOffset   		 = 	lp->dwModemVersionOffset          ;
		dwModemVersionSize     		 = 	lp->dwModemVersionSize            ;
		dwDialOptions          		 = 	lp->dwDialOptions                 ;
		dwCallSetupFailTimer   		 = 	lp->dwCallSetupFailTimer          ;
		dwInactivityTimeout    		 = 	lp->dwInactivityTimeout           ;
		dwSpeakerVolume        		 = 	lp->dwSpeakerVolume               ;
		dwSpeakerMode          		 = 	lp->dwSpeakerMode                 ;
		dwModemOptions         		 = 	lp->dwModemOptions                ;
		dwMaxDTERate				 = 	lp->dwMaxDTERate				  ;          
		dwMaxDCERate 				 = 	lp->dwMaxDCERate				  ;
	}

	UpdateData(FALSE);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
