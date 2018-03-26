#if !defined(AFX_SYSTRAY_H__0D3E266A_3299_49EA_B650_9B01CC628D94__INCLUDED_)
#define AFX_SYSTRAY_H__0D3E266A_3299_49EA_B650_9B01CC628D94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#undef  _WIN32_IE
#define _WIN32_IE 0x0600
#include <shellapi.h>
#include <shlwapi.h>
#include <vector>


#define TRAY_ACTION_NONE				0
#define TRAY_ACTION_FILEMON_CLOSED		1
#define TRAY_ACTION_WILL_QUIT			2
#define TRAY_ACTION_FULL_SCAN			3


typedef struct
{
	UINT nAction;
	UINT Style;
	TCHAR szTipTitle[64];
	TCHAR szTipInfo[256];
	UINT uTimeout;
} ST_BALLOONTIP;

using namespace std;
typedef vector<ST_BALLOONTIP> BALLOON_TIPS;


const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(TEXT("TaskbarCreated"));

/* 
	ע��ʹ�ñ���ʱҪ��Ԥ����ͷ�ļ��У�һ��ΪStdAfx.h��
	����#define _WIN32_IE 0x0600�����������ܲ�ͨ��				*/


//#define	S_IGNORE			(MAKE_HRESULT(0, FACILITY_NULL, 1))
//////////////////////////////////////////////////////////////////////
//
//	��       ��:	ITrayEvents
//	�� �� �� ��:	�ӿڣ��ṩ��ϵͳ���̲����¼��Ĵ���
//
//////////////////////////////////////////////////////////////////////
class ITrayEvents
{
public:
	/*		���º����������������κδ�����ֱ�ӷ���S_IGNORE		*/
	/*		����������뷵��S_OK									*/
	virtual LRESULT OnTrayIconMsg(LPARAM lParam, INT x, INT y, BOOL &bHandle) = 0;	//����ͼ����Ϣ
	virtual LRESULT OnShowLeftButtonMenu(INT x, INT y, BOOL &bHandle) = 0;			//����˵�
	virtual LRESULT OnShowRightButtonMenu(INT x, INT y, BOOL &bHandle) = 0;		//�Ҽ��˵�
	virtual LRESULT OnBalloonClicked(UINT nActionID, BOOL &bHandle)=0;				//���򱻵��
	virtual LRESULT OnOtherMsg(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandle) = 0; //������Ϣ
};


// ������ʾ��ͼ����ʽ��������PopupTip�ĵ���������ʹ��
#define	POPTIPSTYLE_INFORMATION 0x00000001	// ̾��
#define	POPTIPSTYLE_WARNING     0x00000002	// ����(��)
#define	POPTIPSTYLE_ERROR       0x00000003	// ����(��)


//////////////////////////////////////////////////////////////////////
//
//	��		 ��:	CSysTray
//  ��		 ��:    N/A
//	�� �� �� ��:	�ṩϵͳ������ز�����
//  �� �� �� д:	
//  �� �� �� ��:	2010/10/27
//
//////////////////////////////////////////////////////////////////////
class CSysTray  
{
public:
	// Methods
	CSysTray(HINSTANCE hInst = NULL, UINT uEmptyIconId = 0);
	virtual ~CSysTray();
	
	// ����Tray��ά����Ӧ�ṹ
	virtual BOOL CreateTray(HWND hOwnerWnd, UINT nMsgID, UINT nUID, ITrayEvents *pEvents=NULL);

	virtual LRESULT WinProc(UINT, WPARAM, LPARAM, BOOL &bHandle);
	// ����Tray
	virtual VOID Destroy();	

	// ����ͼ�����ʾ
	virtual BOOL UpdateTray(UINT nIcon, LPCTSTR lpcszTips, BOOL bOfficial, IN HINSTANCE = NULL);

	// ����ͼ�����ʾ(for path)
	virtual BOOL UpdateTray(LPCTSTR lpcszIcon, LPCTSTR lpcszTips, BOOL bOfficial);

	// ����ͼ�����ʾ(for HICON)
	virtual BOOL UpdateTray(HICON, LPCTSTR lpcszTips, BOOL bOfficial, BOOL bLoadIcon = FALSE);

