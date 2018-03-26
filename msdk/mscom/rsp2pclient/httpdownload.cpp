// HttpDownload.cpp: implementation of the CHttpDownload class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HttpDownload.h"
#include "url.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")

#ifdef __ATLMISC_H__
#define _ATL_USE_CSTRING_FLOAT
#endif


#define __HTTP_VERB_GET					_T("GET")
#define __HTTP_VERB_POST                _T("POST")
#define __HTTP_ACCEPT_TYPE				_T("*/*")
#define __HTTP_POST_HEAD_FLAGS          _T("InPost=")
#define	__HTTP_POST_CONTENT_TYPE        _T("Content-Type: application/x-www-form-urlencoded")

#define __HTTP_HEADER_BUF_SIZE			64
#define __HTTP_LOCATION_BUF_SIZE		1024
#define __HTTP_LASTMODIFIED_BUF_SIZE	64
#define __HTTP_READ_BUF_SIZE			(4*1024)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL GetIEVersion(LPTSTR lpszVersion, UINT uSize)
{
	//////////////////////////////////////////////////////////////////////////
	TCHAR szFileName[MAX_PATH] = { 0 };
	HMODULE hModule = ::GetModuleHandle(_T("WININET.DLL"));
	DWORD dwHandle = 0;
	if (hModule == 0)
		return FALSE;
	DWORD dwLen = GetModuleFileName(hModule, szFileName, sizeof(szFileName) / sizeof(szFileName[0]));
	if (dwLen <= 0)
		return FALSE;

	dwLen = GetFileVersionInfoSize(szFileName, &dwHandle);
	if (dwLen <= 0)
		return FALSE;
	BYTE *pVersionInfo = new BYTE[dwLen];
	if (!::GetFileVersionInfo(szFileName, dwHandle, dwLen, pVersionInfo))
	{
		delete pVersionInfo;
		return FALSE;
	}

	VS_FIXEDFILEINFO *pFixed = NULL;
	UINT uLen = 0;
	if (!VerQueryValue(pVersionInfo, _T("\\"), (LPVOID*)&pFixed, &uLen))
	{
		delete pVersionInfo;

		return FALSE;
	}

	//wsprintf(lpszVersion, _T("MSIE %d.%d"), verInfo.dwMajorVersion, verInfo.dwMinorVersion);
	wsprintf(lpszVersion,
		_T("MSIE %d.%d"), 
		HIWORD(pFixed->dwFileVersionMS/*dwProductVersionMS*/),
		LOWORD(pFixed->dwFileVersionMS/*dwProductVersionMS*/));	

	delete pVersionInfo;

	return TRUE;
}

BOOL GetOSVersion(LPTSTR lpszVersion, UINT uSize)
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
		{
			*lpszVersion = _T('\0');
			return FALSE;
		}
	}

	switch (osvi.dwPlatformId)
	{
		// Test for the Windows NT product family.
	case VER_PLATFORM_WIN32_NT:
		{
			wsprintf(lpszVersion, _T("Windows NT %d.%d"), osvi.dwMajorVersion, osvi.dwMinorVersion);
		}

		break;

		// Test for the Windows 95 product family.
	case VER_PLATFORM_WIN32_WINDOWS:

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			wsprintf(lpszVersion, _T("Windows 95"));
			
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			wsprintf(lpszVersion, _T("Windows 98"));
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			wsprintf(lpszVersion, _T("Windows Me"));
		} 
		break;

	case VER_PLATFORM_WIN32s:

		wsprintf(lpszVersion, _T("Windows"));
		break;
	}
	return TRUE; 

}
CHttpDownload::CHttpDownload()
{
	m_hInternet				= NULL;
	m_hConnect				= NULL;
	m_hRequest				= NULL;

	m_bServerAlter			= FALSE;
	m_bAuthorization		= FALSE;
	m_bProxyAuthorization	= FALSE;
	m_bUseProxy				= FALSE;
	m_bStop					= FALSE;
	
	m_nServerPort			= INTERNET_DEFAULT_HTTP_PORT;
	m_nProxyPort			= 0;
	m_dwLastError			= S_OK;
	m_hNotifyWnd			= NULL;
	m_nConnectType          = CONNECT_TYPE_IE;
	m_dwFileSize			= 0;

	// _T("Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)")
	TCHAR szIEVer[MAX_PATH] = { 0 };
	GetIEVersion(szIEVer, MAX_PATH);
	TCHAR szOSVer[MAX_PATH] = { 0 };
	GetOSVersion(szOSVer, MAX_PATH);
	m_strUserAgent.Format(_T("Mozilla/4.0 (compatible; %s; %s; Margin)"), szIEVer, szOSVer);
 }

CHttpDownload::~CHttpDownload()
{
	CloseHandles();
}


