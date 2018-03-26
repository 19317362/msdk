//
// Ŀ�ģ����¶��������е�crt���ڴ������
// ������ļ�����������ʹ�õĹ����У�����Խ����ڴ�й©�ļ��
//
#include "Syslay/dbg.h"
#include "memalloc.h"
#include <string.h>


#ifdef _WIN32
	// ǿ�ƵĴ򿪵��Ա�־
	#define _DEBUG 1
	#include <crtdbg.h>
#elif _LINUX
	#define __cdecl
#endif



// ��׼��CRT����
#ifdef _WIN32
// ֻ����WIN32��ʹ��
// ��linux��malloc()����libc�ֵ�malloc()���ܻ����һ���ݹ���ѭ��
// ��Ϊ g_pMemAlloc->Alloc()�����malloc
extern "C"
{
	
	void *malloc( size_t nSize )
	{
		return g_pMemAlloc->Alloc(nSize);
	}

	void free( void *pMem )
	{
		g_pMemAlloc->Free(pMem);
	}

	void *realloc( void *pMem, size_t nSize )
	{
		return g_pMemAlloc->Realloc(pMem, nSize);
	}

	void *calloc( size_t nCount, size_t nElementSize )
	{
		void *pMem = g_pMemAlloc->Alloc(nElementSize * nCount);
		memset(pMem, 0, nElementSize * nCount);
		return pMem;
	}

} // end extern "C"
#endif //end _WIN32

//-----------------------------------------------------------------------------
//  CRT ����
//-----------------------------------------------------------------------------
extern "C"
{
	
	void *_malloc_base( size_t nSize )
	{
		return g_pMemAlloc->Alloc(nSize);
	}

	void _free_base( void *pMem )
	{
		g_pMemAlloc->Free(pMem);
	}

	void *_realloc_base( void *pMem, size_t nSize )
	{
		return g_pMemAlloc->Realloc(pMem, nSize);
	}

	int _heapchk()
	{
		return g_pMemAlloc->heapchk();
	}

	int _heapmin()
	{
		return 1;
	}

	size_t _msize( void *pMem )
	{
		return g_pMemAlloc->GetSize(pMem);
	}

	size_t msize( void *pMem )
	{
		return g_pMemAlloc->GetSize(pMem);
	}

	void *__cdecl _heap_alloc( size_t nSize )
	{
		return g_pMemAlloc->Alloc(nSize);
	}

	void *__cdecl _nh_malloc( size_t nSize, int )
	{
		return g_pMemAlloc->Alloc(nSize);
	}

	void *__cdecl _expand( void *pMem, size_t nSize )
	{
		return g_pMemAlloc->Expand(pMem, nSize);
	}

	unsigned int _amblksiz = 16; //BYTES_PER_PARA;

	size_t __cdecl _get_sbh_threshold( void )
	{
		return 0;
	}

	int __cdecl _set_sbh_threshold( size_t )
	{
		return 0;
	}

	int __cdecl _heapadd( void *, size_t )
	{
		return 0;
	}

	int __cdecl _heapset( unsigned int )
	{
		return 0;
	}

	size_t __cdecl _heapused( size_t *, size_t * )
	{
		return 0;
	}

	#ifdef _WIN32
		int __cdecl _heapwalk( _HEAPINFO * )
		{
			return 0;
		}
	#endif	// end _WIN32

} // end extern "C"


//-----------------------------------------------------------------------------
// ���Ժ���
//-----------------------------------------------------------------------------

extern "C"
{
	
	void *malloc_db( size_t nSize, const char *pFileName, int nLine )
	{
		return g_pMemAlloc->Alloc(nSize, pFileName, nLine);
	}

	void free_db( void *pMem, const char *pFileName, int nLine )
	{
		g_pMemAlloc->Free(pMem, pFileName, nLine);
	}

	void *realloc_db( void *pMem, size_t nSize, const char *pFileName, int nLine )
	{
		return g_pMemAlloc->Realloc(pMem, nSize, pFileName, nLine);
	}
	
} // end extern "C"



