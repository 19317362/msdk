// srvclientinfo.h

#if !defined(_INCLUDE_SRVCLIENTINFO_H__)
#define _INCLUDE_SRVCLIENTINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

namespace msdk {

enum
{
	CLIENT_INFO_MSGS_MAX = 16,
	SERVER_RPC_NAME_MAX = 64,
	SUB_SYSTEM_NAME_MAX = 8,
};

//����ͨѶ����ʱ����ֻ�����������������Ķ��󣬻�����������������Ķ���
enum
{
	CREATE_COMM_OBJECT_MULTI = 0,
	CREATE_COMM_OBJECT_SINGLE,
};

//��ʼ�����������
enum
{	
	CCENTER_START_CLIENT = 0x1,
	CCENTER_START_SERVER = 0x2,
	CCENTER_START_BOTH = CCENTER_START_CLIENT|CCENTER_START_SERVER,
};

//������˵����
//1��defid��maskid��������һ�������Ķ��󣬶�objid���ڶ���ɹ�����ʱ�������Զ����ɵ��������
//2��maskidΪ��λ�ļ���id��Ҳ����˵��ͬ�İ�λ�ļ��ϵ�defidӦ�ö��ǲ�ͬ��
//3����ͬ��defid��ômaskid������ͬ��Ҳ�������ǲ���ͬʱ����һ����ϵͳ�У������������ڶ�����ϣ���Ϊ�ǰ�λ����ģ�
enum
{
	CI_MASK_EMPTY =		0x0,
	CI_MASK_DEFID =		0x0001,
	CI_MASK_MASKID =	0x0002,
	CI_MASK_TYPEID =	0x0004,
	CI_MASK_OBJID =		0x0008,
	CI_MASK_POBJID =	0x0010,
	CI_MASK_TID =		0x0020,
	CI_MASK_PID =		0x0040,
	CI_MASK_SID =		0x0080,
	CI_MASK_HID =		0x0100,
	CI_MASK_PMCB =		0x0200,
	CI_MASK_EMSGID =	0x0400,
	CI_MASK_DMSGID =	0x0800,

	SI_MASK_SUBSYSID =	0x1000,
	SI_MASK_INTERID =	0x2000,
	SI_MASK_SERVERNAME =0x4000,

	CI_MASK_NORMALID = CI_MASK_DEFID|CI_MASK_MASKID|SI_MASK_SUBSYSID|CI_MASK_EMSGID|CI_MASK_DMSGID,
	CI_MASK_GROUPID =				 CI_MASK_MASKID|SI_MASK_SUBSYSID|CI_MASK_EMSGID|CI_MASK_DMSGID,
	CI_MASK_EXTENDID =				  CI_MASK_OBJID|SI_MASK_SUBSYSID|CI_MASK_EMSGID|CI_MASK_DMSGID,	

	CI_MASK_FULL =		0xffffffff,
};

enum
{
	CALL_TYPE_INPROCESS =	0x1,	//������
	CALL_TYPE_INHOST =		0x2,	//����
	CALL_TYPE_INNET =		0x4,	//����

	CALL_TYPE_SYNC =		0x10,	//ͬ��
	CALL_TYPE_NSYS =		0x20,	//�첽
};

enum
{
	MASK_ALL_GROUP_OBJECTS =			0xffffffff,

