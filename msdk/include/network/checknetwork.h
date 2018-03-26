#pragma once

//���������
#include <wininet.h>
#include <SensAPI.h>
#include <Netlistmgr.h>


#pragma comment(lib,"Wininet.lib")
#pragma comment(lib,"Sensapi.lib")

namespace msdk{;


//���VISTA����֮��汾
static BOOL IsHasInternetConnectionForVista(BOOL &bHandle)
{
	bHandle = FALSE;
	BOOL bHasInternet = FALSE;
	CComPtr<INetworkListManager> spNetList;
	HRESULT hr = CoCreateInstance( CLSID_NetworkListManager, NULL, CLSCTX_ALL, IID_INetworkListManager, (void**) &spNetList );
	if( SUCCEEDED(hr) )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		hr = spNetList->get_IsConnectedToInternet(&vb);
		if( SUCCEEDED(hr) )
		{
			bHandle = TRUE;
			if( vb == VARIANT_TRUE )
				bHasInternet = TRUE;
		}
	}

	return bHasInternet;
}


//�ж������Ƿ����
//��һЩ���������������Ҫ��鵱ǰ������״̬��
//����������ܼ�鵽��ǰ�����Ƿ�������״̬
static BOOL WINAPI IsHasInternetConnection()
{

	//////////////////////////////////////////////////////////////////////////
	//΢����WINDOWS VISTA֮���ṩ��һ����NLA(Network List Manager API)�Ľӿڣ����ڻ�ȡ����״̬�仯֪ͨ��һ���ӿڡ���COM����ʵ��
	//////////////////////////////////////////////////////////////////////////
	BOOL bLive = FALSE;
	OSVERSIONINFO	osver = {sizeof(OSVERSIONINFO)};
	GetVersionEx(&osver);
	if(osver.dwMajorVersion  >= 6)
	{
		//��֪����: �ڿ���̻���Send WM_COPYDATA,�������д���COM��ʧ��
		//����A����WM_COPYDATA��B,B��WM_COPYDATA��Ϣ���ָ�A����WM_COPYDATA����ʱ����A��WM_COPYDATA��Ϣ��Ӧ�У�COM��ʧ��
		//����Щ����£�ϵͳ����ͼ����ʾ��������ʵ����������ġ�����������������������������ж�
		BOOL bHandle = FALSE;
		bLive = IsHasInternetConnectionForVista(bHandle);
		if( bLive )
			return bLive;
	}

	
	

	DWORD dwFlag = 0;
	bLive = IsNetworkAlive(&dwFlag);
	DWORD dwError = GetLastError();
	if( dwError == 0 )
	{
		if( bLive )
			return bLive;
	}

	//�������������Σ�1. ����ֵ�����ã�2.System Event Notification ����ֹͣ
	dwFlag = 0;
	if( InternetGetConnectedState(&dwFlag, 0) )
	{
		if(dwFlag&INTERNET_CONNECTION_OFFLINE)
			return FALSE;

		if( (dwFlag&INTERNET_CONNECTION_LAN) ||
			(dwFlag&INTERNET_CONNECTION_MODEM) ||
			(dwFlag&INTERNET_CONNECTION_PROXY) )
			return TRUE;
	}

	return FALSE;
}

};