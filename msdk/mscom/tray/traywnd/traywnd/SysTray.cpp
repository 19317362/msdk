#include "stdafx.h"
#include "SysTray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define GETTRAY_TIMERID(nid) (m_stNID.uID*100 + nid)

#define MouseHover_Timer_Tick 100
enum
{
	TIMER_LBUTTONDOWN = 1,
	TIMER_PopBalloon,
	TIMER_KILLBalloon,
	TIMER_Flicker,
	TIMER_MouseLeave,
};

CSysTray::CSysTray(HINSTANCE hInst/* = NULL*/, UINT uEmptyIconId/* = 0*/)
:m_hInst(hInst), m_uEmptyIconId(uEmptyIconId), m_bHideIcon(FALSE)
{
	m_hHideWnd	= NULL;
	m_pEvents	= NULL;
	m_nMsgID		= 0;
	SHELL_VEMSION	= 0;
	m_bHoldRButton	  = FALSE;
	m_stNID.cbSize    = 0;
	m_bBalloonShowing = FALSE;
	m_bLoadIcon = FALSE;
	m_hEmpty = NULL;
	m_bIsFlicker = FALSE;
	m_bSetMouseLeaveTimer = FALSE;
	m_bNotifyMouseHover = FALSE;
	m_dwMouseHoverCount = 0;
	ZeroMemory(&m_stSaveNID, sizeof(m_stSaveNID));
}

CSysTray::~CSysTray()
{
	Destroy();
}


