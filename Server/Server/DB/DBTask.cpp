/************************************************************************/
/*                                                                      */
/************************************************************************/
#include "DBTask.h"

POOL_IMPL(DBTask);

DBTask::DBTask()
{

}

DBTask::~DBTask()
{

}

bool DBTask::Init()
{
	__ENTER_FUNCTION
		bool bRet = EventMgr::Init();
		Assert(bRet);

		return true;
	__LEAVE_FUNCTION
	return false;
}


