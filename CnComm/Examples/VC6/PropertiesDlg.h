#if !defined(AFX_PROPERTIESDLG_H__C9A7983E_8C4D_4BCD_8A7E_C5A5CECE62EE__INCLUDED_)
#define AFX_PROPERTIESDLG_H__C9A7983E_8C4D_4BCD_8A7E_C5A5CECE62EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PropertiesDlg dialog

class PropertiesDlg : public CDialog
{
// Construction
public:
	PropertiesDlg(CnComm *p, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PropertiesDlg)
	enum { IDD = IDD_DIALOG_PROPERTIES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CnComm	*pComm_;

	DWORD dwPacketLength;       
    DWORD dwPacketVersion;      
    DWORD dwServiceMask;       
    DWORD dwMaxTxQueue;        
    DWORD dwMaxRxQueue;        
    DWORD dwMaxBaud;           
    DWORD dwProvSubType;       
    DWORD dwProvCapabilities;  
    DWORD dwSettableParams;    
    DWORD dwSettableBaud;      
    DWORD dwSettableData;       
    DWORD dwSettableStopParity; 
    DWORD dwCurrentTxQueue;    
    DWORD dwCurrentRxQueue;    
    DWORD dwProvSpec1;         
    DWORD dwProvSpec2;         
    DWORD dwRequiredSize;			
    DWORD dwDevSpecificOffset;		
    DWORD dwDevSpecificSize;		
    DWORD dwModemProviderVersion;   
    DWORD dwModemManufacturerOffset;
    DWORD dwModemManufacturerSize;  
    DWORD dwModemModelOffset;       
    DWORD dwModemModelSize;         
    DWORD dwModemVersionOffset;     
    DWORD dwModemVersionSize;       
    DWORD dwDialOptions;            
    DWORD dwCallSetupFailTimer;     
    DWORD dwInactivityTimeout;      
    DWORD dwSpeakerVolume;          
    DWORD dwSpeakerMode;            
    DWORD dwModemOptions;           
    DWORD dwMaxDTERate;             
    DWORD dwMaxDCERate;           


	// Generated message map functions
	//{{AFX_MSG(PropertiesDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESDLG_H__C9A7983E_8C4D_4BCD_8A7E_C5A5CECE62EE__INCLUDED_)
