
#pragma once
namespace DuiKit{;

struct IDuiImage : public IDuiObject
{
	virtual VOID SetName(LPCWSTR lpszName) = 0;
	virtual LPCWSTR GetName() = 0;

	virtual BOOL SetFile(LPCWSTR lpszFile) = 0;
	virtual LPCWSTR GetFile() = 0;

	virtual VOID SetDuiSkin(IDuiSkin* Skin) = 0;
	virtual IDuiSkin* GetDuiSkin() = 0;

	/*�������õķ�Χ*/
	virtual VOID SetAffectRect( CDuiRect AffectRect) = 0;
	virtual CDuiRect GetAffectRect() = 0;

	//������ָͼƬ��װscale9��ʽ����
	virtual VOID SetScale9(CDuiRect Scale9Rect) = 0;
	virtual CDuiRect GetScale9() = 0;

	//�����Ǹ���֧��alphaͨ����ͼƬ��ʽ����bmp��ָ��͸��ɫ
	virtual VOID SetMask(DWORD dwMask) = 0;
	virtual DWORD GetMask() = 0;

	//����ͼƬ��͸����
	virtual VOID SetFade(BYTE fade) = 0; 
	virtual BYTE GetFade() = 0;

	//������ָ��scale9����ʱҪ��Ҫ�����м䲿��
	virtual VOID SetHole(BOOL bHole) = 0;
	virtual BOOL GetHole() = 0;

	//�������ó�true����ָ��ͼƬ��x�᲻Ҫ�������ƽ��
	virtual VOID SetXtiled(BOOL xtiled) = 0;
	virtual BOOL GetXtiled() = 0;

	//�������ó�true����ָ��ͼƬ��y�᲻Ҫ�������ƽ��
	virtual VOID SetYtiled(BOOL ytiled) = 0;
	virtual BOOL GetYtiled() = 0;


	//


	virtual BOOL Create() = 0;
	virtual HBITMAP GetHandle() = 0;

	virtual DWORD GetHeight() = 0;		//ͼƬ��ԭʼ�߶�
	virtual DWORD GetWidth() = 0;		//ͼƬ��ԭʼ���

	//�Ƿ�֧��͸��
	virtual BOOL  IsAlphaChannel() = 0;
};

};//namespace DuiKit{;