//-----------------------------------------------------------------------------
// ȷ������debug����release����������ȷ�ĺ���
//-----------------------------------------------------------------------------
void *operator new( unsigned int nSize )
{
	return g_pMemAlloc->Alloc( nSize );
}

void *operator new( unsigned int nSize, int nBlockUse, const char *pFileName, int nLine )
{
	return g_pMemAlloc->Alloc(nSize, pFileName, nLine);
}

void operator delete( void *pMem )
{
	g_pMemAlloc->Free( pMem );
}

void *operator new[] ( unsigned int nSize )
{
	return g_pMemAlloc->Alloc( nSize );
}

void *operator new[] ( unsigned int nSize, int nBlockUse, const char *pFileName, int nLine )
{
	return g_pMemAlloc->Alloc(nSize, pFileName, nLine);
}

void operator delete[] ( void *pMem )
{
	g_pMemAlloc->Free( pMem );
}


//-----------------------------------------------------------------------------
// ��׼��MSVC�Ѻ���
//-----------------------------------------------------------------------------
extern "C"
{

	int __cdecl _heap_init()
	{
		return g_pMemAlloc != NULL;
	}

	void __cdecl _heap_term()
	{
	}
	
}


//-----------------------------------------------------------------------------
// ���ú���ʱ����λ����Ϣ��LINUX ��������֧��
//-----------------------------------------------------------------------------
#ifdef _WIN32

	#define CRT_INTERNAL_FILE_NAME "C-runtime internal"

	class CAttibCRT
	{
	public:
		CAttibCRT(int nBlockUse) : m_nBlockUse(nBlockUse)
		{
			if (m_nBlockUse == _CRT_BLOCK)
			{
				g_pMemAlloc->PushAllocDbgInfo(CRT_INTERNAL_FILE_NAME, 0);
			}
		}
		
		~CAttibCRT()
		{
			if (m_nBlockUse == _CRT_BLOCK)
			{
				g_pMemAlloc->PopAllocDbgInfo();
			}
		}
		
	private:
		int m_nBlockUse;
	};


	#define AttribIfCrt() CAttibCRT _attrib(nBlockUse)
#elif _LINUX
	#define AttribIfCrt()
#endif // _WIN32


extern "C"
{
	
	void *__cdecl _nh_malloc_dbg( size_t nSize, int nFlag, int nBlockUse,
									const char *pFileName, int nLine )
	{
		AttribIfCrt();
		return g_pMemAlloc->Alloc(nSize, pFileName, nLine);
	}

	void *__cdecl _malloc_dbg( size_t nSize, int nBlockUse,
								const char *pFileName, int nLine )
	{
		AttribIfCrt();
		return g_pMemAlloc->Alloc(nSize, pFileName, nLine);
	}

	void *__cdecl _calloc_dbg( size_t nNum, size_t nSize, int nBlockUse,
								const char *pFileName, int nLine )
	{
		AttribIfCrt();
		void *pMem = g_pMemAlloc->Alloc(nSize * nNum, pFileName, nLine);
		memset(pMem, 0, nSize * nNum);
		return pMem;
	}

	void *__cdecl _realloc_dbg( void *pMem, size_t nNewSize, int nBlockUse,
								const char *pFileName, int nLine )
	{
		AttribIfCrt();
		return g_pMemAlloc->Realloc(pMem, nNewSize, pFileName, nLine);
	}

	void *__cdecl _expand_dbg( void *pMem, size_t nNewSize, int nBlockUse,
								const char *pFileName, int nLine )
	{
		AttribIfCrt();
		return g_pMemAlloc->Expand(pMem, nNewSize, pFileName, nLine);
	}

	void __cdecl _free_dbg( void *pMem, int nBlockUse )
	{
		AttribIfCrt();
		g_pMemAlloc->Free(pMem);
	}

	size_t __cdecl _msize_dbg( void *pMem, int nBlockUse )
	{
		#ifdef _WIN32
			return _msize(pMem);
		#elif _LINUX
			Assert( "_msize_dbg unsupported" );
			return 0;
		#endif
	}


} // end extern "C"


