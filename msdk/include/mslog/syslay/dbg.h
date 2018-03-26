//
// ��Ŀ����:
//
// ʵ�ֹ���: ��Ϊ����ģ�����������ͷ�ļ�
//
// �ļ�����: dbg.h
//
// ��������:
//
// ��������:
//
// �޸�����:
//
// �޸�����:
//
// �������µ��޸�������������ӣ�
//
// ʹ��˵��:
// 1. һ�����Ϣ��ӡ.
//
//     Assert( (f == 5) );
//     AssertMsg( (f == 5), ("F needs to be %d here!\n", 5) );
//     AssertFunc( (f == 5), BadFunc() );
//     AssertEquals( f, 5 );
//     AssertFloatEquals( f, 5.0f, 1e-3 );
//
//     Warning("Oh I feel so %s all over\n", "yummy");
//
//	   Msg( "Isn't this exciting %d?", 5 );
//
//	   Error( "I'm just thrilled" );
//
//     GrpWarning( "group", level, "Oh I feel even yummier!\n" );
//
//     DbgOutputFunc( OutputFunc );
//
//
// 2. ֻ������DEBUG���µĴ���
//
//   DBG_CODE(
//				{
//					int x = 5;
//					++x;
//				}
//           );
//
//
//   DBG_GRPCODE( "group", level,
//              { int x = 5; ++x; }
//            );
//
// 3. �쳣����������Ķϵ�
//
//   DBG_BREAK();
//
//	 DebuggerBreak();
//=============================================================================


#ifndef _DBG_INCLUDE_H_
#define _DBG_INCLUDE_H_
#ifdef _WIN32
#pragma once
#endif

#include "mslog/Syslay/platform.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>


#ifdef _WIN32
	// ����ע�ͣ� ���磺#pragma reminder(fix it later)
	#define chSTR2(x)		#x
	#define chSTR(x)		chSTR2(x)
	#define reminder(desc)	message(__FILE__ "(" chSTR(__LINE__) "):" #desc)

	// �ļ����Ŀ��ַ�����
	#define WIDEN2(x) L ## x
	#define WIDEN(x) WIDEN2(x)
	#define __WFILE__ WIDEN(__FILE__)

	#if defined(_UNICODE) || defined(UNICODE)
		#define __TFILE__	__WFILE__
	#else
		#define __TFILE__	__FILE__
	#endif

#endif

//-----------------------------------------------------------------------------
// �����Ķ���
//-----------------------------------------------------------------------------

#ifdef SYSLAY_DLL_EXPORT
	#define DBG_INTERFACE	DLL_EXPORT
	#define DBG_OVERLOAD	DLL_GLOBAL_EXPORT
	#define DBG_CLASS		DLL_CLASS_EXPORT
#else
	#define DBG_INTERFACE	DLL_IMPORT
	#define DBG_OVERLOAD	DLL_GLOBAL_IMPORT
	#define DBG_CLASS		DLL_CLASS_IMPORT
#endif


#define DEBUG_MESSAGE_BUFLEN_MAX		2048
//-----------------------------------------------------------------------------
// ������Ϣ������
enum DbgType_t
{
	DBG_MESSAGE = 0,
	DBG_WARNING,
	DBG_ASSERT,
	DBG_ERROR,
	DBG_LOG,
	DBG_REPORT,

	DBG_TYPE_COUNT
};

enum Msg_Level
{
	MsgLevel_Error		=0,	//1
	MsgLevel_Warning	=1,	//3
	MsgLevel_Msg		=2,	//5	
	MsgLevel_Log		=3,	//5	
	MsgLevel_Notify		=4	//10
};

enum DbgRetval_t
{
	DBG_RETURNED_DEBUGGER = 0,
	DBG_RETURNED_CONTINUES,
	DBG_RETURNED_ABORT
};

// ��ӡ������Ϣ�ĺ���ԭ��
typedef DbgRetval_t (*DbgOutputFunc_tA)( DbgType_t spewType, char const *pMsg );
typedef DbgRetval_t (*DbgOutputFunc_tW)( DbgType_t spewType, wchar_t const *pMsg );

// �ض����µĴ�ӡ������Ϣ�ĺ���
DBG_INTERFACE void   DbgOutputFuncA( DbgOutputFunc_tA func );
DBG_INTERFACE void   DbgOutputFuncW( DbgOutputFunc_tW func );

