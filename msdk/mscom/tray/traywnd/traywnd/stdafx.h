// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//#define  _WTL_NO_CSTRING		//��ʹ��WTL��CString
#include <atlbase.h>
//#include <atlstr.h>	//ʹ��ATL��CString ���һ��Ҫ��base��appǰ
//#include <atlapp.h>

//mscomʹ�ò���
#include <mscom/mscominc.h>

using namespace mscom;


#define GroupName L"traywnd"


#define CFG_FILE_NAME _T("traywnd_log.ini")
#define LOG_FILE_NAME _T("traywnd_log.log")


#include "mslog/logtool/logtool.h"