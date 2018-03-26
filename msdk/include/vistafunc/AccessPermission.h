
#pragma once

#include <AccCtrl.h>
#include <AclAPI.h>
#include <Lmcons.h>
class CAccessPermission  
{
public:

	CAccessPermission()
	{

	}
	~CAccessPermission()
	{

	}

	// ʹ��LocalFree���ɵ�PSID
	PSID GetSidFromUserName(LPCTSTR lpUserName)
	{
		SID_NAME_USE snu;

		TCHAR	szDomain[_MAX_PATH];
		DWORD	chDomain = _MAX_PATH;

		if(lpUserName)
		{
			DWORD cbSid = 0;
			DWORD cbDomainName = 0;

			LookupAccountName(NULL, lpUserName, NULL, &cbSid, szDomain, &chDomain, &snu);
			// ���ݸ�ϵͳ���õ���������̫С
			if(ERROR_INSUFFICIENT_BUFFER == GetLastError() && cbSid)
			{
				PSID lpSid = LocalAlloc(GPTR, cbSid);

				if(lpSid)
				{
					if( LookupAccountName(NULL, lpUserName, lpSid, &cbSid, szDomain, &chDomain, &snu) )
					{
						return lpSid;
					}
					LocalFree(lpSid);
				}
			}
		}

		return NULL;
	}

	// ��һ��SIDȡ���û���
	BOOL GetUserNameFromSid(PSID pUserSid, LPTSTR szUserName, LPDWORD pcbLen)
	{
		// sanity checks and default value
		if (pUserSid == NULL || szUserName == NULL || pcbLen == NULL || *pcbLen == 0)
			return FALSE;

		SID_NAME_USE   snu;
		TCHAR          szDomain[_MAX_PATH];
		DWORD          chDomain = _MAX_PATH;
		PDWORD         pcchDomain = &chDomain;

		// Retrieve user name and domain name based on user's SID.
		return ::LookupAccountSid(NULL, pUserSid, szUserName, pcbLen, szDomain, pcchDomain, &snu);
	}

	// ע���-�������Ȩ��
	BOOL Reg_GrantAccess(HKEY hKey, LPCTSTR lpSubkey, LPCTSTR lpTrusteeName, DWORD AccessPermissions=KEY_ALL_ACCESS)
	{
		LPCTSTR lpRootkey = _ConvRegRootkey(hKey);
		if(lpRootkey)
		{
			TCHAR szObjectName[1024];
			TCHAR szUserName[UNLEN + 1];

			if( lpTrusteeName == NULL )
			{
				DWORD dwSize = sizeof(szUserName);

				GetUserName(szUserName, &dwSize);
			}
			else
				_tcscpy( szUserName, lpTrusteeName );

			_sntprintf(szObjectName, sizeof(szObjectName), _T("%s\\%s"), lpRootkey, lpSubkey);
			return _GrantAccess(szObjectName, SE_REGISTRY_KEY, szUserName, AccessPermissions);
		}

		return FALSE;
	}

	// ע���-���þܾ�Ȩ��
	BOOL Reg_DenyAccess(HKEY hKey, LPCTSTR lpSubkey, LPCTSTR lpTrusteeName, DWORD AccessPermissions=KEY_SET_VALUE)
	{
		LPCTSTR lpRootkey = _ConvRegRootkey(hKey);
		if(lpRootkey)
		{
			TCHAR szObjectName[1024];
			TCHAR szUserName[UNLEN + 1];

			if( lpTrusteeName == NULL )
			{
				DWORD dwSize = sizeof(szUserName);

				GetUserName(szUserName, &dwSize);
			}
			else
				_tcscpy( szUserName, lpTrusteeName );

			_sntprintf(szObjectName, sizeof(szObjectName), _T("%s\\%s"), lpRootkey, lpSubkey);
			return _DenyAccess(szObjectName, SE_REGISTRY_KEY, szUserName, AccessPermissions);
		}

		return FALSE;
	}

	// �ļ���Ŀ¼�������Ȩ��
	BOOL File_GrantAccess(LPCTSTR lpPath, LPCTSTR lpTrusteeName, DWORD AccessPermissions=FILE_ALL_ACCESS)
	{
		TCHAR szUserName[UNLEN + 1];

		if( lpTrusteeName == NULL )
		{
			DWORD dwSize = sizeof(szUserName);

			GetUserName(szUserName, &dwSize);
		}
		else
			_tcscpy( szUserName, lpTrusteeName );

		return _GrantAccess(lpPath, SE_FILE_OBJECT, szUserName, AccessPermissions);
	}

