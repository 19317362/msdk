// mem.cpp
// ˵���������Ƚ��ȳ���ջʽ�Ķ��ڴ�

#include "mslog/Syslay/mem.h"
#include <malloc.h>
#include "mslog/Syslay/dbg.h"


enum 
{
	MAX_STACK_DEPTH = 32
};

static unsigned char *s_pBuf = NULL;
static int s_pBufStackDepth[MAX_STACK_DEPTH];
static int s_nBufDepth = -1;
static int s_nBufCurSize = 0;
static int s_nBufAllocSize = 0;


//-----------------------------------------------------------------------------
// Ŀ�� : �����ڴ�ʱ��С��1024 * 1024�İ�1024 * 1024��
// ���� : nMemSize - 
// ��� : void* - 
//-----------------------------------------------------------------------------
void* MemAllocStack( int nMemSize )
{	
	// Minimally allocate 1M stack
	if (s_nBufAllocSize < s_nBufCurSize + nMemSize)
	{
		s_nBufAllocSize = s_nBufCurSize + nMemSize;
		if (s_nBufAllocSize < 1024 * 1024)
		{
			s_nBufAllocSize = 1024 * 1024;
		}

		if (s_pBuf)
		{
#ifdef _WIN32
			if (s_nBufDepth < 0)
			{
				s_pBuf = (unsigned char*)realloc( s_pBuf, s_nBufAllocSize );
			}
			else
			{
				s_pBuf = (unsigned char*)_expand( s_pBuf, s_nBufAllocSize );
				Assert( s_pBuf );
			}
#elif _LINUX
			s_pBuf = (unsigned char*)realloc( s_pBuf, s_nBufAllocSize );
			Assert( s_pBuf );	
#endif
		}
		else
		{
			s_pBuf = (unsigned char*)malloc( s_nBufAllocSize );
		}
	}

	int nBase = s_nBufCurSize;
	s_nBufCurSize += nMemSize;
	++s_nBufDepth;
	Assert( s_nBufDepth < MAX_STACK_DEPTH );
	s_pBufStackDepth[s_nBufDepth] = nMemSize;

	return &s_pBuf[nBase];
}

//-----------------------------------------------------------------------------
// Ŀ�� : 
//-----------------------------------------------------------------------------
void MemFreeStack()
{
	Assert( s_nBufDepth >= 0 );
	s_nBufCurSize -= s_pBufStackDepth[s_nBufDepth];
	--s_nBufDepth;
}


#ifdef _LINUX
void ZeroMemory( void *mem, size_t length )
{
	memset( mem, 0x0, length );
}
#endif
