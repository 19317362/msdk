//
// ��Ŀ����: 
//
// ʵ�ֹ���: ��Ϊ�ڴ���������ģ�飬�����ڴ�������ӿڣ����ڸ����ڴ�ķ��䡣
//
// �ļ�����: memalloc.h
//
// ��������: 
//
// ��������: 
//
// �޸�����: 
//
// �޸�����: 
//
// �������µ��޸�������������ӣ�
//
// ʹ��˵��: 
// CValidateAlloc�࣬�����ڷ�����ڴ��ǰ����뱣����ǣ��Ǹ���ѡ��
// CStdMemAlloc�࣬����Release��ĵ���ϵͳ�ڴ����ӿ�
// CDbgMemAlloc�࣬����Debug��ĵ��ô�������Ϣ��ϵͳ�ڴ����ӿ�
//
// ����MS_pMemAlloc��ʵ��ָ�룺
// 1��Ĭ������£�û�ж���SYSLAY_VALIDATE_HEAP�꣩
//	Release�棺	MS_pMemAlloc = CStdMemAlloc��
//  Debug�棺	MS_pMemAlloc = CDbgMemAlloc��
// 2��������SYSLAY_VALIDATE_HEAP��
//	Release�棺	MS_pMemAlloc = CValidateAlloc - > CStdMemAlloc��
//  Debug�棺	MS_pMemAlloc = CValidateAlloc - > CDbgMemAlloc��
//
// �������ǿ���д�Լ����ڴ�ѵĵ��ȹ���������Щ�������Ӧ�ļ򻯡�����
//=============================================================================

#ifndef _MEMALLOC_INCLUDE_H_
#define _MEMALLOC_INCLUDE_H_
#ifdef _WIN32
#pragma once
#endif


#include <stddef.h>
#include "mem.h"


struct _CrtMemState;


enum
{
	ReportType_MemTotals = 0,
};

enum
{
	BYTE_COUNT_16 = 0,
	BYTE_COUNT_32,
	BYTE_COUNT_128,
	BYTE_COUNT_1024,
	BYTE_COUNT_GREATER,
	
	NUM_BYTE_COUNT_BUCKETS
};

struct st_Totals_MemInfo_
{
	st_Totals_MemInfo_()
	{
		memset( this, 0, sizeof(*this) );
	}
	
	// Size in bytes
	int m_nCurrentSize;
	int m_nPeakSize;
	int m_nTotalSize;
	int m_nOverheadSize;
	int m_nPeakOverheadSize;
	
	// Count in terms of # of allocations
	int m_nCurrentCount;
	int m_nPeakCount;
	int m_nTotalCount;
	
	// Count in terms of # of allocations of a particular size
	int m_pCount[NUM_BYTE_COUNT_BUCKETS];
	
	// Time spent allocating + deallocating	(microseconds)
	__int64 m_nTime;
};

struct _Crt_Mem_Status_Item : public st_Totals_MemInfo_
{
	char m_szFilename[MAX_PATH];
	int m_nLine;
};

typedef struct _Crt_Mem_Status_Item	CRT_MEM_ITEM, * PCRT_MEM_ITEM;

typedef struct _Crt_Mem_Status_Head
{
	int ReportType;
	int length;
	int count;
	int Reserved;
	CRT_MEM_ITEM item[1];
}CRT_MEM_HEAD, * PCRT_MEM_HEAD;

//-----------------------------------------------------------------------------
// �����ڴ�������ӿ�
//-----------------------------------------------------------------------------
class IMemAlloc
{
public:
	// Release versions
	virtual void *Alloc( size_t nSize ) = 0;
	virtual void *Realloc( void *pMem, size_t nSize ) = 0;
	virtual void Free( void *pMem ) = 0;
    virtual void *Expand( void *pMem, size_t nSize ) = 0;

	// Debug versions
    virtual void *Alloc( size_t nSize, const char *pFileName, int nLine ) = 0;
    virtual void *Realloc( void *pMem, size_t nSize, const char *pFileName, int nLine ) = 0;
    virtual void  Free( void *pMem, const char *pFileName, int nLine ) = 0;
    virtual void *Expand( void *pMem, size_t nSize, const char *pFileName, int nLine ) = 0;

	// ��÷����С
	virtual size_t GetSize( void *pMem ) = 0;

	// ǿ���ڷ���λ���ϲ���file + line����Ϣ
    virtual void PushAllocDbgInfo( const char *pFileName, int nLine ) = 0;
    virtual void PopAllocDbgInfo() = 0;

	// Crt���Թ���ʵ��
	virtual long CrtSetBreakAlloc( long lNewBreakAlloc ) = 0;
	virtual	int CrtSetReportMode( int nReportType, int nReportMode ) = 0;
	virtual int CrtIsValidHeapPointer( const void *pMem ) = 0;
	virtual int CrtCheckMemory( void ) = 0;
	virtual int CrtSetDbgFlag( int nNewFlag ) = 0;
	virtual void CrtMemCheckpoint( _CrtMemState *pState ) = 0;
	
	// �����µĵ��������������ӡ�ڴ�״̬	
	typedef void (*HeapReportFunc_t)( unsigned char * pBuffer, int iLen );
	virtual void DbgOutputFunc( HeapReportFunc_t func) = 0;	
	virtual void DumpStatus() = 0;

	// Crt�Ķѵ��Ը��ٱ���ӿ�
	virtual void* CrtSetReportFile( int nRptType, void* hFile ) = 0;
	virtual void* CrtSetReportHook( void* pfnNewHook ) = 0;
	virtual int CrtDbgReport( int nRptType, const char * szFile,
			int nLine, const char * szModule, const char * pMsg ) = 0;

	virtual int heapchk() = 0;
};


//-----------------------------------------------------------------------------
// ����һ��ȫ�ֵ��ڴ�������
//-----------------------------------------------------------------------------
MEM_INTERFACE IMemAlloc *MS_pMemAlloc;


#endif // _MEMALLOC_INCLUDE_H_