// ��ȡ�ϵĺ���
DBG_INTERFACE DbgOutputFunc_tA GetDbgOutputFuncA( void );
DBG_INTERFACE DbgOutputFunc_tW GetDbgOutputFuncW( void );

#if defined(_UNICODE) || defined(UNICODE)

#define DbgOutputFunc_t				DbgOutputFunc_tW
#define DbgOutputFunc				DbgOutputFuncW
#define GetDbgOutputFunc			GetDbgOutputFuncW

#else

#define DbgOutputFunc_t				DbgOutputFunc_tA
#define DbgOutputFunc				DbgOutputFuncA
#define GetDbgOutputFunc			GetDbgOutputFuncA

#endif

//-----------------------------------------------------------------------------
// ���ڹ������飬��������Դ�ӡ������Ϣ���������
// pGroupName - ÿ���˻���ÿ�������Ҫ�̶������֣���ӡʱ����Grp*�صĽӿ�
//			������涨����һ�����˵�APP_GROUP
// level - ��־���� 0����ʾ������1������Ҫ����־����2������Ҫ������<=2���������3���Դ�����4��5����������
//
// ˵����	�����Ҫ����ģ�����ڵĵ����߼���Σ��������������
//-----------------------------------------------------------------------------
DBG_INTERFACE void   DbgActivateA( char const* pGroupName, int level );
DBG_INTERFACE void   DbgActivateW( wchar_t const* pGroupName, int level );
DBG_INTERFACE bool   IsDbgActiveA( char const* pGroupName, int level );
DBG_INTERFACE bool   IsDbgActiveW( wchar_t const* pGroupName, int level );

#if defined(_UNICODE) || defined(UNICODE)

#define DbgActivate				DbgActivateW
#define IsDbgActive				IsDbgActiveW

#else

#define DbgActivate				DbgActivateA
#define IsDbgActive				IsDbgActiveA

#endif

//-----------------------------------------------------------------------------
// һ�㲻����ֱ�ӵ���
// ��ӡ��Ϣ���ڲ�ʹ�ã�
DBG_INTERFACE void			_DbgInfoA( DbgType_t type, char const* pFile, int line );
DBG_INTERFACE DbgRetval_t   _DbgMessageA( char const* pMsg, ... );
DBG_INTERFACE DbgRetval_t   _DDbgMessageA( char const *pGroupName, int level, char const* pMsg, ... );
DBG_OVERLOAD DbgRetval_t	_DbgMessageA( DbgType_t DebugType, char const* pMsgFormat, va_list args ,const char*szGrpName = NULL);
DBG_OVERLOAD DbgRetval_t	_DbgMessageModuleA( char const* pModuleName, DbgType_t DebugType, char const* pMsgFormat, va_list args );

DBG_INTERFACE void			_DbgInfoW( DbgType_t type, wchar_t const* pFile, int line );
DBG_INTERFACE DbgRetval_t   _DbgMessageW( wchar_t const* pMsg, ... );
DBG_INTERFACE DbgRetval_t   _DDbgMessageW( wchar_t const *pGroupName, int level, wchar_t const* pMsg, ... );
DBG_OVERLOAD DbgRetval_t	_DbgMessageW( DbgType_t DebugType, wchar_t const* pMsgFormat, va_list args ,const wchar_t*szGrpName = NULL);
DBG_OVERLOAD DbgRetval_t	_DbgMessageModuleW( wchar_t const* pModuleName, DbgType_t DebugType, wchar_t const* pMsgFormat, va_list args );

DBG_INTERFACE void			_ReportA( unsigned char * pBuffer, int iLen );
DBG_INTERFACE void			_ReportW( unsigned char * pBuffer, int iLen );

#if defined(_UNICODE) || defined(UNICODE)

#define _DbgInfo				_DbgInfoW
#define _DbgMessage				_DbgMessageW
#define _DDbgMessage			_DDbgMessageW
#define _DbgMessageModule		_DbgMessageModuleW

#else

#define _DbgInfo				_DbgInfoA
#define _DbgMessage				_DbgMessageA
#define _DDbgMessage			_DDbgMessageA
#define _DbgMessageModule		_DbgMessageModuleA

#endif

