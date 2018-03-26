#include "StdAfx.h"
#include "DBRecordSetRemote.h"

#include <rscom/prophelpers.h>

CDBRecordSetRemote::CDBRecordSetRemote(void)
{
	m_iSession = cli_bad_address;
	m_iStatement = 0;
	m_iRecordCount = 0;
	m_iCurRecordIndex = 0;
}

CDBRecordSetRemote::CDBRecordSetRemote(
									   string&			strTableName
									   ,VEC_FIELDSET&	theFieldsSet
									   ,LPCTSTR			szSql
									   ,wstring&		strDBName
									   , INT			iSession
									   )
{
	m_iSession = cli_bad_address;
	m_iStatement = 0;
	m_iRecordCount = 0;
	m_iCurRecordIndex = 0;

	m_bIsGetCount = FALSE;
	m_nCount = 0;

	return;
}

CDBRecordSetRemote::~CDBRecordSetRemote(void)
{
	if(m_iStatement)
	{
		int nRet = cli_commit(m_iSession);
		FAILD_LOGONLY(cli_ok == nRet);
		nRet = cli_free(m_iStatement);
		FAILD_LOGONLY(cli_ok == nRet);
	}
	
	HRESULT hr = UnitCurRecordMem();
	FAILD_LOGONLY(SUCCEEDED(hr));


	return;
}

HRESULT CDBRecordSetRemote::CheckIsCount(const string& strSql)
{
	m_bIsGetCount = FALSE;
	//�ж��Ƿ���select count ( * )  from table_name where a = 0��䡣
	Helper::CTransStr theTrans;

	string strLeftPart;
	BOOL bRet = theTrans.CutStrTail(strSql.c_str(),"from",strLeftPart);
	FAILEXIT_FAIL(bRet);

	string strFind;
	bRet = theTrans.CutStrHead(strLeftPart.c_str(),"count",strFind)	;
	if(true == strFind.empty())
		return S_OK;

	bRet = theTrans.CutStrHead(strFind.c_str(),"(",strFind)	;
	if(true == strFind.empty())
		return S_OK;

	bRet = theTrans.CutStrHead(strFind.c_str(),"*",strFind)	;
	if(true == strFind.empty())
		return S_OK;

	bRet = theTrans.FindCharInStr(strFind.c_str(),')');
	if(FALSE == bRet)
		return S_OK;

	m_bIsGetCount = TRUE;

	return S_OK;
}

HRESULT CDBRecordSetRemote::Init(
			 string&		strTableName
			 ,VEC_FIELDSET&	theFieldsSet
			 ,LPCTSTR		szSql
			 ,wstring&		strDBName
			 , INT			iSession
			 )
{
	FAILEXIT_FAIL(strTableName.size());
	FAILEXIT_FAIL(theFieldsSet.size());
	FAILEXIT_FAIL(szSql);
	FAILEXIT_FAIL(0 < iSession);
	m_iSession = iSession;
	
	Helper::CTransStr theTrans;
	string strSql = theTrans.MYW2A(szSql);

	/*OutputDebugStringA(strSql.c_str());
	OutputDebugStringA("\n");*/

	//�ж��Ƿ���count(*) ���
	CheckIsCount(strSql);

	//�ѷ��ֶβ�ѯ��sql�����select * from table_ttt where a = 30 ;���������.
	string strSqlRightPart ;
	BOOL bRet =  theTrans.CutStrHead(strSql.c_str(),"from",strSqlRightPart);
	FAILEXIT_FAIL(bRet);

	string strTempSql = "select * ";
	strTempSql += strSqlRightPart;

	//��ѯ���
	m_iStatement = cli_statement(m_iSession, strTempSql.c_str());
	FAILEXIT_FAIL(m_iStatement >= 0);

	//Ϊÿ���ֶΰ󶨽����õı���.
	HRESULT hr = InitCurRecordMem(theFieldsSet);
	FAILEXIT_FAIL(SUCCEEDED(hr));

	m_iRecordCount = cli_fetch(m_iStatement, cli_view_only);
	//FAILEXIT_FAIL_N(m_iRecordCount);
	
	//����ǲ�ѯ������֤�ɹ�.
	if(TRUE == m_bIsGetCount)
	{
		/*wostringstream oo ;
		oo << L"count:"<<m_iRecordCount<<L"\n";
		OutputDebugStringW(oo.str().c_str());*/

		m_nCount = m_iRecordCount;
		return S_OK;
	}

	FAILEXIT_FAIL_N(m_iRecordCount);
	
	hr = MoveFirst();
	FAILEXIT_FAIL(SUCCEEDED(hr));

	return S_OK;
}

HRESULT CDBRecordSetRemote::UnitCurRecordMem()
{
	for(UINT i = 0; i < m_vCurRecord.size(); ++i)
	{
		if(NULL != m_vCurRecord[i])
		{
			CHAR* pTemp = m_vCurRecord[i];
			delete [] pTemp;
		}
	}
	m_vCurRecord.clear();

	return S_OK;
}

