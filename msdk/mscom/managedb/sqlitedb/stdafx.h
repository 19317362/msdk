// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

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

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include "mscom/mscominc.h"
using namespace mscom;

#include <map>
#include "mscom\tproperty.h"
#include "mscom\prophelpers.h"

// LPSTR MyW2A( LPCWSTR szW );
// LPWSTR MyA2W( LPCSTR szA );

#define MODULE_NAME L"sqlitedb"
#define DEFAULT_LEV	2

#include "mslog/stdlog_dll.h"