//////////////////////////////////////////////////////////////////////////////////
//	��������BOOL Connect()
//	��  ;����ʼ��������session��Http���ӣ��ڲ�ʹ�ã�
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ������
//	����ֵ���ɹ���TRUE��ʧ��FALSE
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpDownload::Connect()
{
	DWORD dwTimeOut = 30*1000;

	if (!m_hInternet)
	{
		if (m_bUseProxy)
		{
			// ��pszProxyName��lpszProxyBypass Ϊ��IE7�᷵��ʧ��
			m_hInternet = InternetOpen(m_strUserAgent, INTERNET_OPEN_TYPE_PROXY, TEXT("proxy"), TEXT("<local>"), 0L);
		}
		else
			m_hInternet = InternetOpen(m_strUserAgent, m_nConnectType, NULL,	NULL, 0L);
		
		if (NULL == m_hInternet)
		{
			m_dwLastError = ::GetLastError();
			return FALSE;
		}
		
		INTERNET_PROXY_INFO proxyInfo;
		ZeroMemory(&proxyInfo, sizeof(INTERNET_PROXY_INFO));
		TCHAR szProxy[MAX_PATH] = { 0 };
		if (m_bUseProxy)
		{				
			proxyInfo.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
			wsprintf(szProxy, _T("%s:%d"), m_strProxyServer, m_nProxyPort );
			proxyInfo.lpszProxy = szProxy;
			
			if (!InternetSetOption(m_hInternet, INTERNET_OPTION_PROXY, &proxyInfo, sizeof(INTERNET_PROXY_INFO)))
			{
				m_dwLastError = ::GetLastError();
				CloseHandles();
				return FALSE;
			}
		}
		
		if (!InternetSetOption(m_hInternet, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(DWORD)))
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			return FALSE;
		}
	}
	
	if (!m_hConnect)
	{
		if (!m_bAuthorization)
		{
			m_hConnect = InternetConnect(m_hInternet,
				m_strServerName,
				m_nServerPort,
				NULL,
				NULL,
				INTERNET_SERVICE_HTTP,
				0L,
				NULL);
		}
		else
		{
			m_hConnect = InternetConnect(m_hInternet,
				m_strServerName,
				m_nServerPort,
				m_strUsername,
				m_strPassword,
				INTERNET_SERVICE_HTTP,
				0L,
				NULL);
		}

		if (NULL == m_hConnect)
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			return FALSE;
		}

		if (ERROR_SUCCESS != ::InternetAttemptConnect(NULL))
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			return FALSE;
		}
		
		if (!InternetSetOption(m_hConnect, INTERNET_OPTION_SEND_TIMEOUT, &dwTimeOut, sizeof(DWORD)))
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			return FALSE;
		}
		
		if (!InternetSetOption(m_hConnect, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwTimeOut, sizeof(DWORD)))
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			return FALSE;
		}

		if (m_bProxyAuthorization)
		{
			if (!InternetSetOption(m_hConnect, 
						INTERNET_OPTION_PROXY_USERNAME,
						(LPVOID)(LPCTSTR)m_strProxyUsername,
						m_strProxyUsername.GetLength() + 1))
			{
				m_dwLastError = ::GetLastError();
				CloseHandles();
				return FALSE;
			}
				
			if (!InternetSetOption(m_hConnect, 
						INTERNET_OPTION_PROXY_PASSWORD,
						(LPVOID)(LPCTSTR)m_strProxyPassword,
						m_strProxyPassword.GetLength() + 1))
			{
				m_dwLastError = ::GetLastError();
				CloseHandles();
				return FALSE;
			}
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//	��������BOOL _CreateDirectory(LPCTSTR lpFileName,
//					   BOOL    bIsFile,
//					   LPSECURITY_ATTRIBUTES lpSecurityAttributes)
//	��  ;�������༶·��(�ڲ�ʹ��)
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		lpFileName		: ��������Ŀ¼
//		bIsFile			: lpFileName�Ƿ�ΪĿ¼�����ļ���TRUEΪ�ļ�����FALSEΪĿ¼��
//      lpSecurityAttributes : NULL
//	����ֵ���ɹ���TRUE��ʧ��FALSE
/////////////////////////////////////////////////////////////////////////////////
static BOOL _CreateDirectory(LPCTSTR lpFileName,
					   BOOL    bIsFile,
					   LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	
	BOOL	fRet = TRUE;
	TCHAR	szFileName[MAX_PATH] = { 0 };
	TCHAR*	lpChr = NULL;
	TCHAR*	lpHead = NULL;

	_ASSERT(lpFileName != NULL && *lpFileName != _T('\0'));
	if (lpFileName == NULL || lstrlen(lpFileName) <= 0)
		return FALSE;
		
	if (lstrlen(lpFileName) > sizeof(szFileName))
	{
		SetLastError(ERROR_BUFFER_OVERFLOW);
		return FALSE;
	}
	
	lstrcpyn(szFileName, lpFileName, sizeof(szFileName)-1);
	if (bIsFile)
		PathRemoveFileSpec(szFileName);
	lpHead = PathSkipRoot(szFileName);
	if (lstrlen(lpHead) == 0)
	{
		CreateDirectory(szFileName, lpSecurityAttributes);
		return TRUE;
	}

	while (true)
	{
		lpChr = _tcschr(lpHead, _T('\\'));

		// zcg+, ���Ӷ�·������ '/' ��֧��. 
		if (NULL == lpChr)
		{
			lpChr = _tcschr(lpHead, _T('/'));
		}

		if (NULL == lpChr)
		{
			fRet &= CreateDirectory(szFileName, lpSecurityAttributes);
			if (!fRet && (GetLastError() == ERROR_ALREADY_EXISTS))
				fRet = TRUE;
			break;
		}
		else
		{
			*lpChr = NULL;
			fRet &= CreateDirectory(szFileName, lpSecurityAttributes);
			if (!fRet && (GetLastError() == ERROR_ALREADY_EXISTS))
				fRet = TRUE;
			*lpChr = _T('\\');
			lpHead = lpChr+1;
		}
	}
	return fRet;
}


//////////////////////////////////////////////////////////////////////////////////
//	��������HRESULT Download(LPCTSTR lpszURL,
//				LPCTSTR lpszSaveFileName	/*= NULL*/,
//				BOOL bAutoRedirect /*= FALSE*/)
//	��  ;��������ڣ����ô˺�������ʼ���ع���
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		lpszDownloadUrl   : �����ص�����URL
//		lpszSaveFileName  : ���ر����ļ���(��Ҫ�õ��ض���URLʱ����ΪNULL)	
//      bAutoRedirect     : �Ƿ��Զ��ض���
//		lpszResetHost	  : [in]�����Ҫ����HTTPЭ��ͷ��Hostʱ���˲�����Ч��
//			ΪNULL��ʾ����Ҫ����HTTPЭ��ͷ��Host�����lpszURL��ָ�����Ƿ�����
//			��IP��ַʱ���õ��������
//	����ֵ��HRESULT
//		ERR_HTTP_USER_STOP		: �û�ֹͣ����(������StopDownload����)
//		ERR_HTTP_FAIL			: ����ʧ��
//		ERR_HTTP_SUCCESS		: ���سɹ�
//		ERR_HTTP_REDIRECT		: �ض���HTTP
//		ERR_HTTP_INVALID_URL	: ��ЧURL
//      ERR_HTTP_DEST_FILENAME_EMPTY : ���ش洢�ļ�Ϊ��
//      ERR_HTTP_CREATE_FILE_FAIL : ���������ļ�ʧ��
//      ERR_HTTP_RENAME_FILE_FAIL : �����������ļ�ʧ��
//      ERR_HTTP_SEND_TIMEOUT     : ����ʱ
/////////////////////////////////////////////////////////////////////////////////
HRESULT CHttpDownload::Download(LPCTSTR lpszURL, LPCTSTR lpszSaveFileName /* = NULL */, 
								BOOL bAutoRedirect /*= FALSE*/, LPCTSTR lpszResetHost/* = NULL*/)
{
	_ASSERT(lpszURL);
	m_strRedirectURL.Empty();		// ����ض����ַ

	// �õ����ص���ʱ�ļ�������ԭ���ļ��Ļ����ϼ�.rs
	CString strTmpFile = lpszSaveFileName;
	if (!strTmpFile.IsEmpty())
	{
		strTmpFile += _T(".rs");
	}
	
	// ����URL�Ƿ�Ϸ�
	if (!ParseUrl(lpszURL))
	{
		return ERR_HTTP_INVALID_URL;
	}

	if (m_bServerAlter)
		CloseHandles();
	
	// ��������ֹ
	if (m_bStop)
	{
		CloseHandles();
		return ERR_HTTP_USER_STOP;
	}
	
	// ��ʼ����Ϣ�ṹ
	HTTPDOWNLOADSTATUS httpDown;
	httpDown.dwFileSize = 0;
	httpDown.dwFileDownloadedSize = 0;
	httpDown.strFileName = m_strObjectName.Mid(m_strObjectName.ReverseFind(_T('/')) + 1);

	httpDown.nStatusType = DOWNLOAD_STATUS_FILENAME;
	// ����ô��ڷ��������ļ�����Ϣ
	if (!strTmpFile.IsEmpty())
	{
		Notify(WM_HTTPDOWN_NOTIFY_MSG, 0L, (LPARAM)&httpDown);
	}
	
	// ��ʼ��WinINet�������������
	if (!Connect())
	{
		return ERR_HTTP_FAIL;
	}

	// ��������ֹ
	if (m_bStop)
	{
		CloseHandles();
		return ERR_HTTP_USER_STOP;
	}

	static LPCTSTR szAcceptType[]= { __HTTP_ACCEPT_TYPE, 0 };
	DWORD dwOpenFlags = INTERNET_FLAG_NEED_FILE | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION;
	
	if (!bAutoRedirect)
		dwOpenFlags |= INTERNET_FLAG_NO_AUTO_REDIRECT;

	// Open Request
	m_hRequest = HttpOpenRequest(m_hConnect, __HTTP_VERB_GET,
					m_strObjectName, HTTP_VERSION, NULL,
					szAcceptType, dwOpenFlags, NULL);
	// ERROR return
	if (NULL == m_hRequest)
	{
		m_dwLastError = ::GetLastError();
		CloseHandles();
		
		return ERR_HTTP_FAIL;
	}

	TCHAR szHost[272] = { _T('\0') };
	if (lpszResetHost != NULL) 
	{
		// �滻Httpͷ��Hostֵ
		lstrcpy(szHost, _T("Host: "));
		lstrcat(szHost, lpszResetHost);
		//lstrcat(szHost, "\r\n");
		HttpAddRequestHeaders(m_hRequest, szHost, -1, HTTP_ADDREQ_FLAG_ADD );
	}

	TCHAR szHeader[__HTTP_HEADER_BUF_SIZE+1] = { 0 };
	BOOL bSendRange = FALSE;
	
	SYSTEMTIME stLocalFile;						// ���ش��ڴ������ļ�ʱ��
	SYSTEMTIME stNetFile;						// �������������ļ�ʱ��

	// Ĭ��ȡ��ǰʱ��
	GetLocalTime(&stNetFile);

	// �����ش��ڴ�������ʱ�ļ�����õ��ļ���Сд��Htppͷ��Range��
	HANDLE hFileDown = NULL;
	if (!strTmpFile.IsEmpty())
	{
		WIN32_FIND_DATA finder = {0};
		HANDLE hFileDown = FindFirstFile(strTmpFile, &finder);
		if (hFileDown != INVALID_HANDLE_VALUE)
		{
			FindClose(hFileDown);
			hFileDown = NULL;
			FileTimeToSystemTime(&finder.ftLastWriteTime, &stLocalFile);

			httpDown.dwFileDownloadedSize  = finder.nFileSizeLow;
			
            if (httpDown.dwFileDownloadedSize > 0 )
            {
                wsprintf(szHeader, _T("Range: bytes=%d-"), finder.nFileSizeLow);
			    bSendRange = TRUE;
            }
            else
            {
                SetFileAttributes(strTmpFile, FILE_ATTRIBUTE_NORMAL);
                DeleteFile(strTmpFile);
            }
		}	
	}

	// ��������ֹ
	if (m_bStop)
	{
		CloseHandles();
		return ERR_HTTP_USER_STOP;
	}

	// SEND REQUEST
	if (!::HttpSendRequest(m_hRequest, szHeader, lstrlen(szHeader),	NULL, 0))
	{
		m_dwLastError = ::GetLastError();
		CloseHandles();	
		
		switch (m_dwLastError)
		{
		case ERROR_INTERNET_CANNOT_CONNECT:
		case ERROR_INTERNET_NAME_NOT_RESOLVED:
			return ERR_HTTP_NET_ERROR;
			break;
		case ERROR_INTERNET_TIMEOUT:
			return ERR_HTTP_SEND_TIMEOUT;
		default:
			{
			}
			return ERR_HTTP_FAIL;;
		}
	}	
	
	// ��������ֹ
	if (m_bStop)
	{
		CloseHandles();
		return ERR_HTTP_USER_STOP;
	}

	DWORD dwCode = 0;
	DWORD dwSize = sizeof(DWORD) ;
	if (!HttpQueryInfo (m_hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwCode, &dwSize, NULL))
	{
		m_dwLastError = ::GetLastError();
		CloseHandles();
		return ERR_HTTP_FAIL;
	}
	
	BOOL bSendAgain = FALSE;
	switch (dwCode)
	{
	// �ض���
	case HTTP_STATUS_REDIRECT:
	case HTTP_STATUS_MOVED:
		{
			TCHAR* pszLocation = new TCHAR[__HTTP_LOCATION_BUF_SIZE + 1];
			ZeroMemory(pszLocation, __HTTP_LOCATION_BUF_SIZE +1 );
			dwSize = __HTTP_LOCATION_BUF_SIZE;
			if (!HttpQueryInfo (m_hRequest, HTTP_QUERY_LOCATION, pszLocation, &dwSize, NULL))
			{
				delete pszLocation;
				m_dwLastError = ::GetLastError();
				CloseHandles();

				return ERR_HTTP_FAIL;
			}

			m_strRedirectURL = pszLocation;

			CloseHandles();
			delete pszLocation;
			
			return ERR_HTTP_REDIRECT;
		}
	// 200 OK
	case HTTP_STATUS_OK:
		{
			if (strTmpFile.IsEmpty())
			{
				CloseHandles();
				return ERR_HTTP_DEST_FILENAME_EMPTY;
			}
			else
			{
				// ����ɾ���ϴ������жϵ��ļ�
				DeleteFile(strTmpFile);
			}
		}
		break;
	case HTTP_STATUS_NOT_FOUND:
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			return ERR_HTTP_NOT_FIND;
		}
		break;

	case HTTP_STATUS_FORBIDDEN:
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			return ERR_HTTP_FORBIDDEN_ACCESS;
		}
	// �ϵ����󷵻�
	case HTTP_STATUS_PARTIAL:
	case HTTP_STATUS_PARTIAL_CONTENT:
		{
			TCHAR szLastModified[__HTTP_LASTMODIFIED_BUF_SIZE + 1] = { 0 };
			dwSize = __HTTP_LASTMODIFIED_BUF_SIZE;
			if (!HttpQueryInfo(m_hRequest, HTTP_QUERY_LAST_MODIFIED, szLastModified, &dwSize, NULL))
			{
				m_dwLastError = ::GetLastError();
                DeleteFile(strTmpFile);
                bSendAgain = TRUE;
                httpDown.dwFileDownloadedSize = 0;
				CloseHandles();
				return ERR_HTTP_FAIL;
			}
			HttpDateToSystemTime(szLastModified, stNetFile);

			if (stNetFile.wSecond != stLocalFile.wSecond
				|| stNetFile.wMinute != stLocalFile.wMinute
				|| stNetFile.wHour != stLocalFile.wHour
				|| stNetFile.wDay != stLocalFile.wDay
				|| stNetFile.wMonth != stLocalFile.wMonth
				|| stNetFile.wYear != stLocalFile.wYear)
			{
				// ɾ���ϴ������жϵ��ļ�
				DeleteFile(strTmpFile);
				bSendAgain = TRUE;
				httpDown.dwFileDownloadedSize = 0;
			}
		}
		break;
	case HTTP_STATUS_SERVER_ERROR:
		{
			m_dwLastError = ::GetLastError();
			return ERR_HTTP_SERVER_ERROR;
		}		
		break;
	default:
		if (bSendRange)
		{
			bSendAgain = TRUE;
			break;
		}
		else
		{
			m_dwLastError = ::GetLastError();
			return ERR_HTTP_FAIL;
		}
	}
	
	// ��������ֹ
	if (m_bStop)
	{
		CloseHandles();
		return ERR_HTTP_USER_STOP;
	}

	// ���·�������
	if (bSendAgain)
	{
		InternetCloseHandle(m_hRequest);
		m_hRequest = HttpOpenRequest(m_hConnect, __HTTP_VERB_GET,
						m_strObjectName, HTTP_VERSION, NULL,
						szAcceptType, dwOpenFlags, NULL);		
		if (NULL == m_hRequest)
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			return ERR_HTTP_FAIL;
		}

		if (lpszResetHost != NULL)
			HttpAddRequestHeaders(m_hRequest, szHost, -1, HTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON);

		if (m_bStop)
		{
			CloseHandles();
			return ERR_HTTP_USER_STOP;
		}
		// SEND REQUEST
		if (!::HttpSendRequest(m_hRequest, NULL, 0L, NULL,0L))
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			return ERR_HTTP_FAIL;
		}

		if (m_bStop)
		{
			CloseHandles();
			return ERR_HTTP_USER_STOP;
		}
		
		dwSize = sizeof(DWORD);
		if (!HttpQueryInfo (m_hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwCode, &dwSize, NULL))
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			return ERR_HTTP_FAIL;
		}

		if (dwCode != HTTP_STATUS_OK)
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			return ERR_HTTP_FAIL;
		}
	}

	TCHAR szLastModified[__HTTP_LASTMODIFIED_BUF_SIZE + 1] = { 0 };
	dwSize = __HTTP_LASTMODIFIED_BUF_SIZE;
	// �õ��������ļ�������޸�ʱ��,���ܻ�ò���
	if (HttpQueryInfo(m_hRequest, HTTP_QUERY_LAST_MODIFIED, szLastModified, &dwSize, NULL))
	{
		HttpDateToSystemTime(szLastModified, stNetFile);
	}

	DWORD dwContentLength = 0;
	dwSize = sizeof(DWORD);
	// �õ��������ļ��Ĵ�С
	if (!HttpQueryInfo(m_hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &dwContentLength, &dwSize, NULL))
	{
		// ��Щ���������������ش��󣬵����������ļ�����ʱĬ��200K
		dwContentLength = 200 * 1024;
		httpDown.dwFileDownloadedSize = 0;
	}

	//httpDown.dwFileSize = dwContentLength;
	// �̵������õ��Ĵ�СΪʣ�µĴ�С
	httpDown.dwFileSize = dwContentLength + httpDown.dwFileDownloadedSize;
	m_dwFileSize = httpDown.dwFileSize;
	
	httpDown.nStatusType = DOWNLOAD_STATUS_FILESIZE;
	// �����ش��ڷ��������ļ���С��Ϣ
	if (!strTmpFile.IsEmpty())
	{
		Notify(WM_HTTPDOWN_NOTIFY_MSG, 0L, (LPARAM)&httpDown);
	}
	
	if (m_bStop)
	{
		CloseHandles();
		return ERR_HTTP_USER_STOP;
	}
	// ��������Ŀ¼
	if (FALSE == _CreateDirectory(strTmpFile, TRUE, NULL))
	{
		m_dwLastError = GetLastError();
		CloseHandles();
		return ERR_HTTP_CREATE_FILE_FAIL;
	}
	// ����������ʱ�ļ�
	hFileDown = CreateFile(strTmpFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFileDown == INVALID_HANDLE_VALUE)
	{
		m_dwLastError = GetLastError();
		CloseHandles();
		return ERR_HTTP_CREATE_FILE_FAIL;
	}

	FILETIME ftWrite;			
	SystemTimeToFileTime(&stNetFile, &ftWrite);
	SetFileTime(hFileDown, NULL, NULL, &ftWrite);

	if (bSendAgain)
	{
		SetFilePointer(hFileDown, 0L, 0L, FILE_BEGIN);
		SetEndOfFile(hFileDown);
	}
	else
	{
		SetFilePointer(hFileDown, 0L, 0L, FILE_END);
	}
		
							// �����ļ�дʱ��
	CHAR* pszData = new CHAR[__HTTP_READ_BUF_SIZE + 1]; // ���ļ�������
	DWORD dwRead = 0;

	httpDown.nStatusType = DOWNLOAD_STATUS_DOWNLOADEDSIZE;
	do 
	{
		ZeroMemory(pszData, __HTTP_READ_BUF_SIZE + 1);
		if (m_bStop)
		{
			// ��������ֹ������
			delete pszData;
			pszData = NULL;
			CloseHandles();

			FlushFileBuffers(hFileDown);
			SetEndOfFile(hFileDown);
			
			// ���������ص��ļ����޸�ʱ���޸�Ϊ��������ļ�ʱ��һ��
			SystemTimeToFileTime(&stNetFile, &ftWrite);
			SetFileTime(hFileDown, NULL, NULL, &ftWrite);
			CloseHandle(hFileDown);
			hFileDown = NULL;

			return ERR_HTTP_USER_STOP;
		}

		// ��ʱ����������
		if (!InternetReadFile(m_hRequest, pszData, __HTTP_READ_BUF_SIZE, &dwRead))
		{
			m_dwLastError = ::GetLastError();
			// ��������ֹ������
			delete pszData;
			pszData = NULL;

			m_dwLastError = ::GetLastError();
			CloseHandles();

			FlushFileBuffers(hFileDown);
			SetEndOfFile(hFileDown);
			
			// ���������ص��ļ����޸�ʱ���޸�Ϊ��������ļ�ʱ��һ��
			SystemTimeToFileTime(&stNetFile, &ftWrite);
			SetFileTime(hFileDown, NULL, NULL, &ftWrite);
			CloseHandle(hFileDown);
			hFileDown = NULL;

			return ERR_HTTP_RECEIVE_TIMEOUT;
		}

		httpDown.dwFileDownloadedSize += dwRead;
		
		// ������ߴ��ڷ������ش�С��Ϣ
		Notify(WM_HTTPDOWN_NOTIFY_MSG, 0L, (LPARAM)&httpDown);

		if (dwRead > 0)
		{
			DWORD dwWrited = 0;
			WriteFile(hFileDown, pszData, dwRead, &dwWrited, NULL);
		}
	}while (dwRead > 0);

	httpDown.nStatusType = DOWNLOAD_STATUS_DOWNLOADCOMPLETE;
	Notify(WM_HTTPDOWN_NOTIFY_MSG, 0L, (LPARAM)&httpDown);
	
	// û���������
	if ( httpDown.dwFileDownloadedSize < httpDown.dwFileSize )
	{
		CloseHandles();

		delete pszData;
		pszData = NULL;

		FlushFileBuffers(hFileDown);
		SetEndOfFile(hFileDown);

		// ���������ص��ļ����޸�ʱ���޸�Ϊ��������ļ�ʱ��һ��
		SystemTimeToFileTime(&stNetFile, &ftWrite);
		SetFileTime(hFileDown, NULL, NULL, &ftWrite);
		CloseHandle(hFileDown);	
		hFileDown = NULL;

		return ERR_HTTP_FAIL;
	}
	
	// httpDown.dwFileDownloadedSize = httpDown.dwFileSize;
	//Notify(WM_HTTPDOWN_NOTIFY_MSG, 0L, (LPARAM)&httpDown);

	TCHAR szConnection[__HTTP_HEADER_BUF_SIZE + 1] = { 0 };
	dwSize = __HTTP_HEADER_BUF_SIZE;
	if (!HttpQueryInfo(m_hRequest, HTTP_QUERY_CONNECTION, szConnection, &dwSize, NULL))
	{
		m_dwLastError = ::GetLastError();
		CloseHandles();
	}
	else
	{
		if ( 0 ==lstrcmpi(szConnection, _T("close")) )
		{
			CloseHandles();
		}
		else
		{
			InternetCloseHandle(m_hRequest);
			m_hRequest = NULL;
		}
	}	

	delete pszData;
	pszData = NULL;

	FlushFileBuffers(hFileDown);
	SetEndOfFile(hFileDown);

	// ���������ص��ļ����޸�ʱ���޸�Ϊ��������ļ�ʱ��һ��
	SystemTimeToFileTime(&stNetFile, &ftWrite);
	SetFileTime(hFileDown, NULL, NULL, &ftWrite);
	CloseHandle(hFileDown);	
	hFileDown = NULL;
	
	// ��ָ�����ش洢�ļ����ļ����ڣ�ɾ�����ļ����������ļ�����ʱ�ļ�Ϊָ���洢�ļ�
	if (PathFileExists(lpszSaveFileName))
	{
		SetFileAttributes(lpszSaveFileName, FILE_ATTRIBUTE_NORMAL);
		DeleteFile(lpszSaveFileName);
	}
	if (!MoveFile(strTmpFile, lpszSaveFileName))
	{
		return ERR_HTTP_RENAME_FILE_FAIL;
	};

	return ERR_HTTP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////