// �궨�壨�ڲ�ʹ�ã�
#define  _AssertA( _exp )			do {															\
									if (!(_exp)) 													\
									{ 																\
										_DbgInfoA( DBG_ASSERT, __FILE__, __LINE__ );				\
										if (_DbgMessageA("Assertion Failed: " #_exp) == DBG_RETURNED_DEBUGGER)		\
										{															\
											DebuggerBreak();										\
										}															\
									}																\
									} while (0)

#define  _AssertMsgA( _exp, _msg )	do {															\
									if (!(_exp)) 													\
									{ 																\
										_DbgInfoA( DBG_ASSERT, __FILE__, __LINE__ );				\
										if (_DbgMessageA(_msg) == DBG_RETURNED_DEBUGGER)					\
										{															\
											DebuggerBreak();										\
										}															\
									}																\
									} while (0)

#define  _AssertFuncA( _exp, _f )	do {															\
									if (!(_exp)) 													\
									{ 																\
										_DbgInfoA( DBG_ASSERT, __FILE__, __LINE__ );				\
										DbgRetval_t ret = _DbgMessageA("Assertion Failed!" #_exp);	\
										_f;															\
										if (ret == DBG_RETURNED_DEBUGGER)									\
										{															\
											DebuggerBreak();										\
										}															\
									}																\
									} while (0)

#define  _AssertEqualsA( _exp, _expectedValue ) \
									do {															\
									if ((_exp) != (_expectedValue)) 								\
									{ 																\
										_DbgInfoA( DBG_ASSERT, __FILE__, __LINE__ );				\
										DbgRetval_t ret = _DbgMessageA("Expected %d but got %d!", (_expectedValue), (_exp));	\
										if (ret == DBG_RETURNED_DEBUGGER)									\
										{															\
											DebuggerBreak();										\
										}															\
									}																\
									} while (0)

#define  _AssertFloatEqualsA( _exp, _expectedValue, _tol ) \
									do {															\
									if (fabs((_exp) - (_expectedValue)) > (_tol))					\
									{ 																\
										_DbgInfoA( DBG_ASSERT, __FILE__, __LINE__ );				\
										DbgRetval_t ret = _DbgMessageA("Expected %f but got %f!", (_expectedValue), (_exp));	\
										if (ret == DBG_RETURNED_DEBUGGER)									\
										{															\
											DebuggerBreak();										\
										}															\
									}																\
									} while (0)


// �궨�壨�ڲ�ʹ�ã�
#define  _AssertW( _exp )			do {															\
	if (!(_exp)) 													\
{ 																\
	_DbgInfoW( DBG_ASSERT, __WFILE__, __LINE__ );				\
	if (_DbgMessageW(L"Assertion Failed: " L#_exp) == DBG_RETURNED_DEBUGGER)		\
{															\
	DebuggerBreak();										\
}															\
}																\
} while (0)

#define  _AssertMsgW( _exp, _msg )	do {															\
	if (!(_exp)) 													\
{ 																\
	_DbgInfoW( DBG_ASSERT, __WFILE__, __LINE__ );				\
	if (_DbgMessageW(_msg) == DBG_RETURNED_DEBUGGER)					\
{															\
	DebuggerBreak();										\
}															\
}																\
} while (0)

#define  _AssertFuncW( _exp, _f )	do {															\
	if (!(_exp)) 													\
{ 																\
	_DbgInfoW( DBG_ASSERT, __WFILE__, __LINE__ );				\
	DbgRetval_t ret = _DbgMessageW(L"Assertion Failed!" L#_exp);	\
	_f;															\
	if (ret == DBG_RETURNED_DEBUGGER)									\
{															\
	DebuggerBreak();										\
}															\
}																\
} while (0)

#define  _AssertEqualsW( _exp, _expectedValue ) \
	do {															\
	if ((_exp) != (_expectedValue)) 								\
{ 																\
	_DbgInfoW( DBG_ASSERT, __WFILE__, __LINE__ );				\
	DbgRetval_t ret = _DbgMessageW(L"Expected %d but got %d!", (_expectedValue), (_exp));	\
	if (ret == DBG_RETURNED_DEBUGGER)									\
{															\
	DebuggerBreak();										\
}															\
}																\
	} while (0)

#define  _AssertFloatEqualsW( _exp, _expectedValue, _tol ) \
	do {															\
	if (fabs((_exp) - (_expectedValue)) > (_tol))					\
{ 																\
	_DbgInfoW( DBG_ASSERT, __WFILE__, __LINE__ );				\
	DbgRetval_t ret = _DbgMessageW(L"Expected %f but got %f!", (_expectedValue), (_exp));	\
	if (ret == DBG_RETURNED_DEBUGGER)									\
{															\
	DebuggerBreak();										\
}															\
}																\
	} while (0)


