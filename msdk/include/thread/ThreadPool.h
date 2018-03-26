
#pragma once
#include <map>
#include <Windows.h>
#include <process.h>
#include "rwlock.h"
namespace ThreadPool{;

/************************************************************************/
/* �ٽ���ʹ�ð�����                                                     */
/************************************************************************/
class CAutoCriticalSection
{
public:
	void Lock() {EnterCriticalSection(&m_sec);}
	void Unlock() {LeaveCriticalSection(&m_sec);}
	CAutoCriticalSection() {InitializeCriticalSection(&m_sec);}
	~CAutoCriticalSection() {DeleteCriticalSection(&m_sec);}

	CRITICAL_SECTION m_sec;
};

template <class T>
class CStackLockWrapper
{
public:

	CStackLockWrapper(T& _t) : t(_t){t.Lock();}

	~CStackLockWrapper(){t.Unlock();}
	T &t;
};

typedef CStackLockWrapper<CAutoCriticalSection> CStackAutoCSLock;

};//namespace ThreadPool


/************************************************************************/
/*                                                                      */
/************************************************************************/


/*ʹ�ð���
class CThreadPoolContext : public IThreadPoolContext
{
	virtual DWORD OnThreadPoolTaskRun(CThreadPoolTask* pTask, LPVOID lpParam)
	{
		BOOL bExit = pTask->IsTaskNeedExit();
		if (lpParam)
		{
			
		}
		
		//TODO:

		return 0;
	}
}; 


CThreadPoolContext ThreadPoolContext;
CThreadPool threadPool;
threadPool.InitThreadPool(1, &ThreadPoolContext);

threadPool.Dispatch(&threadPool);
threadPool.Dispatch(NULL);
threadPool.Dispatch(NULL);

*/

class CThreadPoolTask;  //ǰ������
class CThreadPool;

//�̳߳��ڲ�ʹ��
struct IThreadPoolTaskNotify
{
	virtual VOID OnThreadTaskRun(CThreadPoolTask* pTask) = 0;
	virtual VOID OnThreadTaskEnd(CThreadPoolTask* pTask) = 0;

	virtual DWORD OnThreadPoolTaskRun(CThreadPoolTask* pTask, LPVOID lpParam) = 0;
};


//�ⲿ�̳�ʵ��
struct IThreadPoolContext
{
	virtual DWORD OnThreadPoolTaskRun(CThreadPoolTask* pTask, LPVOID lpParam) = 0;
};


//�̳߳ص�λ
class CThreadPoolTask
{
public:
	CThreadPoolTask(IThreadPoolTaskNotify* pNotify)
	{
		m_pNotify		= pNotify;
		m_hThread		= NULL;
		m_dwThreadID	= 0;
		m_lpParam		= 0;
		m_hExit			= NULL;
		m_hWork			= NULL;
	}


	//�ж��Ƿ��˳�
	BOOL IsTaskNeedExit()
	{
		if(!m_hExit)
			return TRUE;

		return (WAIT_OBJECT_0 == WaitForSingleObject(m_hExit, 0));
	}

	DWORD GetThreadID(){return m_dwThreadID;} //��ȡ�߳�ID
	HANDLE GetThreadHandle(){return m_hThread;}//��ȡ�߳̾��

private:  //�ڲ�ʹ��
	BOOL RunTask(LPVOID lpParam)
	{
		m_lpParam = lpParam;
		
		if(!m_hWork)
			return FALSE;

		SetEvent(m_hWork);
		return TRUE;
	}

	BOOL Create()
	{
		if (!m_pNotify)
			return FALSE;
		
		if(NULL == m_hWork)
			m_hWork	= CreateEvent(NULL, FALSE, FALSE, NULL);
	
		if (m_hWork == NULL)
		{
			return FALSE;
		}

		if(NULL == m_hExit)
			m_hExit	= CreateEvent(NULL, TRUE, FALSE, NULL);

		if (m_hExit == NULL)
		{
			CloseHandle(m_hWork);
			m_hWork = NULL;

			return FALSE;
		}

		m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, this, /*CREATE_SUSPENDED*/0, (unsigned int*)&m_dwThreadID);
		if (NULL == m_hThread)
		{
			CloseHandle(m_hExit);
			m_hExit = NULL;

			CloseHandle(m_hWork);
			m_hWork = NULL;

			return FALSE;
		}

