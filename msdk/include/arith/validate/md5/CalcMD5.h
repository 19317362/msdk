#pragma once

#include "\arithmetic\validate\md5\MD5.h"
// ����\arithmetic\validate\md5\MD5.cpp
#define MD5_DATA_LEN 32

class CRavMonCalcMD5
{
public:

	CRavMonCalcMD5(void)
	{
	}

	~CRavMonCalcMD5(void)
	{
	}

	static BOOL GetFileMD5String(LPCTSTR lpFileName,LPSTR lpszMD5String)
	{
		if (lpFileName == NULL)
		{
			return FALSE;
		}

		if (lpszMD5String == NULL)
		{
			return FALSE;
		}

		BYTE	szMD5[MD5_DATA_LEN]={0};

		if(CalcOneFileMD5(lpFileName, szMD5))
		{
			return Bin2HString(szMD5,MD5_DATA_LEN,lpszMD5String);
		}

		return FALSE;

	}

	static BOOL GetDataMd5Byte(LPBYTE lpbData, DWORD dwDataLen, LPBYTE lpbMd5)
	{
		if(!lpbData ||
			!dwDataLen ||
			!lpbMd5)
		{
			return FALSE;
		}

		return CalcDataMD5(lpbData, dwDataLen, lpbMd5);
	}

	static BOOL GetDataMd5String(LPBYTE lpbData, DWORD dwDataLen, LPSTR pszMd5)
	{
		if(!lpbData ||
			!dwDataLen ||
			!pszMd5)
		{
			return FALSE;
		}

		BYTE szMD5[MD5_DATA_LEN] = {0};
		if(CalcDataMD5(lpbData, dwDataLen, szMD5))
		{
			return Bin2HString(szMD5, MD5_DATA_LEN, pszMd5);
		}

		return FALSE;
	}


private:
	//��ƺ���
	static BOOL Bin2HString(CONST BYTE* pbySrc, ULONG ulSize, LPSTR lpszDest)
	{
		BYTE byTemp;
		ULONG i = 0;
		for ( i = 0; i < ulSize/2; i++ )
		{
			byTemp = pbySrc[i] & 0x0F;
			if ( byTemp < 0x0A )
				lpszDest[ (i<<1) + 1 ] = byTemp + 0x30;
			else
				lpszDest[ (i<<1) + 1 ] = byTemp + 0x37;

			byTemp = (( pbySrc[i] >> 4 ) & 0x0F);
			if ( byTemp < 0x0A )
				lpszDest[ i<<1 ] = byTemp + 0x30;
			else
				lpszDest[ i<<1 ] = byTemp + 0x37;
		}
		lpszDest[ i<<1 ] = '\0';

		return TRUE;
	}


	static BOOL CheckFileSize(LPCTSTR lpFile)
	{
		WIN32_FILE_ATTRIBUTE_DATA data;
		memset(&data, 0, sizeof (data));
		if (!::GetFileAttributesEx(lpFile, GetFileExInfoStandard, &data))
			return FALSE;

		// < 2MB //��ʱȡ��2Mb����
		//if (data.nFileSizeLow > 2*1024*1024 || data.nFileSizeHigh > 0)
		//	return FALSE;

		if (data.nFileSizeLow >= 2047*1024*1024 || data.nFileSizeHigh > 0)
			return FALSE;

		// = 0
		if (data.nFileSizeLow == 0 && data.nFileSizeHigh == 0)
			return FALSE;

		return TRUE;
	}

	//����md5
	static BOOL CalcOneFileMD5(LPCTSTR lpFile, LPBYTE lpszMD5)
	{
		if (!CheckFileSize(lpFile))
			return FALSE;

		//����md5
		HANDLE hFile = 	CreateFile(lpFile, GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if( hFile == INVALID_HANDLE_VALUE )
		{  //���ļ�ʧ��
			return FALSE;
		}

		DWORD iFileSize = GetFileSize(hFile,NULL);
		HANDLE hFileMap = CreateFileMapping(hFile,NULL,PAGE_READONLY,NULL,NULL,NULL);

		if(hFileMap == NULL)
		{//�����ļ�Ӱ��ʧ��
			CloseHandle(hFile);
			return FALSE;
		}

		BYTE* MapAddr = (BYTE*)MapViewOfFile(hFileMap,FILE_MAP_READ,NULL,NULL,NULL);
		if(MapAddr == NULL)
		{
			//Ӱ���ļ�ʧ��
			CloseHandle(hFileMap);
			CloseHandle(hFile);
			return FALSE;
		}

		md5_state_t mdt;

		if (iFileSize > 2*1024*1024)//����2M����β1M
		{
			md5_init(&mdt);
			md5_append(&mdt, MapAddr, 1*1024*1024); //ͷ1M
			md5_append(&mdt, MapAddr+iFileSize-1024*1024, 1*1024*1024); //ĩ1M
			md5_finish(&mdt, lpszMD5);
		}
		else
		{
			md5_init(&mdt);
			md5_append(&mdt, MapAddr, iFileSize);
			md5_finish(&mdt, lpszMD5);
		}

		UnmapViewOfFile(MapAddr);
		CloseHandle(hFileMap);
		CloseHandle(hFile);

		return TRUE;
	}


	static BOOL CalcDataMD5(LPBYTE lpbData, DWORD dwDataLen, LPBYTE lpbMd5)
	{
		md5_state_t mdt;

		if (dwDataLen > 2*1024*1024)//����2M����β1M
		{
			md5_init(&mdt);
			md5_append(&mdt, lpbData, 1*1024*1024); //ͷ1M
			md5_append(&mdt, lpbData+dwDataLen-1024*1024, 1*1024*1024); //ĩ1M
			md5_finish(&mdt, lpbMd5);
		}
		else
		{
			md5_init(&mdt);
			md5_append(&mdt, lpbData, dwDataLen);
			md5_finish(&mdt, lpbMd5);
		}

		return TRUE;
	}
};
