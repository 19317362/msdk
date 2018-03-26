#ifndef _RULEBUFFER_H_
#define _RULEBUFFER_H_

/************************************************************************/
//
//		���ڿ�ģ�����������ݴ��ݣ����ڷֲ�����
//		����ģ��䴫��buf���Ѷȣ��÷���������ӵ�
//		����һ������ aa(bufdef& buf)
//		Ȼ������Ҫ�����������ʱ��
//
//		bufdef buf;
//		aa(buf);
//		���ʱ��buf��������õ�������
//
/************************************************************************/

#include <vector>

template <class T>
class CRuleBuffer
{
public:
	CRuleBuffer(){}
	typedef std::vector<T> RULEBUFFERVECTOR;

	long  GetSize() const    {return (long)m_Vector.size();}
	
	//ע�⣬rulebuf�ǿ�dll�ģ��������������virtual
	virtual void push_back(const T& Unit) {m_Vector.push_back(Unit);}

	size_t GetNoOfBytes()const  
	{
		return m_Vector.size()*sizeof(T);
	}

	T& operator[](long nIndex)
	{
		return m_Vector[nIndex];
	}

	const T& operator[](long nIndex)const
	{
		return m_Vector[nIndex];
	}

	operator const T*()const {return &m_Vector[0];}
	operator T*()            {return &m_Vector[0];}

	RULEBUFFERVECTOR& GetVector(){return m_Vector;}

	void  Clear()   {m_Vector.clear();}

protected:
	RULEBUFFERVECTOR m_Vector;
};

#endif