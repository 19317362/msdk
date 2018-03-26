// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>


#include "mscom\mscominc.h"


#include <map>
#include "mscom\tproperty.h"
#include "mscom\prophelpers.h"

using namespace mscom;

#include <assert.h>
#include <string>
#include <tchar.h>
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�


#ifndef tstring
#ifdef _UNICODE
#define tstring std::wstring
#else	tstring std::string
#endif
#endif

#ifndef tostringstream
#ifdef _UNICODE
#define tostringstream wostringstream
#else	tostringstream ostringstream
#endif
#endif


#define WXSQLITE3S_HAVE_METADATA TRUE
#define WXSQLITE3_HAVE_CODEC	 TRUE
#define MSQLITES_VERSION_NUMBER  3008004

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_context sqlite3_context;
typedef struct sqlite3_stmt sqlite3_stmt;
typedef __int64 sqlite_int64;
typedef sqlite_int64 sqlite3_int64;
typedef struct Mem sqlite3_value;
typedef struct sqlite3_backup sqlite3_backup;
typedef struct sqlite3_blob sqlite3_blob;

typedef int (*sqlite3_callback)(void*,int,char**, char**);




// LPSTR MyW2A( LPCWSTR szW );
// LPWSTR MyA2W( LPCSTR szA );

#define MODULE_NAME L"sqlitesdb"
#define DEFAULT_LEV	2

#include "mslog/stdlog_dll.h"
