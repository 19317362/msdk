// UITest.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "UITest.h"



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	msui::IUIObjectFactoryMgr* pFactoryMgr = GetObjectFactoryMgr();
	msui::IUIObject* pObject = CreateObject(OBJECT_SKIN_FILE);
	pFactoryMgr->DestoryObject(pObject);
	return 0;
}