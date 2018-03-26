#if !defined(AFX_PROCESSTOKEN_H__5B5CF775_78D5_4FD8_949E_63A988A598B8__INCLUDED_)
#define AFX_PROCESSTOKEN_H__5B5CF775_78D5_4FD8_949E_63A988A598B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace msproc
{


//ģ���������
class CProcessToken
{

public:
	CProcessToken():
		m_hAdvapi32Module(NULL),
		m_pOpenProcessToken(NULL),
		m_pImpersonateLoggedOnUser(NULL),
		m_pRevertToSelf(NULL)
	{
		m_hAdvapi32Module = LoadLibrary(_T("Advapi32.dll"));
		if(m_hAdvapi32Module)
		{
			m_pOpenProcessToken = (POPENPROCESSTOKEN)GetProcAddress(m_hAdvapi32Module, "OpenProcessToken");
			m_pImpersonateLoggedOnUser = (PIMPERSONATELOGGEDONUSER)GetProcAddress(m_hAdvapi32Module, "ImpersonateLoggedOnUser");
			m_pRevertToSelf = (PREVERTTOSELF)GetProcAddress(m_hAdvapi32Module, "RevertToSelf");
		}
	}

	virtual ~CProcessToken()
	{
		if (m_hAdvapi32Module)
		{
			FreeLibrary(m_hAdvapi32Module);
		}
		m_hAdvapi32Module = NULL;
		m_hAdvapi32Module = NULL;
		m_pOpenProcessToken = NULL;
		m_pImpersonateLoggedOnUser = NULL;
		m_pRevertToSelf = NULL;
	}

	/* ����ǰ�̵߳�����Ȩ�޵�����ָ�����̵�����Ȩ��\n
	 *	@param[in] dwProcessID ����id
	 *	@return �ɹ�����TRUE�����򷵻�FLASE
	 */
	BOOL SetThreadTokenAsProcessUser(DWORD dwProcessID)
	{
		BOOL bRet = FALSE;

		if(	m_pOpenProcessToken && m_pImpersonateLoggedOnUser && dwProcessID >= 1)
		{
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, TRUE, dwProcessID);
			if(NULL == hProcess)
				return bRet;

			HANDLE hToken = NULL;
			if(m_pOpenProcessToken(hProcess, TOKEN_QUERY|TOKEN_DUPLICATE, &hToken))
			{
				bRet = m_pImpersonateLoggedOnUser(hToken);
				CloseHandle(hToken);
			}

			CloseHandle(hProcess);
		}

		return bRet;
	}

	/* ����ǰ�̵߳�����Ȩ�޻ָ���ԭ�ȵ�Ȩ��\n
	 *	@return �ɹ�����TRUE�����򷵻�FLASE
	 */
	BOOL RevertToSelf()
	{
		if (m_pRevertToSelf)
			return m_pRevertToSelf();
		else
			return FALSE;
	}

private:
	typedef BOOL (WINAPI* POPENPROCESSTOKEN)(HANDLE, DWORD, PHANDLE);
	typedef BOOL (WINAPI* PIMPERSONATELOGGEDONUSER)(HANDLE);
	typedef BOOL (WINAPI* PREVERTTOSELF)();

	/** Advapi32��ʵ��
	*/
	HMODULE						m_hAdvapi32Module;

	/** ��Ҫ����
	*/
	POPENPROCESSTOKEN			m_pOpenProcessToken;
	PIMPERSONATELOGGEDONUSER	m_pImpersonateLoggedOnUser;
	PREVERTTOSELF				m_pRevertToSelf;
};

};//msproc
