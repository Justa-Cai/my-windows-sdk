// VC6.h : main header file for the VC6 application
//

#if !defined(AFX_VC6_H__BAE34B0B_BB29_4921_9B11_1988EA7CB084__INCLUDED_)
#define AFX_VC6_H__BAE34B0B_BB29_4921_9B11_1988EA7CB084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CnCommApp:
// See VC6.cpp for the implementation of this class
//

class CnCommApp : public CWinApp
{
public:
	CnCommApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CnCommApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CnCommApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VC6_H__BAE34B0B_BB29_4921_9B11_1988EA7CB084__INCLUDED_)
