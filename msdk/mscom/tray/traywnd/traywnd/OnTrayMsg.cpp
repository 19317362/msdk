#include "StdAfx.h"
#include "OnTrayMsg.h"
#include "tray\traymenudef.h"

COnTrayMsg::COnTrayMsg(void)
{
}

COnTrayMsg::~COnTrayMsg(void)
{
}

STDMETHODIMP COnTrayMsg::init_class(IRSBase* prot, IRSBase* punkOuter)
{
	m_pRot = prot;	//��rot��������
	RASSERT(m_pRot, E_FAIL);

	m_pRot->GetObject(ClSID_CTrayWnd, __uuidof(ITrayWnd), (IRSBase**)&m_pTrayWnd);

	return S_OK;
}

STDMETHODIMP COnTrayMsg::Init(void*)
{
	UTIL::com_ptr<IRsConnectionPointContainer> p = this;
	m_OnMenuMsgConnectPoint.SetInfo(__uuidof(IOnMenuMsgConnectPoint), p);
	m_OnMenuMsgConnectPoint.AddRef(); //����һ�����ü���

	UTIL::com_ptr<IRsConnectionPoint> pPoint(&m_OnMenuMsgConnectPoint);
	AddConnectionPoint(__uuidof(IOnMenuMsgConnectPoint), pPoint);

	return S_OK;
}

STDMETHODIMP COnTrayMsg::Uninit()
{
	//�Ͽ����ӵ�
	if(m_pOnTrayMsgConnectPoint)
	{
		m_pOnTrayMsgConnectPoint->Unadvise(m_dwOnTrayMsgConnectPointCookie);
		m_pOnTrayMsgConnectPoint = INULL;
	}

	m_OnMenuMsgConnectPoint.EmptyConnection();	//�����������
	DelConnectionPoint(__uuidof(IOnMenuMsgConnectPoint));

	if(m_pTrayWnd)
		m_pTrayWnd = INULL;

	if(m_pTraySrv)
		m_pTraySrv = INULL;

	if(m_pRot)
		m_pRot = INULL;

	return S_OK;
}

STDMETHODIMP COnTrayMsg::SetTraySrv(ITraySrv* pTraySrv)
{
	RASSERT(pTraySrv, E_POINTER);

	m_pTraySrv = pTraySrv;
	RASSERT(m_pTraySrv, E_INVALIDARG);

	//////////////////////////////////////////////////////////////////////////
	UTIL::com_ptr<IRsConnectionPointContainer> pCPC(m_pTraySrv);
	RASSERT(pCPC, E_FAIL);

	RFAILED(pCPC->FindConnectionPoint(__uuidof(ITrayMsgConnectPoint), (IRsConnectionPoint**)&m_pOnTrayMsgConnectPoint));
	RASSERT(m_pOnTrayMsgConnectPoint, E_FAIL);

	UTIL::com_ptr<ITrayMsgConnectPoint> pMCP(this);
	RFAILED(m_pOnTrayMsgConnectPoint->Advise(pMCP, &m_dwOnTrayMsgConnectPointCookie));
	//////////////////////////////////////////////////////////////////////////

	return S_OK;
}

STDMETHODIMP COnTrayMsg::GetTraySrv(ITraySrv** pTraySrv)
{
	RASSERT(pTraySrv, E_POINTER);
	RASSERT(m_pTraySrv, E_INVALIDARG);

	return m_pTraySrv->QueryInterface(__uuidof(ITraySrv), (void**)pTraySrv);
}

//����˵�
LRESULT	COnTrayMsg::OnShowLeftButtonMenu(INT x, INT y, BOOL& bHandle)
{
	return 0;
}

//�Ҽ��˵�
LRESULT COnTrayMsg::OnShowRightButtonMenu(INT x, INT y, BOOL& bHandle)
{
	return 0;
}

LRESULT COnTrayMsg::OnMenuCommand(WORD nMenuId, BOOL &bHandle)
{
	return 0;
}

LRESULT COnTrayMsg::OnMenuCommand(int nMenuIndex, HMENU hMenu, BOOL &bHandle)
{
	return 0;
}

//����ͼ����Ϣ
LRESULT COnTrayMsg::OnTrayIconMsg(LPARAM lParam, INT x, INT y, BOOL& bHandle)
{
	return 0;
}

//���򱻵��
LRESULT	COnTrayMsg::OnBalloonClicked(UINT nActionID, BOOL& bHandle)
{
	return 0;
}

//������Ϣ
LRESULT COnTrayMsg::OnOtherMsg(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandle)
{
	return 0;
}