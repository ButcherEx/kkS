/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __MAP_H__
#define __MAP_H__

#include "BaseLib.h"
#include "Task.h"
#include "EventMgr.h"
#include "EventMsg.h"

class Map
{
public:
	virtual int32_t GetMapType( ) { return 0; }
	int32_t	GetMapId() const { return m_MapId; }
	int32_t GetMapInstanceId() const { return m_MapInstanceId; }
private:
	int32_t	m_MapId;
	int32_t m_MapInstanceId;
};

POOL_DECL(Map);
typedef MakeInvoker<Map, 100> MapInvoker;

#endif 