//-----------------------------------------------------------------------------
// ����һЩMSVC�йص�CRT���Ժ���
//-----------------------------------------------------------------------------
#ifdef _WIN32


	extern "C"
	{
		
	int _CrtDumpMemoryLeaks(void)
	{
		return 0;
	}

	_CRT_DUMP_CLIENT _CrtSetDumpClient( _CRT_DUMP_CLIENT dumpClient )
	{
		return NULL;
	}

	int _CrtSetDbgFlag( int nNewFlag )
	{
		return g_pMemAlloc->CrtSetDbgFlag( nNewFlag );
	}

	long _crtBreakAlloc;      /* Break on this allocation */
	int _crtDbgFlag = _CRTDBG_ALLOC_MEM_DF;

	void __cdecl _CrtSetDbgBlockType( void *pMem, int nBlockUse )
	{
		DebuggerBreak();
	}

	_CRT_ALLOC_HOOK __cdecl _CrtSetAllocHook( _CRT_ALLOC_HOOK pfnNewHook )
	{
		DebuggerBreak();
		return NULL;
	}

	long __cdecl _CrtSetBreakAlloc( long lNewBreakAlloc )
	{
		return g_pMemAlloc->CrtSetBreakAlloc( lNewBreakAlloc );
	}
						 
	int __cdecl _CrtIsValidHeapPointer( const void *pMem )
	{
		return g_pMemAlloc->CrtIsValidHeapPointer( pMem );
	}

	int __cdecl _CrtCheckMemory( void )
	{		
		return g_pMemAlloc->CrtCheckMemory( );
	}

	int __cdecl _CrtIsMemoryBlock( const void *pMem, unsigned int nSize,
		long *plRequestNumber, char **ppFileName, int *pnLine )
	{
		DebuggerBreak();
		return 1;
	}

	int __cdecl _CrtMemDifference( _CrtMemState *pState, const _CrtMemState * oldState, const _CrtMemState * newState )
	{
		DebuggerBreak();
		return FALSE;
	}

	void __cdecl _CrtMemDumpStatistics( const _CrtMemState *pState )
	{
		DebuggerBreak();	
	}

	void __cdecl _CrtMemCheckpoint( _CrtMemState *pState )
	{		
		g_pMemAlloc->CrtMemCheckpoint( pState );
	}

	void __cdecl _CrtMemDumpAllObjectsSince( const _CrtMemState *pState )
	{
		DebuggerBreak();
	}

	void __cdecl _CrtDoForAllClientObjects( void (*pfn)(void *, void *), void * pContext )
	{
		DebuggerBreak();
	}


	//-----------------------------------------------------------------------------
	// dbgrpt.c �е�һЩ
	//-----------------------------------------------------------------------------
	long _crtAssertBusy = -1;

	int __cdecl _CrtSetReportMode( int nReportType, int nReportMode )
	{
		return g_pMemAlloc->CrtSetReportMode( nReportType, nReportMode );
	}

	_HFILE __cdecl _CrtSetReportFile( int nRptType, _HFILE hFile )
	{
		return (_HFILE)g_pMemAlloc->CrtSetReportFile( nRptType, hFile );
	}

	_CRT_REPORT_HOOK __cdecl _CrtSetReportHook( _CRT_REPORT_HOOK pfnNewHook )
	{
		return (_CRT_REPORT_HOOK)g_pMemAlloc->CrtSetReportHook( pfnNewHook );
	}

	int __cdecl _CrtDbgReport( int nRptType, const char * szFile,
			int nLine, const char * szModule, const char * szFormat, ... )
	{
		static char output[1024];
		va_list args;
		va_start( args, szFormat );
		vsprintf( output, szFormat, args );
		va_end( args );

		return g_pMemAlloc->CrtDbgReport( nRptType, szFile, nLine, szModule, output );
	}
	
	} // end extern "C"

#endif // _WIN32


// ����һ�����ݶ�����ʾ���ǵĵ���ģ��
#ifdef _WIN32

	#ifndef NDEBUG // _DEBUG
		#pragma data_seg("RisingDebug") 

		volatile const char* DBG = "*** DEBUG STUB ***";                     

	#endif

#endif
