// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "sqliteconn.h"

USE_DEFAULT_DLL_MAIN;
BEGIN_CLIDMAP
	CLIDMAPENTRY_BEGIN
		CLIDMAPENTRY_NOROT_PROGID(CLSID_DbSqliteSafeDB, CSqliteConn,_T("DbSqliteSafeDB.1"))
	CLIDMAPENTRY_END
END_CLIDMAP_AND_EXPORTFUN