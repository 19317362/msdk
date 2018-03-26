// HttpDownload.h: interface for the CHttpDownload class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_HTTPDOWNLOAD_H__)
#define _HTTPDOWNLOAD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Wininet.h>
#pragma comment(lib, "wininet.lib")

////////////////////////////////////////////////////////////////////////////////
// �Զ�����Ϣ
//#define WM_HTTPDOWN_NOTIFY_MSG (WM_USER + 2005) // 2005�������г�ͻ
#define WM_HTTPDOWN_NOTIFY_MSG (WM_USER + 2008)

// �ṹ����, ����������Ϣ
typedef struct _tagHttpDownloadStatus
{
	UINT	nStatusType;					// ״̬����: 1�� �ļ���; 2���ļ���С��3���ļ��Ѿ����ش�С
	DWORD	dwFileSize;					    // �ļ��ܵĴ�С
	DWORD	dwFileDownloadedSize;		    // �Ѿ����ش�С
	CString	strFileName;				    // �ļ���
}HTTPDOWNLOADSTATUS,*PHTTPDOWNLOADSTATUS;
// �����Զ�����Ϣ�ṹ״̬����,��ӦHTTPDOWNLOADSTATUS.nStatusType
enum
{
	DOWNLOAD_STATUS_FILENAME			= 1, // �ļ���
	DOWNLOAD_STATUS_FILESIZE			= 2,	// �ļ���С
	DOWNLOAD_STATUS_DOWNLOADEDSIZE		= 3,	// �ļ��Ѿ����ش�С
	DOWNLOAD_STATUS_DOWNLOADCOMPLETE    = 4, // �������
};
//////////////////////////////////////////////////////////////////////////
//LRESULT OnHttpdownloadNotify(WPARAM /*wParam*/, LPARAM lParam,)
//{
//	PHTTPDOWNLOADSTATUS pHttpStatus = (PHTTPDOWNLOADSTATUS)lParam;
//
//	CString strFileName;
//	LONG lFileSize =0, lDownloadedSize = 0
//	switch ( pHttpStatus->nStatusType )
//	{
//	case DOWNLOAD_STATUS_FILENAME:
//		strFileName = pHttpStatus->strFileName;
//		break;
//	case DOWNLOAD_STATUS_FILESIZE:
//		lFileSize = pHttpStatus->dwFileSize;
//		break;
//	case DOWNLOAD_STATUS_DOWNLOADEDSIZE:
//		lDownloadedSize = pHttpStatus->dwFileDownloadedSize;
//		break;
//	default:
//		break;
//		}
//	return 0;
//}
//////////////////////////////////////////////////////////////////////////

// Download ��������ֵ
enum
{
	ERR_HTTP_SUCCESS				=	0x00000000,			// ���سɹ�
	ERR_HTTP_FAIL					=	0xFFFFFFFF,			// ����ʧ��
	ERR_HTTP_REDIRECT				=   0x00000001,			// �ض���HTTP
	ERR_HTTP_USER_STOP              =   0x00000002,			// �û�ֹͣ����(������StopDownload����)
	ERR_HTTP_INVALID_URL			=   0x80000001,         // ��ЧURL
	ERR_HTTP_DEST_FILENAME_EMPTY	=   0x80000002,			// ���ش洢�ļ�Ϊ��
	ERR_HTTP_CREATE_FILE_FAIL		=   0x80000003,			// ���������ļ�ʧ��
	ERR_HTTP_RENAME_FILE_FAIL       =   0x80000004,			// �����������ļ�ʧ��
	ERR_HTTP_SEND_TIMEOUT           =   0x80000005,			// ����ʱ
	ERR_HTTP_BUFFER_TOO_SMALL       =   0x80000006,			// buffer too samll
	ERR_HTTP_NOT_FIND				=   0x80000007,			// ���󲻴���
	ERR_HTTP_NET_ERROR              =   0x80000008,         // �������
	//HTTP_RESULT_CONNECT_FAIL    =   0x80000002,         // CONNECT Error 
	ERR_HTTP_RECEIVE_TIMEOUT        =   0x80000009,			// ����ʱ
	ERR_HTTP_FORBIDDEN_ACCESS       =   0x80000010,			// �ܾ�����
	ERR_HTTP_SERVER_ERROR           =   0x80000011,			// ����������internal server error����500
};

