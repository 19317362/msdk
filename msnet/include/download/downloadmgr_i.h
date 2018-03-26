#pragma once
#include "download_def.h"


#ifndef DOWNLOAD_DLL_EXPORT
#	define DOWNLOAD_API __declspec(dllimport)
#else
#	define DOWNLOAD_API __declspec(dllexport)
#endif


//////////////////////////////////////////////////////////////////////////
static UCHAR g_anyUrlHash[20] = 
{
	0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff,
};



static UCHAR g_nullUrlHash[20] = {0};

typedef struct sha1_hash
{
	sha1_hash(){
		memset(hash ,0 ,sizeof(hash));
	}

	sha1_hash(CONST UCHAR* szHash){
		memset(hash ,0 ,sizeof(hash));
		memcpy(hash,szHash,sizeof(hash));
	}

	unsigned char hash[20];
}sha1_hash;


//ʹ����map��
struct hashless{
	bool operator() (const sha1_hash&left, const sha1_hash&right) const{
		return memcmp(left.hash, right.hash, sizeof(right.hash)) < 0;
	}
};


inline bool operator == (const sha1_hash& _hash_l , const sha1_hash& _hash_r)
{
	return memcmp(_hash_l.hash,_hash_r.hash , sizeof(_hash_r.hash)) == 0;
}

inline bool operator != (const sha1_hash& _hash_l , const sha1_hash& _hash_r)
{
	return memcmp(_hash_l.hash,_hash_r.hash , sizeof(_hash_r.hash)) != 0;
}

#define AnyUrlHash  sha1_hash(g_anyUrlHash)
#define NullUrlHash sha1_hash(g_nullUrlHash)



//////////////////////////////////////////////////////////////////////////
//���״̬
enum FileCompleteState
{
	FCS_Fiald       = 0x0,//ʧ��
	FCS_Success     = 0x1,//�ɹ�
	FCS_Stop        = 0x2,//ֹͣ
	FCS_Wait        = 0x4, //�ȴ�����

	//LPARAM ���س���
	FCS_Downloading = 0x8, //��������

	FCS_Unknow     = -1,
};


//��ʼ״̬
enum StartDownloadState
{
	SDS_Start     = 0x0,//���ؿ�ʼ
	SDS_Continue  = 0x1,//�ϵ�����
};

//��������,���ͼ�������




enum NotifyMessageID
{
	//����������Ϣ
	Msg_QueryInfo			= (1<<1),	

	//�����ٶȣ���Ϣ���� wp Ϊ�����ٶ�
	//AnyUrlHash ʱΪȫ���ٶ�
	Msg_DownloadSpeed		= (1<<2),
	//FileCompleteState  
	//���ؽ�����Ϣ����Ϣ���� wp Ϊ���ؽ��FileCompleteState ��lp ����
	Msg_FileComplete		= (1<<3),

	//StartDownloadState 
	//��ʼ������Ϣ����Ϣ���� wp Ϊ�������ͣ�����Ϊ DWORD��(0x0: ��ʼ; 0x1: ����(��ͣ��))��lp ����
	Msg_StartDownload		= (1<<4),  

	//������״̬��wp(DWORD) �����ֽڳ��ȣ�lp ����
	Msg_Downloading			= (1<<5),

	//�������ͱ����wp(DownloadType) �����ֽڳ��ȣ�lp ����
	Msg_DownloadTypeChange	= (1<<6),
};

#define DefaultNotifyMsgMask (Msg_QueryInfo|Msg_DownloadSpeed|Msg_FileComplete|Msg_StartDownload)




enum DownLaodOption
{
	//todo:
	//�������������������PDWORD , SIZEOF(DWORD)
	//default:5
	DLO_MaxTaskNum_Option,

	//�������� IAT_ProxyInfo
	DLO_IAT_Option,

	//������Ҫ���յ���ϢID��Ŀ���ǹ���һЩ��ϢID���Ч��
	//default: DefaultNotifyMsgMask
	DLO_NotifyMsgMask,
	DLO_GetDownloadType,  //��ѯ��������
};

