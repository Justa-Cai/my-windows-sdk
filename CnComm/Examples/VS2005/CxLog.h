/*
Destination: 文件型日志封装类
Author: 吴健
Date: 2008-6-10
*/

#ifndef _CX_LOG_H_
#define	_CX_LOG_H_

#if defined(_MSC_VER)
	#pragma warning(disable: 4530)
#endif

#include <stdio.h>
#include <time.h>
#include <assert.h>

#include <list>
#include <iterator>
#include <stdexcept>
#include <exception>

#if !defined(UNDER_CE) && (defined(_MSC_VER)||defined(__INTEL_COMPILER))
	#include <comdef.h>
#endif

#include <tchar.h>
#include <windows.h>

#ifndef UNDER_CE
	#include <process.h>
#endif


//////////////////////////////////////////////////////////////////////////
///文件型多线程日志类 class for log file
//定义格式化输出
#ifndef CX_LOG_DEFAULT_FORMAT
	#if (defined(_MSC_VER) && _MSC_VER<=1310) || defined(__BORLANDC__)  || (defined(__GNUC__) && defined(_UNICODE))
		#define CX_LOG_DEFAULT_FORMAT _T("%s %s %s(%d): %s\r\n")
	#else
		#define CX_LOG_DEFAULT_FORMAT _T("%s %s %s(%s, %d): %s\r\n")
	#endif
#endif
//定义日志类的扩展名
#ifndef CX_LOG_EXT_NAME
	#define CX_LOG_EXT_NAME _T(".log")
#endif
//定义格式化输出的缓冲区大小
#ifndef CX_LOG_DEFAULT_FORMAT_SIZE
	#define CX_LOG_DEFAULT_FORMAT_SIZE 2048
#endif
//定义是否替换控制用ASCII
#ifndef CX_LOG_REPLACE_CONTROL
	#define CX_LOG_REPLACE_CONTROL true
#endif
//定义控制用ASCII的默认替换字符
#ifndef CX_LOG_REPLACE_ASCII
	#define CX_LOG_REPLACE_ASCII _T(' ')
#endif
//定义窗口通知消息
#ifndef WM_CX_LOG
	#define WM_CX_LOG WM_USER + 2200
#endif

class CxLog
{
public:
	enum EnumType{
		CX_LOG_MESSAGE = 0,
		CX_LOG_WARNING,
		CX_LOG_EXCEPTION,
		CX_LOG_ERROR
	};

	static CxLog& Instance()
	{
		static bool alive = false;
		static CxLog log(alive);

		if(!alive)
		{
			OutputDebugString(_T("CxLog has destroy!"));
			throw std::runtime_error("CxLog has destroy!");
		}

		return log;
	}

	struct Item
	{
		SYSTEMTIME _Time;//>time stamp
		TCHAR _szTime[24];

		LPTSTR _szSrc;//>source file name
		LPTSTR _szFunc;//>founction name
		ULONG _uLine;//>line number

		EnumType _eType;//
		LPTSTR _szDesc;//>content

		LPBYTE _pBin;//>binary data szBuffer
		ULONG _uBinSize;//>the size of binary data szBuffer
		//BOOL _bLog;//>complete the log

		Item() 
		{
			InitItem(NULL, NULL, 0, CX_LOG_MESSAGE, NULL, NULL, 0);
		}

		Item(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType eType, 
			LPCTSTR szDesc, LPVOID pBin = NULL, ULONG uSize = 0) 
		{
			InitItem(szSrc, szFunc, uLine, eType, szDesc, pBin, uSize);
		}

		~Item()
		{
			ReleaseStringBuffer(_szSrc);	
			ReleaseStringBuffer(_szFunc);
			ReleaseStringBuffer(_szDesc);
			
			if(_pBin)
			{
				delete []_pBin;
				_pBin = NULL;
			}
		}

		VOID InitItem(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType eType, 
			LPCTSTR szDesc, LPVOID pBin, ULONG uSize)
		{
			GetLocalTime(&_Time);
			wsprintf(_szTime, _T("%04d-%02d-%02d %02d:%02d:%02d.%03d"), 
				_Time.wYear,
				_Time.wMonth,
				_Time.wDay,
				_Time.wHour,
				_Time.wMinute,
				_Time.wSecond,
				_Time.wMilliseconds
				);

			_eType = eType;
			_uBinSize = _uLine = 0;
			_szSrc = _szFunc = _szDesc = NULL;
			_pBin = NULL;

			if(szSrc)
			{
				LPCTSTR p = szSrc;

			#ifndef CX_LOG_FULL_SOURCE_NAME
				p = szSrc + _tcslen(szSrc);
				while(p>szSrc && *p!=_T('\\'))
					p--;
				
				if(*p == _T('\\'))
					p++;
			#endif

				AllocStringBuffer(_szSrc, p);
				_uLine = uLine;
			}
			
			AllocStringBuffer(_szFunc, szFunc);
			AllocStringBuffer(_szDesc, szDesc);

			if(pBin && uSize)
			{
				_pBin = new BYTE[uSize];
				assert(_pBin);
				memcpy(_pBin, pBin, uSize);
				_uBinSize = uSize;
			}
		}

