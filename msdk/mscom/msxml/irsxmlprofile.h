#ifndef IRSXMLPROFILE_H
#define IRSXMLPROFILE_H

#include <objbase.h>

interface IRsXMLProfile
{
	/**
	* �� ���� XML �ļ�
	* <p>�� ���� XML �ļ�
	* @param lpFileName			XML �ļ�����·��
	* @param bCreate			����ļ������ڣ��Ƿ񴴽�
	* @return					�ɹ�����  TRUE, ���򷵻� FALSE
	*/
	virtual BOOL Open(IN LPCTSTR lpFileName, BOOL bCreate = FALSE) = 0;
	
	/**
	* ��λ XML �ڵ�
	* <p>��λ XML �ڵ�	
	* @param lpKey				XML �ڵ�·�� "procect2007\rav\rising"
	* @param bCreate			����ļ����ڵ㣬�Ƿ񴴽�
	* @return					�ɹ�����  TRUE, ���򷵻� FALSE
	*/
	virtual BOOL Goto(IN LPCTSTR lpKey, BOOL bCreate = FALSE) = 0;

	/**
	* ��õ�ǰ�ӽڵ�����
	* <p>��õ�ǰ�ӽڵ�����
	* @param lpKeyName			ָ���ӽڵ����ƣ� NULLΪ�����ӽڵ�
	* @return					�����ӽڵ�����
	*/
	virtual INT GetKeyCount(IN LPCTSTR lpKeyName) = 0;
	/**
	* ��õ�ǰ�ڵ�����
	* <p>��õ�ǰ�ڵ�����
	* @return					��ǰ�ڵ�����
	*/
	virtual LPCTSTR GetKeyName() = 0;
	/**
	* ����ӽڵ�����
	* <p>����ӽڵ�����
	* @param nIndex				���ӽڵ�����
	* @return					ָ���ӽڵ�����
	*/
	virtual LPCTSTR GetSubKeyName(INT nIndex) = 0;
	/**
	* ��ýڵ�����
	* <p>��ýڵ�����
	* @return					���ؽڵ�����
	*/
	virtual LPCTSTR GetValue() = 0;
	/**
	* ��ýڵ�����
	* <p>��ýڵ�����
	* @param lpAtt				���Ե�����
	* @return					�������Ե�����
	*/
	virtual LPCTSTR GetAttrib(IN LPCTSTR lpAtt) = 0;
	/**
	* ��ýڵ���������
	* <p>��ýڵ���������
	* @param nIndex				�ڵ��������
	* @return					�������Ե�����
	*/
	virtual LPCTSTR GetAttribName(IN INT nIndex) = 0;
	/**
	* ��ýڵ����Ը���
	* <p>��ýڵ����Ը���
	* @param lpAttribName		ָ���ڵ��������ƣ� NULLΪ���нڵ�����
	* @return					�ڵ����Եĸ���
	*/
	virtual INT GetAttribCount(IN LPCTSTR lpAttribName) = 0;
	/**
	* �ر� XML �ļ�
	* <p>�ر� XML �ļ�
	* @return					��Զ Ϊ TRUE 
	*/
	virtual BOOL Close() = 0;

	/**
	* �趨��ǰ�ڵ�����
	* <p>�趨��ǰ�ڵ�����
	* @param lpName				�µĽڵ�����
	* @return					���� TRUE 
	*/
	virtual BOOL SetKeyName(IN LPCTSTR lpName) = 0;
	/**
	* �趨��ǰ�ڵ�����
	* <p>�趨��ǰ�ڵ�����
	* @param lpValue			�µĽڵ�����
	* @return					���� TRUE 
	*/
	virtual BOOL SetValue(IN LPCTSTR lpValue) = 0;
	/**
	* �趨��ǰ�ڵ�����
	* <p>�趨��ǰ�ڵ�����
	* @param lpAtt				�ڵ���������
	* @param lpValue			�µĽڵ���������
	* @return 
	*/
	virtual BOOL SetAttrib(IN LPCTSTR lpAtt, IN LPCTSTR lpValue) = 0;
	/**
	* ���浽�ļ�
	* <p>���浽�ļ�
	* @param lpFileName			�����ļ���ȫ·���ļ���
	* @return					
	*/
	virtual BOOL Save(IN LPCTSTR lpFileName) = 0;

	/**
	* �ͷŵ�ǰ�ӿ�
	* <p>�ͷŵ�ǰ�ӿڣ� �ͷŽӿ�ʹ���ڴ�
	* @return					���� TRUE
	*/
	virtual BOOL Release() = 0;
};

#endif // IRSXMLLORDER_H	