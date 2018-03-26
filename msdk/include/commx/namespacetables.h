// namespacetables.h

#pragma once
#include "srvclientinfo.h"

namespace msdk {

#define FIND_INVALID_INDEX				(__int32(-1))

typedef __int32 HANDLE32;

//�����������ֹ����������������ֵ���ɾ���
struct ICCenterNames : public IMSBase
{
public:
	virtual HRESULT Open(LPCSTR szName) = 0;
	virtual HRESULT Close() = 0;
	virtual HRESULT FlushData() = 0;

	virtual HRESULT BeginTrans() = 0;
	virtual HRESULT EndTrans() = 0;
	//ͨ��pid���Ҵ��ڵ�ServerName������S_OK��û���򷵻�S_FALSE
	virtual HRESULT Find(SERVERINFO* psi) = 0;
	//����(ͨ��pid)���ڵ�ServerName���ظ��˸����Ƿ�ǿ�Ƹ��Ǿ�����������E_ADD_EXIST_ONE�����߸����ϵļ�¼
	virtual HRESULT Add(SERVERINFO* psi, BOOL bForce) = 0;
	//
	virtual HRESULT Delete(SERVERINFO* psi) = 0;
};

MS_DEFINE_IID(ICCenterNames, "{503FFDE3-C990-4d66-B5EC-716AA57194EE}");
// {99B11F35-D7E4-46af-B1C3-90CE5ED5E871}
MS_DEFINE_GUID(CLSID_CCenterNames, 
			0x99b11f35, 0xd7e4, 0x46af, 0xb1, 0xc3, 0x90, 0xce, 0x5e, 0xd5, 0xe8, 0x71);



//����ͨѶ�������ֹ����������������ֵ���ɾ���
struct IClientObjectNames : public IMSBase
{
public:
	virtual HRESULT Open(LPCSTR szName) = 0;
	virtual HRESULT Close() = 0;
	virtual HRESULT FlushData() = 0;
	
	virtual HRESULT BeginTrans() = 0;
	virtual HRESULT EndTrans() = 0;
	//��ѯ�Ƿ���ڣ�����Ѿ���һ���򷵻ش��ڵ�objid������S_OK��û���򷵻�S_FALSE
	virtual HRESULT Find(CLIENTINFO* pci) = 0;
	//��ѯ�Ƿ���ڣ�����Ѿ���һ���򷵻ش��ڵ�objid������E_ADDEXISTONE��û�������һ���µ����ֲ������µ�objid���سɹ�
	virtual HRESULT Add(CLIENTINFO* pci, BOOL bForce) = 0;
	//ɾ��ָ���Ķ���
	virtual HRESULT Delete(CLIENTINFO* pci) = 0;

	//����
	virtual HRESULT QueryFirstObject(HANDLE32 &handle) = 0;
	virtual HRESULT QueryNextObject(HANDLE32 &handle) = 0;
	virtual HRESULT QueryObject(HANDLE32 handle, CLIENTINFO* pClientInfo) = 0;
};

MS_DEFINE_IID(IClientObjectNames, "{A1E0780D-BF48-44ed-9065-27BC15DB6608}");
// {09923979-75D7-406c-96D7-59BBEA638B3A}
MS_DEFINE_GUID(CLSID_CClientObjectNames, 
			0x9923979, 0x75d7, 0x406c, 0x96, 0xd7, 0x59, 0xbb, 0xea, 0x63, 0x8b, 0x3a);

} //namespace msdk

