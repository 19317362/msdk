// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����

// Windows ͷ�ļ�:
#include <windows.h>

extern HINSTANCE g_hInstance;

#include <atlstr.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlgdi.h>

#define _WTL_NO_CSTRING
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlframe.h>

#include "dsui.h"

#include "dsUIState.h"
#include "9squares.h"
#include "dsImageMgr.h"
#include "dsUIClassInfo.h"

#include "dsTools.h"
using namespace dsTools;

#include "dsGlobalStateMgr.h"

#include "dsNoPaintMsgMgr.h"



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
