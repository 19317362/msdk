#pragma once

#include "stdlog/stdlog.h"
#include "syslay/dbg.h"
#include "syslay/platform.h"

 #pragma comment(lib,"syslay.lib")
 #pragma comment(lib,"stdlog.lib")

#ifndef S_STD_LOG_INIT
#define S_STD_LOG_INIT
static class CDumpLib
{
public:
	CDumpLib()
	{
		//ͨ�õ���־�������
		InitDumpLib(0);
	}
	~CDumpLib()
	{
		TermDumpLib();
	}
}s_log_init;
#endif
