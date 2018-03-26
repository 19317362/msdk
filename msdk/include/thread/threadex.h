#pragma once
#include <functional>
#include <process.h>

namespace msdk{;

class CThreadEx
{
public:
	CThreadEx()
	{
		m_hThread = NULL;
		m_dwThreadId = NULL;
	}

	~CThreadEx()
	{
		if ( m_hThread )
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
	}


	BOOL Do( const std::function<void()> &f)
	{
		auto func = new std::function<void()>(f);
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, func, 0, &m_dwThreadId);
		if ( !m_hThread )
		{
			delete func;
			return FALSE;
		}
		
		return TRUE;
	}

	//����True��ʾ�����˳�������false��ʾǿ���˳�
	BOOL WaitForExit(DWORD dwMilliseconds = INFINITE)
	{
		BOOL bForce = FALSE;
		//Ϊ�˷�ֹ����ͬһ���߳������ĵ��õȴ�����������������
		if(m_hThread )
		{
			DWORD dwWait = WaitForSingleObject(m_hThread, dwMilliseconds);
			if(WAIT_OBJECT_0 != dwWait)
			{
				TerminateThread(m_hThread, -1);
				bForce = TRUE;
			}

			CloseHandle(m_hThread);
			m_hThread = NULL;
			m_dwThreadId = 0;
		}

		return bForce;
	}

private:
	static unsigned int __stdcall ThreadFun(PVOID pM)
	{
		auto func = (std::function<void()> *)pM;
		(*func)();
		delete func;

		return 0;
	}

private:
	HANDLE m_hThread;
	UINT m_dwThreadId;

};
};