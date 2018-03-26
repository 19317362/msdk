#pragma once

#include <Windows.h>
extern "C"{

typedef  void* LZMA_HANDLE;


LZMA_HANDLE LZMA_OpenFileW(LPCWSTR lpszFile);
LZMA_HANDLE LZMA_OpenFileA(LPCSTR  lpszFile);
LZMA_HANDLE LZMA_OpenMem(LPVOID pMem, DWORD dwLength);

VOID		LZMA_Close(LZMA_HANDLE _handle);

DWORD		LZMA_GetFileCount(LZMA_HANDLE _handle);

/*!
	�����ļ����Ƴ��ȣ�-1ʧ��
*/
DWORD		LZMA_GetFileNameW(LZMA_HANDLE _handle, DWORD dwIndex, LPWSTR lpszName, DWORD dwLenght);
DWORD		LZMA_GetFileNameA(LZMA_HANDLE _handle, DWORD dwIndex, LPSTR  lpszName, DWORD dwLenght);



/*!
	���س��ȣ���ѹ����ĳ��ȣ�-1ʧ��
*/
DWORD       LZMA_ExtractW(LZMA_HANDLE _handle, DWORD dwIndex, LPCWSTR lpszFile);
DWORD		LZMA_ExtractA(LZMA_HANDLE _handle, DWORD dwIndex, LPCSTR  lpszFile);

};


