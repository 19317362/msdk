// memdbg.cpp

#ifdef _DEBUG

#include <malloc.h>
#include <string.h>
#include "mslog/Syslay/Dbg.h"
#include "mslog/Syslay/memalloc.h"
#include <crtdbg.h>
#include <map>
#include <limits.h>
#include "util\TempMem.h"
using namespace msdk;


//-----------------------------------------------------------------------------
// ���԰��ڴ������
//-----------------------------------------------------------------------------
class CDbgMemAlloc : public IMemAlloc
{
public:
	CDbgMemAlloc();
	virtual ~CDbgMemAlloc();

	// Release versions
	virtual void *Alloc( size_t nSize );
	virtual void *Realloc( void *pMem, size_t nSize );
	virtual void  Free( void *pMem );
    virtual void *Expand( void *pMem, size_t nSize );

	// Debug versions
    virtual void *Alloc( size_t nSize, const char *pFileName, int nLine );
    virtual void *Realloc( void *pMem, size_t nSize, const char *pFileName, int nLine );
    virtual void  Free( void *pMem, const char *pFileName, int nLine );
    virtual void *Expand( void *pMem, size_t nSize, const char *pFileName, int nLine );

	// ��÷����С
	virtual size_t GetSize( void *pMem );

    // ǿ���ڷ���λ���ϲ���file + line����Ϣ
    virtual void PushAllocDbgInfo( const char *pFileName, int nLine );
    virtual void PopAllocDbgInfo();

	// Crt���Թ���ʵ��
	virtual long CrtSetBreakAlloc( long lNewBreakAlloc );
	virtual	int CrtSetReportMode( int nReportType, int nReportMode );
	virtual int CrtIsValidHeapPointer( const void *pMem );
	virtual int CrtCheckMemory( void );
	virtual int CrtSetDbgFlag( int nNewFlag );
	virtual void CrtMemCheckpoint( _CrtMemState *pState );

	// �����µĵ��������������ӡ�ڴ�״̬	
	virtual void DbgOutputFunc( HeapReportFunc_t func ) {m_OutputFunc = func;}
	virtual void DumpStatus();

	// Crt�Ķѵ��Ը��ٱ���ӿ�
	virtual void* CrtSetReportFile( int nRptType, void* hFile );
	virtual void* CrtSetReportHook( void* pfnNewHook );
	virtual int CrtDbgReport( int nRptType, const char * szFile,
			int nLine, const char * szModule, const char * szFormat );

	virtual int heapchk();

private:
	enum
	{
		DBG_INFO_STACK_DEPTH = 32
	};

	struct DbgInfoStack_t
	{
		const char *m_pFileName;
		int m_nLine;
	};

	//�ڴ�����Ϣ
	typedef st_Totals_MemInfo_	MemInfo_t;
	
	//�����ڴ�����ڵ��ļ����к�
	struct MemInfoKey_t
	{
		MemInfoKey_t( const char *pFileName, int line ) : m_pFileName(pFileName), m_nLine(line) {}
		bool operator<( const MemInfoKey_t &key ) const
		{
			int iret = _stricmp( m_pFileName, key.m_pFileName );
			if ( iret < 0 )
				return true;

			if ( iret > 0 )
				return false;

			return m_nLine < key.m_nLine;
		}

		const char *m_pFileName;
		int			m_nLine;
	};

	// VC��crt���������ڴ�Ŀ�ͷ����Ӱ��	
	struct CrtDbgMemHeader_t
	{
		unsigned char m_Reserved[8];
		const char *m_pFileName;
		int			m_nLineNumber;
		unsigned char m_Reserved2[16];
	};
	
	// ʹ����STL��MAP
	typedef std::map< MemInfoKey_t, MemInfo_t > StatMap_t;
	typedef StatMap_t::iterator StatMapIter_t;
	typedef StatMap_t::value_type StatMapEntry_t;

private:
	// ��õ�ǰջ��ָ�ĵ�����Ϣ
	void GetActualDbgInfo( const char *&pFileName, int &nLine );

	// ��MAP�в��һ��ߴ����ļ��к���Ϣ��������ڴ�����Ϣ
	MemInfo_t &FindOrCreateEntry( const char *pFileName, int line );

