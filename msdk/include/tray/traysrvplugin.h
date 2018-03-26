
#ifndef _MSPLUGIN_TRAYSRV_H_
#define _MSPLUGIN_TRAYSRV_H_

#include <mscom/msbase.h>

namespace msdk {

//TraySrv�ṩ�����ӵ����
interface ITrayMsgConnectPoint : public IMSBase	//��Ӧ������Ϣ
{
	/*lParam:
		WM_RBUTTONDOWN
		WM_LBUTTONDBLCLK
		WM_RBUTTONDBLCLK
		WM_MOUSEMOVE
		WM_LBUTTONDOWN
		WM_MOUSELEAVE
		WM_MOUSEHOVER
	*/
	STDMETHOD_(LRESULT, OnTrayIconMsg)(LPARAM lParam, INT x, INT y, BOOL& bHandle) = 0;	//����ͼ����Ϣ
	STDMETHOD_(LRESULT, OnBalloonClicked)(UINT nActionID, BOOL& bHandle)=0;				//���򱻵��
	STDMETHOD_(LRESULT, OnOtherMsg)(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandle) = 0; //������Ϣ
};
MS_DEFINE_IID(ITrayMsgConnectPoint, "{CA2DFAFD-8E48-4588-B6FC-1300BC2A4AEA}");

interface ITrayMenuConnectPoint : public IMSBase	//��Ӧ������Ϣ
{
	STDMETHOD_(LRESULT, OnShowLeftButtonMenu)(INT x, INT y, BOOL& bHandle) = 0;			//����˵�
	STDMETHOD_(LRESULT, OnShowRightButtonMenu)(INT x, INT y, BOOL& bHandle) = 0;		//�Ҽ��˵�
	STDMETHOD_(LRESULT, OnMenuCommand)(WORD nMenuId, BOOL &bHandle) = 0;
	STDMETHOD_(LRESULT, OnMenuCommand)(UINT nMenuIndex, HMENU hMenu, BOOL &bHandle) = 0;
	STDMETHOD_(LRESULT, OnDefaultMenu)(BOOL &bHandle) = 0;	//���˫����ӦĬ�ϲ˵�
};
MS_DEFINE_IID(ITrayMenuConnectPoint, "{8C649637-5E05-4c06-87F9-D1778BC7B239}");

//////////////////////////////////////////////////////////////////////////
interface ITraySrv;
interface ITraySrvPlugin : public IMSBase
{
	STDMETHOD(SetTraySrv)(ITraySrv* pTraySrv) = 0;  //�������������TraySrv�����
	STDMETHOD(GetTraySrv)(ITraySrv** pTraySrv) = 0; //�䱻ITraySrvPlugin���ʹ�õ����ͨ����������õ�ITraySrv
};
MS_DEFINE_IID(ITraySrvPlugin, "{087EA29A-7559-42e6-8D32-14317E650AF8}");

//****************************************************************************************
// ������ʾ��ͼ����ʽ��������PopupTip�ĵ���������ʹ��
#define	TRAYSRV_POPTIPSTYLE_INFORMATION 0x00000001	// ̾��
#define	TRAYSRV_POPTIPSTYLE_WARNING     0x00000002	// ����(��)
#define	TRAYSRV_POPTIPSTYLE_ERROR       0x00000003	// ����(��)

interface ITraySrv : public IMSBase	//���̷���
{
	//InitTray��clsOnTrayMsg���Ӧ����Ҫʵ��ITrayMsg����
	STDMETHOD(CreateTray)(UINT nMsgID, UINT nUID, CLSID clsOnTrayMsg/* = ClSID_CTrayMenuBase*/) = 0;
	STDMETHOD(CreateTray)(UINT nMsgID, UINT nUID, ITraySrvPlugin* pTrayMsg = NULL) = 0;
	STDMETHOD(DestroyTray)() = 0;
	STDMETHOD(GetTraySrvPlugin)(ITraySrvPlugin** pTrayMsg) = 0;
	
	//bOfficial:�����Ƿ�����ʽ״̬��ResetTray���ǻ�ص����һ����ʶbOfficial��״̬
	STDMETHOD(UpdateTray)(LPCTSTR lpcszIcon, LPCTSTR lpcszTips, BOOL bOfficial = TRUE) = 0;
	STDMETHOD(UpdateTray)(HICON hIcon, LPCTSTR lpcszTips, BOOL bOfficial = TRUE) = 0;

	STDMETHOD(UpdateTray)(UINT nIconResId, LPCTSTR lpcszTips, BOOL bOfficial, IN HINSTANCE hInstance/* = NULL*/) = 0;
	STDMETHOD(ShowTray)(bool bShow = TRUE) = 0;

	// ��ʾTray������Ϣ
	STDMETHOD(ShowBalloon)(LPCTSTR lpcszTips, LPCTSTR lpcszTitle = NULL, BOOL bSynchronous = FALSE
		, UINT uTimeout = 5000, UINT uStyle=TRAYSRV_POPTIPSTYLE_INFORMATION, UINT nActionID=0) = 0;

	//����������nTimerElapse���������0��ֹͣ����
	STDMETHOD(FlickerTray)(UINT nTimerElapse = 0) = 0;
	STDMETHOD_(BOOL, IsFlicker)() = 0;

	//���̸�λ���ص�֮ǰ����趨���ȶ�״̬
	STDMETHOD(ResetTray)() = 0;
};
MS_DEFINE_IID(ITraySrv, "{0C5CEB28-9317-416d-A4EC-DF7C0ADA62C8}");

// {4FCE6281-8849-4fc6-A764-95C793EB8A48}
MS_DEFINE_GUID(ClSID_CTraySrv, 
			0x4fce6281, 0x8849, 0x4fc6, 0xa7, 0x64, 0x95, 0xc7, 0x93, 0xeb, 0x8a, 0x48);

} //namespace msdk

#endif	//_MSPLUGIN_TRAYSRV_H_