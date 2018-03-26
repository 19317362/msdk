
#ifndef _MS_TRAYMENUDEF_H_
#define _MS_TRAYMENUDEF_H_

#include <mscom/msbase.h>

namespace msdk {

// {FCA0E62A-5DD4-46fb-AFB2-BDC74EA7DB36} //�ṩ��һ��Ĭ�ϵ�������Ϣһ�δ�����
MS_DEFINE_GUID(ClSID_CTrayMenuBase,
			   0xfca0e62a, 0x5dd4, 0x46fb, 0xaf, 0xb2, 0xbd, 0xc7, 0x4e, 0xa7, 0xdb, 0x36);

typedef struct _MENUCMD_INFO	//�ο� CMINVOKECOMMANDINFO
{
	DWORD	cbSize;
	HMENU	hMenu;
	UINT	uMenuItem;
	UINT	wMenuID;
}MENUCMD_INFO;
typedef MENUCMD_INFO *LPMENUCMD_INFO;

enum
{
	MenuType_RightButton = 0,
	MenuType_LeftButton,
};

enum
{
	MenuFlag_Normal = 0,	//��ͨ�˵���
	MenuFlag_DefaultOnly,	//Ĭ�ϲ˵���
};

//ʵ��ITrayMsg�����֧�ֵ����ӵ�
//�˵�����
interface ITrayMenuBaseConnectPoint : public IMSBase	//��Ӧ�˵�
{
	STDMETHOD_(UINT, QueryMenu)(	//����ռ�ò˵�ID��������
		HMENU hMenu,				//�˵�
		UINT indexMenu,				//��ǰ����Ӳ˵���λ��
		UINT idCmdFirst,			//��ǰ���ò˵�ID��
		UINT uFlags,				//��ӵĲ˵����ʶ MenuFlag_Normal | MenuFlag_DefaultOnly
		UINT uMenuType) = 0;		//�˵����ͣ���������Ҽ���	 MenuType_RightButton | MenuType_LeftButton

	STDMETHOD(InvokeMenuCmd)(
		LPMENUCMD_INFO lpmci) = 0;	//��Ӧ�˵���Ϣ
};
MS_DEFINE_IID(ITrayMenuBaseConnectPoint, "{A450CD33-A022-4a2f-B815-A7A2080C7FEA}");

} //namespace msdk

#endif	//_MS_TRAYMENUDEF_H_