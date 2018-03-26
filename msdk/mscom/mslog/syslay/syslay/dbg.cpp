// dbg.cpp
// ����ģ���ʵ��

#ifdef _WIN32
#define WIN_32_LEAN_AND_MEAN
	#include <windows.h>	// IsBadReadPtr, IsBadWritePtr
	#pragma comment(lib,"user32.lib")	// For MessageBox
#endif
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "mslog/Syslay/dbg.h"
#include <math.h>
#include <atltrace.h>



//--------------------------------------------------------------------
// �ڲ�ʹ�õĽṹ
//-----------------------------------------------------------------------------
enum 
{ 
	MAX_GROUP_NAME_LENGTH = 48 
};

struct DbgGroup_t
{
	wchar_t	m_GroupName[MAX_GROUP_NAME_LENGTH];
	int		m_Level;
};


//-----------------------------------------------------------------------------
// ���ָ��ĺϷ���

DBG_INTERFACE void _AssertValidReadPtr( void* ptr, int count/* = 1*/ )
{
#ifdef _WIN32
	Assert(!IsBadReadPtr( ptr, count ));
#else
	Assert(ptr);
#endif

}

DBG_INTERFACE void _AssertValidWritePtr( void* ptr, int count/* = 1*/ )
{
#ifdef _WIN32
	Assert(!IsBadWritePtr( ptr, count ));
#else
	Assert(ptr);
#endif
}

DBG_INTERFACE void _AssertValidReadWritePtr( void* ptr, int count/* = 1*/ )
{
#ifdef _WIN32
	Assert(!( IsBadWritePtr(ptr, count) || IsBadReadPtr(ptr,count)));
#else
	Assert(ptr);
#endif
}

DBG_INTERFACE void AssertValidStringPtr( const char* ptr, int maxchar/* = 0xFFFFFF */ )
{
#ifdef _WIN32
	Assert(!IsBadStringPtrA( ptr, maxchar ));
#else
	Assert(ptr);
#endif
}


//-----------------------------------------------------------------------------
// Ĭ��ȫ��ʹ�õĻص����ܣ�
//		debug���԰�Ĭ�ϵĶ����� OutputDebugString �� printf
//		rlease��һ�ɷŹ�
//-----------------------------------------------------------------------------
DbgRetval_t DefaultDbgFuncA( DbgType_t type, char const *pMsg )
{
#ifdef _DEBUG
	#ifdef _WIN32
		OutputDebugStringA(pMsg);
	#endif
	printf_s( "%s", pMsg );

	if( type == DBG_ASSERT )//���Ϊ��������������
		return DBG_RETURNED_DEBUGGER;
	else if( type == DBG_ERROR )//����Ƿ��������˳�
		return DBG_RETURNED_ABORT;
	else //����������Լ�������
		return DBG_RETURNED_CONTINUES;
#else
	return DBG_RETURNED_CONTINUES;
#endif
}

//#include "locale.h"
DbgRetval_t DefaultDbgFuncW( DbgType_t type, wchar_t const *pMsg )
{
#ifdef _DEBUG
#ifdef _WIN32
	OutputDebugStringW(pMsg);
#endif
//	setlocale( LC_ALL, "chs" );
	wprintf_s( L"%s", pMsg );

	if( type == DBG_ASSERT )//���Ϊ��������������
		return DBG_RETURNED_DEBUGGER;
	else if( type == DBG_ERROR )//����Ƿ��������˳�
		return DBG_RETURNED_ABORT;
	else //����������Լ�������
		return DBG_RETURNED_CONTINUES;
#else
	return DBG_RETURNED_CONTINUES;
#endif
}

static DbgOutputFunc_tA   s_DbgOutputFuncA = DefaultDbgFuncA;
static DbgOutputFunc_tW   s_DbgOutputFuncW = DefaultDbgFuncW;

static char const*		s_pFileNameA;
static wchar_t const*	s_pFileNameW;
static int				s_Line;
static DbgType_t		s_DbgType;

static DbgGroup_t*		s_pDbgGroups = 0;
static int				s_GroupCount = 0;
static int				s_DefaultLevel = 9;

static struct _FREE_s_pDbgGroups
{
	_FREE_s_pDbgGroups()
	{

	}
	~_FREE_s_pDbgGroups()
	{
		if(s_pDbgGroups)
		{
			free(s_pDbgGroups);
			s_pDbgGroups = NULL;
		}
	}

} ____FREE_s_pDbgGroups;


