/************************************************************************/
/* 
Author:

lourking. .All rights reserved.

Create Time:

	4,22th,2014

Module Name:

	dsLocalPeriodObject.h 

Abstract: �����ڶ���ָ�룬�������if��else ��ͬ��������


*/
/************************************************************************/


#ifndef __DSLOCALPERIODOBJECT_H__
#define __DSLOCALPERIODOBJECT_H__


template<class T>
class dsLocalPeroidObject
{
public:
	T m_pobj;

public:
	dsLocalPeroidObject(const T *pobj):m_pobj(pobj)
	{
		
	}

	dsLocalPeroidObject():m_pobj(NULL)
	{

	}

	~dsLocalPeroidObject()
	{
		if(NULL != m_pobj)
			delete m_pobj;
	}


	void Attach(T *pobj){
		m_pobj = pobj;
	}

	void Detach(){
		m_pobj = NULL;
	}
};

#endif /*__DSLOCALPERIODOBJECT_H__*/

