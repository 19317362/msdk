// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "signalapp.h"
USE_DEFAULT_DLL_MAIN;
BEGIN_CLIDMAP
	CLIDMAPENTRY_BEGIN
		CLIDMAPENTRY_PROGID(CLSID_SignalApp , CSignalAppliction , _T("SignalApp.1"))
	CLIDMAPENTRY_END
END_CLIDMAP
DEFINE_ALL_EXPORTFUN


