// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>
//#define FORCE_LOGGING 1

//#include "RsDebug.h"
//rscomʹ�ò���
#include <mscom/mscominc.h>
using namespace mscom;

#ifndef ASSERT
#include "assert.h"
#define ASSERT assert
#endif
#include "mstask\imstask.h"

#define RSXML_DETAIL	0
#define RSXML_ACTION	1
#define RSXML_WAINNING	2
#define RSXML_ALERT		3
#define RSXML_FATAL		4

#define RSLOG_DETAIL	RSXML_DETAIL
#define RSLOG_ACTION	RSXML_ACTION
#define RSLOG_WAINNING	RSXML_WAINNING
#define RSLOG_ALERT		RSXML_ALERT
#define RSLOG_FATAL		RSXML_FATAL

#define RSTASK_EVENT  _T("{38865E03-6FB8-4497-A141-3A8779C2CB68}")
extern void RSLOG(DWORD dwLevel,const TCHAR *format, ... );
extern HANDLE CreateCfgEvent();
extern BOOL GetProductType(TCHAR *szProduct,DWORD dwMax);

#include "traintool.h"
using namespace train;


// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
