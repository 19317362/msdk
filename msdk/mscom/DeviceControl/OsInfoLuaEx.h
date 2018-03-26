#pragma once
#include "luahost/ILuaHost.h"
class COsInfoLuaEx:
	public ILuaExtend,
	public CUnknownImp
{
public:
	UNKNOWN_IMP1_(ILuaExtend);

	HRESULT init_class(IMSBase* pRot, IMSBase* pOuter)
	{
		RASSERT(pRot, E_FAIL);
		m_pRot = pRot;
		return S_OK;
	}

	COsInfoLuaEx(void);
	~COsInfoLuaEx(void);

protected:
	STDMETHOD(RegisterLuaEx)(ILuaVM* pLuaVm);

	//��ȡ����ϵͳ�汾��
	static int GetOsVersion();

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

private:
	UTIL::com_ptr<IMscomRunningObjectTable> m_pRot;
};

