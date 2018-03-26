#pragma once
#include <mscom\msplugin.h>
#include <tray\trayplugin.h>
#include <tray/traymenudef.h>
#include <mscomhelper/connectionpointhelper.h>
#include <mscomhelper/UseConnectionPoint.h>
#include <mscomhelper/TraySrvPluginHelper.h>
#include <list>

struct ST_MENUPLUGIN_INFO
{
	UINT	idBegin;	//�������俪ʼ ������Ĭ�������ID
	UINT	idEnd;		//����������� ������Ĭ�������λ��
	DWORD	dwPluginCookie; //��������ӵ�����Cookie
	BOOL	bDefaultMenu;

	ST_MENUPLUGIN_INFO()
		:idBegin(0),idEnd(0),dwPluginCookie(-1),bDefaultMenu(FALSE)
	{}

	ST_MENUPLUGIN_INFO(UINT u1, UINT u2, DWORD dw, BOOL bDefault = FALSE)
		:idBegin(u1),idEnd(u2),dwPluginCookie(dw),bDefaultMenu(bDefault)
	{}
};

class CUseMenuPlugin
{
public:
	CUseMenuPlugin();
	~CUseMenuPlugin();

	HRESULT CreateMenu();
	HRESULT ReleaseMenu();
	UINT QueryPluginMenu(ITrayMenuBaseConnectPoint* pPoint, DWORD dwPointCookie, UINT idCmdFirst, UINT uFlags, UINT uMenuType);
	BOOL FindMenuIdPlugin(UINT id, ST_MENUPLUGIN_INFO& info);
	BOOL FindDefaultMenuIdPlugin(ST_MENUPLUGIN_INFO& info);
	UINT GetMenuPluginCount();

	operator HMENU () const {return m_hMenu;}
private:
	HMENU	m_hMenu;

	typedef std::list<ST_MENUPLUGIN_INFO> MENUPLUGIN_INFOLIST;
	MENUPLUGIN_INFOLIST m_listMenuPluginInfo;
	
	BOOL FindPlugin(UINT id, ST_MENUPLUGIN_INFO& info, BOOL bFindDefault);
};

//////////////////////////////////////////////////////////////////////////
class CTrayMenuBase : public ITrayMenuConnectPoint
	, public IMsPlugin
	, public CTraySrvPluginHelper<CTrayMenuBase>
	, public CConnectionPointContainerHelper<CTrayMenuBase> //ʵ�����ӵ�
	, public CUnknownImp
{
public:
	CTrayMenuBase(void);
	~CTrayMenuBase(void);

	UNKNOWN_IMP4_(ITrayMenuConnectPoint, IMsPlugin, ITraySrvPlugin, IMsConnectionPointContainer);

	STDMETHOD(init_class)(IMSBase* prot, IMSBase* punkOuter);

	//IMsPlugin
	STDMETHOD(Init)(void*);
	STDMETHOD(Uninit)();

	//ITraySrvPlugin ����CTraySrvPluginHelper��Ӧ����
	STDMETHOD(SetTraySrv)(ITraySrv* pTraySrv);  //�������������TraySrv�����

	//ITrayMenuConnectPoint
	STDMETHOD_(LRESULT, OnShowLeftButtonMenu)(INT x, INT y, BOOL& bHandle);			//����˵�
	STDMETHOD_(LRESULT, OnShowRightButtonMenu)(INT x, INT y, BOOL& bHandle);		//�Ҽ��˵�
	STDMETHOD_(LRESULT, OnMenuCommand)(WORD nMenuId, BOOL &bHandle);
	STDMETHOD_(LRESULT, OnMenuCommand)(UINT nMenuIndex, HMENU hMenu, BOOL &bHandle);
	STDMETHOD_(LRESULT, OnDefaultMenu)(BOOL &bHandle);	//���˫����ӦĬ�ϲ˵�

	//bShow=FALSE �տ�ʼ��ʱ�����һ�£���������ȡ��Ĭ�ϲ˵���˫����ʱ�����Ӧ
	LRESULT ShowTrayMenu(int nMenuType, INT x, INT y, BOOL& bHandle, BOOL bShow = TRUE);

private:
	//ʵ�����ӵ�ITrayMenuBaseConnectPoint
	CConnectionPointHelper m_TrayMenuBaseConnectPoint;

	//����TraySrv�����ӵ�ʵ��
	UseConnectPoint<ITrayMenuConnectPoint> m_UseTrayMenuConnectPoint;

private:
	UTIL::com_ptr<IMscomRunningObjectTable>	m_pRot;	//ROT
	UTIL::com_ptr<ITrayWnd>		m_pTrayWnd;			//������
	CUseMenuPlugin	m_useMenuplugin;
};