//	��������HRESULT PostData(LPCTSTR lpszURL,LPVOID lpBuffer, DWORD cbBuffer,
//				LPBYTE lpData, LPDWORD lpcbData );
//	��  ;�����������������
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		lpszURL			  : [in]�������ݷ������գң�
//		lpBuffer          : [in]Ҫ���͵�����
//      cbBuffer          : [in]Ҫ���͵����ݵ��ֽڳ���
//      lpData            : [out]�������ݻ�����
//      lpcbData          : [in,out]����ʱΪlpData�������ֽ���,����ʱΪ���������ֽ���
//      bAddPostHeadFlag  : [in]�Ƿ���Post����ǰ�Զ���InPost=��־
//	����ֵ��HRESULT
//		ERR_HTTP_USER_STOP		: �û�ֹͣ����(������StopDownload����)
//		ERR_HTTP_FAIL			: ����ʧ��
//		ERR_HTTP_SUCCESS		: ���ͳɹ�
//		ERR_HTTP_REDIRECT		: �ض���HTTP
//		ERR_HTTP_INVALID_URL	: ��ЧURL
//      ERR_HTTP_BUFFER_TOO_SMALL: ���ջ�����̫С
/////////////////////////////////////////////////////////////////////////////////
HRESULT CHttpDownload::PostData(LPCTSTR lpszURL, LPVOID lpBuffer, DWORD cbBuffer,
				LPBYTE lpData, LPDWORD lpcbData, BOOL bAddPostHeadFlag)
{
	m_strRedirectURL.Empty();
	_ASSERT(lpszURL);
	if (!ParseUrl(lpszURL))
		return ERR_HTTP_INVALID_URL;

	// ��ʼ��WinINet�������������
	if (!Connect())
	{
		return ERR_HTTP_FAIL;
	}

	// ��������ֹ
	if (m_bStop)
	{
		CloseHandles();
		return ERR_HTTP_USER_STOP;
	}

	static LPCTSTR szAcceptType[]= { __HTTP_ACCEPT_TYPE, 0 };
	DWORD dwOpenFlags = INTERNET_FLAG_NEED_FILE | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION;
	//if (!bAutoRedirect)
	dwOpenFlags |= INTERNET_FLAG_NO_AUTO_REDIRECT;

	m_hRequest = HttpOpenRequest(m_hConnect, __HTTP_VERB_POST, m_strObjectName, 
			NULL, NULL, szAcceptType, dwOpenFlags, 0);

	// ERROR return
	if (NULL == m_hRequest)
	{
		m_dwLastError = ::GetLastError();
		CloseHandles();
		return ERR_HTTP_FAIL;
	}
	
	DWORD dwSendBufferCount = cbBuffer;
	if (bAddPostHeadFlag)
		dwSendBufferCount += lstrlen(__HTTP_POST_HEAD_FLAGS);
	LPVOID lpSendBuffer = new BYTE[dwSendBufferCount];

	ZeroMemory(lpSendBuffer, dwSendBufferCount);
	if (bAddPostHeadFlag)
	{
		memcpy(lpSendBuffer, __HTTP_POST_HEAD_FLAGS, lstrlen(__HTTP_POST_HEAD_FLAGS));
		memcpy( (LPBYTE)lpSendBuffer+lstrlen(__HTTP_POST_HEAD_FLAGS), lpBuffer, cbBuffer);
	}
	else
	{
		memcpy( (LPBYTE)lpSendBuffer, lpBuffer, cbBuffer);
	}
	
    if	(!HttpSendRequest( m_hRequest, __HTTP_POST_CONTENT_TYPE, lstrlen(__HTTP_POST_CONTENT_TYPE), lpSendBuffer, dwSendBufferCount))
    {
		delete lpSendBuffer;
        m_dwLastError = ::GetLastError();
		CloseHandles();
		if (m_dwLastError == ERROR_INTERNET_TIMEOUT)
		{
			return ERR_HTTP_SEND_TIMEOUT;
		}
		else
			return ERR_HTTP_FAIL;
    }
	delete lpSendBuffer; 
	lpSendBuffer = NULL;

	DWORD dwCode = 0;
	DWORD dwSize = sizeof(DWORD) ;
	if (!HttpQueryInfo (m_hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwCode, &dwSize, NULL))
	{
		m_dwLastError = ::GetLastError();
		CloseHandles();
		return ERR_HTTP_FAIL;
	}
	
	switch (dwCode)
	{
	// �ض���
	case HTTP_STATUS_REDIRECT:
	case HTTP_STATUS_MOVED:
		{
			TCHAR szLocation[__HTTP_LOCATION_BUF_SIZE + 1] = { 0 };
			dwSize = __HTTP_LOCATION_BUF_SIZE;
			if (!HttpQueryInfo (m_hRequest, HTTP_QUERY_LOCATION, szLocation, &dwSize, NULL))
			{
				m_dwLastError = ::GetLastError();
				CloseHandles();
				return ERR_HTTP_FAIL;
			}

			m_strRedirectURL = szLocation;
			InternetCloseHandle(m_hRequest);
			m_hRequest = NULL;
			return ERR_HTTP_REDIRECT;
		}
	// 200 OK
	case HTTP_STATUS_OK:
		break;
	default:
		{
			CloseHandles();
			return ERR_HTTP_FAIL;
		}
	}
	
	// ��������ֹ
	if (m_bStop)
	{
		CloseHandles();
		return ERR_HTTP_USER_STOP;
	}

	// ��ȡ�Ż����ݵ�ָ��BUFFER��
	CHAR pcReadBuffer[__HTTP_READ_BUF_SIZE];
	ZeroMemory(pcReadBuffer, sizeof( pcReadBuffer ));
	DWORD dwBytesRead = 0;

	LPBYTE pByte = (LPBYTE)lpData;			// ��¼��ȡλ��ָ��
	DWORD dwDataMaxCount = *lpcbData;       // ���洫��BUFFER��С
	*lpcbData = 0L;
	do
	{	
		dwBytesRead = 0;
		// ��������ֹ
		if (m_bStop)
		{
			CloseHandles();
			return ERR_HTTP_USER_STOP;
		}
		if (InternetReadFile(m_hRequest, pcReadBuffer, __HTTP_READ_BUF_SIZE-1, &dwBytesRead))
		{
			if (dwBytesRead && ((dwBytesRead + *lpcbData) <= dwDataMaxCount))
			{
				memcpy(pByte, pcReadBuffer, dwBytesRead);
				
				pByte += dwBytesRead;
				*lpcbData += dwBytesRead;
			}
			else
			{
				*lpcbData += dwBytesRead;
			}
		}
		else
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			
			return ERR_HTTP_FAIL;
		}
		
	}while (dwBytesRead>0);
		
	*(++pByte) = _T('\0');
	InternetCloseHandle(m_hRequest);
	m_hRequest = NULL;

	if ( *lpcbData > dwDataMaxCount )
		return ERR_HTTP_BUFFER_TOO_SMALL;

	return ERR_HTTP_SUCCESS;
}

