#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <windows.h>

#include "Lua_Unicode.h"
#include "lauxlib.h"

#define BUFF_SIZE 1024
#define CHAR_SCALE (sizeof(wchar_t)/sizeof(char))
#define CMD_BUFF_SIZE 4096

wchar_t * AToU (const char *str)
{
	int textlen;
	wchar_t *result;

	textlen = MultiByteToWideChar (CP_ACP, 0, str, -1, NULL, 0);
	result = (wchar_t *) malloc ((textlen + 1) * sizeof (wchar_t));
	memset (result, 0, (textlen + 1) * sizeof (wchar_t));
	MultiByteToWideChar (CP_ACP, 0, str, -1, (LPWSTR) result, textlen);
	return result;
}

char * UToA (const wchar_t * str)
{
	char *result;
	int textlen;

	// wide char to multi char
	textlen = WideCharToMultiByte (CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char *) malloc ((textlen + 1) * sizeof (char));
	memset (result, 0, sizeof (char) * (textlen + 1));
	WideCharToMultiByte (CP_ACP, 0, str, -1, result, textlen, NULL, NULL);
	return result;
}

wchar_t * U8ToU (const char *str)
{
	int textlen = 0;
	wchar_t *result = 0;

	textlen = MultiByteToWideChar (CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t *) malloc ((textlen + 1) * sizeof (wchar_t));
	memset (result, 0, (textlen + 1) * sizeof (wchar_t));
	MultiByteToWideChar (CP_UTF8, 0, str, -1, (LPWSTR) result, textlen);
	return result;
}

char * UToU8 (const wchar_t * str)
{
	char *result;
	int textlen;

	// wide char to multi char
	textlen = WideCharToMultiByte (CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char *) malloc ((textlen + 1) * sizeof (char));
	memset (result, 0, sizeof (char) * (textlen + 1));
	WideCharToMultiByte (CP_UTF8, 0, str, -1, result, textlen, NULL, NULL);
	return result;
}

static int Unicode_a2u (lua_State * L)
{
	const char *str;
	wchar_t *result;

	/*���ݵ�һ������ */
	str = lua_tostring (L, -1);
	/*��ʼת�� */
	result = AToU (str);
	/*����ֵ�� */
	lua_pushlstring (L, (char *) result, wcslen (result) * CHAR_SCALE);
    free(result);
	return 1;
}

static int Unicode_u2a (lua_State * L)
{
	const wchar_t *str;
	char *result;

	/*���ݵ�һ������ */
	str = (wchar_t *) lua_tostring (L, -1);
	/*��ʼת�� */
	result = UToA (str);
	/*����ֵ�� */
	lua_pushstring (L, result);
    free(result);
	return 1;
}

static int Unicode_u2u8 (lua_State * L)
{
	const wchar_t *str;
	char *result;

	/*���ݵ�һ������ */
	str = (wchar_t *) lua_tostring (L, -1);
	/*��ʼת�� */
	result = UToU8 (str);
	/*����ֵ�� */
	lua_pushstring (L, result);
    free(result);
	return 1;
}

static int Unicode_u82u (lua_State * L)
{
	const char *str = 0;
	wchar_t *result = 0;

	int len = 0;
	/*���ݵ�һ������ */
	str = lua_tostring (L, -1);
	/*��ʼת�� */
	result = U8ToU (str);
	/*����ֵ�� */
	len = wcslen (result) * CHAR_SCALE;
	lua_pushlstring (L, (char *) result, wcslen (result) * CHAR_SCALE);
    free(result);
	return 1;
}

static int Unicode_a2u8 (lua_State * L)
{
	const char *str;
	wchar_t *temp;
	char *result;

	/*���ݵ�һ������ */
	str = lua_tostring (L, -1);
	/*��ʼת�� */
	temp = AToU (str);
	result = UToU8 (temp);
	/*����ֵ�� */
	lua_pushstring (L, result);
    free(result);
	return 1;
}

static int Unicode_u82a (lua_State * L)
{
	const char *str;
	wchar_t *temp;
	char *result;

	/*���ݵ�һ������ */
	str = lua_tostring (L, -1);
	/*��ʼת�� */
	temp = U8ToU (str);
	result = UToA (temp);
	/*����ֵ�� */
	lua_pushstring (L, result);
    free(result);
	return 1;
}

/*��ȡһ���ļ���С*/
static int _GetFileSize (const char *filename)
{
	long len;
	FILE *fp;

	/*��ֻ�����ļ���seek���ļ�ĩβ�ķ�ʽ��ȡ�ļ���С */
	if ((fp = fopen (filename, "r")) == NULL)
	{
		printf ("%s is not invalid\n", filename);
		return 0;
	}
	fseek (fp, 0, SEEK_END);
	len = ftell (fp);
	fclose (fp);
	return len;
}

/*Lua ��ȡ �ļ���С*/
static int GetFileSizeW (lua_State * L)
{
	/*���ݵ�һ���������ļ��� */
	const char *filename = lua_tostring (L, -1);
	lua_pushinteger (L, _GetFileSize (filename));
	return 1;
}

