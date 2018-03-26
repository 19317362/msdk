// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

#include <atlstr.h>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include "mscom/mscominc.h"

#include "use_inetfile.h"


#define GroupName _T("downloadmgr")
#include "mslog/stdlog_dll.h"

static CString FormatUINT64(UINT64 uvalue)
{
	CString strRet;
	_i64tot_s(uvalue,strRet.GetBufferSetLength(MAX_PATH),MAX_PATH,10);
	strRet.ReleaseBuffer();
	return strRet;
}

static FILETIME UINT64ToFileTime(const UINT64& val64)
{
	FILETIME time = {0};
	LARGE_INTEGER l;
	l.QuadPart = val64;

	time.dwHighDateTime = l.HighPart;
	time.dwLowDateTime =  l.LowPart;
	return time;
}
static CString FormatFileTimeToStr(UINT64 uFileTime ,BOOL bYears = TRUE)
{
	FILETIME fileTime = UINT64ToFileTime(uFileTime);
	SYSTEMTIME sysTime;
	FileTimeToSystemTime(&fileTime,&sysTime);

	TCHAR str[100] = {0};
	if (bYears)
	{
		_stprintf(str,_T("%4d-%02d-%02d %2d:%2d:%2d"), 
			sysTime.wYear,  
			sysTime.wMonth, 
			sysTime.wDay, 
			sysTime.wHour, 
			sysTime.wMinute,
			sysTime.wSecond);//��ʱ���ʽ�� 
	}

	else
	{
		_stprintf(str,_T("%2d:%2d:%2d"), 
			sysTime.wHour, 
			sysTime.wMinute,
			sysTime.wSecond);//��ʱ���ʽ�� 
	}

	CString strTime = str;
	return strTime;
}

static UINT64 FileTimeToUINT64(const FILETIME& time)
{
	LARGE_INTEGER l;
	l.LowPart = time.dwLowDateTime;
	l.HighPart = time.dwHighDateTime;
	return l.QuadPart;
}


static UINT64 GetCurrentFileTime()
{
	FILETIME   createFileTime = {0};
	SYSTEMTIME createSysTime =  {0};
	GetLocalTime(&createSysTime);
	SystemTimeToFileTime(&createSysTime,&createFileTime);
	return  FileTimeToUINT64(createFileTime);
}

static FILETIME StrToFileTime(LPTSTR lpszTime,BOOL bYears = TRUE)
{
	SYSTEMTIME sysTime = {0};
	if(bYears)
	{
		_stscanf_s(lpszTime, _T("%4d-%02d-%02d %2d:%2d:%2d"), &sysTime.wYear,  
			&sysTime.wMonth, 
			&sysTime.wDay, 
			&sysTime.wHour, 
			&sysTime.wMinute,
			&sysTime.wSecond);
	}
	else
	{
		_stscanf_s(lpszTime, _T("%2d:%2d:%2d"), 
			&sysTime.wHour, 
			&sysTime.wMinute,
			&sysTime.wSecond);//��ʱ���ʽ�� 
	}
	
	FILETIME fileTime = {0};
	 SystemTimeToFileTime(&sysTime, &fileTime);
	 return fileTime;
}
