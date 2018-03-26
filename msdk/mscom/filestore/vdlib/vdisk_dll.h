
#ifndef __vdisk_dll_h__
#define __vdisk_dll_h__

#define VDISK_SECTOR_SIZE 512

/* �豸��ͷ���ַ��������ڼ�ʶ���豸 */
#define VDISK_DEVICE_NAME_PREFIX _T("\\Device\\VDisk")

/* ������� */

/* �ɹ� */
#define VDISK_ERROR_SUCCESS			0

/* �ļ�û�з��֣����ߴ��ļ�ʧ�� */
#define VDISK_ERROR_FILE_NOT_FOUND		1

/* �������� */
#define VDISK_ERROR_INVALID_PARAMETER		2

/* û���㹻����Դ��ͨ��Ϊ�ڴ����ʧ�� */
#define VDISK_ERROR_INSUFFICIENT_RESOURCES	3

/* ִ��ʧ�� */
#define VDISK_ERROR_UNSUCCESSFUL		4

/* ������� */
#define VDISK_ERROR_INVALID_KEY			5

/* ��Ч���豸��ͨ���Ǵ��豸ʧ�� */
#define VDISK_ERROR_INVALID_DEVICE		6

/* �û��ṩ�Ļ�����̫С */
#define VDISK_ERROR_BUFFER_TOO_SMALL		7

/* �û��ṩ�Ļ�������С���������Դ洢��Χ */
#define VDISK_ERROR_BUFFER_OVERFLOW		8

typedef unsigned long vdisk_error;


/* ����������� */
typedef int bool_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;

#ifndef FALSE
#define FALSE ((bool_t)0)
#endif

#ifndef TRUE
#define TRUE ((bool_t)1)
#endif

#define VDISK_MAX_KEY_LENGTH	128

/* ������Ϣ�ṹ */

#define VDISK_MAX_FILE_PATH	0x200

struct vdisk_info_entry {

	char diskfile[VDISK_MAX_FILE_PATH];	/* ����ӳ���ļ�·�� */
	uint32_t userid;			/* �û���ʶ */
	char drv_letter;			/* ��������ĸ���� 'c', 'd', ... */
	uint64_t local_time;			/* ���ʹ��ʱ�� */
};



#define VDISK_MAGIC_HI ((uint64_t)('R' << 24) | (uint64_t)('S' << 16) | (uint64_t)('V' << 8) | (uint64_t)('D'))
#define VDISK_MAGIC_LO ((uint64_t)('S' << 24) | (uint64_t)('I' << 16) | (uint64_t)('G' << 8) | (uint64_t)(0xfe))
#define VDISK_MAGIC ((VDISK_MAGIC_HI << 32) | VDISK_MAGIC_LO)
#define VDISK_VERSION 1

/* ���̾����ļ�ͷ */

struct vdisk_header {
	//non crypt info
	uint64_t magic;	/* ��־���� VDISK_MAGIC */
	uint64_t version; /* �汾 VDISK_VERSION */
};


vdisk_error vdisk_init();

vdisk_error vdisk_uninit();
/************************************************************************
 * ���ܣ�
 *	���ؾ����ļ�
 * ������
 *	��IN��diskfile - ���̾����ļ�����
 *	��IN��readonly - �Ƿ����Ϊֻ�����̣�TRUE Ϊֻ��
 *	��IN��key - �������룬��󳤶� VDISK_MAX_KEY_LENGTH
 *	��IN��userid - �û�ID
 *	��IN��drv_letter - ������������ĸ
 * ����ֵ��
 *	vdisk_error ���궨��
/************************************************************************/ 
vdisk_error vdisk_mount( char *diskfile, bool_t readonly, char *key, uint32_t userid, char drv_letter );


/************************************************************************
 * ���ܣ�
 *	ж�ؾ����ļ�
 * ������
 *	��IN��userid - �û�ID
 *	��IN��drv_letter - ������������ĸ
 * ����ֵ��
 *	vdisk_error ���궨��
/************************************************************************/ 
vdisk_error vdisk_umount( uint32_t userid, char drv_letter );


