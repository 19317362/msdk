////////////////////////////////////////////////////////////////////////////////
/**
* @file
* IRsXMLLoader �ӿ�ʵ��, XML �Ľ���
* <p>��Ŀ���ƣ�RsGUILib
* <br>�ļ����ƣ�rsxmlloader.h
* <br>ʵ�ֹ��ܣ�IRsXMLLoader �ӿ�ʵ��
* <br>��    �ߣ�Dooyan
* <br>��д���ڣ�2007-1-8 13:59:37
* <br>
* <br>�޸���ʷ�����¶�����д ���ݰ���������  �޸���  �޸�˵����
*/
////////////////////////////////////////////////////////////////////////////////

// RsXMLLoader.h: interface for the CRsXMLLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSXMLLOADER_H__7F85A30B_E929_4989_A2E3_52E4244DDA96__INCLUDED_)
#define AFX_RSXMLLOADER_H__7F85A30B_E929_4989_A2E3_52E4244DDA96__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include "XMLite.h"
#include "xmlconfig.h"

// XNode, *LPXNode			XML �ڵ�	
// XNodes, *LPXNodes		XML �ڵ���
// XAttr, *LPXAttr			XML �ڵ�����	
// XAttrs					XML �ڵ�������	
#include "IRsXMLLoader.h"
class CRsXMLLoader  : public IRsXMLLoader
{
public:
	CRsXMLLoader();
	virtual ~CRsXMLLoader();
	
public:
	virtual BOOL LoadXMLFiles(LPCTSTR lpPath, LPCTSTR lpFileName, PRISING_XML pBuff, UINT uSize = NULL, BOOL bLoadModuleXML = TRUE);
	virtual BOOL SaveXMLFiles(LPCTSTR lpPath, LPCTSTR lpFileName, PRISING_XML pBuff, UINT uSize = NULL, BOOL bSaveModuleXML = TRUE);

	virtual INT SetVerFunc(IN INT nIndex, IN BOOL bEnable, IN OUT LPTSTR lpVerFuncString, IN INT nMaxLen);
	virtual BOOL GetVerFunc(IN INT nIndex, IN LPCTSTR lpVerFuncString);

	INT VFA32TOB2(IN LPCTSTR lpA32, OUT LPTSTR lpB2, IN INT nLen);
	INT VFB2TOA32(IN LPCTSTR lpB2, IN INT nLen, OUT LPTSTR lpA32, IN INT n32Len);

	virtual BOOL Release() { delete this; return 1;};
private:
	/**
	* ����ļ�ȫ·��
	* <p>����ļ�ȫ·��
	* @param lpBuff				��� Buffer
	* @param uBuffLen			��� Buffer �Ĵ�С
	* @param lpPath				�ļ�·��, Ϊ NULL ��ʹ������·��
	* @param lpFileName			�ļ���
	* @return					�ɹ����� TRUE 
	*/
	BOOL CombinePathName(LPTSTR lpBuff, UINT uBuffLen, LPCTSTR lpPath, LPCTSTR lpFileName);
	/**
	* XML �ļ�����Ԥ����
	* <p> XML �ļ�����Ԥ����, �����л�����XML����������Ķ���ɾ����
	* @param rssFile			���� XML �ļ�����string
	* @return					�ɹ����� TRUE
	*/
	BOOL PreFormatXML(string &rssFile);

	// ��ȡ�ļ��� �����ļ�������䵽�ṹ
	/**
	* Ѱ��ģ��
	* <p>Ѱ��ģ��
	* @param lpCodeName			ģ������
	* @param ppComponentList	ģ���б�
	* @return					����ģ��ָ��, ʧ�ܷ��� NULL
	*/
	PRSCOMPONENT FindComponent(LPCTSTR lpCodeName, COMPONENT_LIST* ppComponentList);

