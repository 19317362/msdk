#pragma once
/*
	˵�����ṩMSC��Ϣ�������Ĺ���
*/

interface IMsgCenter
{
/*
	���ܣ����ö������ĵ������ռ�
	������lpstrName �����ռ�
	���أ�SUCCESSED(hr)
	˵��������ӿڽ����� Init ֮ǰ���ã������� ��ס����е���
		  �������û�����������ռ�����ȡEnvInfo��EnvInfo_MsgCenter��������Ϣ
		  ����������鿴 envinfo_i.h
*/
	STDMETHOD(SetNameSpace)(LPCWSTR lpstrName) = 0;
};
MS_DEFINE_IID(IMsgCenter , "{800E4D86-8297-45C6-AFD8-B1EC8B34C90D}");

// {62D8CCDE-7CA1-428c-AE51-9C92E6B39E89}
MS_DEFINE_GUID(CLSID_MsgCenter, 
	0x62d8ccde, 0x7ca1, 0x428c, 0xae, 0x51, 0x9c, 0x92, 0xe6, 0xb3, 0x9e, 0x89);
