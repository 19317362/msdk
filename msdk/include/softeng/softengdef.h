#pragma once
#include <util/CRuleBuffer.h>

typedef DWORD SOFTID;
#define INVALID_SOFTID	0

#define SOFTLIB_DEFAULE		0x0001	//Ĭ�Ͽ�
#define SOFTLIB_USER		0x0002	//�û���
#define SOFTLIB_ENUM		0x0004	//ɨ��

#define SOFTLIB_MERGE		0x10000	//ʹ��SOFTLIB_ENUM��Ч��ö�ٽ����֮ǰ��������ƺϲ�

#define SOFTID_USER_BASE	1000000
#define SOFTID_ENUM_MASK	0x80000000

#pragma pack(push, 4)

typedef struct tag_softfile 
{
	WCHAR   strFile[MAX_PATH*2];
}ST_SOFTFILE;
typedef CRuleBuffer<ST_SOFTFILE> SOFTFILE_LIST;

enum
{
	PATHTYPE_FILE = 0,		//�ļ�
	PATHTYPE_COM,			//COM

	PATHTYPE_NOFILE = 100,
	PATHTYPE_REG_KEY,		//ע�����
	PATHTYPE_REG_VALUE,		//ע���ֵ
};
typedef struct tag_softpath 
{
	int		nType;
	WCHAR   strPath[MAX_PATH*2];
}ST_SOFTPATH;
typedef CRuleBuffer<ST_SOFTPATH> SOFTPATH_LIST;

enum
{
	//���õľ߱�������ֵ
	SOFT_APPID = 0,	//��ʶ��ϵͳ�⣨0�������û��⣨�����û����Զ���ֵ��
	SOFT_PATH,		//·��
	SOFT_CLASS,		//����
	SOFT_NAME,		//����
	SOFT_COMPANY,	//��˾
	SOFT_TYPE,		//
	SOFT_VERSION,	//�汾
	
	//һ���������õ�
	SOFT_VERREG,	//�����汾��ע���
	SOFT_INSREG,	//�����Ƿ�װ��ע���


	

	// ���ص�ַ
	SOFT_DLURL,




	SOFT_MORE = 100,
	SOFT_UNINSTALLSTR,	//ж��·������·��������������
	SOFT_REG_UNINSTALL,	//ע���ж��·��
	SOFT_VER,
	SOFT_SIZE,

	
	

	SOFT_WHITE,	//��������ɨ���ʱ��������


	SOFT_CLEAN_LEAVE,	//������
	
};


//���������
enum 
{
	SOFT_WHITE_SOFT_NAME_WHOLEWORD,		//ͨ�����������ȫƥ��
	SOFT_WHITE_SOFT_NAME,				//ͨ�����������ȫƥ��

	SOFT_WHITE_SOFT_COMPANY_WHOLEWORD,	//ͨ����˾����ȫƥ��
	SOFT_WHITE_SOFT_COMPANY,			//ͨ����˾������ȫƥ��
};


#pragma pack()


//ɨ��ص�
interface ISoftCallback
{
	virtual VOID OnSoftFind( DWORD dwSoftID) = 0;	//һ�������ɨ�赽
	virtual VOID OnSoftClean( DWORD dwSoftID) = 0;		//һ�����������
};

interface ISoftScan
{
	STDMETHOD_(BOOL, Init)( /*WCHAR cRoot, LPCWSTR lpszBk*//*���ݵ�Ŀ¼*/) = 0;

	STDMETHOD_(BOOL, IsPe)() = 0;

	//ö�������, ͬʱ֧��ȫ���ֻö�ٴ��ڵ�
	STDMETHOD_(SOFTID, FindSoft)(BOOL bOnlyExist = TRUE, DWORD softLib = SOFTLIB_DEFAULE|SOFTLIB_USER|SOFTLIB_ENUM|SOFTLIB_MERGE) = 0;
	STDMETHOD_(SOFTID, FindNextSoft)() = 0;
	STDMETHOD_(BOOL,   IsExist)(SOFTID) = 0;

	STDMETHOD(ScanSoft)(LPCWSTR lpExpendPath, LPCWSTR lpRegPath, OUT LPWSTR lpPath, DWORD dwPathSize) = 0;
	STDMETHOD(ScanSoftSubPath)(SOFTID softid, LPCWSTR lpSubPath, SOFTFILE_LIST& fileList) = 0;

