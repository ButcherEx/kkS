/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __MAP_H__
#define __MAP_H__

#include "BaseLib.h"
#include "Task.h"
#include "EventMgr.h"
#include "EventMsg.h"

class Map : public TaskBase
{
public:
	virtual int32_t GetMapType( ) { return 0; }
};



#endif 

