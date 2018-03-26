/*********************************************************
�ļ�����:���ļ���������������������������Ľӿ�����
ʱ��    :2010/07/12
         �ں˰�ȫ�о���
*********************************************************/

/****************************************************
       ����������̾����ļ�:
       [create] [filename] [size] [disktype:1=Dyn;0=Static] [encrypttype:1=AES Encrypt;0=No Encrypt] [encryptcode:AES code <=16 char]
	 |        |           |        |                         |                                        |
       *argv[0]  *argv[1]    *argv[2]  *argv[3]                 *argv[4]                                 *argv[5]

       eg:create c:\rsdisk.img 1G 0 1 123456
                               xM
			       xK

       ��ʽ���������:���ڼ���������̲���ǰ���������ʽ���������Ϊָ���ļ���ʽ��
       [format] [filename] [format:NTFS;FAT32;FAT16] [lable] [quick:q=quick;n=normal] [encryptcode:AES code <=16 char]
       |         |           |                        |          |                         |
       *argv[0]  *argv[1]   *argv[2]                 *argv[3]   *argv[4]                  *argv[5]

       eg:format c:\rsdisk.img NTFS worldcup q 123456


       �����������:
       [mount] [filename] [drive] [readonly:1=ROnly;0=RWOnly] [encryptcode:AES code <=16 char]
       |         |          |           |                         |
       *argv[0]  *argv[1]   *argv[2]    *argv[3]                  *argv[4]

       eg:mount c:\rsdisk.img Z 0 123456

       ж���������:
       [umount] [drive]
       |         |
       *argv[0]  *argv[1]

       eg:umount Z
****************************************************/



/****************************************************
��������:rsvdisk_create
����    :����������̾����ļ���
����    :AES keyΪС��16�ֽڡ���ͬ
����ֵ  :TRUE  �ɹ�
	 FALSE ʧ��
****************************************************/
typedef __int64 int64_t;

typedef enum {
	STATIC_DISK = 0,
	DYN_DISK
} DISKTYPE;

typedef enum {
	NO_ENCRYPT = 0,
	AES_ENCRYPT
} ENCRYPTTYPE;

BOOL WINAPI rsvdisk_create(const char *filename, int64_t disksize,
	       DISKTYPE disktype, ENCRYPTTYPE encrypttype, const char *key);


/****************************************************
��������:rsvdisk_format
����    :��ʽ��������̡����������δ����ʱʹ��
����    :
����ֵ  :TRUE  �ɹ�
	 FALSE ʧ��
****************************************************/
typedef enum {
	NTFS = 0,
	FAT32,
	FAT16
} FSFORMAT;

typedef enum {
	QUICK = 0,
	NO_QUICK	
} FORMATTYPE;

BOOL WINAPI rsvdisk_format(const char *filename, FSFORMAT fsformat, 
	       const char *disklabel, FORMATTYPE formattype, const char *key);


/****************************************************
��������:rsvdisk_mount
����    :����������̡�
����    :
����ֵ  :TRUE  �ɹ�
	 FALSE ʧ��
****************************************************/
typedef enum {
	RWONLY = 0,
	RONLY
} RWTYPE;

BOOL WINAPI rsvdisk_mount(const char *filename, char driverletter, 
			   RWTYPE rwtype, const char *key);


/****************************************************
��������:rsvdisk_umount
����    :ж��������̡�
����    :
����ֵ  :TRUE  �ɹ�
	 FALSE ʧ��
****************************************************/
BOOL WINAPI rsvdisk_umount(char driverletter);