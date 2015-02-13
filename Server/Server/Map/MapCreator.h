/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __MAP_CREATOR_H__
#define __MAP_CREATOR_H__

#include "BaseLib.h"
#include "TaskDefine.h"
#include "EventMgr.h"
#include "Task.h"
#include "EventMsg.h"
#include "MapMgr.h"

class MapCreator : public Task
{
public:
	virtual bool		Init();
	virtual void		Tick(const TimeInfo& rTimeInfo);
	virtual int32_t		GetTaskID() { return TaskDefine::MAP;} 
public:
	virtual	void		Start();
	virtual	void		Load();
	virtual void		Shutdown();
	virtual void		FinalSave();
public:
	void				Exit();
private:
	TaskManager m_MapTaskManager;
};


#endif

