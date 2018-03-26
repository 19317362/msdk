#pragma once
#include <Unknwn.h>

#include <pshpack1.h>
//---------------------------------------------------
//��һ��ʱ����,���ظ��������������
//����Ҫ������ÿ��,ÿ��,ÿ�����������,�ڼ��Ͽ���������չ
//�ȶ����ڴ�,�ݲ�ʵ��
typedef struct tagREPEAT{
	//-----------------------------------
	WORD wIntervalType;    //�������

	WORD wIntervalTime;    //�ڼ������wIntervalType�ж� 
						   //1:ÿ�������Сʱ
						   //2:ÿ������ٷ��� 2ѡһ
	//---------------------------------
	WORD wStopType;        //ֹͣ����,
	WORD wHour;            //1:ֱֹͣʱ��, ֱ�����㼸��ֹͣ
	WORD wMinute;          //1:
						   //2:������ʱ��, ������Сʱ���ֺ�ֹͣ
						   //2:
	//----------------------------------
	WORD wStopRunning;     //��ֹͣʱ��,������������,��ֹͣ
}REPEAT,*LPREPEAT;

//----------------------------------------------------
//������Ĳ�ͬ���ַֿ�����,��ͬ�ķŵ�RSTASK�ṹ��
//��ÿ��,ÿ��,ÿ��
//ÿСʱ
typedef struct tagEVERYHOUR{
	UINT uInterval_Hour;   //ÿ����Сʱ
	WORD wMinute;          //�ڼ������� 
}EVERYHOUR,*LPEVERYHOUR;

//ÿ����
typedef struct tagEVERYDAY{
	UINT uInterval_Day;     //ÿ������
	WORD wHour;             //�ڼ��㼸������  
	WORD wMinute;
	//REPEAT RepeatTime;
}EVERYDAY,*LPEVERYDAY;

//��
typedef struct tagEVERYWEEK{
	UINT uInterval_Week;    //ÿ������
	WORD wHour;             //�ڼ��㼸������  
	WORD wMinute;
	//
	WORD wWeekMark;       //��λ����ƶ�ѡ�����ڼ�.  0111 1111 ���ҵ�����������,����һ ...������ 
	//REPEAT RepeatTime;
}EVERYWEEK,*LPEVERYWEEK;

//��
typedef struct tagEVERYMONTH{
	WORD wHour;             //�ڼ��㼸������  
	WORD wMinute;
	//---------------------------------------------
	// ���ź� ���ڼ� 2ѡ1
	WORD wDay;              //1:ÿ�µĵڼ���

	WORD wWeekNum;          //2:ÿ�µĵڼ�����1-5 ,5��ʾ���һ��
	WORD wDayOfWeek;        //2:���ڼ�

	DWORD wDayMark;         //3:ÿ�µļ��� ��ѡ, �Ȳ�ʵ��
	//----------------------------------------------
	WORD wMonthMark;       //�ڼ���,��λ�����ѡ���·�.�����0x0FFF,����ÿ��,0x1��һ��,0x7����1-3��	 
	                       //0x1 :1, 0x2 :2,0x4 :3, 0x8 :4

	//REPEAT RepeatTime;
}EVERYMONTH,*LPEVERYMONTH;

//-------------------------------------------------------
//����ʼֹͣ����
typedef struct tagDATE{
	WORD wYear;
	WORD wMonth;
	WORD wDay;
}ST_DATE;

//����һ�ε�����ʱ��,������ʱ��
typedef struct tagRUNONESHOT{
	ST_DATE date;
	WORD wHour;             //�ڼ��㼸������  
	WORD wMinute;
}RUNONESHOT,*LPRUNONESHOT;

//�𶯶�ÿ�ʼ����
typedef struct tagAfetrBoot{
	WORD wHours;             //�ڼ��㼸������  
	WORD wMinutes;
	WORD wSeconds;
}AFTERBOOT,*LPAFTERBOOT;

