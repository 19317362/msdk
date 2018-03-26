//
// ��Ŀ����: 
//
// ʵ�ֹ���: �����ڴ�й¶�Ĵ�����
//
// �ļ�����: memleak.h
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
//
//=============================================================================

#ifndef _MEMLEAK_INCLUDE_H_
#define _MEMLEAK_INCLUDE_H_
#ifdef _WIN32
#pragma once
#endif

#include <stdio.h>
#include "syslay/dbg.h"
#include "syslay/memalloc.h"
#include "syslay/platform.h"



#ifdef _DEBUG
#define BEGIN_RISING_ALLOCATION(x)	MS_pMemAlloc->PushAllocDbgInfo( x , __LINE__)
#define END_RISING_ALLOCATION()	MS_pMemAlloc->PopAllocDbgInfo()
#else
#define BEGIN_RISING_ALLOCATION()	0
#define END_RISING_ALLOCATION()	0
#endif

class CMemeoryLeakChecker
{
public:
	static char* TimeFormat(const char* pFormat, char* szOut, int nLen)
	{
		time_t t = time(NULL);
		struct tm* ptmTemp = localtime(&t);
		if (ptmTemp == NULL || !strftime(szOut, nLen, pFormat, ptmTemp))//_tcsftime
			return NULL;
		return szOut;
	}
	static void MyHeapReportFunc( unsigned char * pBuffer, int iLen )
	{		
		static const char* s_pCountHeader[NUM_BYTE_COUNT_BUCKETS] = 
		{
			"<=16 byte allocations", 
			"17-32 byte allocations",
			"33-128 byte allocations", 
			"129-1024 byte allocations",
			">1024 byte allocations",
		};

		FILE* m_f = fopen("c:\\memstats.txt", "a+w");		
		if(m_f)
		{
			char szTime[128];
			if(TimeFormat("%Y-%m-%d %H:%M:%S", szTime, sizeof(szTime)))
			{
				fprintf(m_f, "%s: ", szTime);
			}

			// ͳ�Ʊ���
			PCRT_MEM_HEAD pHead = (PCRT_MEM_HEAD)pBuffer;
			Assert(pHead->ReportType == ReportType_MemTotals);
			if(ReportType_MemTotals!=pHead->ReportType) return ;

			//��ӡ��������Ϣ
			fprintf(m_f, "\nAllocation type\t\tCurrent Size(k)\tPeak Size(k)\tTotal Allocations(k)\tOverhead Size(k)\tPeak Overhead Size(k)\tTime(ms)\tCurrent Count\tPeak Count\tTotal Count");
			for (int i = 0; i < NUM_BYTE_COUNT_BUCKETS; ++i)
			{
				fprintf(m_f,  "\t%s", s_pCountHeader[i] );
			}
			fprintf(m_f, "\n");

			//��ӡͳ����Ϣ
			for(int iPos=0;iPos<pHead->count;iPos++)
			{					
				CRT_MEM_ITEM& Item = pHead->item[iPos];
				fprintf(m_f, "%s, line %i\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%d\t%d\t%d\t%d",
					Item.m_szFilename,
					Item.m_nLine,
					Item.m_nCurrentSize / 1024.0f,
					Item.m_nPeakSize / 1024.0f,
					Item.m_nTotalSize / 1024.0f,
					Item.m_nOverheadSize / 1024.0f,
					Item.m_nPeakOverheadSize / 1024.0f,
					(int)(Item.m_nTime / 1000),
					Item.m_nCurrentCount,
					Item.m_nPeakCount,
					Item.m_nTotalCount
					);

				for (int i = 0; i < NUM_BYTE_COUNT_BUCKETS; ++i)
				{
					fprintf(m_f, "\t%d", Item.m_pCount[i] );
				}

				fprintf(m_f, "\n");
			}
			fclose(m_f);
		}
	}
	CMemeoryLeakChecker()
	{
		// �ҹ����ǵĴ������
		MS_pMemAlloc->DbgOutputFunc(CMemeoryLeakChecker::MyHeapReportFunc);
	}
	~CMemeoryLeakChecker()
	{
		DumpStatus();
	}

	static void DumpStatus()
	{
		MS_pMemAlloc->DumpStatus();
	}
};


#endif // _MEMLEAK_INCLUDE_H_
