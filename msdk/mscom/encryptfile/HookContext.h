#pragma once

#include <map>
#include <string>
#include <set>
#include "hookapi.h"
#include "AsynFunc.h"
//////////////////////////////////////////////////////////////////////////
//�����������



typedef struct _HOOK_CONTEXT_
{
	int		nHookTag;	//�������
	DWORD	dwThread;	//���ú������߳�
	LPVOID  pUserData;	//�û�����

	BOOL	bHasCall;
	DWORD	dwEnterCount;
	_HOOK_CONTEXT_* PrevContext; //��һ������
	_HOOK_CONTEXT_* NextContext; //��һ������


} HOOK_CONTEXT,*LPHOOK_CONTEXT;





struct mpool;
class CHookContext
{
public:
	static CHookContext& GetInstance();

	//��ǰ�̵߳�hook������
	LPHOOK_CONTEXT GetHookContext();

	//��ȡ��ǰ�Ǻ�����ǵ�������
	LPHOOK_CONTEXT GetHookContext(int nTag);


	LPHOOK_CONTEXT CreateContext(int nTag);
	VOID DestoryContext(LPHOOK_CONTEXT pHookContext);

	//�Ƿ���HOOK��������
	BOOL IsHookContext();
private:
	LPHOOK_CONTEXT Alloc();
	VOID Free(LPHOOK_CONTEXT pContext);

private:
	CHookContext();
	~CHookContext();
	CRITICAL_SECTION m_sec;

	DWORD			m_dwTlsIndex;

	mpool*			m_MemPool;
};


typedef DWORD (*pRegeditHookEntry)(PHOOK_ENTRY* ppHookEntry); 
class CHookManager
{
public:
	static CHookManager& GetInstance();

	PHOOK_ENTRY GetEntry(int nHookTag);

	//��ȡԭʼ��ַ
	FARPROC GetHookRealAddress( int nHookTag);

	//��ȡ�µ�ַ
	FARPROC GetHookNewAddress(int nHookTag);
	
	BOOL	IsHook(int nHookTag);

	BOOL	SetRegeditHookEntry(pRegeditHookEntry pFunc);

	//BOOL	HookCanRun();

	//ִ��HOOK
	BOOL DoHook();
	BOOL DoUnHook();
private:

	typedef std::map<int, HOOK_ENTRY> CHookEntryMap;
	CHookEntryMap m_hookEntryMap;

	typedef std::set<pRegeditHookEntry> CRegeditHookEntrySet;
	CRegeditHookEntrySet m_RegeditHookEntrySet;

	TCHAR				m_s_ini_path[MAX_PATH];
};

class CHookContextCheck
{
public:
	CHookContextCheck(int nTag);
	~CHookContextCheck();

	LPHOOK_CONTEXT GetContext();
	BOOL IsReEnter();

	LPHOOK_CONTEXT m_pContext;
};


/*
*/
// class CSetLastErrorHelp
// {
// public:
// 	CSetLastErrorHelp(DWORD dwLastError = GetLastError());
// 	~CSetLastErrorHelp();
// 	DWORD m_dwError;
// };

/**
��ȡHOOK��ԭʼ��ַ
*/
template<typename T>
T GetRealAddress(int nTag)
{
	return (T)CHookManager::GetInstance().GetHookRealAddress(nTag);
}

/**
��ȡHOOK���µ�ַ
*/
template<typename T>
T GetNewAddress(int nTag)
{
	return (T)CHookManager::GetInstance().GetHookNewAddress(nTag);
}


//�жϺ����Ƿ�HOOK
BOOL IsEntryHooked(int nTag); 

//////////////////////////////////////////////////////////////////////////
//
class CRegeditHookEntryHelp
{
public:
	CRegeditHookEntryHelp(pRegeditHookEntry pFunc);
};

#define DECLARE_HOOK_ENTRY_BEGIN(_space)\
	static DWORD RegeditHookEntry__##space##(PHOOK_ENTRY* ppHookEntry)\
	{static  HOOK_ENTRY g_entry[] = {\


//��ȡԭʼ����
#define GetRealProcAddress(module, func) GetProcAddressEx(GetModuleHandleA(module), func)
//���平����ڵ�
#define DEFINE_HOOK_ENTRY_NAME(tag, module, realf, newf,user, hooked) {tag, module, realf,  GetRealProcAddress(module, realf) , (FARPROC)&newf, user, hooked}

#define DEFINE_HOOK_ENTRY_POINT(tag, module, realf, newf,user, hooked){tag, module, #realf,  (FARPROC)&realf , (FARPROC)&newf, user, hooked}

#define DECLARE_HOOK_ENTRY_END(_space) }; *ppHookEntry = g_entry;return _countof(g_entry);} 


#define DEFINE_HOOK_ENTRY_HELP(_space) static CRegeditHookEntryHelp g__##_space##(RegeditHookEntry__##space);