		VOID AllocStringBuffer(LPTSTR& szDest, LPCTSTR& szSrc)
		{
			if(szSrc)
			{
				ULONG uSize = _tcslen(szSrc) + 1;
				szDest = new TCHAR[uSize];
				assert(szDest);
				memcpy(szDest, szSrc, uSize*sizeof(TCHAR));
			}
		}

		VOID ReleaseStringBuffer(LPTSTR& lpDest)
		{
			if(lpDest)
			{
				delete []lpDest;
				lpDest = NULL;
			}
		}
		
		LPTSTR Format(LPTSTR szBuffer, ULONG uSize, ULONG* pSize = NULL)
		{
			static LPCTSTR szType[] = {_T("Message"), _T("Warning"), _T("Exception"), _T("Error")};

			if(!szBuffer)
				return szBuffer;

			int iLen;

			#if (defined(_MSC_VER) && _MSC_VER<=1310) || defined(__BORLANDC__) || (defined(__GNUC__) && defined(_UNICODE))
				iLen = _sntprintf(szBuffer, uSize, 
					CX_LOG_DEFAULT_FORMAT, 
					_szTime, szType[_eType], 
					_szSrc?_szSrc:_T(""), _uLine,
					_szDesc?_szDesc:_T(""));
			#else
				iLen = _sntprintf(szBuffer, uSize, 
					CX_LOG_DEFAULT_FORMAT, 
					_szTime, szType[_eType], 
					_szSrc?_szSrc:_T(""), _szFunc?_szFunc:_T(""), _uLine,
					_szDesc?_szDesc:_T(""));
			#endif

			if (iLen < 0)
				iLen = uSize - 1, szBuffer[uSize-1] = _T('\0'), szBuffer[uSize-2] = _T('\n'), szBuffer[uSize-3] = _T('\r');

			if(iLen>4 && !_tcsncmp(szBuffer+iLen-4, _T("\r\n"), 2))
				*(szBuffer+iLen-2) = TCHAR(NULL), iLen -= 2;
			
			if(pSize)
				*pSize = iLen;

			return szBuffer;
		}

		LPTSTR FormatBinary(LPTSTR szBuffer, ULONG uSize, ULONG* pSize = NULL,
			bool bReplace  = CX_LOG_REPLACE_CONTROL, 
			TCHAR chReplaceAscii = CX_LOG_REPLACE_ASCII)
		{
			return FormatBinary(_pBin, _uBinSize, szBuffer, uSize,  pSize, 
				bReplace, chReplaceAscii);
		}
		
		LPTSTR FormatBinary(LPVOID lpBin, ULONG uBinSize, 
			LPTSTR szBuffer, ULONG uSize,  ULONG* pSize = NULL, 
			bool bReplace  = CX_LOG_REPLACE_CONTROL, 
			TCHAR chReplaceAscii = CX_LOG_REPLACE_ASCII)
		{
			static TCHAR chHex[] = {
				_T('0'), _T('1'), _T('2'), _T('3'), _T('4'), _T('5'), _T('6'), _T('7'), 
				_T('8'), _T('9'), _T('A'), _T('B'), _T('C'), _T('D'), _T('E'), _T('F')};

			TCHAR temp[8+2+3*16+16+2+1];
			ULONG uActualSize = 0;

			if(!_pBin)
			{
				if(pSize)
					*pSize = uActualSize;
				return szBuffer;
			}

			if(!szBuffer)
			{
				uSize = ((_uBinSize>>4)+1)*(8+2+3*16+16+2)+1;
				szBuffer = new TCHAR[uSize];
				assert(szBuffer);
			}

			for(ULONG p = 0; p<_uBinSize && uActualSize<uSize-1; p += 16)
			{
				wsprintf(temp, _T("%08X: "), p); 

				int i;

				for(i = 0; i < 16; i++)
				{
					if(p+i<_uBinSize)
					{
						*(temp+8+2+3*i) = chHex[*(_pBin+p+i)>>4];
						*(temp+8+2+3*i+1) = chHex[*(_pBin+p+i)&0x0F];
						*(temp+8+2+3*i+2) = _T(' ');
					}
					else
						_tcscpy(temp+8+2+3*i, _T("   "));
				}

				for(i = 0; i < 16; i++)
				{
					if(p+i<_uBinSize)
						*(temp+8+2+3*16+i) = (bReplace&&_istcntrl(TCHAR(*(_pBin+p+i)))) ? chReplaceAscii : *(_pBin+p+i);
					else
						*(temp+8+2+3*16+i) = _T(' ');
				}
				
					
				_tcscpy(temp+8+2+3*16+16, _T("\r\n"));

				ULONG uLen = uSize-uActualSize-1>8+2+3*16+16+2? 8+2+3*16+16+2 : uSize-uActualSize-1;
				memcpy(szBuffer+uActualSize, temp, uLen*sizeof(TCHAR));
				uActualSize += uLen;
			}

			if(pSize)
				*pSize = uActualSize;

			szBuffer[uActualSize] = TCHAR(NULL);

			return szBuffer;
		}
	};

