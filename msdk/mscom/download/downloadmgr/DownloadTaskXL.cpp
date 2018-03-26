#include "StdAfx.h"
#include "DownloadTaskXL.h"


CXlModule CDownloadTaskXL::m_xlModule;

static class CXlModuleInit
{
public:
	CXlModuleInit()
	{
		CDownloadTaskXL::m_xlModule.Load();
		if ( CDownloadTaskXL::m_xlModule.IsLoaded() )
		{
			if ( !CDownloadTaskXL::m_xlModule.XL_Init() )
			{
				CDownloadTaskXL::m_xlModule.XL_UnInit();
				CDownloadTaskXL::m_xlModule.UnLoad();
			}
		}
		
	}
	~CXlModuleInit()
	{
		if ( CDownloadTaskXL::m_xlModule.IsLoaded() )
		{
			//��ʱ�����ˣ��ڷ���ʼ����ʱ���̹߳���
			//CDownloadTaskXL::m_xlModule.XL_UnInit();
			//CDownloadTaskXL::m_xlModule.UnLoad();
		}
	}
} _XlModuleInit;

STDMETHODIMP CDownloadTaskXL::init_class(IMSBase* pRot , IMSBase* pOuter)
{
	//����Ѹ������
	if ( !m_xlModule.IsLoaded() )
		return E_FAIL;


	return CMsComBase::init_class(pRot, pOuter);
}

CDownloadTaskXL::CDownloadTaskXL(void)
{
	m_dwTaskID = 0;
	m_dwDownloadThreadID = 0;
	m_dlTaskState = DL_TaskState_UnKnown;
	m_dlError = DLE_SUCCESS;
	m_bAddToStore = FALSE;
	m_llTotalSize	= 0;
	m_llCurSize = 0;
	m_hDLTask = 0;
}


CDownloadTaskXL::~CDownloadTaskXL(void)
{
	DestroyTask();
}

STDMETHODIMP_(LPCWSTR) CDownloadTaskXL::GetUrl()
{
	return m_strUrl;
}

STDMETHODIMP_(LPCWSTR		) CDownloadTaskXL::GetSavePath()
{
	return m_strSavePath;
}

STDMETHODIMP_(DL_TaskType	) CDownloadTaskXL::GetTaskType()
{
	CStringW strUrl = m_strUrl;
	strUrl.MakeLower();
	if (strUrl.Find(_T("http://")) == 0)
	{
		return DL_TaskType_Http;
	}

	if (strUrl.Find(_T("https://")) == 0)
	{
		return DL_TaskType_Https;
	}

	return DL_TaskType_Http;
}

STDMETHODIMP_(LONGLONG		) CDownloadTaskXL::GetTotalFileSize()
{
	if ( !m_xlModule.IsLoaded() )
		return m_llTotalSize;

	DownTaskInfo info;
	if ( !m_xlModule.XL_QueryTaskInfoEx(m_hDLTask, info) )
		return m_llTotalSize;


	switch( info.stat )
	{
	case TSC_DOWNLOAD:
		m_llTotalSize = info.nTotalSize;
		break;
	}

	return m_llTotalSize;
}

STDMETHODIMP_(LONGLONG		) CDownloadTaskXL::GetCurrentFileSize()
{
	if ( !m_xlModule.IsLoaded() )
		return m_llCurSize;


	DownTaskInfo info;
	if ( !m_xlModule.XL_QueryTaskInfoEx(m_hDLTask, info) )
		return m_llCurSize;

	if ( info.nTotalDownload )
		m_llCurSize = info.nTotalDownload;

	return m_llCurSize;
}

STDMETHODIMP_(LPCWSTR		) CDownloadTaskXL::GetProxyService()
{
	return m_strProxyService;
}

STDMETHODIMP_(LPCWSTR		) CDownloadTaskXL::GetProxyUser()
{
	return m_strProxyUser;
}

STDMETHODIMP_(LPCWSTR		) CDownloadTaskXL::GetProxyPwd()
{
	return m_strProxyPwd;
}

STDMETHODIMP_(DWORD		) CDownloadTaskXL::GetTaskId()
{
	return m_dwTaskID;
}

