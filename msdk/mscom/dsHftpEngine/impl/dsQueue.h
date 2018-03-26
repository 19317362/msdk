/************************************************************************/
/* 
Author:

lourking. All rights reserved.

Create Time:

	4,22th,2014

Module Name:

	dsQueue.h 

Abstract: ��� ������̺����ݵ� ����ͬ�������¼�����


*/
/************************************************************************/

#ifndef __DSQUEUE_H__
#define __DSQUEUE_H__

#include <list>
using namespace std;

#include "dsSyncDataWork.h"


template<class T>
class dsSyncQueue:
	public list<T>,
	public dsExtLock
{
public:

	void Push(T &_data){

		dsLocalRefLock lock(GetLock());
		push_back(_data);
	}

	BOOL Empty(){
		dsLocalRefLock lock(GetLock());
		return empty();
	}

	BOOL Pop(__out T &_data){ //�ڲ��Ѿ��ж�empty

		dsLocalRefLock lock(GetLock());

		if(empty())
			return FALSE;

		_data = front();

		pop_front();

		return TRUE;
	}

	void Clear(){
		dsLocalRefLock lock(GetLock());		
		clear();
	}


	int Size(){
		return size();
	}
};


template<class T>
class dsWorkQueue:
	public list<T>,
	public dsSyncDataWork
{
public:
	dsWorkQueue(BOOL bManualReset = FALSE):dsSyncDataWork(bManualReset)
	{
	}
public:

	void Push(T &_data){

		dsLocalRefLock lock(GetLock());
		push_back(_data);
		SetWork();
	}

	BOOL Empty(){
		dsLocalRefLock lock(GetLock());
		return empty();
	}

	BOOL Pop(__out T &_data){ //�ڲ��Ѿ��ж�empty

		dsLocalRefLock lock(GetLock());

		if(empty())
			return FALSE;

		_data = front();

		pop_front();

		return TRUE;
	}

	void Clear(){
		dsLocalRefLock lock(GetLock());		
		clear();
	}

	int Size(){
		return size();
	}
};

#endif /*__DSQUEUE_H__*/