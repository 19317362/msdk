// dsHftpEngine.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "dsHftpEngine.h"

DSHTTPFTPENGINE_API IdsHftpEngine * IdsNew_HftpEngine()
{
	return new dsHftpEngine();
}