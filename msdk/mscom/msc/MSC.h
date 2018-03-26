#ifndef _MSC_H_
#define _MSC_H_

/////////////////////////////////////////////////////////////////////////////
#ifdef USE_COMMONDLL
#	ifdef MSC_EXPORTS
#		define MSCAPI __declspec(dllexport)
#	else
#		define MSCAPI __declspec(dllimport)
#	endif
#else
#	define MSCAPI 
#endif

#include "XMSCProxy.h"
#ifdef __cplusplus
extern "C" {
#endif
/////////////////////////////////////////////////////////////////////////////
//��ʼ���������������Ϊ������ϵͳ�ṩ��Ʒ���ƺͰ�װ·��
MSCAPI BOOL WINAPI MSC_Initialize(LPCTSTR lpszProduct, LPCTSTR lpszInstallPath);
//����ʼ������������ϵͳһ���������
MSCAPI VOID WINAPI MSC_Uninitialize();
//������Ҫ�������ϵͳ�Լ����߳����У���������

//?û���init
MSCAPI BOOL WINAPI MSC_Start();
//ֹͣ��Ҫ�������ϵͳֹͣ�Լ�������
MSCAPI VOID WINAPI MSC_Stop();
//����MSCProxy����
MSCAPI BOOL WINAPI MSC_CreateMSCProxy(XMSCProxy** lppXMSCProxy);
/////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#endif