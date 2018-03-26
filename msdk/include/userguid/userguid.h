
#ifndef _MSUSERGUID_H
#define _MSUSERGUID_H

#include "GetNetAdapterInfo.h"
#include "GetDiskInfo.h"
//#include "GetCpuInfo.h" //modify by magj:64λ���ñ��룬��ȥ����
#include "GetMBSerial.h"

#include "guidio.h"
#include "guidcrc.h"
#ifndef RSCPU_SIZE
#define RSCPU_SIZE	8
#endif
namespace msdk {


/*using namespace msdk;*/

struct GuidItem
{
	BYTE itemValue[10];
	int  itemLen;
	GuidItem(BYTE* value, int len)
	{
		itemLen = len;
		ZeroMemory(itemValue, sizeof(itemValue));
		memcpy(itemValue, value, itemLen);
	}

	GuidItem(const GuidItem &item)
	{
		itemLen = item.itemLen;
		ZeroMemory(itemValue, sizeof(itemValue));
		memcpy(itemValue, item.itemValue, itemLen);
	}

	GuidItem& operator=(const GuidItem &item)
	{
		itemLen = item.itemLen;
		ZeroMemory(itemValue, sizeof(itemValue));
		memcpy(itemValue, item.itemValue, itemLen);

		return( *this );
	}
};
typedef std::vector<GuidItem> GuidValue;

#define USERGUID_MAC		0x01
#define USERGUID_DISK		0x02
#define USERGUID_CPU		0x04
#define USERGUID_MAINBOARD	0x08

//��ӦֵҪ������ֵ�����������
#define RANDOM_MAC			0x10
#define RANDOM_DISK			0x20
#define RANDOM_CPU			0x40
#define RANDOM_MAINBOARD	0x80

#define USERGUID_NOWMI		0x80	//��WMI��ʽ��ȡӲ����Ϣ

//Լ������guid��ȡ��ЩӲ����Ϣ������
//ȫ��
#define	USERGUID_MAKE_ALL	(USERGUID_MAC|USERGUID_DISK|USERGUID_CPU|USERGUID_MAINBOARD)

//esmר�ã�ֻʹ��disk��mainboard����Ҫ��Ӳ���춯
#define	USERGUID_MAKE_ESM	(USERGUID_DISK|USERGUID_MAINBOARD|USERGUID_NOWMI)


#define GUIDSIZE_MAC	6
#define GUIDSIZE_DISK	4
#define GUIDSIZE_CPU	1
#define GUIDSIZE_MB		4
	/*
		USERGUID�����
		---------------------------------------------------------------
		|1byte|     6byte      |    4byte    |1byte|    4byte    |
		|- M -|------ A -------|----- B -----|- C -|----- D -----|
		---------------------------------------------------------------
		M:	��ʶ�����A��B��C��D��Щ��Ϣ��������Ӳ����Ϣ����֮�����������
		A:	MAC��ַ		����
		B��	Ӳ����Ϣ	����
		C��	CPU��Ϣ		����
		D��	������Ϣ	����
	*/
	class CUserGuid
	{
	public:
		CUserGuid()
		{
			operator=(CUserGuid::NewGuid());
		}

		CUserGuid(const GUID& guid)
		{
			operator=(guid);
		}

		CUserGuid(const CUserGuid& userGuid)
		{
			operator=(userGuid.GetUserGuid());
		}

		//��ȡGUID��Ϣ
		GUID GetUserGuid() const{return m_userGuid;}
	
		//��ȡGUID��ռ��λ��Ϣ
		BYTE GetMask(){return m_infoBit;}

		operator GUID&(){return m_userGuid;};

