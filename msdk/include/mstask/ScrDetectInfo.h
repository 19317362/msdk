#ifndef __SCREEN_SAVER_DECTECTER__H_
#define __SCREEN_SAVER_DECTECTER__H_
#include <pshpack4.h>
/*******************************************************************************
*
*
*  �ļ�����:    ScrDetectInfo.h 
*
*  ��    ��:    1.00
*  
*  ����ĵ�:    ��
*
*
*
*  Rstray��RavTask������������״̬��ͨѶ�ṹ��
*  ���̣�Rstray������� -> ��������֪ͨRavTask -> Rstrayÿ��һ��ʱ��ͼ���Ƿ�
*��������������������� -> ֪ͨRavTask�������� -> ������������� -> ֪ͨRavTask
*        �뿪������
*
*******************************************************************************/

// ����״̬��־
enum {
	SCRSTATE_START_SCREEN_SAVER = 1,	// �û����濪ʼ��������״̬
	SCRSTATE_STILL_IN_SCREEN_SAVER,		// �û�������Ȼ��������״̬
	SCRSTATE_OVER_SCREEN_SAVER			// �û������뿪����״̬
};

struct tag_ScreenSaverDetectInfo
{
	DWORD dwSessionID;		//SessionID
	DWORD dwScrState;		//����״̬��־
};
typedef tag_ScreenSaverDetectInfo SCREENSAVERDETECTINFO,*LP_SCREENSAVERDETECTINFO;

#include <poppack.h>
#endif // end of __SCREEN_SAVER_DECTECTER__H_