//-----------------------------------------------------------------------------
// ����������Ҫʹ�õĸ��ֶ���:-)��ͬMFC�÷����ƣ�Release��ʱ�����Ե�
//-----------------------------------------------------------------------------
#ifdef _DEBUG

#define  AssertA( _exp )           _AssertA( _exp )
#define  AssertMsgA( _exp, _msg )  _AssertMsgA( _exp, _msg )
#define  AssertFuncA( _exp, _f )   _AssertFuncA( _exp, _f )
#define  AssertEqualsA( _exp, _expectedValue )              _AssertEqualsA( _exp, _expectedValue )
#define  AssertFloatEqualsA( _exp, _expectedValue, _tol )   _AssertFloatEqualsA( _exp, _expectedValue, _tol )

#define  AssertW( _exp )           _AssertW( _exp )
#define  AssertMsgW( _exp, _msg )  _AssertMsgW( _exp, _msg )
#define  AssertFuncW( _exp, _f )   _AssertFuncW( _exp, _f )
#define  AssertEqualsW( _exp, _expectedValue )              _AssertEqualsW( _exp, _expectedValue )
#define  AssertFloatEqualsW( _exp, _expectedValue, _tol )   _AssertFloatEqualsW( _exp, _expectedValue, _tol )

#define  AssertMsg1( _exp, _msg, a1 )									_AssertMsg( _exp, CDbgFmtMsg( _msg, a1 ) )
#define  AssertMsg2( _exp, _msg, a1, a2 )								_AssertMsg( _exp, CDbgFmtMsg( _msg, a1, a2 ) )
#define  AssertMsg3( _exp, _msg, a1, a2, a3 )							_AssertMsg( _exp, CDbgFmtMsg( _msg, a1, a2, a3 ) )
#define  AssertMsg4( _exp, _msg, a1, a2, a3, a4 )						_AssertMsg( _exp, CDbgFmtMsg( _msg, a1, a2, a3, a4 ) )
#define  AssertMsg5( _exp, _msg, a1, a2, a3, a4, a5 )					_AssertMsg( _exp, CDbgFmtMsg( _msg, a1, a2, a3, a4, a5 ) )
#define  AssertMsg6( _exp, _msg, a1, a2, a3, a4, a5, a6 )				_AssertMsg( _exp, CDbgFmtMsg( _msg, a1, a2, a3, a4, a5, a6 ) )
#define  AssertMsg6( _exp, _msg, a1, a2, a3, a4, a5, a6 )				_AssertMsg( _exp, CDbgFmtMsg( _msg, a1, a2, a3, a4, a5, a6 ) )
#define  AssertMsg7( _exp, _msg, a1, a2, a3, a4, a5, a6, a7 )			_AssertMsg( _exp, CDbgFmtMsg( _msg, a1, a2, a3, a4, a5, a6, a7 ) )
#define  AssertMsg8( _exp, _msg, a1, a2, a3, a4, a5, a6, a7, a8 )		_AssertMsg( _exp, CDbgFmtMsg( _msg, a1, a2, a3, a4, a5, a6, a7, a8 ) )
#define  AssertMsg9( _exp, _msg, a1, a2, a3, a4, a5, a6, a7, a8, a9 )	_AssertMsg( _exp, CDbgFmtMsg( _msg, a1, a2, a3, a4, a5, a6, a7, a8, a9 ) )

#if defined(_UNICODE) || defined(UNICODE)

#define Assert					AssertW
#define AssertMsg				AssertMsgW
#define AssertFunc				AssertFuncW
#define AssertEquals			AssertEqualsW
#define AssertFloatEquals		AssertFloatEqualsW

#else

#define Assert					AssertA
#define AssertMsg				AssertMsgA
#define AssertFunc				AssertFuncA
#define AssertEquals			AssertEqualsA
#define AssertFloatEquals		AssertFloatEqualsA

#endif

#else /* Not _DEBUG */

#define  Assert( _exp )           ((void)0)
#define  AssertMsg( _exp, _msg )  ((void)0)
#define  AssertFunc( _exp, _f )   ((void)0)
#define  AssertEquals( _exp, _expectedValue )              ((void)0)
#define  AssertFloatEquals( _exp, _expectedValue, _tol )   ((void)0)

