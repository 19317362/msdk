#pragma once

namespace msdk{
namespace mscom{


//////////////////////////////////////////////////////////////////////////
//mscom�ײ��������ʼ��MSCOM���л������൱��ע���
struct IMsObjectLoader:public IMSBase
{
	//�õ�rclsid����Ӧ��DLLģ��ʱ��DLL�Żᱻ���ص��ڴ���
	STDMETHOD(CreateInstance)(IMSBase* prot, const CLSID& rclsid, IMSBase *punkOuter, const IID& riid, void **ppv) = 0;
};
MS_DEFINE_IID(IMsObjectLoader , "{51F1439C-549F-49be-A556-ADEC01F329D2}");


interface IMsOLManager: public IMSBase
{
	STDMETHOD(Register)(const CLSID& rclsid, LPCWSTR filename) = 0;
	STDMETHOD(Revoke)(const CLSID& rclsid) = 0;
};
MS_DEFINE_IID(IMsOLManager , "{FA06B469-8884-4066-8395-019957AE3659}");


/*ʵ�ֽӿ�
public IMsObjectLoader ,
public IMsOLManager ,
public IMsOLManagerEx ,
*/
//{B37BFFCA-29FA-4b19-AE42-5FF0F533D7E6}
MS_DEFINE_GUID(CLSID_ObjectLoader,
			   0xb37bffca, 0x29fa, 0x4b19, 0xae, 0x42, 0x5f, 0xf0, 0xf5, 0x33, 0xd7, 0xe6);


//////////////////////////////////////////////////////////////////////////
//ȫ��Ψһʵ����
interface IMscomRunningObjectTable : public IMSBase
{
	/************************************************************************/
	/*  ���ٴ����������ڲ�ʵ����Ҳ����IMsObjectLoader�����Ķ���
	 */
	STDMETHOD(CreateInstance)(const CLSID& rclsid, IMSBase *punkOuter, const IID& riid, void **ppv) = 0;

	//���punkΪ�գ���ͨ��IMsObjectLoader����ʱ����
	STDMETHOD(Register)(const CLSID& rpid, IMSBase *punk /*=NULL*/) = 0;
	STDMETHOD(Revoke)(const CLSID& rpid) = 0;
	STDMETHOD_(BOOL ,IsRunning)(const CLSID& rpid) = 0;
	STDMETHOD(GetObject)(const CLSID& rpid, const IID& iid, void **ppunk) = 0;
	STDMETHOD(RevokeAll)() = 0;
};
MS_DEFINE_IID(IMscomRunningObjectTable , "{14B578A4-11BC-48e5-8FCB-4E9974C5EC29}");


/*
	ö��Rot
*/
struct IRunningObjectTableEnum
{
	/*
		���ܣ�ROT ö�ٻص�
		����: 
			 clsid MSCOM �������ID
			 pObject Rot ����
		����:FALSE ����ö��
	*/
	virtual BOOL OnRunningObjectTableEnum(REFCLSID clsid , IMSBase* pObject) = 0;
};

interface IMscomRunningObjectTableEx : public IMSBase
{
	/*
		����:ö��ROT
		����:pEnum ö�ٻص��ӿ�

	*/
	STDMETHOD(EnumRunningObjectTable)(IRunningObjectTableEnum* pEnum, IID iid = __uuidof(IMSBase)) = 0;
	STDMETHOD_(REFCLSID, GetMainPlugin)() = 0;
};

MS_DEFINE_IID(IMscomRunningObjectTableEx , "{CBC62BD1-000C-4329-80A9-1E7DEF71FA30}");

/*�ӿ�ʵ��
public IMscomRunningObjectTable,
public IMscomRunningObjectTableEx,
public IMsCfgBase ,
*/
// {CEC19C43-D1D3-437c-B467-C77ED23BA5BC}
MS_DEFINE_GUID(CLSID_RunningObjectTable,
			0xcec19c43, 0xd1d3, 0x437c, 0xb4, 0x67, 0xc7, 0x7e, 0xd2, 0x3b, 0xa5, 0xbc);


//////////////////////////////////////////////////////////////////////////
//�������Ҫʵ�ֵ�
struct IExit : public IMSBase
{
	//�˳�������������У�һ����˵�����������ͨ�����е�����һ���߳������õ�
	STDMETHOD(NotifyExit)(bool* bExit = NULL) = 0;
};
MS_DEFINE_IID(IExit, "{AFFA4FB2-EFF6-4371-AADD-62C0B0766178}");


//////////////////////////////////////////////////////////////////////////
//env


struct IMsEnvMgr : public IMSBase
{
	STDMETHOD(Init)(
		LPCWSTR lpszComPath, 
		LPCWSTR lpszComXml,
		LPCWSTR lpszRotPath,
		LPCWSTR lpszRotXml,

		//��Ҫ��������activex�У���������ʱ�õĶ��Ǹ�ģ������·��������
		HINSTANCE hInstance = NULL,
		LPCWSTR lpszWorkPath = NULL //����Ŀ¼
		) = 0;

