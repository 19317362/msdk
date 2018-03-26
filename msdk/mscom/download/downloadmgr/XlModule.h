#pragma once

#include <util/callapi.h>


// ���нṹ�嶨�尴1�ֽڶ���
#pragma pack(push, 1)

struct DownTaskParam
{
	DownTaskParam()
	{
		memset(this, 0, sizeof(DownTaskParam));
		nReserved1		 = 5;
		bReserved			 = FALSE;
		DisableAutoRename	 = FALSE;
		IsOnlyOriginal		 = FALSE;
		IsResume			 = TRUE;
	}
	int nReserved;
	wchar_t szTaskUrl[2084];          // ����URL
	wchar_t szRefUrl[2084];           // ����ҳ
	wchar_t szCookies[4096];          // �����cookie
	wchar_t szFilename[MAX_PATH];     // ���ر����ļ���.
	wchar_t szReserved0[MAX_PATH];
	wchar_t szSavePath[MAX_PATH];     // �ļ�����Ŀ¼
	HWND  hReserved;
	BOOL bReserved; 
	wchar_t szReserved1[64];
	wchar_t szReserved2[64];
	BOOL IsOnlyOriginal;            // �Ƿ�ֻ��ԭʼ��ַ����
	UINT nReserved1;
	BOOL DisableAutoRename;         // ��ֹ��������
	BOOL IsResume;                  // �Ƿ�������
	DWORD reserved[2048];
};
enum  DOWN_TASK_STATUS
{
	NOITEM = 0,
	TSC_ERROR,		//����
	TSC_PAUSE,		//��ͣ
	TSC_DOWNLOAD,	//����
	TSC_COMPLETE,	//���
	TSC_STARTPENDING,	//�ȴ���ʼ
	TSC_STOPPENDING		//�ȴ�ֹͣ
};
enum TASK_ERROR_TYPE
{
	TASK_ERROR_UNKNOWN	   =			0x00,   // δ֪����
	TASK_ERROR_DISK_CREATE =			0x01,   // �����ļ�ʧ��
	TASK_ERROR_DISK_WRITE =				0x02,   // д�ļ�ʧ��
	TASK_ERROR_DISK_READ =				0x03,   // ���ļ�ʧ��
	TASK_ERROR_DISK_RENAME =			0x04,   // ������ʧ��
	TASK_ERROR_DISK_PIECEHASH =			0x05,   // �ļ�ƬУ��ʧ��
	TASK_ERROR_DISK_FILEHASH =			0x06,   // �ļ�ȫ��У��ʧ��
	TASK_ERROR_DISK_DELETE =			0x07,   // ɾ���ļ�ʧ��ʧ��
	TASK_ERROR_DOWN_INVALID =			0x10,   // ��Ч��DOWN��ַ
	TASK_ERROR_PROXY_AUTH_TYPE_UNKOWN = 0x20,   // ��������δ֪
	TASK_ERROR_PROXY_AUTH_TYPE_FAILED = 0x21,   // ������֤ʧ��
	TASK_ERROR_HTTPMGR_NOT_IP =			0x30,   // http��������ip����
	TASK_ERROR_TIMEOUT =				0x40,   // ����ʱ
	TASK_ERROR_CANCEL =					0x41,   // ����ȡ��
	TASK_ERROR_TP_CRASHED=              0x42,   // MINITP����
	TASK_ERROR_ID_INVALID =             0x43,   // TaskId �Ƿ�
};
struct DownTaskInfo
{
	DownTaskInfo()
	{
		memset(this, 0, sizeof(DownTaskInfo));
		stat		= TSC_PAUSE;
		fail_code	= TASK_ERROR_UNKNOWN;
		fPercent = 0;
		bIsOriginUsable = false;
		fHashPercent = 0;
	}
	DOWN_TASK_STATUS	stat;
	TASK_ERROR_TYPE		fail_code;
	wchar_t		szFilename[MAX_PATH];
	wchar_t		szReserved0[MAX_PATH];
	__int64     nTotalSize;         // �������ܴ�С(�ֽ�)
	__int64     nTotalDownload;     // ������Ч�ֽ���(���ܴ��ڻ��˵����)
	float		fPercent;           // ���ؽ���
	int			nReserved0;
	int			nSrcTotal;          // ����Դ��
	int			nSrcUsing;          // ������Դ��
	int			nReserved1;
	int			nReserved2;
	int			nReserved3;
	int			nReserved4;
	__int64     nReserved5;
	__int64		nDonationP2P;       // p2p�����ֽ���
	__int64		nReserved6;
	__int64		nDonationOrgin;		// ԭʼ��Դ�����ֽ���
	__int64		nDonationP2S;		// ������Դ�����ֽ���
	__int64		nReserved7;
	__int64     nReserved8;
	int			nSpeed;             // ��ʱ�ٶ�(�ֽ�/��)
	int			nSpeedP2S;          // ��ʱ�ٶ�(�ֽ�/��)
	int			nSpeedP2P;          // ��ʱ�ٶ�(�ֽ�/��)
	bool		bIsOriginUsable;    // ԭʼ��Դ�Ƿ���Ч
	float		fHashPercent;       // �ֲ��ṩ��ֵ
	int			IsCreatingFile;     // �Ƿ����ڴ����ļ�
	DWORD		reserved[64];
};
enum DOWN_PROXY_TYPE
{
	PROXY_TYPE_IE	 = 0,
	PROXY_TYPE_HTTP  = 1,
	PROXY_TYPE_SOCK4 = 2,
	PROXY_TYPE_SOCK5 = 3,
	PROXY_TYPE_FTP   = 4,
	PROXY_TYPE_UNKOWN  = 255,
};
enum DOWN_PROXY_AUTH_TYPE
{
	PROXY_AUTH_NONE =0,
	PROXY_AUTH_AUTO,
	PROXY_AUTH_BASE64,
	PROXY_AUTH_NTLM,
	PROXY_AUTH_DEGEST,
	PROXY_AUTH_UNKOWN,
};
struct DOWN_PROXY_INFO
{
	BOOL		bIEProxy;
	BOOL		bProxy;
	DOWN_PROXY_TYPE	stPType;
	DOWN_PROXY_AUTH_TYPE	stAType;
	wchar_t		szHost[2048];
	INT32		nPort;
	wchar_t		szUser[50];
	wchar_t		szPwd[50];
	wchar_t		szDomain[2048];
};

