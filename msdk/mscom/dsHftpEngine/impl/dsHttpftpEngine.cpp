// dsHftpEngine.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "dsHftpEngine.h"

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� dsHftpEngine.h
dsHftpEngine::dsHftpEngine()
{
	m_missonmgr.Init();

	return;
}

int dsHftpEngine::AddMission( __in PDSHFTPMISSON pDsHftpMission )
{
	return m_missonmgr.AddMission(pDsHftpMission);
}
BOOL dsHftpEngine::DeleteMission( __in LPCTSTR lpszMissionMark )
{
	return m_missonmgr.DeleteMission(lpszMissionMark);
}

void dsHftpEngine::DeleteAllMission()
{
	m_missonmgr.DeleteAllMission();
}
BOOL dsHftpEngine::ConfigureEngine( HFTPPROC pfnHftpProc, UINT uMax )
{
	if(NULL == pfnHftpProc)
		return FALSE;

	if(uMax < MINLEN_LOCAL_PIECE || uMax > MINLEN_LOCAL_PIECE)
		return FALSE;

	g_dwMaxLocalPieceSize = uMax;
	return TRUE;
}

BOOL dsHftpEngine::TestUrlRetSize( __in LPCTSTR lpszUrl, __out PDSURLTESTRET pUtr )
{
	return TRUE;
}

BOOL dsHftpEngine::SuspendMission( __in LPCTSTR lpszMissionMark )
{
	return TRUE;
}

void dsHftpEngine::SuspendAllMission()
{

}



DSHTTPFTPENGINE_API IdsHftpEngine * IdsNew_HftpEngine()
{
	return new dsHftpEngine();
}