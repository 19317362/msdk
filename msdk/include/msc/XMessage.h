#ifndef _X_MESSAGE_H_
#define _X_MESSAGE_H_


//#define USE_MARGINCOM  COM�ӿ�ģʽ
//#define USE_COMMONDLL  C�ӿ�ģʽ


/////////////////////////////////////////////////////////////////////////////
// +----------------+----------------+----------------+----------------+
// |  VersionMajor  |  VersionMinor  |             Reserve             |
// +----+-----------+----------------+----------------+----------------+    +--+--+--+--+
// |ProductType: 12              |     SubSystemID:20                  | <- | MessageID |
// +----+-----------+----------------+----------------+----------------+    +--+--+--+--+
// |Type| Reserve:6 |                   MessagePartID                  | <- | MessageID |
// +----+-----------+----------------+----------------+----------------+    +--+--+--+--+
// |                              UniqueID                             |
// +----------------+----------------+----------------+----------------+
// |                                                                   |
// +                              InnerUse                             +
// |                                                                   |
// +----------------+----------------+----------------+----------------+
// |                              DataLen                              |
// +----------------+----------------+----------------+----------------+
// |                                                                   |
// +                                                                   +
// |                                                                   |
// ...                            DataBuf                            ...
// |                                                                   |
// +                                                                   +
// |                                                                   |
// +----------------+----------------+----------------+----------------+
#define RSMSGID   LONGLONG

#define XMTYPE_NOTIFY                       0
#define XMTYPE_REQUEST                      1
#define XMTYPE_RESPONSE                     2
#define XMTYPE_RESERVE                      3

#define XMMASK_NOTIFY                       (0x0000000000000000)
#define XMMASK_REQUEST                      (0x0000000040000000)
#define XMMASK_RESPONSE                     (0x0000000080000000)
#define XMMASK_RESERVE                      (0x00000000c0000000)

//#define XMNOTIFY(xMessagePartID)            (XMMASK_NOTIFY  |(0xFFFFFF & (xMessagePartID)))
//#define XMREQUEST(xMessagePartID)           (XMMASK_REQUEST |(0xFFFFFF & (xMessagePartID)))
//#define XMRESPONSE(xMessagePartID)          (XMMASK_RESPONSE|(0xFFFFFF & (xMessagePartID)))

#define XMNOTIFY(xProduct,xSubSystem,xMessagePartID)   ( ((RSMSGID)(xProduct)<<52 )|((RSMSGID)(xSubSystem) <<32) |XMMASK_NOTIFY  |(0xFFFFFF & (xMessagePartID)))
#define XMREQUEST(xProduct,xSubSystem,xMessagePartID)  ( ((RSMSGID)(xProduct)<<52 )|((RSMSGID)(xSubSystem) <<32) |XMMASK_REQUEST |(0xFFFFFF & (xMessagePartID)))
#define XMRESPONSE(xProduct,xSubSystem,xMessagePartID) ( ((RSMSGID)(xProduct)<<52 )|((RSMSGID)(xSubSystem) <<32) |XMMASK_RESPONSE|(0xFFFFFF & (xMessagePartID)))

#define XMMASK_PRODUCT                      (0xFFF0000000000000) 
#define XMMASK_SUBSYSTEM                    (0x000FFFFF00000000) 
#define XMMASK_MESSAGEPARTID                (0x0000000000FFFFFF) 
#define XMMASK_XMTTYPE                      (0x00000000c0000000) 

#define XMGET_PRODUCT(xMeassageID)          ( (RSMSGID)(XMMASK_PRODUCT & (xMeassageID)) >> 52 )
#define XMGET_SUBSYSTEM(xMeassageID)        ( (RSMSGID)(XMMASK_SUBSYSTEM & (xMeassageID)) >> 32 )
#define XMGET_MESSAGEPARTID(xMeassageID)    ( (RSMSGID)(XMMASK_MESSAGEPARTID & (xMeassageID)) )
#define XMGET_TYPE(xMeassageID)				( (RSMSGID)(0x00000000C0000000 & (xMeassageID)) >>24 )

//����������Ϣ
//�ͻ��˶���ʱ Subscribe(0,NULL);