// SetConnectType����
enum
{
	CONNECT_TYPE_IE                = 0,	// ʹ�ãɣ�����
	CONNECT_TYPE_DIRECT            = 1, // ֱ������
	CONNECT_TYPE_PROXY             = 3  // ʹ�ô���
};
class CHttpDownload  
{
public:
	//////////////////////////////////////////////////////////////////////////////////
	//	��������HRESULT Download(LPCTSTR lpszURL,
	//				LPCTSTR lpszSaveFileName	/*= NULL*/��
	//				BOOL    bAutoRedirect /*= FALSE*/)
	//	��  ;��������ڣ����ô˺�������ʼ���ع���
	//	��ȫ�ֱ�����Ӱ�죺��
	//	��  ����
	//		lpszURL			  : [in]�����ص�����URL
	//		lpszSaveFileName  : [in]���ر����ļ���(��Ҫ�õ��ض���URLʱ����ΪNULL)
	//      bAutoRedirect     : [in]�Ƿ��Զ��ض���
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
	HRESULT Download(LPCTSTR lpszURL, LPCTSTR lpszFileName = NULL, 
		BOOL bAutoRedirect = FALSE, LPCTSTR lpszResetHost = NULL);
	
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
	HRESULT PostData(LPCTSTR lpszURL, LPVOID lpBuffer, DWORD cbBuffer,
				LPBYTE lpData, LPDWORD lpcbData , BOOL bAddPostHeadFlag = FALSE);

	//////////////////////////////////////////////////////////////////////////////////
	//	��������GetContent
	//	��  ;����Internet�ļ�����
	//	��ȫ�ֱ�����Ӱ�죺��
	//	��  ����
	//		lpszURL			  : [in]�������ݷ������գң�
	//		lpContent         : [in]�����ļ����ݵĻ�����
	//      lpcbContent       : [in/out]�����ļ����ݵĻ��������ֽڳ���
	//      bAutoRedirect     : [in]�Ƿ��Զ��ض���
	//	����ֵ��HRESULT
	//		ERR_HTTP_USER_STOP		: �û�ֹͣ����(������StopDownload����)
	//		ERR_HTTP_FAIL			: ʧ��
	//		ERR_HTTP_SUCCESS		: �ɹ�
	//		ERR_HTTP_REDIRECT		: �ض���HTTP
	//		ERR_HTTP_INVALID_URL	: ��ЧURL
	//      ERR_HTTP_BUFFER_TOO_SMALL: ���ջ�����̫С
	/////////////////////////////////////////////////////////////////////////////////
	HRESULT GetContent(LPCTSTR lpszURL, LPVOID lpContent, LPDWORD lpcbContent, BOOL bAutoRedirect);

	//////////////////////////////////////////////////////////////////////////////////
	//	��������void StopDownload()
	//	��  ;��ֹͣ����
	//	��ȫ�ֱ�����Ӱ�죺��
	//	��  ������
	//	����ֵ����
	/////////////////////////////////////////////////////////////////////////////////
	void    StopDownload();

	//////////////////////////////////////////////////////////////////////////////////
	//	��������BOOL SetNotifyHwnd( HWND hNotifyWnd )
	//	��  ;�����ý���������Ϣ�Ĵ��ھ��
	//	��ȫ�ֱ�����Ӱ�죺��
	//	��  ����hNotifyWnd : [in]����������Ϣ�Ĵ��ھ��
	//	����ֵ���Ϸ�����Ϊ-TRUE������-FALSE
	/////////////////////////////////////////////////////////////////////////////////
	BOOL	SetNotifyHwnd(HWND hNotifyWnd);

	//////////////////////////////////////////////////////////////////////////////////
	//	��������LPCTSTR GetRedirectURL()
	//	��  ;���õ��ض����ģգң�
	//	��ȫ�ֱ�����Ӱ�죺��
	//	��  ������
	//	����ֵ��URL�ַ���
	//  ˵��  : ������Download�󷵻�ֵΪHTTP_RES_REDIRECT���ٵ��ô˺����õ��ض���URL
	/////////////////////////////////////////////////////////////////////////////////
	LPCTSTR  GetRedirectURL();
	
	//////////////////////////////////////////////////////////////////////////////////
	//	��������BOOL SetProxy( LPCTSTR lpszProxyServer,
	//					USHORT nProxyPort, 
	//					BOOL bUseProxy /* =TRUE */,
	//					BOOL bProxyAuthorization /* =FALSE */,
	//					LPCTSTR lpszProxyUsername /* =NULL */,
	//					LPCTSTR lpszProxyPassword /* =NULL */ )
	//	��  ;�����ô�����������ʲ���
	//	��ȫ�ֱ�����Ӱ�죺��
	//	��  ����
	//		lpszProxyServer		: [in]�����������
	//      nProxyPort			: [in]����������˿�
	//		bUseProxy			: [in]�Ƿ�ʹ�ô���(��ֵΪFALSEʱ��ǰ����������������)
	//		bProxyAuthorization : [in]����������Ƿ���Ҫ�����֤
	//		lpszProxyUsername   : [in]��������������֤�û���
	//		lpszProxyUsername   : [in]��������������֤����
	//	����ֵ���ɹ���TRUE��ʧ��FALSE
	/////////////////////////////////////////////////////////////////////////////////
	BOOL SetProxy(LPCTSTR lpszProxyServer,
					USHORT nProxyPort, 
					BOOL bUseProxy = TRUE,
					BOOL bProxyAuthorization = FALSE,
					LPCTSTR lpszProxyUsername = NULL,
					LPCTSTR lpszProxyPassword = NULL);

