/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __DBMAIN_TASK_H__
#define __DBMAIN_TASK_H__

#include "BaseLib.h"
#include "Task.h"
#include "TaskDefine.h"
#include "EventMgr.h"
#include "EventMsg.h"
#include "DBTask.h"

class DBMainTask : public Task, public EventMgr
{
public:
	DBMainTask();
	virtual ~DBMainTask();
public:
	virtual bool		Init();
	virtual uint32_t	Tick(const TimeInfo& rTimeInfo);
	virtual int32_t		GetTaskID() { return TaskDefine::DATABASE;} 
public:
private:
	void				InitDBTask();
private:
	TVector<DBTaskPtr> m_DBTaskVec;
};


#endif