	DEFINE_ALL_OBJECTS =				-1L,					//�����ڲ�����Ķ��󣺴������ж����������maskid=0��
};

typedef __int32 MSGID;
typedef __int32 DEFID;
typedef DEFID MASKID;
typedef __int32 OBJID;
typedef __int32 SUBSYSID;
typedef __int32 INTERID;

typedef struct st_Client_Info
{
	DWORD dwSize;
	DWORD dwMask;
	DWORD dwType;
	DEFID defid;
	MASKID maskid;
	SUBSYSID subsysid;
	OBJID objid;
	OBJID pobjid;	
	__int32 tid;
	__int32 pid;
	__int32 sid;
	__int32 hid;
	__int32 pMessageCallBack;//LPVOID
	MSGID enablemsgs[CLIENT_INFO_MSGS_MAX];
	MSGID disablemsgs[CLIENT_INFO_MSGS_MAX];	
}CLIENTINFO;
inline bool operator==(const CLIENTINFO& _X,	const CLIENTINFO& _Y)
{
	DWORD dwMask = _X.dwMask;

	bool rt = true;
	if(DEFINE_ALL_OBJECTS==_X.defid)//DEFINE_ALL_OBJECTS��ʾ���ж���
	{
		rt &= true;
		if(0==_Y.maskid)//���_YΪ0���������κ��飬�����Ͳ����ٱȽ����ˡ�
		{
			dwMask &= ~CI_MASK_MASKID;
		}
	}
	else
	{
		if(CI_MASK_DEFID & dwMask)
			rt &= (_X.defid==_Y.defid);
	}	
	if(CI_MASK_MASKID & dwMask)
	{
		if(_X.maskid!=_Y.maskid)
			rt &= (0!=(_X.maskid&_Y.maskid));
	}
	/* ��ͬ����ϵͳ�ڲ�ͬ�Ĺ����ڴ��ļ��У���������Ӧ���Ƕ���ͬ��
	if(SI_MASK_SUBSYSID & dwMask)
		rt &= (_X.subsysid==_Y.subsysid);
	*/
	if(CI_MASK_TYPEID & dwMask)//Ŀ������Ƿ��з�������
		rt &= (0!=(CCENTER_START_SERVER&_Y.dwType));

	if(CI_MASK_OBJID & dwMask)
		rt &= (_X.objid==_Y.objid);

	if(CI_MASK_POBJID & dwMask)
		rt &= (_X.pobjid==_Y.pobjid);

	if(CI_MASK_PID & dwMask)
		rt &= (_X.pid==_Y.pid);

	if(CI_MASK_SID & dwMask)
		rt &= (_X.sid==_Y.sid);

	if(CI_MASK_HID & dwMask)
		rt &= (_X.hid==_Y.hid);
	
	return rt;
}

typedef struct st_Server_Info
{
	DWORD dwSize;
	DWORD dwMask;
	DWORD dwType;
	__int32 pid;
	SUBSYSID subsysid;
	INTERID interid;
	OBJID pobjid;
	CHAR ServerName[SERVER_RPC_NAME_MAX];
}SERVERINFO;
inline bool operator==(const SERVERINFO& _X,	const SERVERINFO& _Y)
{
	DWORD dwMask = _X.dwMask;	

	bool rt = true;
	if(CI_MASK_TYPEID & dwMask)//Ŀ������Ƿ��з�������
		rt &= (0!=(CCENTER_START_SERVER&_Y.dwType));
	if(CI_MASK_PID & dwMask)
		rt &= (_X.pid==_Y.pid);
	if(CI_MASK_POBJID & dwMask)
		rt &= (_X.pobjid==_Y.pobjid);
	if(SI_MASK_SUBSYSID & dwMask)
		rt &= (_X.subsysid==_Y.subsysid);
	if(SI_MASK_INTERID & dwMask)
		rt &= (_X.interid==_Y.interid);
	return rt;
}

typedef struct st_Message_Info
{
	DWORD dwSize;
	DWORD dwCallType;
	DWORD dwTimeout;	

	DEFID source_defid;
	MASKID source_maskid;
	OBJID source_objid;

	DEFID defid;
	MASKID maskid;
	OBJID objid;

	SUBSYSID subsysid;

	MSGID msgid;
	byte* pInData;
	__int32 nInCch;
	byte** ppOutData;
	__int32* pOutCch;
}MESSAGEINFO;

struct ISendMessage;
struct IReceMessage;
struct IMessageCallBack;
struct IClientObjectManager;
struct IClientObjectNames;
struct IMessageQuene;
struct IThreadPools;
struct ICCenter;
struct IServer;
struct ICCenterObject;
struct ICCenterNamesManager;
struct ICommInProcess;
struct ICommInHost;
struct IClientObject;

}	//namespace msdk
#endif // !defined(_INCLUDE_SRVCLIENTINFO_H__)