#define  AssertMsg1( _exp, _msg, a1 )									((void)0)
#define  AssertMsg2( _exp, _msg, a1, a2 )								((void)0)
#define  AssertMsg3( _exp, _msg, a1, a2, a3 )							((void)0)
#define  AssertMsg4( _exp, _msg, a1, a2, a3, a4 )						((void)0)
#define  AssertMsg5( _exp, _msg, a1, a2, a3, a4, a5 )					((void)0)
#define  AssertMsg6( _exp, _msg, a1, a2, a3, a4, a5, a6 )				((void)0)
#define  AssertMsg6( _exp, _msg, a1, a2, a3, a4, a5, a6 )				((void)0)
#define  AssertMsg7( _exp, _msg, a1, a2, a3, a4, a5, a6, a7 )			((void)0)
#define  AssertMsg8( _exp, _msg, a1, a2, a3, a4, a5, a6, a7, a8 )		((void)0)
#define  AssertMsg9( _exp, _msg, a1, a2, a3, a4, a5, a6, a7, a8, a9 )	((void)0)

#endif   /* _DEBUG */


//_exp�ܻ�ִ�е�
#if defined(_UNICODE) || defined(UNICODE)
#define  Verify( _exp )           _AssertW( _exp )
#else
#define  Verify( _exp )           _AssertA( _exp )
#endif

//-----------------------------------------------------------------------------
// Ŀ�� : ���ǵı�׼��־���
// ���� : *pGroupName - ���ִ�Сд������
//		  level - ����
//		  *pMsg - ������ĵ�����Ϣ
// ��� : void -
// ˵�� :	1��Grp* �ؽӿ�����֧�ַ����ӡ��
//			2��GrpError �������κ����뼶���飬�� Error �ȼۣ�
//			3��Msg ʹ�� DBG_MESSAGE �࣬������ͨ��Ϣ������������̣�
//			4��Log ʹ�� DBG_LOG �࣬���ڳ�������̱�ʽ��������Ϣ��������Ϊ����д��ϵͳ���¼����ݿ��У�
//			5��Warning ʹ�� DBG_WARNING �࣬���ڸ澯��Ϣ������Ӱ��������У�
//			6��Error ʹ�� DBG_ERROR �࣬˵�������������󣬺���������Ҫ�����˳���
//			7��Report ʹ�� DBG_REPORT�࣬�������һЩ�����Ƶ�������Ϣ��������Ϊ����д��ϵͳ���¼����ݿ��У�
//-----------------------------------------------------------------------------
DBG_INTERFACE void MsgA( char const* pMsg, ... );
DBG_INTERFACE void MsgW( wchar_t const* pMsg, ... );
DBG_INTERFACE void GrpMsgA( char const *pGroupName, int level, char const *pMsg, ... );
DBG_INTERFACE void GrpMsgW( wchar_t const *pGroupName, int level, wchar_t const *pMsg, ... );

DBG_INTERFACE void LogA( char const *pMsg, ... );
DBG_INTERFACE void LogW( wchar_t const *pMsg, ... );
DBG_INTERFACE void GrpLogA( char const *pGroupName, int level, char const *pMsg, ... );
DBG_INTERFACE void GrpLogW( wchar_t const *pGroupName, int level, wchar_t const *pMsg, ... );

DBG_INTERFACE void WarningA( char const *pMsg, ... );
DBG_INTERFACE void WarningW( wchar_t const *pMsg, ... );
DBG_INTERFACE void GrpWarningA( char const *pGroupName, int level, char const *pMsg, ... );
DBG_INTERFACE void GrpWarningW( wchar_t const *pGroupName, int level, wchar_t const *pMsg, ... );

DBG_INTERFACE void ErrorA( char const *pMsg, ... );
DBG_INTERFACE void ErrorW( wchar_t const *pMsg, ... );
DBG_INTERFACE void GrpErrorA( char const *pGroupName, int level, char const *pMsg, ... );
DBG_INTERFACE void GrpErrorW( wchar_t const *pGroupName, int level, wchar_t const *pMsg, ... );