#define NAME_LEN 128
typedef struct tagTASKATTRIB{
	UINT  uType;		      //��������  ��Ӧenum RS_RAVTASK_START_TYPE
	UINT  uIndex;		      //���������index  ��Ӧenum RS_RAVTASK_START_INDEX
	WORD  wUse;               //�Ƿ������������1,0
	WORD  wPower;             //���ñʼǵ��ʱ �Ƿ���������

	WORD  wDelete;            //�Ƿ������������1,0   //�ڲ�ɾ���ã��Գ�����������ɾ��
	WORD  wLock;			  //�Ƿ������������1,0  //����汣��
	UINT  uClassID;           //����ϸ�ִ���- ��ӦRS_RAVTASK_CLASS
	UINT  uIsRising;          //=1�����Ǵ���xml,
	TCHAR szTaskName[NAME_LEN];    //�����ǵ�,�����ID.=0,�û���ֱ���Ǵ�
	TCHAR szTaskDesc[MAX_PATH];    //
}TASKATTRIB,*LPTASKATTRIB;

typedef struct tagTASKSPECIFIC{
	//���ⲿ��,��ѡ��
	WORD wFindAction;      //���������Ѿ����� 0������,1�ر��������,����������
	WORD wRight;           //����Ȩ��,����û����,ûʵ��
	WORD wFloatSecond;     //��������,������ڳ���������ķ�Χ��,����Ϊ��Ч
	WORD wSpecificMethod;  //��������������,��չһЩ���������� RS_RAVTASK_START_SPECIFIC.
}TASKSPECIFIC,*LPTASKSPECIFIC;

typedef struct tagTASKCMD{
	TCHAR cmd[MAX_PATH]; //�����Ĳ��� �ɴ���%SYSTEM% %INSTALL% %WINDIR% ������ 
	TCHAR end[MAX_PATH]; //�����Ĳ��� �ɴ���%SYSTEM% %INSTALL% %WINDIR% ������ 
}TASKCMD,*LPTASKCMD;


typedef struct tagTASKTIME{
	SYSTEMTIME   last_time;   //�����ϴ�������ʱ��,������ѯ��
	SYSTEMTIME   next_time;   //�����´�������ʱ��,������ѯ��
	//ʱ�����
	ST_DATE start_time;  //����ʼ��ʱ��,������ָ�����ʱ�俪��ʼ,����Ϊ��
	ST_DATE stop_time;   //���������ʱ�� ,����ÿ�,��ʾ����ֹͣ����
	//----���union ҪUINT  uType������Ҫ�Ǹ��ṹ
	union{
		EVERYHOUR   every_hour;    //ÿСʱ,START_TASK_TYPE_HOUR
		EVERYDAY    every_day;     //ÿ��,  START_TASK_TYPE_DAY   
		EVERYWEEK   every_week;    //ÿ��,  START_TASK_TYPE_WEEK
		EVERYMONTH  every_month;   //ÿ��,  START_TASK_TYPE_MONTH
		//-----------------------------------------------------
		RUNONESHOT  oneshot_time;  //A:ֻ����һ�ε�����, ʱ��������Сʱ����
		                         
								   //B:��������, Task���к�����һ��,��������������,������ʱ��,��nType�ж���
		AFTERBOOT	after_boot;	   //C:��ϵͳ����,���ǵ�ϵͳ����һ��ʱ���Ժ�����
		//------------------------------------------------------
		//ע���every_hour������: every_hour�й̶��ķ��Ӳ�����; interval_hour��Task���������ʱ��
		//���õ���Աע���Լ������
		//���Сʱ����.(̫���ļ��ʱ��û��,��������Ĵ���)
		UINT      interval_hour;   //��Task������������Сʱ����
	}every; 
}TASKTIME,*LPTASKTIME;