	//////////////////////////////////////////////////////////////////////////////////
	//	��������void SetAuthorization(
	//				LPCTSTR lpszUsername,
	//				LPCTSTR lpszPassword,
	//				BOOL bAuthorization/* = TRUE*/ ) 
	//	��  ;������WWW��֤��Ϣ(���ʱ�������ҳ��ʱ��Ҫ)
	//	��ȫ�ֱ�����Ӱ�죺��
	//	��  ����
	//		lpszUsername   : [in]����ҳ����û���
	//		lpszPassword   : [in]����
	//		bAuthorization : [in]�Ƿ���Ҫ��֤(��ֵΪFALSEʱ��ǰ����������������)
	//	����ֵ��void
	////////////////////////////////////////////////////////////////////////////////
	void SetAuthorization(LPCTSTR lpszUsername,LPCTSTR lpszPassword,BOOL bAuthorization = TRUE);

	//////////////////////////////////////////////////////////////////////////////////
	//	��������void SetConnectType(INT nType /*= CONNECT_TYPE_IE*/) 
	//	��  ;��������������
	//	��ȫ�ֱ�����Ӱ�죺��
	//	��  ����
	//		nType   : [in]���ͣ�CONNECT_TYPE_IE/* 0 - IE; 1 - ֱ������; 3 - Proxy*/��
	//	����ֵ��void
	////////////////////////////////////////////////////////////////////////////////
	void SetConnectType(INT nType = CONNECT_TYPE_IE/* 0 - IE; 1 - ֱ������; 3 - Proxy*/) { m_nConnectType = nType; }

	DWORD GetLastError(){return m_dwLastError;}

	DWORD GetFileSize() {return m_dwFileSize;}

public:
	CHttpDownload();
	virtual ~CHttpDownload();

protected:
	BOOL ParseUrl(LPCTSTR lpszUrl);
	BOOL Connect();
	BOOL HttpDateToSystemTime(LPCTSTR lpszTime, SYSTEMTIME& st);
	BOOL CloseHandles();
	virtual BOOL Notify(int msg, WPARAM wp, LPARAM lp);

protected:
	HINTERNET m_hInternet;					// internet open handle
	HINTERNET m_hConnect;					// internet connection handle
	HINTERNET m_hRequest;					// internet request handle

	// ��������������
	CString         m_strUserAgent;         // UserAgent (IE version;os version)
	CString			m_strServerName;		// ��������(hostname,���磺www.Margin.com.cn)
	INTERNET_PORT   m_nServerPort;			// �������˿�
	BOOL			m_bAuthorization;		// ���������Ƿ���Ҫ�����֤
	CString			m_strUsername;			// ����������½�û���
	CString			m_strPassword;			// ����������½����
	CString			m_strObjectName;		// ���URL·��(���磺/register/pcver/AutoUpgradePad/ver2005/MSAGENT/NewVer/MsAgent_exe.zip)

	// ��Դ����������
	CString			m_strProxyServer;		// �����������
	INTERNET_PORT	m_nProxyPort;			// ����������˿�
	BOOL			m_bUseProxy;			// �Ƿ�ʹ�ô��������
	BOOL			m_bProxyAuthorization;	// ����������Ƿ���Ҫ�����֤
	CString			m_strProxyUsername;     // �������������½�û���
	CString			m_strProxyPassword;		// �������������½����

	BOOL			m_bStop;				// �û���ֹ����������
	BOOL			m_bServerAlter;         // �������ʱ���ڶ���URL��ַ��HostName�Ƿ�ı�
	
	CString         m_strRedirectURL;       // �ض����ַ
	HWND			m_hNotifyWnd;           // ����������Ϣ�Ĵ��ھ��

	DWORD			m_dwLastError;          // �洢�ڲ�������
	DWORD           m_nConnectType;
	DWORD			m_dwFileSize;
};

#endif // !defined(_HTTPDOWNLOAD_H__)
