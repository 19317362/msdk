#pragma once




BOOL IsRunServer(__int32 pid);

typedef CStackLockWrapper<CGlobalMutex> CStackAutoMutexLock;

//size������32�ı���
template<class _Base, typename _T, size_t size>
class CMapTable : public _Base
{
public:
	CMemMapFile m_memtable;
	__int32 m_iMaxAllocSize;			//bytes	
	__int32 m_iMaxCount;				//counts==bits
	CMapTable():m_memtable( size * sizeof(_T) )
	{
		Assert(size%32==0);
		m_iMaxAllocSize = size/8;
		m_iMaxCount = size;

		//Msg("AllocSize = %x , Count = %x, T = %d\n", m_iMaxAllocSize, m_iMaxCount, sizeof(_T));
	}	
	CGlobalMutex m_tablelock;
	CGlobalMutex m_tableopen;
protected:
	inline unsigned __int32& GetAllocBlockMark(__int32 iAllocIndex)
	{
		//���еĿ���
		return *((unsigned __int32*)(m_memtable.GetBuffer())+iAllocIndex);
	}
	inline unsigned __int32 CheckAllocBlock(__int32 iIndex)
	{		
		return GetAllocBlockMark(iIndex/32)&(1<<(iIndex%32));
	}
	inline void SetAllocBlock(__int32 iIndex)
	{
		GetAllocBlockMark(iIndex/32)|=(1<<(iIndex%32));
	}
	inline void SetFreeBlock(__int32 iIndex)
	{
		GetAllocBlockMark(iIndex/32)&=~(1<<(iIndex%32));
	}
	inline _T* GetArray(__int32 nIndex)
	{
		return (_T*)( ((byte*)(m_memtable.GetBuffer())+m_iMaxAllocSize )) + nIndex;
	}	
	__int32 GetFreeIndex()
	{
		for(__int32 i=0;i<m_iMaxCount;i++)
		{
			//��λ��飬�õ��Ƿ�Ϊ���еĿ�
			if( 0==CheckAllocBlock(i) )
				return i;
		}
		return FIND_INVALID_INDEX;
	}
	__int32 GetUsedIndex()
	{
		for(__int32 i=0;i<m_iMaxCount;i++)
		{
			//��λ��飬�õ��Ƿ�Ϊ���õĿ�
			if( 0!=CheckAllocBlock(i) )
				return i;
		}
		return FIND_INVALID_INDEX;
	}
	__int32 FreeUnused()
	{
		__int32 iTotal = 0;
		for(__int32 i=0;i<m_iMaxCount;i++)
		{
			//��λ��飬�õ��Ƿ�Ϊ���õĿ�
			if( 0!=CheckAllocBlock(i) )
			{
				//�������Ч
				_T* pCur = GetArray(i);
				if(!IsRunServer(pCur->pid))
				{
					//Msg("FreeUnused pid=%d\n", pCur->pid);
					//����Ĩ����������ļ�¼
					SetFreeBlock(i);
					ZeroMemory(pCur, sizeof(_T));
					iTotal++;
				}
			}
		}
		return iTotal;
	}
public:
	HRESULT OpenMap(LPCSTR szPrefix, LPCSTR szName)
	{
		TCHAR szTableLock[MAX_PATH ] = {0};		
		lstrcpy(szTableLock, szPrefix);
		lstrcat(szTableLock, "Lock_");
		lstrcpyn(szTableLock+lstrlen(szTableLock), szName, min(MAX_PATH-lstrlen(szTableLock),lstrlen(szName)+1));
		if(!m_tablelock.CreateMutex(NULL, FALSE, szTableLock))
			return E_COMMX_OPEN_SHARE_MUTEX;


		TCHAR szTableOpen[MAX_PATH ] = {0};
		lstrcpy(szTableOpen, szPrefix);
		lstrcat(szTableOpen, "Open_");
		lstrcpyn(szTableOpen+lstrlen(szTableOpen), szName, min(MAX_PATH-lstrlen(szTableOpen),lstrlen(szName)+1));
		if(!m_tableopen.CreateMutex(NULL, FALSE, szTableOpen))
		{
			m_tablelock.CloseHandle();
			return E_COMMX_OPEN_SHARE_MUTEX;
		}

		//szName = "c:\\ravcc.dat";
		// alloctable(4*8) + clientinfo1 + clientinfo2 + clientinfo3 + ... + clientinfo256
		//if(m_memtable.MapFile(szName, FALSE, m_iMaxAllocSize + m_iMaxCount * sizeof(_T),
		//	TRUE, FILE_SHARE_READ|FILE_SHARE_WRITE, FALSE, FALSE))
		//	return S_OK;
		
		//��Ϊmapmemory�����п�����������������������ֻ�ø�Ϊ���Ի�ȡ���open��������open����ʹ�ǻ�ȡ������
		//������ζ��mapmemory����ͻ������⣬ֻҪ�н���������ʼ�������map���Ͳ��������⣬���ǵ�open����Ҫ
		//�Ƿ�ֹͬʱ������																		---onlyu
		//CStackAutoMutexLock so(m_tableopen);
		BOOL bLock = m_tableopen.Lock(5000);	//��Ϊlockʱ��5����
		if(m_memtable.MapMemory(szName, m_iMaxAllocSize + m_iMaxCount * sizeof(_T), FALSE))
		{
			if(bLock) m_tableopen.Unlock();
// 			Msg("<<<begin map %s, dump alloc table:\n", szName);
// 			PBYTE pDate = (PBYTE)(m_memtable.GetBuffer());
// 			for(int i=0;i<m_iMaxAllocSize;i++)
// 			{
// 				Msg("%02x", *(pDate+i));
// 			}
// 			Msg("end map %s>>>\n", szName);
			return S_OK;
		}

		if(bLock) m_tableopen.Unlock();
		return E_COMMX_OPEN_SHARE_FILE;
	}
	HRESULT CloseMap()
	{		
		m_tablelock.CloseHandle();
		m_tableopen.CloseHandle();
		m_memtable.UnMap();
		return S_OK;
	}
	virtual HRESULT FlushData()
	{
		//m_memtable.Flush();
		return S_OK;
	}
	//
	virtual HRESULT QueryFirstObject(HANDLE32 &handle)
	{
		__int32 iFree = GetUsedIndex();
		handle = (HANDLE32)iFree;
		return FIND_INVALID_INDEX==iFree?S_FALSE:S_OK;
	}
	virtual HRESULT QueryNextObject(HANDLE32 &handle)
	{
		CStackAutoMutexLock ssl(m_tablelock);
		__int32 iFree = (__int32)handle;		
		Assert(iFree>=0);
		Assert(iFree<m_iMaxCount);
		iFree++;
		for(__int32 i=iFree;i<m_iMaxCount;i++)
		{
			//��λ��飬�õ��Ƿ�Ϊ���еĿ�
			if( 0!=CheckAllocBlock(i) )
			{
				//�������Ч
				_T* pCur = GetArray(i);
				if(IsRunServer(pCur->pid))
				{
					handle = (HANDLE32)i;
					return S_OK;
				}
				else
				{
					//����Ĩ����������ļ�¼
					SetFreeBlock(i);
					ZeroMemory(pCur, sizeof(_T));					
				}				
			}
		}
		handle = (HANDLE32)FIND_INVALID_INDEX;
		return S_FALSE;
	}
	virtual HRESULT QueryObject(HANDLE32 handle, _T* pClientInfo)
	{		
		Assert(pClientInfo);
		__int32 iFree = (__int32)handle;
		Assert(iFree>=0);
		Assert(iFree<m_iMaxCount);
		if(iFree==FIND_INVALID_INDEX)
		{				
			return S_FALSE;
		}
		CopyMemory(pClientInfo, GetArray(iFree), sizeof(_T));		
		return S_OK;
	}
	virtual HRESULT BeginTrans()
	{
		m_tablelock.Lock();
		return S_OK;
	}
	virtual HRESULT EndTrans()
	{
		m_tablelock.Unlock();
		return S_OK;
	}

