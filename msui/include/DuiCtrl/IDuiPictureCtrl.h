#pragma once
namespace DuiKit{;

enum 
{
	PICTURE_SHOWTYPE_NULL = 0x0001,		//��ԭʼ�ȣ��ӿؼ������Ͻǿ�ʼ���㣬ͼƬ����ԭʼ��С
	PICTURE_SHOWTYPE_FULL = 0x0002,		//���ؼ������
	PICTURE_SHOWTYPE_CENTER = 0x0004,	//ͼƬ������ʾ
	PICTURE_SHOWTYPE_ORIGINAL = 0x0008,	//ԭʼ�ߴ�
	PICTURE_SHOWTYPE_STRETCH = 0x0010,	//����������
};

struct IDuiPictureCtrl : public IDuiObject
{
	virtual VOID SetImage(LPCWSTR lpszFile) = 0;
	virtual LPCWSTR GetImage() = 0;

	virtual DWORD GetImageWidth() = 0;
	virtual DWORD GetImageHeigh() = 0;

	virtual VOID SetShowTyte(INT showType) = 0;
	virtual INT GetShowType() = 0;
};

};