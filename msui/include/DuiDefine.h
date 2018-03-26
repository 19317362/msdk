#pragma once


#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif


#ifndef lengthof
#define lengthof(x) (sizeof(x)/sizeof(*x))
#endif

#ifndef MAX
#define MAX max
#endif

#ifndef MIN
#define MIN min
#endif

#ifndef CLAMP //ȡ�м���
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))
#endif



#define RASSERT(x, _h_r_) { if(!(x)) return _h_r_; }
#define RASSERTV(x) { if(!(x)) return; }
#define RASSERTP(x, _h_r_) { if(NULL==(x)) return _h_r_; }
#define RASSERTPV(x) RASSERTP(x, ; )
#define RASSERT2(x, _t) { if(!(x)) {_t;} }
#define TASSERT(x, e) { if (!(x)) throw (e); }
#define RTEST(x, _h_r_) { if((x)) return _h_r_; }

#define DEFINE_OBJECT(_C, _N) static DuiKit::CUIObjectFactoryRegistHelp<_C> __g__##_C(_N);

//#define DEFINE_OBJECT_EX(_C, _I, _N) DuiKit::CUIObjectFactoryRegistHelp<_C, _I> __g__##_C(_N);


//////////////////////////////////////////////////////////////////////////
#define DUI_BEGIN_DEFINE_INTERFACEMAP(_I, _CN)\
	public:\
	virtual LPCWSTR GetObjectClassName(){return _CN;}\
	virtual VOID DeleteThis(){delete this;}\
	virtual IDuiObject* QueryInterface(IIDef __iid) {\
		_I* p = (_I*)(this);\
		if( __iid == IIDuiObject){ return (IDuiObject*)(p);}\
		
	
#define DUI_DEFINE_INTERFACE(_I, _IIDef)\
	if( __iid == _IIDef){ return (IDuiObject*)((_I*)this);}\

#define DUI_END_DEFINE_INTERFACEMAP return NULL; }
//////////////////////////////////////////////////////////////////////////

#define DUI_BEGIN_SETATTRIBUTE(__THIS) \
	virtual VOID SetAttribute(LPCWSTR lpszName, LPCWSTR lpszValue) {\
	if ( !(lpszName && wcslen(lpszName) && lpszValue && wcslen(lpszValue))){ return ;}

#define DUI_SETATTRIBUTE_STRING(__NAME, _VALUE)\
	if (_wcsicmp(lpszName,  L#__NAME) == 0)  { _VALUE = lpszValue;}
	
#define DUI_SETATTRIBUTE_STRING_FUNCTION(__NAME, _FUNCTION)\
	if (_wcsicmp(lpszName,  L#__NAME) == 0)  { _FUNCTION(lpszValue);}

#define DUI_SETATTRIBUTE_INT(__NAME, _VALUE)\
	if (_wcsicmp(lpszName,  L#__NAME) == 0)  \
	{ if(lpszValue[0] == L'#')  _VALUE = wcstoul(lpszValue+1, NULL, 16); else _VALUE = _wtoi(lpszValue);}

#define DUI_SETATTRIBUTE_INT_FUNCTION(__NAME, _FUNC)\
	if (_wcsicmp(lpszName,  L#__NAME) == 0)  \
	{ if(lpszValue[0] == L'#') _FUNC(wcstoul(lpszValue+1, NULL, 16)); else _FUNC(_wtoi(lpszValue));}


#define DUI_SETATTRIBUTE_BOOL(__NAME, _VALUE)\
	if (_wcsicmp(lpszName,  L#__NAME) == 0)  { _VALUE = _wtoi(lpszValue) == 0 ? FALSE : TRUE;}

#define DUI_SETATTRIBUTE_BOOL_FUNCTION(__NAME, _FUNCTION)\
	if (_wcsicmp(lpszName, L#__NAME) == 0)  { _FUNCTION(_wtoi(lpszValue) == 0 ? FALSE : TRUE);}

#define DUI_SETATTRIBUTE_SIZE(__NAME, _VALUE)\
	if (_wcsicmp(lpszName,  L#__NAME) == 0)  { swscanf_s(lpszValue,L"%d,%d", &_VALUE.cx, &_VALUE.cy);}

#define DUI_SETATTRIBUTE_SIZE_FUNCTION(__NAME, _FUNCTION)\
	if (_wcsicmp(lpszName,  L#__NAME) == 0)  { CDuiSize size; swscanf_s(lpszValue,L"%d,%d", &size.cx, &size.cy); _FUNCTION(size);}


#define DUI_SETATTRIBUTE_RECT(__NAME, _VALUE)\
	if (_wcsicmp(lpszName,  L#__NAME) == 0)  { swscanf_s(lpszValue,L"%d,%d,%d,%d", &_VALUE.left, &_VALUE.top,&_VALUE.right,&_VALUE.bottom);}

#define DUI_SETATTRIBUTE_RECT_FUNCTION(__NAME, _FUNCTION)\
	if (_wcsicmp(lpszName,  L#__NAME) == 0)  { \
		CDuiRect rect ; \
		swscanf_s(lpszValue,L"%d,%d,%d,%d", &rect.left, &rect.top,&rect.right,&rect.bottom);\
		_FUNCTION(rect);}

#define DUI_SETATTRIBUTE_POINT(__NAME, _VALUE)\
	if (_wcsicmp(lpszName,  L#__NAME) == 0)  { swscanf_s(lpszValue,L"%d,%d", &_VALUE.x, &_VALUE.y);}



#define DUI_BENGIN_SUB_SETATTRIBUTE(__NAME)\
	if (_wcsicmp(lpszName, L#__NAME) == 0){\

#define DUI_SUB_SETATTRIBUTE_FUNCTION(_SUBNAME, __FUNCTION, __VALUE)\
		if(_wcsicmp(lpszValue, L#_SUBNAME) == 0){__FUNCTION(__VALUE);}
	
#define DUI_END_SUB_SETATTRIBUTE }



#define DUI_SETATTRIBUTE_SUB_CLASS(__CLASS) __CLASS::SetAttribute(lpszName, lpszValue);

#define DUI_END_SETATTRIBUTE }


//////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define DUI_BEGIN_MSG_MAP( _Class)\
	virtual LRESULT DoMessage(const DuiMsg& duiMsg, BOOL& bHandle){ \
		LRESULT lResult = 0; if (0){}

#define DUI_MESSAGE_HANDLER(_Msg, _Func)\
	else if (duiMsg.nMsg == (_Msg)){\
	lResult = _Func(duiMsg.nMsg, duiMsg.CtrlFrom, duiMsg.CtrlTo, duiMsg.wParam, duiMsg.lParam, duiMsg.lpData, bHandle);\
	if( !bHandle ) return lResult;}\

#define DUI_MESSAGE_HANDLER_NO_PARAM(_Msg, _Func)\
	else if (duiMsg.nMsg == _Msg){\
	lResult = _Func();\
	if( !bHandle ) return lResult;}\

#define DUI_MESSAGE_HANDLER_HAVE_HANDELER(_Msg, _Func)\
	else if (duiMsg.nMsg == _Msg){\
	lResult = _Func(bHandle);\
	if( !bHandle ) return lResult;}\


#define DUI_END_MSG_MAP()\
	return __super::DoMessage(duiMsg, bHandle);}


#define DUI_MSG_TIMER_HANDLE(_TimerID, _Func)\
	else if (duiMsg.nMsg == DuiMsg_Timer && duiMsg.wParam == _TimerID){\
		lResult = _Func();\
		if( !bHandle ) return lResult;}\

#define DUI_END_MSG_MAP_SUPER()\
	return lResult;}
//////////////////////////////////////////////////////////////////////////
//�¼�����

#define DUI_BEGIN_EVENT_MAP( _Class)\
	virtual BOOL OnRoutedEvent(const DuiEvent& event){ BOOL bHandle = FALSE;

#define DUI_EVENT_HANDLER(_Event, _Func)\
	if (event.nEvent == _Event){\
		bHandle =  _Func(event);\
	}\

#define DUI_END_EVENT_MAP()\
	return bHandle;}


//ԭʼ�¼�������
#define DUI_BEGIN_CTRL_EVENT_MAP( _Class)\
	virtual BOOL DoEvent(const DuiEvent& event){ BOOL bHandle = TRUE; \

#define DUI_END_CTRL_EVENT_MAP() if(bHandle) return __super::DoEvent(event); return FALSE;}\

//////////////////////////////////////////////////////////////////////////
#ifndef GET_X_LPARAM
#	define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#	define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))
#endif

//////////////////////////////////////////////////////////////////////////
//CDuiMap ����������

#define DuiMapRemoveDuiObject(_Key, _Obj)	\

//////////////////////////////////////////////////////////////////////////

//���Ķ���
#define OBJECT_CORE						L"Core.Core"

//�ؼ�������
#define OBJECT_CORE_BUILD				L"Core.Build"

#define OBJECT_CORE_LANG				L"Lang"
#define OBJECT_CORE_LANGTEXT			L"LangText"
#define OBJECT_CORE_LANG_SET			L"Core.LangSet"

#define OBJECT_CORE_IMAGE				L"Image"
//��
#define OBJECT_CORE_BYTE_STREAM			L"Core.ByteStream"


/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ļ�����Ƥ��
#define OBJECT_CORE_SKIN_FILE			L"Core.Skin.File"

//ZIPѹ�����ļ�Ƥ��
#define OBJECT_CORE_SKIN_ZIP_FILE		L"Core.Skin.Zip.File"




//ZIPѹ������ԴƤ��
#define OBJECT_SKIN_ZIP_RESOURCE		L"SKIN.ZIPRESOURCE"

#define CTRL_CONTROL					L"Control"

#define CTRL_CONTAINER					L"Container"

#define CTRL_WINDOW						L"Window"
#define CTRL_HOST_WINDOW				L"HostWindow"

#define CTRL_VERTICALLAYOUT				L"VerticalLayout"
#define CTRL_HORIZONTALLAYOUT			L"HorizontalLayout"
#define CTRL_LABEL						L"Label"
#define CTRL_BUTTON						L"Button"
#define CTRL_OPTION						L"Option"
#define CTRL_PROGRESS					L"Progress"
#define CTRL_TEXT						L"Text"
#define CTRL_TABLELAYOUT				L"TableLayout"
#define CTRL_EDIT						L"Edit"
#define CTRL_GROUP						L"Group"
#define CTRL_GIF						L"Gif"
#define CTRL_PICTURE					L"Picture"

#define CTRL_SCROLL						L"Scroll"

#define CTRL_Theme						L"Theme"

#define CTRL_RichEdit					L"RichEdit"

typedef INT IIDef; //�ӿ�ID

//����ӿ�

//////////////////////////////////////////////////////////////////////////
//�ܴ�XML�з��ʵ��Ķ���
#define OBJECT_CORE_FONT				L"Font"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����
//��Ϣ������Ϊֱ�ӵ���Ŀ�ĵصģ�����һ����������Ϣ
enum
{
	DuiMsg_RefreshPost	= (WM_USER + 1000), //ˢ����Ϣ���ڲ�ʹ��
	DuiMsg_Create,							//���󴴽�,��ʱ���ӿؼ���δ�������
	DuiMsg_Initialize,						//�����ʼ������ʱ�����е��ӿռ䶼�Ѿ���ʼ����ɣ�
											//����Ϣֻ����IDuiContainerCtrl�ؼ���Ӧ

	DuiMsg_Destory,							//��������
	//DuiMsg_Close,							//�ر�,bHandle = TRUEʱ�����˳�

	DuiMsg_Z_OrderChanged,					//Z ��˳�����仯
	DuiMsg_SizeChanged,						//���ڴ�С�����仯

	DuiMsg_Timer,

	//�����Ϣ
	DuiMsg_MouseEnter,						//������
	DuiMsg_MouseLeave,						//����뿪
	DuiMsg_MouseMove,						//����ƶ�
	DuiMsg_MouseHover,						//���ͣ��
	DuiMsg_LButtonDown,						//����������
	DuiMsg_LButtonDblClk,					//���˫���¼�
	DuiMsg_LButtonUp,						//
	DuiMsg_RButtonDown,
	DuiMsg_MouseWheel,
	DuiMsg_SetCursor,

	//�ڲ�ʹ��
	DuiMsg_OwnerDuiKit = 9999,

	//Option����Group֮�������Ҫ֪ͨ����Option
	DuiMsg_OptionSelectChanged = DuiMsg_OwnerDuiKit + 1,

	

};

//�¼�����Ϊ�ӿ������У����¼�Դ���������ڵ㴫�ݵ���Ϣ
enum
{
	DuiEvent_Initialize,
	DuiEvent_Click,			//��굥��
	DuiEvent_DbClick,
	DuiEvent_Timer,			//��ʱ��
	DuiEvent_SelectChanged,	//ѡ��״̬�ı�
	DuiEvent_TextChange,
	DuiEvent_SetFocus,
	DuiEvent_KillFocus,
	DuiEvent_VisableChanged,	//��ʾ״̬�����仯
	DuiEvent_Size,				//��С�����仯
};

//////////////////////////////////////////////////////////////////////////
//�ؼ�״̬
#define UISTATE_NORMAL       0x00000001
#define UISTATE_FOCUSED      0x00000002
#define UISTATE_SELECTED     0x00000004
#define UISTATE_DISABLED     0x00000008
#define UISTATE_HOT          0x00000010   //����ڿؼ���
#define UISTATE_PUSHED       0x00000020   //��갴��
#define UISTATE_READONLY     0x00000040
#define UISTATE_CAPTURED     0x00000080   //��ý���


// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

namespace DuiKit{;


//����ӿ�

enum 
{
	IIDuiObject,
	IIDuiControlCtrl, 
	IIDuiAnime,
	IIDuiCore,
	IIDuiContainerCtrl,
	IIDuiWindowCtrl,
	IIDuiHostWindowCtrl,
	IIDuiSkin,
	IIDuiBuilder,
	IIDuiByteStream,
	IIDuiSkinSet,
	IIDuiFont,
	IIDuiFontSet,
	IIDuiLang,
	IIDuiLangSet,
	IIDuiImage,
	IIDuiVerticalLayoutCtrl,
	IIDuiHorizontalLayoutCtrl,
	IIDuiButtonCtrl,
	IIDuiLabelCtrl,
	IIDuiTextCtrl,
	IIDuiEditCtrl,
	IIDuiOptionCtrl,
	IIDuiProgressCtrl,
	IIDuiTableLayoutCtrl,
	IIDuiGroupCtrl,
	IIDuiGifCtrl,
	IIDuiPictureCtrl,
	IIDuiVerticalScrollCtrl,
	IIDuiHorizontalScrollCtrl,
	IIDuiScrollCtrl,
	IIDuiRichEditCtrl,
};


//����һЩģ�������





};//namespace DuiKit{;