//-----------------------------------------------------------------------------
// ����������������
//-----------------------------------------------------------------------------

void  DbgOutputFuncA( DbgOutputFunc_tA func )
{
	s_DbgOutputFuncA = func ? func : DefaultDbgFuncA;
}

DbgOutputFunc_tA GetDbgOutputFuncA( void )
{
	if( s_DbgOutputFuncA )
	{
		return s_DbgOutputFuncA;
	}
	else
	{
		return DefaultDbgFuncA;
	}
}

void  DbgOutputFuncW( DbgOutputFunc_tW func )
{
	s_DbgOutputFuncW = func ? func : DefaultDbgFuncW;
}

DbgOutputFunc_tW GetDbgOutputFuncW( void )
{
	if( s_DbgOutputFuncW )
	{
		return s_DbgOutputFuncW;
	}
	else
	{
		return DefaultDbgFuncW;
	}
}

//-----------------------------------------------------------------------------
// ���ֵ��Թ���
//-----------------------------------------------------------------------------

void  _DbgInfoA( DbgType_t type, char const* pFile, int line )
{
	// Only grab the file name. Ignore the path.
	char const* pSlash = strrchr( pFile, '\\' );
	char const* pSlash2 = strrchr( pFile, '/' );
	if (pSlash < pSlash2) pSlash = pSlash2;
	
	s_pFileNameA = pSlash ? pSlash + 1: pFile;
	s_Line = line;
	s_DbgType = type;
}

void  _DbgInfoW( DbgType_t type, wchar_t const* pFile, int line )
{
	// Only grab the file name. Ignore the path.
	wchar_t const* pSlash = wcsrchr( pFile, L'\\' );
	wchar_t const* pSlash2 = wcsrchr( pFile, L'/' );
	if (pSlash < pSlash2) pSlash = pSlash2;

	s_pFileNameW = pSlash ? pSlash + 1: pFile;
	s_Line = line;
	s_DbgType = type;
}

DbgRetval_t  _DbgMessageA( DbgType_t DebugType, char const* pMsgFormat, va_list args , const char*szGrpName)
{
	DbgRetval_t ret = DBG_RETURNED_DEBUGGER;

	char pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX];

	// ǰ׺�ļ������к�
	int len = 0;
	if ((DebugType == DBG_ASSERT) )
	{
		len = sprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, "%s (%d) : ", s_pFileNameA, s_Line );
	}

	if (szGrpName)
	{
		len += sprintf_s(pTempBuffer +len , DEBUG_MESSAGE_BUFLEN_MAX-len , " [%s] " ,szGrpName);
	}
	// ��Ϣ��
	len += vsprintf_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, pMsgFormat, args );

	// ��һ���س�����
	if ((DebugType == DBG_ASSERT) )
	{
		strcat_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, "\n" ); 
		len++;
	}
	
	// buffer������
	assert( len < DEBUG_MESSAGE_BUFLEN_MAX );
	assert( s_DbgOutputFuncA );
	
	// ���ݻص������ķ��ؽ�����д������仰˵�������ܿ������Զ��庯���з����Լ���ֵ��
	// �Դ����ı�����ĳ�������̣�
	ret = s_DbgOutputFuncA( DebugType, pTempBuffer );
	switch (ret)
	{
	case DBG_RETURNED_DEBUGGER:
		break;
		
	case DBG_RETURNED_ABORT:
#ifdef _WIN32
		//MessageBoxA(NULL,pTempBuffer,"Error",MB_OK);
		//ExitProcess(atoi(pTempBuffer));
#else
		printf(STD_ERR,pTempBuffer);
		exit(0);
#endif
		break;
	}
	return ret;
}

