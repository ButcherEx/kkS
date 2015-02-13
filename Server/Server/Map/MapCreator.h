/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __MAP_CREATOR_H__
#define __MAP_CREATOR_H__

#include "BaseLib.h"
#include "EventMgr.h"
#include "Task.h"
#include "EventMsg.h"
#include "MapMgr.h"

class MapCreator : public Task
{
public:
	bool		Init();
	void		Excute();
	void		Exit();
	int32_t		GetTaskID();
private:
	TaskManager m_MapTaskManager;
};


#endif