#define XM_MAXDATALEN                       (10 * 1024 * 1024)
/////////////////////////////////////////////////////////////////////////////
// ��ϵͳ               | MessagePartID��Χ | UniqueID��Χ
//---------------------------------------------------------------------
// ��Ϣ����������ϵͳ	| 0x000000~0x00FFFF	| 0x00000000~0x0000FFFF
// ɱ����ϵͳ           | 0x010000~0x01FFFF | 0x00010000~0x0001FFFF
// ©ɨ��ϵͳ           | 0x020000~0x02FFFF | 0x00020000~0x0002FFFF
// ���ô洢��ϵͳ       | 0x030000~0x03FFFF | 0x00030000~0x0003FFFF
// ����UI��ϵͳ         | 0x040000~0x04FFFF | 0x00040000~0x0004FFFF
// ������־��ϵͳ       | 0x050000~0x05FFFF | 0x00050000~0x0005FFFF
// ��ҵ��ͨѶ������ϵͳ | 0x060000~0x06FFFF | 0x00060000~0x0006FFFF
// �����ϵͳ           | 0x070000~0x07FFFF | 0x00070000~0x0007FFFF
// ��ʱ������ϵͳ       | 0x080000~0x08FFFF | 0x00080000~0x0008FFFF
// ������ϵͳ           | 0x090000~0x09FFFF | 0x00090000~0x0009FFFF
// ��װ��ϵͳ           | 0x0A0000~0x0AFFFF | 0x000A0000~0x000AFFFF

#define XMPARTIDBASE_MSC                    0x000000
#define XMPARTIDBASE_SCAN                   0x010000
#define XMPARTIDBASE_LEAK                   0x020000
#define XMPARTIDBASE_CONFIG                 0x030000
#define XMPARTIDBASE_PERSONALUI             0x040000
#define XMPARTIDBASE_PERSONALLOG            0x050000
#define XMPARTIDBASE_NETWORKPROXY           0x060000
#define XMPARTIDBASE_MON                    0x070000
#define XMPARTIDBASE_TIMER                  0x080000
#define XMPARTIDBASE_UPDATE                 0x090000
#define XMPARTIDBASE_SETUP                  0x0A0000

#define XSUBSYSTEMID_MSC                    0x00000000
#define XSUBSYSTEMID_SCAN                   0x00000001
#define XSUBSYSTEMID_LEAK                   0x00000002
#define XSUBSYSTEMID_CONFIG                 0x00000003
#define XSUBSYSTEMID_PERSONALUI             0x00000004
#define XSUBSYSTEMID_PERSONALLOG            0x00000005
#define XSUBSYSTEMID_NETWORKPROXY           0x00000006
#define XSUBSYSTEMID_MON                    0x00000007
#define XSUBSYSTEMID_TIMER                  0x00000008
#define XSUBSYSTEMID_UPDATE                 0x00000009
#define XSUBSYSTEMID_SETUP                  0x0000000A
#define XSUBSYSTEMID_EPS                    0x0000000B 

/////////////////////////////////////////////////////////////////////////////
// 


#pragma pack(push, Enter_XMessage, 4)
typedef struct tagXMessage
{
    BYTE  byVersionMajor;
    BYTE  byVersionMinor;
    WORD  wReserve;

    union
    {
		RSMSGID dwMessageID;
        struct
        {	 
			//��Ʒ����
			RSMSGID dwProductType:12;
			//��Ʒ�е���ϵͳ
			RSMSGID dwSubSystemID:20;
			//��Ϣ����
			RSMSGID dwType:2;
			//����
            RSMSGID dwReserve:6;
			//��Ϣ��
            RSMSGID dwMessagePartID:24;
        };
    };
    DWORD dwUniqueID;
    DWORD dwInnerUseHight;
    DWORD dwInnerUseLow;
    DWORD dwDataLen;
    //YTE byDataBuffer[0];
} XMessage, *LPXMESSAGE;

#pragma pack(pop, Enter_XMessage)
/////////////////////////////////////////////////////////////////////////////
/*
XMessage's initialization like this:

LPBYTE InitXMessage(LPXMESSAGE lpXMessage, DWORD dwMessageID, DWORD dwUniqueID = 0xFFFFFFFF)
{
	_ASSERT(NULL != lpXMessage);
	lpXMessage->dwMessageID = dwMessageID;
	lpXMessage->dwUniqueID  = dwUniqueID;
	return (LPBYTE)(lpXMessage + 1);
}

{
	...
	lpXMessage = lpXMSCProxy->AllocMessage(XDATALEN);
	LPBYTE lpData = InitXMessage(XMESSAGEID, XUNIQUEID);
	CopyMemory(lpData, DATA);

	if (!lpXMSCProxy->Issue(lpXMessage))
	{
		lpXMSCProxy->FreeMessage(lpXMessage);
	}
	...
}

*/

/////////////////////////////////////////////////////////////////////////////
#endif
