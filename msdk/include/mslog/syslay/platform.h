//
// ��Ŀ����: 
//
// ʵ�ֹ���: ƽ̨���ö���
//
// �ļ�����: platform.h
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
//
//=============================================================================

#ifndef _PLATFORM_INCLUDE_H_
#define _PLATFORM_INCLUDE_H_
#ifdef _WIN32
#pragma once
#endif


// ʹ���� _alloca
#include <malloc.h>

// ʹ���� memset
#include <string.h>

// ��ʾ����
typedef signed char      int8;
typedef signed short     int16;
typedef signed long      int32;

#ifdef _WIN32
	#ifdef _MSC_VER
	typedef signed __int64   int64;
	#endif
#elif defined _LINUX
	typedef long long	int64;
#endif

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned long      uint32;
#ifdef _WIN32
	#ifdef _MSC_VER
	typedef unsigned __int64   uint64;
	#endif
#elif defined _LINUX
	typedef unsigned long long uint64;
#endif


typedef float  float32;
typedef double float64;

typedef unsigned int uint;

#ifndef HRESULT
typedef int32 HRESULT;
#endif

#ifdef _MSC_VER
	#define SINGLE_INHERITANCE __single_inheritance 
	#define MULTIPLE_INHERITANCE __multiple_inheritance 
#else
	#define SINGLE_INHERITANCE  
	#define MULTIPLE_INHERITANCE 
#endif

// һЩ�������Ĳ��
#if defined(_WIN32) && !defined(WINDED)

	#if defined(_M_IX86)
		#define __i386__	1
	#endif

#elif _LINUX

	typedef unsigned int DWORD;
	typedef unsigned short WORD;
	typedef void * HINSTANCE;

	#define _MAX_PATH PATH_MAX

#endif // defined(_WIN32) && !defined(WINDED)

 
// Defines MAX_PATH
#ifndef MAX_PATH
	#define MAX_PATH  260
#endif

// �����ϵ�
#define  DebuggerBreak() do{}while(0);

// ��������
#ifndef EXPORT
	#ifdef _WIN32
		#define EXPORT	_declspec( dllexport )
	#else 
		#define EXPORT	/* */
	#endif
#endif

#if defined __i386__ && !defined __linux__
	#define id386	1
#else
	#define id386	0
#endif  // __i386__

#ifdef _WIN32
	
#ifdef SYSLAY_LINK_STATIC

	// ��̬��ĺ��������뵼������
	#define  DLL_EXPORT   extern "C"
	#define  DLL_IMPORT   extern "C"

	// һ����ĵ����뵼��������ʹ��extern "C"	
	#define  DLL_CLASS_EXPORT
	#define  DLL_CLASS_IMPORT

	// һ��ȫ�ֱ����ĵ����뵼��������ʹ��extern "C"
	#define  DLL_GLOBAL_EXPORT
	#define  DLL_GLOBAL_IMPORT

#else
	// ��̬��ĺ��������뵼������
	#define  DLL_EXPORT   extern "C" __declspec( dllexport ) 
	#define  DLL_IMPORT   extern "C" __declspec( dllimport )

	// һ����ĵ����뵼��������ʹ��extern "C"	
	#define  DLL_CLASS_EXPORT   __declspec( dllexport ) 
	#define  DLL_CLASS_IMPORT   __declspec( dllimport )

	// һ��ȫ�ֱ����ĵ����뵼��������ʹ��extern "C"
	#define  DLL_GLOBAL_EXPORT   extern __declspec( dllexport ) 
	#define  DLL_GLOBAL_IMPORT   extern __declspec( dllimport )
#endif

#elif defined _LINUX

	// ��̬��ĵ����뵼����������
	#define  DLL_EXPORT   extern "C" 
	#define  DLL_IMPORT   extern "C" 

	// һ����ĵ����뵼��������ʹ��extern "C"
	#define  DLL_CLASS_EXPORT   
	#define  DLL_CLASS_IMPORT  

	// һ��ȫ�ֱ����ĵ����뵼��������ʹ��extern "C"
	#define  DLL_GLOBAL_EXPORT   extern
	#define  DLL_GLOBAL_IMPORT   extern 

#else

	#error "Unsupported Platform."

#endif

// ��������
#ifdef _WIN32
	#define  STDCALL				__stdcall
	#define  FASTCALL			   __fastcall
	#define  FORCEINLINE		   __forceinline	
#else
	#define  STDCALL
	#define  FASTCALL			   
	#define  FORCEINLINE		   inline
#endif


// ����MSVC�е�__assume�Ż�����
#ifdef _MSC_VER
	#define HINT(THE_HINT)	__assume((THE_HINT))
