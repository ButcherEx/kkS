/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __MAP_MGR_H__
#define __MAP_MGR_H__

#include "Map.h"
class MapMgr
{
public:
	virtual InvokerPtr NewMap( );
public:
public:
	InvokerPtr			FetchInvoker( );
	void				AddInvoker(InvokerPtr taskPtr);
private:
	TSList<InvokerPtr>	m_InvokerPtrList;
private:
	TList<MapPtr> m_MapInstanceList;
};

typedef boost::shared_ptr<MapMgr> MapMgrPtr;

#endif

