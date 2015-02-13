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

uint32_t DBMainTask::Tick(const TimeInfo& rTimeInfo)
{
	__ENTER_FUNCTION

	EventMgr::Update(rTimeInfo);
	Task::Tick(rTimeInfo);

	__LEAVE_FUNCTION
	return 0;
}

void DBMainTask::InitDBTask()
{
	__ENTER_FUNCTION

#define DBTASK_NUM 10
	for( int32_t i = 0; i < DBTASK_NUM; i++)
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


