#pragma once

#include "XmlValueW.h"
#include <Windows.h>
#include <Guiddef.h>


// ����ʱ��ת����������
#define TICKSPERMSEC       10000
#define TICKSPERSEC        10000000
#define SECSPERDAY         86400
#define SECSPERDAY         86400
#define SECSPERHOUR        3600
#define SECSPERMIN         60
#define EPOCHWEEKDAY       1
#define DAYSPERWEEK        7
#define EPOCHYEAR          1601
#define DAYSPERNORMALYEAR  365
#define DAYSPERLEAPYEAR    366
#define MONSPERYEAR        12


typedef struct _CALL_HEADER_
{
	std::wstring   strfrom;
	std::wstring   strto;

}call_header, *pcall_header;

enum reponse_type
{
	NORMAL_REPONSE = 1,
	FAULT_REPONSE = 2,
	UNKNOWN_REPONSE = 3,
};




class CXmlCall
{
public:

	// ���ɴ�����Ϣ
	static std::wstring GenerateBody( const std::wstring& methodName, CXmlValueW const& params );

 	static std::wstring GenerateRequest(
		const call_header & header,
		const std::wstring& methodName, 
		CXmlValueW const& params
		);

	// ��ȡ����ͷ
	static bool GetCallHeaderFromRequest(  const std::wstring & strRequest, call_header & header );
	
	// ��ȡ���ú����������ò���
	static bool GetParamsFromRequest( const std::wstring & strRequest, std::wstring & strFunName, CXmlValueW & params );

	// ����������
	static std::wstring GenerateNormalResponse(const call_header & header, 
		const std::wstring &methodName,
		const std::wstring &plugName,
		const CXmlValueW & params, 
		const CXmlValueW & result);

	// ����Ƿ��ǵ�һ�����ɻ�ִ���
	static bool IsFirstReponse( const std::wstring & str );

	// ��ȡ��ִ����
	static reponse_type  GetReponseType( std::wstring & orgStr, const std::wstring & plugName );


	// ׷�ӽ��
	static std::wstring AppendNormalResponse( std::wstring & orgStr, const std::wstring & plugName, CXmlValueW& result );

	// ׷�Ӵ�����
	static std::wstring AppendFaultReponse( std::wstring & orgStr, const std::wstring & plugName, CXmlValueW& result );

	
	// ���ɴ����
	static std::wstring GenerateFaultResponse( const call_header & header,  const std::wstring & strErrorDescription, const int iErrorCode ); 

	// ��ȡ����ִ�н��
	static bool GetNormalResponse(CXmlValueW& result,  std::wstring strResponse, const std::wstring & plugName );

	// ��ȡ����ִ�н��
	static bool GetFaultResponse( CXmlValueW & fault, std::wstring strResponse, const std::wstring & plugName );


	// ����GUID
	static bool CreateGuid( GUID * pGuid );
	static bool ConvertGuidToString(const GUID* pGuid , std::wstring & strUuid );
	static int  XmlSwprintf(  wchar_t *string, size_t size, const WCHAR *format, ...);
	static bool CreateGuidString( std::wstring & strUuid );

	// ����ʱ��
	static int DaysSinceEpoch(int Year);
	static bool ConvertTimeToString( LONGLONG IntTime , std::wstring & strTime );
	static int IsLeapYear(int Year);
	static bool XmlGetCurrentTime( std::wstring & strTime );
};

