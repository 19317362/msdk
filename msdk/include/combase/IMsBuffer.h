#pragma once
//////////////////////////////////////////////////////////////////////////
//���е��ڴ�Ĵ�С��λ�����ֽ�
//////////////////////////////////////////////////////////////////////////


namespace msdk{
namespace mscom{
struct IMsBuffer : public IUnknown
{
	//��ȡԭʼbufferָ��
	//��������Ƚ�Σ��,֮����Ҫ��¶��Ϊ������
	//dwBufSize �ǵ�ǰbuffer�Ĵ�С������ʵ���ڴ�Ĵ�С
	//�������ʱΪ�����٣���һ��copy����
	//ԭ�����ǲ�Ҫ�޸�lppoutData ����Ķ��������������ӿھ����ǽ���������ڴ�
	STDMETHOD(GetRawBuffer)(LPBYTE* lppoutData,DWORD* dwBufSize) = 0;

	//dwStartIndex �Ǵ�buffer���Ǹ��ڴ�ο�ʼд , ����ԭʼ����
	STDMETHOD(SetBuffer)(LPBYTE lpBuf,DWORD dwSize,DWORD dwStartIndex = 0) = 0;

	//��buf�е��ڴ渴�Ƶ��û�ָ�����ڴ���
	//lpBuf �� dwSize �����ݵ�ָ��ʹ�С
	//dwStartIndex �Ǵ�buffer���Ǹ��ڴ�ο�ʼд
	//pDwReadCount �Ƕ�ȡ�˶����ֽ�
	STDMETHOD(GetBuffer)(LPBYTE lpoutData, DWORD dwSize,DWORD* pDwReadCount,DWORD dwStartIndex = 0) = 0;

	//��֤buffer�ڲ����ڴ���DwDesiredSize �Ĵ�С
	//0,Ĭ�� 256 �ֽ�
	STDMETHOD(Reserve)(DWORD dwDesiredSize = 0) = 0;

	//ȡ��buffer�ڲ����ڴ��ǵĴ�С
	STDMETHOD_(DWORD,GetReservedSize)() = 0;

	//ȡ�õ�ǰbuffer ����Ч���ݵĴ�С
	STDMETHOD_(DWORD,GetBufferSize)() = 0;

	//��ȡ�ڲ����ݣ�����ȫ
	STDMETHOD_(const LPBYTE, GetData)() = 0;

	//��¡һ������ǰbufferһ����buffer,����buf���಻����
	STDMETHOD(Clone)(IMsBuffer** lpIBuufer) = 0;

	//��һ��buffer���ӵ�β����ԭ�����ݲ���
	//��Ӱ�쵽 ���ݵĴ�С���ڲ�����Ĵ�С
	STDMETHOD(AddTail)(LPBYTE lpBuf,DWORD dwSize) = 0;

	//��һ��buffer���ӵ�ͷ����ԭ�����ݲ���
	//��Ӱ�쵽 ���ݵĴ�С���ڲ�����Ĵ�С
	STDMETHOD(AddHead)(LPBYTE lpBuf,DWORD dwSize) = 0;

	//����ڲ����ݣ����ǻ��������ȱ��ֲ���
	STDMETHOD(Clear)() = 0;

	//����ڲ����ݣ�ͬʱ�ͷ��ڴ�
	STDMETHOD(ClearAndFree)() = 0;
};

MS_DEFINE_IID(IMsBuffer, "{42A79FCF-4F0C-44b5-8CDC-A8627CFAFBB1}");



struct IMsBufferEx
	:public IMsBuffer
{
	//�趨���ݴ�С�ĳ���
	//��������ͨ�Ų�ר�ã�������ģ�龡����Ҫ��
	STDMETHOD(SetDataSize)(DWORD dwDataSize) = 0;
};

MS_DEFINE_IID(IMsBufferEx, "{AC44D6C2-68BF-428a-8147-29FEE7EDD682}");
	// {B021C8D6-7D21-4F41-99ED-259DD3DB996B}
MS_DEFINE_GUID(CLSID_MsBuffer, 
	0xb021c8d6, 0x7d21, 0x4f41, 0x99, 0xed, 0x25, 0x9d, 0xd3, 0xdb, 0x99, 0x6b);

// {3593C74E-E047-4868-8E30-3761AEEBD3E7}
MS_DEFINE_GUID(CLSID_MsBuffer_Safe, 
	0x3593c74e, 0xe047, 0x4868, 0x8e, 0x30, 0x37, 0x61, 0xae, 0xeb, 0xd3, 0xe7);


//com��������envmgr.dll
//ֱ�ӿ���rot��ȡ��
//�ڴ������ ����һ����ط�����
interface IMsBufferMgr :public IUnknown
{
	//dwDesiredSize �ǳ�ʼ�ڴ�Ĵ�С , dwDesiredSize < 256? 256:dwDesiredSize
	STDMETHOD(CreateMsBuf)(IMsBuffer** lpbuf,DWORD dwDesiredSize) = 0;

	//�����̰߳�ȫ��buffer
	STDMETHOD(CreateSafeBuf)(IMsBuffer** lpbuf,DWORD dwDesiredSize) = 0;
};	
MS_DEFINE_IID(IMsBufferMgr, "{EFF8F1E1-5316-44fe-916D-186283D6A064}");

};//namespace mscom
};//namespace msdk

//{98FB74FA-6BE0-401b-B986-A78A80EED7A1}
MS_DEFINE_GUID(CLSID_MsBufferMgr,
			0x98fb74fa, 0x6be0, 0x401b, 0xb9, 0x86, 0xa7, 0x8a, 0x80, 0xee, 0xd7, 0xa1);