///////////////////////////////////////////////
// ����Tray��������Ӧ����
// ����ֵ: �ɹ�����TRUE����hOwnerWndΪNULL��ʧ��
///////////////////////////////////////////////
BOOL CSysTray::CreateTray(HWND hOwnerWnd, UINT nMsgID, 
					  UINT nUID, ITrayEvents *pEvents/*=NULL*/)
{
	// ���ݵ�ǰ�Ĳ���ϵͳDLL�汾��ȷ��NID�ṹ��cbSize
	// �������Ա�֤�ڲ�ͬWin OS�µļ�����
	ZeroMemory(&m_stNID, sizeof(m_stNID));
	ZeroMemory(&m_stEmptyNID, sizeof(m_stEmptyNID));

	SHELL_VEMSION =CheckSysDllMajorVersion(TEXT("shell32.dll"));
	if ( SHELL_VEMSION < 5 )
		m_stNID.cbSize =NOTIFYICONDATA_V1_SIZE; // the version is less than 5
	else if (SHELL_VEMSION < 6 )
		m_stNID.cbSize =NOTIFYICONDATA_V2_SIZE; // the version is 5
	else
		m_stNID.cbSize =sizeof(NOTIFYICONDATA);	// the version 6 or later

	// ���洫�ݹ����Ĳ���
	m_hHideWnd					= hOwnerWnd;

	m_stNID.hWnd				= m_hHideWnd;
	m_stNID.uCallbackMessage	= nMsgID;
	m_stNID.uID					= nUID;

	m_nMsgID					= nMsgID;
	m_pEvents					= pEvents;

	if(m_uEmptyIconId)
	{
		m_hEmpty = (HICON)::LoadImage(m_hInst?m_hInst:GetModuleHandle(NULL), MAKEINTRESOURCE(m_uEmptyIconId),
			IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	}

	return TRUE;
}

///////////////////////////////////////////////
// ����Tray
// ����ֵ: N/A
// Created by Leon @ 2005/07/22
///////////////////////////////////////////////
VOID CSysTray::Destroy()
{
	::KillTimer( m_hHideWnd, GETTRAY_TIMERID(TIMER_PopBalloon) );
	::KillTimer(m_hHideWnd, GETTRAY_TIMERID(TIMER_Flicker));
	m_bIsFlicker = FALSE;

	m_bBalloonShowing = FALSE;

	// ����Icon
	DelIcon();		

	m_vtBalloonTips.clear();

	m_stNID.cbSize = 0;

	m_hHideWnd =NULL;
}


///////////////////////////////////////////////
// ͬʱ����ͼ�����ʾ(for ravmon only)
// ����ֵ: �ɹ�����TRUE
// Created by Leon @ 2007/08/28
///////////////////////////////////////////////
BOOL CSysTray::UpdateTray(UINT nIcon, LPCTSTR lpcszTips, BOOL bOfficial, IN HINSTANCE hInstance/* = NULL*/)
{
	if ( !m_stNID.cbSize ) {
		return FALSE;
	}

	if(bOfficial)
	{
		ZeroMemory(&m_stSaveNID, sizeof(m_stSaveNID));
		if(lpcszTips)
		{
			_tcscpy_s(m_stSaveNID.szTip, sizeof(m_stSaveNID.szTip)/sizeof(TCHAR), lpcszTips);
		}

		m_stSaveNID.iconType = ICONTYPE_ICONID;
		m_stSaveNID.iconId = nIcon;
	}

	// ����Icon��Դ
	HICON hIco =(HICON)::LoadImage(hInstance?hInstance:GetModuleHandle(NULL), MAKEINTRESOURCE(nIcon),
		IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);

	return UpdateTray(hIco, lpcszTips, FALSE, TRUE);
}


///////////////////////////////////////////////
// ͬʱ����ͼ�����ʾpath version(for ravmon only)
// ����ֵ: �ɹ�����TRUE
// Created by Leon @ 2008/02/21
///////////////////////////////////////////////
BOOL CSysTray::UpdateTray(LPCTSTR lpcszIcon, LPCTSTR lpcszTips, BOOL bOfficial)
{
	if ( !m_stNID.cbSize ) {
		return FALSE;
	}

	if ( !lpcszIcon ) {
		return FALSE;
	}

	if(bOfficial)
	{
		ZeroMemory(&m_stSaveNID, sizeof(m_stSaveNID));
		if(lpcszTips)
		{
			_tcscpy_s(m_stSaveNID.szTip, sizeof(m_stSaveNID.szTip)/sizeof(TCHAR), lpcszTips);
		}

		m_stSaveNID.iconType = ICONTYPE_ICONPATH;
		_tcscpy_s(m_stSaveNID.szIconPath, sizeof(m_stSaveNID.szIconPath)/sizeof(TCHAR), lpcszIcon);
	}

	// ����Icon��Դ
	HICON hIco =(HICON)::LoadImage(GetModuleHandle(NULL), lpcszIcon,
		IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR|LR_LOADFROMFILE);

	return UpdateTray(hIco, lpcszTips, FALSE, TRUE); //��������������һ�����FALSE����һ��Ͳ�Ҫ������
}

BOOL CSysTray::UpdateTray(HICON hIco, LPCTSTR lpcszTips, BOOL bOfficial, BOOL bLoadIcon/* = FALSE*/)
{
	if ( !m_stNID.cbSize ) {
		return FALSE;
	}

	if (NULL == hIco) {
		return FALSE;
	}

	if(bOfficial)
	{
		ZeroMemory(&m_stSaveNID, sizeof(m_stSaveNID));
		if(lpcszTips)
		{
			_tcscpy_s(m_stSaveNID.szTip, sizeof(m_stSaveNID.szTip)/sizeof(TCHAR), lpcszTips);
		}

		m_stSaveNID.iconType = ICONTYPE_ICON;
		m_stSaveNID.hIcon = hIco;
	}

	if (m_bLoadIcon && m_stNID.hIcon) //��һ��Icon��load�����ģ���Ҫ��Destroy
		::DestroyIcon(m_stNID.hIcon);

	m_bLoadIcon = bLoadIcon; //��¼�����Ƿ���Load������
	m_stNID.hIcon = hIco;

	// ������ʾ
	if(lpcszTips)
	{
		ZeroMemory(m_stNID.szTip, sizeof(m_stNID.szTip));
		_tcscpy_s(m_stNID.szTip, 128, lpcszTips);
	}

	if(m_bHideIcon)	//�����ǰ���������صģ�����Ե�
		return TRUE;

	// �ȳ����޸ģ������޸ľ����
	m_stNID.uFlags = NIF_TIP|NIF_ICON;
	if ( !::Shell_NotifyIcon(NIM_MODIFY, &m_stNID) ) {
		m_stNID.uFlags = NIF_TIP|NIF_ICON|NIF_MESSAGE;
		if ( !::Shell_NotifyIcon(NIM_ADD, &m_stNID) ) {
			AnalyzeExplorer(); // �������explorer�ĵ�����Ϣ
			return FALSE;
		}
	}

	return TRUE;
}

//����������nTimerElapse���������0��ֹͣ����
BOOL CSysTray::FlickerTray(UINT nTimerElapse)
{
	KillTimer(m_hHideWnd, GETTRAY_TIMERID(TIMER_Flicker));
	m_bIsFlicker = FALSE;
	
	FlickerTray(TRUE);

	if(nTimerElapse > 0)
	{
		//��ʼtimer
		m_bIsFlicker = TRUE;
		SetTimer(m_hHideWnd, GETTRAY_TIMERID(TIMER_Flicker), nTimerElapse / 2, NULL );
	}
	return FALSE;
}

BOOL CSysTray::IsFlicker()
{
	return m_bIsFlicker;
}

void CSysTray::FlickerTray(BOOL bInit/* = FALSE*/)
{
	static BOOL bShow = TRUE;
	
	if(bInit)
	{
		bShow = TRUE;
	}
	else
	{
		bShow = !bShow;
	}

	if(m_bHideIcon)	//�����ǰ���������صģ�����Ե�
		return;

	if(bShow)
	{
		// �ȳ����޸ģ������޸ľ����
		m_stNID.uFlags = NIF_TIP|NIF_ICON;
		if ( !::Shell_NotifyIcon(NIM_MODIFY, &m_stNID) ) {
			m_stNID.uFlags = NIF_TIP|NIF_ICON|NIF_MESSAGE;
			if ( !::Shell_NotifyIcon(NIM_ADD, &m_stNID) ) {
			}
		}
	}
	else
	{
		if(m_stEmptyNID.cbSize != m_stNID.cbSize)
			memcpy(&m_stEmptyNID, &m_stNID, m_stNID.cbSize);

		if(m_stEmptyNID.hIcon != m_hEmpty)
			m_stEmptyNID.hIcon = m_hEmpty;

		// �ȳ����޸ģ������޸ľ����
		m_stNID.uFlags = NIF_ICON;
		::Shell_NotifyIcon(NIM_MODIFY, &m_stEmptyNID);
	}
}

//���̸�λ���ص�֮ǰ����趨���ȶ�״̬
BOOL CSysTray::ResetTray()
{
	if(ICONTYPE_EMPTY == m_stSaveNID.iconType)
		return FALSE;

	switch(m_stSaveNID.iconType)
	{
	case ICONTYPE_ICON:
		return UpdateTray(m_stSaveNID.hIcon, m_stSaveNID.szTip, FALSE);
		break;
	case ICONTYPE_ICONPATH:
		return UpdateTray(m_stSaveNID.szIconPath, m_stSaveNID.szTip, FALSE);
		break;
	case ICONTYPE_ICONID:
		return UpdateTray(m_stSaveNID.iconId, m_stSaveNID.szTip, FALSE);
		break;
	}

	return FALSE;
}

///////////////////////////////////////////////
// ����Tray������ʾ
// ����ֵ: �ɹ�����TRUE
// Created by Leon @ 2007/08/28
///////////////////////////////////////////////
BOOL CSysTray::ShowBalloon(LPCTSTR lpcszTips, LPCTSTR lpcszTitle, UINT uTimeout, UINT uStyle, UINT nActionID)
{
	if ( !lpcszTips )
	{
		return FALSE;
	}

	ST_BALLOONTIP stTip;
	ZeroMemory(&stTip, sizeof stTip);

	stTip.nAction = nActionID;
	stTip.uTimeout = uTimeout;

	switch ( uStyle )
	{
	case POPTIPSTYLE_INFORMATION: 
		stTip.Style =NIIF_INFO;
		break;
	case POPTIPSTYLE_WARNING:
		stTip.Style =NIIF_WARNING;
		break;
	case POPTIPSTYLE_ERROR:
		stTip.Style =NIIF_ERROR;
		break;
	default:
		stTip.Style =NIIF_INFO;
	}

	DWORD dwTipLen   = (DWORD)_tcslen(lpcszTips);
	DWORD dwTitleLen = 0;
	if (lpcszTitle)
	{
		dwTitleLen = (DWORD)_tcslen(lpcszTitle);
	}

	ZeroMemory(stTip.szTipInfo, sizeof(stTip.szTipInfo));
	ZeroMemory(stTip.szTipTitle, sizeof(stTip.szTipTitle));

	_tcscpy_s(stTip.szTipInfo, 256, lpcszTips);
	if(lpcszTitle)
	{
		_tcscpy_s(stTip.szTipTitle, 64, lpcszTitle);
	}

	// ��vector���ҵ���û��Action��TRAY_ACTION_FILEMON_CLOSED�ģ�����о͸��ǵ�
	/*if ( TRAY_ACTION_FILEMON_CLOSED == stTip.nAction )
	{
		for ( BALLOON_TIPS::iterator i = m_vtBalloonTips.begin();
				i != m_vtBalloonTips.end(); i++ )
		{
			if ( (*i).nAction == TRAY_ACTION_FILEMON_CLOSED )
			{
				m_vtBalloonTips.erase(i);
				break;
			}
		}
	}*/

	m_vtBalloonTips.push_back(stTip);

	::KillTimer(m_hHideWnd, GETTRAY_TIMERID(TIMER_PopBalloon) );
	::SetTimer(m_hHideWnd, GETTRAY_TIMERID(TIMER_PopBalloon), 100, NULL);

	return TRUE;
}


// ��ʾTray������Ϣ(ͬ����ʽ)
BOOL CSysTray::ShowBalloonimmediately(LPCTSTR lpcszTips, LPCTSTR lpcszTitle, UINT uTimeout, UINT uStyle, UINT nActionID)
{
	if ( !lpcszTips )
	{
		return FALSE;
	}

	ST_BALLOONTIP stTip;
	ZeroMemory(&stTip, sizeof stTip);

	stTip.nAction = nActionID;
	stTip.uTimeout = uTimeout;

	switch ( uStyle )
	{
	case POPTIPSTYLE_INFORMATION: 
		stTip.Style =NIIF_INFO;
		break;
	case POPTIPSTYLE_WARNING:
		stTip.Style =NIIF_WARNING;
		break;
	case POPTIPSTYLE_ERROR:
		stTip.Style =NIIF_ERROR;
		break;
	default:
		stTip.Style =NIIF_INFO;
	}

	ZeroMemory(stTip.szTipInfo, sizeof(stTip.szTipInfo));
	ZeroMemory(stTip.szTipTitle, sizeof(stTip.szTipTitle));

	_tcscpy_s(stTip.szTipInfo, 256, lpcszTips);
	if(lpcszTitle)
	{
		_tcscpy_s(stTip.szTipTitle, 64, lpcszTitle);
	}

	BALLOON_TIPS BalloonTips;
	BalloonTips.push_back(stTip);

	PopBalloon(BalloonTips);

	return TRUE;
}



///////////////////////////////////////////////
// ��ȡtips�����ռ�
// ����ֵ: the size
// Created by Leon @ 2007/05/14
///////////////////////////////////////////////
int CSysTray::GetTipCap()
{
	return sizeof(m_stNID.szTip);
}




/*
						P R I V A T E		A R E A
																			*/

///////////////////////////////////////////////
// ������ϵͳ��ĳ��DLL�����汾
// ����ֵ: �������汾�ţ���Ϊ0��ʾ����
// Created by Leon @ 2005/07/22
///////////////////////////////////////////////
typedef HRESULT (CALLBACK *pfnDllGetVersion)(DLLVERSIONINFO*);
UINT CSysTray::CheckSysDllMajorVersion(LPCTSTR lpcszSystemDllFilename)
{
	if (NULL == lpcszSystemDllFilename)
		return 0;
	
	// ���ش�DLL�������У�����ʧ�ܷ���0
	HMODULE	hDll;
	hDll =::LoadLibrary(lpcszSystemDllFilename);
	if (NULL == hDll)
		return 0;

	// ��ѯ"DllGetVersion"���ţ������ṩ��˵����DLL���ǲ���ϵͳDLL������0
	pfnDllGetVersion DllGetVersion;
	DllGetVersion =(pfnDllGetVersion)::GetProcAddress(hDll, "DllGetVersion");
	if (NULL == DllGetVersion)
	{
		::FreeLibrary(hDll);
		return 0;
	}

	// ����DllGetVersion
	DLLVERSIONINFO	dllvi;
	ZeroMemory(&dllvi, sizeof(dllvi));
	dllvi.cbSize	=sizeof(dllvi);
	if (NOERROR != DllGetVersion(&dllvi) )
	{
		::FreeLibrary(hDll);
		return 0;
	}

	// �ͷ�DLL
	::FreeLibrary(hDll);

	// �������汾��
	return dllvi.dwMajorVersion;
}


///////////////////////////////////////////////
// ��鲢ɾ�����е�ICON
// ����ֵ: N/A
// Created by Leon @ 2005/07/22
///////////////////////////////////////////////
VOID CSysTray::DelIcon()
{
	::Shell_NotifyIcon(NIM_DELETE, &m_stNID);
	if (NULL!=m_stNID.hIcon && NULL!=m_stNID.hWnd)
	{
		if(m_bLoadIcon)
		{
			::DestroyIcon(m_stNID.hIcon);
			m_bLoadIcon = FALSE;
		}
	}

	m_stNID.hIcon =NULL;
	if(m_hEmpty)
	{
		::DestroyIcon(m_hEmpty);
		m_hEmpty = NULL;
	}
}


///////////////////////////////////////////////
// ��vector��ȡ��һ��������ʾ���ݲ���ʾ
// ����ֵ: N/A
// Created by Leon @ 2007/09/06
///////////////////////////////////////////////
VOID CSysTray::PopBalloon(BALLOON_TIPS& vtBalloonTips)
{
	if ( vtBalloonTips.empty() )
		return;
	
	if ( !m_stNID.cbSize ) {
		return;
	}

	memcpy(&m_stCurTip, &(*vtBalloonTips.begin()), sizeof m_stCurTip);
	
	vtBalloonTips.erase(vtBalloonTips.begin());

	if(m_bHideIcon)	//�����ǰ���������صģ�����Ե�
		return;

	m_stNID.dwInfoFlags = m_stCurTip.Style;
	m_stNID.uFlags	    = NIF_INFO;
	m_stNID.uTimeout	= m_stCurTip.uTimeout;

	ZeroMemory(m_stNID.szInfo, sizeof(m_stNID.szInfo));
	ZeroMemory(m_stNID.szInfoTitle, sizeof(m_stNID.szInfoTitle));

	_tcscpy_s(m_stNID.szInfo, 256, m_stCurTip.szTipInfo);
	_tcscpy_s(m_stNID.szInfoTitle, 64, m_stCurTip.szTipTitle);

#if (NTDDI_VEMSION >= NTDDI_LONGHORN)
	//m_stNID.hBalloonIcon = m_stNID.hIcon;
	#error _WIN32_WINNT >= 0x0600 ���ݳ�����
#endif

	// ���������~
	::Shell_NotifyIcon(NIM_MODIFY, &m_stNID);

	m_bBalloonShowing = TRUE;
}

BOOL CSysTray::ShowTray(bool bShow/* = TRUE*/)
{
	m_stNID.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;

	m_bHideIcon = !bShow;
	m_bBalloonShowing = FALSE;	
	return Shell_NotifyIcon(bShow?NIM_ADD:NIM_DELETE, &m_stNID);
}

///////////////////////////////////////////////
// ���ش�����Ϣ�����������ڽ��ղ��ַ�TrayIcon
// ���ݹ�������Ϣ��
// ����ֵ: N/A
// Created by Leon @ 2005/07/22
///////////////////////////////////////////////
LRESULT CSysTray::WinProc(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandle)
{
	// ������Ϣ��������ITrayEvents�ӿڵ���Ӧ����
	POINT	CurPos;
	GetCursorPos(&CurPos);

	if(NULL == m_pEvents)
		return 0;

	if ( m_nMsgID ==msg && m_stNID.uID ==(UINT)wParam)
	{
		// ������TrayIcon���ݹ�������Ϣ
		if ( WM_LBUTTONDOWN == lParam )
		{
			::SetTimer(GetWnd(), GETTRAY_TIMERID(TIMER_LBUTTONDOWN), ::GetDoubleClickTime(), NULL);
			return 0;
		}
		else if ( WM_RBUTTONDOWN == lParam )
		{
			m_pEvents->OnTrayIconMsg(lParam, CurPos.x, CurPos.y, bHandle);
			m_bHoldRButton = TRUE;
			return 0;
		}
		else if ( WM_RBUTTONUP == lParam )
		{
			if ( m_bHoldRButton )
			{
				::SetForegroundWindow(m_stNID.hWnd);	
				m_pEvents->OnShowRightButtonMenu(CurPos.x, CurPos.y, bHandle);// ��ʾ�Ҽ��˵�
				::PostMessage(m_stNID.hWnd,WM_NULL, 0, 0);
			}

			m_bHoldRButton = FALSE;	
			return 0;
		}
		else if ( WM_LBUTTONDBLCLK == lParam )
		{
			::KillTimer(GetWnd(), GETTRAY_TIMERID(TIMER_LBUTTONDOWN) );
			m_pEvents->OnTrayIconMsg(lParam, CurPos.x, CurPos.y, bHandle);// ˫�����
			return 0;
		}
		else if(WM_RBUTTONDBLCLK == lParam )
		{
			::KillTimer(GetWnd(), GETTRAY_TIMERID(TIMER_LBUTTONDOWN) );
			m_pEvents->OnTrayIconMsg(lParam, CurPos.x, CurPos.y, bHandle);// ˫���Ҽ�
			return 0;
		}
		else if ( WM_MOUSEMOVE == lParam )
		{
			GetCursorPos(&m_ptMouse);
			if (!m_bSetMouseLeaveTimer)
			{
				::SetTimer(GetWnd(), GETTRAY_TIMERID(TIMER_MouseLeave), MouseHover_Timer_Tick, NULL);
				m_bSetMouseLeaveTimer = TRUE;
			}
			m_pEvents->OnTrayIconMsg(lParam, CurPos.x, CurPos.y, bHandle);// ����ƶ�
			return 0;
		}
		else if ( NIN_BALLOONUSERCLICK == lParam )
		{
			// �û��������������,����hiden
			m_pEvents->OnBalloonClicked(m_stCurTip.nAction, bHandle);
			m_bBalloonShowing = FALSE;
		}
		else if ( NIN_BALLOONSHOW == lParam )
		{
			// ����shown
			m_bBalloonShowing = TRUE;
		}
		else if ( NIN_BALLOONHIDE == lParam )
		{
			// �û�������ݹرհ�ť������hiden
			m_bBalloonShowing = FALSE;
		}
		else if ( NIN_BALLOONTIMEOUT == lParam )
		{
			// ���ݳ�ʱ��hiden
			m_bBalloonShowing = FALSE;
		}

		return 0;
	}
	else if ( WM_TIMER == msg )
	{	
		// ����Timer��Ϣ
		if ( GETTRAY_TIMERID(TIMER_LBUTTONDOWN) == wParam ) 
		{
			::KillTimer(GetWnd(), GETTRAY_TIMERID(TIMER_LBUTTONDOWN));
			m_pEvents->OnTrayIconMsg(WM_LBUTTONDOWN, CurPos.x, CurPos.y, bHandle);
			if ( !bHandle )
			{
				::SetForegroundWindow(m_stNID.hWnd);
				m_pEvents->OnShowLeftButtonMenu(CurPos.x, CurPos.y, bHandle);// ��ʾ����˵�
				::PostMessage(m_stNID.hWnd,WM_NULL, 0, 0);
			}
		}
		else if ( GETTRAY_TIMERID(TIMER_PopBalloon) == wParam )
		{
			if ( !m_bBalloonShowing )
			{
				::KillTimer( GetWnd(), GETTRAY_TIMERID(TIMER_PopBalloon) );
				PopBalloon(m_vtBalloonTips);
				if(m_vtBalloonTips.size() > 0)
					::SetTimer(GetWnd(), GETTRAY_TIMERID(TIMER_PopBalloon), 1000, NULL);
				if (SHELL_VEMSION < 6)//�����֧�����ݵ���볬ʱ��Ϣ��ô�Դ���
				{
					::SetTimer(GetWnd(), GETTRAY_TIMERID(TIMER_KILLBalloon), 5000, NULL);
				}	
			}
		}
		else if (GETTRAY_TIMERID(TIMER_KILLBalloon) == wParam)
		{
			if (m_bBalloonShowing)
			{
				m_bBalloonShowing = FALSE;
			}
			KillTimer(GetWnd(), GETTRAY_TIMERID(TIMER_KILLBalloon));
		}
		else if( GETTRAY_TIMERID(TIMER_Flicker) == wParam ) //����������ͼ��
		{
			FlickerTray();
		}
		else if (GETTRAY_TIMERID(TIMER_MouseLeave) == wParam)
		{
			POINT		ptMouse;
			GetCursorPos(&ptMouse);
			if(ptMouse.x != m_ptMouse.x || ptMouse.y != m_ptMouse.y)
			{
				KillTimer(GetWnd(), GETTRAY_TIMERID(TIMER_MouseLeave));
				m_pEvents->OnTrayIconMsg(WM_MOUSELEAVE, CurPos.x, CurPos.y, bHandle);
				m_bSetMouseLeaveTimer = FALSE;
				m_bNotifyMouseHover = FALSE;
				m_dwMouseHoverCount = 0;
			}
			else
			{
				m_dwMouseHoverCount ++;
				if (!m_bNotifyMouseHover && m_dwMouseHoverCount*MouseHover_Timer_Tick > 800)
				{
					m_bNotifyMouseHover = TRUE;
					m_dwMouseHoverCount = 0;
					m_pEvents->OnTrayIconMsg(WM_MOUSEHOVER, CurPos.x, CurPos.y, bHandle);
				}
			}
		}
	}
	else if ( WM_KEYUP == msg )
	{
		// ����ESC��Enter����Ϣ
		if ( VK_ESCAPE == wParam || VK_RETURN == wParam )
		{
			::Shell_NotifyIcon(NIM_SETFOCUS, &m_stNID);
		}
	}
	else if (WM_TASKBARCREATED == msg)
	{
		// Tray�ؽ���Ϣ��һ������Explorer.exe���������õ�����Ϣ
		// �ڴ����ǽػ����Ϣ��������ʾTray
		m_stNID.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
		if (!m_bHideIcon  && !::Shell_NotifyIcon(NIM_ADD, &m_stNID) )
		{
		}
		m_bBalloonShowing = FALSE;

		return 0;
	}
	
	//else //ֻҪǰ��û��return����Ϣ����һ��OnOtherMsg
	{
		bHandle = FALSE;	//�������˾���Ϊû�д���
		return m_pEvents->OnOtherMsg(msg, wParam, lParam, bHandle);
	}

	return 0;
}

// ����explorer��ʲô�����ˣ����ڵ���trayicon����
VOID CSysTray::AnalyzeExplorer()
{
	return;

	//����������ڵ��ԣ��о���release��ʹ��

	HWND hDesktop = NULL;

	hDesktop = ::FindWindow(NULL, TEXT("Program Manager"));
	if ( hDesktop && ::IsWindow(hDesktop) )
	{
		DWORD dwResult = 0;
		if ( !SendMessageTimeout(hDesktop, WM_NULL, 0, 0, SMTO_ABORTIFHUNG, 1000, &dwResult) )
		{
			dwResult = GetLastError();
			if ( ERROR_TIMEOUT == dwResult )
			{
			}
			else
			{
			}
		}
	}
	else
	{
	}
}