#pragma pack(pop)

//Ѹ�׽ӿ�
class CXlModule : public tImpModuleMid<CXlModule>  // ��ֹĳЩ���๦��
{
public:
	// ��ʼ��
	BOOL  ( *XL_Init)( void );
	
	// �������񡣿����½�����Ҳ�����������񣬲�����IsResume���ڱ�ʾ�����Ƿ�����������
	HANDLE ( *XL_CreateTaskByURL)( const wchar_t *url, const wchar_t *path, const wchar_t *fileName, BOOL IsResume );
	
	// ���񴴽����ǲ������Ͼ����صģ���Ҫ����XL_StartTask���첽ִ�С�
	BOOL ( *XL_StartTask)( HANDLE hTask );

	BOOL  ( *XL_ForceStopTask)( HANDLE hTask );

	// XL_DeleteTask�ڲ�ֻ�������������Ӧ���ڴ����ݣ����Դ���ͣ����Ĺ��ܣ����Ե���XL_DeleteTask֮ǰ��Ҫ����ͣ����
	BOOL  ( *XL_DeleteTask)( HANDLE hTask );

	// ��ѯ����ǰ��Ϣ
	BOOL  (* XL_QueryTaskInfoEx)( HANDLE hTask, DownTaskInfo & stTaskInfo );

	// ����ʼ��
	BOOL  ( *XL_UnInit)( void );

	DECLARE_FUN_BEGIN(CXlModule, "xldl.dll")
		DECLARE_FUN(XL_Init)
		DECLARE_FUN(XL_CreateTaskByURL)
		DECLARE_FUN(XL_StartTask)
		DECLARE_FUN(XL_ForceStopTask)
		DECLARE_FUN(XL_DeleteTask)
		DECLARE_FUN(XL_QueryTaskInfoEx)
		DECLARE_FUN(XL_UnInit)
	DECLARE_FUN_END()
};

