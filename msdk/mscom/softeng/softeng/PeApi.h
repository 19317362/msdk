#pragma once

//PE�г��õ�API
class CPeApi
{
public:
	CPeApi(void);
	~CPeApi(void);

public:	// static ����

	static BOOL IsPe();		//�ж��Ƿ�ΪPEϵͳ
	static BOOL IsWindows64();	//Ŀ��ϵͳ�Ƿ�ΪX86
	static BOOL IsPeVistaLatter();	//PE�Ƿ�Ϊvista֮��İ汾
	static BOOL IsWindowsVistaLatter();	//Ŀ��ϵͳ�Ƿ�ΪVista֮��汾

	//Ŀ¼����
	static BOOL		PathIsSystemFolder(LPCTSTR lpszPath);	//�ж��Ƿ�ΪϵͳĿ¼
	static CString	GetRealPath( LPCTSTR lpszPath);	//��ȡʵ�ʵ�·��


	//ע������
	static CString GetRealRegPath(LPCTSTR lpszRegPath);	//��ȡʵ�ʵ�ע���·��
	static HKEY GetRootKey( HKEY hRoot );
	static CString GetFullRegPath( HKEY hRoot, LPCTSTR lpszSub);
	

private:
	static CString GetPeRealRegPath(LPCTSTR lpszRegPath);
	static CString GetLocalRealRegPath(LPCTSTR lpszRegPath);
};

class CPERegLoader
{
public:
	CPERegLoader(LPCTSTR szFilePath, LPCTSTR szName)
	{
		m_bLoadResult = FALSE;
		LONG lRet = RegLoadKey(HKEY_LOCAL_MACHINE, szName, szFilePath);
		if( ERROR_SUCCESS == lRet ) 
		{
			m_strName = szName;
			m_bLoadResult = TRUE;
		}
	}

	~CPERegLoader()
	{
		if( m_bLoadResult ) 
		{
			RegUnLoadKey(HKEY_LOCAL_MACHINE, m_strName);
		}
	}


	BOOL GetLoadStatus() {return m_bLoadResult;}
private:
	CString m_strName;
	BOOL m_bLoadResult;
};
