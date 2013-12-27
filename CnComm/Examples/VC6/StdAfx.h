// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__51D93A4E_C7C5_44AC_BBEF_9F9B2F7D3817__INCLUDED_)
#define AFX_STDAFX_H__51D93A4E_C7C5_44AC_BBEF_9F9B2F7D3817__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define CN_COMM_MFC_EXCEPTION
#include "..\..\Src\CnComm.h"
#include <vector>
#include <algorithm>
using namespace std;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__51D93A4E_C7C5_44AC_BBEF_9F9B2F7D3817__INCLUDED_)
