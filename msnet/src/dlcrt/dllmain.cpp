// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "DownloadCentre.h"

USE_DEFAULT_DLL_MAIN;
BEGIN_CLIDMAP
	CLIDMAPENTRY_BEGIN
		CLIDMAPENTRY_PROGID(CLSID_DownLoadCentre ,CDownloadCentre , L"Prog.DownloadCentre.1")
	CLIDMAPENTRY_END
END_CLIDMAP_AND_EXPORTFUN;
