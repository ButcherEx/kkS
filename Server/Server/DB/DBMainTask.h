/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __DBMAIN_SERVICE_H__
#define __DBMAIN_SERVICE_H__

#include "BaseLib.h"
#include "TaskDefine.h"
#include "EventMgr.h"
#include "Task.h"
#include "EventMsg.h"
#include "DBTask.h"

class DBMainTask : public Service
{
public:
	DBMainTask();
	virtual ~DBMainTask();
public:
	virtual bool		Init();
	virtual void		Tick(const TimeInfo& rTimeInfo);
	virtual int32_t		GetServiceID() { return ServiceDefine::DATABASE;} 
public:
private:
	void				InitDBTask();
private:
	TVector<DBTaskPtr> m_DBTaskVec;
};

typedef MakeInvoker<DBMainTask, 10> DBMainTaskInvoker;
#endif