/* ���ܷ�ʽ */
#define VDISK_CRYPT_NONE	0
#define VDISK_CRYPT_AES		1
#define VDISK_CRYPT_CAST5	2
#define VDISK_CRYPT_TWOFISH	3
#define VDISK_CRYPT_MAX		3

/* �û����ݴ�С */
#define VDISK_USER_DATA_SIZE 0x80

#pragma pack (push, 1)

struct vdisk_diskfile_info {

	uint64_t disksize;		/* ���̴�С����λΪ�ֽ� */
	uint32_t crypt_method;
	uint8_t user_data[VDISK_USER_DATA_SIZE];	/* �Զ������ݣ����� vdisk_set_diskfile_info �������ã���СΪ VDISK_USER_DATA_SIZE */
};

#pragma pack (pop)


/************************************************************************
 * ���ܣ�
 *	��ȡ�����ļ���Ϣ
 * ������
 *	��IN��diskfile - ���̾����ļ�����
 *	��IN��info - ��Ϣ���ݣ��� vdisk_diskfile_info
 *	��IN��info_len - info ���ݵ��ܴ�С   sizeof (struct vdisk_diskfile_info)
 * ����ֵ��
 *	vdisk_error ���궨��
/************************************************************************/ 
vdisk_error vdisk_get_diskfile_info( char *diskfile, struct vdisk_diskfile_info *info, uint32_t info_len );

/************************************************************************
 * ���ܣ�
 *	�����û��Զ������ݣ�д���ļ�
 * ������
 *	��IN��diskfile - ���̾����ļ�����
 *	��IN��info - ��Ϣ���ݣ�Ϊ�Զ������ݣ���С������ VDISK_USER_DATA_SIZE
 *	��IN��info_len - info ���ݵ��ܴ�С
 * ����ֵ��
 *	vdisk_error ���궨��
/************************************************************************/ 
vdisk_error vdisk_set_diskfile_info( char *diskfile, void *info, uint32_t info_len );

/************************************************************************
 * ���ܣ�
 *	���������ļ�
 * ������
 *	��IN��diskfile - ���̾����ļ�����
 *	��IN��disksize - ���̾����С����λΪ ���������� VDISK_SECTOR_SIZE ��
 *	��IN��crypt_method - ���ܷ�ʽ
 *	��IN��key - �������룬��󳤶� VDISK_MAX_KEY_LENGTH
 *	��IN��is_dyndisk - �Ƿ��Ƕ�̬���̣�TRUE Ϊ ��̬���̣�����Ϊ��̬����
 * ����ֵ��
 *	vdisk_error ���궨��
/************************************************************************/ 
vdisk_error vdisk_create( char *diskfile, uint64_t disksize, int crypt_method, char *key, bool_t is_dyndisk );


/************************************************************************
 * ���ܣ�
 *	�����û�����
 * ������
 *	��IN��userid - �û�ID
 *	��IN��user_data - �û��Զ�������
 *	��IN��user_data_len - ���ݳ���
 * ����ֵ��
 *	vdisk_error ���궨��
/************************************************************************/ 
vdisk_error vdisk_set_user_data( uint32_t userid, uint8_t *user_data, size_t user_data_len );


/************************************************************************
 * ���ܣ�
 *	��ȡ�û�����
 * ������
 *	��IN�� userid - �û�ID
 *	��OUT��user_data - �û��Զ�������
 *	��IN�� user_data_len - ���ݳ���
 * ����ֵ��
 *	vdisk_error ���궨��
/************************************************************************/ 
vdisk_error vdisk_get_user_data( uint32_t userid, uint8_t *user_data, size_t user_data_len );


/************************************************************************
 * ���ܣ�
 *	��ȡ��ǰ��Ϣ
 * ������
 *	��OUT��info - vdisk_info_entry �ṹ�����飬���ص�ǰ��Ϣ
 *	��IN�� innr - info �������
 *	��OUT��outnr - ����ʵ�ʵĸ���
 * ����ֵ��
 *	vdisk_error ���궨��
/************************************************************************/ 
vdisk_error vdisk_get_info( struct vdisk_info_entry *info, size_t innr, size_t *outnr );