STDMETHODIMP_(DL_Error		) CDownloadTaskXL::GetTaskError()
{

	if ( !m_xlModule.IsLoaded() )
		return DLE_SUCCESS;


	DownTaskInfo info;
	if ( !m_xlModule.XL_QueryTaskInfoEx(m_hDLTask, info) )
		return DLE_SUCCESS;


	switch( info.fail_code )
	{
		
	case TASK_ERROR_UNKNOWN:		// δ֪����
		m_dlError = DLE_S_FALSE;break;

	case TASK_ERROR_DISK_CREATE:    // �����ļ�ʧ��
		m_dlError = DLE_FILENOTOPENED;break;

	case TASK_ERROR_DISK_WRITE:		// д�ļ�ʧ��
		m_dlError = DLE_FILENOTOPENED;break;


	case TASK_ERROR_DISK_READ:		// ���ļ�ʧ��
		m_dlError = DLE_FILENOTOPENED;break;


	case TASK_ERROR_DISK_RENAME:    // ������ʧ��
		m_dlError = DLE_FILENOTOPENED;break;


	case TASK_ERROR_DISK_PIECEHASH: // �ļ�ƬУ��ʧ��
		m_dlError = DLE_FILENOTOPENED;break;

	case TASK_ERROR_DISK_FILEHASH:  // �ļ�ȫ��У��ʧ��
		m_dlError = DLE_FILENOTOPENED;break;

	case TASK_ERROR_DISK_DELETE:    // ɾ���ļ�ʧ��ʧ��
		m_dlError = DLE_FILENOTOPENED;break;

	case TASK_ERROR_DOWN_INVALID:   // ��Ч��DOWN��ַ
		m_dlError = DLE_BADURL;break;

	case TASK_ERROR_PROXY_AUTH_TYPE_UNKOWN:    // ��������δ֪
		m_dlError = DLE_PROXYAUTHREQ;break;

	case TASK_ERROR_PROXY_AUTH_TYPE_FAILED:    // ������֤ʧ��
		m_dlError = DLE_PROXYAUTHREQ;break;

	case TASK_ERROR_HTTPMGR_NOT_IP: // http��������ip����
		m_dlError = DLE_BADURL;break;

	case TASK_ERROR_TIMEOUT:		// ����ʱ
		m_dlError = DLE_TIMEOUT;break;

	case TASK_ERROR_CANCEL:			// ����ȡ��
		m_dlError = DLE_S_FALSE;break;

	case TASK_ERROR_TP_CRASHED:     // MINITP����
		m_dlError = DLE_WINERROR;break;

	case TASK_ERROR_ID_INVALID:     // TaskId �Ƿ�
		m_dlError = DLE_ERROR;break;

	}
	return m_dlError;
}

STDMETHODIMP CDownloadTaskXL::SetCookie( LPCWSTR lpszCookie )
{
	return S_OK;
}

STDMETHODIMP CDownloadTaskXL::SetProxyService( LPCWSTR lpszService )
{
	RASSERT(lpszService, E_INVALIDARG);
	m_strProxyService = lpszService;
	return S_OK;
}

STDMETHODIMP CDownloadTaskXL::SetProxyUser( LPCWSTR lpszUsr )
{
	RASSERT(lpszUsr, E_INVALIDARG);
	m_strProxyUser = lpszUsr;
	return S_OK;
}

STDMETHODIMP CDownloadTaskXL::SetProxyPwd( LPCWSTR lpszPwd )
{
	RASSERT(lpszPwd, E_INVALIDARG);
	m_strProxyPwd = lpszPwd;

	return S_OK;
}

STDMETHODIMP CDownloadTaskXL::CreateTask( LPCWSTR lpszUrl, LPCWSTR lpszSavePath, IDownloadNotify* pNotify , DWORD dwTaskId)
{
	if (!(lpszUrl && lpszSavePath && pNotify))
	{
		m_dlTaskState = DL_TaskState_Error;
		return E_INVALIDARG;
	}

	m_dlNotify = pNotify;
	m_dwTaskID = dwTaskId;

	m_strUrl = lpszUrl;
	m_strSavePath = lpszSavePath;

	m_CreateFileTime = UINT64ToFileTime(GetCurrentFileTime());
	CString strTempPath = lpszSavePath;// strTempPath += _T(".dltemp");


	if ( !m_xlModule.IsLoaded() )
		return E_FAIL;

	
	m_hDLTask = m_xlModule.XL_CreateTaskByURL(lpszUrl,strTempPath.Left(strTempPath.ReverseFind('\\') + 1), strTempPath.Mid(strTempPath.ReverseFind('\\') + 1), TRUE);
	if ( m_hDLTask )
		return S_OK;
	
	return E_FAIL;
}

STDMETHODIMP CDownloadTaskXL::DestroyTask()
{
	Stop();
	CSubThread::WaitForSubThreadExit();

	if ( m_xlModule.IsLoaded() && m_hDLTask)
	{
		m_xlModule.XL_DeleteTask(m_hDLTask);
		m_hDLTask = 0;
	}

	

	SAFE_RELEASE(m_dlNotify);
	SAFE_RELEASE(m_pRot);
	return S_OK;
}

STDMETHODIMP CDownloadTaskXL::Start()
{
	if ( !m_xlModule.IsLoaded() )
		return E_FAIL;


	CSubThread::StartThread();

	DownTaskInfo info;
	if ( !m_xlModule.XL_QueryTaskInfoEx(m_hDLTask, info) )
		return E_FAIL;

	//��ͣ
	switch( info.stat )
	{
	case TSC_PAUSE:
		m_xlModule.XL_StartTask(m_hDLTask);
		break;
	}
	
	return E_FAIL;
}

