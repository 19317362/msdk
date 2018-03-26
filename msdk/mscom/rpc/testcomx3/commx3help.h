// commx3help.h

#pragma once

#include "util/callapi.h"
#include "commx/commx3.h"

class CProcCom3Help : public tImpModuleMid<CProcCom3Help>
{
public:	
	ICCenter* (*RS_InitializeCallCenter)(LPCSTR szSubsysName, LPCSTR szWorkPath, DWORD dwType);
	void (*RS_UninitializeCallCenter)(ICCenter *pCCenter);
	void* (*RS_AllocateCallCenter)(size_t size);
	void (*RS_FreeCallCenter)(void *pointer);
	void (*RS_ShutDown)(DWORD dwParam);

	DECLARE_FUN_BEGIN(CProcCom3Help, "comx3.dll")
	DECLARE_FUN(RS_InitializeCallCenter)
	DECLARE_FUN(RS_UninitializeCallCenter)
	DECLARE_FUN(RS_AllocateCallCenter)
	DECLARE_FUN(RS_FreeCallCenter)
	DECLARE_FUN(RS_ShutDown)
	DECLARE_FUN_END()
};

// �������comx3�����ģ��
template<class _TBase>
class CCommX3ListObjectT
{
protected:
	ICCenter* m_pCCenter;
public:
	CCommX3ListObjectT(ICCenter* pCCenter):m_pCCenter(pCCenter){}
	virtual	~CCommX3ListObjectT(){}
public:	
	//�Լ�����Ҫ����
	virtual BOOL EnumObjectCallback(CLIENTINFO* pClientInfo){return TRUE;}

	virtual void ListObject()
	{
		_TBase* pT = static_cast<_TBase*>(this);
		HANDLE hFind;
		for(HRESULT hr=m_pCCenter->QueryFirstObject(hFind);S_OK==hr;hr=m_pCCenter->QueryNextObject(hFind))
		{
			CLIENTINFO ci;
			if(S_OK==m_pCCenter->QueryObject(hFind, &ci))
			{
				if(!pT->EnumObjectCallback(&ci))
					break;
			}
		}
	}
	virtual void ListObjectEx(CLIENTINFO* pMatchInfo)
	{
		_TBase* pT = static_cast<_TBase*>(this);
		HANDLE hFind;
		for(HRESULT hr=m_pCCenter->QueryFirstObjectEx(hFind, pMatchInfo);S_OK==hr;hr=m_pCCenter->QueryNextObjectEx(hFind, pMatchInfo))
		{
			CLIENTINFO ci;
			if(S_OK==m_pCCenter->QueryObject(hFind, &ci))
			{
				if(!pT->EnumObjectCallback(&ci))
					break;
			}
		}
	}
};

//
//���ӣ� CCommX3ListObjectT<class _TBase> ��ʹ��:
//
// class CMyListObject : public CCommX3ListObjectT<CMyListObject>
// {
// public:
// 	CMyListObject(ICCenter* pCCenter):CCommX3ListObjectT<CMyListObject>(pCCenter){}
// 	virtual BOOL EnumObjectCallback(CLIENTINFO* pClientInfo)
// 	{
//		//��ȡ��ָ���Ķ���
// 		Msg("\tobjid=%d, defid=%d, maskid=%X, pid = %d, pobj=%d, susys=%08X\n", 
// 			pClientInfo->objid, pClientInfo->defid, pClientInfo->maskid, pClientInfo->pid, pClientInfo->pobjid, pClientInfo->subsysid);
// 		return TRUE;
// 	}
// };
//
// Test()
// {
//	...
// 	CMyListObject ls(pICenter);
// 	ls.ListObjectEx(&cTargetInfo);
//  ...
// 	ls.ListObject();
//	...
// }
//