typedef struct tagTASKFINISH{
	//������������Ĳ���
	WORD  wFinishDel;   //���ټƻ������������Ƿ�ɾ��
	WORD  wHours;       //��������������� �����˶���Сʱ���ٷ����Զ�ֹͣ
	WORD  wMintues;     //ͬ��һ��
}TASKFINISH,*LPTASKFINISH;

//----------------------------------------------------------------------
typedef struct tagRSTASK
{
	//XML���ò���
	TASKATTRIB   task_attrib;
	//---------------------------------------------------------
	TASKCMD      task_cmd;
	//---------------------------------------------------------
	//��ѡ����
	TASKSPECIFIC task_specific;
	//---------------------------------------------------------
	TASKTIME     task_time;
	//---------------------------------------------------------
	//��ѡ����
	TASKFINISH   task_finish;
	//XML���ò��ֽ���
	//---------------------------------------------------------
	//---------------------------------------------------------
	DWORD        dwProgressID;  //��Ž���ID,�Ա�ֹͣʹ��
}ST_RSTASK,*LPST_RSTASK; 
#include "PopPack.h"

//RavTask wType - ���������ķ�ʽ ��ӦRSTASK��nType
enum RS_RAVTASK_START_TYPE
{
	START_TASK_TYPE_DISABLE=0,	//����Ϊ��Ч
	START_TASK_TYPE_WITHBOOT,	//��������, Task���к�����һ��,��������������
	START_TASK_TYPE_AFTERBOOT,	//��ϵͳ����,���ǵ�ϵͳ����һ��ʱ���Ժ�����

	START_TASK_TYPE_RUNONESHOT, //����ʱ������һ�� ��-������Сʱ����
	START_TASK_TYPE_MONTH,		//ÿ��		��-wDay, wHour, wMinute
	START_TASK_TYPE_WEEK,		//ÿ��		��-wDayOfWeek, wHour, wMinute
	START_TASK_TYPE_DAY,		//ÿ��		��-wHour, wMinute
	START_TASK_TYPE_HOUR,		//ÿСʱ	��-wMinute

	START_TASK_TYPE_INTERVAL_HOUR,	      //ÿ�����Сʱ����
	START_TASK_TYPE_MESSEAGE,            //��Ӧ���ǵ���Ϣ������,ûʱ��ṹ,����ɨ��
	START_TASK_TYPE_AFTERUPDATE,          //����������ɱ��
	START_TASK_TYPE_INSTANTUPDATE,        //��ʱ����

};

//RavTask �������������,��ӦRSTASK��nInex
//�޸�,����,��ѯ����ʱ��Ҫ
//ÿ������Ҫ��ͬ���������� ��Ӧ������rstask.xml��index="xx"
//�û��ֶ���ӵ����񶼴� 1001 ��ʼ����
//����ֻ��˵��һ��
enum RS_RAVTASK_START_INDEX
{
	START_TASK_INDEX_RAV_TIMER=1,  
	START_TASK_INDEX_RAV_BOOT,  
	START_TASK_INDEX_RAVMON,  

	START_TASK_INDEX_CUSTOM=1000,    //�ɴӴ�ֵ��+1,���û�ж��������,�Ժ�������鿴��ʹ��
};

//---------------------------------
//���������������,Ҫ�����ش���֧��,���ڻ���û��
enum RS_RAVTASK_START_SPECIFIC
{
	START_TASK_SPECIFIC_DISABLE = 0,	//û�����⴦���
	START_TASK_SPECIFIC_SMARTUP,		//��ʱ����

	//START_TASK_SPECIFIC_RAVMON ,		//�������	RavMon.exe
	//START_TASK_SPECIFIC_RAV_BOOT,		//����ɨ��	Rav.exe -boot
	//START_TASK_SPECIFIC_RAV_TIMER,	//��ʱɱ��
	//START_TASK_SPECIFIC_NEWVER,		//����µİ汾
	//START_TASK_SPECIFIC_RAVHDBAK,		//Ӳ�̱���
	//START_TASK_SPECIFIC_CLEAN_LOG,	//��ʱ������־
};

