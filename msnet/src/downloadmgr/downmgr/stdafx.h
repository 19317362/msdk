// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <atlbase.h>
#include <atlstr.h>
#include <mscom/mscominc.h>
#include "criticalsection.h"
using namespace msdk;
#include "download/downloadmgr_i.h"

#define GroupName _T("downmgr")

#include <string>
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include "base64/sha1ex.h"
#include "use_inetfile.h"
#include "util/mspath.h"


/*
static LPCTSTR hash2sting(const sha1_hash& hash , TCHAR lpBuf[MAX_PATH])
{
	std::string encode = BASE64::Base64Encode(hash.hash,sizeof(hash.hash));
	USES_CONVERSION;
	TCHAR* p = A2W(encode.c_str());
	_tcscpy_s(lpBuf, MAX_PATH, p);
	return lpBuf;
}

static sha1_hash string2hash(TCHAR lpBuf[MAX_PATH])
{
	USES_CONVERSION;
	std::string str = W2A(lpBuf);
	std::string src =BASE64::Base64Decode(str);
	
	sha1_hash hash;
	CopyMemory(hash.hash,src.c_str(),sizeof(hash.hash));
	return hash;
}
*/
static sha1_hash calc_hash(LPCTSTR lpUrl)
{
	RASSERT(lpUrl , NullUrlHash);
	USES_CONVERSION;

	CHAR* strUrlA = W2A(lpUrl);
	hasher hash(strUrlA ,strlen(strUrlA));
	return hash.final();
}

static LPTSTR GetLastErrorText( LPTSTR lpszBuf, DWORD dwSize )
{
	LPTSTR lpszTemp = 0;
	DWORD dwRet = ::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
		0,
		GetLastError(),
		LANG_NEUTRAL, //Ĭ�ϵ���������
		(LPTSTR)&lpszTemp,
		0,
		0
		);

	if( !dwRet || (dwSize < dwRet+14) )
		lpszBuf[0] = TEXT('\0');
	else {
		lpszTemp[_tcsclen(lpszTemp)-2] = TEXT('\0');  //remove cr/nl characters
		_tcscpy_s(lpszBuf,dwSize, lpszTemp);
	}

	if( lpszTemp )
		LocalFree(HLOCAL(lpszTemp));

	return lpszBuf;
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

