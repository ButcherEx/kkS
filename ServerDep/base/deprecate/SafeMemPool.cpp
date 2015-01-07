#include "x_common.h"
#include "safe_mem_pool.h"

namespace wrapper {
//-----------------------------------------------------------------------------
// construction / destruction
//-----------------------------------------------------------------------------
SafeMemPool::SafeMemPool(size_t dwMaxPoolSize)
{
	m_dwMaxSize = dwMaxPoolSize;
	m_dwCurrentSize = 0;
	m_dwMalloc = 0;
	m_dwGCTimes = 0;
	m_lLock = 0;
	for( int n=0; n<16; n++ )
		m_Pool[n].pFirst = m_Pool[n].pLast = NULL;
}


SafeMemPool::~SafeMemPool()
{
	for( int n=0; n<16; n++ )
	{
		while( m_Pool[n].pFirst )
		{
			tagNode* pNode = m_Pool[n].pFirst;
			m_Pool[n].pFirst = m_Pool[n].pFirst->pNext;
			free(pNode);
		}
	}
}

} // namespace wrapper {