	STDMETHOD(Uninit)() = 0;

	STDMETHOD(Start)() = 0;
	STDMETHOD(Stop)() = 0;

	//���Ӳ������һ��������ROTʱʹ��
	STDMETHOD(GetRunningObjectTable)(IMscomRunningObjectTable** pRot) = 0;
};
MS_DEFINE_IID(IMsEnvMgr , "{B253DFEA-E8E7-456A-9858-3A38B508C5BB}");


struct IMsEnv : public IMSBase
{
	
	//��ȡʹ�� envmgr �ľ���������ǽ��̣�Ҳ������DLL
	STDMETHOD_(HINSTANCE,GetInstance)() = 0;
	

	//��ȡ������,�ڷǷ���ģʽ�·���NULL
	STDMETHOD_(SERVICE_STATUS_HANDLE, GetServiceStatusHandle)() = 0;

	//��ȡ��ǰ����Ŀ¼
	STDMETHOD_(LPCWSTR, GetWorkPath)() = 0;

	/*
		��ȡ����������
		��Ӧ **.xml �е�<envinfo> �ڵ�
		IPropertyStr
	*/
	STDMETHOD(GetEnvParam)(IMSBase** ppIPropertyStr) = 0;
};



//MS_DEFINE_IID(IExtGrp ,"{EE2AA817-B521-43ed-AC17-4D7E0C9EDDB8}");

/*ʵ�ֽӿ�
IMsEnv
IExit �˳����������ڲ����õ��������IExit
//IExtGrp
imspluginrun
IServiceCtrlConnectPoint ʵ�ַ���������ӵ�
*/
MS_DEFINE_IID(IMsEnv , "{453D4064-779F-4771-9641-1D998138E2A4}");


/*����ⲿ�Ŀ��Ƿ���Ļ�����Ҫ�õ�*/
struct IEvnSrv : public IMSBase
{
	//����Ҫ��Init֮ǰ����
	virtual VOID SetServiceStatusHandle( SERVICE_STATUS_HANDLE hSrv) = 0;

	virtual DWORD SrvHandle(DWORD dwControl,DWORD dwEventType,LPVOID lpEventData, LPVOID lpContext) = 0;
};
MS_DEFINE_IID(IEvnSrv , "{EF7C1999-96CF-4C08-AB92-B1D479C71C40}");

//��Ӧ��������¼����ӵ�
//�����ӵ��ڷ���ģʽ�±�����

interface IServiceCtrlConnectPoint : public IMSBase
{
	//�μ�MSDN:RegisterServiceCtrlHandlerEx || HandlerEx
	STDMETHOD_(DWORD, OnServiceCtrl)(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext) = 0;
};
MS_DEFINE_IID(IServiceCtrlConnectPoint, "{88E3C375-B6EA-4dd8-8C4E-FE00689AD25A}");


// {A3637D66-7A9F-4a7c-B7CA-FEA028137586}
MS_DEFINE_GUID(CLSID_MsEnv,
			   0xa3637d66, 0x7a9f, 0x4a7c, 0xb7, 0xca, 0xfe, 0xa0, 0x28, 0x13, 0x75, 0x86);


}
}