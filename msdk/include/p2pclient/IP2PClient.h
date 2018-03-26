#ifndef __IP2PClient_H__
#define __IP2PClient_H__


#include <mscom/msplugin.h>

typedef ::msdk::mscom::IMsPlugin    IMsP2PPlugin;
#define P2PString                   wchar_t*
#define P2P_HASH_SIZE               20
#ifndef AnyUrlHash
static unsigned char g_anyUrlHash[20] = { \
										0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
										0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
									};
static unsigned char g_nullUrlHash[20] = {0};
#define AnyUrlHash					g_anyUrlHash
#define NullUrlHash					g_nullUrlHash
#define HttpUrlHash                 AnyUrlHash
#endif



enum P2POption
{
	// P2P �������
    Opt_P2PMaxUpSpeedInBps = 0, // �����ϴ��ٶȣ���������Ϊ DWORD��Ĭ��Ϊ 0��������
    Opt_P2PMaxDownSpeedInBps,   // ���������ٶȣ���������Ϊ DWORD��Ĭ��Ϊ 0��������
    Opt_P2PListenPort,          // �����˿ڣ�      ��������Ϊ USHORT��Ĭ��Ϊ (11935 ~ 15000) ������һ��
    Opt_P2PMaxConnectNum,       // �����������    ��������Ϊ DWORD��Ĭ��Ϊ 0��������
    Opt_P2PTracker,	            // �� tracker ��ַ�� ��������Ϊ P2PString�����磺"udp://192.168.150.2:7369"
	Opt_P2PTrackerList,         // tracker ��ַ������������Ϊ vector<P2PString>*
	Opt_P2PSavePath,            // �����ļ�����·������������Ϊ P2PString

    Opt_P2PTimeOut,             // P2P ���ӵĳ�ʱʱ�䣬    ��������Ϊ DWORD����λΪ�룬Ĭ��Ϊ 60 �룬 -1 Ϊ������.
	Opt_P2PTolerate,            // P2P ������С�ٶ�����,   ��������Ϊ DWORD, ��λΪ�ֽ�, Ĭ��Ϊ 1024, -1 Ϊ������.
	Opt_P2PMsgInterval,         // P2P ���غ��ϴ��¼��������������Ϊ DWORD����λΪ���룬Ĭ��Ϊ 1000 ����
	Opt_P2PRidUrl,              // �ϱ� UrlHash ��ȡ Rid�� ��������Ϊ P2PString��Ĭ��Ϊ "http://s.rising.cn/p2p/searchrid.php"

	Opt_P2PUpsLimit,            // �ϴ������ƣ� ��������Ϊ DWORD��Ĭ��Ϊ -1��������

	// ��Ʒ���
	Opt_P2PHostName = 0x100,    // P2P ʹ���߱�ʾ�� �������� P2PString, ��Ҫ���� init
	Opt_P2PAutoUpload,			// ģ�����ģʽ, 0, �Զ��ϴ�����; 1, �Զ�����; 2, �Զ��ϴ�; 3, �����κ���. ��Ҫ���� init
	Opt_P2PDataReport,			// 0x1 ������Ϣ�ϱ� 0x2 ������Ϣ�ϱ�  0x4 �ϴ���Ϣ�ϱ�  0x8 .....
	Opt_P2PReportUrl            // �ϱ� URL���������� P2PString��Ĭ��Ϊ "http://u.rising.cn/p2p/upinfo.php"
};

//���״̬
enum FileCompleteState
{
	//ʧ��
	FileCompleteState_Fiald   = 0x0,

	//�ɹ�
	FileCompleteState_Success = 0x1,

	//�û�ȡ��
	FileCompleteState_Cancel  = 0x2,

	//�û���ͣ
	FileCompleteState_Pause   = 0x3,

	//ֹͣ
	FileCompleteState_Stop    = 0x4,
};

//��ʼ״̬
enum StartDownloadState
{
	//���ؿ�ʼ
	StartDownloadState_Start     = 0x0,