	/**
	* ��� PRISING_XML �ṹ
	* <p>����XML���� ��� PRISING_XML �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillAll(LPXNode lpNode, PRISING_XML pBuff);
	/**
	* ��� PRODUCT_ATTRIBUTE �ṹ
	* <p>����XML���� ��� PRODUCT_ATTRIBUTE �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillProduct(LPXNode lpNode, PPRODUCT_ATTRIBUTE pBuff);
	/**
	* ��� TARGETSYSTEM_LIST �ṹ
	* <p>����XML���� ��� TARGETSYSTEM_LIST �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillTargetSystem(LPXNode lpNode, TARGETSYSTEM_LIST* pBuff);
	/**
	* ��� CONFLICT_LIST �ṹ
	* <p>����XML���� ��� CONFLICT_LIST �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillConflict(LPXNode lpNode, CONFLICT_LIST* pBuff);
	/**
	* ��� DEFAULT_PARAMETER �ṹ
	* <p>����XML���� ��� DEFAULT_PARAMETER �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillDefault(LPXNode lpNode, PDEFAULT_PARAMETER pBuff);
	/**
	* ��� RUNBEFOREINSTALL_LIST �ṹ
	* <p>����XML���� ��� RUNBEFOREINSTALL_LIST �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillRunBeforeInstall(LPXNode lpNode, RUNBEFOREINSTALL_LIST* pBuff);
	/**
	* ��� RSCOMPONENT �ṹ
	* <p>����XML���� ��� RSCOMPONENT �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponent(LPXNode lpNode, PRSCOMPONENT pBuff);
	/**
	* ��� COMPONENT_ATTRIBUTE �ṹ
	* <p>����XML���� ��� COMPONENT_ATTRIBUTE �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentAttrib(LPXNode lpNode, PCOMPONENT_ATTRIBUTE pBuff);
	/**
	* ��� COMPONENT_TREE �ṹ
	* <p>����XML���� ��� COMPONENT_TREE �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @param ppComponentList	����б�
	* @param pParent			Ŀǰ���ڵ�			
	* @return 
	*/
	BOOL xFillComponentTree(LPXNode lpNode, PCOMPONENT_TREE pBuff, COMPONENT_LIST *ppComponentList, PCOMPONENT_TREE_ITEM pParent = NULL);
	/**
	* ��� COMPONENT_LIST �ṹ
	* <p>����XML���� ��� COMPONENT_LIST �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentList(LPXNode lpNode, COMPONENT_LIST* pBuff);
	/**
	* ��� DEPEND_LIST �ṹ
	* <p>����XML���� ��� DEPEND_LIST �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
//	BOOL xFillComponentVersion(LPXNode lpNode, VERSION_LIST* pBuff);
	BOOL xFillComponentDepend(LPXNode lpNode, DEPEND_LIST* pBuff);
	/**
	* ��� COMCONFLICT_LIST �ṹ
	* <p>����XML���� ��� COMCONFLICT_LIST �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentConflict(LPXNode lpNode, COMCONFLICT_LIST* pBuff);
	/**
	* ��� FILES_LIST �ṹ
	* <p>����XML���� ��� FILES_LIST �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentFiles(LPXNode lpNode, FILES_LIST* pBuff);
	/**
	* ��� REGISTS_LIST �ṹ
	* <p>����XML���� ��� REGISTS_LIST �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentRegisters(LPXNode lpNode, REGISTS_LIST* pBuff);
	/**
	* ��� LINKS_LIST �ṹ
	* <p>����XML���� ��� LINKS_LIST �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentLinks(LPXNode lpNode, LINKS_LIST* pBuff);
	/**
	* ��� RUNAFTERINSTALL_LIST �ṹ
	* <p>����XML���� ��� RUNAFTERINSTALL_LIST �ṹ
	* @param lpNode				XML ��Ӧ�ڵ�
	* @param pBuff				���ݽṹָ��
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentRunLast(LPXNode lpNode, RUNAFTERINSTALL_LIST* pBuff);
	
	/**
	* ��� XML �ڵ���������
	* <p>��� XML �ڵ���������
	* @param lpNode				XML �ڵ�
	* @param attrname			XML �ڵ�������
	* @param lpBuff				��� Buffer
	* @param uSize				��� Buffer ��С
	* @return					�ɹ����� TRUE
	*/
	BOOL xGetNodeAttr(LPXNode lpNode, LPCTSTR attrname, LPTSTR lpBuff, UINT uSize);
	/**
	* ��� XML �ڵ�����ֵ
	* <p>��� XML �ڵ�����ֵ
	* @param lpNode				XML �ڵ�
	* @param attrname			XML �ڵ�������
	* @param uValue				��� ��ֵ
	* @return					�ɹ����� TRUE
	*/
	BOOL xGetNodeAttr(LPXNode lpNode, LPCTSTR attrname, UINT& uValue);

	// �����ļ��� ���ṹ������䵽�ļ�
	/**
	* �趨 XML �ڵ���������
	* <p>�趨 XML �ڵ���������
	* @param lpNode				XML �ڵ�
	* @param attrname			XML �ڵ�������
	* @param lpBuff				�µĽڵ���������
	* @return 
	*/
	BOOL xSetNodeAttr(LPXNode lpNode, LPCTSTR attrname, LPCTSTR lpBuff);
	/**
	* �趨 XML �ڵ�����ֵ
	* <p>�趨 XML �ڵ�����ֵ
	* @param lpNode				XML �ڵ�
	* @param attrname			XML �ڵ�������
	* @param uValue				��� ��ֵ
	* @return 
	*/
	BOOL xSetNodeAttr(LPXNode lpNode, LPCTSTR attrname, UINT uValue);
	/**
	* �����µ��ӽڵ�
	* <p>�����µ��ӽڵ�
	* @param lpNode				�µĽڵ�ĸ��ڵ�
	* @param szNodename			�µĽڵ�����
	* @param lpBuff				�µĽڵ�����
	* @return					�½ڵ��ָ��
	*/
	LPXNode xCreateChildNode(LPXNode lpNode, LPCTSTR szNodename, LPCTSTR lpBuff);

