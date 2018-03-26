////////////////////////////////////////////////////////////////////////////////
/**
* @file
* RAV �ṹ����
* <p>��Ŀ���ƣ�RsGUILib
* <br>�ļ����ƣ�xmlconfig.h
* <br>ʵ�ֹ��ܣ�RAV �ṹ����
* <br>��    �ߣ�Dooyan
* <br>��д���ڣ�2007-1-8 14:20:34
* <br>
* <br>�޸���ʷ�����¶�����д ���ݰ���������  �޸���  �޸�˵����
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef XML_CONFIG_H
#define XML_CONFIG_H

#pragma once
#pragma warning (disable : 4786)

#include <list>
#define MD5_LEN 33
using std::list;
#pragma warning (disable : 4183)
#pragma warning (default : 4786)


/*//********** �ܽṹͼ�꣬�ṹ��ϸ�б�������Ӧ�ṹ  **********

	  �ܽṹ				��Ʒ����
	tagRISING_XML	---- tagPRODUCT_ATTRIBUTE
					 |
					 |		��������б�				�����Ʒ�ڵ�
					 |-- CONFLICT_LIST	----------- tagCONFLICT_ITEM
					 |								 ......
					 |		��װϵͳ�б�				��Ʒ��װϵͳ�ڵ�
					 |-- TARGETSYSTEM_LIST	------- tagTARGETSYSTEM_ITEM
					 |								 .......
					 |		Ĭ�ϰ�װ����
					 |-- DEFAULT_PARAMETER 
					 |
					 |		��װǰ�����б�				��װǰ���г���ڵ�
					 |-- RUNBEFOREINSTALL_LIST ---- tagRUNBEFOREINSTALL_ITEM
					 |
					 |		�б�������б�			 ���					�������
					 |-- COMPONENT_LIST	  ---------	tagCOMPONET	------- tagCOMPONENT_ATTRIBUTE
					 |								 .......		|	
					 |												|		���������ϵ			�����ڵ�
					 | 												|-- DEPEND_LIST	------------- tagDEPEND_ITEM
					 |												|								......	
					 |												|		��������ϵ			����ڵ�
					 | 												|-- COMCONFLICT_LIST  ------- tagCOMCONFLICT_ITEM
					 |												|								......	
					 |												|		����ļ��б�			�ļ��ڵ�
					 |												|-- FILES_LIST  ------------- tagFILES_ITEM
					 |												|								......	
					 |												|		���ע����б�			ע���ڵ�
					 |												|-- REGISTS_LIST ------------ tagREGISTS_ITEM
					 |												|								......	
					 |												|		��������б�			���ӽڵ�
					 |												|-- LINKS_LIST -------------- tagLINKS_ITEM
					 |												|								......	
					 |												|		�����װ�������б�		���нڵ�
					 |												|-- RUNAFTERINSTALL_LIST ---- tagRUNAFTERINSTALL_ITEM
					 |																				......	
					 |	
					 |
					 |		��������б�		���ڵ�						���
					 |-- COMPONENT_TREE ------ tagCOMPONENT_TREE_ITEM ---- tagCOMPONET
												......			

//*/

template<class T>
class pointer_list : public list<T>
{
public:
	void remove_pitems()
	{
		for( pointer_list<T>::iterator i=begin(); i!=end(); i++ )
		{
			delete *i;
			*i = 0;
		}
	}
	~pointer_list()
	{
		remove_pitems();
	}
	
	pointer_list()
	{
	}
	
	// ��Ԫ��� ��ֹ�б��Ʋ���
	pointer_list(const pointer_list& _X)
	{
		DebugBreak();// ָ���б��ܸ��ƣ���Ϊ���б���Զ��ͷ�ָ��ָ���ڴ�
	}

	pointer_list& operator=(const pointer_list& _X)
	{
		DebugBreak();// ָ���б��ܸ��ƣ���Ϊ���б���Զ��ͷ�ָ��ָ���ڴ�
		return (*this); 
	}
};
#pragma warning( default : 4183 )

//////////////////////////////////////////////////////////////////////////
//	product settings