	// ���¼�¼��״̬
	void RegisterAllocation( MemInfo_t &info, int nSize, int nTime );
	void RegisterDeallocation( MemInfo_t &info, int nSize, int nTime );

	// �����ڴ�ָ�����ļ��������к�
	const char *GetAllocatonFileName( void *pMem );
	int GetAllocatonLineNumber( void *pMem );

	// �ڲ�ʹ�õ��ڴ汨�溯��	
	void PrintMemStruct();
	inline void CopyToItem(PCRT_MEM_ITEM pItem, MemInfo_t &info)
	{
		st_Totals_MemInfo_*p1 = pItem;
		st_Totals_MemInfo_*p2 = &info;
		memcpy(p1, p2, sizeof(st_Totals_MemInfo_));
	}
		

private:
	DbgInfoStack_t	m_DbgInfoStack[DBG_INFO_STACK_DEPTH];
	int m_nDbgInfoStackDepth;
	StatMap_t m_StatMap;
	MemInfo_t m_GlobalInfo;
	CFastTimer m_Timer;

	HeapReportFunc_t m_OutputFunc;
	static int s_pCountSizes[NUM_BYTE_COUNT_BUCKETS];
	static const char *s_pCountHeader[NUM_BYTE_COUNT_BUCKETS];
};


//-----------------------------------------------------------------------------
// ����һ��ȫ��ʵ��
//-----------------------------------------------------------------------------
static CDbgMemAlloc s_DbgMemAlloc;

#ifndef SYSLAY_VALIDATE_HEAP
IMemAlloc *MS_pMemAlloc = &s_DbgMemAlloc;
#else
IMemAlloc *g_pActualAlloc = &s_DbgMemAlloc;
#endif


//-----------------------------------------------------------------------------
// ���尴��С������ڴ�飬���ڷ���ͳ��
//-----------------------------------------------------------------------------
int CDbgMemAlloc::s_pCountSizes[NUM_BYTE_COUNT_BUCKETS] = 
{
	16, 32, 128, 1024, INT_MAX
};

const char *CDbgMemAlloc::s_pCountHeader[NUM_BYTE_COUNT_BUCKETS] = 
{
	"<=16 byte allocations", 
	"17-32 byte allocations",
	"33-128 byte allocations", 
	"129-1024 byte allocations",
	">1024 byte allocations"
};


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CDbgMemAlloc::CDbgMemAlloc()
{
	m_OutputFunc = NULL;
	m_nDbgInfoStackDepth = -1;
}

CDbgMemAlloc::~CDbgMemAlloc()
{
	StatMapIter_t iter = m_StatMap.begin();
	while(iter != m_StatMap.end())
	{
		if (iter->first.m_pFileName)
		{
			char *pFileName = (char*)iter->first.m_pFileName;
			delete [] pFileName;
		}
		iter++;
	}
}


//-----------------------------------------------------------------------------
// Release versions
//-----------------------------------------------------------------------------
static char const *pUnknown = "unknown";

void *CDbgMemAlloc::Alloc( size_t nSize )
{
	// Ĭ�Ͽ����޷���λ�ļ�
	const char *pFileName = pUnknown;
	int nLine;
	GetActualDbgInfo( pFileName, nLine );
	if (pFileName == pUnknown)
	{
		//_asm int 3;
	}
	
	return Alloc( nSize, pUnknown, 0 );
//	return malloc( nSize );
}

void *CDbgMemAlloc::Realloc( void *pMem, size_t nSize )
{	
	return Realloc( pMem, nSize, pUnknown, 0 );
//	return realloc( pMem, nSize );
}

void CDbgMemAlloc::Free( void *pMem )
{
	Free( pMem, pUnknown, 0 );
//	free( pMem );
}

void *CDbgMemAlloc::Expand( void *pMem, size_t nSize )
{
	return Expand( pMem, nSize, pUnknown, 0 );
//	return _expand( pMem, nSize );
}


