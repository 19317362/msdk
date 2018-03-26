#pragma once
/*
ע������ע�⣺
�ڱ�ͷ�ļ��У����С������һ��Ҫ��ѭ˭����˭�ͷŵ�ԭ��
*/
namespace msdk{
namespace mscom{
/*
	�ɿ��ٴ���������
	���ʵ���˲���ӿڣ��������Ӧ�ĳ�ʼ��������ʼ����
*/
template<typename I = IMSBase>
class CMsCreateObject
{
protected:
	UTIL::com_ptr<I> m_pObject;
	virtual void Destory(){
		if(m_pObject) {
			m_pObject = INULL;
		}
	}
public:
	CMsCreateObject(){
		m_pObject = INULL;
	}

	CMsCreateObject(IMscomRunningObjectTable* pRot ,REFCLSID rclsid , IMSBase *punkOuter = NULL){
		CreateInstance(pRot , rclsid , punkOuter);
	}

	virtual ~CMsCreateObject(){
		Destory();
	}

	I* operator->(){
		return m_pObject;
	}

	I* operator*(){
		return m_pObject;
	}

	virtual I* operator()(IMscomRunningObjectTable* pRot ,REFCLSID rclsid , IMSBase *punkOuter = NULL){
		CreateInstance(pRot , rclsid , punkOuter);
		return m_pObject;
	}
	CMsCreateObject( CMsCreateObject& obj){
		this->m_pObject = obj.m_pObject;
		obj.Destory();
	}

	CMsCreateObject(const I* p){
		m_pObject = p;
	}

	CMsCreateObject operator=(const I* p){
		this->m_pObject = p;
		return *this;
	}

	virtual HRESULT CreateInstance(IMscomRunningObjectTable* pRot, REFCLSID rclsid, IMSBase *punkOuter = NULL) {
		RASSERT(pRot, E_FAIL);
		RASSERT(rclsid != GUID_NULL, E_NOINTERFACE);
		UTIL::com_ptr<IMsObjectLoader> pObjectLoader = INULL;
		RFAILED(pRot->GetObject(CLSID_CObjectLoader, re_uuidof(IMsObjectLoader), (IMSBase**)&pObjectLoader));
		RASSERT(pObjectLoader, E_FAIL);
		RFAILED(pObjectLoader->CreateInstance(pRot, rclsid, punkOuter , re_uuidof(I), (void**)&m_pObject));
		RASSERT(m_pObject, E_FAIL);
		return S_OK;
	}

};

template<typename I>
class CMsCreatePluginObject : public CMsCreateObject<I>
{
	
public:
	~CMsCreatePluginObject(){
		Destory();
	}

	CMsCreatePluginObject(IMscomRunningObjectTable* pRot, REFCLSID rclsid, IMSBase *punkOuter = NULL){
		CreateInstance(pRot,  rclsid,  punkOuter);
	}

	HRESULT Init(){
		UTIL::com_ptr<IMsPlugin> pPlugin(m_pObject);
		if(pPlugin) {
			RFAILED(pPlugin->Init(NULL));
		}
		return S_OK;
	}

	HRESULT UnInit(){
		UTIL::com_ptr<IMsPlugin> pPlugin(m_pObject);
		if(pPlugin) {
			RFAILED(pPlugin->Init(NULL));
		}
		return S_OK;
	}

	HRESULT CreateInstance(IMscomRunningObjectTable* pRot, REFCLSID rclsid, IMSBase *punkOuter = NULL) {
		RFAILED(CMsCreateObject<I>::CreateInstance(pRot , rclsid , punkOuter));
		return Init();
	}

	 CMsCreatePluginObject( CMsCreatePluginObject& obj){
		Destory();
		this->m_pObject = obj.m_pObject;
		obj.Destory();
	}

	CMsCreatePluginObject(CMsCreateObject& obj){
		Destory();
		m_pObject = obj.m_pObject;
		Init();
		obj.Destory();
	}

	CMsCreatePluginObject(const I* p){
		m_pObject = p;

	}

	CMsCreatePluginObject operator=(const I* p){
		this->m_pObject = p;
		return *this;
	}

private:
	//���ã���Ϊ�ǻ��ڲ��������Ҫ���г�ʼ���뷴��ʼ��������
	I* operator()(IMscomRunningObjectTable* pRot ,REFCLSID rclsid , IMSBase *punkOuter = NULL){
		return NULL;
	}

	void Destory(){
		UnInit();
		CMsCreateObject<I>::Destory();
	}
};

















}
}