// ��Ʒ���Խṹ 
typedef struct tagPRODUCT_ATTRIBUTE
{
	UINT uNameID;				// Ĭ������
	UINT uPreSetup;				// ��װ����ͼƬ��Դ
	UINT uBackBmpID;			// ��װ����ͼƬ��Դ
	UINT ulisenceID;			// ��װ��Ȩ��Դ
	TCHAR szDefName[MAX_PATH];	// Ĭ������
} PRODUCT_ATTRIBUTE, *PPRODUCT_ATTRIBUTE;

// Ŀ��ϵͳ (����Ʒ����װ��ϵͳ����)
typedef struct tagTARGETSYSTEM_ITEM
{
	UINT  uMajorVersion;		// ϵͳ���汾�ţ� ������
	UINT  uMinorVersion;		// ϵͳС�汾�ţ� ������
	TCHAR szWindowName[64];		// ϵͳ�����ַ���
} TARGETSYSTEM_ITEM, *PTARGETSYSTEM_ITEM;

typedef pointer_list<PTARGETSYSTEM_ITEM> TARGETSYSTEM_LIST;
typedef TARGETSYSTEM_LIST::iterator TARGETSYSTEM_LIST_ITER;

// �����Ʒ���������
typedef struct tagCONFLICT_ITEM
{
	UINT uNameID;					// ���������ԴID
	TCHAR szRegkey[MAX_PATH];		// ע���λ��
	TCHAR szRegkeyName[MAX_PATH];	// ע������
	UINT uRegkeyDataType;			// ��ֵ����
	TCHAR szRegkeyValue[MAX_PATH];	// ��ֵ
	UINT uTestType;					// ��ֵ�������, 0 ��ֵ��Ӧ��1 �ļ�����Ŀ¼�Ƿ����
} CONFLICT_ITEM, *PCONFLICT_ITEM;

typedef pointer_list<PCONFLICT_ITEM> CONFLICT_LIST;
typedef CONFLICT_LIST::iterator CONFLICT_LIST_ITER;

// ��װ�������� - Ĭ�ϰ�װʱ�Ĳ���
typedef struct tagDEFAULT_PARAMETER
{
	UINT uLanguageCode;				// Ĭ��ʹ������ PageCode
	UINT uGroupNameID;				// ����������������ԴID 2233
	UINT uUser;						// ��ǰ��Ʒʹ���û� ALLUSER
	TCHAR szSerialNum[128];			// Ĭ�����к� EIWOJI-EISLNI-SDIOEI-DIEKLI 
	TCHAR szUserID[64];				// Ĭ���û�ID 888888888888
	TCHAR szRisingRootPath[MAX_PATH];//	Ĭ�����ǰ�װĿ¼ C:\Program Files\Rising
	TCHAR szRisingPath[MAX_PATH];	//	Ĭ������ע���Ŀ¼ %RISINGREGROOT%\SOFTWARE\Rising
	UINT bSilence;					// �Ƿ�Ĭ��װ 0
	UINT bAutoInstall;				// �Ƿ��Զ���װ 0
	UINT bScanMemory;				// �Ƿ�ɨ���ڴ� 0
} DEFAULT_PARAMETER, *PDEFAULT_PARAMETER;

// ��װǰ���е�ģ����Ϣ
typedef struct tagRUNBEFOREINSTALL_ITEM
{
	UINT uNameID;				// ��������ID
	UINT bMustRun;				// �Ƿ��������
	UINT bWaitEnd;				// �Ƿ�ȴ��˳������
	UINT bSelected;				// Ĭ����ʾ�Ƿ�ѡ��
	UINT bShowSel;				// �Ƿ����б�����ʾ
	UINT bIsServer;				// �Ƿ�����Server��������
	UINT bRunOS;				// �ļ��Ƿ�Ҫ���ض�ϵͳ�Ű�װ
	TCHAR szProcPathName[MAX_PATH * 2]; //��������·���Ͳ���
	TCHAR szProcArguments[MAX_PATH]; //��������·���Ͳ���
} RUNBEFOREINSTALL_ITEM, *PRUNBEFOREINSTALL_ITEM;
typedef pointer_list<PRUNBEFOREINSTALL_ITEM> RUNBEFOREINSTALL_LIST;
typedef RUNBEFOREINSTALL_LIST::iterator RUNBEFOREINSTALL_LIST_ITER;


