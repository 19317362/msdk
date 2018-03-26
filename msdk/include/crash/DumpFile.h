/*
 ----------------------------------------------------------------------------------
	DumpFile.h
	ģ�鹦�ܣ�����ת��ģ�飬Dump�ļ�Ĭ�ϱ�����%Temp%Ŀ¼��
 ----------------------------------------------------------------------------------
 */
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <windows.h>
#include <Dbghelp.h> 
#include <vector>
#include <time.h>
#include <strsafe.h>

#pragma comment(lib, "Dbghelp.lib")

// �Զ�����DUMP�ļ�����
#define DeclareDumpFile() NSDumpFile::RunCrashHandler();

namespace NSDumpFile
{
	// �������ܣ�����DUMP�ļ���д��������ݣ����浽����
	// �β��б�__in_opt LPCWSTR lpstrDumpFilePathName - ָ��DUMP�ļ����ƣ�����ָ��·�������δָ��·����Ĭ��Ϊ��ǰ����·��
	//           __in EXCEPTION_POINTERS *pException - �����쳣���ݽṹָ��
	// ����ֵ����
	void CreateDumpFile( __in_opt LPCTSTR lpstrDumpFilePathName, __in EXCEPTION_POINTERS *pException )  
	{  
		// ����Dump�ļ�
		HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
		
		// Dump��Ϣ
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;  
		dumpInfo.ExceptionPointers = pException;  
		dumpInfo.ThreadId = GetCurrentThreadId();  
		dumpInfo.ClientPointers = TRUE;

		// д��Dump�ļ�����
		MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL );
		CloseHandle(hDumpFile);  
	}  


	LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
	{
		return NULL;
	}


	BOOL PreventSetUnhandledExceptionFilter()
	{
		HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
		if ( hKernel32 == NULL )
			return FALSE;


		void *pOrgEntry = GetProcAddress( hKernel32, "SetUnhandledExceptionFilter" );
		if( pOrgEntry == NULL )
			return FALSE;


		unsigned char newJump[100];
		DWORD dwOrgEntryAddr = (DWORD) pOrgEntry;
		dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far


		void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
		DWORD dwNewEntryAddr = (DWORD) pNewFunc;
		DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;


		newJump[0] = 0xE9;  // JMP absolute
		memcpy( &newJump[1], &dwRelativeAddr, sizeof(pNewFunc) );
		SIZE_T bytesWritten;
		BOOL bRet = WriteProcessMemory( GetCurrentProcess(),  pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
		return bRet;
	}

	// �������ܣ��ص�������ʧ�ص��쳣�������ʱ����
	// �β��б�__in struct _EXCEPTION_POINTERS *pException - �쳣�������ݽṹ
	// ����ֵ�����ʽexcept()�ĺϷ��ķ���ֵ
	LONG WINAPI UnhandledExceptionFilterEx( __in struct _EXCEPTION_POINTERS *pException )
	{
		srand( (unsigned int)time(NULL) );

		TCHAR szMbsFile[MAX_PATH] = { 0 };
		TCHAR szFileName[MAX_PATH] = { 0 };

		GetModuleFileName(NULL, szMbsFile, MAX_PATH );
		LPTSTR pTail = _tcsrchr(szMbsFile, _T('\\'));
		if ( pTail )
			_tcscpy_s(szFileName, MAX_PATH, pTail + 1);

		pTail = _tcsrchr(szFileName, _T('.'));

		if ( pTail )
			pTail[0] = 0;

		pTail = _tcsrchr(szMbsFile, _T('\\'));
		if( pTail )
			pTail[0] = 0;


		// �����ļ�·�����ļ���
		SYSTEMTIME st;
		GetLocalTime( &st );
		TCHAR szTempStr[MAX_PATH] = {0};
		StringCchPrintf( szTempStr, MAX_PATH, _T("\\%s_%d%02d%02d_%d.dmp"), szFileName, st.wYear, st.wMonth, st.wDay, rand()%10000+1000 );
		StringCchCat( szMbsFile, MAX_PATH, szTempStr );
		CreateDumpFile( szMbsFile, pException );
		return EXCEPTION_CONTINUE_EXECUTION;
	}


	void RunCrashHandler()
	{
		// ȷ������ʧ�ص��쳣����ʱ���õĺ���ΪUnhandledExceptionFilterEx
		SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);
		PreventSetUnhandledExceptionFilter();
	}
};

