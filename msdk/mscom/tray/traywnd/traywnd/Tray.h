#pragma once
#include <mscom\msplugin.h>
#include <tray\trayplugin.h>
#include "HideWnd.h"
#include <mscomhelper/ConnectionPointHelper.h>
#include <atlapp.h>

class CTray : public ITrayWnd
	, public IExit
	, public IMsPlugin
	, public IMsPluginRun
	, public IWinProc	//������Ǵ�IUnknown����
	, public CConnectionPointContainerHelper<CTray> //ʵ�����ӵ�
	, public CUnknownImp
	, public CMessageLoop
{
public:
	CTray(void);
	~CTray(void);

	UNKNOWN_IMP5_(IExit, ITrayWnd, IMsConnectionPointContainer, IMsPlugin, IMsPluginRun);

	STDMETHOD(init_class)(IMSBase* prot, IMSBase* punkOuter);

	//IMsPlugin
	STDMETHOD(Init)(void*);
	STDMETHOD(Uninit)();

	//IMsPluginRun
	STDMETHOD(Start)();
	STDMETHOD(Stop)();

	//ITrayWnd
	STDMETHOD_(HWND, GetWndHwnd)();

	//IExit �˳�����
	STDMETHOD(NotifyExit)(bool* bExit);

	//IWinProc
	LRESULT OnWinMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandle);

private:
	//ʵ�����ӵ�IWndMessageConnectPoint
	CConnectionPointHelper m_WndMsgConnectPoint;

private:
	UTIL::com_ptr<IMscomRunningObjectTable>	m_pRot;			//ROT

	CHideWnd	m_hideWnd;
	bool		m_bStart;
	DWORD		m_dwRunThreadId;
};
