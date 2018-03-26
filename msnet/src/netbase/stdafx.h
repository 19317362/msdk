// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <atlbase.h>
#include <atlstr.h>

#define GrpName _T("netbase")
#include <mscom/mscominc.h>
#include "msnetlog.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>
using namespace mscom;
using namespace boost::asio::ip;
extern HINSTANCE g_hInstance;
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
static BOOL IsIPV6Address(LPCSTR lpAddr)
{
	if(NULL == lpAddr)
		return FALSE;
	char*pPos = strstr((char*)lpAddr,":");
	if(pPos != NULL)
		return TRUE;
	return FALSE;
}


//��IPv6��ַת���� XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX��ʽ
static std::string GetIPString(tcp::endpoint& endpoint)
{
    //�Ȳ��ж��ˣ���%�ĵ�ַȥ��������
    return endpoint.address().to_string();
/*
   if(endpoint.protocol() == boost::asio::ip::tcp::v6())
   {
       std::string strIP ="";
       boost::array<unsigned char, 16> arrayAddr;
       arrayAddr = endpoint.address().to_v6().to_bytes();
       if( 16 != arrayAddr.size())
           return strIP;
       char szIP[MAX_PATH] = {0};
       sprintf_s(szIP,MAX_PATH,"%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
           arrayAddr[0],arrayAddr[1],arrayAddr[2],arrayAddr[3],
           arrayAddr[4],arrayAddr[5],arrayAddr[6],arrayAddr[7],
           arrayAddr[8],arrayAddr[9],arrayAddr[10],arrayAddr[11],
           arrayAddr[12],arrayAddr[13],arrayAddr[14],arrayAddr[15] );
       strIP = szIP;
       return strIP;
   }
   return endpoint.address().to_string();
 //*/
}