		return TRUE;
	}

	BOOL Stop()
	{
		SetEvent(m_hExit);
		return TRUE;
	}


	BOOL Destory()
	{
		Stop();
		WaitForTaskExit();
		return TRUE;
	}
	
	void WaitForTaskExit(DWORD dwMilliseconds = INFINITE)
	{
		if(m_hThread)
		{
			DWORD dwWait = WaitForSingleObject(m_hThread, dwMilliseconds);
			if(WAIT_OBJECT_0 != dwWait)
			{
				TerminateThread(m_hThread, -1);
			}

			CloseHandle(m_hThread);
			m_hThread	 = NULL;
			m_dwThreadID = 0;
		}

		if(m_hExit)
		{
			CloseHandle(m_hExit);
			m_hExit = NULL;
		}
		if (m_hWork)
		{
			CloseHandle(m_hWork);
			m_hWork = NULL;
		}

	}
protected:

	static UINT WINAPI ThreadFunc(LPVOID lp)
	{
		CThreadPoolTask* pThis = static_cast<CThreadPoolTask*>(lp);
		return pThis->ThreadProcImpl();
	}

	DWORD ThreadProcImpl()
	{
		HANDLE hWait[2] = {m_hWork,m_hExit};

		while(TRUE)
		{
			DWORD dwWait = WaitForMultipleObjects(_countof(hWait), hWait, FALSE, -1);
			if (dwWait == WAIT_OBJECT_0) 
			{//m_hWork
				m_pNotify->OnThreadTaskRun(this);
				m_pNotify->OnThreadPoolTaskRun(this, m_lpParam);
				if (!IsTaskNeedExit())
				{
					m_pNotify->OnThreadTaskEnd(this);
				}
			}
			else if (dwWait == WAIT_OBJECT_0 + 1)
			{//m_hExit
				//m_pNotify->OnThreadTaskEnd(this);
				break;
			}
		}

		return TRUE;
	}

private:
	IThreadPoolTaskNotify* m_pNotify;
	HANDLE				   m_hThread;     //�߳̾��
	DWORD				   m_dwThreadID;  //�߳�ID
	LPVOID				   m_lpParam;     //����
	HANDLE				   m_hExit;		  //�˳��¼�
	HANDLE				   m_hWork;

	friend CThreadPool;
};


