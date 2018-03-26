#include "stdafx.h"
#include "Device.h"

BOOL CDevice::StartDevice()
{
	SP_DEVINFO_DATA				hDeviceInfoData;
	BOOL						bRet = FALSE;

	if( m_strHID.empty() )
		return bRet;

	// ת���ɴ�д
	transform( m_strHID.begin(), m_strHID.end(),
		m_strHID.begin(), toupper );

	// ��ȡָ�����͵��豸��Ϣ��
	HDEVINFO hDevInfo = SetupDiGetClassDevs( (LPGUID)&(S2GUIDW(m_strClassGuid.c_str())), 
		0,
		0, 
		DIGCF_PRESENT  );

	if ( hDevInfo == INVALID_HANDLE_VALUE )
		return FALSE;

	hDeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	// ö���豸��Ϣ���е������豸��ϢԪ��
	for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &hDeviceInfoData); i++)
	{
		std::wstring strDeviceID = GetDeviceProperty(hDevInfo,hDeviceInfoData, SPDRP_HARDWAREID);

		// ת���ɴ�д
		transform( strDeviceID.begin(), strDeviceID.end(),
			strDeviceID.begin(), toupper );

		// ʹ���豸Ӳ��IDɸѡ��ָ���豸
		if( !strDeviceID.compare(m_strHID) )
		{
			// �ı�ָ���豸��״̬
			bRet = ChangeDeviceState(hDevInfo, hDeviceInfoData, DICS_ENABLE);
			break;
		}
	}

	if( hDevInfo)
		SetupDiDestroyDeviceInfoList(hDevInfo);

	return bRet;
}

// ͣ��ĳһ���豸
BOOL CDevice::StopDevice()
{
	SP_DEVINFO_DATA	hDeviceInfoData;
	BOOL			bRet = FALSE;

	if( m_strHID.empty() )
		return bRet;

	// ��ȡָ�������豸����Ϣ��
	HDEVINFO hDevInfo = SetupDiGetClassDevs( (LPGUID)&(S2GUIDW(m_strClassGuid.c_str())), // All Classes
		0,
		0, 
		DIGCF_PRESENT );

	if (hDevInfo == INVALID_HANDLE_VALUE)
		return FALSE;

	hDeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	// ö����Ϣ���е������豸��ϢԪ��
	for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &hDeviceInfoData); i++)
	{
		// ʹ��Ӳ���豸ID��ɸѡ��ָ���豸
		if( GetDeviceProperty(hDevInfo,hDeviceInfoData, SPDRP_HARDWAREID).compare(m_strHID) == 0 )
		{
			// �ж��豸�Ƿ��Ѿ���ͣ��
			if( IsDeviceDisable(hDeviceInfoData) )	// �ж��豸�Ƿ�ֹͣ�����ֹͣ�����ٴ���
			{
				bRet = TRUE;
				goto ErrorHandle;
			}

			// �ı��豸״̬Ϊͣ��
			bRet = ChangeDeviceState(hDevInfo,hDeviceInfoData, DICS_DISABLE);
			break;
		}
	}

ErrorHandle:
	if( hDevInfo)
		SetupDiDestroyDeviceInfoList(hDevInfo);

	return bRet;
}