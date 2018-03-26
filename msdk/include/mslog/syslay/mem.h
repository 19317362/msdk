//
// ��Ŀ����: 
//
// ʵ�ֹ���: �����Ƚ��ȳ�ջ�Ķ��ڴ�����������кܺõķ������ܺ����ڴ���Ƭ���ص�
//
// �ļ�����: mem.h
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
// ʹ��˵��: ע���ڴ��������ʼ�����ڴ��ʵ�ʴ�С����С��1M��
//
//=============================================================================

#ifndef _MEM_INCLUDE_H_
#define _MEM_INCLUDE_H_
#ifdef _WIN32
#pragma once
#endif


#include <stddef.h>
#include "mslog/Syslay/platform.h"

#ifdef SYSLAY_DLL_EXPORT
#  define MEM_INTERFACE DLL_EXPORT
#else
#  define MEM_INTERFACE DLL_IMPORT
#endif


//-----------------------------------------------------------------------------
// Ŀ�� : �����ڴ�
// ���� : nMemSize - ��С
// ��� : MEM_INTERFACE void * - �ڴ�ָ��
//-----------------------------------------------------------------------------
MEM_INTERFACE void * MemAllocStack( int nMemSize );

//-----------------------------------------------------------------------------
// Ŀ�� : �ͷ����������ڴ�
// ��� : MEM_INTERFACE void - 
//-----------------------------------------------------------------------------
MEM_INTERFACE void MemFreeStack();


#ifdef _LINUX
MEM_INTERFACE void ZeroMemory( void *mem, size_t length );
#endif


#endif // _MEM_INCLUDE_H_