//�̳߳�
class CThreadPool
	:public IThreadPoolTaskNotify
{
public:
	typedef std::map<DWORD , CThreadPoolTask*> CThreadPoolTaskMap;
public:
	
	CThreadPool()
	{
		m_dwIdleThread	= 1;	//Ĭ�Ͽ���һ���߳�
		m_pContext		= NULL;
	}

	//��ʼ��ʱ�߱����̸߳���
	BOOL InitThreadPool(DWORD dwThreadCount, IThreadPoolContext* pContext)
	{
		if(!pContext) return FALSE;
		m_pContext = pContext;

		m_dwIdleThread = dwThreadCount ? dwThreadCount : m_dwIdleThread;

		for (DWORD dwIndex = 0 ; dwIndex < m_dwIdleThread ; dwIndex++)
		{
			CThreadPoolTask* pTask = new CThreadPoolTask(this);

			if (pTask && pTask->Create())
			{
				InsertToIdleMap(pTask);
			}
			else
			{
				DestoryThreadTask(pTask);
				return FALSE;
			}
		}

		return TRUE;
	}

	BOOL UninitThreadPool()
	{
		StopAllTask();
		DestoryAllTask();
		return TRUE;
	}

	//��ǲ����
	CThreadPoolTask* Dispatch(LPVOID lpParam)
	{
		//���ȿ������ж���������û���߳�
		if (GetIdleTaskCount()) //�п��е��߳�
		{
			//������ȡ����һ��
			CThreadPoolTask* pTask = m_idleTaskMap.begin()->second;
			
			if (pTask->RunTask(lpParam))
			{
				InstertToWorkMap(pTask);
				RemoveFromIdleMap(pTask);
				return pTask;
			}
			else
			{
				RemoveFromIdleMap(pTask);
				DestoryThreadTask(pTask);
				_ASSERT("<CThreadPool><Dispatch><pTask->RunTask(lpParam)>");
			}

			return NULL;
		}
		else
		{
			//û�п����߳�,��̬����һ���߳�
			CThreadPoolTask* pTask = new CThreadPoolTask(this);
			if(!pTask) return NULL;

			if (pTask->Create() && pTask->RunTask(lpParam))
			{
 				InstertToWorkMap(pTask);
				return pTask;
			}
			else
			{
				DestoryThreadTask(pTask);
				_ASSERT("<CThreadPool><Dispatch><pTask->RunTask(pTask->Create() && pTask->RunTask(lpParam))>");
			}

			
			return NULL;
		}
	}

	//�����������
	DWORD GetWorkTaskCount()
	{
		CAutoReadLock lock(m_WorkMapCriticalSection);
		return m_workTaskMap.size();
	}

	//��������ĸ���
	DWORD GetIdleTaskCount()
	{
		CAutoReadLock lock(m_IdleMapCriticalSection);
		return m_idleTaskMap.size();
	}

protected:
	virtual VOID OnThreadTaskRun(CThreadPoolTask* pTask)
	{
		RemoveFromIdleMap(pTask);
		InstertToWorkMap(pTask);
	}

	virtual VOID OnThreadTaskEnd(CThreadPoolTask* pTask)
	{
		RemoveFromWorkMap(pTask);
		InsertToIdleMap(pTask);

		//���ն�����߳�
		CAutoWriteLock locki(m_IdleMapCriticalSection);
		if (m_idleTaskMap.size() > m_dwIdleThread)
		{
			for (CThreadPoolTaskMap::iterator it = m_idleTaskMap.begin(); it != m_idleTaskMap.end();)
			{
				if (m_idleTaskMap.size() > m_dwIdleThread  )
				{
					//����Ϊ�˲����߳��Լ��ȴ��Լ�������������
					if(pTask->GetThreadID() != it->second->GetThreadID())
					{
						CThreadPoolTask* pDelTask = it->second;
						it = m_idleTaskMap.erase(it);
						DestoryThreadTask(pDelTask);
					}
					else
						it ++;
				}
				else
					break;
			}
		}
	}

	virtual DWORD OnThreadPoolTaskRun(CThreadPoolTask* pTask, LPVOID lpParam)
	{
		return m_pContext->OnThreadPoolTaskRun(pTask,lpParam);
	}

private:

	VOID InsertToIdleMap(CThreadPoolTask* pTask)
	{
		CAutoWriteLock lock(m_IdleMapCriticalSection);
		if (m_idleTaskMap.find(pTask->GetThreadID()) == m_idleTaskMap.end())
		{
			m_idleTaskMap[pTask->GetThreadID()] = pTask;
		}
	}

	VOID RemoveFromIdleMap(CThreadPoolTask* pTask)
	{
		CAutoWriteLock lock(m_IdleMapCriticalSection);
		m_idleTaskMap.erase(pTask->GetThreadID());
	}



	VOID InstertToWorkMap(CThreadPoolTask* pTask)
	{
		CAutoWriteLock lock(m_WorkMapCriticalSection);
		if (m_workTaskMap.find(pTask->GetThreadID()) == m_workTaskMap.end())
		{
			m_workTaskMap[pTask->GetThreadID()] = pTask;
		}
	}

	VOID RemoveFromWorkMap(CThreadPoolTask* pTask)
	{
		CAutoWriteLock lock(m_WorkMapCriticalSection);
		m_workTaskMap.erase(pTask->GetThreadID());
	}

	VOID DestoryThreadTask(CThreadPoolTask* pTask)
	{
		if (pTask)
		{
			pTask->Destory();
			delete pTask;
		}
	}

	VOID StopAllTask()
	{
		CAutoReadLock r_lock_i(m_IdleMapCriticalSection);
		CAutoReadLock r_lock_w(m_WorkMapCriticalSection);

		for (CThreadPoolTaskMap::iterator it = m_workTaskMap.begin(); it != m_workTaskMap.end() ; it++)
		{
			CThreadPoolTask* pTask = it->second;
			pTask->Stop();
		}
	
		for (CThreadPoolTaskMap::iterator it = m_idleTaskMap.begin(); it != m_idleTaskMap.end() ; it++)
		{
			CThreadPoolTask* pTask = it->second;
			pTask->Stop();
		}
	}

	VOID DestoryAllTask()
	{
		{
			CAutoReadLock r_lock_i(m_IdleMapCriticalSection);
			CAutoReadLock r_lock_w(m_WorkMapCriticalSection);

			for (CThreadPoolTaskMap::iterator it = m_workTaskMap.begin(); it != m_workTaskMap.end() ; it++)
			{
				DestoryThreadTask(it->second);
			}
			
			for (CThreadPoolTaskMap::iterator it = m_idleTaskMap.begin(); it != m_idleTaskMap.end() ; it++)
			{
				DestoryThreadTask(it->second);
			}
		}
		
		{
			CAutoWriteLock w_lock_w(m_WorkMapCriticalSection);
			CAutoWriteLock w_lock_i(m_IdleMapCriticalSection);

			m_workTaskMap.clear();
			m_idleTaskMap.clear();
		}
	}
private:
	CThreadPoolTaskMap m_workTaskMap;
	CThreadPoolTaskMap m_idleTaskMap;

	//��Դ��
	//ThreadPool::CAutoCriticalSection m_WorkMapCriticalSection;
	//ThreadPool::CAutoCriticalSection m_IdleMapCriticalSection;
	CReadWriteLock m_WorkMapCriticalSection;
	CReadWriteLock m_IdleMapCriticalSection;

	DWORD	m_dwIdleThread;
	IThreadPoolContext* m_pContext;
};
