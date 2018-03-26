#pragma once
namespace msdk{;
namespace msapi{;

static BOOL WriteAsciiStringToClipboard(LPCWSTR sClipdata, HWND hOwningWnd)
{

	BOOL bRet = FALSE;

	do 
	{
		if ( !OpenClipboard(hOwningWnd))
			break;

		if ( !EmptyClipboard() )
			break;

		HGLOBAL hClipboardData = NULL;
		size_t sLen = wcslen(sClipdata);

		hClipboardData = GlobalAlloc(GMEM_DDESHARE, (sLen+1)*sizeof(wchar_t));
		if( !hClipboardData )
			break;

		wchar_t * pchData = (wchar_t*)GlobalLock(hClipboardData);
		if( !pchData )
			break;

		wcscpy_s(pchData, sLen+1, sClipdata);

		if ( !GlobalUnlock(hClipboardData) )
			break;

		if ( SetClipboardData(CF_UNICODETEXT, hClipboardData) )
		{
			bRet = TRUE;
			break;
		}
		
	} while (0);

	CloseClipboard();

	return bRet;
}

static int CopyFileToClipboard(const char szFileName[], DWORD dwDropEffect = DROPEFFECT_COPY)
{
	UINT uDropEffect;
	HGLOBAL hGblEffect;
	LPDWORD lpdDropEffect;
	DROPFILES stDrop;

	HGLOBAL hGblFiles;
	LPSTR lpData;

	//������彻�������ݸ�ʽ���Ǳ�׼�������ʽ�������Ҫע��
	uDropEffect = RegisterClipboardFormatA("Preferred DropEffect");
	//Ϊ���������ݷ���ռ�
	hGblEffect = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE,sizeof(DWORD));
	lpdDropEffect = (LPDWORD)GlobalLock(hGblEffect);
	//���ò�����ģʽ
	*lpdDropEffect = dwDropEffect;//����; ��������DROPEFFECT_MOVE
	GlobalUnlock(hGblEffect);

	//������������Ҫһ������ṹ
	stDrop.pFiles = sizeof(DROPFILES);//�ļ��б�����ڽṹ��ƫ����
	stDrop.pt.x = 0;
	stDrop.pt.y = 0;
	stDrop.fNC = FALSE;
	stDrop.fWide = FALSE;//���ַ�����

	//�������ݿռ䣬����Ԥ���ļ����Ŀռ�
	//�ļ��б�Ϊ����'\0'��β�������Ҫ�����2���ֽ�
	hGblFiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE,\
		sizeof(DROPFILES)+strlen(szFileName)+2);

	lpData = (LPSTR)GlobalLock(hGblFiles);
	memcpy(lpData,&stDrop,sizeof(DROPFILES));
	strcpy(lpData+sizeof(DROPFILES),szFileName);
	GlobalUnlock(hGblFiles);

	//��ͨ�ļ��������
	OpenClipboard(NULL);
	EmptyClipboard();
	//���ü������ļ���Ϣ
	SetClipboardData(CF_HDROP,hGblFiles);
	//���ü����������Ϣ
	SetClipboardData(uDropEffect,hGblEffect);
	CloseClipboard();

	return 1;
}
};};