//�������,����ѯ��
enum RS_RAVTASK_CLASS
{
	RS_RAVTASK_CLASS_ALL = 0,
	RS_RAVTASK_CLASS_SMARTUP,
	RS_RAVTASK_CLASS_CLEAN_LOG,
	RS_RAVTASK_CLASS_BACUP_BOOT,
	RS_RAVTASK_CLASS_IDLE_SCAN,   //���ж�������
	RS_RAVTASK_CLASS_TMIER_TASK,  
	RS_RAVTASK_CLASS_SCREEN,      // ��������
};
//#define ERROR_SUCCESS         0
#define ERROR_EXIST_TASK      (-1)
#define ERROR_NOEXIST_TASK    (-2)
#define ERROR_SAVE_FILE       (-3)
#define ERROR_LOAD_FILE       (-4)
#define ERROR_NOEXIST_FILE    (-5)


interface ITASK :public IUnknown
{
	//------------------------------------------------------------
	//�����������
	//------------------------------------------------------------
	//����: �������ò�Ʒ����XML�ļ�,�����ĳ���ʹ��,
	//      Ĭ�ϵ�xmlΪͬrstask.dllĿ¼�µ�rstask.xml 
	//����: lpFileName �ļ��� 
	//Ӱ��: ��ͬĿ¼������rstask.ini��¼rstask.xml��ȫ·��
	STDMETHOD(SetConfigFile)(IN const TCHAR * lpFileName) = 0;
	//------------------------------------------------------------
	//����: ���������Ժ󶼲����û�������,
	//����: dwset 1����,0,������
	//Ӱ��: �����ļ�,�޸�USE��ʶ
	STDMETHOD(SetTask)(IN DWORD dwTaskIndex,IN DWORD dwSet) = 0;
	//------------------------------------------------------------
	//����: �������е������Ժ󶼲����û�������,
	//����: dwset 1����,0,������
	//Ӱ��: �����ļ�,�޸�USE��ʶ
	STDMETHOD(SetAllTasks)(IN DWORD dwSet) = 0;
	//-------------------------------------------------------------
	//����: ���һ���������� 
	//����: lpTask =RSTASK������ṹ
	//����: �ɹ��ʹ�����
	//Ӱ��: �����ļ�
	STDMETHOD(AddTask)(IN ST_RSTASK * lpTask) = 0;
	//-------------------------------------------------------------
	//����: ɾ��һ���������� 
	//����: dwTaskIndex ��������
	//Ӱ��: �����ļ�
	STDMETHOD(DeleteTask)(IN DWORD dwTaskIndex) = 0;
	//-------------------------------------------------------------
	//����: �޸�һ���������� 
	//����: lpTask ����ṹ
	//Ӱ��: XML�����ļ�
	STDMETHOD(ModifyTask)(IN ST_RSTASK * lpTask) = 0;
	//-------------------------------------------------------------
	//����: ȡһ���������� 
	//����: dwTaskIndex ��������,lpTask������ṹ
	//Ӱ��: ��
	//����ֵ :0�ɹ�,1û�д�����,�����Ǵ�����
	STDMETHOD(GetTask)(IN DWORD dwTaskIndex,OUT ST_RSTASK& lpTask) = 0;
	//--------------------------------------------------------------------
	//dwTaskClassID = 0 �����е����  TaskList�����б�
	//list Ӧ�øĵ���������list������
	//��һ���õ�dwCount QueryTask(1,NULL,dwCount);
	//�ڶ��� �����߷��� ST_RSTASK*pTaskArray =  new ST_RSTASK[dwCount];QueryTask(1,pTaskArray,dwCount);
	STDMETHOD(QueryTask)(IN DWORD dwTaskClassID,IN OUT ST_RSTASK* pTaskArray,IN OUT DWORD dwCount) = 0;

