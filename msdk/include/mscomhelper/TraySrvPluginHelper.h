

#ifndef _MSTRAYSRVPLUGIN_HELPER_H_
#define _MSTRAYSRVPLUGIN_HELPER_H_

namespace msdk {

template<class T>
class CTraySrvPluginHelper : public ITraySrvPlugin
{
public:
	UTIL::com_ptr<ITraySrv>		m_pTraySrv;		//���̷���

	virtual ~CTraySrvPluginHelper(){ReleaseTraySrv();}

	//�������������TraySrv�����
	STDMETHOD(SetTraySrv)(ITraySrv* pTraySrv)
	{
		RASSERT(pTraySrv, E_POINTER);

		m_pTraySrv = pTraySrv;
		RASSERT(m_pTraySrv, E_INVALIDARG);

		return S_OK;
	}

	//�䱻TrayMsg���ʹ�õ����ͨ����������õ�ITraySrv
	STDMETHOD(GetTraySrv)(ITraySrv** pTraySrv)
	{
		RASSERT(pTraySrv, E_POINTER);
		RASSERT(m_pTraySrv, E_INVALIDARG);

		return m_pTraySrv->QueryInterface(__uuidof(ITraySrv), (void**)pTraySrv);
	}

	HRESULT ReleaseTraySrv()
	{
		if(m_pTraySrv)
			m_pTraySrv = INULL;

		return S_OK;
	}

	ITraySrv* operator -> () const {return m_pTraySrv;}
	operator ITraySrv* () const {return m_pTraySrv;}
};

} //namespace msdk

#endif //_MSTRAYSRVPLUGIN_HELPER_H_