DBG_INTERFACE void ReportA( unsigned char * pBuffer, int iLen );
DBG_INTERFACE void ReportW( unsigned char * pBuffer, int iLen );
DBG_INTERFACE void GrpReportA( char const *pGroupName, int level, unsigned char * pBuffer, int iLen );
DBG_INTERFACE void GrpReportW( wchar_t const *pGroupName, int level, unsigned char * pBuffer, int iLen );


#if defined(_UNICODE) || defined(UNICODE)

#define Msg			MsgW
#define GrpMsg		GrpMsgW
#define Log			LogW
#define GrpLog		GrpLogW
#define Warning		WarningW
#define GrpWarning	GrpWarningW
#define Error		ErrorW
#define GrpError	GrpErrorW
#define Report		ReportW
#define GrpReport	GrpReportW

#else

#define Msg			MsgA
#define GrpMsg		GrpMsgA
#define Log			LogA
#define GrpLog		GrpLogA
#define Warning		WarningA
#define GrpWarning	GrpWarningA
#define Error		ErrorA
#define GrpError	GrpErrorA
#define Report		ReportA
#define GrpReport	GrpReportA

#endif

// ����������жϣ����� ErrorIfNot( bCondition, ("a b c %d %d %d", 1, 2, 3) );
#define ErrorIfNot( condition, msg ) \
	if ( (condition) )		\
		;					\
	else 					\
	{						\
		Error msg;			\
	}


