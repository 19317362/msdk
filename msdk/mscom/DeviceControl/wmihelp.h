#pragma once
#include <string>
#include <devguid.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <algorithm>
#pragma comment(lib,"setupapi.lib")


enum  OSTypeEnum
{
	WIN7 = 0,							// Win7
	WinXP,
	WinServer2008,
	WinVista,
	Win2008,
	Win2000,
	OnKnown
};

// ��ȡ����ϵͳ�汾
static OSTypeEnum GetSystemVersion()
{
	OSTypeEnum  osType = OnKnown;

	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return OnKnown;
	}

	if( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
		{
			if( osvi.wProductType == VER_NT_WORKSTATION )
			{
				osType = WIN7;
				return osType;
			}
			else
			{
				osType = WinServer2008;
				return osType;
			}
		}
		if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
		{
			if( osvi.wProductType == VER_NT_WORKSTATION )
			{
				osType = WinVista;
				return osType;
			}
			else
			{
				osType = Win2008;
				return osType;
			}
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			osType = WinXP;
			return osType;
		}
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
			osType = Win2000;
			return osType;
		}
	}
	return osType;
}


// ��ȡָ���豸��ָ������
static std::wstring GetDeviceProperty(HDEVINFO hDevInfo, SP_DEVINFO_DATA &hDeviceInfoData,DWORD dwProperty)
{
	std::wstring	strProperty;
	DWORD	dwData;
	DWORD	dwSize = 4096;
	LPTSTR	buffer = (WCHAR *)LocalAlloc(LPTR, dwSize);	

	while (!SetupDiGetDeviceRegistryProperty( hDevInfo,
		&hDeviceInfoData,
		dwProperty,
		&dwData,
		(PBYTE)buffer,
		dwSize,
		&dwSize) )
	{	
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (buffer != NULL)
				LocalFree(buffer);

			buffer = (WCHAR *)LocalAlloc(LPTR, dwSize*2);
		}
		else
		{		
			break;
		}
	}

	strProperty = buffer;

	if (buffer != NULL)
		LocalFree(buffer);

	// ת���ɴ�д
	std::transform( strProperty.begin(), strProperty.end(),
		strProperty.begin(), toupper);

	return strProperty;
}

// �ж�ĳһ�豸�Ƿ��ѱ�ͣ��
static BOOL IsDeviceDisable(SP_DEVINFO_DATA hDeviceInfoData)
{
	DWORD dwStatus			= -1;
	DWORD dwProblenNumber	= -1;
	DWORD dwResult;
	BOOL  bReturn = FALSE;

	dwResult = CM_Get_DevNode_Status(&dwStatus, &dwProblenNumber, hDeviceInfoData.DevInst, 0);

	if (CR_SUCCESS == dwResult)
	{
		if (dwProblenNumber == CM_PROB_DISABLED || (dwStatus & DN_HAS_PROBLEM) != 0)
			bReturn = TRUE;
	}

	return bReturn;
}

// �ı�ĳһ�豸��״̬
static BOOL ChangeDeviceState(HDEVINFO hDevInfo, 
					   SP_DEVINFO_DATA hDeviceInfoData, 
					   DWORD dwState)
{

	BOOL bResult = FALSE;
	SP_PROPCHANGE_PARAMS pcpParams;
	OSTypeEnum	OsType = OnKnown;

	OsType = GetSystemVersion();

	// �ж��豸�Ƿ��Ѿ���ͣ��
	BOOL bStop = IsDeviceDisable(hDeviceInfoData);

	// �����ظ�����
	if( bStop == TRUE && dwState == DICS_DISABLE)
		return TRUE;

	if( bStop == FALSE && dwState == DICS_ENABLE )
		return TRUE;

	// Win7������
	if( OsType == WIN7 ||
		OsType == WinVista )
	{
		pcpParams.Scope = DICS_FLAG_GLOBAL; 
	}
	else  // XP��ǰ 
	{
		pcpParams.Scope = DICS_FLAG_CONFIGSPECIFIC;
		pcpParams.HwProfile = 0;
	}

	pcpParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
	pcpParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	pcpParams.StateChange = dwState;

	// ���û����ĳһ�豸��Ϣ������ĳһ�豸��ϢԪ���еİ�װ����
	// ��
	bResult = SetupDiSetClassInstallParams( hDevInfo, 
		&hDeviceInfoData, // ΪNULL��������Ϣ����ص������豸
		(PSP_CLASSINSTALL_HEADER)&pcpParams, //�洢��װ��Ϣ
		sizeof(pcpParams) // �ṹ��С
		);

	if(bResult == FALSE)
		return bResult;

	// �ı�ָ���豸״̬
	bResult = SetupDiChangeState(hDevInfo, &hDeviceInfoData);

	return bResult;
}


static BOOL IsRootDevice(HDEVINFO hDevInfo, SP_DEVINFO_DATA& lpDevInfo)
{
	BOOL bRet = FALSE;
	std::wstring	strRoot = L"ROOT";
	std::wstring	strEnumRatorName;


	// ��ȡ���豸��ö��������
	strEnumRatorName = GetDeviceProperty( hDevInfo, lpDevInfo, SPDRP_ENUMERATOR_NAME );

	// ȫ��ת���ɴ�д
	transform(strEnumRatorName.begin(), strEnumRatorName.end(), 
		strEnumRatorName.begin(), toupper);



	if( !strEnumRatorName.empty() )
	{
		// ִ�бȶ�, ����Ա����
		if( !strEnumRatorName.compare( 0, strRoot.size(), strRoot ) )
		{
			bRet = TRUE;
		}
	}

	return bRet;
}