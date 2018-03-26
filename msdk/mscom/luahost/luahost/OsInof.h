#pragma once

#include "luahost/luahost.h"
//lua  ��չ
class COsInof
{
public:
	COsInof();
	~COsInof(void);


	static BOOL Bind(CLuaHost* pLuaHost);


	//��ȡ����ϵͳ��Ϣ
	static std::string GetOsInfo();

	//��ȡϵͳ���ܴ�С
	static std::string GetDiskSize();

	//��ȡϵͳ��ʣ��ռ��С
	static std::string GetDiskFreeSize();

	//��ȡ�û�����
	static std::string GetUserName();

	//��ȡ��������
	static std::string GetComputerName();

	static bool IsWow64();

	static bool IsVM();
};

