
#pragma once

namespace msdk{;
namespace mscom{;
//--------------------------------------------------------------------
//	IGenericMS
//	ͨ�ü�¼���ӿ�

//  Demo( ʹ��prophelpers.h�еİ����� ):
//  
//  IGenericDB* pDB; //   �Ѿ��򿪵�����
//  
//  UTIL::com_ptr<IGenericMS>	pRS;
//  RFAILED( pDB->Execute( _T("SELECT * FROM Table"), &pRS.m_p ) );
//  
//  while( !pRS->IsEOF() )
//  {
//  	UTIL::com_ptr<IProperty2>	prop;
//  	RFAILED( pRS->GetRecord( &prop.m_p ) );
//  	CPropSet propSet( prop );
//  
//  	int nFiled1 = propSet[0];
//  	LPTSTR szField2 = propSet[1];
//  
//  	RFAILED( pRS->MoveNext() );
//  }
//---
//  while( !pRS->IsEOF() )
//  {
//  	UTIL::com_ptr<IPropertyStr>	prop;
//  	RFAILED( pRS->GetRecord( &prop.m_p ) );
//  	CPropSet propStrSet( prop );
//  
//  	int nFiled1 = propSet["id"];
//  	LPTSTR szField2 = propSet["url"];
//  
//  	RFAILED( pRS->MoveNext() );
//  }
//--------------------------------------------------------------------
interface IGenericMS : public IUnknown
{
	//************************************
	// Method:    GetRecord ��ȡ��ǰλ�õļ�¼
	// Returns:   ����ɹ���ȡ��¼������S_OK�����򷵻�E_FAIL
	// Parameter: ppRecord[out] �����ǰλ�õļ�¼�������ǰλ���Ѿ�
	//	          ΪEOF�������NULL���ҷ���ֵΪE_FAIL��
	// Remark:	  1.��ppRecord�е��ֶν����޸Ĳ��ᱣ������ݿ�
	//			  2.��ȡppRecord�е��ֶΣ���ֵʹ���ֶε���������0��ʼ����
	//				������IGenericStmt::ExecuteDML����IGenericDB::Execute
	//				��ͨ��pFieldsΪ�ֶ�ָ����ֵ
	//************************************
	STDMETHOD( GetRecord )( IProperty2** ppRecord ) PURE;


	STDMETHOD( GetRecord )( IPropertyStr** ppRecord ) PURE;

	//************************************
	// Method:    GetScale ��ȡ�������������ǰλ�ü�¼����Ϊ0���ֶ�ֵ��
	// Returns:   ����ɹ���ȡ��¼������S_OK�����򷵻�E_FAIL
	// Parameter: pScale[out]
	// Remark:	  �ṩ���������Ŀ���Ǽ��ڲ�ѯ�ۼ���������COUNT(*)��,
	//			  ����ֻ��ѯ��һ�ֶΣ���ID��ʱ�Ľ����ȡ����
	//************************************
	STDMETHOD( GetScale )( PROPVARIANT* pScale ) PURE;

	//************************************
	// Method:    MoveFirst ����ǰλ���ƶ�����һ����¼
	// Returns:   �ɹ�����S_OK�����򷵻�E_FAIL
	// Parameter: VOID
	// Remark:	  ���״λ�ȡ��һ����¼��ʱ��������ô˺�����
	//			  �ڱ��������м�¼��ʱ�������Ҫ�ӵ�һ���ٴ�
	//			  ����������Ҫ����
	//************************************
	STDMETHOD( MoveFirst )( VOID ) PURE;


	//************************************
	// Method:    MoveNext ����ǰλ���ƶ�����һ����¼
	// Returns:   �ɹ�����S_OK������Ѿ��ƶ���EOF����E_FALSE
	// Parameter: VOID
	// Remark:	  �ڱ�����������Ҫ���ô˺�����GetRecord����
	//			  �Զ��ƶ���ǰλ��
	//************************************
	STDMETHOD( MoveNext )( VOID ) PURE;
	