	// �ļ���Ŀ¼���þܾ�Ȩ��
	BOOL File_DenyAccess(LPCTSTR lpPath, LPCTSTR lpTrusteeName, DWORD AccessPermissions=FILE_GENERIC_WRITE)
	{
		TCHAR szUserName[UNLEN + 1];

		if( lpTrusteeName == NULL )
		{
			DWORD dwSize = sizeof(szUserName);

			GetUserName(szUserName, &dwSize);
		}
		else
			_tcscpy( szUserName, lpTrusteeName );

		return _DenyAccess(lpPath, SE_FILE_OBJECT, szUserName, AccessPermissions);
	}

protected:
	// �������ͻ�����ɾ��ACE
	void _DeleteAce(PACL pAcl, ACCESS_MODE AccessMode, LPCTSTR lpTrusteeName)
	{
		if(pAcl)
		{
			ULONG lCount = 0;
			ULONG lDels = 0;
			PEXPLICIT_ACCESS lpea;
			PSID  lpSid = NULL;

			//		if(lpTrusteeName)
			//			lpSid = GetSidFromUserName(lpTrusteeName);

			if( ERROR_SUCCESS == GetExplicitEntriesFromAcl(pAcl, &lCount, &lpea) )
			{
				for(ULONG i=0; i<lCount; ++i)
				{
					if(lpea[i].grfAccessMode == AccessMode )
					{
#if 1
						DeleteAce(pAcl, i-lDels);
						lDels ++;
#else
						if(lpSid)
						{
							if(lpea[i].Trustee.TrusteeForm == TRUSTEE_IS_SID )
							{
								if( EqualSid(GetTrusteeName(&lpea[i].Trustee), lpSid) )
								{
									DeleteAce(pAcl, i-lDels);
									lDels ++;
								}
							}
							else
							{
								// ������������ֵ
								ATLASSERT(FALSE);
							}
						}
						else
						{
							DeleteAce(pAcl, i-lDels);
							lDels ++;
						}
#endif
					}
				}
			}

			if(lpSid)
				LocalFree(lpSid);
		}
	}

	// ����Ȩ��
	BOOL _SetAccess(LPCTSTR pObjectName, SE_OBJECT_TYPE ObjectType, ACCESS_MODE accessMode, LPCTSTR lpTrusteeName, DWORD AccessPermissions)
	{
		if( pObjectName == NULL ||
			lpTrusteeName == NULL)
			return FALSE;

		BOOL bRet = FALSE;
		PSECURITY_DESCRIPTOR pSD = NULL;
		PACL pOldDacl = NULL; 

		// ȡ��DACL
		if( ERROR_SUCCESS == GetNamedSecurityInfo(
			(LPTSTR)pObjectName, ObjectType, DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDacl, NULL, &pSD) )
		{
			if( accessMode == SET_ACCESS ||
				accessMode == GRANT_ACCESS)
			{
				// ɾ������ģʽΪDENY_ACCESS������ΪlpTrusteeName��Ȩ��
				_DeleteAce(pOldDacl, DENY_ACCESS, lpTrusteeName);
			}

			// ����һ���µ�ACE
			PACL pNewDacl = NULL;
			EXPLICIT_ACCESS ea;
			ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));

			BuildExplicitAccessWithName(&ea, (LPTSTR)lpTrusteeName, AccessPermissions, accessMode, SUB_CONTAINERS_AND_OBJECTS_INHERIT);

			// ���µ�ACE����DACL
			if( ERROR_SUCCESS == SetEntriesInAcl(1, &ea, pOldDacl, &pNewDacl) )
			{
				// ����DACL
				bRet = (ERROR_SUCCESS == SetNamedSecurityInfo(
					(LPTSTR)pObjectName, ObjectType, DACL_SECURITY_INFORMATION, NULL, NULL, pNewDacl, NULL) );

				if(pNewDacl)
					LocalFree(pNewDacl);
			}

			if(pSD)
				LocalFree(pSD);
		}

		return bRet;
	}

	// ���þܾ�Ȩ��
	inline BOOL _DenyAccess(LPCTSTR pObjectName, SE_OBJECT_TYPE ObjectType, LPCTSTR lpTrusteeName, DWORD AccessPermissions)
	{
		return _SetAccess(pObjectName, ObjectType, DENY_ACCESS, lpTrusteeName, AccessPermissions);
	}

	// �������Ȩ��
	inline BOOL _GrantAccess(LPCTSTR pObjectName, SE_OBJECT_TYPE ObjectType, LPCTSTR lpTrusteeName, DWORD AccessPermissions)
	{
		return _SetAccess(pObjectName, ObjectType, SET_ACCESS, lpTrusteeName, AccessPermissions);
	}

	inline LPCTSTR _ConvRegRootkey(HKEY hRootkey)
	{
		LPCTSTR lpRootkey = NULL;

		switch((LONG)hRootkey)
		{
		case HKEY_CLASSES_ROOT:
			lpRootkey = _T("CLASSES_ROOT");
			break;

		case HKEY_CURRENT_USER:
			lpRootkey = _T("CURRENT_USER");
			break;

		case HKEY_LOCAL_MACHINE:
			lpRootkey = _T("MACHINE");
			break;

		case HKEY_USERS:
			lpRootkey = _T("USERS");
			break;

		default:
			break;
		}

		return lpRootkey;
	}
};
