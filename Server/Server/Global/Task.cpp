/************************************************************************/
/*                                                                      */
/************************************************************************/

#include "BaseLib.h"
#include "Config.h"
#include "Task.h"

//////////////////////////////////////////////////////////////////////////

TaskDelegate::TaskDelegate(uint32_t interval )
: m_TaskInterval(interval), m_State(STATE_READY)
{

}

TaskDelegate::~TaskDelegate()
{

}

void TaskDelegate::UpdateTimeInfo( )
{
	m_TimeInfo.Update();
}

void TaskDelegate::Excute( )
{
	SetState(STATE_EXECUTE);

	__ENTER_FUNCTION_EX
		Do();
	__LEAVE_FUNCTION_EX

	SetState(STATE_READY);
}
//////////////////////////////////////////////////////////////////////////
Task::Task( )
{
	SetState(TASK_STOP);
}

Task::~Task()
{
	m_TaskPtrList.Clear( );
}

bool Task::Init()
{
	return true;
}

uint32_t Task::Tick(const TimeInfo& rTimeInfo)
{
	return 0;
}


TaskDelegatePtr Task::FetchTask()
{
	TaskDelegatePtr taskPtr;
	m_TaskPtrList.PopFront(taskPtr);
	return taskPtr;
}

void Task::AddTask(TaskDelegatePtr taskPtr)
{
	m_TaskPtrList.PushBack(taskPtr);
}

//////////////////////////////////////////////////////////////////////////
LOG_DEF(TaskManager);

TaskManager g_TaskManager;

TaskManager::TaskManager()
{

}

TaskManager::~TaskManager()
{

}


bool TaskManager::Init(int32_t maxTask, int32_t maxThread)
{
	__ENTER_FUNCTION

		Assert((maxTask && maxThread));

		m_ThreadPoolPtr = ThreadPoolPtr(new ThreadPool(maxThread));
		Assert(m_ThreadPoolPtr);

		m_TaskPtrVec.resize(maxTask);

		return true;
	__LEAVE_FUNCTION

		return false;
}

bool TaskManager::Register(TaskPtr taskPtr)
{
	__ENTER_FUNCTION
	Assert(taskPtr);
	Assert((taskPtr->GetTaskID() >= 0));
	Assert((taskPtr->GetTaskID() < (int32_t)m_TaskPtrVec.size()));
	Assert(!m_TaskPtrVec[taskPtr->GetTaskID()]);

	m_TaskPtrVec[taskPtr->GetTaskID()] = taskPtr;

	return true;
	__LEAVE_FUNCTION
		return false;
}

void TaskManager::Excute( )
{
	__ENTER_FUNCTION
	__LEAVE_FUNCTION
}

void TaskManager::Exit()
{
	__ENTER_FUNCTION
		boost::xtime now_xt;
		xtime_get(&now_xt, boost::TIME_UTC_);
		now_xt.sec += 300;

		m_ThreadPoolPtr->wait(now_xt);

		m_TaskDelegatePtrList.Clear();
		m_TaskPtrVec.clear();
	__LEAVE_FUNCTION
}