STDMETHODIMP CDownloadTaskXL::Stop()
{
	if ( !m_xlModule.IsLoaded() )
		return E_FAIL;
	

	DownTaskInfo info;
	if ( !m_xlModule.XL_QueryTaskInfoEx(m_hDLTask, info) )
		return E_FAIL;

	switch( info.stat )
	{
	case TSC_STARTPENDING:
	case TSC_DOWNLOAD:
		{
			m_xlModule.XL_ForceStopTask(m_hDLTask);
			return S_OK;//���ﲻ���˳��̣߳�ֹͣ�ɹ��̻߳��Զ��˳�
		}
	}
	
	
	

	return S_OK;
}

HRESULT CDownloadTaskXL::Run()
{
	for( ; !CSubThread::IsNeedSubThreadExit() ; Sleep(200))
	{
		DownTaskInfo info;
		if ( !m_xlModule.XL_QueryTaskInfoEx(m_hDLTask, info) )
			continue;

		switch( info.stat )
		{
		case TSC_STARTPENDING:
			SendNotify(DL_NotifyEvnet_Connecting);
			break;
		case TSC_DOWNLOAD:
			SendNotify(DL_NotifyEvnet_DownloadStart);
			SendNotify(DL_NotifyEvnet_Downloading);
			break;
		case TSC_COMPLETE:
			SendNotify(DL_NotifyEvnet_Complete);
			return 0;
			break;
		case TSC_STOPPENDING:
			SendNotify(DL_NotifyEvnet_Stop);
			break;
		case TSC_PAUSE:
			SendNotify(DL_NotifyEvnet_Stop);
			return 0;
			break;
		case TSC_ERROR:
			SendNotify(DL_NotifyEvent_Error);
			return 0;
			break;
		}
	}

	return 0;
}

STDMETHODIMP_(DL_TaskState) CDownloadTaskXL::GetTaskState()
{
	return m_dlTaskState;
}


STDMETHODIMP CDownloadTaskXL::SetCreateTime( const FILETIME& time )
{
	if (time.dwHighDateTime || time.dwLowDateTime)
	{
		m_CreateFileTime = time;
	}
	else
	{
		m_CreateFileTime = UINT64ToFileTime(GetCurrentFileTime());
	}
	return S_OK;
}

STDMETHODIMP_(FILETIME		) CDownloadTaskXL::GetCreateTime()
{
	return m_CreateFileTime;
}

STDMETHODIMP CDownloadTaskXL::SetTaskState( DL_TaskState state )
{
	m_dlTaskState = state;
	return S_OK;
}

STDMETHODIMP CDownloadTaskXL::SetTaskError( DL_Error error )
{
	m_dlError = error;
	return S_OK;
}

STDMETHODIMP CDownloadTaskXL::set_AddToStore( BOOL bAdd )
{
	m_bAddToStore = bAdd;
	return S_OK;
}

STDMETHODIMP_(BOOL) CDownloadTaskXL::get_AddToStore()
{
	return m_bAddToStore;
}

STDMETHODIMP CDownloadTaskXL::SetTotalSize(LONGLONG llTotalSize)
{
	m_llTotalSize = llTotalSize;
	return S_OK;
}

STDMETHODIMP CDownloadTaskXL::SetCurSize(LONGLONG llCurSize)
{
	m_llCurSize = llCurSize;
	return S_OK;
}

STDMETHODIMP_(DWORD) CDownloadTaskXL::GetEngine()
{
	return DL_EngineXL;
}

STDMETHODIMP_(DWORD) CDownloadTaskXL::GetSpeed()
{
	DownTaskInfo info;
	if ( !m_xlModule.XL_QueryTaskInfoEx(m_hDLTask, info) )
		return 0;

	return info.nSpeed;
}

STDMETHODIMP_(DWORD) CDownloadTaskXL::GetSpeed2()
{
	DWORD dwSpeed = m_speedMeter.GetSpeed();
	return dwSpeed;
}

VOID CDownloadTaskXL::SendNotify( DL_NotifyEvnet event )
{
	switch(event)
	{
	case DL_NotifyEvnet_Connecting:
		{
			m_dlTaskState = DL_TaskState_Connecting;
			break;
		}
	case DL_NotifyEvnet_Downloading:
		{
			m_dlTaskState = DL_TaskState_Downloading;
			break;
		}
	case DL_NotifyEvnet_DownloadStart:
		{
			break;
		}
	case DL_NotifyEvnet_Complete:
		{
			m_dlTaskState = DL_TaskState_Done;
			break;
		}
	case DL_NotifyEvnet_Stop:
		{
			m_dlTaskState = DL_TaskState_Stoped;
			break;
		}
	case DL_NotifyEvent_Error:
		{
			m_dlTaskState = DL_TaskState_Error;
			//CloseDownloadSession();
			break;
		}
	}

	if ( m_dlNotify )
	{
		m_dlNotify->OnDownloadNotify(event, m_dwTaskID);
	}
	
}