HRESULT CHttpDownload::GetContent(LPCTSTR lpszURL, LPVOID lpContent, 
								  LPDWORD lpcbContent, BOOL bAutoRedirect)
{
	m_strRedirectURL.Empty();
	_ASSERT(lpszURL);
	if (!ParseUrl(lpszURL))
		return ERR_HTTP_INVALID_URL;

	// ��ʼ��WinINet�������������
	if (!Connect())
	{
		return ERR_HTTP_FAIL;
	}

	// ��������ֹ
	if (m_bStop)
	{
		CloseHandles();
		return ERR_HTTP_USER_STOP;
	}

	static LPCTSTR szAcceptType[]= { __HTTP_ACCEPT_TYPE, 0 };
	DWORD dwOpenFlags = INTERNET_FLAG_NEED_FILE | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION;
	
	if (!bAutoRedirect)
		dwOpenFlags |= INTERNET_FLAG_NO_AUTO_REDIRECT;

	// Open Request
	m_hRequest = HttpOpenRequest(m_hConnect, __HTTP_VERB_GET,
					m_strObjectName, HTTP_VERSION, NULL,
					szAcceptType, dwOpenFlags, NULL);
	// ERROR return
	if (NULL == m_hRequest)
	{
		m_dwLastError = ::GetLastError();
		CloseHandles();
		return ERR_HTTP_FAIL;
	}
	
	// SEND REQUEST
	if (!::HttpSendRequest(m_hRequest, NULL, 0,	NULL, 0))
	{
		m_dwLastError = ::GetLastError();
		CloseHandles();
		
		switch (m_dwLastError)
		{
		case ERROR_INTERNET_CANNOT_CONNECT:
		case ERROR_INTERNET_NAME_NOT_RESOLVED:
			return ERR_HTTP_NET_ERROR;
		case ERROR_INTERNET_TIMEOUT:
			return ERR_HTTP_SEND_TIMEOUT;
		default:
			return ERR_HTTP_FAIL;;
		}
	}

	DWORD dwCode = 0;
	DWORD dwSize = sizeof(DWORD) ;
	if (!HttpQueryInfo (m_hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwCode, &dwSize, NULL))
	{
		m_dwLastError = ::GetLastError();
		CloseHandles();
		return ERR_HTTP_FAIL;
	}
	
	switch (dwCode)
	{
	// �ض���
	case HTTP_STATUS_REDIRECT:
	case HTTP_STATUS_MOVED:
		{
			TCHAR szLocation[__HTTP_LOCATION_BUF_SIZE + 1] = { 0 };
			dwSize = __HTTP_LOCATION_BUF_SIZE;
			if (!HttpQueryInfo (m_hRequest, HTTP_QUERY_LOCATION, szLocation, &dwSize, NULL))
			{
				m_dwLastError = ::GetLastError();
				CloseHandles();
				return ERR_HTTP_FAIL;
			}

			m_strRedirectURL = szLocation;
			InternetCloseHandle(m_hRequest);
			m_hRequest = NULL;
			return ERR_HTTP_REDIRECT;
		}
	// 200 OK
	case HTTP_STATUS_OK:
		break;
	default:
		{
			CloseHandles();
			return ERR_HTTP_FAIL;
		}
	}
	
	// ��������ֹ
	if (m_bStop)
	{
		CloseHandles();
		return ERR_HTTP_USER_STOP;
	}

	// ��ȡ�Ż����ݵ�ָ��BUFFER��
	CHAR pcReadBuffer[__HTTP_READ_BUF_SIZE];
	ZeroMemory(pcReadBuffer, sizeof( pcReadBuffer ));
	DWORD dwBytesRead = 0;

	LPBYTE pByte = (LPBYTE)lpContent;			// ��¼��ȡλ��ָ��
	DWORD dwDataMaxCount = *lpcbContent;       // ���洫��BUFFER��С
	*lpcbContent = 0L;
	do
	{	
		dwBytesRead = 0;
		// ��������ֹ
		if (m_bStop)
		{
			CloseHandles();
			return ERR_HTTP_USER_STOP;
		}

		if (InternetReadFile(m_hRequest, pcReadBuffer, __HTTP_READ_BUF_SIZE-1, &dwBytesRead))
		{
			if (dwBytesRead > 0) 
			{
				if ((dwBytesRead + *lpcbContent) <= dwDataMaxCount)
				{
					memcpy(pByte, pcReadBuffer, dwBytesRead);
					
					pByte += dwBytesRead;
					*lpcbContent += dwBytesRead;
				}
				else	// ������̫С������������Ϊֹ
				{
					DWORD dwRemain = dwDataMaxCount - *lpcbContent;
					memcpy(pByte, pcReadBuffer, dwRemain);
					*lpcbContent += dwRemain;
					break;
				}
			}
		}
		else
		{
			m_dwLastError = ::GetLastError();
			CloseHandles();
			
			return ERR_HTTP_FAIL;
		}
		
	}while (dwBytesRead>0);

	InternetCloseHandle(m_hRequest);
	m_hRequest = NULL;

	if (dwBytesRead > 0)	// ��Ϊ������̫С��ֹͣ���ļ�
		return ERR_HTTP_BUFFER_TOO_SMALL;

	return ERR_HTTP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////
//	��������BOOL ParseUrl(LPCTSTR lpszUrl)
//	��  ;����������URL�ĺϷ��Լ����URL(�ڲ�ʹ��)
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		lpszUrl		: ����ֵ�URL
//	����ֵ���ɹ���TRUE��ʧ��FALSE
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpDownload::ParseUrl(LPCTSTR lpszUrl)
{
	if (NULL == lpszUrl || lstrlen(lpszUrl) <=0)
		return FALSE;

	CSplitURL url;
	
	if (!url.Split(lpszUrl))
	{
		return FALSE;
	}

	CString strHostName;
	strHostName = url.GetHostName();

	// �鿴��������ַ�Ƿ�ı�
	if (m_strServerName.CompareNoCase(strHostName) != 0)
	{
		m_bServerAlter = TRUE;
	}
	else
	{
		m_bServerAlter = FALSE;
	}
	m_strServerName = strHostName;
	
	/*
	CString strUserName, strPassword
	strUserName = url.GetUserName();
	strPassword = url.GetPassword();
	if (!strUserName.IsEmpty())
	{
		SetAuthorization(strUserName, strPassword);
	}

	//*/
	m_nServerPort = url.GetPort() ? url.GetPort() :INTERNET_DEFAULT_HTTP_PORT;

	m_strObjectName.Format(_T("%s%s"), url.GetURLPath(), url.GetExtraInfo()); 

	return TRUE;
}

// �ַ���ת��Ϊ���ִ�(�ڲ�ʹ��)
static int IntVal(CString strVal)
{
	int nVal = 0;
	strVal.TrimLeft();
	for(int ndx = 0; ndx < strVal.GetLength(); ++ndx)
		nVal = nVal*10 + strVal.GetAt(ndx) - _T('0');

	return nVal;
}

// ת���·�Ϊ����(�ڲ�ʹ��)
static int MonthFromStr(const CString& str)
{
	LPCTSTR aMonths[] = {
		_T("xxx"), _T("jan"), _T("feb"), _T("mar"), _T("apr"), _T("may"), _T("jun"),
		_T("jul"), _T("aug"), _T("sep"), _T("oct"), _T("nov"), _T("dec") };
	int nMonth = 1;
	for(nMonth = 1; nMonth <= 12; ++nMonth)
	{
		if (str.CompareNoCase(aMonths[nMonth]) == 0)
			break;
	}

	return nMonth;
}


//////////////////////////////////////////////////////////////////////////////////
//	��������BOOL HttpDateToSystemTime(LPCTSTR lpszTime, SYSTEMTIME &st)
//	��  ;��ת������ʱ�䵽ϵͳʱ��(�ڲ�ʹ��)
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		lpszTime		: HTTP��ʽϵͳʱ��
//      st			    : ��ת����ϵͳʱ��
//	����ֵ���ɹ���TRUE��ʧ��FALSE
//  ˵����// Dow, dd Mon year hh:mm:ss GMT
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpDownload::HttpDateToSystemTime(LPCTSTR lpszTime, SYSTEMTIME &st)
{
	ZeroMemory(&st, sizeof(SYSTEMTIME));
		
	CString strHttp = lpszTime;
	// assume we couldn't get a good time conversion....
	BOOL bOk = FALSE;

	int ndx = 0;
	switch(strHttp.GetAt(3))
	{
	case ',':
		// read RFC-1123 (preferred)....
		st.wDay = IntVal(strHttp.Mid(5,2));
		st.wMonth = MonthFromStr(strHttp.Mid(8,3));
		st.wYear = IntVal(strHttp.Mid(12,4));
		st.wHour = IntVal(strHttp.Mid(17,2));
		st.wMinute = IntVal(strHttp.Mid(20,2));
		st.wSecond = IntVal(strHttp.Mid(23,2));
		break;
	case ' ':
		// read ANSI-C time format....
		st.wDay = IntVal(strHttp.Mid(8,2));
		st.wMonth = MonthFromStr(strHttp.Mid(4,3));
		st.wYear = IntVal(strHttp.Mid(20,4));
		st.wHour = IntVal(strHttp.Mid(11,2));
		st.wMinute = IntVal(strHttp.Mid(14,2));
		st.wSecond = IntVal(strHttp.Mid(17,2));
		break;
	default:
		if ((ndx = strHttp.Find(_T(", "))) != -1)
		{
			st.wDay = IntVal(strHttp.Mid(ndx+2,2));
			st.wMonth = MonthFromStr(strHttp.Mid(ndx+5,3));
			st.wYear = IntVal(strHttp.Mid(ndx+9,2));
			st.wHour = IntVal(strHttp.Mid(ndx+12,2));
			st.wMinute = IntVal(strHttp.Mid(ndx+15,2));
			st.wSecond = IntVal(strHttp.Mid(ndx+18,2));
			// add the correct century....
			st.wYear += (st.wYear > 50)?1900:2000;
		}
		break;
	}
	// if year not zero....
	if (st.wYear != 0)
	{
		return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
//	��������BOOL SetProxy(LPCTSTR lpszProxyServer,
//					USHORT nProxyPort, 
//					BOOL bUseProxy /* =TRUE */,
//					BOOL bProxyAuthorization /* =FALSE */,
//					LPCTSTR lpszProxyUsername /* =NULL */,
//					LPCTSTR lpszProxyPassword /* =NULL */)
//	��  ;�����ô�����������ʲ���
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		lpszProxyServer		: �����������
//      nProxyPort			: ����������˿�
//		bUseProxy			: �Ƿ�ʹ�ô���(��ֵΪFALSEʱ��ǰ����������������)
//		bProxyAuthorization : ����������Ƿ���Ҫ�����֤
//		lpszProxyUsername   : ��������������֤�û���
//		lpszProxyUsername   : ��������������֤����
//	����ֵ���ɹ���TRUE��ʧ��FALSE
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpDownload::SetProxy(LPCTSTR lpszProxyServer,
							USHORT nProxyPort, 
							BOOL bUseProxy /* =TRUE */,
							BOOL bProxyAuthorization /* =FALSE */,
							LPCTSTR lpszProxyUsername /* =NULL */,
							LPCTSTR lpszProxyPassword /* =NULL */)
{
	m_bUseProxy				= bUseProxy;
	m_strProxyServer		= lpszProxyServer;
	m_nProxyPort			= nProxyPort;
	m_bProxyAuthorization	= bProxyAuthorization;
	m_strProxyUsername		= lpszProxyUsername;
	m_strProxyPassword		= lpszProxyPassword;
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//	��������void SetAuthorization(
//				LPCTSTR lpszUsername,
//				LPCTSTR lpszPassword,
//				BOOL bAuthorization/* = TRUE*/) 
//	��  ;������WWW��֤��Ϣ(���ʱ�������ҳ��ʱ��Ҫ)
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		lpszUsername   : ����ҳ����û���
//		lpszPassword   : ����
//		bAuthorization : �Ƿ���Ҫ��֤(��ֵΪFALSEʱ��ǰ����������������)
//	����ֵ��void
////////////////////////////////////////////////////////////////////////////////
void CHttpDownload::SetAuthorization(LPCTSTR lpszUsername,LPCTSTR lpszPassword,BOOL bAuthorization/* = TRUE*/)
{
	if(bAuthorization && lpszUsername != NULL)
	{
		m_bAuthorization = TRUE;
		m_strUsername	 = lpszUsername;
		m_strPassword	 = lpszPassword;
	}
	else
	{
		m_bAuthorization = FALSE;
		m_strUsername	 = _T("");
		m_strPassword	 = _T("");
	}
}

//////////////////////////////////////////////////////////////////////////////////
//	��������BOOL CloseHandles(void)
//	��  ;���ر������������е�INTERNET������ڲ�ʹ�ã�
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ������
//	����ֵ���ɹ�-TRUE��ʧ��-FALSE
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpDownload::CloseHandles()
{
	if (m_hRequest)
	{
		InternetCloseHandle(m_hRequest);
		m_hRequest = NULL;
	}

	if (m_hConnect)
	{
		InternetCloseHandle(m_hConnect);
		m_hConnect = NULL;
	}

	if (m_hInternet)
	{
		InternetCloseHandle(m_hInternet);
		m_hInternet = NULL;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//	��������BOOL SetNotifyHwnd(HWND hNotifyWnd)
//	��  ;�����ý���������Ϣ�Ĵ��ھ��
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����hNotifyWnd : ����������Ϣ�Ĵ��ھ��
//	����ֵ���Ϸ�����Ϊ-TRUE������-FALSE
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpDownload::SetNotifyHwnd(HWND hNotifyWnd)
{
	// hNotifyWnd ����Ч���ھ��
	if (hNotifyWnd && IsWindow(hNotifyWnd))
	{
		m_hNotifyWnd = hNotifyWnd;
		return TRUE;
	}
	else
	{
		m_hNotifyWnd = NULL;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
//	��������void StopDownload()
//	��  ;��ֹͣ����
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ������
//	����ֵ����
/////////////////////////////////////////////////////////////////////////////////
void CHttpDownload::StopDownload()
{
	m_bStop = TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//	��������LPCTSTR GetRedirectURL()
//	��  ;���õ��ض����ģգң�
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ������
//	����ֵ��URL�ַ���
//  ˵��  : ������Download�󷵻�ֵΪERR_HTTP_REDIRECT���ٵ��ô˺����õ��ض���URL
/////////////////////////////////////////////////////////////////////////////////
LPCTSTR CHttpDownload::GetRedirectURL()
{
	return m_strRedirectURL;
}

//////////////////////////////////////////////////////////////////////////////////
// 
// 
BOOL CHttpDownload::Notify(int msg, WPARAM wp, LPARAM lp)
{
	if (m_hNotifyWnd && IsWindow(m_hNotifyWnd))
	{
		return SendMessage(m_hNotifyWnd, msg, wp, lp) != NULL ? TRUE : FALSE;
	}

	//switch(msg)
	//{
	//case WM_HTTPDOWN_NOTIFY_MSG:
	//	if(lp != NULL)
	//	{
	//		HTTPDOWNLOADSTATUS* ds = (HTTPDOWNLOADSTATUS*)lp;
	//		switch(ds->nStatusType)
	//		{
	//		case DOWNLOAD_STATUS_FILENAME:
	//			break;

	//		case DOWNLOAD_STATUS_FILESIZE:
	//			// here, may get 'filename' and 'filesize'
	//			break;
	//		}
	//	}
	//	break;
	//}

	return TRUE;
}