//////////////////////////////////////////////////////////////////////////
//	component settings
// �������
typedef struct tagCOMPONENT_ATTRIBUTE
{
	UINT uNameID;				// �������ID
	UINT bMustInstall;			// ������Ƿ���밲װ
	UINT bShowSel;				// ������Ƿ������ѡ���б�����ʾ
	UINT bSelected;				// ������Ƿ������ѡ���б���Ĭ�ϱ�ѡ��
	UINT bShare;				// ������Ƿ���Ŀǰû��ʹ��
	UINT uDescripeID;			// ���������ԴID
	UINT uInstallID;			// �����װ������ԴID
	UINT uNeedSpace;			// �����Ҫ���̿ռ�(��λ���ֽ�)
	UINT bIsServer;				// ������Ƿ�Ҫ��Server������
	UINT uType;					// ������ͣ�1��ʾ�ǹ���
	TCHAR szUpDate[64];			// ������ĸ�������
	TCHAR szVersion[64];		// ������İ汾��
//	TCHAR szCheck[MD5_LEN];		// ������ļ��� MD5 У��ֵ
	TCHAR szCoreVersion[32];	// ������ĺ��İ汾
	TCHAR szCodeName[32];		// ������Ĵ�������
	TCHAR szReferrFile[MAX_PATH];// ����������ļ���
	TCHAR szUseDLL[MAX_PATH];	// �����ר�ö�̬��
	TCHAR szVerFunc[MAX_PATH];	// ����ĸ��ݰ汾��ͬ���Ƿ��ܹ�ʹ��
} COMPONENT_ATTRIBUTE, *PCOMPONENT_ATTRIBUTE;

// �������������ϵ
typedef struct tagDEPEND_ITEM
{
	TCHAR szCodeName[32];		// ��Ҫ�������������
	TCHAR szCoreVersion[32];	// Ҫ�����������汾
} DEPEND_ITEM, *PDEPEND_ITEM;
typedef pointer_list<PDEPEND_ITEM> DEPEND_LIST;
typedef DEPEND_LIST::iterator DEPEND_LIST_ITER;

// �����Ʒ���������
typedef struct tagCOMCONFLICT_ITEM
{
	TCHAR szCodeName[32];		// ��Ҫ��������������
	TCHAR szCoreVersion[32];	// Ҫ�󻥳���İ汾
} COMCONFLICT_ITEM, *PCOMCONFLICT_ITEM;
typedef pointer_list<PCOMCONFLICT_ITEM> COMCONFLICT_LIST;
typedef COMCONFLICT_LIST::iterator COMCONFLICT_LIST_ITER;

typedef struct tagFILES_ITEM
{
	BOOL bExectue;				// �Ƿ������г���
	UINT bRunOS;				// �ļ��Ƿ�Ҫ���ض�ϵͳ�Ű�װ
	UINT uRpSize;				// �ļ����� RP ���ߴ�
	UINT uComDll;				// COM��� ��װʱע�� ж��ʱ�ȷ�ע��
	UINT uMovEx;				// 0���Ƿ���Ҫ����ȡ�����Ƿ��Ƴɹ���1�����㸴��ʧ��Ҳ����ʾ����������RavExt.dll��2�����Ƿ��滻�ɹ������¶���Ҫ���𣬱���ĳЩ�����ļ�hookhelp.sys hooknots.sys
	UINT uNoCheck;				// ����У����ļ�
	TCHAR szPath[MAX_PATH];		// ��װ·��
	TCHAR szSourcePath[MAX_PATH];		// ԭ�ļ�·��
	TCHAR szCheck[MD5_LEN];		// �ļ� MD5 У��ֵ
	TCHAR szFileName[MAX_PATH];	// �ļ���
} FILES_ITEM, *PFILES_ITEM;
typedef pointer_list<PFILES_ITEM> FILES_LIST;
typedef FILES_LIST::iterator FILES_LIST_ITER;