enum DLTaskOption
{
	////////////////
	//SetTaskOption
	DLTO_Cookies,      //wchar_t*  //����Cookie����֤�ض����ļ������سɹ�
	DLTO_Referer,      //wchar_t*	//����Referer����֤�������ļ������سɹ�
	DLTO_NoCache,	   //BOOL (TRUE)�޻���ģʽ���أ��ƹ����������棬��֤���ص��ļ������µģ�����������
	
	//������Ҫ���յ���ϢID��Ŀ���ǹ���һЩ��ϢID���Ч��
	//default: DefaultNotifyMsgMask
	DLTO_NotifyMsgMask,
};

struct DownLoadTaskInfo
{
	DownLoadTaskInfo()
	{
		task_hash = NullUrlHash;
		lpUrl= NULL;
		lpSavePath= NULL;
		Fsc = FCS_Unknow;
		ZeroMemory(&DLSize , sizeof(DLSize));
		ZeroMemory(&DLTime,sizeof(DLTime));
		Speed = 0;
	}

	//������һ�£������ظ�����
	sha1_hash task_hash;  

	LPCWSTR   lpUrl;      //url
	LPCWSTR   lpSavePath; //�����ַ

	FileCompleteState  Fsc; //���״̬

	struct DownLoadSize
	{
		UINT64 totalSize;    //�ļ��ܴ�С
		UINT64 currentSize; //�����ش�С
	} DLSize;

	struct DownLoadTime//���������Ϊ����
	{
		UINT64   createTime; //���񴴽�ʱ�� 
		UINT64   finishTime; //���ʱ��
		DWORD    userTime;   //����
		UINT64   needTime;    //Ԥ����Ҫ����ʱ�䣬-1��Ч,��λ(��)
	} DLTime;

	DWORD  Speed;		//��ǰ�ٶ�, ��λ(byte/s)
	DownloadType  DT;	//�������ͣ���������Ϲ�ϵ(DT_HTTP|DT_P2P),���û�п�ʼ����(DT_UNKNOWN)
};

struct IEnumDownLoadTask
{
	/*
	���ܣ�ö�ٵ�����������
	ʹ�÷�����������IDownLoadMgr::QueryTaskInfoһ��ʹ��
	*/
	virtual VOID OnEnumDownLoadTask(const sha1_hash& hash) = 0;
};

//�ص��ӿ�
struct IDownLoadNotify
{
	/*
	���ܣ�����״̬ʵʱ֪ͨ
	������
		hash ����id ���ر�ע�� AnyUrlHash �� NullUrlHash
		msgid ��Ϣid
	*/
	virtual VOID OnDownLaodNotify(const sha1_hash& hash , NotifyMessageID msgid ,WPARAM wp, LPARAM lp) = 0;
};


