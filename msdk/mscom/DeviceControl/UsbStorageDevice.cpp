#include "StdAfx.h"
#include "UsbStorageDevice.h"

CUsbStorageDevice::CUsbStorageDevice(void)
{
}

CUsbStorageDevice::~CUsbStorageDevice(void)
{
}

BOOL CUsbStorageDevice::Scan(DeviceScanNotify* pNotify)
{
	RASSERT(pNotify, FALSE);
	SP_DEVINFO_DATA hDeviceInfoData = {0};
	HDEVINFO hDevInfo = SetupDiGetClassDevs((LPGUID)&GUID_DEVCLASS_USB, 0, 0, DIGCF_PRESENT);
	RASSERT(hDevInfo != INVALID_HANDLE_VALUE, FALSE);

	hDeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	for (int i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&hDeviceInfoData);i++)
	{
		CUsbStorageDevice* pDiskDevice = new CUsbStorageDevice();
		pDiskDevice->SetClass(GetDeviceProperty(hDevInfo, hDeviceInfoData, SPDRP_CLASS));			// �豸��װ������ ���������� ϵͳ�豸����ʾ��
		pDiskDevice->SetClassGuid(GetDeviceProperty(hDevInfo, hDeviceInfoData, SPDRP_CLASSGUID));	// �豸��װ���GUID
		pDiskDevice->SetDesc(GetDeviceProperty(hDevInfo, hDeviceInfoData, SPDRP_DEVICEDESC));		// �豸����
		pDiskDevice->SetFName(GetDeviceProperty(hDevInfo,hDeviceInfoData,  SPDRP_FRIENDLYNAME));	// �Ѻ�(�׶�)����
		pDiskDevice->SetHID(GetDeviceProperty(hDevInfo, hDeviceInfoData, SPDRP_HARDWAREID));		// Ӳ���豸ID
		pDiskDevice->SetDriver(GetDeviceProperty(hDevInfo, hDeviceInfoData, SPDRP_DRIVER));			// ����������Ϣ��ע����е�λ��
		pDiskDevice->SetManufacturer(GetDeviceProperty(hDevInfo, hDeviceInfoData, SPDRP_MFG));				// �豸������
		pDiskDevice->SetCompatibleID(GetDeviceProperty(hDevInfo,hDeviceInfoData, SPDRP_COMPATIBLEIDS));
		pDiskDevice->SetServiceName( GetDeviceProperty(hDevInfo,hDeviceInfoData, SPDRP_SERVICE));
		pDiskDevice->SetNumeratorName(GetDeviceProperty(hDevInfo,hDeviceInfoData, SPDRP_ENUMERATOR_NAME)); 
		
		
		BOOL isUsbStorageDevice = FALSE;
		//  �ж��Ƿ���USB�洢�豸		
		std::wstring strTemp = L"USB";
		// ƥ���豸ö�����������Ƿ�Ϊ"USB"
		if( CompareString(pDiskDevice->GetNumeratorName(), strTemp) )
		{
			strTemp = L"USB\\CLASS_08";
			if( CompareString(pDiskDevice->GetCompatibleID(), strTemp) )
			{
				isUsbStorageDevice = TRUE;
			}
		}
		
		if (isUsbStorageDevice)
		{
			pNotify->OnDeviceScanNotify(pDiskDevice);
		}
		else
		{
			pDiskDevice->DelayDelete();
		}

	}

	SetupDiDestroyDeviceInfoList(hDevInfo);
	return TRUE;
}