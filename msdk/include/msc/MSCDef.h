#ifndef _MSC_DEF_H_
#define _MSC_DEF_H_

#include "XMSCProxy.h"
#include <mscom/msplugin.h>

/////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *MSC_CREATEMSCPROXY)(XMSCProxy** lppXMSCProxy);
typedef BOOL (WINAPI *MSC_INITIALIZE)(LPCTSTR lpszProduct, LPCTSTR lpszInstallPath);
typedef VOID (WINAPI *MSC_UNINITIALIZE)();
typedef BOOL (WINAPI *MSC_START)();
typedef VOID (WINAPI *MSC_STOP)();
/////////////////////////////////////////////////////////////////////////////
//�ӿ�

/*
public IMsgBus,
public IMsPluginRun
*/
interface IMsgBus:public IMsPlugin
{
	//SetPipeName������IMsPlugin::Initǰ���ò���Ч
	STDMETHOD(SetPipeName)(LPCTSTR szName, DWORD dwNameLen)=0;

	STDMETHOD_(BOOL,CreateProxy)(IXMsgBusProxy** lppXMSCProxy)=0;
};
MS_DEFINE_IID(IMsgBus,"{1C204372-ABFD-4461-92C3-0BAFE9BB7E81}");
 // {7E26320D-2526-4adf-804F-38A5FC9F326D}
MS_DEFINE_GUID(CLSID_MsgBus,
			   0x7e26320d, 0x2526, 0x4adf, 0x80, 0x4f, 0x38, 0xa5, 0xfc, 0x9f, 0x32, 0x6d);

 //CLSID_MsgBus �ڲ�ʵ����IMsPluginRun�ӿ�,���ڷ����

#endif