//���ع���ӿ�
struct IDownLoadMgr
	: public IMSBase
{
	/*
	����:
		��ʼ�������������
	����:
		pNotify �����¼��ص��ӿڡ�
		lpNameSpace ���������ռ�
		bCache �Ƿ񻺴�����״̬
	����:
		S_OK���ɹ�
	*/
	virtual HRESULT InitDownloadMgr(IDownLoadNotify* pNotify, LPCWSTR lpNameSpace = NULL,BOOL bCache = TRUE) = 0;
	
	/*
	����:
	����:
	����:
	*/
	virtual HRESULT UninitDownloadMgr() = 0;


	/*
	���ܣ�ö���������񣬸ú���ͬ������
	������ö�ٻص�����
	���أ�SUCCESSEDD(hr)
	*/
	virtual HRESULT EnumDownloadTask(IEnumDownLoadTask* pCallBack) = 0;

	/*
	���ܣ���ѯ����������Ϣ
	����������di
	���أ�null ʧ��
	ʹ�÷�����Ϊ�˱��������������٣���Ҫ�洢�ýṹ
	*/
	virtual const DownLoadTaskInfo* QueryTaskInfo(const sha1_hash& hash) = 0; 
	
	/*
	���ܣ�������������
	������
		lpUrl��		����URL
		lpSavePath�������ַ
		hash��������id
	���أ�SUCCESSED(HR)
	*/
	virtual HRESULT CreateDownloadTask(LPCWSTR lpUrl,LPCWSTR lpSavePath, OUT sha1_hash& hash) = 0;

	/*
	���ܣ���ʼһ������
	������urlhash ��������hash��ע��AnyUrlHash ��ʼ����
	���أ�SUCCESSED(hr)
	*/
	virtual HRESULT Start(const sha1_hash& urlhash) = 0;
	
	/*
	���ܣ�ֹͣһ����������
	������urlhash ��������hash��ע��AnyUrlHash ��ͣ����
	���أ�SUCCESSED(hr)
	*/
	virtual HRESULT Stop(const sha1_hash& urlhash, BOOL remove_files = FALSE) = 0;

	/*
	���ܣ�ɾ��һ���������񣬴������б�����ɾ��
	������urlhash ��������hash��ע��AnyUrlHash ɾ������
	���أ�SUCCESSED(hr)
	*/
	virtual HRESULT Delete(const sha1_hash& urlhash,BOOL remove_files=FALSE)  = 0;
	
	/*
	���ܣ�����
	������
		option ������,����DownLaodOption���Զ����ʹ�� DWORD ֵ
		lpdata ��������
		dwLen  ����
	���أ�SUCCESSED(HR)
	*/
	virtual HRESULT SetOption(/*DownLaodOption*/DWORD option,LPVOID lpdata,DWORD dwLen) = 0;
	
	/*
	���ܣ���ȡ����״̬
	������option ��������� DownLaodOption���Զ����ʹ�� DWORD��
		  lpData ��ȡ������������
		  dwLen  ���ݳ���
	ע�⣺������صĽ����Ҫ����ʹ�ã������һ�»��棬��������������ڣ����±���.
	���أ�S_OK,�ɹ���ȡ�������õĽ��������û�����ø���
	*/
	virtual HRESULT GetOption(/*DownLaodOption*/DWORD option, OUT LPVOID* lpData, OUT DWORD&dwLen) = 0;

	
	/*
	���ܣ�������������
	������
		hash   ����ID
		option ������,����DLTaskOption���Զ����ʹ�� DWORD ֵ
		lpdata ��������
		dwLen  ����
	���أ�SUCCESSED(HR)
	*/
	virtual HRESULT SetTaskOption(const sha1_hash& hash, /*DLTaskOption*/DWORD option,LPVOID lpData,DWORD dwLen) = 0;

	/*
	���ܣ���ȡ��������
	������
		  hash   ����ID
		  option ��������� DLTaskOption���Զ����ʹ�� DWORD��
		  lpData ��ȡ������������
		  dwLen  ���ݳ���
	ע�⣺������صĽ����Ҫ����ʹ�ã������һ�»��棬��������������ڣ����±���.
	���أ�S_OK,�ɹ���ȡ�������õĽ��������û�����ø���
	*/
	virtual HRESULT GetTaskOption(const sha1_hash& hash, /*DLTaskOption*/DWORD option,OUT LPVOID* lpData,OUT DWORD&dwLen) = 0;
};

MS_DEFINE_IID(IDownLoadMgr,"{3DD441EA-9537-4785-8CC3-830AE9FD8F8F}");


EXTERN_C
{
	/*
	���ܣ��������ع�����
	������
		pNotify ����֪ͨ�ص��ӿ�
		lpNameSpace �����ռ����,���Ϊ�գ����Ե�ǰ����������Ϊ�����ռ�
		bCache �Ƿ�洢��ǰ״̬
	���أ�IDownLoadMgr�ӿ�ָ�룬����NULLʧ��
	*/
	DOWNLOAD_API IDownLoadMgr*  CreateDownLoadMgr(IDownLoadNotify* pNotify, LPCWSTR lpNameSpace = NULL,BOOL bCache = TRUE);
	DOWNLOAD_API VOID DistoryDownLoadMgr(IDownLoadMgr** pMgr);

	//DOWNLOAD_API VOID Get
};