		CUserGuid& operator=(const GUID& guid)
		{
			m_userGuid = guid;

			int nIndex = 0;

			memcpy(&m_infoBit, (BYTE*)&m_userGuid + nIndex, sizeof(BYTE));
			nIndex += sizeof(BYTE);

			if(m_infoBit&USERGUID_MAC)
			{
				memcpy(m_macAddress, (BYTE*)&m_userGuid + nIndex, sizeof(m_macAddress));
				nIndex += sizeof(m_macAddress);
			}

			if(m_infoBit&USERGUID_DISK)
			{
				memcpy(m_diskInfo, (BYTE*)&m_userGuid + nIndex, sizeof(m_diskInfo));
				nIndex += sizeof(m_diskInfo);
			}

			if(m_infoBit&USERGUID_CPU)
			{
				memcpy(m_cpuInfo, (BYTE*)&m_userGuid + nIndex, sizeof(m_cpuInfo));
				nIndex += sizeof(m_cpuInfo);
			}

			if(m_infoBit&USERGUID_MAINBOARD)
			{
				memcpy(m_mainBoardInfo, (BYTE*)&m_userGuid + nIndex, sizeof(m_mainBoardInfo));
				nIndex += sizeof(m_mainBoardInfo);
			}

			return( *this );
		}

		bool operator==(const GUID& guid)
		{
			return operator==(CUserGuid(guid));
		}

		bool operator==(const CUserGuid& userGuid)
		{
			//����Ҫbit���
			if(userGuid.m_infoBit != this->m_infoBit)
				return false;

			//������Ҫ��Ч��λֵ
			if( (userGuid.m_infoBit&USERGUID_MAC) && !(userGuid.m_infoBit&RANDOM_MAC) &&
				memcmp(&userGuid.m_macAddress, &this->m_macAddress, GUIDSIZE_MAC)
				)
				return false;

			if(userGuid.m_infoBit&USERGUID_DISK && !(userGuid.m_infoBit&RANDOM_DISK) &&
				memcmp(&userGuid.m_diskInfo, &this->m_diskInfo, GUIDSIZE_DISK))
				return false;

			if(userGuid.m_infoBit&USERGUID_CPU && !(userGuid.m_infoBit&RANDOM_CPU) &&
				memcmp(&userGuid.m_cpuInfo, &this->m_cpuInfo, GUIDSIZE_CPU))
				return false;

			if(userGuid.m_infoBit&USERGUID_MAINBOARD && !(userGuid.m_infoBit&RANDOM_MAINBOARD) &&
				memcmp(&userGuid.m_mainBoardInfo, &this->m_mainBoardInfo, GUIDSIZE_MB))
				return false;

			return true;
		}

		static GUID GetGuid(BYTE byMakeMask = USERGUID_MAKE_ALL, IGuidIO* pIO = NULL)
		{
			IGuidIO* pGuidIO = pIO;
			if(NULL == pGuidIO)
			{
				pGuidIO = static_cast<IGuidIO*>(new CRegGuidIO());
			}
			if(!pGuidIO) //�ⶼʧ���ˣ������´����İ�
				return NewGuid(byMakeMask);

			//�ȶ�һ���Ƿ���Ҫ��ע����������maskֵ
			BYTE byRegMask = pGuidIO->GetMask();

			//�����ȡ������һ��
			GUID oldGuid = GUID_NULL;	//��guid��Ҳ�Ƿ��ص�guid

			if ( byRegMask == byMakeMask)
			{
				if ( SUCCEEDED(pGuidIO->Get(oldGuid)) )
				{
					return oldGuid;
				}
			}

			if(byRegMask)
				byMakeMask = byRegMask;

			GUID newGuid = NewGuid(byMakeMask);

			BOOL bUseNew = TRUE;
			
			if(SUCCEEDED(pGuidIO->Get(oldGuid)) && CUserGuid(oldGuid) == newGuid)
			{
				//�������ϵ� �� �¾�һ���Ǿ��ü���������
				bUseNew = FALSE;
			}

			if(bUseNew)
			{
				oldGuid = newGuid;
				pGuidIO->Set(oldGuid);
				pGuidIO->SetMask(byMakeMask);
			}

			if(!pIO && pGuidIO)
			{
				delete static_cast<CRegGuidIO*>(pGuidIO);
				pGuidIO = NULL;
			}

			return oldGuid;
		}

		static HRESULT RegMask(BYTE& byMask, BOOL bSet = TRUE, IGuidIO* pIO = NULL)
		{
			IGuidIO* pGuidIO = pIO;
			if(NULL == pGuidIO)
			{
				pGuidIO = static_cast<IGuidIO*>(new CRegGuidIO());
			}
			if(!pGuidIO)
				return E_FAIL;

			HRESULT hr = S_OK;
			if(bSet)
				hr = pGuidIO->SetMask(byMask);
			else
				byMask = pGuidIO->GetMask();

			if(!pIO && pGuidIO)
			{
				delete static_cast<CRegGuidIO*>(pGuidIO);
				pGuidIO = NULL;
			}

			return hr;
		}