	/**
	* ���� RSCOMPONENT �ṹ��� XML ����
	* <p>���� RSCOMPONENT �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponent(PRSCOMPONENT pBuff, LPXNode lpParentNode);
	/**
	* ���� COMPONENT_ATTRIBUTE �ṹ��� XML ����
	* <p>���� COMPONENT_ATTRIBUTE �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentAttrib(PCOMPONENT_ATTRIBUTE pBuff, LPXNode lpParentNode);
	/**
	* ���� VERSION_LIST �ṹ��� XML ����
	* <p>���� VERSION_LIST �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
//	BOOL xFillComponentVersion(VERSION_LIST* pBuff, LPXNode lpParentNode);
	/**
	* ���� DEPEND_LIST �ṹ��� XML ����
	* <p>���� DEPEND_LIST �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentDepend(DEPEND_LIST* pBuff, LPXNode lpParentNode);
	/**
	* ���� COMCONFLICT_LIST �ṹ��� XML ����
	* <p>���� COMCONFLICT_LIST �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentConflict(COMCONFLICT_LIST* pBuff, LPXNode lpParentNode);
	/**
	* ���� FILES_LIST �ṹ��� XML ����
	* <p>���� FILES_LIST �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentFiles(FILES_LIST* pBuff, LPXNode lpParentNode);
	/**
	* ���� REGISTS_LIST �ṹ��� XML ����
	* <p>���� REGISTS_LIST �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentRegisters(REGISTS_LIST* pBuff, LPXNode lpParentNode);
	/**
	* ���� LINKS_LIST �ṹ��� XML ����
	* <p>���� LINKS_LIST �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentLinks(LINKS_LIST* pBuff, LPXNode lpParentNode);
	/**
	* ���� RUNAFTERINSTALL_LIST �ṹ��� XML ����
	* <p>���� RUNAFTERINSTALL_LIST �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentRunLast(RUNAFTERINSTALL_LIST* pBuff, LPXNode lpParentNode);

	/**
	* ���� PRISING_XML �ṹ��� XML ����
	* <p>���� PRISING_XML �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillAll(PRISING_XML pBuff, LPXNode lpParentNode);
	/**
	* ���� PPRODUCT_ATTRIBUTE �ṹ��� XML ����
	* <p>���� PPRODUCT_ATTRIBUTE �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillProduct(PPRODUCT_ATTRIBUTE pBuff, LPXNode lpParentNode);
	/**
	* ���� TARGETSYSTEM_LIST �ṹ��� XML ����
	* <p>���� TARGETSYSTEM_LIST �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillTargetSystem(TARGETSYSTEM_LIST* pBuff, LPXNode lpParentNode);
	/**
	* ���� CONFLICT_LIST �ṹ��� XML ����
	* <p>���� CONFLICT_LIST �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillConflict(CONFLICT_LIST* pBuff, LPXNode lpParentNode);
	/**
	* ���� PDEFAULT_PARAMETER �ṹ��� XML ����
	* <p>���� PDEFAULT_PARAMETER �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillDefault(PDEFAULT_PARAMETER pBuff, LPXNode lpParentNode);
	/**
	* ���� RUNBEFOREINSTALL_LIST �ṹ��� XML ����
	* <p>���� RUNBEFOREINSTALL_LIST �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillRunBeforeInstall(RUNBEFOREINSTALL_LIST* pBuff, LPXNode lpParentNode);

	/**
	* ���� PCOMPONENT_TREE �ṹ��� XML ����
	* <p>���� PCOMPONENT_TREE �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param pParent			���ݽṹ�и����ڵ�ĸ��ڵ�ָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentTree(PCOMPONENT_TREE pBuff, PCOMPONENT_TREE_ITEM pParent, LPXNode lpParentNode);
	/**
	* ���� COMPONENT_LIST �ṹ��� XML ����
	* <p>���� COMPONENT_LIST �ṹ��� XML ����
	* @param pBuff				���ݽṹָ��
	* @param lpParentNode		XML ��Ӧ�ĸ��ڵ�
	* @return					�ɹ����� TRUE
	*/
	BOOL xFillComponentList(COMPONENT_LIST* pBuff, LPXNode lpParentNode);

private:

	TCHAR m_szPathName[MAX_PATH];
	BOOL m_bSaveModuleFiles;
	BOOL m_bLoadModuleFiles;
};

#endif // !defined(AFX_RSXMLLOADER_H__7F85A30B_E929_4989_A2E3_52E4244DDA96__INCLUDED_)