	//���ؼ������ϵ��ϴ�
	StartDownloadState_continue  = 0x1,
};

enum DownloadMode
{
	Mod_Auto = 0, 
	Mod_P2P, 
	Mod_Http, 
	Mod_P2SP, 
	Mod_Unknown
};

enum NotifyMessageID
{
    Msg_FileLength = 0,	        // �ļ�������Ϣ����Ϣ���� wp Ϊ�ļ����ȸ��ֽڣ�����Ϊ DWORD��lp Ϊ�ļ����ȵ��ֽڣ�����Ϊ DWORD
    Msg_DownloadInfo,           // ������Ϣ��Ϣ����Ϣ���� wp Ϊ�������ֽڣ�    ����Ϊ DWORD��lp Ϊ������ʱ�䣬    ����Ϊ DWORD
    Msg_UploadInfo,             // �ϴ���Ϣ��Ϣ����Ϣ���� wp Ϊ�ϴ����ֽڣ�    ����Ϊ DWORD��lp Ϊ�ϴ���ʱ�䣬    ����Ϊ DWORD
	Msg_FileComplete,           // ���ؽ�����Ϣ����Ϣ���� wp Ϊ���ؽ����      ����Ϊ DWORD, (0x0: ʧ��; 0x1: �ɹ�; 0x2: �û�ȡ��; 0x3: �û���ͣ, 0x4; �ϴ�ֹͣ)��lp ����

	Msg_ModeChange,             // ����ģʽ�ı���Ϣ����Ϣ���� wp Ϊ����ģʽ��  ����Ϊ DWORD��lp ����
	Msg_StartDownload,          // ��ʼ������Ϣ����Ϣ���� wp Ϊ�������ͣ�      ����Ϊ DWORD��(0x0: ��ʼ; 0x1: ����(��ͣ��))��lp ����
};

enum StateMessageID
{
	Stat_Info = 0,              // Info: (int) ����״̬ 0 = ? 1 = Downloading 2 = Pause 3 = Stop .....
	Stat_Mode,                  // Mode: (int) ����ģʽ 1 = P2P 2 = HTTP 3 = P2SP

	Stat_FileSavePath,          // FileSavePath: (wchar_t*) xxx �����ļ�����·��
	Stat_FileName,              // FileName: (wchar_t*) xxx �����ļ���
	Stat_FileLength,            // FileLength:    (int) xxx �����ļ�����
	Stat_DownResult,            // DownResult:    (int) xxx ���ؽ�� 0 ~ 3 = ? 4 = finish 8 = error ...

	Stat_DownloadSize,          // DownloadSize:  (int) xxx �������ֽ�
	Stat_DownloadTimes,         // DownloadTimes: (int) xxx ����ʱ��
	Stat_DownloadRate,          // DownloadRate:  (int) xxx �����ٶ�
	Stat_UploadSize,            // UploadSize:    (int) xxx �ϴ����ֽ�
	Stat_UploadTimes,           // UploadTimes:   (int) xxx �ϴ�ʱ��
	Stat_UploadRate,            // UploadRate:    (int) xxx �ϴ��ٶ�
	Stat_Seeds, 
	Stat_Peers
};


interface IMSP2PClientNotify : public IUnknown
{
	STDMETHOD(OnNotify)(const unsigned char* urlhash, NotifyMessageID atype, WPARAM wp, LPARAM lp) = 0;
};

MS_DEFINE_IID(IMSP2PClientNotify, "{15010CF0-6392-438c-983D-84F5A16A597A}");


// urlhash �����ʾ, 20�ֽڵĶ����ƴ�
interface IMSP2PClient : public IMsP2PPlugin
{
	// P2P ����
	STDMETHOD(StartWithURL)(const P2PString url, const P2PString strSaveFilePathName, OUT unsigned char* urlhash) = 0;
	STDMETHOD(Pause)       (const unsigned char* urlhash) = 0;      // urlhash = 0xfffffff....ff   === pause all task
	STDMETHOD(Resume)      (const unsigned char* urlhash) = 0;		
	STDMETHOD(Stop)        (const unsigned char* urlhash, BOOL remove_files = FALSE) = 0;