		//ʵʱ����һ��GUID
		static GUID NewGuid(BYTE byMakeMask = USERGUID_MAKE_ALL)
		{
			BYTE mac[RSMAC_SIZE];
			char disk[RSDISK_SIZE];
			BYTE cpu[RSCPU_SIZE];
			char mb[RSMB_SIZE];

			return NewGuid(mac, disk, cpu, mb, byMakeMask);
		}

		//ʵʱ����һ��GUID
		static GUID NewGuid(BYTE mac[RSMAC_SIZE],
			char disk[RSDISK_SIZE],
			BYTE cpu[RSCPU_SIZE],
			char mb[RSMB_SIZE],
			BYTE byMakeMask = USERGUID_MAKE_ALL)
		{
			ZeroMemory(mac, RSMAC_SIZE);
			ZeroMemory(disk, RSDISK_SIZE);
			ZeroMemory(cpu, RSCPU_SIZE);
			ZeroMemory(mb, RSMB_SIZE);

			BYTE infoBit = 0x0;

			GUID newGuid = GUID_NULL;
			int guidLen = sizeof(GUID);
			GuidValue guidValue;

			BYTE guid_mac[GUIDSIZE_MAC];
			ZeroMemory(guid_mac, GUIDSIZE_MAC);
			{
				BYTE macAddress[RSMAC_SIZE];
				ZeroMemory(macAddress, sizeof(macAddress));
				if(byMakeMask&USERGUID_MAC)
				{
					infoBit |= USERGUID_MAC;
					if(CGetNetAdapterInfo::GetMac(macAddress))
					{
						memcpy(mac, macAddress, RSMAC_SIZE);
						memcpy(guid_mac, macAddress, min(GUIDSIZE_MAC, RSMAC_SIZE));
					}
					else
					{
						RandomByte(guid_mac, GUIDSIZE_MAC);
						infoBit |= RANDOM_MAC;
					}
					guidValue.push_back(GuidItem(guid_mac, GUIDSIZE_MAC));
				}
			}

			BYTE guid_disk[GUIDSIZE_DISK];
			ZeroMemory(guid_disk, GUIDSIZE_DISK);
			{
				char diskInfo[RSDISK_SIZE];
				ZeroMemory(diskInfo, sizeof(diskInfo));
				if(byMakeMask&USERGUID_DISK)
				{
					infoBit |= USERGUID_DISK;
					if(CGetDiskInfo::GetDiskInfo(diskInfo))
					{
						memcpy(disk, diskInfo, min(RSDISK_SIZE, strlen(diskInfo)));
						unsigned long rcCrc = std_crc32(diskInfo, strlen(diskInfo));
						memcpy(guid_disk, &rcCrc, GUIDSIZE_DISK);
					}
					else
					{
						RandomByte(guid_disk, GUIDSIZE_DISK);
						infoBit |= RANDOM_DISK;
					}
					guidValue.push_back(GuidItem(guid_disk, GUIDSIZE_DISK));
				}
			}

			BYTE guid_cpu[GUIDSIZE_CPU];
			ZeroMemory(guid_cpu, GUIDSIZE_CPU);
			{
				BYTE cpuInfo[RSCPU_SIZE];
				ZeroMemory(cpuInfo, sizeof(cpuInfo));
				if(byMakeMask&USERGUID_CPU)
				{
					infoBit |= USERGUID_CPU;
					if(/*CGetCpuInfo::GetCpuId(cpuInfo)*/FALSE) //��ʱȥ����
					{
						memcpy(cpu, cpuInfo, RSCPU_SIZE);
						BYTE rcCrc = std_crc8((const char *)cpuInfo, RSCPU_SIZE);
						memcpy(guid_cpu, &rcCrc, GUIDSIZE_CPU);
					}
					else
					{
						RandomByte(guid_cpu, GUIDSIZE_CPU);
						infoBit |= RANDOM_CPU;
					}
					guidValue.push_back(GuidItem(guid_cpu, GUIDSIZE_CPU));
				}
			}

			BYTE guid_mb[GUIDSIZE_MB];
			ZeroMemory(guid_mb, GUIDSIZE_MB);
			{
				char mainboardInfo[RSMB_SIZE];
				ZeroMemory(mainboardInfo, sizeof(mainboardInfo));
				if(byMakeMask&USERGUID_MAINBOARD)
				{
					infoBit |= USERGUID_MAINBOARD;
					if(CGetMainBoardInfo::GetMainBoardInfo(mainboardInfo, byMakeMask&USERGUID_NOWMI?false:true))
					{
						memcpy(mb, mainboardInfo, min(RSMB_SIZE, strlen(mainboardInfo)));
						unsigned long rcCrc = std_crc32(mainboardInfo, strlen(mainboardInfo));
						memcpy(guid_mb, &rcCrc, GUIDSIZE_MB);
					}
					else
					{
						RandomByte(guid_mb, GUIDSIZE_MB);
						infoBit |= RANDOM_MAINBOARD;
					}
					guidValue.push_back(GuidItem(guid_mb, GUIDSIZE_MB));
				}
			}

			if(0 == infoBit)
				return newGuid;

			int nIndex = 0;
			memcpy((BYTE*)&newGuid + nIndex, &infoBit, sizeof(BYTE));
			nIndex += sizeof(BYTE);

			int guidValueCount = (int)guidValue.size();
			int addCount = 0;
			while(nIndex < 16)	//ֻҪ���Ȳ������Ǿͼ�����
			{
				//����������Ӧ����ʹ���ĸ�Ӳ����ֵ
				int addIndex = addCount%guidValueCount;
				int nCopyLen = min(16 - nIndex, guidValue[addIndex].itemLen);

				memcpy((BYTE*)&newGuid + nIndex, guidValue[addIndex].itemValue, nCopyLen);
				nIndex += nCopyLen;
				addCount++;
			}
			/*memcpy((BYTE*)&newGuid + nIndex, guid_mac, sizeof(guid_mac));
			nIndex += sizeof(guid_mac);

			memcpy((BYTE*)&newGuid + nIndex, guid_disk, sizeof(guid_disk));
			nIndex += sizeof(guid_disk);

			memcpy((BYTE*)&newGuid + nIndex, guid_cpu, sizeof(guid_cpu));
			nIndex += sizeof(guid_cpu);

			memcpy((BYTE*)&newGuid + nIndex, guid_mb, sizeof(guid_mb));
			nIndex += sizeof(guid_mb);*/

			/*guid->Data1^=0xf07f5f46;
			guid->Data2^=0x1234;
			guid->Data3^=0x5678;*/

			return newGuid;
		}

