// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// sqlitedb.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������
LPWSTR MyA2W( LPCSTR szA )
{
	if( !szA || !szA[0] ) 
		return NULL;

	LPWSTR szW = NULL;
	int len = MultiByteToWideChar( CP_UTF8, 0, szA, -1, NULL, 0 );
	szW = new WCHAR[len];
	MultiByteToWideChar( CP_UTF8, 0, szA, -1, szW, len );

	return szW;
}

LPSTR MyW2A( LPCWSTR szW )
{
	if( !szW || !szW[0] ) 
		return NULL;

	LPSTR szA = NULL;
	int len = WideCharToMultiByte( CP_ACP, 0, szW, -1, NULL, 0, NULL, NULL );
	szA = new CHAR[len];
	len = WideCharToMultiByte( CP_ACP, 0, szW, -1, szA, len, NULL, NULL );

	return szA;
}