//-----------------------------------------------------------------------------
// Ŀ�� : ����ÿ��ʹ���Լ��ķּ�������Ϣ�����磺
//			1�������Զ���ʹ��һ�� name �飬���Ҳ�ɹ���һ�����֣����Լ�����
//			2��Ĭ���������Ϊ level ��
//			3�� ��name##Errorû�м����ж���ԭ���ǣ�������Ա��뱻������˵��ڴ�Ҷ�
//				����Error()Ҳ��
//			4������ DECLARE_DEBUG_GROUP(Module1, 2) ��ʾModule1��Ĭ�ϵ��Լ�����Ϊ2
//				������ Module1Msg(3, "OnlyU") �����������Module1Msg("Only you")
//				�ɱ������Ĭ��Ϊ2����Module1Msg(2, "OnlyU")�ɱ��������ʾָ����2��
// ���� : name - ����ʹ���Լ�����
//		  level - ����Ĭ�ϼ���Ϊ level ��
// ��� : �Զ�����8��inline���͵ĺ���
//-----------------------------------------------------------------------------
#define CAT_DEBUG_REPORT(name, level) \
	{if( !IsDbgActive( #name, level ) ) return; \
		_Report( pBuffer, iLen ); \
	}

#define CAT_DEBUG_PRINT(name, type, level) \
	{if( !IsDbgActive( #name, level ) ) return; \
		va_list args; \
		va_start( args, pMsgFormat ); \
		_DbgMessageModule( #name, type, pMsgFormat, args ); \
		va_end(args); \
	}

#define CAT_DEBUG_ERROR(name, type) \
	{ \
		va_list args; \
		va_start( args, pMsgFormat ); \
		_DbgMessageModule( #name, type, pMsgFormat, args ); \
		va_end(args); \
	}

#define DECLARE_DEBUG_GROUP(name, defaultlevel) \
	inline void name##Msg( int level, char const* pMsgFormat, ... ) \
	CAT_DEBUG_PRINT(name, DBG_MESSAGE, level)\
	inline void name##Warning( int level, char const* pMsgFormat, ... ) \
	CAT_DEBUG_PRINT(name, DBG_WARNING, level)\
	inline void name##Log( int level, char const* pMsgFormat, ... ) \
	CAT_DEBUG_PRINT(name, DBG_LOG, level)\
	inline void name##Report( int level, unsigned char * pBuffer, int iLen ) \
	CAT_DEBUG_REPORT(name, level) \
	inline void name##Msg( char const *pMsgFormat, ... ) \
	CAT_DEBUG_PRINT(name, DBG_MESSAGE, defaultlevel)\
	inline void name##Warning( char const *pMsgFormat, ... ) \
	CAT_DEBUG_PRINT(name, DBG_WARNING, defaultlevel)\
	inline void name##Log( char const *pMsgFormat, ... ) \
	CAT_DEBUG_PRINT(name, DBG_LOG, defaultlevel) \
	inline void name##Report( unsigned char * pBuffer, int iLen ) \
	CAT_DEBUG_REPORT(name, defaultlevel) \
	inline void name##Error( char const *pMsgFormat, ... ) \
	CAT_DEBUG_ERROR(name, DBG_ERROR)


// ����ʹ��
#ifdef _DEBUG

	#define DBG_CODE( _code )				if (0) ; else { _code }
	#define DBG_GRPCODE( _g, _l, _code )	if (IsDbgActive( _g, _l )) { _code } else {}
	#define DBG_BREAK()						DebuggerBreak()	/* defined in platform.h */

#else /* not _DEBUG */

	#define DBG_CODE( _code )				((void)0)
	#define DBG_GRPCODE( _g, _l, _code )		((void)0)
	#define DBG_BREAK()						((void)0)

#endif /* _DEBUG */



//-----------------------------------------------------------------------------
// ���ָ��ĺϷ���
//-----------------------------------------------------------------------------
DBG_INTERFACE void _AssertValidReadPtr( void* ptr, int count = 1 );
DBG_INTERFACE void _AssertValidWritePtr( void* ptr, int count = 1 );
DBG_INTERFACE void _AssertValidReadWritePtr( void* ptr, int count = 1 );

DBG_INTERFACE  void AssertValidStringPtr( const char* ptr, int maxchar = 0xFFFFFF );
template<class T> inline void AssertValidReadPtr( T* ptr, int count = 1 )		     { _AssertValidReadPtr( (void*)ptr, count ); }
template<class T> inline void AssertValidWritePtr( T* ptr, int count = 1 )		     { _AssertValidWritePtr( (void*)ptr, count ); }
template<class T> inline void AssertValidReadWritePtr( T* ptr, int count = 1 )	     { _AssertValidReadWritePtr( (void*)ptr, count ); }

#define AssertValidThis() AssertValidReadWritePtr(this,sizeof(*this))

//-----------------------------------------------------------------------------
// ������ǰ���������ݹ�ȷ�ʽ����
//-----------------------------------------------------------------------------
#ifdef _DEBUG
class CReentryGuard
{
public:
	CReentryGuard(int *pSemaphore)
	 : m_pSemaphore(pSemaphore)
	{
		++(*m_pSemaphore);
	}

	~CReentryGuard()
	{
		--(*m_pSemaphore);
	}

private:
	int *m_pSemaphore;
};

#define ASSERT_NO_REENTRY() \
	static int fSemaphore##__LINE__; \
	Assert( !fSemaphore##__LINE__ ); \
	CReentryGuard ReentryGuard##__LINE__( &fSemaphore##__LINE__ )
#else
#define ASSERT_NO_REENTRY()
#endif

//-----------------------------------------------------------------------------
// ��ʽ��������Ϣ
//-----------------------------------------------------------------------------
class CDbgFmtMsg
{
public:
#if defined(_UNICODE) || defined(UNICODE)
	CDbgFmtMsg(const wchar_t *pszFormat, ...)
#else
	CDbgFmtMsg(const char *pszFormat, ...)
#endif
	{
		va_list arg_ptr;

		va_start(arg_ptr, pszFormat);
#if defined(_UNICODE) || defined(UNICODE)
		_vsnwprintf_s(m_szBuf, sizeof(m_szBuf), sizeof(m_szBuf)-1, pszFormat, arg_ptr);
#else
		_vsnprintf_s(m_szBuf, sizeof(m_szBuf), sizeof(m_szBuf)-1, pszFormat, arg_ptr);
#endif
		va_end(arg_ptr);

		m_szBuf[sizeof(m_szBuf)-1] = 0;
	}
#if defined(_UNICODE) || defined(UNICODE)
	operator const wchar_t *() const
#else
	operator const char *() const
#endif
	{
		return m_szBuf;
	}

private:

#if defined(_UNICODE) || defined(UNICODE)
	wchar_t m_szBuf[1024];
#else
	char m_szBuf[1024];
#endif

};

// //-----------------------------------------------------------------------------
// // Ŀ�ģ� ���԰�ʱ��Ŀ���ļ���Ƕ�����ʱ����Ϣ
// //-----------------------------------------------------------------------------
// #ifdef _WIN32
//
// 	#ifdef _DEBUG
// 		#pragma comment(compiler)
// 		#pragma comment(exestr,"Debug Version, Last Compile Time: " __DATE__ ", " __TIME__ " ***")
// 	#endif
//
// #endif


#endif // _DBG_INCLUDE_H_
