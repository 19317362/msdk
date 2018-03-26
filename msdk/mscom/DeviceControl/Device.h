#pragma once

#include "DeviceControl\IDeviceControl.h"
#include "wmihelp.h"





/********************************************************************
	Summary:
		�豸��Ϣ�ṹ��
*********************************************************************/
struct DeviceNodeInfo
{
	std::wstring	m_strClass;					//�豸CLASS
	std::wstring	m_strClassGuid;				//�豸CLASSGUID
	std::wstring	m_strDesc;					//�豸����
	std::wstring	m_strHID;						//�豸HARDWAREID
	std::wstring	m_strFName;					//�豸FriendlyName
	std::wstring	m_strDriver;					//�豸Driver
	std::wstring	m_strCompatibleID;			//�����豸ID
	std::wstring	m_strServiceName;				//������
	std::wstring	m_strNumeratorName;			//ö��������
	std::wstring	m_strDeviceInstanceId;		//��ȡ�豸����ID
	std::wstring	m_strManufacturer;			//����������
	std::wstring	m_strDriverProvider;			//�����ṩ��
	std::wstring	m_strDriverVersion;			//�����汾
	std::wstring	m_strDriverDay;				//��������
	std::wstring	m_strDriveInfFilePath;		//����Inf�ļ� 
	std::wstring	m_strDeviceModel;
}; 

class CDevice
	:public IDevice
	,protected DeviceNodeInfo
{
public:
	CDevice(void){}
	virtual ~CDevice(void){}
	virtual DWORD			GetDeviceID(){return -1;}
	virtual DeviceTypeEnum	GetDeviceType(){return DEVICE_TYPE_UNKNOWN;}
	virtual	VOID			DelayDelete(){delete this;}


	//���豸�Ĳ���
	virtual BOOL StartDevice(); //�����豸
	virtual BOOL StopDevice();  //ͣ���豸

	//��ȡ����
	virtual LPCWSTR GetSerialNumber(){return m_strHID.c_str();}
	virtual	LPCWSTR GetManufacturer(){return this->m_strManufacturer.c_str();}
	virtual LPCWSTR GetDescription(){return m_strDesc.c_str();}
	virtual LPCWSTR GetProperty(DWORD dwProperty){return NULL;}
	virtual LPCWSTR	GetSpecification(){return m_strServiceName.c_str();}
	virtual LPCWSTR GetDeviceModel(){return m_strFName.c_str();}

	//ģ���ڲ�ʹ��
	void SetDeviceNodeInfo(DeviceNodeInfo nodeInfo){*((DeviceNodeInfo*)this) = nodeInfo;}
	void SetClass(std::wstring str){m_strClass = m_strClass;}
	void SetClassGuid (std::wstring str){m_strClassGuid =  str ;}
	void SetDesc (std::wstring str){this->m_strDesc = str;}
	void SetHID (std::wstring str){this->m_strHID = str;}
	void SetFName (std::wstring str){this->m_strFName = str;}
	void SetDriver (std::wstring str){this->m_strDriver = str;}
	void SetCompatibleID (std::wstring str){this->m_strCompatibleID = str ;}
	void SetServiceName (std::wstring str){this->m_strServiceName  = str;}
	void SetNumeratorName (std::wstring str){this->m_strNumeratorName = str ;}
	void SetDeviceInstanceId (std::wstring str){this->m_strDeviceInstanceId  = str;}
	void SetManufacturer (std::wstring str){this->m_strManufacturer  = str;}
	void SetDriverProvider (std::wstring str){this->m_strDriverProvider  = str;}
	void SetDriverVersion (std::wstring str){this->m_strDriverVersion  = str;}
	void SetDriverDay (std::wstring str){this->m_strDriverDay  = str;}
	void SetDriveInfFilePath (std::wstring str){this->m_strDriveInfFilePath  = str;}
	
	void SetDeviceModel(std::wstring str){m_strDeviceModel = str;}
	//ģ���ڲ�ʹ��
public:
	std::wstring GetNumeratorName(){return m_strNumeratorName;}
	std::wstring GetCompatibleID(){return m_strCompatibleID;}
private:
	//���ֱ���ĳ�Ա����
};

struct DeviceScanNotify
{
	virtual VOID OnDeviceScanNotify(CDevice* pDevice) = 0;
};