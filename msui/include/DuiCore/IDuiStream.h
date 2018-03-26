
#pragma once
namespace DuiKit{;

struct IDuiByteStream : public IDuiObject
{
	virtual BOOL SetBuffer(LPBYTE lpBuf,DWORD dwSize,DWORD dwStartIndex = 0) = 0;

	//��֤buffer�ڲ����ڴ���DwDesiredSize �Ĵ�С
	//0,Ĭ�� 256 �ֽ�
	virtual BOOL Reserve(DWORD dwDesiredSize = 0) = 0;

	//ȡ��buffer�ڲ����ڴ��ǵĴ�С
	virtual DWORD GetReservedSize() = 0;

	//ȡ�õ�ǰbuffer ����Ч���ݵĴ�С
	virtual DWORD GetBufferSize() = 0;
	virtual VOID  SetBufSize(DWORD dwSize) = 0;

	//��ȡ�ڲ����ݣ�����ȫ
	virtual LPBYTE GetData() = 0;
	

	//��¡һ������ǰbufferһ����buffer,����buf���಻����
	virtual IDuiByteStream* Clone() = 0;

	//��һ��buffer���ӵ�β����ԭ�����ݲ���
	//��Ӱ�쵽 ���ݵĴ�С���ڲ�����Ĵ�С
	virtual BOOL AddTail(LPBYTE lpBuf,DWORD dwSize) = 0;
	virtual BOOL AddTail(BYTE lpBuf,DWORD dwCch) = 0;

	//��һ��buffer���ӵ�ͷ����ԭ�����ݲ���
	//��Ӱ�쵽 ���ݵĴ�С���ڲ�����Ĵ�С
	virtual BOOL AddHead(LPBYTE lpBuf,DWORD dwSize) = 0;

	//����ڲ����ݣ����ǻ��������ȱ��ֲ���
	virtual BOOL Clear() = 0;

	//����ڲ����ݣ�ͬʱ�ͷ��ڴ�
	virtual BOOL ClearAndFree() = 0;
};

};//namespace DuiKit{;