	// ��ʾTray������Ϣ(�첽��ʽ)
	virtual BOOL ShowBalloon(LPCTSTR lpcszTips, LPCTSTR lpcszTitle=NULL, UINT uTimeout = 5000, UINT uStyle=POPTIPSTYLE_INFORMATION, UINT nActionID=TRAY_ACTION_NONE);

	// ��ʾTray������Ϣ(ͬ����ʽ)
	virtual BOOL ShowBalloonimmediately(LPCTSTR lpcszTips, LPCTSTR lpcszTitle=NULL, UINT uTimeout = 5000, UINT uStyle =POPTIPSTYLE_INFORMATION, UINT nActionID =TRAY_ACTION_NONE);

	//����������nTimerElapse���������0��ֹͣ����
	virtual BOOL FlickerTray(UINT nTimerElapse);

	virtual BOOL IsFlicker();

	//���̸�λ���ص�֮ǰ����趨���ȶ�״̬
	virtual BOOL ResetTray();

	// Tip���������(�ֽ�)
	virtual int GetTipCap();

	virtual BOOL ShowTray(bool bShow = TRUE);

	HWND GetWnd() {return m_hHideWnd;};

	
private:
	// Methods

	// ������ϵͳ��ĳ��DLL�����汾��(�����ɷ��빫����������?)
	UINT CheckSysDllMajorVersion(LPCTSTR lpcszSystemDllFilename);

	// ��鲢ɾ���Ѿ���ӹ���ICON
	VOID DelIcon();

	// ��vector��ȡ��һ��������ʾ���ݲ���ʾ
	VOID PopBalloon(BALLOON_TIPS& vtBalloonTips);
	
	// ���Explorer��ô��?(���LOG)
	VOID AnalyzeExplorer();

	void FlickerTray(BOOL bInit = FALSE);

	// Attributes
	enum
	{
		ICONTYPE_EMPTY = 0,
		ICONTYPE_ICON = 1,
		ICONTYPE_ICONPATH,
		ICONTYPE_ICONID,
	};
	struct ST_OfficialInfo
	{
		TCHAR	szTip[128];
		UINT	iconType;
		HICON	hIcon;
		TCHAR	szIconPath[MAX_PATH*2];
		UINT	iconId;
	};
	//////////////////////////////////////////////////////////////////////////
	HINSTANCE m_hInst;
	UINT m_uEmptyIconId;

	HWND			m_hHideWnd;			// ���ش���,���ڽ���explorer��icon��Ϣ
	HWND			m_hOwnerWnd;		// �ⲿϣ������Tray��Ϣ�Ĵ���
	HICON			m_hEmpty;			//�յ��Ǹ�ͼ��
	BOOL			m_bIsFlicker;
	BOOL			m_bHideIcon;		//��������ͼ��

	BOOL			m_bLoadIcon;
	NOTIFYICONDATA	m_stNID;			// ����Trayʹ�õĽṹ
	NOTIFYICONDATA	m_stEmptyNID;		// ����Trayʹ�õĽṹ
	ST_OfficialInfo	m_stSaveNID;		// ��¼���һ������tray��Ϣ

	ITrayEvents	   *m_pEvents;			// ����Tray�¼��������ָ��
	UINT			m_nMsgID;			// ����Tray����ϢID
	UINT			SHELL_VEMSION;		// ����Shell32.dll�İ汾

	BOOL			m_bHoldRButton;		// ����Ҽ��Ƿ���(�����ж��Ҽ������¼�)

	BALLOON_TIPS	m_vtBalloonTips;	// BalloonTips
	ST_BALLOONTIP	m_stCurTip;
	BOOL			m_bBalloonShowing;


	BOOL			m_bSetMouseLeaveTimer;
	POINT			m_ptMouse;

	BOOL			m_bNotifyMouseHover;
	DWORD			m_dwMouseHoverCount;  //ʱ�����
};

#endif // !defined(AFX_SYSTRAY_H__0D3E266A_3299_49EA_B650_9B01CC628D94__INCLUDED_)