//-----------------------------------------------------------------------------
// ǿ���ڷ���λ���ϲ���file + line����Ϣ
//-----------------------------------------------------------------------------
void CDbgMemAlloc::PushAllocDbgInfo( const char *pFileName, int nLine )
{
	++m_nDbgInfoStackDepth;
	Assert( m_nDbgInfoStackDepth < DBG_INFO_STACK_DEPTH );
	m_DbgInfoStack[m_nDbgInfoStackDepth].m_pFileName = pFileName;
	m_DbgInfoStack[m_nDbgInfoStackDepth].m_nLine = nLine;
}

void CDbgMemAlloc::PopAllocDbgInfo()
{
	--m_nDbgInfoStackDepth;
	Assert( m_nDbgInfoStackDepth >= -1 );
}


//-----------------------------------------------------------------------------
// ��õ�ǰջ��ָ�ĵ�����Ϣ
//-----------------------------------------------------------------------------
void CDbgMemAlloc::GetActualDbgInfo( const char *&pFileName, int &nLine )
{
	if (m_nDbgInfoStackDepth >= 0)
	{
		pFileName = m_DbgInfoStack[m_nDbgInfoStackDepth].m_pFileName;
		nLine = m_DbgInfoStack[m_nDbgInfoStackDepth].m_nLine;
	}
}


//-----------------------------------------------------------------------------
// ��MAP�в��һ��ߴ����ļ��к���Ϣ��������ڴ�����Ϣ
//-----------------------------------------------------------------------------
CDbgMemAlloc::MemInfo_t &CDbgMemAlloc::FindOrCreateEntry( const char *pFileName, int line )
{	
	std::pair<StatMapIter_t, bool> retval;
	retval = m_StatMap.insert( StatMapEntry_t( MemInfoKey_t( pFileName, line ), MemInfo_t() ) );

	//Ϊ�˷�ֹdll��unload��ʧ�ļ��к���Ϣ
	if (retval.second)
	{
		int nLen = strlen(pFileName) + 1;
		char **ppStatsFileName = (char**)(&retval.first->first.m_pFileName);
		*ppStatsFileName = new char[nLen];
		memcpy( *ppStatsFileName, pFileName, nLen );
	}

	return retval.first->second;
}


//-----------------------------------------------------------------------------
// ���¼�¼��״̬
//-----------------------------------------------------------------------------
void CDbgMemAlloc::RegisterAllocation( MemInfo_t &info, int nSize, int nTime )
{
	++info.m_nCurrentCount;
	++info.m_nTotalCount;
	if (info.m_nCurrentCount > info.m_nPeakCount)
	{
		info.m_nPeakCount = info.m_nCurrentCount;
	}

	info.m_nCurrentSize += nSize;
	info.m_nTotalSize += nSize;
	if (info.m_nCurrentSize > info.m_nPeakSize)
	{
		info.m_nPeakSize = info.m_nCurrentSize;
	}

	for (int i = 0; i < NUM_BYTE_COUNT_BUCKETS; ++i)
	{
		if (nSize <= s_pCountSizes[i])
		{
			++info.m_pCount[i];
			break;
		}
	}

	Assert( info.m_nPeakCount >= info.m_nCurrentCount );
	Assert( info.m_nPeakSize >= info.m_nCurrentSize );

	// ����ʵ�ʴ�С�ȵ�
	int nActualSize = ((nSize + 0xF) & 0xFFFFFFF0);
	info.m_nOverheadSize += 12 + (nActualSize - nSize);
	if (info.m_nOverheadSize > info.m_nPeakOverheadSize)
	{
		info.m_nPeakOverheadSize = info.m_nOverheadSize;
	}

	info.m_nTime += nTime;
}

void CDbgMemAlloc::RegisterDeallocation( MemInfo_t &info, int nSize, int nTime )
{
	--info.m_nCurrentCount;
	info.m_nCurrentSize -= nSize;

	Assert( info.m_nPeakCount >= info.m_nCurrentCount );
	Assert( info.m_nPeakSize >= info.m_nCurrentSize );
	Assert( info.m_nCurrentCount >= 0 );
	Assert( info.m_nCurrentSize >= 0 );

	// ����ʵ�ʴ�С�ȵ�
	int nActualSize = ((nSize + 0xF) & 0xFFFFFFF0);
	info.m_nOverheadSize -= 12 + (nActualSize - nSize);

	info.m_nTime += nTime;
}


