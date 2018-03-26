#pragma once


#include "Helper.h"



//ע:�޳̲�ѯ���ݿ�,ʵ����ֻ��,����д.
class CDBRecordSetRemote : public IGenericRS,public CUnknownImp
{

public:
	CDBRecordSetRemote(void);
	CDBRecordSetRemote(
		string&			strTable
		,VEC_FIELDSET&	theFieldsSet
		,LPCTSTR		szSql
		,wstring&		strDBName
		, INT			iSession
		);
	~CDBRecordSetRemote(void);

	UNKNOWN_IMP1(IGenericRS);

	HRESULT Init(
		string&			strTable
		,VEC_FIELDSET&	theFieldsSet
		,LPCTSTR		szSql
		,wstring&		strDBName
		, INT			iSession
		);

public:

	STDMETHOD( GetRecord )( IProperty2** ppRecord ) ;


	STDMETHOD( GetScale )( PROPVARIANT* pScale ) ;

	STDMETHOD( MoveFirst )( VOID ) ;


	STDMETHOD( MoveNext )( VOID ) ;

	STDMETHOD_( BOOL, IsEOF )( VOID ) ;

private:
	

	HRESULT InitCurRecordMem(VEC_FIELDSET&	theFieldsSet);

	HRESULT UnitCurRecordMem();

private:
	HRESULT CheckIsCount(const string& strSql);


private:
	wstring m_strDBName;
	INT m_iSession;
	INT m_iStatement;

	INT m_iRecordCount;
	INT m_iCurRecordIndex;

	VEC_FIELDSET	m_theFieldsSet;
	std::vector<CHAR*> m_vCurRecord;

	//Ϊ��֧��count��䣬���˱�����
	BOOL			m_bIsGetCount;
	INT 			m_nCount;


};