/************************************************************************
 * ���ܣ�
 *	�򿪴��̣������ļ�����
 * ������
 *	��IN��diskfile - �����ļ�·��
 *	��IN�� key - ��������
 *	��OUT��dev - ����һ���豸��������ڲ����ļ�
 * ����ֵ��
 *	vdisk_error ���궨��
/************************************************************************/ 
vdisk_error vdisk_open( char *diskfile, char *key, void **dev );

/************************************************************************
 * ���ܣ�
 *	�رմ���
 * ������
 *	��IN��dev - �豸������� vdisk_open
 * ����ֵ��
 *	vdisk_error ���궨��
/************************************************************************/ 
void vdisk_close( void *dev );


typedef enum {
	fmt_type_unknow,	/* δ֪�ļ�ϵͳ */
	fmt_type_ntfs,
	fmt_type_fat32,
} fmt_type;




/************************************************************************
 * ���ܣ�
 *	��ʽ������
 * ������
 *	��IN��dev - �豸������� vdisk_open
 *	��IN��type - �ļ�ϵͳ���ͣ��� fmt_type
 *	��IN��lable - ���
 * ����ֵ��
 *	vdisk_error ���궨��
/************************************************************************/ 
vdisk_error vdisk_format( void *dev, fmt_type type );

#define	VDISK_OPEN_EXISTING	0x01	/* Opens the file. The function fails if the file is not existing. */
#define	VDISK_CREATE_NEW	0x02	/* Creates a new file. The function fails if the file is already existing. */

/************************************************************************
 * ���ܣ�
 *	��/�����ļ�
 * ������
 *	��IN��dev - �豸������� vdisk_open
 *	��IN��filename - �ļ����ƣ��� "\\abc.txt"
 *	��IN��mode - VDISK_OPEN_EXISTING �� VDISK_CREATE_NEW
 * ����ֵ��
 *	�ļ�ָ�룬���ʧ�����ļ�ָ��Ϊ NULL
/************************************************************************/ 
void *vdisk_fopen( void *dev, char *filename, unsigned char mode );

/************************************************************************
 * ���ܣ�
 *	��ȡ�ļ�
 * ������
 *	��IN��file - �ļ�ָ�룬�� vdisk_fopen
 *	��IN��buffer - ��ȡ������
 *	��IN��bufsize - ��������С
 * ����ֵ��
 *	ʵ�ʶ�ȡ����
/************************************************************************/ 
int vdisk_fread( void *file, void *buffer, unsigned int bufsize );

/************************************************************************
 * ���ܣ�
 *	д���ļ�
 * ������
 *	��IN��file - �ļ�ָ�룬�� vdisk_fopen
 *	��IN��buffer - д�뻺����
 *	��IN��bufsize - ��������С
 * ����ֵ��
 *	ʵ��д�볤��
/************************************************************************/ 
int vdisk_fwrite( void *file, void *buffer, unsigned int bufsize );

/************************************************************************
 * ���ܣ�
 *	�ƶ��ļ�ָ��
 * ������
 *	��IN��file - �ļ�ָ�룬�� vdisk_fopen
 *	��IN��offset - ƫ��
 *	��IN��whence - λ�� SEEK_SET�� SEEK_CUR�� SEEK_END
 * ����ֵ��
 *	��ǰ�ļ�ƫ��
/************************************************************************/ 
int vdisk_flseek( void *file, int offset, int whence );

/************************************************************************
 * ���ܣ�
 *	�ر��ļ�
 * ������
 *	��IN��file - �ļ�ָ�룬�� vdisk_fopen
 * ����ֵ��
 *	��
/************************************************************************/ 
void vdisk_fclose( void *file );

vdisk_error vdisk_mkdir(void *dev, char * path);
vdisk_error vdisk_remove(void *dev, char * name);
vdisk_error vdisk_rename(void *dev, char * oldname, char * newname);
#endif