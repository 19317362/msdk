#include "StdAfx.h"
#include "DiskDevice.h"

CDiskDevice::CDiskDevice(void)
{
}

CDiskDevice::~CDiskDevice(void)
{
}


/*
GUID_DEVCLASS_FDC���̿�����
GUID_DEVCLASS_DISPLAY��ʾ��
GUID_DEVCLASS_CDROM����
GUID_DEVCLASS_KEYBOARD����
GUID_DEVCLASS_COMPUTER�����
GUID_DEVCLASS_SYSTEMϵͳ
GUID_DEVCLASS_DISKDRIVE����������
GUID_DEVCLASS_MEDIA��������Ƶ����Ϸ������
GUID_DEVCLASS_MODEMMODEM
GUID_DEVCLASS_MOUSE��������ָ���豸
GUID_DEVCLASS_NET�����豸��
GUID_DEVCLASS_USBͨ�ô������߿�����
GUID_DEVCLASS_FLOPPYDISK����������
GUID_DEVCLASS_UNKNOWNδ֪�豸
GUID_DEVCLASS_SCSIADAPTERSCSI �� RAID ������
GUID_DEVCLASS_HDCIDE ATA/ATAPI ������
GUID_DEVCLASS_PORTS�˿ڣ�COM �� LPT��
GUID_DEVCLASS_MONITOR������
*/


BOOL CDiskDevice::Scan(DeviceScanNotify* pNotify)
{
	RASSERT(pNotify, FALSE);
	SP_DEVINFO_DATA hDeviceInfoData = {0};
	HDEVINFO hDevInfo = SetupDiGetClassDevs((LPGUID)&GUID_DEVCLASS_DISKDRIVE, 0, 0, DIGCF_PRESENT);
	RASSERT(hDevInfo != INVALID_HANDLE_VALUE, FALSE);
 
	hDeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	
	for (int i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&hDeviceInfoData);i++)
	{
		
		CDiskDevice* pDiskDevice = new CDiskDevice();
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

		pNotify->OnDeviceScanNotify(pDiskDevice);
		
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return TRUE;
}