#else
	#define HINT(THE_HINT)	0
#endif

#define UNREACHABLE() { Assert(0); HINT(0); }

#define NO_DEFAULT default: UNREACHABLE();

// ������ջ�з����ڴ�
#ifdef _WIN32	
	#define  stackalloc( _size ) _alloca( _size )
	#define  stackfree( _p )   0
#elif _LINUX	
	#define  stackalloc( _size ) alloca( _size )
	#define  stackfree( _p )   0
#endif


#ifdef _WIN32
	#pragma warning(disable : 4514) // warning C4514: 'acosl' : unreferenced inline function has been removed
	#pragma warning(disable : 4100) // warning C4100: 'hwnd' : unreferenced formal parameter
	#pragma warning(disable : 4127) // warning C4127: conditional expression is constant
	#pragma warning(disable : 4512) // warning C4512: 'InFileRIFF' : assignment operator could not be generated
	#pragma warning(disable : 4611) // warning C4611: interaction between '_setjmp' and C++ object destruction is non-portable
	#pragma warning(disable : 4706) // warning C4706: assignment within conditional expression
	#pragma warning(disable : 4710) // warning C4710: function 'x' not inlined
	#pragma warning(disable : 4702) // warning C4702: unreachable code
	#pragma warning(disable : 4505) // unreferenced local function has been removed
	#pragma warning(disable : 4239) // nonstandard extension used : 'argument' ( conversion from class Vector to class Vector& )
	#pragma warning(disable : 4097) // typedef-name 'BaseClass' used as synonym for class-name 'CFlexCycler::CBaseFlex'
	#pragma warning(disable : 4324) // Padding was added at the end of a structure
	#pragma warning(disable : 4244) // type conversion warning.
	#pragma warning(disable : 4305)	// truncation from 'const double ' to 'float '
	#pragma warning(disable : 4786)	// Disable warnings about long symbol names

	#if _MSC_VER >= 1300
		#pragma warning(disable : 4511)	// private���͵Ŀ������캯���ĸ澯
	#endif
#endif
//-----------------------------------------------------------------------------
// Ŀ��: ���� endian-ness
//-----------------------------------------------------------------------------

//-------------------------------------
// һ��� swaps
//-------------------------------------

template <typename T>
inline T WordSwapC( T w )
{
   uint16 temp;

   temp  = ((*((uint16 *)&w) & 0xff00) >> 8);
   temp |= ((*((uint16 *)&w) & 0x00ff) << 8);

   return *((T*)&temp);
}

template <typename T>
inline T DWordSwapC( T dw )
{
   uint32 temp;

   temp  =   *((uint32 *)&dw) 				>> 24;
   temp |= ((*((uint32 *)&dw) & 0x00FF0000) >> 8);
   temp |= ((*((uint32 *)&dw) & 0x0000FF00) << 8);
   temp |= ((*((uint32 *)&dw) & 0x000000FF) << 24);

   return *((T*)&temp);
}

//-------------------------------------
// ���ٵ� swaps
// ��Ҫ���MSVC������
//-------------------------------------

#ifdef _MSC_VER

	#define WordSwap  WordSwapAsm
	#define DWordSwap DWordSwapAsm

	#pragma warning(push)
	#pragma warning (disable:4035) // no return value

	template <typename T>
	inline T WordSwapAsm( T w )
	{
		/*
	   __asm
	   {
		  mov ax, w
		  xchg al, ah
	   }
	   */
	}

	template <typename T>
	inline T DWordSwapAsm( T dw )
	{
		/*
	   __asm
	   {
		  mov eax, dw
		  bswap eax
	   }
	   */
	}

	#pragma warning(pop)

	// float������
	template <>
	inline float DWordSwapAsm<float>( float f )
	{
		return DWordSwapC( f );
	}

#else

	#define WordSwap  WordSwapC
	#define DWordSwap DWordSwapC

#endif

//-------------------------------------
// ʶ��ϵͳ
//-------------------------------------

#if defined(__i386__)
	#define LITTLE_ENDIAN 1
#endif

#ifdef _SGI_SOURCE
	#define	BIG_ENDIAN 1
#endif

#if defined(LITTLE_ENDIAN)

	#define BigShort( val )		WordSwap( val )
	#define BigWord( val )		WordSwap( val )
	#define BigLong( val )		DWordSwap( val )
	#define BigDWord( val )		DWordSwap( val )
	#define BigFloat( val )		DWordSwap( val )
	#define LittleShort( val )	( val )
	#define LittleWord( val )	( val )
	#define LittleLong( val )	( val )
	#define LittleDWord( val )	( val )
	#define LittleFloat( val )	( val )

