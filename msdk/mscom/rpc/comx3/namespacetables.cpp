#include "stdafx.h"
#include "util/globalevent.h"
#include "SyncObject/criticalsection.h"
#include "util/memmapfile.h"

#include "icenter_p.h"
#include "commx/commx_error.h"
#include "commx/namespacetables.h"
#include "namespacetabledef.h"

inline unsigned __int32 GetCycleCount() 
{
	srand(GetTickCount());
	return rand();
} 

BOOL IsRunServer(__int32 pid)
{
	BOOL bRun = FALSE;
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if(hProcess!=NULL)
	{
		//if(WAIT_TIMEOUT==::WaitForSingleObject(hProcess, 0))
		//{
			bRun = TRUE;
		//}
		CloseHandle(hProcess);
	}
	else
	{
		if(GetLastError()==5)
			bRun = TRUE;
	}
	return bRun;
}

//�������̷��������ֹ����������������ֵ���ɾ���
void CCCenterNames::RandName(DWORD dwParam)
{
	//ͨ������ID�ͽ��̴�����ʱ������ʾһ��Ψһ�Ľ��̡�
	//ͨ������������ʽ���ⴴ�������endpoint���������endpoint��ʹrpc����ʱ�ⴴ���������߳�
	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());
	if(hProc)
	{
		//ȡ��ʱ����Ϣ��
		FILETIME tCreate, tExit, tKernel, tUser;
		if(GetProcessTimes(hProc,
			&tCreate,
			&tExit,
			&tKernel,
			&tUser)  
			)
		{
			wsprintf(s_szServerName, "%d_%d_%d_%d", RPC_CALL_INTERFACE_VER, GetCurrentProcessId(), tCreate.dwHighDateTime, tCreate.dwLowDateTime);
		}
		CloseHandle(hProc);
	}

	if(strlen(s_szServerName) == 0) //���ɹ��Ļ�ʹ��������
	{
		if(0==s_iProcess)//ȷ�����ֻ�����һ�Σ�
		{
			s_iProcess = GetCycleCount();
			if(0==s_iProcess) s_iProcess = GetCycleCount();//̫���˰�

			//������EndPoint����ֻ�ͽӿڰ汾������id���
			wsprintf(s_szServerName, "%d_%d_%d", RPC_CALL_INTERFACE_VER, GetCurrentProcessId(), s_iProcess);
		}
	}
}

HRESULT CCCenterNames::Open(LPCSTR szName)
{	
	RandName(0);
	return OpenMap("ms_server_table_", szName);
}
HRESULT CCCenterNames::Close()
{		
	return CloseMap();
}
//����(ͨ��pid)���ڵ�ServerName���ظ��˸����Ƿ�ǿ�Ƹ��Ǿ�����������E_COMMX_ADD_EXIST_ONE�����߸����ϵļ�¼
HRESULT CCCenterNames::Add(SERVERINFO* psi, BOOL bForce)
{
	HRESULT hr = S_OK;
	CStackAutoMutexLock ssl(m_tablelock);

	for(__int32 i=0;i<m_iMaxCount;i++)
	{
		//��λ��飬�õ��Ƿ�Ϊ���еĿ�
		if( 0!=CheckAllocBlock(i) )
		{
			//����Ƿ�Ϊƥ��
			SERVERINFO* pCur = GetArray(i);
			if(*psi==*pCur)
			{
				if(!bForce)
				{
					Assert(pCur->pid==psi->pid);
					Assert(pCur->interid==psi->interid);
					CopyMemory(psi, pCur, sizeof(SERVERINFO));
					hr = E_COMMX_ADD_EXIST_ONE;
					break;
				}
				else
				{
					//ǿ�Ƹ���
					psi->pobjid = GetCycleCount();
					if(0==psi->pobjid) psi->pobjid = 1;//̫���˰�
					lstrcpy(psi->ServerName, s_szServerName);//�̶�����

					CopyMemory(pCur, psi, sizeof(SERVERINFO));

					Msg("ServerTable: Replace index = %d,  pid=%d, pobjid=%d in %d\n", i, psi->pid, psi->pobjid, GetCurrentProcessId());
					return hr;
				}
			}
		}
	}
	if(hr==S_OK)
	{
		//����һ��
		__int32 iFree = GetFreeIndex();
		if(FIND_INVALID_INDEX==iFree)
		{
			if(FreeUnused()==0)
			{
				Assert(!"����̫����");
				hr = E_COMMX_OUTOF_PROCESS_BLOCK;
			}
			else
				iFree = GetFreeIndex();
		}
		if(FIND_INVALID_INDEX!=iFree)
		{
//#pragma	reminder("Ӧ�ø����������ֻҪ�ܹ���֤ȫϵͳΨһ������ֻ�����д��һ��")
			psi->pobjid = GetCycleCount();
			if(0==psi->pobjid) psi->pobjid = GetCycleCount();//̫���˰�
			lstrcpy(psi->ServerName, s_szServerName);//�̶�����

			SetAllocBlock(iFree);
			CopyMemory(GetArray(iFree), psi, sizeof(SERVERINFO));

			Msg("ServerTable: Add index = %d,  pid=%d, pobjid=%d in %d\n", iFree, psi->pid, psi->pobjid, GetCurrentProcessId());
		}
	}		
	return hr;
}