HRESULT CDBRecordSetRemote::InitCurRecordMem(VEC_FIELDSET&	theFieldsSet)
{
	m_vCurRecord.clear();
	m_theFieldsSet.clear();
	for(UINT i = 0; i < theFieldsSet.size(); ++i)
	{
		tag_Field* pItem = &(theFieldsSet[i]);
		FAILEXIT_FAIL(NULL != pItem);

		m_theFieldsSet.push_back(*pItem);

		INT iSize = pItem->GetFieldLenByType();
		FAILEXIT_FAIL(iSize);
		INT iFieldType = pItem->GetCliTypeByVTType();
		INT iByteLen = pItem->GetFieldByteLenByVTType();
		FAILEXIT_FAIL(iByteLen);

		Helper::CTransStr theTrans;
		string strFieldName = theTrans.MYW2A(pItem->szFieldName);
		
		CHAR* pBuff = new CHAR[iByteLen];
		memset(pBuff,0,iByteLen);
		FAILEXIT_FAIL(pBuff);
		m_vCurRecord.push_back(pBuff);

		int nRet = cli_column(
			m_iStatement
			,strFieldName.c_str()
			,iFieldType
			,&iByteLen
			,pBuff
			);
		FAILEXIT_FAIL(cli_ok == nRet);

	}

	return S_OK;
}

HRESULT CDBRecordSetRemote::GetRecord ( IProperty2** ppRecord ) 
{
	if(TRUE == m_bIsGetCount)
	{
		//�����Ҫȡ����.��Cursor�����޹�,�൱��ֻ��һ����¼.
		//����ֱ�Ӻϳɼ�¼��
		//=================================
		UTIL::com_ptr<IProperty2> record;
		HRESULT hr = CPropertySet::CreateProp( &record.m_p ) ;
		FAILEXIT_FAIL(SUCCEEDED(hr));

		CPropVar prop; 
		prop = m_nCount;

		DWORD dwID = 0;
		hr = record->SetProperty( dwID, &prop );
		FAILEXIT_FALSE(SUCCEEDED(hr));
		*ppRecord = record.m_p;
		record->AddRef();

		return S_OK;
	}


	UTIL::com_ptr<IProperty2> record;
	HRESULT hr = CPropertySet::CreateProp( &record.m_p ) ;
	FAILEXIT_FAIL(SUCCEEDED(hr));

	for( UINT i = 0; i < m_vCurRecord.size(); ++i )
	{
		CPropVar prop;
		CHAR* pBuff = m_vCurRecord[i];
			
		//��pBuff����_variant_t ֻҪȡ���������������;Ϳ�����.
		BOOL bRet = m_theFieldsSet[i].GetVar(pBuff,prop);	
		FAILEXIT_FAIL(bRet);

		DWORD dwID = i;
		hr = record->SetProperty( dwID, &prop );
		FAILEXIT_FAIL(SUCCEEDED(hr));
	}
	*ppRecord = record.m_p;
	record->AddRef();

	return S_OK;
}


HRESULT CDBRecordSetRemote:: GetScale ( PROPVARIANT* pScale ) 
{
	return E_NOTIMPL;
}

HRESULT CDBRecordSetRemote:: MoveFirst ( VOID ) 
{
	if(TRUE == m_bIsGetCount)
	{
		//�����Ҫȡ����.��Cursor�����޹�,�൱��ֻ��һ����¼.
		//=================================
		m_iCurRecordIndex = 0;
		return S_OK;
	}
	int nRet = cli_get_first(m_iStatement);
	FAILEXIT_FAIL(cli_ok == nRet);
	m_iCurRecordIndex = 0;
	return S_OK;
}


HRESULT CDBRecordSetRemote:: MoveNext ( VOID ) 
{	
	if(TRUE == m_bIsGetCount)
	{
		//�����Ҫȡ����.��Cursor�����޹�,�൱��ֻ��һ����¼.
		//=================================
		++m_iCurRecordIndex;
		return S_OK;
	}

	int nRet = cli_get_next(m_iStatement);
	++m_iCurRecordIndex;
	FAILEXIT_FAIL(cli_ok == nRet);	
	return S_OK;
}

BOOL   CDBRecordSetRemote::IsEOF ( VOID ) 
{
	if(TRUE == m_bIsGetCount)
	{
		//�����Ҫȡ����.��Ϊȡcount,�൱��ֻ��һ����¼.
		//=================================
		if(m_iCurRecordIndex >= 1)
			return TRUE;
		return FALSE;
	}

    if(1 == m_iRecordCount)
    {
        if(m_iCurRecordIndex  >= m_iRecordCount)
            return TRUE;            
        return FALSE;
    }
    if(m_iCurRecordIndex + 1 > m_iRecordCount)
        return TRUE;
    return FALSE;
}