DbgRetval_t  _DbgMessageW( DbgType_t DebugType, wchar_t const* pMsgFormat, va_list args, const wchar_t* szGrpName /*=NULL*/)
{
	DbgRetval_t ret = DBG_RETURNED_DEBUGGER;

	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX];

	// ǰ׺�ļ������к�
	int len = 0;
	if ((DebugType == DBG_ASSERT) )
	{
		len = swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"%s (%d) : ", s_pFileNameW, s_Line );
	}
	
	if (szGrpName)
	{
		len += swprintf_s(pTempBuffer +len , DEBUG_MESSAGE_BUFLEN_MAX-len , L" [%s] " ,szGrpName);
	}
	
	// ��Ϣ��
	len += vswprintf_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, pMsgFormat, args );

	// ��һ���س�����
	if ((DebugType == DBG_ASSERT) )
	{
		wcscat_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, L"\n" ); 
		len++; 
	}

	// buffer������
	assert( len < DEBUG_MESSAGE_BUFLEN_MAX );
	assert( s_DbgOutputFuncW );

	// ���ݻص������ķ��ؽ�����д������仰˵�������ܿ������Զ��庯���з����Լ���ֵ��
	// �Դ����ı�����ĳ�������̣�
	ret = s_DbgOutputFuncW( DebugType, pTempBuffer );
	switch (ret)
	{
	case DBG_RETURNED_DEBUGGER:
		break;

	case DBG_RETURNED_ABORT:
#ifdef _WIN32
		//MessageBoxW(NULL,pTempBuffer,L"Error",MB_OK);
		//ExitProcess(_wtoi(pTempBuffer));
#else
		printf(STD_ERR,pTempBuffer);
		exit(0);
#endif
		break;
	}

	return ret;
}

DbgRetval_t  _DbgMessageMoudleA( char const* pMoudleName, DbgType_t DebugType, char const* pMsgFormat, va_list args )
{
	char pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX];
	
	int len = 0;	
	if(pMoudleName && strlen(pMoudleName)>0)
	{
		len = sprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, "[%s] ", pMoudleName );
	}

	// ǰ׺�ļ������к�
	if ((DebugType == DBG_ASSERT) )
	{
		len += sprintf_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, "%s (%d) : ", s_pFileNameA, s_Line );
	}
	
	// ��Ϣ��
	len += vsprintf_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, pMsgFormat, args );

	// ��һ���س�����
	if ((DebugType == DBG_ASSERT) )
	{
		strcat_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, "\n" ); 
		len++;
	}
	
	// buffer������
	assert( len < DEBUG_MESSAGE_BUFLEN_MAX );
	assert( s_DbgOutputFuncA );
	
	// ���ݻص������ķ��ؽ�����д������仰˵�������ܿ������Զ��庯���з����Լ���ֵ��
	// �Դ����ı�����ĳ�������̣�
	DbgRetval_t ret = s_DbgOutputFuncA( DebugType, pTempBuffer );
	switch (ret)
	{
	case DBG_RETURNED_DEBUGGER:
		break;
		
	case DBG_RETURNED_ABORT:
		break;
#ifdef _WIN32
		//MessageBoxA(NULL,pTempBuffer,"Error",MB_OK);
		//ExitProcess(atoi(pTempBuffer));
#else
		printf(STD_ERR,pTempBuffer);
		exit(0);
#endif
	}

	return ret;
}

DbgRetval_t  _DbgMessageMoudleW( wchar_t const* pMoudleName, DbgType_t DebugType, wchar_t const* pMsgFormat, va_list args )
{
	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX];

	int len = 0;	
	if(pMoudleName && wcslen(pMoudleName)>0)
	{
		len = swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"[%s] ", pMoudleName );
	}

	len += swprintf_s(pTempBuffer +len , DEBUG_MESSAGE_BUFLEN_MAX-len , L" [%s] " ,L"AAAAAAA");
	// ǰ׺�ļ������к�
	if ((DebugType == DBG_ASSERT) )
	{
		len += swprintf_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, L"%s (%d) : ", s_pFileNameW, s_Line );
	}

	// ��Ϣ��
	len += vswprintf_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, pMsgFormat, args );

	// ��һ���س�����
	if ((DebugType == DBG_ASSERT) )
	{
		wcscat_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, L"\n" ); 
		len++;
	}

	// buffer������
	assert( len < DEBUG_MESSAGE_BUFLEN_MAX );
	assert( s_DbgOutputFuncW );

	// ���ݻص������ķ��ؽ�����д������仰˵�������ܿ������Զ��庯���з����Լ���ֵ��
	// �Դ����ı�����ĳ�������̣�
	DbgRetval_t ret = s_DbgOutputFuncW( DebugType, pTempBuffer );
	switch (ret)
	{
	case DBG_RETURNED_DEBUGGER:
		break;

	case DBG_RETURNED_ABORT:
		break;
#ifdef _WIN32
		//MessageBoxW(NULL,pTempBuffer,L"Error",MB_OK);
		//ExitProcess(_wtoi(pTempBuffer));
#else
		printf(STD_ERR,pTempBuffer);
		exit(0);
#endif
	}

	return ret;
}

