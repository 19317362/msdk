// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>
#include <WinInet.h>


//#define  _WTL_NO_CSTRING	
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>
//#include <atlstr.h>
#include <map>
#include <list>
#include <queue>
using namespace std;


#define MAXLEN_LOCAL_PIECE	4*1024*1024
#define MINLEN_LOCAL_PIECE	100*1024
extern DWORD g_dwMaxLocalPieceSize;

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
