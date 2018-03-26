#pragma once
#include <rscom\rsplugin.h>
#include <tray\trayplugin.h>
#include <map>
#include <rscomhelper/connectionpointhelper.h>

class COnTrayMsg : public ITrayMsgConnectPoint
	, public IRsPlugin
	, public ITrayMsg
	, public CConnectionPointContainerHelper<COnTrayMsg> //ʵ�����ӵ�
	, public CUnknownImp
{
public:
	COnTrayMsg(void);
	~COnTrayMsg(void);

	UNKNOWN_IMP4_(ITrayMsgConnectPoint, IRsPlugin, ITrayMsg, IRsConnectionPointContainer);

	STDMETHOD(init_class)(IRSBase* prot, IRSBase* punkOuter);

	//IRsPlugin
	STDMETHOD(Init)(void*);
	STDMETHOD(Uninit)();

	//ITrayMsg
	STDMETHOD(SetTraySrv)(ITraySrv* pTraySrv);  //�������������TraySrv�����
	STDMETHOD(GetTraySrv)(ITraySrv** pTraySrv); //�䱻TrayMsg���ʹ�õ����ͨ����������õ�ITraySrv

	//ITrayMenuConnectPoint
	STDMETHOD_(LRESULT, OnShowLeftButtonMenu)(INT x, INT y, BOOL& bHandle);			//����˵�
	STDMETHOD_(LRESULT, OnShowRightButtonMenu)(INT x, INT y, BOOL& bHandle);		//�Ҽ��˵�
	STDMETHOD_(LRESULT, OnMenuCommand)(WORD nMenuId, BOOL &bHandle);
	STDMETHOD_(LRESULT, OnMenuCommand)(int nMenuIndex, HMENU hMenu, BOOL &bHandle);

	//ITrayMsgConnectPoint
	STDMETHOD_(LRESULT, OnTrayIconMsg)(LPARAM lParam, INT x, INT y, BOOL& bHandle);	//����ͼ����Ϣ
	STDMETHOD_(LRESULT, OnBalloonClicked)(UINT nActionID, BOOL& bHandle);				//���򱻵��
	STDMETHOD_(LRESULT, OnOtherMsg)(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandle); //������Ϣ

private:
	//ʵ�����ӵ�IOnMenuMsgConnectPoint
	CConnectionPointHelper m_OnMenuMsgConnectPoint;

	//����TraySrv�����ӵ�ʵ��
	UTIL::com_ptr<IRsConnectionPoint> m_pOnTrayMsgConnectPoint;	//�����ڵ����ӵ�
	DWORD						m_dwOnTrayMsgConnectPointCookie;

private:
	UTIL::com_ptr<IRscomRunningObjectTable>	m_pRot;			//ROT
	UTIL::com_ptr<ITrayWnd>		m_pTrayWnd;		//������
	UTIL::com_ptr<ITraySrv>		m_pTraySrv;		//���̷���
};