#elif defined(BIG_ENDIAN)

	#define BigShort( val )		( val )
	#define BigWord( val )		( val )
	#define BigLong( val )		( val )
	#define BigDWord( val )		( val )
	#define BigFloat( val )		( val )
	#define LittleShort( val )	WordSwap( val )
	#define LittleWord( val )	WordSwap( val )
	#define LittleLong( val )	DWordSwap( val )
	#define LittleDWord( val )	DWordSwap( val )
	#define LittleFloat( val )	DWordSwap( val )

#else

	//��ĳЩƽ̨�������ϵķ���
	inline short BigShort( short val )		{ int test = 1; return ( *(char *)&test == 1 ) ? WordSwap( val )  : val; }
	inline uint16 BigWord( uint16 val )		{ int test = 1; return ( *(char *)&test == 1 ) ? WordSwap( val )  : val; }
	inline long BigLong( long val )			{ int test = 1; return ( *(char *)&test == 1 ) ? DWordSwap( val ) : val; }
	inline uint32 BigDWord( uint32 val )	{ int test = 1; return ( *(char *)&test == 1 ) ? DWordSwap( val ) : val; }
	inline float BigFloat( float val )		{ int test = 1; return ( *(char *)&test == 1 ) ? DWordSwap( val ) : val; }
	inline short LittleShort( short val )	{ int test = 1; return ( *(char *)&test == 1 ) ? val : WordSwap( val ); }
	inline uint16 LittleWord( uint16 val )	{ int test = 1; return ( *(char *)&test == 1 ) ? val : WordSwap( val ); }
	inline long LittleLong( long val )		{ int test = 1; return ( *(char *)&test == 1 ) ? val : DWordSwap( val ); }
	inline uint32 LittleDWord( uint32 val )	{ int test = 1; return ( *(char *)&test == 1 ) ? val : DWordSwap( val ); }
	inline float LittleFloat( float val )	{ int test = 1; return ( *(char *)&test == 1 ) ? val : DWordSwap( val ); }

#endif



#ifdef SYSLAY_DLL_EXPORT
	#define PLATFORM_INTERFACE	DLL_EXPORT
	#define PLATFORM_OVERLOAD	DLL_GLOBAL_EXPORT
#else
	#define PLATFORM_INTERFACE	DLL_IMPORT
	#define PLATFORM_OVERLOAD	DLL_GLOBAL_IMPORT
#endif


PLATFORM_INTERFACE double			Sys_RunFloatTime();		// ��ģ���һ�ε��õ���ε��õ�ʱ�������룩
PLATFORM_INTERFACE unsigned long	Sys_RunMSTime();		// ʱ����Ϊ����

// CPU ����Ϣ
struct CPUInformation
{
	int	 m_Size;		// Size of this structure, for forward compatability.

	bool m_bRDTSC : 1,	// Is RDTSC supported?
		 m_bCMOV  : 1,  // Is CMOV supported?
		 m_bFCMOV : 1,  // Is FCMOV supported?
		 m_bSSE	  : 1,	// Is SSE supported?
		 m_bSSE2  : 1,	// Is SSE2 Supported?
		 m_b3DNow : 1,	// Is 3DNow! Supported?
		 m_bMMX   : 1,	// Is MMX supported?
		 m_bHT	  : 1;	// Is HyperThreading supported?

	unsigned char m_nLogicalProcessors,		// Number op logical processors.
		          m_nPhysicalProcessors;	// Number of physical processors

	int64 m_Speed;						// In cycles per second.

	char* m_szProcessorID;				// Processor vendor Identification.
};

PLATFORM_INTERFACE const CPUInformation& Sys_CPUInfo();


// ---------------------------------------------------------------------------------- //
// �ڴ����ӿ�
// 1�����̰߳�ȫ�ķ���
// 2�����Խ����ڴ�й©����Ч�Լ�ص�
// ---------------------------------------------------------------------------------- //

// ע�⣺size - ��������ڴ�Ĵ�С
typedef void (*Sys_AllocErrorFn)( unsigned long size );

PLATFORM_INTERFACE void*			Sys_Alloc( unsigned long size );				// Allocate
PLATFORM_INTERFACE void*			Sys_Realloc( void *ptr, unsigned long size );	// Rellocate
PLATFORM_INTERFACE void				Sys_Free( void *ptr );							// Free
PLATFORM_INTERFACE void				Sys_SetAllocErrorFn( Sys_AllocErrorFn fn );		// �������Լ����ڴ���������

//-----------------------------------------------------------------------------
// ��������ʱ������ͷ�ļ�
//
#include "mslog/Syslay/fasttimer.h"


#endif // _PLATFORM_INCLUDE_H_