DbgRetval_t  _DbgMessageA( char const* pMsgFormat, ... )
{
	va_list args;
	va_start( args, pMsgFormat );
	DbgRetval_t ret = _DbgMessageA( s_DbgType, pMsgFormat, args );
	va_end(args);
	return ret;
}

DbgRetval_t  _DbgMessageW( wchar_t const* pMsgFormat, ... )
{
	va_list args;
	va_start( args, pMsgFormat );
	DbgRetval_t ret = _DbgMessageW( s_DbgType, pMsgFormat, args ,NULL);
	va_end(args);
	return ret;
}

DbgRetval_t _DDbgMessageA( char const *pGroupName, int level, char const* pMsgFormat, ... )
{
	if( !IsDbgActiveA( pGroupName, level ) )
		return DBG_RETURNED_CONTINUES;

	va_list args;
	va_start( args, pMsgFormat );
	DbgRetval_t ret = _DbgMessageA( s_DbgType, pMsgFormat, args ,pGroupName);
	va_end(args);
	return ret;
}

DbgRetval_t _DDbgMessageW( wchar_t const *pGroupName, int level, wchar_t const* pMsgFormat, ... )
{
	if( !IsDbgActiveW( pGroupName, level ) )
		return DBG_RETURNED_CONTINUES;

	va_list args;
	va_start( args, pMsgFormat );
	DbgRetval_t ret = _DbgMessageW( s_DbgType, pMsgFormat, args ,pGroupName);
	va_end(args);
	return ret;
}

void MsgA( char const* pMsgFormat, ... )
{
	char pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	sprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, "[MESSAGE] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageA( DBG_MESSAGE, pMsgFormat, args );
	va_end(args);
}

void MsgW( wchar_t const* pMsgFormat, ... )
{
	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"[MESSAGE] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageW( DBG_MESSAGE, pMsgFormat, args ,NULL);
	va_end(args);
}

void GrpMsgA( char const *pGroupName, int level, char const *pMsgFormat, ... )
{
	if( !IsDbgActiveA( pGroupName, level ) )
		return;

	char pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	sprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, "[MESSAGE] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	
	_DbgMessageA( DBG_MESSAGE, pMsgFormat, args ,pGroupName);
	va_end(args);
}

void GrpMsgW( wchar_t const *pGroupName, int level, wchar_t const *pMsgFormat, ... )
{
	if( !IsDbgActiveW( pGroupName, level ) )
		return;

	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"[MESSAGE] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageW( DBG_MESSAGE, pTempBuffer, args ,pGroupName);
	va_end(args);
}

void WarningA( char const *pMsgFormat, ... )
{
	char pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	sprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, "[WARNING] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageA( DBG_WARNING, pMsgFormat, args );
	va_end(args);
}

void WarningW( wchar_t const *pMsgFormat, ... )
{
	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"[WARNING] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageW( DBG_WARNING, pMsgFormat, args );
	va_end(args);
}

void GrpWarningA( char const *pGroupName, int level, char const *pMsgFormat, ... )
{
	if( !IsDbgActiveA( pGroupName, level ) )
		return;

	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"[WARNING] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageA( DBG_WARNING, pMsgFormat, args , pGroupName);
	va_end(args);
}

void GrpWarningW( wchar_t const *pGroupName, int level, wchar_t const *pMsgFormat, ... )
{
	if( !IsDbgActiveW( pGroupName, level ) )
		return;

	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"[WARNING] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageW( DBG_WARNING, pTempBuffer, args ,pGroupName);
	va_end(args);
}

void LogA( char const *pMsgFormat, ... )
{
	char pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	sprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, "[LOG   ] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageA( DBG_LOG, pTempBuffer, args );
	va_end(args);
}

void LogW( wchar_t const *pMsgFormat, ... )
{
	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"[LOG   ] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageW( DBG_LOG, pTempBuffer, args );
	va_end(args);
}