		/*
		static unsigned __int32 GetCycleCount()
		{
			__asm _emit 0x0F
			__asm _emit 0x31
		}
		*/

		static void randominit()
		{
			static bool bInit = false;
			if(!bInit)
			{
				srand(GetTickCount());
				bInit = true;
			}
		}

		static void RandomByte(BYTE* pByte, DWORD dwByteSize)
		{
			//Ϊ�˱�֤ÿ�ζ��ܻ�ȡ��Ψһ��ֵ�����ظ��մ���
			memset(pByte, 0, dwByteSize);
			return ;
			randominit();

			//randһ�β�������BYTE
			for(DWORD i=0; i<dwByteSize; i+=2)
			{
				WORD randNum = rand();
				if((dwByteSize-i) / 2)
				{
					memcpy(pByte + i, &randNum, sizeof(WORD));
				}
				else
				{
					BYTE byTmp = HIBYTE(randNum) ^ LOBYTE(randNum);
					memcpy(pByte + i, &byTmp, sizeof(byTmp));
				}
			}
		}

	private:
		GUID m_userGuid;
		BYTE m_infoBit;
		BYTE m_macAddress[GUIDSIZE_MAC];
		BYTE m_diskInfo[GUIDSIZE_DISK];
		BYTE m_cpuInfo[GUIDSIZE_CPU];
		BYTE m_mainBoardInfo[GUIDSIZE_MB];
	};

} //namespace msdk

#endif	//_MSUSERGUID_H