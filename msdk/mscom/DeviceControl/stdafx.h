// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <algorithm>
#include <string>
#include "3rd/crc32.h"

#include "mscom/mscominc.h"
#include "atlstr.h"

#define GroupName _T("DeviceControl")
#include "mslog/logtool/logtool.h"
//#define DEVICECONTROL_DLL

#define RASSERT(x, _h_r_) { if(!(x)) return _h_r_; }


static BOOL CompareString(std::wstring strLength, std::wstring strLow )
{
	BOOL  bRet = FALSE;

	if( strLength.empty() &&
		strLow.empty() )
	{
		return bRet;
	}

	// ȫ��ת���ɴ�д
	std::transform(strLength.begin(), strLength.end(), strLength.begin(), toupper);

	// ת���ɴ�д
	std::transform( strLow.begin(), strLow.end(),strLow.begin(), toupper );

	// ִ�бȶ�, ����Ա����
	if( strLength.compare( 0, strLow.size(),strLow ) == 0 )
	{
		bRet = TRUE;
	}

	return bRet;
}

static DWORD CalcCrc32(std::wstring str)
{
	return DWORD(std_crc32((const char*)str.c_str(),str.length() * sizeof(WCHAR)));
}