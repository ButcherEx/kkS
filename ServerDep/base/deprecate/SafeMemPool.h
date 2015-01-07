
#pragma once

namespace wrapper {

class SafeMemPool
{
public:
	inline void*	Alloc(size_t dwBytes);
	inline void		Free(void* pMem);
	inline void*	TryAlloc(size_t dwBytes);	// 尝试进入锁定区域
	inline BOOL		TryFree(void* pMem);		// 尝试进入锁定区域
	inline void		SetMaxSize(size_t dwSize) { m_dwMaxSize = dwSize; }
	inline size_t		GetSize() { return m_dwCurrentSize; }
	inline size_t		GetMalloc() { return m_dwMalloc; }
	inline size_t		GetGC() { return m_dwGCTimes; }

	SafeMemPool(size_t dwMaxPoolSize=16*1024*1024);
	~SafeMemPool();

private:
	struct tagNode	// 内存块头描述
	{
		tagNode*	pNext;
		tagNode*	pPrev;
		int			nIndex;
		size_t		dwSize;
		size_t		dwUseTime;
		size_t		pMem[1];	// 实际内存空间
	};

	struct
	{
		tagNode*	pFirst;
		tagNode*	pLast;
	} m_Pool[16];

	size_t m_dwMaxSize;		// 外部设定的最大允许空闲内存
	size_t m_dwMalloc;			// 统计用，实际Malloc次数
	size_t m_dwGCTimes;		// 统计用，实际垃圾收集次数

	size_t volatile 	m_dwCurrentSize;	// 内存池中空闲内存总数
	AutomicInt32 	m_lLock;

	inline void Lock() { while( m_lLock.compareAndSet(0, 1) != 0) this_thread::sleep(get_system_time() + posix_time::milliseconds(0)); }
	inline void Unlock() { m_lLock.getAndSet( 0 ); }
	inline bool TryLock() { return m_lLock.compareAndSet(0, 1) == 0; }