	LPTSTR GetLogFileName()
	{
	#if defined(CX_LOG_BY_DAY) || defined(CX_LOG_BY_WEEK) || defined(CX_LOG_BY_MONTH) || defined(CX_LOG_BY_YEAR)
		SYSTEMTIME st;
		TCHAR szName[64];

		GetLocalTime(&st);

		#if defined(CX_LOG_BY_DAY)
			wsprintf(szName, _T("\\%04d%02d%02d"), st.wYear, st.wMonth, st.wDay);
		#elif defined(CX_LOG_BY_WEEK)

			#define CX_LOG_M1	0
			#define CX_LOG_M2	31
			#define CX_LOG_M3	CX_LOG_M2 + 28
			#define CX_LOG_M4	CX_LOG_M3 + 31
			#define CX_LOG_M5	CX_LOG_M4 + 30
			#define CX_LOG_M6	CX_LOG_M5 + 31
			#define CX_LOG_M7	CX_LOG_M6 + 30
			#define CX_LOG_M8	CX_LOG_M7 + 31
			#define CX_LOG_M9	CX_LOG_M8 + 31
			#define CX_LOG_M10	CX_LOG_M9 + 30
			#define CX_LOG_M11	CX_LOG_M10 + 31
			#define CX_LOG_M12	CX_LOG_M11 + 30

			WORD wDays[] = {CX_LOG_M1, CX_LOG_M2, CX_LOG_M3, CX_LOG_M4, CX_LOG_M5, CX_LOG_M6, 
				CX_LOG_M7, CX_LOG_M8, CX_LOG_M9, CX_LOG_M10, CX_LOG_M11, CX_LOG_M12};

			if ( !(st.wYear&3) && (st.wYear%400==0 || st.wYear%100!=0) )
				wDays[2] += 1;

			wsprintf(szName, _T("\\%04d%02d"), st.wYear, (wDays[st.wMonth-1] + st.wDay - st.wDayOfWeek) / 7 + 1);
		#elif defined(CX_LOG_BY_MONTH)
			wsprintf(szName, _T("\\%04d%02d"), st.wYear, st.wMonth);
		#elif defined(CX_LOG_BY_YEAR)
			wsprintf(szName, _T("\\%04d%s"), st.wYear);
		#endif
		_tcscat(_tcscpy(_szFileName, GetFilePath()), szName); 
		_tcscat(_szFileName, CX_LOG_EXT_NAME); 
	#endif
		return _szFileName;
	}

	TCHAR* GetFilePath()
	{
		static TCHAR szFilePath[MAX_PATH];
		static BOOL bFlag = FALSE;

		if(!bFlag)
		{
			DWORD dwLen = ::GetModuleFileName(NULL, szFilePath, MAX_PATH);

			if(dwLen)
			{
				LPTSTR p = szFilePath + dwLen;
				while(p>szFilePath && *p!=_T('\\'))
					p--;
				
				if(*p == _T('\\'))
					_tcscpy(p, _T("\\Log"));
			}
			else
				_tcscpy(szFilePath, _T(".\\Log"));

			bFlag = TRUE;
		}

		return szFilePath;
	}

public:
	
	CxLog(bool &alive) : _bAlive(alive)
	{
		_bAlive = true;
		_hWnd = NULL;

	#ifdef CX_LOG_FILE_NAME
		ULONG uSize = _tcslen(CX_LOG_FILE_NAME) + 1;
		uSize = uSize>MAX_PATH ? MAX_PATH : uSize;
		memcpy(_szFileName, CX_LOG_FILE_NAME, uSize*sizeof(TCHAR));
	#else
		CreateDirectory(GetFilePath(), NULL);  //>create log sub dir
		#if !defined(CX_LOG_BY_DAY) && !defined(CX_LOG_BY_WEEK) && !defined(CX_LOG_BY_MONTH) && !defined(CX_LOG_BY_YEAR)
			SYSTEMTIME st;
			TCHAR szName[64];
			GetLocalTime(&st);
			wsprintf(szName, _T("\\%04d%02d%02d"), 
				st.wYear,
				st.wMonth,
				st.wDay
				);	
			_tcscat(_tcscpy(_szFileName, GetFilePath()), szName); 
			_tcscat(_szFileName, CX_LOG_EXT_NAME); 
		#endif 
	#endif

		_TerminateEvent = CreateEvent(0, TRUE, FALSE, NULL);
		_LogHandle = CreateEvent(0, FALSE, FALSE, NULL);
		::InitializeCriticalSection(&_LogMutex);

	#if defined(_MT) && !defined(UNDER_CE)
		unsigned int id;
		_hThreadHandle = (HANDLE)::_beginthreadex(NULL, 0, StaticThreadProc, this, 0, &id);
	#else
		DWORD id;
		_hThreadHandle = ::CreateThread(NULL, 0, StaticThreadProc, this, 0, &id); 
	#endif	
	}

	~CxLog()
	{
		Destroy();	
	}