/*��ȡһ��Unicode�ļ���ʹ��fgetwc������IO�����ٶȽ���*/
static int GetAllFileWC (lua_State * L)
{
	/*���ݵ�һ���������ļ��� */
	int i = 0;
	wchar_t *buf = 0;
	FILE *input = 0;
	const char *filename = lua_tostring (L, -1);
	/*��ȡ�ļ���С */
	int len;
	len = _GetFileSize (filename);
	/*���û����С */
	
	/*����wchar_t����Ϊchar����һ��������buf�ռ��СΪ�ļ�����һ�룬�ټ�ĩβ�� '\0' */
	buf = (wchar_t *) malloc (sizeof (wchar_t) * (len / CHAR_SCALE + 1));

	
	input = fopen (filename, "rb");
	while (!feof (input))
	{
		buf[i++] = fgetwc (input);
	}
	/*�ַ���ĩβ���� */
	buf[i - 1] = L'\0';
	lua_pushlstring (L, (char *) buf, wcslen (buf) * CHAR_SCALE);
	free (buf);
	return 1;
}

/*��ȡһ��Unicode�ļ���ʹ��fgetws�������ٶȽϿ�*/
static int GetAllFileWS (lua_State * L)
{

	int i = 0;
	wchar_t *buf = 0;
	/*���ݵ�һ���������ļ��� */
	const char *filename = lua_tostring (L, -1);
	/*��ȡ�ļ���С */
	FILE *input = fopen (filename, "rb");
	int len = _GetFileSize (filename);
	/*��ʼ������ */
	wchar_t *all;
	all = (wchar_t *) malloc (sizeof (wchar_t) * (len / CHAR_SCALE + 1));
	memset (all, 0, sizeof (wchar_t) * (len / CHAR_SCALE + 1));

	

	buf = (wchar_t *) malloc (sizeof (wchar_t) * BUFF_SIZE);
	memset (buf, 0, sizeof (wchar_t) * BUFF_SIZE);
	while (!feof (input))
	{
		int j = 0;

		fgetws (buf, BUFF_SIZE, input);
		/*���ӻ���ռ䣬ԭ��ʹ��wcscat���ӣ����Ǻ����������⣬ʹ��ָ����ٶȺܿ� */
		while (buf[j] != L'\0')
		{
			all[i++] = buf[j++];
		}
		/*�����ʱbuf */
		memset (buf, 0, sizeof (wchar_t) * BUFF_SIZE);
	}
	all[len / CHAR_SCALE] = L'\0';
	lua_pushlstring (L, (char *) all, wcslen (all) * CHAR_SCALE);
	/*ɾ����ʱ���� */
	free (buf);
	free (all);
	return 1;
}

//��ʼ���ַ���ָ��
char * StringInit ()
{
	char *str = (char *) malloc (1 * sizeof (char));
	memset (str, 0, 1 * sizeof (char));
	return str;
}

//�����ַ���Ϊָ���ַ�
char * StringSet (char *str, const char *toset)
{
	int len = strlen (toset) + 1;
	str = (char *) realloc (str, len * sizeof (char));
	strcpy (str, toset);
	return str;
}

//���ַ���ĩβ���ָ���ַ���
char * StringAppent (char *first, const char *last)
{
	int len = strlen (first) + strlen (last) + 1;

	first = (char *) realloc (first, len * sizeof (char));
	strcat (first, last);
	return first;
}

//���ַ���ָ��λ�����ָ���ַ���
char * StringInsert (char *str, const char *insert, int start)
{
	int i = 0 ,pos = 0,len = 0;
	//�Կ�ʼ��������ֽ�����֤
	if (start > strlen (str))
	{
		start = strlen (str);
	}
	else if (start < 0)
	{
		start = 0;
	}
	//���·�������
	pos = strlen (str);
	len = strlen (str) + strlen (insert) + 1;
	str = (char *) realloc (str, len * sizeof (char));

	//ʹ��ָ��ķ�ʽ���б���
	
	//ͨ������ʽ��׼���������ݺ�λ�õ����ݸ��Ƶ����
	for (i = 1; i <= pos - start; i++)
	{
		str[len - i - 1] = str[len - i - strlen (insert) - 1];
	}
	//��׼����������ݲ��뵽��ʼ����ĵط�
	for (i = 1; i <= strlen (insert); i++)
	{
		str[i + start - 1] = insert[i - 1];
	}
	//ĩβ��0
	str[len - 1] = '\0';
	return str;
}

static const luaL_reg PearFunctions[] = {
	{"a2u", Unicode_a2u},
	{"u2a", Unicode_u2a},
	{"u2u8", Unicode_u2u8},
	{"u82u", Unicode_u82u},
	{"a2u8", Unicode_a2u8},
	{"u82a", Unicode_u82a},
	{"getfilesizew", GetFileSizeW},
	{"getallfilewc", GetAllFileWC},
	{"getallfilews", GetAllFileWS},
	
	{NULL, NULL}
};
#if LUA_VERSION_NUM < 502
#  define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
#endif

LUALIB_API int luaopen_LC (lua_State * L)
{
	luaL_newlib(L, PearFunctions);

	lua_pushvalue(L, -1);
	lua_setglobal(L, LUA_LC_LIBNAME);

	//luaL_openlib (L, LUA_LC_LIBNAME, PearFunctions, 0);
	return 1;
}
