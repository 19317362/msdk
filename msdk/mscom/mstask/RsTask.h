// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
//��ı�׼�������� DLL �е������ļ��������������϶���� RSTASK_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
//�κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ 
// RSTASK_API ������Ϊ�ǴӴ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef RSTASK_EXPORTS
#define RSTASK_API __declspec(dllexport)
#else
#define RSTASK_API __declspec(dllimport)
#endif
//#include "irstask.h"

//RSTASK_API IRSTask * GetClassObj();
//RSTASK_API BOOL ReleaseObj(IRSTask * pClass);


