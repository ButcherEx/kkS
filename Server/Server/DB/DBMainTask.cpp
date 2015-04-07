#include "DBMainTask.h"

DBMainTask::DBMainTask()
{

}

DBMainTask::~DBMainTask()
{

}

bool DBMainTask::Init()
{
	__ENTER_FUNCTION

	bool bRet = EventMgr::Init();
	Assert(bRet);

	InvokerPtr Ptr ( new DBMainTaskInvoker(*this) );
	AddInvoker(Ptr);

	InitDBTask();

	return true;

	__LEAVE_FUNCTION
	return false;
}

void DBMainTask::Tick(const TimeInfo& rTimeInfo)
{
	__ENTER_FUNCTION

	Service::Tick(rTimeInfo);

	__LEAVE_FUNCTION
}

void DBMainTask::InitDBTask()
{
	__ENTER_FUNCTION

#define DBSERVICE_NUM 10
	for( int32_t i = 0; i < DBSERVICE_NUM; i++)
	{
		DBTaskPtr Ptr = POOL_NEW(DBTask);
		bool bRet = Ptr->Init( );
		Assert(bRet);
		m_DBTaskVec.push_back(Ptr);

		InvokerPtr deleagate(new DBTaskInvoker(*Ptr) );
		AddInvoker(deleagate);
	}
		

	__LEAVE_FUNCTION

}