	//************************************
	// Method:    IsEOF �ж��Ƿ��Ѿ�������EOF
	//************************************
	STDMETHOD_( BOOL, IsEOF )( VOID ) PURE;
};

MS_DEFINE_IID(IGenericMS, "{A2B1174F-EC18-45D1-8B1C-7526165DCCCE}");

//--------------------------------------------------------------------
//	IGenericStmt
//	ͨ��Ԥ�������ӿڣ�ʹ��Ԥ����������ʡȥ����SQL�Ŀ���������
//	Ԥ��������ά��������ϵͳ��Դ������Ӧ��ΪƵ���Ĳ�ѯʹ��Ԥ����
//
//  Demo( ʹ��prophelpers.h�еİ����� ):
//  
//  IGenericDB* pDB; //   �Ѿ��򿪵�����
//  
//	LPTSTR szSQL = _T("INSERT INTO Table(ID, Name) VALUES(?,?)");
//  UTIL::com_ptr<IGenericStmt>	pStmt;
//  RFAILED( pDB->CompileStmt( szSQL, &pStmt.m_p ) );
//	CPropVar varID, varName;
//  
//  for( int i=0; i<100; i++ )
//  {
//  	varID = i;
//  	varName = _T("Somebody");
//  	
//		RFAILED( pStmt->BindParam( 0, &varID ) );
//  	RFAILED( pStmt->BindParam( 1, &varName ) );
//  	RFAILED( pStmt->ExecuteDML() );
//  
//  	pStmt->Reset();
//  }
//
//	Demo2��BindParams��ʹ��
//	LPTSTR szSQL = _T("INSERT INTO Table(ID, Name) VALUES(?,?)");
//  UTIL::com_ptr<IGenericStmt>	pStmt;
//  RFAILED( pDB->CompileStmt( szSQL, &pStmt.m_p ) );
//	
//	IProperty2*	params;		// ����õ�IProperty2�ӿ�
//	CPropSet propSet( params );	
//  
//  for( int i=0; i<100; i++ )
//  {
//  	LPCTSTR szName = _T("Somebody");
//  	
//		propSet[0] = i;
//		propSet[1] = szName;
//  
//  	pStmt->Reset();
//		pStmt->BindParams( params );
//		RFAILED( pStmt->ExecuteDML() );
//  }
//--------------------------------------------------------------------
interface IGenericStmt : public IUnknown
{
	//************************************
	// Method:    BindParam �ڲ�������ѯ�а󶨲���
	// Returns:   �󶨳ɹ�����S_OK�����򷵻�E_FAIL
	// Parameter: nIndex[in] ������������0��ʼ
	// Parameter: value[in] �󶨵Ĳ���ֵ
	//************************************
	STDMETHOD( BindParam )( INT nIndex, PROPVARIANT* value ) PURE;

	//************************************
	// Method:    Reset ����Ԥ�������
	// Remark:	  ��ÿ�ΰ����в���֮ǰ��Ҫ���ô˺���
	//************************************
	STDMETHOD( Reset )( VOID ) PURE;

	//************************************
	// Method:    ExecuteDML ִ��DML��䣬�����ؽ����
	// Returns:   ִ�гɹ�����S_OK�����򷵻�E_FAIL
	// Parameter: pAffected[out]��������ı��޸ĵļ�¼������ͨ����
	//			  �������������NULL
	//************************************
	STDMETHOD( ExecuteDML )( LPLONG pAffected=NULL ) PURE;

	//************************************
	// Method:    Execute ִ��SELECT��䣬���ؽ����
	// Parameter: ppRST[out] ��������
	// Parameter: pFields��nFields[in] Ϊ������е��ֶ�ָ����ֵ������
	//			  ��IProperty2����ָ����ֵ��ȡ�ֶΣ��粻ָ����Ϊ��
	//			  0��ʼ����������
	//************************************
	STDMETHOD( Execute )( IGenericMS** ppRST, DWORD* pFields=NULL, INT nFields=0 ) PURE;

	//************************************
	// Method:   BindParams �ڲ�������ѯ�а�ȫ������
	// Returns:   �󶨳ɹ�����S_OK�����򷵻�E_FAIL
	// Parameter: params[in]��װ�ز�������������ֵ�������0��ʼ��������Ӧ��
	// Remark:	  ���û���ṩȫ���Ĳ���ֵ�����Զ�Ϊû���ṩֵ�Ĳ�����NULL
	//************************************
	STDMETHOD( BindParams )( IProperty2* params ) PURE;
};

MS_DEFINE_IID(IGenericStmt, "{B111DE8F-C40F-4955-9AAD-5CF02F029575}");

enum MsFieldType
{
	field_invalid = 0,
	field_bigint,
	field_int,
	field_string,
	field_binary,
	field_datetime,
	field_guid,
	field_smallint
};

//--------------------------------------------------------------------
//	IGenericDB ͨ�����ݿ����ӽӿ�
//--------------------------------------------------------------------
interface IGenericDB : public IUnknown
{
	//************************************
	// Method:    Connect �������ݿ�
	// Returns:   ���ӳɹ�����S_OK�����򷵻�E_FAIL
	// Parameter: szConn[in] �����ַ�������½���ݿ���û����ȡ����ڵ�ǰ
	//			  ��SQLITEʵ�֣�ֻ��Ҫ���������ļ���ȫ·�����Ժ������չ��
	// Parameter: lOptions ����ѡ�������չ����ʱû�ж���
	//************************************

