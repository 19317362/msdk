#pragma once
#include <softeng/softengdef.h>
#include "SoftWareEnum.h"

#include ".\sqlite\CppSQLite3u.h"
#include <map>
#include <list>
#include <json/json.h>
struct ST_SOFT_RULE
{
	SOFTID	softid;
	DWORD	appid;				//��ʶ��ϵͳ�⣨0�������û��⣨�����û����Զ���ֵ��
	HKEY    hRootKey;			//����
	CString	strSubKey;			//�Ӽ�
	CString	strValueName;		//��ֵ����
	CString strExpendPath;		//����·������ע�����ȡ��·���������������
};
typedef std::list<ST_SOFT_RULE> SOFTRULES;

typedef std::map<DWORD, CString>	KV_MAP;
struct ST_SOFT_INFO
{
	BOOL    bIsExist;
	KV_MAP  kvValue;
};
typedef std::map<SOFTID, ST_SOFT_INFO> SOFTINFO_MAP;

class CSoftScan : public ISoftEdit
{
public:
	CSoftScan(ISoftCallback* pCallback);
	virtual ~CSoftScan(void);

	//ISoftScan

	STDMETHOD_(BOOL, Init)( /*WCHAR cRoot, LPCWSTR lpszBk*//*���ݵ�Ŀ¼*/);

	STDMETHOD_(BOOL, IsPe)();

	//ö�������, ͬʱ֧��ȫ���ֻö�ٴ��ڵ�
	STDMETHOD_(SOFTID, FindSoft)(BOOL bOnlyExist = TRUE, DWORD softLib = SOFTLIB_DEFAULE|SOFTLIB_USER|SOFTLIB_ENUM|SOFTLIB_MERGE);
	STDMETHOD_(SOFTID, FindNextSoft)();
	STDMETHOD_(BOOL,   IsExist)(SOFTID);

	STDMETHOD(ScanSoft)(LPCWSTR lpExpendPath, LPCWSTR lpRegPath, LPWSTR lpPath, DWORD dwPathSize);
	STDMETHOD(ScanSoftSubPath)(SOFTID softid, LPCWSTR lpSubPath, SOFTFILE_LIST& fileList);

	//��ȡ�����Ϣ, ������ơ����̵ȵ�
	STDMETHOD_(LPCWSTR, GetSoftInfo)(SOFTID, DWORD dwKey);
	STDMETHOD_(LPCWSTR, EnumSoftInfo)(SOFTID, DWORD& dwKey, DWORD& enumPos);
	
	STDMETHOD_(LPCWSTR, GetSoftClass)(DWORD softClass);

	STDMETHOD(GetSoftFile)(SOFTID softid, SOFTFILE_LIST& fileList);
	STDMETHOD(GetSoftPath)(SOFTID softid, SOFTPATH_LIST& pathList);

	///ISoftEdit/////////////////////////////////////////////////////////
	STDMETHOD_(SOFTID, FindSoftEx)(BOOL bOnlyExist, DWORD softLib, LPCWSTR lpCondition = NULL);
	STDMETHOD(GetCurMaxSoftid)(SOFTID* pdefSoftid, SOFTID* puserSoftid);
	STDMETHOD(DelSoft)(SOFTID softid);
	STDMETHOD_(BOOL, IsHave)(SOFTID softid);

	STDMETHOD(GetSoftRule)(SOFTID softid, SOFTRULE_LIST& ruleList);
	STDMETHOD(SetSoftRule)(SOFTID softid, SOFTRULE_LIST& ruleList);

	STDMETHOD(GetSoftAttribute)(SOFTID softid, SOFTATTRIBUTE_LIST& atttibuteList);
	STDMETHOD(SetSoftAttribute)(SOFTID softid, SOFTATTRIBUTE_LIST& atttibuteList);

	STDMETHOD(GetSoftFileRule)(SOFTID softid, SOFTFILE_LIST& fileList);
	STDMETHOD(SetSoftFileRule)(SOFTID softid, SOFTFILE_LIST& fileList);