	//��ѯ�Ƿ���ڣ�����Ѿ���һ���򷵻ش��ڵ�objid������S_OK��û���򷵻�S_FALSE
	virtual HRESULT Find(_T* pci)
	{
		HRESULT hr = S_FALSE;		
		CStackAutoMutexLock ssl(m_tablelock);
		for(__int32 i=0;i<m_iMaxCount;i++)
		{
			//��λ��飬�õ��Ƿ�Ϊ���еĿ�
			if( 0!=CheckAllocBlock(i) )
			{
				//����Ƿ�Ϊƥ��
				_T* pCur = GetArray(i);
				if(*pci==*pCur)
				{						
					CopyMemory(pci, pCur, sizeof(_T));
					hr = S_OK;
					break;
				}
			}
		}		
		return hr;
	}

	//ɾ��ָ���Ķ���
	virtual HRESULT Delete(_T* pci)
	{
		CStackAutoMutexLock ssl(m_tablelock);
		for(__int32 i=0;i<m_iMaxCount;i++)
		{
			//��λ��飬�õ��Ƿ�Ϊ���еĿ�
			if( 0!=CheckAllocBlock(i) )
			{
				//����Ƿ�Ϊƥ��
				_T* pCur = GetArray(i);
				if(*pci==*pCur)
				{					
					SetFreeBlock(i);
					ZeroMemory(pCur, sizeof(_T));						
					break;
				}
			}
		}		
		return S_OK;
	}
};

//�������̷��������ֹ����������������ֵ���ɾ���
class CCCenterNames : public CMapTable<ICCenterNames, SERVERINFO, 128>
	, public CUnknownImp					
{
	static __int32 s_iProcess;
	static CHAR s_szServerName[SERVER_RPC_NAME_MAX];
	void RandName(DWORD dwParam);
public:
	UNKNOWN_IMP1(ICCenterNames);

	STDMETHOD(init_class)(IMSBase* prot, IMSBase* punkOuter){return S_OK;}

	HRESULT Open(LPCSTR szName);
	virtual HRESULT Close();

	//����(ͨ��pid)���ڵ�ServerName���ظ��˸����Ƿ�ǿ�Ƹ��Ǿ�����������E_COMMX_ADD_EXIST_ONE�����߸����ϵļ�¼
	virtual HRESULT Add(SERVERINFO* psi, BOOL bForce);
};


//����ͨѶ�������ֹ����������������ֵ���ɾ���
class CClientObjectNames : public CMapTable<IClientObjectNames, CLIENTINFO, 512>
	, public CUnknownImp
{
public:
	UNKNOWN_IMP1(IClientObjectNames);

	STDMETHOD(init_class)(IMSBase* prot, IMSBase* punkOuter){return S_OK;}

	virtual HRESULT Open(LPCSTR szName);
	virtual HRESULT Close();

	//��ѯ�Ƿ���ڣ�����Ѿ���һ���򷵻ش��ڵ�objid������E_COMMX_ADD_EXIST_ONE��û�������һ���µ����ֲ������µ�objid���سɹ�
	virtual HRESULT Add(CLIENTINFO* pci, BOOL bForce);
};