__int32 CCCenterNames::s_iProcess = 0;
CHAR CCCenterNames::s_szServerName[SERVER_RPC_NAME_MAX] = {0};


//����ͨѶ�������ֹ����������������ֵ���ɾ���
HRESULT CClientObjectNames::Open(LPCSTR szName)
{
	return OpenMap("ms_client_table_", szName);
}
HRESULT CClientObjectNames::Close()
{		
	return CloseMap();
}
//��ѯ�Ƿ���ڣ�����Ѿ���һ���򷵻ش��ڵ�objid������E_COMMX_ADD_EXIST_ONE��û�������һ���µ����ֲ������µ�objid���سɹ�
HRESULT CClientObjectNames::Add(CLIENTINFO* pci, BOOL bForce)
{
	HRESULT hr = S_OK;		
	CStackAutoMutexLock ssl(m_tablelock);
	//��Ҫ����һ�´���ļ�¼������ĳ�����̱��������µļ�¼
	for(__int32 i=0;i<m_iMaxCount;i++)
	{
		//��λ��飬�õ��Ƿ�Ϊ���еĿ�
		if( 0!=CheckAllocBlock(i) )
		{
			//����Ƿ�Ϊƥ��
			CLIENTINFO* pCur = GetArray(i);
			if(*pci==*pCur)
			{
				//����ͬ��������, ��Ҫ�ж��˽����Ƿ����������
				if(IsRunServer(pCur->pid) && !bForce)
				{						
					Assert(pCur->maskid==pci->maskid);
					CopyMemory(pci, pCur, sizeof(CLIENTINFO));
					hr = E_COMMX_ADD_EXIST_ONE;
					break;
				}
				else
				{
					//����Ĩ����������ļ�¼
					SetFreeBlock(i);
					ZeroMemory(pCur, sizeof(CLIENTINFO));
				}
			}
		}
	}
	if(hr==S_OK)
	{
		//����һ��
		__int32 iFree = GetFreeIndex();
		if(FIND_INVALID_INDEX==iFree)
		{
			if(FreeUnused()==0)
			{
				Assert(!"����̫����");
				hr = E_COMMX_OUTOF_OBJECTS_BLOCK;
			}
			else
				iFree = GetFreeIndex();
		}
		else
		{
//#pragma	reminder("Ӧ�ø����������ֻҪ�ܹ���֤ȫϵͳΨһ������ֻ�����д��һ��")
			pci->objid = GetCycleCount();
			if(0==pci->objid) pci->objid = GetCycleCount();//̫���˰�

			SetAllocBlock(iFree);
			CopyMemory(GetArray(iFree), pci, sizeof(CLIENTINFO));
			hr = S_OK;

			Msg("Add [%d, %X] index = %d, objid=%d in %d\n", pci->defid, pci->maskid, iFree, pci->objid, GetCurrentProcessId());
			//RSLOG( RSLOG_DETAIL , _T("[-COMX3.DLL-] <IClientObject* Register> [ Add [%d, %X] index = %d, objid=%d in %d]"), pci->defid, pci->maskid, iFree, pci->objid, GetCurrentProcessId());
		}
	}		
	return hr;
}