#include "StdAfx.h"
#include "XMutex.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


/////////////////////////////////////////////////////////////////////////////
XMutex::XMutex()
{
	InitializeCriticalSection(&m_cs);
}

/////////////////////////////////////////////////////////////////////////////
XMutex::~XMutex()
{
	DeleteCriticalSection(&m_cs);
}

/////////////////////////////////////////////////////////////////////////////
VOID XMutex::Lock()
{
	EnterCriticalSection(&m_cs);
}

/////////////////////////////////////////////////////////////////////////////
VOID XMutex::Unlock()
{
	LeaveCriticalSection(&m_cs);
}