	// 垃圾收集
	inline void GarbageCollect(size_t dwExpectSize, size_t dwUseTime);
	// 返回最匹配的大小
	inline int SizeToIndex(size_t dwSize, size_t& dwRealSize);
};


//-----------------------------------------------------------------------------
// 分配
//-----------------------------------------------------------------------------
void* SafeMemPool::Alloc(size_t dwBytes)
{
	size_t dwRealSize = 0;
	int nIndex = SizeToIndex(dwBytes, dwRealSize);
	if( -1 != nIndex )
	{
		if( m_Pool[nIndex].pFirst )	// 提前尝试
		{
			Lock();
			if( m_Pool[nIndex].pFirst )	// 池里有，就从池里分配
			{
				tagNode* pNode = m_Pool[nIndex].pFirst;
				m_Pool[nIndex].pFirst = m_Pool[nIndex].pFirst->pNext;
				if( m_Pool[nIndex].pFirst )
					m_Pool[nIndex].pFirst->pPrev = NULL;
				else
					m_Pool[nIndex].pLast = NULL;
				m_dwCurrentSize -= dwRealSize;
				++pNode->dwUseTime;
				Unlock();
				return pNode->pMem;	
			}
			Unlock();
		}
	}

	++m_dwMalloc;
	tagNode* pNode = (tagNode*)malloc(dwRealSize + sizeof(tagNode) - sizeof(size_t));
	if( !pNode )
		return NULL;

	pNode->nIndex = nIndex;
	pNode->dwSize = dwRealSize;
	pNode->pNext = NULL;
	pNode->pPrev = NULL;
	pNode->dwUseTime = 0;
	return pNode->pMem;	// 从实际内存中分配
}


//-----------------------------------------------------------------------------
// 释放
//-----------------------------------------------------------------------------
void SafeMemPool::Free(void* pMem)
{
	tagNode* pNode = (tagNode*)(((LPBYTE)pMem) - sizeof(tagNode) + sizeof(size_t));
	if( -1 != pNode->nIndex )
	{
		Lock();
		if( pNode->dwSize + m_dwCurrentSize > m_dwMaxSize && pNode->dwUseTime > 0 )
			GarbageCollect(pNode->dwSize*2, pNode->dwUseTime);	// 垃圾收集

		if( pNode->dwSize + m_dwCurrentSize <= m_dwMaxSize ) // 内存池可以容纳
		{
			pNode->pPrev = NULL;
			pNode->pNext = m_Pool[pNode->nIndex].pFirst;
			if( m_Pool[pNode->nIndex].pFirst )
				m_Pool[pNode->nIndex].pFirst->pPrev = pNode;
			else
				m_Pool[pNode->nIndex].pLast = pNode;

			m_Pool[pNode->nIndex].pFirst = pNode;
			m_dwCurrentSize += pNode->dwSize;
			Unlock();
			return;
		}
		Unlock();
	}

	free(pNode);
}


//-----------------------------------------------------------------------------
// 分配
//-----------------------------------------------------------------------------
void* SafeMemPool::TryAlloc(size_t dwBytes)
{
#if _WIN32_WINNT < 0x0400
	return Alloc(dwBytes);
#else
	size_t dwRealSize = 0;
	int nIndex = SizeToIndex(dwBytes, dwRealSize);
	if( -1 != nIndex )
	{
		if( !TryLock() )
			return NULL;

		if( m_Pool[nIndex].pFirst )	// 池里有，就从池里分配
		{
			tagNode* pNode = m_Pool[nIndex].pFirst;
			m_Pool[nIndex].pFirst = m_Pool[nIndex].pFirst->pNext;
			if( m_Pool[nIndex].pFirst )
				m_Pool[nIndex].pFirst->pPrev = NULL;
			else
				m_Pool[nIndex].pLast = NULL;
			m_dwCurrentSize -= dwRealSize;
			++pNode->dwUseTime;
			Unlock();
			return pNode->pMem;	
		}
		Unlock();
	}

	++m_dwMalloc;
	tagNode* pNode = (tagNode*)malloc(dwRealSize + sizeof(tagNode) - sizeof(size_t));
	if( !pNode )
		return NULL;

	pNode->nIndex = nIndex;
	pNode->dwSize = dwRealSize;
	pNode->pNext = NULL;
	pNode->pPrev = NULL;
	pNode->dwUseTime = 0;
	return pNode->pMem;	// 从实际内存中分配
#endif


}


//-----------------------------------------------------------------------------
// 释放
//-----------------------------------------------------------------------------
BOOL SafeMemPool::TryFree(void* pMem)
{
#if _WIN32_WINNT < 0x0400
	Free(pMem);
	return TRUE;
#else
	tagNode* pNode = (tagNode*)(((LPBYTE)pMem) - sizeof(tagNode) + sizeof(size_t));
	if( -1 != pNode->nIndex )
	{
		if( !TryLock() )
			return FALSE;

		if( pNode->dwSize + m_dwCurrentSize > m_dwMaxSize && pNode->dwUseTime > 0 )
			GarbageCollect(pNode->dwSize*2, pNode->dwUseTime);	// 垃圾收集

		if( pNode->dwSize + m_dwCurrentSize <= m_dwMaxSize ) // 内存池可以容纳
		{
			pNode->pPrev = NULL;
			pNode->pNext = m_Pool[pNode->nIndex].pFirst;
			if( m_Pool[pNode->nIndex].pFirst )
				m_Pool[pNode->nIndex].pFirst->pPrev = pNode;
			else
				m_Pool[pNode->nIndex].pLast = pNode;

			m_Pool[pNode->nIndex].pFirst = pNode;
			m_dwCurrentSize += pNode->dwSize;
			Unlock();
			return TRUE;
		}
		Unlock();
	}
	free(pNode);
	return TRUE;
#endif
}



//-----------------------------------------------------------------------------
// 垃圾收集
//-----------------------------------------------------------------------------
void SafeMemPool::GarbageCollect(size_t dwExpectSize, size_t dwUseTime)
{
	++m_dwGCTimes;
	size_t dwFreeSize = 0;
	for(int n=15; n>=0; --n)	// 从最大的开始回收
	{
		if( !m_Pool[n].pFirst )
			continue;

		tagNode* pNode = m_Pool[n].pLast; // 从最后开始释放，因为后面的Node使用次数少
		while( pNode )
		{
			tagNode* pTempNode = pNode;
			pNode = pNode->pPrev;
			if( pTempNode->dwUseTime < dwUseTime )	// 释放此节点
			{
				if( pNode )
					pNode->pNext = pTempNode->pNext;
				if( pTempNode->pNext )
					pTempNode->pNext->pPrev = pNode;
				if( m_Pool[n].pLast == pTempNode )
					m_Pool[n].pLast = pNode;
				if( m_Pool[n].pFirst == pTempNode )
					m_Pool[n].pFirst = pTempNode->pNext;

				m_dwCurrentSize -= pTempNode->dwSize;
				dwFreeSize += pTempNode->dwSize;
				free(pTempNode);
			}

			if( dwFreeSize >= dwExpectSize )
				return;
		}
	}
}


//-----------------------------------------------------------------------------
// 返回最匹配的大小
//-----------------------------------------------------------------------------
int SafeMemPool::SizeToIndex(size_t dwSize, size_t& dwRealSize)
{
	if( dwSize<=32 )		{ dwRealSize = 32;			return 0; }
	if( dwSize<=64 )		{ dwRealSize = 64;			return 1; }
	if( dwSize<=128 )		{ dwRealSize = 128;			return 2; }
	if( dwSize<=256 )		{ dwRealSize = 256;			return 3; }
	if( dwSize<=512 )		{ dwRealSize = 512;			return 4; }
	if( dwSize<=1024 )		{ dwRealSize = 1024;		return 5; }
	if( dwSize<=2*1024 )	{ dwRealSize = 2*1024;		return 6; }
	if( dwSize<=4*1024 )	{ dwRealSize = 4*1024;		return 7; }
	if( dwSize<=8*1024 )	{ dwRealSize = 8*1024;		return 8; }
	if( dwSize<=16*1024 )	{ dwRealSize = 16*1024;		return 9; }
	if( dwSize<=32*1024 )	{ dwRealSize = 32*1024;		return 10; }
	if( dwSize<=64*1024 )	{ dwRealSize = 64*1024;		return 11; }
	if( dwSize<=128*1024 )	{ dwRealSize = 128*1024;	return 12; }
	if( dwSize<=256*1024 )	{ dwRealSize = 256*1024;	return 13; }
	if( dwSize<=512*1024 )	{ dwRealSize = 512*1024;	return 14; }
	if( dwSize<=1024*1024 )	{ dwRealSize = 1024*1024;	return 15; }
	dwRealSize = dwSize;
	return -1;
}
} // namespace wrapper {