	//��ȡ�����Ϣ, ������ơ����̵ȵ�
	STDMETHOD_(LPCWSTR, GetSoftInfo)(SOFTID softid, DWORD dwKey) = 0;
	STDMETHOD_(LPCWSTR, EnumSoftInfo)(SOFTID softid, DWORD& dwKey, DWORD& enumPos) = 0; //��һ����ʼenumPos=-1
	STDMETHOD_(LPCWSTR, GetSoftClass)(DWORD softClass) = 0;

	//��ȡĳ�����صĽ����б���Ϣ
	STDMETHOD(GetSoftFile)(SOFTID softid, SOFTFILE_LIST& fileList) = 0;

	//�����װ��Ϣ������ǿ��ж��ɾ���ļ���ע���
	STDMETHOD(GetSoftPath)(SOFTID softid, SOFTPATH_LIST& pathList) = 0;

	/*
	STDMETHOD(Clean)( DWORD dwSoftID, DWORD dwClean) = 0;

	STDMETHOD(DelTemp)() = 0;
	STDMETHOD(DelRun)()	= 0;
	*/

};
MS_DEFINE_IID(ISoftScan, "{407A7A84-A3E2-4c50-B145-9E0245C96C3E}");

typedef struct tag_softrule 
{
	WCHAR   strPath[MAX_PATH*2];
	WCHAR   strRegPath[MAX_PATH*2];
	DWORD	dwAppid;
}ST_SOFTRULE;
typedef CRuleBuffer<ST_SOFTRULE> SOFTRULE_LIST;

typedef struct tag_softAttribute
{
	DWORD dwKey;
	WCHAR strValue[1024];
}ST_SOFTATTRIBUTE;
typedef CRuleBuffer<ST_SOFTATTRIBUTE> SOFTATTRIBUTE_LIST;


typedef struct tag_softWhite
{
	DWORD whiteAttr;
	WCHAR strValue[MAX_PATH];
}ST_SOFT_WHITE;

typedef CRuleBuffer<ST_SOFT_WHITE> SOFT_WHITE_LIST;

interface ISoftEdit : public ISoftScan
{
	STDMETHOD_(SOFTID, FindSoftEx)(BOOL bOnlyExist, DWORD softLib, LPCWSTR lpCondition = NULL) = 0;
	STDMETHOD(GetCurMaxSoftid)(SOFTID* pdefSoftid, SOFTID* puserSoftid) = 0;
	STDMETHOD_(BOOL, IsHave)(SOFTID softid) = 0;
	STDMETHOD(DelSoft)(SOFTID softid) = 0;

	STDMETHOD(GetSoftRule)(SOFTID softid, SOFTRULE_LIST& ruleList) = 0;
	STDMETHOD(SetSoftRule)(SOFTID softid, SOFTRULE_LIST& ruleList) = 0;

	STDMETHOD(GetSoftAttribute)(SOFTID softid, SOFTATTRIBUTE_LIST& atttibuteList) = 0;
	STDMETHOD(SetSoftAttribute)(SOFTID softid, SOFTATTRIBUTE_LIST& atttibuteList) = 0;

	STDMETHOD(GetSoftFileRule)(SOFTID softid, SOFTFILE_LIST& fileList) = 0;
	STDMETHOD(SetSoftFileRule)(SOFTID softid, SOFTFILE_LIST& fileList) = 0;

	STDMETHOD(GetSoftPathRule)(SOFTID softid, SOFTPATH_LIST& pathList) = 0;
	STDMETHOD(SetSoftPathRule)(SOFTID softid, SOFTPATH_LIST& pathList) = 0;


	STDMETHOD(SetSoftWhite)( SOFT_WHITE_LIST& softWhite) = 0;
	STDMETHOD(GetSoftWhite)( SOFT_WHITE_LIST& softWhite) = 0;
};
MS_DEFINE_IID(ISoftEdit, "{9C0E71D4-C9DB-4db1-AED8-2C134A35515E}");


enum
{
	SOFT_CLEAN_ALL,

	SOFT_CLEAN_BK,	//��������
};


extern "C"
{
	typedef ISoftScan*  (WINAPI *pCreate)(ISoftCallback* pCallback);
	ISoftScan* WINAPI CreateSoftEng(ISoftCallback* pCallback);

	typedef VOID( *pDestory)(ISoftScan* pSoftScan);
	VOID WINAPI DestorySoftEng( ISoftScan* pSoftScan);
};