//-----------------------------------------------------------------------------
// �����ڴ�ָ�����ļ��������к�
//-----------------------------------------------------------------------------
const char *CDbgMemAlloc::GetAllocatonFileName( void *pMem )
{
	if (!pMem)
		return "";

	CrtDbgMemHeader_t *pHeader = (CrtDbgMemHeader_t*)pMem;
	--pHeader;
	return pHeader->m_pFileName;
}

int CDbgMemAlloc::GetAllocatonLineNumber( void *pMem )
{
	if (!pMem)
		return 0;

	CrtDbgMemHeader_t *pHeader = (CrtDbgMemHeader_t*)pMem;
	--pHeader;
	return pHeader->m_nLineNumber;
}

//-----------------------------------------------------------------------------
// Debug����ڴ����
//-----------------------------------------------------------------------------
void *CDbgMemAlloc::Alloc( size_t nSize, const char *pFileName, int nLine )
{
	GetActualDbgInfo( pFileName, nLine );

	m_Timer.Start();
	void *pMem = _malloc_dbg( nSize, _NORMAL_BLOCK, pFileName, nLine );
	m_Timer.End();

	unsigned long nTime = m_Timer.GetDuration().GetMicroseconds();

	RegisterAllocation( m_GlobalInfo, nSize, nTime );
	RegisterAllocation( FindOrCreateEntry( pFileName, nLine ), nSize, nTime );

	return pMem;
}

void *CDbgMemAlloc::Realloc( void *pMem, size_t nSize, const char *pFileName, int nLine )
{
	GetActualDbgInfo( pFileName, nLine );

	int nOldSize = GetSize( pMem );
	const char *pOldFileName = GetAllocatonFileName( pMem );
	int oldLine = GetAllocatonLineNumber( pMem );

	if ( pMem != 0 )
	{
		RegisterDeallocation( FindOrCreateEntry( pOldFileName, oldLine ), nOldSize, 0 );
		RegisterDeallocation( m_GlobalInfo, nOldSize, 0 );
	}

	m_Timer.Start();
	pMem = _realloc_dbg( pMem, nSize, _NORMAL_BLOCK, pFileName, nLine );
	m_Timer.End();
	
	unsigned long nTime = m_Timer.GetDuration().GetMicroseconds();

	RegisterAllocation( FindOrCreateEntry( pFileName, nLine ), nSize, nTime );
	RegisterAllocation( m_GlobalInfo, nSize, nTime );

	return pMem;
}

void  CDbgMemAlloc::Free( void *pMem, const char *pFileName, int nLine )
{
	if (!pMem)
		return;

	int nOldSize = GetSize( pMem );
	const char *pOldFileName = GetAllocatonFileName( pMem );
	int oldLine = GetAllocatonLineNumber( pMem );

	m_Timer.Start();
	_free_dbg( pMem, _NORMAL_BLOCK );
 	m_Timer.End();

	unsigned long nTime = m_Timer.GetDuration().GetMicroseconds();

	RegisterDeallocation( m_GlobalInfo, nOldSize, nTime );
	RegisterDeallocation( FindOrCreateEntry( pOldFileName, oldLine ), nOldSize, nTime );
}

void *CDbgMemAlloc::Expand( void *pMem, size_t nSize, const char *pFileName, int nLine )
{
	GetActualDbgInfo( pFileName, nLine );

	int nOldSize = GetSize( pMem );
	const char *pOldFileName = GetAllocatonFileName( pMem );
	int oldLine = GetAllocatonLineNumber( pMem );

	RegisterDeallocation( FindOrCreateEntry( pOldFileName, oldLine ), nOldSize, 0 );
	RegisterDeallocation( m_GlobalInfo, nOldSize, 0 );

	m_Timer.Start();
	pMem = _expand_dbg( pMem, nSize, _NORMAL_BLOCK, pFileName, nLine );
	m_Timer.End();

	unsigned long nTime = m_Timer.GetDuration().GetMicroseconds();

	RegisterAllocation( FindOrCreateEntry( pFileName, nLine ), nSize, nTime );
	RegisterAllocation( m_GlobalInfo, nSize, nTime );
	return pMem;
}