	//--------------------------------------------------------------
	//��ǰ���������� - �Ժ������鿴��ʹ��,�������̿�ʹ��
	//--------------------------------------------------------------
	//����: ���Ƶ�ǰ������ 
	//����: dwControlID = 1 ����dwControlID = 0 ֹͣ
	STDMETHOD(ControlTask)(IN DWORD dwTaskIndex,IN DWORD dwControlID) = 0;
	//--------------------------------------------------------------
	//����: ���Ƶ�ǰ�������� 
	//����: ����dwControlID = 0 ֹͣ
	STDMETHOD(ControlTasks)(IN DWORD dwControlID) = 0;
	//------------------------------------------------------------
	//����: ȡ����״̬
	//����: dwTaskIndex,����������, 
	//pPrevStartTime�ϴ�����ʱ��, pNextStartTime�´�����ʱ��
	//��������ֵ: �Ƿ�ɹ�,�ʹ�����
	STDMETHOD(QueryTaskStatus)(IN DWORD dwTaskIndex,OUT SYSTEMTIME*pPrevStartTime,OUT SYSTEMTIME*pNextStartTime) = 0;

	//û�ж������ļ�,ֱ��ͨ��ST_RSTASK& Task�����´�����ʱ��
	STDMETHOD(QueryTaskTime)(IN OUT ST_RSTASK& Task) = 0;

};
struct __declspec(uuid("{67941D87-76CE-4c05-BED2-8E33F79CF747}")) ITASK;

interface IRSTask :public ITASK
{
	//-----------------------------------------------------------
	//�������: �Ժ��������,�����ȷ��������˵��һ��,����ûʵ��ֱ�ӷ���0
	//-----------------------------------------------------------
	//����: �����ӿ�,�ṩ������������ ��������ļ��汾,�������µ����ø�ʽ 
	//����: ��
	//����: �ɹ�0 ʧ��-1 ������
	//Ӱ��: ���ɻ����޸������ļ�
	STDMETHOD(UpdateXMLCfg)() =0;
};
struct __declspec(uuid("{B74B6598-ABE8-4a5b-BD03-EDCBAE364CBC}")) IRSTask;
// {53180022-3A69-4f17-BADD-205DA3F94E62}
MS_DEFINE_GUID(CLSID_RSTask, 
			   0x53180022, 0x3a69, 0x4f17, 0xba, 0xdd, 0x20, 0x5d, 0xa3, 0xf9, 0x4e, 0x62 );

interface IRSTaskSrv :public ITASK
{
	//�����������;
	//����: ��ɶ�ȡ����,����timer,���task������
	STDMETHOD(Run)() = 0;
	STDMETHOD(Stop)() = 0;
	STDMETHOD(CfgModify)() = 0;
	STDMETHOD(OnMessage)(IN DWORD msgid,IN LPVOID buffin,IN size_t cc)=0;
};
struct __declspec(uuid("{CAC019D7-235D-4e90-B63B-D09E59DE21E3}")) IRSTaskSrv;
// {E1828137-480E-492e-A577-37F9968CE730}
MS_DEFINE_GUID(CLSID_RSTaskSrv, 
			   0xe1828137, 0x480e, 0x492e,  0xa5, 0x77, 0x37, 0xf9, 0x96, 0x8c, 0xe7, 0x30  );

/////////////////////////////////////////
//��װIRSTaskSrv ��RSCOM start�ӿ�ʹ��
interface ITaskSvrWrap :public IUnknown
{

};
struct __declspec(uuid("{7BC66DD7-2C21-4597-8084-302F64974C6F}")) ITaskSvrWrap;
MS_DEFINE_GUID(CLSID_RSTaskSvrWrap, 
			   // {3CEE7027-0966-48fa-9BBC-B3DAEC3785F3}
				0x3cee7027, 0x966, 0x48fa,  0x9b, 0xbc, 0xb3, 0xda, 0xec, 0x37, 0x85, 0xf3  );