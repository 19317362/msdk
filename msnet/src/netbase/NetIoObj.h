#pragma once



#include "mscom/mscominc.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <vector>
#include "..\..\include\netbase.h"
using namespace msnet;
typedef boost::asio::io_service AsioService;
typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr;

//�ӿ��ع��ڲ�ʹ��
struct INetIoSrv : public IMSBase
{

	STDMETHOD_(AsioService& ,GetIoService)() = 0;
};
MS_DEFINE_IID(INetIoSrv , "{51540928-CD21-408F-91E0-DF3A53E385A3}");

//////////////////////////////////////////////////////////////////////////
//�����ڴ�ͳ�����Ͻ�����������Ϊ�˼��׶����۾���������ְ�
class CNetIoObj : public INetIoObj,
	public INetIoSrv,
	private CUnknownImp
{
public:
	UNKNOWN_IMP2_(INetIoObj , INetIoSrv );
	HRESULT init_class(IMSBase* , IMSBase*)
	{
		return S_OK;
	}
	CNetIoObj()
	{
		m_dwTdPool = 1;
	}

	~CNetIoObj()
	{
		Uninit();
	}

	STDMETHOD(Init)()
	{
		try
		{
			//�������Ҫ��һ��work��������֤��ɶ˿�һֱ�ڵ��Ž���������ͻ������Ľ���
			work_ptr work(new boost::asio::io_service::work(m_io_service));
			m_pWork = work;

			for (std::size_t i = 0; i < m_dwTdPool; ++i)
			{
				boost::thread*pThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
				boost::shared_ptr<boost::thread> thread_obj(pThread);
				m_threads.push_back(thread_obj);
			}
		}
		catch (std::exception& e)
		{
			ZM1_GrpError(GrpName , _T("netioobj init failed the error test:[%s]"), e.what());
			return E_FAIL;
		}

		return S_OK;
	}

	STDMETHOD(Uninit)()
	{
		m_pWork.reset();
		m_io_service.stop();

		// Wait for all threads in the pool to exit.
		for (std::size_t i = 0; i < m_threads.size(); ++i)
		{
			m_threads[i]->join();
			m_threads[i].reset();
		}
		m_threads.clear();

		m_io_service.reset();
		return S_OK;
	}

	//INetIoObj
	STDMETHOD(SetThreadPoolCount)(DWORD dwTdPool)
	{
		RTEST(dwTdPool <= 0 , E_INVALIDARG);
		m_dwTdPool = dwTdPool;
		return S_OK;
	}

	//INetIoSrv
	STDMETHOD_(AsioService&, GetIoService)()
	{
		return m_io_service;
	}

private:

	std::vector<boost::shared_ptr<boost::thread>> m_threads;
	work_ptr m_pWork;
	AsioService m_io_service;
	DWORD m_dwTdPool;
};