typedef struct tagREGISTS_ITEM
{
	TCHAR szRegkey[MAX_PATH];		// ע����
	TCHAR szRegkeyName[MAX_PATH];	// ע������
	TCHAR szRegkeyValue[MAX_PATH];	// ע���ֵ
	UINT bRunOS;					// ע����Ƿ�Ҫ���ض�ϵͳ�Ű�װ
	UINT uRegkeyDataType;			//��ֵ����
} REGISTS_ITEM, *PREGISTS_ITEM;
typedef pointer_list<PREGISTS_ITEM> REGISTS_LIST;
typedef REGISTS_LIST::iterator REGISTS_LIST_ITER;

typedef struct tagLINKS_ITEM
{
	UINT uNameID;					// ���ɵ�.lnk�ļ�����ԴID
	UINT uLinkSubDirID;				// ����LINK����·����ԴID
	UINT uDescripeID;				// LINK Ŀ���������ԴID
	UINT bRunOS;					// LINK�Ƿ�Ҫ���ض�ϵͳ�Ű�װ
	TCHAR szLinkDir[MAX_PATH];		// ����LINK��·��
	TCHAR szPath[MAX_PATH];			// ���ڶ�λLINKĿ��Ŀ¼
	TCHAR szFileName[MAX_PATH];		//Ŀ��·��
	TCHAR szArguments[32];			//LINK Ŀ��Ĳ��� 
} LINKS_ITEM, *PLINKS_ITEM;
typedef pointer_list<PLINKS_ITEM> LINKS_LIST;
typedef LINKS_LIST::iterator LINKS_LIST_ITER;

typedef struct tagRUNAFTERINSTALL_ITEM
{
	UINT uNameID;					// ��������ID
	UINT bMustRun;					// �Ƿ��������
	UINT bWaitEnd;					// �Ƿ�ȴ��˳��������
	UINT bSelected;					// Ĭ����ʾ�Ƿ�ѡ��
	UINT bShowSel;					// �Ƿ����б�����ʾ
	UINT bRunOS;					// �����Ƿ�Ҫ���ض�ϵͳ�Ű�װ
	TCHAR szProcPath[MAX_PATH];		// ��������·���Ͳ���
	TCHAR szProcFileName[MAX_PATH]; // ��������·���Ͳ���
	TCHAR szProcArguments[MAX_PATH]; // ��������·���Ͳ���
	UINT bIsServer;					// �Ƿ�����SVR����
} RUNAFTERINSTALL_ITEM, *PRUNAFTERINSTALL_ITEM;
typedef pointer_list<PRUNAFTERINSTALL_ITEM> RUNAFTERINSTALL_LIST;
typedef RUNAFTERINSTALL_LIST::iterator RUNAFTERINSTALL_LIST_ITER;

typedef struct tagCOMPONET
{
	COMPONENT_ATTRIBUTE attrib;		// �������
	DEPEND_LIST depend;				// ���������ϵ
	COMCONFLICT_LIST conflict;		// ��������ϵ
	FILES_LIST files;				// ����ļ��б�
	REGISTS_LIST regists;			// ���ע����б�
	LINKS_LIST links;				// ��������б�
	RUNAFTERINSTALL_LIST runafterinstall;// �����װ�������б�
} RSCOMPONENT, *PRSCOMPONENT;

//////////////////////////////////////////////////////////////////////////
// tree
typedef struct tagCOMPONENT_TREE_ITEM
{
	typedef tagCOMPONENT_TREE_ITEM* PCOMPONENT_TREE_ITEM;	// ���ڵ㶨��
	PCOMPONENT_TREE_ITEM pParent;	// ���ڵ�ָ��
	PCOMPONENT_TREE_ITEM pNext;		// ��һ���ڵ�ָ��
	PCOMPONENT_TREE_ITEM pChild;	// �ӽڵ�ָ��
	PRSCOMPONENT pComponent;			// ��ǰ�ڵ�ָ���ģ��
} COMPONENT_TREE_ITEM, *PCOMPONENT_TREE_ITEM;


