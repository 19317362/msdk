// testcomx3.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// Ctestcomx3App:
// �йش����ʵ�֣������ testcomx3.cpp
//

class Ctestcomx3App : public CWinApp
{
public:
	Ctestcomx3App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Ctestcomx3App theApp;
