/************************************************************************/
/* 
Author:

lourking. .All rights reserved.

Create Time:

	3,24th,2014

Module Name:

	dsHftpDef.h 

Abstract: ���ö���


*/
/************************************************************************/

#ifndef __DSHFTPDEF_H__
#define __DSHFTPDEF_H__

#include <wininet.h>

//////////////////////////////////////////////////////////////////////////
//url test
typedef struct dsUrlTestRet
{
	DWORD dwRetLenght;

}DSURLTESTRET,*PDSURLTESTRET;


//////////////////////////////////////////////////////////////////////////
//misson struct
#define MAX_MISSONMARK_LENGTH 50

#define DSHFTP_FILE_IFEXIT_OK	0x0001
#define DSHFTP_FILE_REPLACE	0x0002
#define DSHFTP_FILE_CHECKSIZE	0x0004
#define DSHFTP_FILE_CHANGENAME	0x0008


typedef struct dsHftpMission
{
	int nMissionID;//���� mission id
	WCHAR szMissionMark[MAX_MISSONMARK_LENGTH];
	WCHAR szUrl[INTERNET_MAX_HOST_NAME_LENGTH];
	WCHAR szLocalSavePath[MAX_PATH];
	WCHAR szLocalSaveName[MAX_PATH];
	DWORD dwFailTime;
	DWORD dwFailCount;
	DWORD dwRetryCount;
	DWORD dwFilePieceSize;
	DWORD dwFileSize;//���� �����ļ�size
	DWORD dwFileRecvedSize;//���� �����ص��ļ�size
	int	  nLocalFileWriteFlag;
	char *lpszBufForRecv;
}DSHFTPMISSON,*PDSHFTPMISSON;


#define DSHFTP_MISSION_INVALID		-1
#define DSHFTP_MISSION_UNSTART	0
#define DSHFTP_MISSION_WAIT		1
#define DSHFTP_MISSION_RUNNING	2
#define DSHFTP_MISSION_SUSPEND	3
#define DSHFTP_MISSION_ERROR	4
#define DSHFTP_MISSION_WEBFINISHED	5
#define DSHFTP_MISSION_FINISHED	6
#define DSHFTP_MISSION_SHUTDOWN	7
#define DSHFTP_MISSION_FINISHED_BEFORE	8
#define DSHFTP_MISSION_FILE_EXIST	9
#define DSHFTP_MISSION_DELETED	10




//typedef struct dsHftpStatusInfo
//{
//	int nStatus;
//
//}DSHFTPSTATUSINFO,*PDSHFTPSTATUSINFO;
//
//typedef LRESULT  (CALLBACK* HFTPPROC)(__in const DSHFTPMISSON &/*misson*/, __in const PDSHFTPSTATUSINFO /*pstatusinfo*/);


//////////////////////////////////////////////////////////////////////////
//��������״̬

#define DSHFTP_NETCONN_NULL	0x0000
#define DSHFTP_NETCONN_LOCAL	0x0001
#define DSHFTP_NETCONN_WEB		0x0002

#define DSHFTP_NETCONN_OK		(DSHFTP_NETCONN_LOCAL | DSHFTP_NETCONN_WEB)
#define DSHFTP_NETCONN_ALL		DSHFTP_NETCONN_OK


//////////////////////////////////////////////////////////////////////////
//������Ϣ�ص�

interface IdsHftpEngine;

typedef LRESULT (CALLBACK *DSHFTPPROC) (IdsHftpEngine *pEngine, int nMissionID, UINT uNetMsg, WPARAM wParam, LPARAM lParam);


#define DSHFTP_MSG_PIECEIN			0x0001
#define DSHFTP_MSG_PIECEWRITE		0x0002

#define DSHFTP_MSG_MISSIONFAID		0x0010
#define DSHFTP_MSG_MISSIONSUCCESS	0x0011
#define DSHFTP_MSG_MISSIONRUNNING	0x0012	//wParam-dwBytesRecv(DWORD),lParam-nFlag(int):-1����ʧ��
#define DSHFTP_MSG_MISSIONDELETED	0x0013	//ȡ������
#define DSHFTP_MSG_MISSIONSIZE		0x0014	//lParamΪ size
#define DSHFTP_MSG_MISSIONWAITING	0x0015
#define DSHFTP_MSG_MISSIONSUSPEND	0x0016
#define DSHFTP_MSG_MISSION_RECVSIZE	0x0017

#define DSHFTP_MSG_DISCONNECT		0x0020	//�Ͽ�lParam-��������״̬
#define DSHFTP_MSG_CONNECT			0x0021	//����lParam-��������״̬


#endif /*__DSHFTPDEF_H__*/
