#ifndef _ASYNFUNC_INCLUDE_H_
#define _ASYNFUNC_INCLUDE_H_

#ifdef _WIN32
#pragma once
#endif

#include "criticalsection.h"

/*
 *****************���˼��˵��*********************
 ���󣺺ܶ���Դ��Ӧ���ڷ���ʼ����Ͳ���ʹ����
		ʵ���п����������̣߳����̡߳��ص������ã����ÿ���
 Ҫ��
	1��������Դʹ���ܲ��У����ֱܴ���һ����ȫ��������������
	2��һ����ʼ����ʼ���������̵߳���Դʹ��Ӧ�þͲ����ˣ����߾����ͷ���Դ��ʹ��

 ʵ�֣�
	1����Դ��ʹ������ͷ���ʼ����ʶ���ݵ�������ռ
	2����Դ��ʹ��ʹ�����ü���ͳ��
	3������ʼ����ʱ���ʶ�Ժ�����������Դʹ����
		������������������ʹ�õ���Դ����Ҫ�ȴ���Դ�ͷ�
	4��������Դʹ�ú�Ҫ��ʱ��ʱ�ͷ����ü���

 ���庯��:
	1��ʹ����Դǰʹ��CanRun()�Ƿ�������У����ԵĻ���ռ������Դ
	2��ʹ�������ExitRun�ͷ���Դ
	3������ʼ������CancelRun�ȴ���־��������ʹ����Դ�ҵȴ�����ʹ�õ���Դ�ͷ�

 ��Դʹ�÷���Ŀ��Ӧ�ã��Զ������ͷ���Դ��
	��CAutoAsynFunc�������
*/



class CAsynFunc
{
public:
//////////////////////////////////////////////////////////////////////////
//��ռʹ����Դ�ĺ���ʹ��

	//Ҫʹ����Դ�ĺ�����ڴ����ã��ж��Ƿ��������
	BOOL CanRun()
	{
		CStackAutoCSLock lock(AsynFunc);
		RASSERT(m_canRun, FALSE);

		//�������У��Ǿ��������е����ü���
		m_runCount++;
		if(1 == m_runCount && m_hRunFinish)	//����ǵ�һ�����Ǿ͵�����������ɵ��ź�
			ResetEvent(m_hRunFinish);

		return TRUE;
	}

	BOOL ExitRun()
	{
		CStackAutoCSLock lock(AsynFunc);
		RASSERT(m_runCount > 0, TRUE);

		m_runCount--;
		if(0 == m_runCount && m_hRunFinish)//��������һ�����Ǿ͵���������ɵ��ź�
			SetEvent(m_hRunFinish);

		return TRUE;
	}

//////////////////////////////////////////////////////////////////////////
//�ͷ���Դ��λ��ʹ��

	//�����ͷ���Դ�ĺ������ã����ҿ��Եȵ�����ʹ����Դ�ĺ���������
	BOOL CancelRun(BOOL bNeedWaitFinish = TRUE)
	{
		BOOL bWaitFinish = FALSE;

		{
			 CStackAutoCSLock lock(AsynFunc);
			//���ñ�־��ȷ���ٵ���CanRun������FALSE
			m_canRun = FALSE;

			//���ж������Ѿ��н��뺯�����ٴ��ˣ�ֻ��Ҫ���Ǿ͵ȵ����ǽ���
			if(m_runCount > 0)
				bWaitFinish = TRUE;	//�Ǿ���Ҫ�ȴ������˳���
		}

		if(!bNeedWaitFinish)
			m_bWaitFinish = bWaitFinish;

		if(bNeedWaitFinish && bWaitFinish)
			WaitFinish();

		return TRUE;
	}

	BOOL WaitFinish()
	{
		if(m_bWaitFinish && m_hRunFinish)
			WaitForSingleObject(m_hRunFinish, INFINITE);

		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	CAsynFunc():m_canRun(TRUE),m_runCount(0),m_bWaitFinish(FALSE),m_hRunFinish(NULL)
	{
		m_hRunFinish = CreateEvent(NULL, TRUE, TRUE, NULL);
	}

	~CAsynFunc()
	{
		if(m_hRunFinish)
		{
			CloseHandle(m_hRunFinish);
			m_hRunFinish = NULL;
		}
	}
private:
	CAutoCriticalSection AsynFunc;
	BOOL	m_canRun;
	DWORD	m_runCount;
	HANDLE  m_hRunFinish;
	BOOL	m_bWaitFinish;
};


/*�Զ�����CanRun��ExitRun�İ�����
�÷���
1������߱�CAsynFunc��������������Ա����(m_asynFunc)
2����Ҫͬ���ĺ���һ�����д
	HRESULT func()
	{
		CAutoAsynFunc autoAsyn(m_asynFunc);
		if(!autoAsyn.CanRun())
			return S_FALSE;

		...
		return S_FALSE;

		...
		return S_OK;
	}

*/
class CAutoAsynFunc
{
public:
	CAutoAsynFunc(CAsynFunc* pAsynFunc):m_canRun(TRUE), m_pAsynFunc(pAsynFunc)
	{
		if(m_pAsynFunc)
			m_canRun = m_pAsynFunc->CanRun();
	}

	~CAutoAsynFunc()
	{
		if(m_pAsynFunc)
			m_pAsynFunc->ExitRun();
	}

	BOOL CanRun(){return m_canRun;}

	CAsynFunc* m_pAsynFunc;
	BOOL m_canRun;
};

#ifndef DeclareDefaultAsynFunc
#	define DeclareDefaultAsynFunc CAsynFunc m_asynFunc;
#endif

#ifndef CancleRunAsynFunc
#	define CancleRunAsynFunc  m_asynFunc.CancelRun();
#endif 

#ifndef CanRunAsynFunc
#	define CanRunAsynFunc		   CAutoAsynFunc autoAsyn(&m_asynFunc);if(!autoAsyn.CanRun())return S_FALSE;
#endif

#endif // _ASYNFUNC_INCLUDE_H_