	// ����״̬����
	// nlen: ͳһ���ݻ�������С�����ֽڼ���. 
	STDMETHOD(GetStat)     (const unsigned char* urlhash, StateMessageID statid, void* data, int nlen) = 0;
	STDMETHOD(SetStat)     (const unsigned char* urlhash, StateMessageID statid, void* data, int nlen) = 0;

	// P2P ����
	STDMETHOD(SetOption)(P2POption option, void * data, int nlen) = 0;
	STDMETHOD(GetOption)(P2POption option, void * data, int nlen) = 0;

	// P2P �����¼�֪ͨ
	STDMETHOD(SetNotify)(IMSP2PClientNotify * data) = 0;
};

MS_DEFINE_IID(IMSP2PClient, "{A6023D5C-E39A-4e85-829B-7E4602BED8B5}");

// {5065C70F-0F00-4b66-8323-CDB22AC08FEE}
MS_DEFINE_GUID(CLSID_MSP2PClient, 0x5065c70f, 0xf00, 0x4b66, 0x83, 0x23, 0xcd, 0xb2, 0x2a, 0xc0, 0x8f, 0xee);


// ---------------------------------------------------------------------
// ��ע��

// ����ģʽ
// ����ʹ��P2P����
// ������س�ʱ�������޶�ʱ����û�����ص��κ����ݣ� �л��� http ���ط�ʽ 

// ��ʱ
// ��ʱ�������ֿ���, ���ӳ�ʱ���� (Opt_P2PTimeOut) �������ٶȹ�С���� (Opt_P2PTolerate). 
// ��ʱʱ����Ҫ�������ԣ�������Ի���ʱ���������л����ط�ʽ

// ����
// ȫ��������  SetOption ����
// ������������ SetStat ����

// �ַ���������� UNICODE ��ʽ

// ֪ͨ��Ϣ
// 1 �����Ϣ��
//   ���������ʱ����֪ͨʹ���ߡ���������û��ֹͣ�����û�ֹͣʱ���������ֹͣ��Ϣ��
// 

// ע�⣺
// 1 ����ֹͣ����ӿ�ʱ
//   ���һ��һ��������ã��п������ֹͣ��������
//   ������Ϊ P2P �ڲ����������У����ض��� �� �ȴ����С������ض����е��������ؽ�������
//   ֹͣ����ӵȴ������л�ȡ��һ����������.
//   ����Ƽ�ֹͣ���ýӿڣ�
//   IMSP2PClient->Stop(AnyUrlHash).
//   �����ȵ���δ��ʼ���ص������ٵ����������ص�����
// 

// ģ��ʹ�÷�ʽָ��:
// 1 ����������
//  IMSP2PClient->SetOption(Opt_P2PHostName, _T("RAV"));
//  IMSP2PClient->SetOption(Opt_P2PAutoUpload, 1);
//  IMSP2PClient->Init()
//. IMSP2PClient->StartWithURL()
//
// 2 ����������������ػ��ϴ�
//  IMSP2PClient->SetOption(Opt_P2PAutoUpload, 0, 4);
//  IMSP2PClient->Init();
// 
// 3 ���Ӵ� HTTP ���ؽӿ�. ��������� p2p ���ӻ�ȡ. 
//   P2P ģʽ���÷�ʽ���䣺
//  unsigned char urlhash[20];
//  memcpy(urlhash, NullUrlHash, P2P_HASH_SIZE);
//  IMSP2PClient->StartWithURL();
//   HTTP ģʽ���÷�ʽ��
//  unsigned char urlhash[20];
//  memcpy(urlhash, HttpUrlHash, P2P_HASH_SIZE);
//  IMSP2PClient->StartWithURL();
// 


#endif