	STDMETHOD(GetSoftPathRule)(SOFTID softid, SOFTPATH_LIST& pathList);
	STDMETHOD(SetSoftPathRule)(SOFTID softid, SOFTPATH_LIST& pathList);

	STDMETHOD(SetSoftWhite)( SOFT_WHITE_LIST& softWhite);
	STDMETHOD(GetSoftWhite)( SOFT_WHITE_LIST& softWhite);

	//ISfotClean
	//STDMETHOD(Clean)( DWORD dwSoftID, DWORD dwClean);

	//STDMETHOD(DelTemp)();
//	STDMETHOD(DelRun)();

private:
	CString		m_strModulePath;
	BOOL		m_bIsSvc;
	//�����Ϣ
	SOFTINFO_MAP	m_softInfo;
	SOFTINFO_MAP::const_iterator m_find;
	KV_MAP			m_softClass;

	SOFT_WHITE_LIST	m_softWiteList;

	HRESULT RefushSoftInfo(BOOL bOnlyExist, LPCWSTR lpCondition);
	HRESULT	RefushSoftClass();

	HRESULT GetSoftRules(SOFTRULES& rules, LPCWSTR lpCondition);
	HRESULT ScanSoft(CString &strPath, LPCTSTR lpExpendPath, HKEY hRootKey, LPCTSTR lpSubKey, LPCTSTR lpValueName);
	HRESULT ScanPath(CString &strPath, LPCTSTR lpExpendPath);
	HRESULT ScanService(CString &strPath, LPCTSTR lpSrvName);
	HRESULT GetSoftAttribute(SOFTID softid, KV_MAP &kvValue, DWORD softLib);

	HRESULT ParseSoftFile(LPCTSTR lpPath, LPCTSTR lpFile, SOFTFILE_LIST& fileList);
	HRESULT InsertFile(LPCTSTR lpFile, SOFTFILE_LIST& fileList);

	HRESULT ParseSoftPath(LPCTSTR lpPath, LPCTSTR lpBuf, int nType, SOFTPATH_LIST& pathList);
	HRESULT InsertPath(LPCTSTR lpPath, int nType, SOFTPATH_LIST& pathList);

	HRESULT EnumSoftInfo(BOOL bMerge);
	HRESULT CheckEnumSoft(const REG_SOFTWARE_INFO &softinfo, BOOL bMerge);

	HRESULT GetMaxSoftid(CppSQLite3DB& db, SOFTID &softid);

	STDMETHOD(SetSoftWhite)( DWORD softLib, SOFT_WHITE_LIST& softWhite);
	STDMETHOD(GetSoftWhite)( DWORD softLib, SOFT_WHITE_LIST& softWhite);


	//////////////////////////////////////////////////////////////////////////
	//������
	//BOOL DelSoftFiles(Json::Value& root,DWORD dwSoftID, SOFTFILE_LIST& files);
	//BOOL DelSoftPaths(Json::Value& root,DWORD dwSoftID, SOFTPATH_LIST& dirs);

	/////////////////////////////////////////////////////////////
	//���ݿ�ͨ�ò���
	BOOL  m_dbOK;
	BOOL  m_udbOK;
	CppSQLite3DB m_DB;		//ϵͳ��
	CppSQLite3DB m_uDB;		//�û���
	/////////////////////////////////////////////////////////////
	BOOL    Open(DWORD softLib);
	BOOL    OpenDB(DWORD softLib);
	VOID    CloseDB(DWORD softLib);
	HRESULT BeginTrans(DWORD softLib);
	HRESULT EndTrans(DWORD softLib, BOOL bFailedRollBack = TRUE) ;
	HRESULT RollBackTrans(DWORD softLib) ;

	//void    TableExists(); //�жϱ��Ƿ���ڣ�û�оʹ�����
	HRESULT execDML(const TCHAR* sql, DWORD softLib);

	//setting
	BOOL m_bDebug;

	ISoftCallback* m_pCallback;
};