	//**
	//	CLSID_DbSqliteDB ���ӷ�ʽ��
	//		1��(�������SQLITE���ݿ�)Database=c:\\database.db;Pwd=password
	//		2��(û�������SQLITE���ݿ�) Database=c:\\database.db �� c:\\database.db
	//*/
	STDMETHOD( Connect )( LPCTSTR szConn, LONG lOptions=0 ) PURE;

	//************************************
	// Method:    Close �Ͽ����ݿ�����
	//************************************
	STDMETHOD( Close )( void ) PURE;

	//************************************
	// Method:    IsTableExists ��ѯָ�������ݱ��Ƿ����
	// Returns:   ���ڷ���TRUE�����򷵻�FALSE
	// Parameter: szTable[in] ����
	//************************************
	STDMETHOD_( BOOL, IsTableExists )( LPCTSTR szTable ) PURE;

	//************************************
	// Method:    CompileStmt Ԥ����SQL���
	// Returns:   ����ɹ�����S_OK�����򷵻�E_FAIL
	// Parameter: szSQL[in] Ҫ�����SQL���
	// Parameter: ppStmt[out] ���IGenericStmtָ��
	//************************************
	STDMETHOD( CompileStmt )( LPCTSTR szSQL, IGenericStmt** ppStmt ) PURE;

	//************************************
	// Method:    ExecuteDML ֱ��ִ��DML��䣨��Ԥ���룩
	// Returns:   ִ�гɹ�����S_OK�����򷵻�E_FAIL
	// Parameter: szSQL[in] : DML��������
	// Parameter: pAffected[out]��������ı��޸ĵļ�¼������ͨ����
	//			  �������������NULL
	//************************************
	STDMETHOD( ExecuteDML )( LPCTSTR szSQL, LONG* pAffected=NULL ) PURE;

	//************************************
	// Method:    Execute ֱ��ִ��SELECT��䣬���ؽ��������Ԥ���룩
	// Returns:   ִ�гɹ�����S_OK�����򷵻�E_FAIL
	// Parameter: szSQL[in] : SELECT��������
	// Parameter: ppRecordset[out] ��������
	// Parameter: pFields��nFields[in] Ϊ������е��ֶ�ָ����ֵ������
	//			  ��IProperty2����ָ����ֵ��ȡ�ֶΣ��粻ָ����Ϊ��
	//			  0��ʼ����������
	//************************************
	STDMETHOD( Execute )( LPCTSTR szSQL, IGenericMS** ppRecordset, DWORD* pFields=NULL, INT nFields=0 ) PURE;

	//************************************
	// Method:    GetTypeDecl ��ȡָ�����͵���������
	// Returns:   �ɹ�����S_OK�����û�ж�Ӧ���������ͣ�����E_FAIL
	// Parameter: vt[in] ָ��������
	// Parameter: szBuf[out] �������ͽ�����������
	// Parameter: nBufLen[in] szBuf�Ĵ�С�����ַ�Ϊ��λ
	// Remark:	  �˺�������ҪӦ�ó����Ƕ�̬����CREATE TABLE��䡣��Ϊ
	//			  ��ͬ�����ݿ��е���������������һ���������Ӧ�ö�̬��
	//			  ��ȡ��������������CREATE TABLE��䣬�Ӷ�ʵ�ֶ����ݿ�
	//			  ���͵ĳ���
	//			  �ٸ����ӣ�����GUID����(vt=VT_CLSID)�����ʹ�õ���SQLITE
	//			  ���ݿ⣬��Ӧ������������BLOB�����ʹ��SQLSever2K5���õ�
	//			  ������������binary(16)
	//************************************
	STDMETHOD( GetTypeDecl )( VARTYPE vt, LPTSTR szBuf, INT nBufLen ) PURE;

	//************************************
	// Method:    ��������Ľӿں���
	// Remark:	  ��Ϊ��ͬ���ݿ������ṩ����������﷨��һ��������Խ���
	//			  ��Ҫֱ����SQL����������������ʹ�������ṩ�ĺ���
	//************************************
	STDMETHOD( BeginTrans )( VOID ) PURE;
	STDMETHOD( CommitTrans )( VOID ) PURE;
	STDMETHOD( RollbackTrans )( VOID ) PURE;

