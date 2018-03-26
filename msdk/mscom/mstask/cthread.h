////////////////////////////////////////////////////////////////////////////////
/**
* @file
* �ļ���Ҫ˵��
* <p>��Ŀ���ƣ����ǵ���2008��ɱ�����
* <br>�ļ����ƣ�cthread.h
* <br>ʵ�ֹ��ܣ�
* <br>��    �ߣ����t��
* <br>��д���ڣ�2007-4-21 14:05:58
* <br>
* <br>�޸���ʷ�����¶�����д ���ݰ���������  �޸���  �޸�˵����
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CTHREAD_H__
#define __CTHREAD_H__




#ifndef _WINDOWS_
#include <windows.h>
#endif

#include <process.h>


class CThread 
{
public:	

	CThread ()
	{
		hThread		= NULL;
		hThreadId		= 0;
		hMainThread	= 0;
		hMainThreadId = 0;
		Timeout		= 2000; //milliseconds
	}

	virtual ~CThread ()
	{
		//waiting for the thread to terminate
		TerminateThread();
	}

	int	IsCreated ()
	{	return (hThread != NULL);	}

	void SetTimeOut(DWORD Time)
	{
		Timeout = Time;
	}

	HANDLE	GetThreadHandle ()
	{	
		return hThread;	
	}

	DWORD	GetThreadId ()
	{	
		return hThreadId;	
	}

	HANDLE	GetMainThreadHandle ()
	{	
		return hMainThread;	
	}

	DWORD	GetMainThreadId ()
	{	
		return hMainThreadId;	
	}

	BOOL IsAlive() 
	{
		DWORD status = 0;
		::GetExitCodeThread(hThread, &status);

		if((WAIT_OBJECT_0 != ::WaitForSingleObject(hThread, 0)) && (status ==  STILL_ACTIVE))
			return TRUE;

		return FALSE;
	}

	//�����߳�
	BOOL CreateThread (unsigned ( __stdcall * func)( void * ),void * lParam,unsigned initflag)
	{
		if (!IsCreated () && func) 
		{	
			unsigned id = 0;
			
			hMainThread	= ::GetCurrentThread ();
			hMainThreadId = ::GetCurrentThreadId ();
			
			hThread = (HANDLE)::_beginthreadex 
				(NULL, 
				0, 
				func, 
				lParam, 
				initflag, 
				&id
				);
			
			hThreadId = id;
			
			return hThread ? TRUE : FALSE;
		}
		
		return FALSE;
	}

	//�����߳���Ϣ
	BOOL PostThreadMessage(UINT Msg,WPARAM wParam,LPARAM lParam)
	{
		if (!IsCreated())
		{
			return FALSE;	
		}

		return ::PostThreadMessage(hThreadId,Msg,wParam,lParam);
	}
	
	//�ָ��߳�ִ��
	DWORD ResumeThread()
	{
		return ::ResumeThread(hThread);
	}

	//ֹͣ�߳�ִ��
	DWORD SuspendThread()
	{
		return ::SuspendThread(hThread);
	}

	//�����߳�
	void TerminateThread()
	{
		if (IsCreated()) 
		{
			if (::WaitForSingleObject (hThread, Timeout) == WAIT_TIMEOUT)
				::TerminateThread (hThread, 1);

			::CloseHandle (hThread);

			hThread		= NULL;
			hThreadId		= 0;
			hMainThread	= 0;
			hMainThreadId = 0;
		}
	}

protected:

	DWORD		Timeout;
	DWORD		hThreadId;
	HANDLE		hThread;
	DWORD		hMainThreadId;
	HANDLE		hMainThread;

private:
	 CThread (const CThread& );
	 const CThread operator= (const CThread&);
};


template <class T>
struct ThreadData
{
public:
	typedef void (T::*TFunc)();	
	T* pThreadObject;
	TFunc pThreadFunc;
	static unsigned __stdcall _ThreadFunc(ThreadData<T>* pThis)
	{
		((*(pThis->pThreadObject)).*(pThis->pThreadFunc))();
		delete pThis;
		return 0;
	}
};

template <class T>
BOOL CreateMemberThread(T* p,void (T::*func)(),CThread& Thread,unsigned initflag = 0)
{
	ThreadData<T> * ptd = new ThreadData<T>;
	ptd->pThreadObject=p;
	ptd->pThreadFunc=func;

	return Thread.CreateThread(
		(unsigned ( __stdcall * )( void * ))ThreadData<T>::_ThreadFunc,
		(void*)ptd,initflag);
}

template <class T>
struct ThreadDataEx
{
public:
	typedef void (T::*TFunc)(LPVOID);	
	T* pThreadObject;
	TFunc pThreadFunc;
	LPVOID lpThreadPram;
	static unsigned __stdcall _ThreadFuncEx(ThreadDataEx<T>* pThis)
	{
		((*(pThis->pThreadObject)).*(pThis->pThreadFunc))(pThis->lpThreadPram);
		delete pThis;
		return 0;
	}
};

template <class T>
BOOL CreateMemberThreadEx(T* p,void (T::*func)(LPVOID),CThread& Thread,LPVOID lpThreadPram = NULL,unsigned initflag = 0)
{
	ThreadDataEx<T>* ptd = new ThreadDataEx<T>;
	ptd->pThreadObject=p;
	ptd->pThreadFunc=func;
	ptd->lpThreadPram = lpThreadPram;
	
	return Thread.CreateThread(
		(unsigned ( __stdcall * )( void * ))ThreadDataEx<T>::_ThreadFuncEx,
		(void*)ptd,initflag);
}

#endif //__CTHREAD_H__