#pragma once

namespace DuiKit{;

struct IDuiCore;
struct IDuiSkin;
struct IDuiLangSet;
struct IDuiControlCtrl;
struct IDuiBuilder;
struct DuiMsg
{
	IDuiControlCtrl* CtrlFrom;
	IDuiControlCtrl* CtrlTo;
	INT		 nMsg;
	WPARAM	 wParam;
	LPARAM	 lParam;
	LPVOID	 lpData;
};

struct DuiEvent
{
	IDuiControlCtrl* Ctrl; //�¼��Ĵ�����
	INT nEvent;
	WPARAM wParam;
    LPARAM lParam;
};

struct IDuiPreMessageFilter
{
	virtual LRESULT OnPreMessageFilter(const DuiMsg& duiMsg, BOOL& bHandle) = 0;
};

struct IDuiProMessageFilter
{
	virtual LRESULT OnProMessageFilter(const DuiMsg& duiMsg, BOOL& bHandle) = 0;
};

struct IDuiTranslateAccelerator
{
	virtual LRESULT OnTranslateAccelerator(const MSG& pMsg, BOOL& bHandle) = 0;
};

struct IDuiRoutedEventHandler
{
	/*����FALSE ��ֹ���ϴ���*/
	virtual BOOL OnRoutedEvent(const DuiEvent& event) = 0;
};

//���ж���Ӧ�̳иû���
struct IDuiObject
{
	virtual LPCWSTR GetObjectClassName() = 0;
	virtual IDuiObject* QueryInterface(IIDef iid) = 0;
	virtual VOID DeleteThis() = 0;

	//Ϊ�˷����XMLֱ����������
 	virtual VOID SetAttribute(LPCWSTR lpszName, LPCWSTR lpszValue) = 0;

	/*
		����ֵ��S_OK , S_FALSE , E_FAIL
			 S_OK	 ���ɹ��������ӽڵ�
			 S_FALSE ��ʧ�ܣ������ӽڵ㣬�ڽ�������ӽڵ�֮��ɾ����
			 E_FAIL	 ��ʧ�ܣ��������ӽڵ㣬����ɾ����
	*/
 	virtual HRESULT DoCreate(IDuiObject* Parent, IDuiCore* Core, IDuiBuilder* pBuilder) = 0;
};

//���󴴽��ص�
struct IDuiObjectBuilder
{
	virtual IDuiObject* CreateObject() = 0;
	virtual VOID	   DestoryObject(IDuiObject* Object) = 0;
};



//��Ϊȫ�ֶ��󷵻�
struct IDuiObjectFactory
{
	virtual IDuiObject* CreateObject(LPCWSTR lpszObjectClassName) = 0;
	virtual VOID	   DestoryObject(IDuiObject* pObject) = 0;

	virtual VOID	   RegisterObjectBuilder(LPCWSTR lpszObjectClassName, IDuiObjectBuilder* pCallBack) = 0;
	virtual VOID	   UnRegisterObjectBuilder(LPCWSTR lpszObjectClassName) = 0;
};


struct IDuiCoreCallBack
{
	virtual LRESULT OnCoreCallBack( IDuiObject* pCtrl, int nCallBack) = 0;
};

/*
���Ľӿ�
*/
struct IDuiCore : public IDuiObject
{
	virtual BOOL InitDuiCore() = 0;
	virtual BOOL UnInitDuiCore() = 0;

	/*�������ͷŲ���*/
	virtual IDuiObject* GetSignalObject(LPCWSTR lpszName) = 0;
	virtual VOID AddSignalObject(LPCWSTR lpszName, IDuiObject* pObject) = 0;
	virtual VOID RemoveSignalObject(LPCWSTR lpszName) = 0;
	

	virtual VOID SetModuleInstance(HINSTANCE hInstance) = 0;
	virtual HINSTANCE GetModuleInstace() = 0;


	/*ȫ����Ϣ����*/
	virtual VOID AddPreMessageFilter(IDuiPreMessageFilter* pFilter) = 0;
	virtual VOID RemovePreMessageFilter(IDuiPreMessageFilter* pFilter) = 0;

	virtual VOID AddProMessageFilter(IDuiProMessageFilter* pFilter) = 0;
	virtual VOID RemoveProMessageFilter(IDuiProMessageFilter* pFilter) = 0;

	virtual VOID AddTranslateAccelerator(IDuiTranslateAccelerator* pFilter) = 0;
	virtual VOID RemoveTranslateAccelerator(IDuiTranslateAccelerator* pFilter) = 0;

	virtual BOOL PostMessage(IDuiControlCtrl* pCtrl,INT nMsg,IDuiControlCtrl* pToCtrl = NULL,WPARAM wParam = NULL,LPARAM lParam = NULL,LPVOID	lpData = NULL) = 0;
	virtual LRESULT SendMessage(IDuiControlCtrl* pCtrl,INT nMsg,IDuiControlCtrl* pToCtrl = NULL,WPARAM wParam = NULL,LPARAM lParam = NULL,LPVOID	lpData = NULL) = 0;

	/*����Ƥ��*/

	/*Ƥ������*/
	virtual IDuiSkin* GetSkin(LPCWSTR lpszSkinName) = 0;
	virtual VOID AddSkin( LPCWSTR lpszSkinName, IDuiSkin* pTheme ) = 0;
	virtual VOID RemoveSkin(LPCWSTR lpszSkinName) = 0;

	//���õ�ǰ��������
	virtual VOID SetCurrentLang(LPCWSTR lpszLangName) = 0;
	virtual LPCWSTR GetCurrentLang() = 0;


	virtual DWORD MessageLoop() = 0;
	virtual BOOL  Term(DWORD dwExitCode) = 0;
};

};