	//************************************
	// Method:    ��������Ľӿں���
	// Remark:	  ��Ϊ��ͬ���ݿ������ṩ����������﷨��һ��������Խ���
	//			  ��Ҫֱ����SQL����������������ʹ�������ṩ�ĺ���
	//************************************
	STDMETHOD( GetTypeDecl )( MsFieldType fieldType, DWORD dwSize, LPTSTR szBuf, INT nBufLen ) PURE;

	STDMETHOD( GetAutoIDDecl )( LPTSTR szBuf, INT nBufLen ) PURE;

	//************************************
	// Method:    BindRealConn
	// Remark:	  ��ʽ�󶨵��������ݿ�����
	//************************************
	STDMETHOD( AttachRealConn )( VOID ) PURE;

	//************************************
	// Method:    BindRealConn
	// Remark:	  ��ʽ������������ݿ����ӵİ�
	//************************************
	STDMETHOD( DetachRealConn )( VOID ) PURE;

	//************************************
	// Method:     GetTableFieldsCount ��ѯָ�������ݱ����ֶθ���
	// Returns:   �ɹ�����S_OK�����ɹ�����E_FAIL
	// Parameter: szTable[in] ����
	// Parameter: pdwCount[out] �����ֶε�����
	//************************************
	STDMETHOD( GetTableFieldsCount )( LPCTSTR szTable,DWORD& dwCount ) PURE;

	//************************************
	// Method:     GetTableFieldInfo ��ѯָ�������ݱ����ֶθ���
	// Returns:   �ɹ�����S_OK�����ɹ�����E_FAIL
	// Parameter: szTable[in] ����
	// Parameter: index[in]    �ֶ�����ֵ����һ���ֶδ�1��ʼ�����ۼ�
	// Parameter: FieldName[out] �ֶ�����
	// Parameter: FieldType[out]    �ֶ�����
	// Parameter: dwSize[out]    �ֶ����ʹ�С
	//************************************
	STDMETHOD(GetTableFieldInfo) ( LPCTSTR szTable,DWORD index, LPTSTR FieldName, LPTSTR FieldType, DWORD& dwSize  ) PURE;

	//�жϴ������Ƿ���á��Է�������
	STDMETHOD_(BOOL, IsOpen)( VOID ) PURE;
};

MS_DEFINE_IID(IGenericDB, "{50A0CA4D-341F-44F2-86EC-8CC0BA5D7BBD}");

//--------------------------------------------------------------------
//	���ݿ���ʽӿڵ����ͣ����ڲ�ͬ�����ݿ�����ӿ�
//--------------------------------------------------------------------
enum DBAPIType
{
	DBAPI_UNKNOWN	= 0,
	DBAPI_SQLITE,
	DBAPI_ADO,
	DBAPI_OTL
};

//--------------------------------------------------------------------
//	IMsDBSelector �˽ӿ�ͨ��ROT��ȡ
//--------------------------------------------------------------------
interface IMsDBSelector : public IMSBase
{
	//************************************
	// Method:    ��ȡ���ݿ�����ӿ�
	// Returns:   �ɹ�����S_OK�����򷵻�E_FAIL
	// Parameter: DBAPIType[in] ���ݿ���ʽӿڵ�����
	// Parameter: ppConn[out] IGenericConnָ��
	// Remark:	  ������Ϊ����һ����������������������ݿ���ʽӿڵĲ�ͬʹ��
	//			  ��ͬ�����ݿ�����ʵ��
	//************************************
	STDMETHOD( GetConn )( DBAPIType type, IGenericDB** ppConn ) PURE;
};

MS_DEFINE_IID(IMsDBSelector, "{812DD829-BFC0-4126-BA6C-F3DA6E625C00}");


// {967D9D9A-BA09-43E6-98AC-F4E03408F8F4}
MS_DEFINE_GUID(CLSID_GenericDB, 
			0x967d9d9a, 0xba09, 0x43e6, 0x98, 0xac, 0xf4, 0xe0, 0x34, 0x8, 0xf8, 0xf4);



//--------------------------------------------------------------------
//	IMsDBManager �˽ӿ�ͨ��ROT��ȡ
//--------------------------------------------------------------------
enum MsDBType
{
	DB_INVALID  = 0	,
	DB_CLIENT		,
	DB_CENTER		,
	DB_LOGCENTER	,		//��һ�����ͣ���Ϊ��־���ݿ�����ӡ�
};

interface IMsDBManager : public IMsPlugin
{
	STDMETHOD( GetConn )( MsDBType type, IGenericDB** ppConn, LPCTSTR szConn=NULL ) PURE;

	enum DbgType
	{
		DBG_INVALID		= 0,
		DBG_CONN_POOL,
	};

	STDMETHOD( GetDbgInfo )( DbgType dwDbgType, IMsBuffer** ppDbgInfo, IMsBuffer* pParams=NULL ) PURE;

