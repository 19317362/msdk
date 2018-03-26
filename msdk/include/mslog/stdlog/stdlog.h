
// ��Ŀ����:
//
// ʵ�ֹ���: ��Ϊ����ģ�����������ͷ�ļ�
//
// �ļ�����: stdlog.h
//
// ��������:
//
// ��������:
//
// �޸�����:
//
// �޸�����:
//
// �������µ��޸�������������ӣ�
//
// ʹ��˵��:
//		����ڵ�ǰ���̱���A.EXE��Ŀ¼����stdlog.ini��������:
//	[CmdLine]
//	A.EXE = -pid -tid
//	B.EXE...
//	...
//
//	��ô���ǻ���Խ����Լ��������У�ʹ��stdlog.ini��ָ���Ĵ˽��̵������в������˹�����Ҫ���ڱ������������־�������
//
// �Ѿ�ʵ�ֵ� ������
// -dbgstr	�����OutputDebugString��һ���ڵ������п��Կ����������ù���DebugView.exeҲ���ԡ�
// -file [c:\a.txt]	�������־�ļ��У�Ĭ��ΪXXX.exe.log��XXXΪ���exe�����֣�����ѡ��ָ��һ���������ļ�����
// -cons			���������̨����stdout��Ҳ����ʹ��printf�������
// -pipe [sss]		�����ĳ���ܵ�������Ϊǰ׺\\.\pipe\XXX������XXXĬ�϶���ΪDebugWindow������ָ��Ϊһ�������ġ�
// -pid				�����ÿһ����־������ǰ����id����[4527]
// -tid				�����ÿһ����־������ǰ�߳�id����[5279]
// -time			�����ÿһ����־������ǰʱ�䣬��2010-01-20 17:48:22
// -group rsplugdll 2 onlyu 3	���Գ�ʼ��ָ�����Դ�ӡ�����־��������ر��-group * 1��ʶĬ����־����Ϊ1��
// -mt				������Ϊ���߳�����ʱ��Ϊ�˷�ֹ��־������̻߳��ң���˿������ô˲���������Ч�ʿ϶����ֹۡ�
//
// ��δʵ��
// -pdb [c:\pdb;]	��Debug���������Assert����ʱ����ʹ���ڵ������ܣ�Ҳ����������õĶ�ջ�Լ�����ֵ�����ҿ���ѡ��pdb����·��
// -wmcpy					�������WM_COPYDATA������־
// -tcp [193.168.19.66:800]	�������Tcp��ʽ������־

//=============================================================================
#pragma once
#include "mslog/syslay/dbg.h"

//�ڱ����Ŀ��ֱ���÷�����İ�
#ifdef STDLOG_LINK_STATIC
	#define STDLOG_API
#else

	#ifdef STDLOG_EXPORTS
		#define STDLOG_API __declspec(dllexport)
	#else
		#define STDLOG_API __declspec(dllimport)
	#endif
#endif


extern STDLOG_API int nstdlog;

extern"C" STDLOG_API  void  InitDumpLib(int iReserved = 0, HMODULE hModule = 0);
extern"C" STDLOG_API  void	TermDumpLib();