	VOID Destroy()
	{
		_bAlive = false;
		if(_hThreadHandle)
		{
			SetEvent(_TerminateEvent);
			if(::WaitForSingleObject(_hThreadHandle, 500) != WAIT_OBJECT_0)
				::TerminateThread(_hThreadHandle, 0);
			CloseHandle(_hThreadHandle);
		}
		::DeleteCriticalSection(&_LogMutex);
	}

	VOID Lock()
	{
		::EnterCriticalSection(&_LogMutex);
	}

	VOID Unlock()
	{
		::LeaveCriticalSection(&_LogMutex);
	}

	VOID Log(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType eType, 
			LPCTSTR szDesc, LPVOID pBin = NULL, ULONG uSize = 0)
	{
		Item* p = new Item(szSrc, szFunc, uLine, eType, szDesc, pBin, uSize);

	#if defined(_CONSOLE) || defined(_DEBUG)
		TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE];
		p->Format(szBuffer, CX_LOG_DEFAULT_FORMAT_SIZE);

		LPTSTR pBinStr = p->FormatBinary(p->_pBin, p->_uBinSize, NULL, 0,  &uSize);

		#ifdef _CONSOLE
			if(szBuffer)
				_tprintf(szBuffer);

			if(pBinStr)
				_tprintf(pBinStr);
		#endif

		#ifdef _DEBUG
			OutputDebugString(szBuffer);

			if (pBinStr)
				OutputDebugString(pBinStr);
		#endif

		delete []pBinStr;
	#endif

		if (IsWindow(_hWnd))
		{
			Item *sp = new Item(szSrc, szFunc, uLine, eType, szDesc, pBin, uSize);
			PostMessage(_hWnd, WM_CX_LOG, 0, (LPARAM)sp);
		}

		Lock();
		_ItemList.push_back(p);
		Unlock();
		
		SetEvent(_LogHandle);
	}

	VOID LogBin(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType eType, LPVOID pBin, ULONG uSize)
	{
		Log(szSrc, szFunc, uLine, eType, NULL, pBin, uSize);
	}

	VOID LogN(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType eType, LPCTSTR szFormat, ...) 
	{
		TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE] = {0};

		va_list va;
		va_start(va, szFormat);
		_vsntprintf(szBuffer, 1024-1, szFormat, va);
		va_end(va);

		Log(szSrc, szFunc, uLine, eType, szBuffer);
	}

#ifdef _INC_COMDEF
	VOID Log(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType /*eType*/, _com_error &e)
	{
		TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE] = {0};
		_sntprintf(szBuffer, 1024, _T("_com_error Code=%08X Meaning=%s Source=%s Description=%s"), 
			e.Error(),
			(LPCSTR)_bstr_t(e.ErrorMessage()),
			(LPCSTR)e.Source(),
			(LPCSTR)e.Description());

		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_EXCEPTION, szBuffer);
	}
#endif
	//Log HRESULT
	VOID LogResult(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType /*eType*/, const HRESULT hr)
	{
		TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE] = {0};

		int iLen = _stprintf(szBuffer, _T("(0x%X):"), hr);
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			szBuffer + iLen,
			CX_LOG_DEFAULT_FORMAT_SIZE - iLen - 3,
			NULL
			);

		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_ERROR, szBuffer);
	}

	VOID LogError(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType /*eType*/, const DWORD dwError)
	{
		TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE] = {0};

		int iLen = _stprintf(szBuffer, _T("(%d):"), dwError);
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			szBuffer + iLen,
			CX_LOG_DEFAULT_FORMAT_SIZE - iLen - 3,
			NULL
			);

		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_ERROR, szBuffer);
	}

#if defined(INC_VCL) && defined(__BORLANDC__)
	VOID Log(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType /*eType*/, Exception *e)
	{
	#ifdef _UNICODE
		TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE];
		e->Message.WideChar(szBuffer, CX_LOG_DEFAULT_FORMAT_SIZE);
		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_EXCEPTION, szBuffer);
	#else
		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_EXCEPTION, e->Message.c_str());
	#endif
	}

	VOID Log(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType /*eType*/, Exception &e)
	{
	#ifdef _UNICODE
		TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE];
		e.Message.WideChar(szBuffer, CX_LOG_DEFAULT_FORMAT_SIZE);
		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_EXCEPTION, szBuffer);
	#else
		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_EXCEPTION, e.Message.c_str());
	#endif
	}
#endif

#if (defined(_MFC_VER) && defined(__AFX_H__))
	VOID Log(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType /*eType*/, CException *e)
	{
		TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE];
		e->GetErrorMessage(szBuffer, CX_LOG_DEFAULT_FORMAT_SIZE, NULL);
		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_EXCEPTION, szBuffer);
	}

	VOID Log(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType /*eType*/, CException &e)
	{
		TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE];
		e.GetErrorMessage(szBuffer, CX_LOG_DEFAULT_FORMAT_SIZE, NULL);
		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_EXCEPTION, szBuffer);
	}