	//++++++++++++++++++++++++++
	STDMETHOD_( BOOL, IsConnExist )(MsDBType dwDbgType ) PURE;
	STDMETHOD( ReSetConn )(VOID) PURE;
	//++++++++++++++++++++++++++
};

MS_DEFINE_IID(IMsDBManager, "{E7784FDD-CC1A-419A-9E6C-7FE0211A9B94}");

// {4AB1D199-538D-4FF4-810F-A6EE9A18B9D5}
MS_DEFINE_GUID(CLSID_MsDBManager, 
			0x4ab1d199, 0x538d, 0x4ff4, 0x81, 0xf, 0xa6, 0xee, 0x9a, 0x18, 0xb9, 0xd5);

//--------------------------------------------------------------------
//	��RAII���������ύ/�ع�����
//--------------------------------------------------------------------
class CScopeTrans
{
public:
	CScopeTrans( IGenericDB* pConn )
		:m_pConn(pConn), m_bInTrans(FALSE)
	{
		if( m_pConn )
		{
			HRESULT hr = m_pConn->BeginTrans();
			m_bInTrans = SUCCEEDED(hr) ? TRUE : FALSE;
		}
	}

	~CScopeTrans()
	{
		if( m_pConn && m_bInTrans )
			m_pConn->RollbackTrans();
	}

	HRESULT Begin()
	{
		if( !m_pConn || m_bInTrans )
			return E_FAIL;

		HRESULT hr = m_pConn->BeginTrans();
		m_bInTrans = SUCCEEDED(hr) ? TRUE : FALSE;

		return hr;
	}

	HRESULT Commit()
	{
		if( !m_pConn || !m_bInTrans )
			return E_FAIL;

		HRESULT hr = m_pConn->CommitTrans();
		if( SUCCEEDED(hr) )
			m_bInTrans = FALSE;

		return hr;
	}

	HRESULT Rollback()
	{
		if( !m_pConn || !m_bInTrans )
			return E_FAIL;

		HRESULT hr = m_pConn->RollbackTrans();
		if( SUCCEEDED(hr) )
			m_bInTrans = FALSE;

		return hr;
	}

	BOOL IsInTransaction() { return m_bInTrans; }

private:
	IGenericDB*	m_pConn;
	BOOL		m_bInTrans;	
};

//--------------------------------------------------------------------
//	��RAII����ʽ�󶨰󶨵��������ݿ�����
//--------------------------------------------------------------------
class CScopeAttach
{
public:
	CScopeAttach( IGenericDB* pConn )
		:m_pConn(pConn), m_bAttached(FALSE)
	{
		if( m_pConn )
		{
			HRESULT hr = m_pConn->AttachRealConn();
			m_bAttached = SUCCEEDED(hr) ? TRUE : FALSE;
		}
	}

	~CScopeAttach()
	{
		if( m_pConn && m_bAttached )
			m_pConn->DetachRealConn();
	}

private:
	IGenericDB*	m_pConn;
	BOOL		m_bAttached;	
};


	// {6C484F8F-B94F-4F59-A6AE-4D90805BF128}
MS_DEFINE_GUID(CLSID_DbAdoConn,
	0x6c484f8f, 0xb94f, 0x4f59, 0xa6, 0xae, 0x4d, 0x90, 0x80, 0x5b, 0xf1, 0x28);



//ͨ�������Զ��ж�ʹ�� CLSID_DbSqliteDB ���� CLSID_DbSqliteSafeDB
// {3066762A-BE38-4f9e-8CFB-2037133486B3}
MS_DEFINE_GUID(CLSID_DbSqliteConn, 
	0x3066762a, 0xbe38, 0x4f9e, 0x8c, 0xfb, 0x20, 0x37, 0x13, 0x34, 0x86, 0xb3);



//sqlitedb.dll
// {2A7C48DA-D086-4E36-88DC-2734532FFC7E}
MS_DEFINE_GUID(CLSID_DbSqliteDB,
	0x2a7c48da, 0xd086, 0x4e36, 0x88, 0xdc, 0x27, 0x34, 0x53, 0x2f, 0xfc, 0x7e);


//sqlitesdb.dll
// {D9F63AD9-4A97-40e5-92FD-FA5ED7CA4252}
MS_DEFINE_GUID(CLSID_DbSqliteSafeDB, 
	0xd9f63ad9, 0x4a97, 0x40e5, 0x92, 0xfd, 0xfa, 0x5e, 0xd7, 0xca, 0x42, 0x52);

 };//namespace mscom{;
 };//namespace msdk{;
