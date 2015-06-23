/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __DB_SERVICE_H__
#define __DB_SERVICE_H__


#include "BaseLib.h"
#include "TaskDefine.h"
#include "EventMgr.h"
#include "Task.h"
#include "EventMsg.h"

class DBTask : public EventMgr
{
public:
	DBTask( );
	virtual ~DBTask();
public:
	virtual void		Tick(const TimeInfo& rTimeInfo){}
	bool Init();
};

POOL_DECL(DBTask);
typedef MakeInvoker<DBTask, 50> DBTaskInvoker;

#endif