#endif

	VOID Log(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType /*eType*/, const std::exception *e)
	{
	#ifdef _UNICODE
		TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE];
		MultiByteToWideChar(CP_ACP, 0, e->what(), -1, szBuffer, CX_LOG_DEFAULT_FORMAT_SIZE);
		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_EXCEPTION, szBuffer);
	#else
		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_EXCEPTION, e->what());
	#endif
	}

	VOID Log(LPCTSTR szSrc, LPCTSTR szFunc, const ULONG uLine, const EnumType /*eType*/, const std::exception &e)
	{
	#ifdef _UNICODE
		TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE];
		MultiByteToWideChar(CP_ACP, 0, e.what(), -1, szBuffer, CX_LOG_DEFAULT_FORMAT_SIZE);
		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_EXCEPTION, szBuffer);
	#else
		Log(szSrc, szFunc, uLine, CxLog::CX_LOG_EXCEPTION, e.what());
	#endif
	}

	VOID SetWnd(HWND hWnd)
	{
		_hWnd = hWnd;
	}

protected:
	bool& _bAlive;
	CRITICAL_SECTION _LogMutex;
	std::list<Item*> _ItemList;
	TCHAR _szFileName[MAX_PATH];
	HANDLE _hThreadHandle, _LogHandle, _TerminateEvent;
	HWND _hWnd;

	virtual VOID Run()
	{
		HANDLE HandleArray[2];
		HandleArray[0] = _LogHandle;
		HandleArray[1] = _TerminateEvent;

		for(;;)
		{
			DWORD ret = ::WaitForMultipleObjects(2, HandleArray, false, INFINITE);
			if(ret == WAIT_OBJECT_0)
			{
				HANDLE hHandle = ::CreateFile(
					GetLogFileName(),
					GENERIC_READ | GENERIC_WRITE ,
					FILE_SHARE_READ,
					NULL,
					OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL, 
					NULL
					);

				if(!hHandle)
					return;
				
				DWORD dwNumberOfBytesWritten;

				SetFilePointer(hHandle, 0, 0, FILE_END);

				try
				{
					Lock();
					while(_ItemList.size())
					{
						Item *p = _ItemList.front();

						TCHAR szBuffer[CX_LOG_DEFAULT_FORMAT_SIZE];
						ULONG uSize = 0;
						p->Format(szBuffer, CX_LOG_DEFAULT_FORMAT_SIZE, &uSize);
						WriteFile(hHandle, szBuffer, uSize*sizeof(TCHAR), &dwNumberOfBytesWritten, NULL);

						if(p->_pBin && p->_uBinSize)
						{
							ULONG uSize = 0;
							LPTSTR t = p->FormatBinary(p->_pBin, p->_uBinSize, NULL, 0,  &uSize);
							WriteFile(hHandle, t, uSize*sizeof(TCHAR), &dwNumberOfBytesWritten, NULL);
							delete []t;
						}

						_ItemList.pop_front();
						//p->_bLog = TRUE;
						delete p;
					}
					Unlock();
				}
				catch (...) 
				{
					Unlock();
				}
								
				SetEndOfFile(hHandle);
				CloseHandle(hHandle);
			}

            if(ret == WAIT_OBJECT_0 + 1)
				break;
		}	
	}

private:
	CxLog(const CxLog&);
	CxLog& operator=(CxLog&);

#if defined(_MT) && !defined(UNDER_CE)
	static UINT APIENTRY StaticThreadProc(LPVOID lpPara) //允许C多线程运行库
#else
	static DWORD WINAPI StaticThreadProc(LPVOID lpPara)
#endif
	{
		((CxLog*)(lpPara))->Run();

	#if defined(_MT) && !defined(UNDER_CE)
		::_endthreadex(0);
	#endif
		return 0;
	}
};

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

#define __WFILE__ WIDEN(__FILE__)
#ifdef _UNICODE
	#define __TFILE__ __WFILE__
#else
	#define __TFILE__ __FILE__
#endif

#if (_MSC_VER && _MSC_VER<=1200) || (__BORLANDC__) 
	#define __FUNCTION__ NULL
	#define __WFUNCTION__ NULL
#else //_MSC_VER>1200 __GNUC__ __INTEL_COMPILER
	#define __WFUNCTION__ WIDEN(__FUNCTION__)
#endif

#ifdef _UNICODE
	#ifdef __GNUC__
		#define __TFUNCTION__ WIDEN(__func__)
	#else
		#define __TFUNCTION__ __WFUNCTION__
	#endif
#else
	#define __TFUNCTION__ __FUNCTION__
#endif

#define CHECK_LAST_ERROR() do \
{ \
	DWORD dwErr = GetLastError(); \
	if(dwErr) \
		CxLog::Instance().LogError(__TFILE__, __TFUNCTION__, __LINE__, CxLog::CX_LOG_ERROR, dwErr); \
} \
while (0)

#define CHECK_HR(hr) do \
{ \
	if(FAILED(hr)) \
		LOG_HR(hr); \
} \
while (0)

