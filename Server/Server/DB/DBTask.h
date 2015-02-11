/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __DB_TASK_H__
#define __DB_TASK_H__


#include "BaseLib.h"
#include "Task.h"
#include "TaskDefine.h"
#include "EventMgr.h"
#include "EventMsg.h"

class DBTask : public TaskBase, public EventMgr
{
public:
	DBTask( );
	virtual ~DBTask();
public:
	bool Init();
};

POOL_DECL(DBTask);
typedef MakeTaskDelegate<DBTask, 50> DBTaskDelegate;

#endif

