// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

#include "mscom/mscominc.h"
#include "msapi/mswinapi.h"
#include "util/msmap.h"
using namespace mscom;


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include <atlstr.h>

#define GroupName _T("luahost")
#define GroupNameA "luahost"

#define RSLOG_DETAIL GroupName

static void GrpErrorW_N( wchar_t const *pGroupName, int level, wchar_t const *pMsg, ... )
{

}
//#ifdef _DEBUG
	#define RSLOG GrpError
// #else
// 	#define RSLOG GrpErrorW_N  
// #endif // _DEBUG


#define GRP_NAME _T("luahost")
#define LOG_OUTPUT_DIR _T("C:\\") //����־�����C:\��
#define LOG_FILE_NAME _T("luahost.log")

#define BOOL2bool( X )			(( X )==TRUE?true:false)
#define bool2BOOL( X )			(( X )== true ? TRUE : FALSE)


#define RSLOG_ALERT GroupName

#define  GroupNameA "luahost"

#include "mslog/stdlog_dll.h"


#ifndef tstring
#if UNICODE
#	define tstring std::wstring
#else
#	define tstring std::string
#endif
#endif
//#include "luahost/ILuaHost.h"
//#include "luahost/luahost.h"