// ����������
typedef struct tagCOMPONENT_TREE_DATA
{
	COMPONENT_TREE_ITEM  Root;
	PCOMPONENT_TREE_ITEM pHot;
} COMPONENT_TREE_DATA, *PCOMPONENT_TREE_DATA;

typedef pointer_list<PRSCOMPONENT> COMPONENT_LIST;
typedef COMPONENT_LIST::iterator COMPONENT_LIST_ITER;

// �������ݱ�����
class tagCOMPONENT_TREE : public COMPONENT_TREE_DATA 
{
public:
	tagCOMPONENT_TREE()
	{pHot = &Root; ZeroMemory(&Root, sizeof(Root)); };

	virtual ~tagCOMPONENT_TREE()
	{ DestroyChild(&Root); };
	
public:
	static PCOMPONENT_TREE_ITEM InsertTreeItem(PCOMPONENT_TREE_ITEM pItemNew, PCOMPONENT_TREE_ITEM pParentItem = 0)
	{
		if(!pItemNew) return NULL;
		if(!pParentItem) return NULL;
		
		if(!pParentItem->pChild)//if parent has no child
		{
			pParentItem->pChild = pItemNew;
			pItemNew->pParent = pParentItem;
		}
		else
		{	
			//else find the last child of the parent item
			PCOMPONENT_TREE_ITEM pBrother;
			for (pBrother = pParentItem->pChild; pBrother->pNext; pBrother = pBrother->pNext)
				;
			pBrother->pNext = pItemNew;
			pItemNew->pParent = pParentItem;
		}
		return pItemNew;
	}

	 void DestroyChild(PCOMPONENT_TREE_ITEM pItem)
	 {
		 PCOMPONENT_TREE_ITEM pTmp = pItem->pChild;
		 PCOMPONENT_TREE_ITEM pTmp1 = 0;
		 if(!pTmp)
			 return;
		 
		 do
		 {
			 DestroyChild(pTmp);
			 pTmp1 = pTmp->pNext;
			 delete pTmp;
		 }
		 while(pTmp=pTmp1);
		 pItem->pChild = 0;
	 }

	PCOMPONENT_TREE_ITEM MoveRoot()
	{ pHot = &Root; return pHot; }

	PCOMPONENT_TREE_ITEM MoveChild()
	{
		if(pHot->pChild)
		{
			pHot= pHot->pChild;
			return pHot;
		}
		else
			return NULL;
	}
	
	PCOMPONENT_TREE_ITEM MoveNext()
	{
		if(pHot->pNext)
		{
			pHot = pHot->pNext;
			return pHot;
		}
		else
			return NULL;
	}
	
	PCOMPONENT_TREE_ITEM MoveParent()
	{
		if(pHot->pParent)
		{
			pHot = pHot->pParent;
			return pHot;
		}
		else
			return NULL;
	}

	PCOMPONENT_TREE_ITEM GetNode()
	{	return pHot; }
	
//	static PCOMPONENT_TREE_ITEM InsertTreeItem(PCOMPONENT_TREE_ITEM pItemNew, PCOMPONENT_TREE_ITEM pParentItem);
};
typedef class tagCOMPONENT_TREE	  COMPONENT_TREE;
typedef class tagCOMPONENT_TREE*  PCOMPONENT_TREE;

// ��������
typedef struct tagRISING_XML
{
	CONFLICT_LIST conflict_list;			// ��������б�
	TARGETSYSTEM_LIST targetsystem_list;	// ��Ʒ��װ
	PRODUCT_ATTRIBUTE product_attribute;	// ��Ʒ����
	DEFAULT_PARAMETER default_parameter;	// Ĭ�ϰ�װ����
	RUNBEFOREINSTALL_LIST runbeforeinstall_list;	// ��װǰ����
	COMPONENT_TREE component_tree;			// ��������б�
	COMPONENT_LIST component_list;			// �б�������б�
	DWORD	dwReserved;
}RISING_XML, *PRISING_XML;

#endif //XML_CONFIG_H	