//-----------------------------------------------------------------------------
// ��÷����С
//-----------------------------------------------------------------------------
size_t CDbgMemAlloc::GetSize( void *pMem )
{
	if (!pMem)
		return 0;

	return _msize_dbg( pMem, _NORMAL_BLOCK );
}


//-----------------------------------------------------------------------------
// �򵥵ĵ���Crt�ĺ���
//-----------------------------------------------------------------------------
long CDbgMemAlloc::CrtSetBreakAlloc( long lNewBreakAlloc )
{
	return _CrtSetBreakAlloc( lNewBreakAlloc );
}

int CDbgMemAlloc::CrtSetReportMode( int nReportType, int nReportMode )
{
	return _CrtSetReportMode( nReportType, nReportMode );
}

int CDbgMemAlloc::CrtIsValidHeapPointer( const void *pMem )
{
	return _CrtIsValidHeapPointer( pMem );
}

int CDbgMemAlloc::CrtCheckMemory( void )
{
	return CrtCheckMemory( );
}

int CDbgMemAlloc::CrtSetDbgFlag( int nNewFlag )
{
	return _CrtSetDbgFlag( nNewFlag );
}

void CDbgMemAlloc::CrtMemCheckpoint( _CrtMemState *pState )
{
	_CrtMemCheckpoint( pState );
}

//-----------------------------------------------------------------------------
// Crt�ĶѸ��ٱ���ӿ�
//-----------------------------------------------------------------------------
void* CDbgMemAlloc::CrtSetReportFile( int nRptType, void* hFile )
{
	return (void*)_CrtSetReportFile( nRptType, (_HFILE)hFile );
}

void* CDbgMemAlloc::CrtSetReportHook( void* pfnNewHook )
{
	return (void*)_CrtSetReportHook( (_CRT_REPORT_HOOK)pfnNewHook );
}

int CDbgMemAlloc::CrtDbgReport( int nRptType, const char * szFile,
		int nLine, const char * szModule, const char * pMsg )
{
	return _CrtDbgReport( nRptType, szFile, nLine, szModule, pMsg );
}

int CDbgMemAlloc::heapchk()
{
	return _heapchk();
}

//-----------------------------------------------------------------------------
// ͳ�Ʊ���
//-----------------------------------------------------------------------------
void CDbgMemAlloc::PrintMemStruct()
{
	int iCount = m_StatMap.size() + 1;
	int length = sizeof(CRT_MEM_HEAD) +  sizeof(CRT_MEM_ITEM)*iCount;

	CTempMem<unsigned char> pMem(length);

	CRT_MEM_HEAD* pHead = (CRT_MEM_HEAD*)(unsigned char*)pMem;	
	pHead->ReportType = ReportType_MemTotals;
	pHead->count = iCount;
	pHead->length = length;
	pHead->Reserved = 0;
	// ��һ��Ϊͳ����
	strcpy_s(pHead->item[0].m_szFilename, MAX_PATH, "Totals");
	pHead->item[0].m_nLine = 0;
	CopyToItem(pHead->item, m_GlobalInfo);
	
	// ���������ڴ�����ļ���
	int iIndex = 0;
	PCRT_MEM_ITEM pItem = &pHead->item[1];
	StatMapIter_t iter = m_StatMap.begin();
	while(iter != m_StatMap.end())
	{
		strcpy_s(pItem[iIndex].m_szFilename, MAX_PATH, iter->first.m_pFileName);
		pItem[iIndex].m_nLine = iter->first.m_nLine;
		CopyToItem(&pItem[iIndex], iter->second);		
		iIndex++;
		iter++;
	}
	m_OutputFunc(pMem, length);
}


//-----------------------------------------------------------------------------
// ͳ�Ʊ���
//-----------------------------------------------------------------------------
void CDbgMemAlloc::DumpStatus()
{	
	if(m_OutputFunc == NULL) return ;

	PrintMemStruct();	
}



#endif // _DEBUG