#define BASE_LOG(type, msg)											CxLog::Instance().Log(__TFILE__, __TFUNCTION__, __LINE__, (type), (msg))
#define LOG(msg)													BASE_LOG(CxLog::CX_LOG_MESSAGE, (msg))
#define LOG_WARN(msg)												BASE_LOG(CxLog::CX_LOG_WARNING, (msg))
#define LOGE(e)														CxLog::Instance().Log(__TFILE__, __TFUNCTION__, __LINE__, CxLog::CX_LOG_EXCEPTION, (e))
#define LOG_ERR(msg)												BASE_LOG(CxLog::CX_LOG_ERROR, (msg))

#define BASE_LOG_BIN(type, bin, size)								CxLog::Instance().LogBin(__TFILE__, __TFUNCTION__, __LINE__, (type), (bin), (size))
#define LOG_BIN(bin, size)											BASE_LOG_BIN(CxLog::CX_LOG_MESSAGE, (bin), (size))
#define LOG_WARN_BIN(bin, size)										BASE_LOG_BIN(CxLog::CX_LOG_WARNING, (bin), (size))
#define LOG_ERR_BIN(bin, size)										BASE_LOG_BIN(CxLog::CX_LOG_ERROR, (bin), (size))

#if __GNUC__ || __INTEL_COMPILER || _MSC_VER>1310
	#define BASE_LOGN(type, msg, ...)								CxLog::Instance().LogN(__TFILE__, __TFUNCTION__, __LINE__, (type), (msg), ##__VA_ARGS__)
	#define LOGN(msg, ...)											BASE_LOGN(CxLog::CX_LOG_MESSAGE, (msg), ##__VA_ARGS__)
	#define LOGN_WARN(msg, ...)										BASE_LOGN(CxLog::CX_LOG_WARNING, (msg), ##__VA_ARGS__)
	#define LOGN_ERR(msg, ...)										BASE_LOGN(CxLog::CX_LOG_ERROR, (msg), ##__VA_ARGS__)
#endif

#define PRE_LOG														CxLog::Instance().LogN(__TFILE__, __TFUNCTION__, __LINE__, CxLog::CX_LOG_MESSAGE,
#define LOG1(str, p1)												PRE_LOG (str), (p1))
#define LOG2(str, p1, p2)											PRE_LOG (str), (p1), (p2))
#define LOG3(str, p1, p2, p3)										PRE_LOG (str), (p1), (p2), (p3))
#define LOG4(str, p1, p2, p3, p4)									PRE_LOG (str), (p1), (p2), (p3), (p4))
#define LOG5(str, p1, p2, p3, p4, p5)								PRE_LOG (str), (p1), (p2), (p3), (p4), (p5))
#define LOG6(str, p1, p2, p3, p4, p5, p6)							PRE_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6))
#define LOG7(str, p1, p2, p3, p4, p5, p6, p7)						PRE_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7))
#define LOG8(str, p1, p2, p3, p4, p5, p6, p7, p8)					PRE_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8))
#define LOG9(str, p1, p2, p3, p4, p5, p6, p7, p8, p9)				PRE_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9))

#define PRE_T_LOG													CxLog::Instance().LogN(__TFILE__, __TFUNCTION__, __LINE__, 
#define LOG_T1(t, str, p1)											PRE_T_LOG (t), (str), (p1))
#define LOG_T2(t, str, p1, p2)										PRE_T_LOG (t), (str), (p1), (p2))
#define LOG_T3(t, str, p1, p2, p3)									PRE_T_LOG (t), (str), (p1), (p2), (p3))
#define LOG_T4(t, str, p1, p2, p3, p4)								PRE_T_LOG (t), (str), (p1), (p2), (p3), (p4))
#define LOG_T5(t, str, p1, p2, p3, p4, p5)							PRE_T_LOG (t), (str), (p1), (p2), (p3), (p4), (p5))
#define LOG_T6(t, str, p1, p2, p3, p4, p5, p6)						PRE_T_LOG (t), (str), (p1), (p2), (p3), (p4), (p5), (p6))
#define LOG_T7(t, str, p1, p2, p3, p4, p5, p6, p7)					PRE_T_LOG (t), (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7))
#define LOG_T8(t, str, p1, p2, p3, p4, p5, p6, p7, p8)				PRE_T_LOG (t), (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8))
#define LOG_T9(t, str, p1, p2, p3, p4, p5, p6, p7, p8, p9)			PRE_T_LOG (t), (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9))

#define PRE_WARN_LOG												CxLog::Instance().LogN(__TFILE__, __TFUNCTION__, __LINE__, CxLog::CX_LOG_WARNING, 
#define LOG_WARN1(str, p1)											PRE_WARN_LOG (str), (p1))
#define LOG_WARN2(str, p1, p2)										PRE_WARN_LOG (str), (p1), (p2))
#define LOG_WARN3(str, p1, p2, p3)									PRE_WARN_LOG (str), (p1), (p2), (p3))
#define LOG_WARN4(str, p1, p2, p3, p4)								PRE_WARN_LOG (str), (p1), (p2), (p3), (p4))
#define LOG_WARN5(str, p1, p2, p3, p4, p5)							PRE_WARN_LOG (str), (p1), (p2), (p3), (p4), (p5))
#define LOG_WARN6(str, p1, p2, p3, p4, p5, p6)						PRE_WARN_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6))
#define LOG_WARN7(str, p1, p2, p3, p4, p5, p6, p7)					PRE_WARN_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7))
#define LOG_WARN8(str, p1, p2, p3, p4, p5, p6, p7, p8)				PRE_WARN_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8))
#define LOG_WARN9(str, p1, p2, p3, p4, p5, p6, p7, p8, p9)			PRE_WARN_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9))