void GrpLogA( char const *pGroupName, int level, char const *pMsgFormat, ... )
{
	if( !IsDbgActiveA( pGroupName, level ) )
		return;

	char pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	sprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, "[LOG    ] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageA( DBG_LOG, pTempBuffer, args ,pGroupName);
	va_end(args);
}

void GrpLogW( wchar_t const *pGroupName, int level, wchar_t const *pMsgFormat, ... )
{
	if( !IsDbgActiveW( pGroupName, level ) )
		return;

	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"[LOG    ] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageW( DBG_LOG, pTempBuffer, args ,pGroupName);
	va_end(args);
}

void ErrorA( char const *pMsgFormat, ... )
{
	char pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	sprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, "[ERROR ] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageA( DBG_ERROR, pTempBuffer, args );
	va_end(args);
}

void ErrorW( wchar_t const *pMsgFormat, ... )
{
	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"[ERROR  ] %s", pMsgFormat);

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageW( DBG_ERROR, pMsgFormat, args );
	va_end(args);
}

//-----------------------------------------------------------------------------
// Ŀ�� : ע��!!! ����������Ҫ�������������Ǹ����Ƿ�ּ���
// ���� : *pGroupName - 
//		  level - 
//		  *pMsgFormat - 
//		  ... - 
//-----------------------------------------------------------------------------
void GrpErrorA( char const *pGroupName, int level, char const *pMsgFormat, ... )
{
// 	if( !IsDbgActive( pGroupName, level ) )
// 		return;
	char pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	sprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, "[ERROR  ] %s", pMsgFormat);
	

	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageA( DBG_ERROR, pTempBuffer, args ,pGroupName);
	va_end(args);
}

void GrpErrorW( wchar_t const *pGroupName, int level, wchar_t const *pMsgFormat, ... )
{
// 	if( !IsDbgActive( pGroupName, level ) )
// 			return;
	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX] = { 0 };
	swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"[ERROR  ] %s", pMsgFormat);


	va_list args;
	va_start( args, pMsgFormat );
	_DbgMessageW( DBG_ERROR, pTempBuffer, args ,pGroupName);
	va_end(args);
}

void ReportA( unsigned char * pBuffer, int iLen )
{
	char pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX];

	int len = sprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, "bin:" );

	// ��Ϣ�����ֻ������ǰ1K�ֽڵ�����
	for(int i = 0; i<iLen&&len<1020; i++)
	{
		len += sprintf_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, "%02X", *((char *)pBuffer+i));
	}
	// buffer������
	assert( len < DEBUG_MESSAGE_BUFLEN_MAX );
	assert( s_DbgOutputFuncA );
		
	DbgRetval_t ret = s_DbgOutputFuncA( DBG_REPORT, (const char *)pBuffer );
	switch (ret)
	{
	case DBG_RETURNED_DEBUGGER:
		DebuggerBreak();
		break;
		
	case DBG_RETURNED_ABORT:
		{			
#ifdef _WIN32
			//MessageBoxA(NULL,pTempBuffer,"Error",MB_OK);
#endif
			//exit(0);
		}
	}
}

void ReportW( unsigned char * pBuffer, int iLen )
{
	wchar_t pTempBuffer[DEBUG_MESSAGE_BUFLEN_MAX];
	
	int len = swprintf_s( pTempBuffer, DEBUG_MESSAGE_BUFLEN_MAX, L"bin:" );

	// ��Ϣ�����ֻ������ǰ1K�ֽڵ�����
	for(int i = 0; i<iLen&&len<1020; i++)
	{
		len += swprintf_s( pTempBuffer+len, DEBUG_MESSAGE_BUFLEN_MAX-len, L"%02X", *((char *)pBuffer+i));
	}
	// buffer������
	assert( len < DEBUG_MESSAGE_BUFLEN_MAX );
	assert( s_DbgOutputFuncW );

	DbgRetval_t ret = s_DbgOutputFuncW( DBG_REPORT, (const wchar_t *)pBuffer );
	switch (ret)
	{
	case DBG_RETURNED_DEBUGGER:
		DebuggerBreak();
		break;

	case DBG_RETURNED_ABORT:
		{			
#ifdef _WIN32
			//MessageBoxW(NULL,pTempBuffer,L"Error",MB_OK);
#endif
			//exit(0);
		}
	}
}

