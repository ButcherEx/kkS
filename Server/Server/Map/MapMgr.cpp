#include "MapMgr.h"

InvokerPtr MapMgr::NewMap()
{
	__ENTER_FUNCTION
	
	MapPtr PtrMap = POOL_NEW(Map);
	
	Assert(PtrMap);
	Assert(PtrMap->Init());

	m_MapInstanceList.PushBack(PtrMap);
	
	InvokerPtr invokerPtr( new MapInvoker(*PtrMap) );
	return invokerPtr;

	__LEAVE_FUNCTION
		return InvokerPtr();
}