#define PRE_EXCEPTION_LOG											CxLog::Instance().LogN(__TFILE__, __TFUNCTION__, __LINE__, CxLog::CX_LOG_EXCEPTION,
#define LOG_EXCEPTION1(str, p1)										PRE_EXCEPTION_LOG (str), (p1))
#define LOG_EXCEPTION2(str, p1, p2)									PRE_EXCEPTION_LOG (str), (p1), (p2))
#define LOG_EXCEPTION3(str, p1, p2, p3)								PRE_EXCEPTION_LOG (str), (p1), (p2), (p3))
#define LOG_EXCEPTION4(str, p1, p2, p3, p4)							PRE_EXCEPTION_LOG (str), (p1), (p2), (p3), (p4))
#define LOG_EXCEPTION5(str, p1, p2, p3, p4, p5)						PRE_EXCEPTION_LOG (str), (p1), (p2), (p3), (p4), (p5))
#define LOG_EXCEPTION6(str, p1, p2, p3, p4, p5, p6)					PRE_EXCEPTION_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6))
#define LOG_EXCEPTION7(str, p1, p2, p3, p4, p5, p6, p7)				PRE_EXCEPTION_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7))
#define LOG_EXCEPTION8(str, p1, p2, p3, p4, p5, p6, p7, p8)			PRE_EXCEPTION_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8))
#define LOG_EXCEPTION9(str, p1, p2, p3, p4, p5, p6, p7, p8, p9)		PRE_EXCEPTION_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9))

#define LOGE1														LOG_EXCEPTION1
#define LOGE2														LOG_EXCEPTION2
#define LOGE3														LOG_EXCEPTION3
#define LOGE4														LOG_EXCEPTION4
#define LOGE5														LOG_EXCEPTION5
#define LOGE6														LOG_EXCEPTION6
#define LOGE7														LOG_EXCEPTION7
#define LOGE8														LOG_EXCEPTION8
#define LOGE9														LOG_EXCEPTION9

#define PRE_ERR_LOG													CxLog::Instance().LogN(__TFILE__, __TFUNCTION__, __LINE__, CxLog::CX_LOG_ERROR,
#define LOG_ERR1(str, p1)											PRE_ERR_LOG (str), (p1))
#define LOG_ERR2(str, p1, p2)										PRE_ERR_LOG (str), (p1), (p2))
#define LOG_ERR3(str, p1, p2, p3)									PRE_ERR_LOG (str), (p1), (p2), (p3))
#define LOG_ERR4(str, p1, p2, p3, p4)								PRE_ERR_LOG (str), (p1), (p2), (p3), (p4))
#define LOG_ERR5(str, p1, p2, p3, p4, p5)							PRE_ERR_LOG (str), (p1), (p2), (p3), (p4), (p5))
#define LOG_ERR6(str, p1, p2, p3, p4, p5, p6)						PRE_ERR_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6))
#define LOG_ERR7(str, p1, p2, p3, p4, p5, p6, p7)					PRE_ERR_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7))
#define LOG_ERR8(str, p1, p2, p3, p4, p5, p6, p7, p8)				PRE_ERR_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8))
#define LOG_ERR9(str, p1, p2, p3, p4, p5, p6, p7, p8, p9)			PRE_ERR_LOG (str), (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9))

#define LOG_LAST_ERROR()											CxLog::Instance().LogError(__TFILE__, __TFUNCTION__, __LINE__, CxLog::CX_LOG_ERROR, GetLastError())
#define LOG_LE														LOG_LAST_ERROR
#define CHECK_LE													CHECK_LAST_ERROR
#define LOG_HR(hr)													CxLog::Instance().LogResult(__TFILE__, __TFUNCTION__, __LINE__, CxLog::CX_LOG_ERROR, (hr))

