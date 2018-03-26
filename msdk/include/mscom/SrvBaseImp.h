#pragma once

/**********************************************************************
ͷ�ļ�˵����
	ʵ��MSCOM���������
ͷ�ļ����ã�
	͸���������ϵ�е�MSC��Ϣ����
***********************************************************************/

#define USE_MARGINCOM
#include "mscom\srvbase.h"
#include "msc\XMSCProxy.h"


/******��������*****/
#define EnvInfo_MsgCenterNameSpace "msgcrt"


#define MSC_BEGIN_DEFINE_MSG(_type) \
	virtual DWORD GetSubscribeID(DWORD dwCount, RSMSGID* lpMessageID)\
	{\
		RSMSGID msgid[] = {
#define MSC_DEFINE_MSG(x)	x,

#define MSC_END_DEFINE_MSG \
						  };\
		if(!dwCount || !lpMessageID)\
		{\
			return SIZEOF_BUFFER(msgid);\
		}\
		memcpy(lpMessageID , msgid , sizeof(msgid));\
		return SIZEOF_BUFFER(msgid);\
	}

#define MSC_BEGIN_MSG_MAP(_type)\
	virtual HRESULT OnMsg(RSMSGID dwMessageID,IMsBuffer* lpXMessage)\
	{\
	switch (dwMessageID)\
		{
#define MSC_MSG_MAP(msgid , func)\
	case msgid:\
	return func(dwMessageID , lpXMessage);

#define MSC_DEFAULT_MSG(func)\
	default:\
	return func(dwMessageID , lpXMessage);

#define MSC_END_MSG_MAP\
		}\
	return S_OK;}




template<typename _TypeBase>
class CSrvPluginImp:
	public IMsPlugin,
	public IMsPluginRun,
	public XMSCDispatcher,
	public virtual CMsComBase<CSrvPluginImp<_TypeBase>>
{
public:
	UNKNOWN_IMP2_( IMsPlugin , IMsPluginRun);
	//��Ҫʵ�����
	virtual HRESULT OnAfterInit(void*){return S_OK;}
	virtual HRESULT OnBeforeUnint(){return S_OK;}
	virtual HRESULT OnAfterStart(){return S_OK;}
	virtual HRESULT OnBeforeStop(){return S_OK;}

	//�����Ҫʵ�ֽ�����Ϣ��ʵ��������������
	virtual DWORD GetSubscribeID(DWORD dwCount, RSMSGID* lpMessageID) {return 0;};
	virtual HRESULT OnMsg(RSMSGID dwMessageID,IMsBuffer* lpXMessage){return S_OK;}

	STDMETHOD(Init)(void*p)
	{
		
		RASSERT(m_pRot , E_FAIL);

		RFAILED(m_pRot->CreateInstance(CLSID_MsgBusProxy , NULL , re_uuidof(IXMsgBusProxy) , (void**)&m_pProxy));
		RASSERT(m_pProxy , E_FAIL);
		
		return OnAfterInit(p);
	}

	STDMETHOD(Uninit)()
	{
		HRESULT hr = OnBeforeUnint();
		SAFE_RELEASE(m_pRot);
		SAFE_RELEASE(m_pProxy);
		return hr;
	}

	STDMETHOD(Start)()
	{
		
		RASSERT(m_pProxy , E_FAIL);

		RASSERT(m_pProxy->Login(0 , this , TRUE , INFINITE , GetEnvParamString(EnvInfo_MsgCenterNameSpace)) ,E_FAIL);
		DWORD dwIdCount = GetSubscribeID(0 , NULL);

		if (dwIdCount)
		{
			RSMSGID* lpMessageID = new RSMSGID[dwIdCount];
			GetSubscribeID(dwIdCount , lpMessageID);
			BOOL bRet = m_pProxy->Subscribe(dwIdCount , lpMessageID);
			SAFE_DELETE_BUFFER(lpMessageID);
		}
		
		
		return OnAfterStart();
	}

	STDMETHOD(Stop)()
	{
		RASSERT(m_pProxy , E_FAIL);
		m_pProxy->Logoff();

		DWORD dwIdCount = GetSubscribeID(0 , NULL);
		if (dwIdCount)
		{
			RSMSGID* lpMessageID = new RSMSGID[dwIdCount];
			GetSubscribeID(dwIdCount , lpMessageID);
			m_pProxy->UnSubscribe(dwIdCount , lpMessageID);
			SAFE_DELETE_BUFFER(lpMessageID);
		}
		
		return OnBeforeStop();
	}

	HRESULT Issue(RSMSGID dwMessageID,IMsBuffer* lpXMessage = NULL)
	{
		RASSERT(m_pProxy , E_FAIL);
		return m_pProxy->Issue(dwMessageID , lpXMessage) ? S_OK : E_FAIL;
	}

	HRESULT Issue(RSMSGID dwMessageID,LPVOID lpXMessage, DWORD dwLength)
	{
		RASSERT(m_pProxy , E_FAIL);
		UTIL::com_ptr<IMsBuffer> pBuf;
		RFAILED(CreateBuffer(&pBuf.m_p, dwLength));
		RFAILED(pBuf->SetBuffer((LPBYTE)lpXMessage, dwLength));
		return m_pProxy->Issue(dwMessageID , pBuf) ? S_OK : E_FAIL;
	}

	HRESULT Issue(RSMSGID dwMessageID,LPCWSTR lpXMessage)
	{
		RASSERT( lpXMessage && wcslen(lpXMessage), E_INVALIDARG);
		DWORD dwLenght = (wcslen(lpXMessage) + 1) * sizeof(wchar_t);
		return Issue(dwMessageID, (LPVOID)lpXMessage, dwLenght);
	}

	HRESULT Issue(RSMSGID dwMessageID,LPCSTR lpXMessage)
	{
		RASSERT( lpXMessage && strlen(lpXMessage), E_INVALIDARG);
		DWORD dwLenght = strlen(lpXMessage);
		return Issue(dwMessageID, (LPVOID)lpXMessage, dwLenght);
	}

	virtual VOID WINAPI Handle(RSMSGID dwMessageID,IMsBuffer* lpXMessage)
	{
		OnMsg(dwMessageID , lpXMessage);
	}

	/*
		�����Ӧ�õĳ����������ʵ�ָķ������Ա��������˳�����
	*/
	virtual VOID WINAPI FireConnectBroken()
	{

		//��ʱֱ���˳�
		//OutputDebugString(_T("FireConnectBroken\r\n"));
	}

private:
	UTIL::com_ptr<IXMsgBusProxy> m_pProxy;
};