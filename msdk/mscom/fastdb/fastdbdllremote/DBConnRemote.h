#pragma once
#include <database/genericdb.h>


#include "DBRecordSetRemote.h"


enum FastDBCmd
{
	FDBC_Delete = 100	,
	FDBC_Insert			,
	FDBC_Update			,
	FDBC_CreateTable	,
};

enum FieldFlag
{
	FF_default			= 0,
	FF_hashed           = 1, /* field should be indexed usnig hash table */
	FF_indexed          = 2, /* field should be indexed using B-Tree */
	FF_cascade_delete   = 8,  /* perfrom cascade delete for for reference or array of reference fields */
	FF_autoincremented  = 16 /* field is assigned automaticall incremented value */
};

//ע��:Զ���������ݿ�,ʵ����ֻ��.����д����.
class CFastDBConnRemote : public IGenericDB, CUnknownImp
{
public:
	CFastDBConnRemote(void);
	CFastDBConnRemote(IN LPCTSTR lpDBName,IN LPCTSTR lpIP,IN INT iPort);
	~CFastDBConnRemote(void);

	UNKNOWN_IMP1( IGenericDB );

	HRESULT Init(IN LPCTSTR lpDBName,IN LPCTSTR lpIP,IN INT iPort);
	HRESULT Uninit();

public:

	STDMETHOD( Connect )( LPCTSTR szConn, LONG lOptions=0 );
	STDMETHOD( Close )( void ) ;

	STDMETHOD_( BOOL, IsTableExists )( LPCTSTR szTable );

	STDMETHOD( CompileStmt )( LPCTSTR szSQL, IGenericStmt** ppStmt );

	STDMETHOD( ExecuteDML )( LPCTSTR szSQL, LONG* pAffected=NULL );

	STDMETHOD( Execute )( LPCTSTR szSQL, IGenericRS** ppRecordset, DWORD* pFields=NULL, INT nFields=0 ) ;

	STDMETHOD( GetTypeDecl )( VARTYPE vt, LPTSTR szBuf, INT nBufLen ) ;


	STDMETHOD( BeginTrans )( VOID ) ;
	STDMETHOD( CommitTrans )( VOID ) ;
	STDMETHOD( RollbackTrans )( VOID ) ;


	STDMETHOD( GetTypeDecl )( RsFieldType fieldType, DWORD dwSize, LPTSTR szBuf, INT nBufLen ) ;

	STDMETHOD( GetAutoIDDecl )( LPTSTR szBuf, INT nBufLen ) ;


	STDMETHOD( AttachRealConn )( VOID ) ;


	STDMETHOD( DetachRealConn )( VOID ) ;


	STDMETHOD( GetTableFieldsCount )( LPCTSTR szTable,DWORD& dwCount ) ;


	STDMETHOD(GetTableFieldInfo) ( LPCTSTR szTable,DWORD index, LPTSTR FieldName, LPTSTR FieldType, DWORD& dwSize  ) ;

	//�жϴ������Ƿ���á��Է�������
	STDMETHOD_(BOOL, IsOpen)( VOID ) ;

	//------------------------------
	//�¼ӵĽӿ�
	
	//ע��,����Ϣ������.��νC/S˫����֪��һ���ı���Ϣ,�����໥����.
	STDMETHOD(RegistTableInfo)(LPCWSTR lpTableName, tag_Field* pFieldsSet,INT iFieldsCount) ;

	STDMETHOD(CreateTableRemote)(LPCWSTR lpTableName) ;
	//ע�����Ϣ������,ͬʱԶ�̽���.
	STDMETHOD(CreateTableRemote)(LPCWSTR lpTableName, tag_Field* pFieldsSet,INT iFieldsCount) ;

	STDMETHOD( InsertStatement)(LPCTSTR szSQL, LONG* pAffected=NULL );
	STDMETHOD( UpdateStatement)(LPCTSTR szSQL, LONG* pAffected=NULL );

	STDMETHOD( DeleteStatement)(LPCTSTR szSelectSql,LONG* pAffected=NULL );

	STDMETHOD( SelectStatement )(LPCTSTR szTableName, LPCTSTR szSQL, IGenericRS** ppRecordset, DWORD* pFields=NULL, INT nFields=0 ) ;
	
private:
	HRESULT QueryFieldsInfo(LPCTSTR szTable);
	HRESULT RegistTableInfo(LPCWSTR lpTableName, vector<tag_Field>& theFieldSet) ;

	HRESULT GetFindInfoOut(tag_Field* pField,LPTSTR FieldName, LPTSTR FieldType, DWORD& dwSize );

	HRESULT GetFieldSubSet(IN LPCWSTR szSql,IN VEC_FIELDSET& theWholeSet,OUT VEC_FIELDSET& SubSet);

private:
	

private:
	wstring m_strDBName;
	INT m_iSession;
	
	

	MAP_TABLE  m_theTableSet;
};