#ifdef _DEBUG
	#define DLOG													LOG
	#define DLOG_WARN												LOG_WARN
	#define DLOGE													LOGE
	#define DLOG_ERR												LOG_ERR

	#define DLOG_BIN												LOG_BIN
	#define DLOG_WARN_BIN											LOG_WARN_BIN
	#define DLOG_ERR_BIN											LOG_ERR_BIN

	#define DLOG_ERR1												LOG_ERR1
	#define DLOG_ERR2												LOG_ERR2
	#define DLOG_ERR3												LOG_ERR3
	#define DLOG_ERR4												LOG_ERR4
	#define DLOG_ERR5												LOG_ERR5
	#define DLOG_ERR6												LOG_ERR6
	#define DLOG_ERR7												LOG_ERR7
	#define DLOG_ERR8												LOG_ERR8
	#define DLOG_ERR9												LOG_ERR9

	#define DLOG_EXCEPTION1											LOG_EXCEPTION1
	#define DLOG_EXCEPTION2											LOG_EXCEPTION2
	#define DLOG_EXCEPTION3											LOG_EXCEPTION3
	#define DLOG_EXCEPTION4											LOG_EXCEPTION4
	#define DLOG_EXCEPTION5											LOG_EXCEPTION5
	#define DLOG_EXCEPTION6											LOG_EXCEPTION6
	#define DLOG_EXCEPTION7											LOG_EXCEPTION7
	#define DLOG_EXCEPTION8											LOG_EXCEPTION8
	#define DLOG_EXCEPTION9											LOG_EXCEPTION9

	#define DLOGE1													LOGE1 
	#define DLOGE2													LOGE2 
	#define DLOGE3													LOGE3 
	#define DLOGE4													LOGE4 
	#define DLOGE5													LOGE5 
	#define DLOGE6													LOGE6 
	#define DLOGE7													LOGE7 
	#define DLOGE8													LOGE8 
	#define DLOGE9													LOGE9 

	#define DLOG_T1													LOG_T1
	#define DLOG_T2													LOG_T2
	#define DLOG_T3													LOG_T3
	#define DLOG_T4													LOG_T4
	#define DLOG_T5													LOG_T5
	#define DLOG_T6													LOG_T6
	#define DLOG_T7													LOG_T7
	#define DLOG_T8													LOG_T8
	#define DLOG_T9													LOG_T9

	#define DLOG1													LOG1
	#define DLOG2													LOG2
	#define DLOG3													LOG3
	#define DLOG4													LOG4
	#define DLOG5													LOG5
	#define DLOG6													LOG6
	#define DLOG7													LOG7
	#define DLOG8													LOG8
	#define DLOG9													LOG9

	#define DLOG_WARN1												LOG_WARN1
	#define DLOG_WARN2												LOG_WARN2
	#define DLOG_WARN3												LOG_WARN3
	#define DLOG_WARN4												LOG_WARN4
	#define DLOG_WARN5												LOG_WARN5
	#define DLOG_WARN6												LOG_WARN6
	#define DLOG_WARN7												LOG_WARN7
	#define DLOG_WARN8												LOG_WARN8
	#define DLOG_WARN9												LOG_WARN9
	
#else
	#define DLOG													((void)0)			
	#define DLOG_WARN												((void)0)
	#define DLOGE													((void)0)
	#define DLOG_ERR												((void)0)
	            
	#define DLOG_BIN												((void)0)
	#define DLOG_WARN_BIN											((void)0)
	#define DLOG_ERR_BIN											((void)0)
	                   
	#define DLOG_ERR1												((void)0)		
	#define DLOG_ERR2												((void)0)		
	#define DLOG_ERR3												((void)0)	
	#define DLOG_ERR4												((void)0)
	#define DLOG_ERR5												((void)0)	
	#define DLOG_ERR6												((void)0)
	#define DLOG_ERR7												((void)0)
	#define DLOG_ERR8												((void)0)
	#define DLOG_ERR9												((void)0)
	                            
	#define DLOG_EXCEPTION1											((void)0)
	#define DLOG_EXCEPTION2											((void)0)
	#define DLOG_EXCEPTION3											((void)0)
	#define DLOG_EXCEPTION4											((void)0)
	#define DLOG_EXCEPTION5											((void)0)
	#define DLOG_EXCEPTION6											((void)0)
	#define DLOG_EXCEPTION7											((void)0)
	#define DLOG_EXCEPTION8											((void)0)
	#define DLOG_EXCEPTION9											((void)0)
	                            									
	#define DLOGE1													((void)0)
	#define DLOGE2													((void)0)
	#define DLOGE3													((void)0)
	#define DLOGE4													((void)0)
	#define DLOGE5													((void)0)
	#define DLOGE6													((void)0)
	#define DLOGE7													((void)0)
	#define DLOGE8													((void)0)
	#define DLOGE9													((void)0)
	                            									
	#define DLOG_T1													((void)0)
	#define DLOG_T2													((void)0)
	#define DLOG_T3													((void)0)
	#define DLOG_T4													((void)0)
	#define DLOG_T5													((void)0)
	#define DLOG_T6													((void)0)
	#define DLOG_T7													((void)0)
	#define DLOG_T8													((void)0)
	#define DLOG_T9													((void)0)
	                            									
	#define DLOG1													((void)0)
	#define DLOG2													((void)0)
	#define DLOG3													((void)0)
	#define DLOG4													((void)0)
	#define DLOG5													((void)0)
	#define DLOG6													((void)0)
	#define DLOG7													((void)0)
	#define DLOG8													((void)0)
	#define DLOG9													((void)0)
	                            									
	#define DLOG_WARN1												((void)0)
	#define DLOG_WARN2												((void)0)
	#define DLOG_WARN3												((void)0)
	#define DLOG_WARN4												((void)0)
	#define DLOG_WARN5												((void)0)
	#define DLOG_WARN6												((void)0)
	#define DLOG_WARN7												((void)0)
	#define DLOG_WARN8												((void)0)
	#define DLOG_WARN9												((void)0)
#endif																
																	
																	
#endif //_CX_LOG_H_ 
