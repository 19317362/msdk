// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "NetBaseProxy.h"

USE_DEFAULT_DLL_MAIN
BEGIN_CLIDMAP
	CLIDMAPENTRY_BEGIN
 		CLIDMAPENTRY(CLSID_NetBaseProxy,CNetBaseProxy)
	CLIDMAPENTRY_END
	END_CLIDMAP
DEFINE_ALL_EXPORTFUN
