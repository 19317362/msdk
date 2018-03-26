
#pragma once


//ǰ������


namespace msdk{;
namespace mscom{

/*struct msdk::mscom::IStringA;*/

enum	LuaVm_OpenMask
{
	LuaVm_Crypt			= 0x0001,
	LuaVm_Service		= 0x0002,
	LuaVm_RegKey		= 0x0004,
	LuaVm_FileFind		= 0x0008,
	LuaVm_FileSystems	= 0x0010,
	LuaVm_QuickLink		= 0x0020,
	LuaVm_RapidXml		= 0x0040,
	LuaVm_Ini			= 0x0080,
	LuaVm_Socket		= 0x0100,
	LuaVm_All			= 0xFFFF,
};


//LUA ���
//�ڸ�������Ҫʵ�� ConvertTo** ע:(**Ϊ������������lua�ű���������ת��)

struct ILuaVM;
struct ILuaPlugin : public IMSBase
{
	STDMETHOD_(LPCSTR , GetObjectName)() = 0;
	STDMETHOD_(LPVOID ,  GetObjectAddr)() = 0;
	STDMETHOD(LuaCallFuncton)(LPCSTR lpszFunctionName, LPCSTR lpParam, IStringA* pResult) = 0;
	STDMETHOD(BindToLua)(ILuaVM* pLuaVm) = 0;
};
MS_DEFINE_IID(ILuaPlugin, "{FC584B85-7EB2-4314-84D5-30D58F37DE31}");

struct ILuaVM : public IMSBase
{
	STDMETHOD(OpenVM)(LuaVm_OpenMask openMask= LuaVm_All) = 0;
	STDMETHOD(ClosetVM)() = 0;
	STDMETHOD_(LPVOID, GetLuaState)(void) = 0;
	STDMETHOD_(LPVOID, GetContext)() = 0;	//�����<luahost.h>���

	STDMETHOD(DoString)(LPCWSTR lpszScript) = 0;
	STDMETHOD(DoFile)(LPCWSTR lpszPath) = 0;

	STDMETHOD(Register)(ILuaPlugin* pLuaPlugin) = 0;
	STDMETHOD(GetObject)(LPCWSTR lpszName, ILuaPlugin**ppObj) = 0;
	STDMETHOD(Revoke)(LPCWSTR lpszName) = 0;

	STDMETHOD(GetRunningObjectTable)(IMSBase** pRot) = 0;

	//����NULLʧ��
	STDMETHOD_(LPCSTR, CallLuaFunction)(LPCSTR lpszFunctionName, LPCSTR lpszJsonParam) = 0;
};
MS_DEFINE_IID(ILuaVM, "{6538AD4C-7B13-4E86-A771-655AAE1CA21B}");



/*@
lua ��չ�⣬�ӵ�ǰĿ¼��ȡ*.luaex�����ļ�
<luaex>
	<class clsid="{3C15AD18-7CD4-44C2-91D0-CBD775068738}" progid="CLSID_LuaEx" name="CLSID_LuaEx"/>
</luaex>
*/




//����Ϊ����չlua�ű�
//��Ҫ������չһ����������
//�������ڲ�Ҳ������ ILuaVM::Registerǿ��ע��һ�������ʵ��
struct ILuaExtend : public IMSBase
{
	STDMETHOD(RegisterLuaEx)(ILuaVM* pLuaVm) = 0;
};
MS_DEFINE_IID(ILuaExtend, "{F0E8E313-6818-476C-A2AA-1AC040F4079C}");


/*@
	��Rot��ʹ�ã��������ڳ�ʼ���Ĺ����д���һЩ�ű�
*/
struct IluaHost : public IMSBase
{
	STDMETHOD(GetLuaVM)(ILuaVM** pVM) = 0;
};

MS_DEFINE_GUID(CLSID_LuaVM, 
	0x2a18a449, 0xf334, 0x4bd2, 0xb6, 0xbf, 0xc0, 0x2b, 0xcd, 0xc9, 0x3d, 0x3c);

MS_DEFINE_IID(IluaHost, "{FB47FDDC-FFD7-4664-B0BA-28530B5D1C34}");

// {4ED76816-8BE3-43E6-85FF-94AC1985D574}
MS_DEFINE_GUID(CLSID_LuaHost, 
	0x4ed76816, 0x8be3, 0x43e6, 0x85, 0xff, 0x94, 0xac, 0x19, 0x85, 0xd5, 0x74);

};};

// {2A18A449-F334-4BD2-B6BF-C02BCDC93D3C}


/*!
<class clsid="{4ED76816-8BE3-43E6-85FF-94AC1985D574}" name="CLSID_LuaHost" muststart="0">
	<param>
		<file>test.lua</file>
		<script>
			local i = 100 
		</script>
	</param>
</class>
*/