#pragma once
#include <mscom\msplugin.h>
#include <tray\trayplugin.h>
#include "SysTray.h"
#include <map>
#include <mscomhelper/connectionpointhelper.h>
#include <mscomhelper/UseConnectionPoint.h>
#include <mscomhelper/CreateObjectHelper.h>

class CTraySrv : public ITraySrv
	, public IMsPlugin
	, public IWndMessageConnectPoint
	, public CConnectionPointContainerHelper<CTraySrv> //ʵ�����ӵ�
	, public ITrayEvents	//������Ǵ�IUnknown����
	, public CUnknownImp
{
public:
	CTraySrv(void);
	~CTraySrv(void);

	UNKNOWN_IMP4_(ITraySrv, IMsPlugin, IWndMessageConnectPoint, IMsConnectionPointContainer);

	STDMETHOD(init_class)(IMSBase* prot, IMSBase* punkOuter);

	//IMsPlugin
	STDMETHOD(Init)(void*);
	STDMETHOD(Uninit)();

	//IWndMessageConnectPoint
	STDMETHOD_(LRESULT, OnWndMessage)(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandle);

	//ITraySrv
	//InitTray��clsOnTrayMsg���Ӧ����Ҫʵ��ITrayMsg����
	STDMETHOD(CreateTray)(UINT nMsgID, UINT nUID, CLSID clsOnTrayMsg/* = ClSID_COnTrayMsg*/);
	STDMETHOD(CreateTray)(UINT nMsgID, UINT nUID, ITraySrvPlugin* pTrayMsg = NULL);
	STDMETHOD(DestroyTray)();
	STDMETHOD(GetTraySrvPlugin)(ITraySrvPlugin** pTrayMsg);

	STDMETHOD(UpdateTray)(LPCTSTR lpcszIcon, LPCTSTR lpcszTips, BOOL bOfficial = TRUE);
	STDMETHOD(UpdateTray)(HICON hIcon, LPCTSTR lpcszTips, BOOL bOfficial = TRUE);
	STDMETHOD(UpdateTray)(UINT nIcon, LPCTSTR lpcszTips, BOOL bOfficial, IN HINSTANCE hInstance/* = NULL*/);
	STDMETHOD(ShowTray)(bool bShow = TRUE);

	// ��ʾTray������Ϣ
	STDMETHOD(ShowBalloon)(LPCTSTR lpcszTips, LPCTSTR lpcszTitle = NULL, BOOL bSynchronous = FALSE
		, UINT uTimeout = 5000, UINT uStyle=POPTIPSTYLE_INFORMATION, UINT nActionID=TRAY_ACTION_NONE);

	//����������nTimerElapse���������0��ֹͣ����
	STDMETHOD(FlickerTray)(UINT nTimerElapse = 0);
	STDMETHOD_(BOOL, IsFlicker)();

	//���̸�λ���ص�֮ǰ����趨���ȶ�״̬
	STDMETHOD(ResetTray)();

	//ITrayEvents
	virtual LRESULT OnTrayIconMsg(LPARAM lParam, INT x, INT y, BOOL& bHandle);
	virtual LRESULT OnShowLeftButtonMenu(INT x, INT y, BOOL& bHandle);
	virtual LRESULT OnShowRightButtonMenu(INT x, INT y, BOOL& bHandle);
	virtual LRESULT OnBalloonClicked(UINT nActionID, BOOL& bHandle);				//���򱻵��
	virtual LRESULT OnOtherMsg(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandle);

	LRESULT OnCommand(WPARAM wParam, LPARAM lParam, BOOL &bHandle);
	LRESULT OnMenuCommand(WPARAM wParam, LPARAM lParam, BOOL &bHandle);

	LRESULT OnMenuCommand(WORD nMenuId, BOOL &bHandle);
	LRESULT OnMenuCommand(int nMenuIndex, HMENU hMenu, BOOL &bHandle);

private:
	CConnectionPointHelper m_TrayMsgConnectPoint;	//ʵ�����ӵ�ITrayMsgConnectPoint
	CConnectionPointHelper m_TrayMenuConnectPoint;	//ʵ�����ӵ�ITrayMenuConnectPoint

	//����Tray�����ӵ�ʵ��
	UseConnectPoint<IWndMessageConnectPoint> m_CPWndMessageConnectPoint; //�����ڵ����ӵ�

private:	
	UTIL::com_ptr<IMscomRunningObjectTable>	m_pRot;			//ROT
	UTIL::com_ptr<ITrayWnd>		m_pTrayWnd;
	CCreateObjectHelper<ITraySrvPlugin> m_pTraySrvPlugin;

	CSysTray	m_sysTray;
};