void GrpReportA( char const *pGroupName, int level, unsigned char * pBuffer, int iLen )
{
	if( !IsDbgActiveA( pGroupName, level ) )
		return;

	ReportA(pBuffer, iLen);
}

void GrpReportW( wchar_t const *pGroupName, int level, unsigned char * pBuffer, int iLen )
{
	if( !IsDbgActiveW( pGroupName, level ) )
		return;

	ReportW(pBuffer, iLen);
}


#ifdef _WIN32
#include <atlconv.h>
#endif
//-----------------------------------------------------------------------------
// ���ҵ����飬���������
//-----------------------------------------------------------------------------
bool FindDbgGroupW( wchar_t const* pGroupName, int* pInd )
{
	int s = 0;
	if (s_GroupCount)
	{
		int e = (int)(s_GroupCount - 1);
		while ( s <= e )
		{
			int m = (s+e) >> 1;
			int cmp = _wcsicmp( pGroupName, s_pDbgGroups[m].m_GroupName );
			if ( !cmp )
			{
				*pInd = m;
				return true;
			}
			if ( cmp < 0 )
				e = m - 1;
			else
				s = m + 1;
		}
	}
	*pInd = s;
	return false;
}

bool FindDbgGroupA( char const* pGroupName, int* pInd )
{
	//Ӧ��ǿ�ƶ�ת��δunicode�汾����!
#ifdef _WIN32 
	USES_CONVERSION;
	wchar_t const* pGroupNameW = A2W(pGroupName);
	return FindDbgGroupW(pGroupNameW, pInd);
#endif
	return false;
}
//-----------------------------------------------------------------------------
// Ŀ�� : ���õ�������Լ���
// ���� : pGroupName - ����
//		  level - ���� 
//-----------------------------------------------------------------------------
void DbgActivateW( wchar_t const* pGroupName, int level )
{
	Assert( pGroupName );
	
	// ���Ϊ'*'�ţ�����������Ĭ�ϼ���
	if ((pGroupName[0] == L'*') && (pGroupName[1] == L'\0'))
	{
		s_DefaultLevel = level;
		return;
	}
	
	// �������б���
	int ind;
	if ( !FindDbgGroupW( pGroupName, &ind ) )
	{
		// ���û�������һ��
		++s_GroupCount;
		if (s_pDbgGroups)
		{
			s_pDbgGroups = (DbgGroup_t*)realloc( s_pDbgGroups, 
				s_GroupCount * sizeof(DbgGroup_t) );
						
			int numToMove = s_GroupCount - ind - 1;
			memmove( &s_pDbgGroups[ind+1], &s_pDbgGroups[ind], 
				numToMove * sizeof(DbgGroup_t) );
		}
		else
			s_pDbgGroups = (DbgGroup_t*)malloc( s_GroupCount * sizeof(DbgGroup_t) ); 
		
		Assert( wcslen( pGroupName ) < MAX_GROUP_NAME_LENGTH );
		wcscpy_s( s_pDbgGroups[ind].m_GroupName, MAX_GROUP_NAME_LENGTH, pGroupName );
	}
	s_pDbgGroups[ind].m_Level = level;
}

void DbgActivateA( char const* pGroupName, int level )
{
	//Ӧ��ǿ�ƶ�ת��δunicode�汾����!
#ifdef _WIN32 
	USES_CONVERSION;
	wchar_t const* pGroupNameW = A2W(pGroupName);
	DbgActivateW(pGroupNameW, level);
#endif
}
//-----------------------------------------------------------------------------
// ���ݼ��𣬾����Ƿ���Ҫ���
//-----------------------------------------------------------------------------
bool IsDbgActiveA( char const* pGroupName, int level )
{
	// ����Ҳ��������飬��ʹ��Ĭ���������
	int ind;
	if ( FindDbgGroupA( pGroupName, &ind ) )
		return s_pDbgGroups[ind].m_Level >= level;
	else
		return s_DefaultLevel >= level;
}

bool IsDbgActiveW( wchar_t const* pGroupName, int level )
{
	// ����Ҳ��������飬��ʹ��Ĭ���������
	int ind;
	if ( FindDbgGroupW( pGroupName, &ind ) )
		return s_pDbgGroups[ind].m_Level >= level;
	else
		return s_DefaultLevel >= level;
}
