/************************************************************************/
/*                                                                      */
/************************************************************************/

#include "BaseLib.h"
#include "Config.h"
#include "Task.h"

//////////////////////////////////////////////////////////////////////////

TaskDelegate::TaskDelegate(uint32_t interval )
: m_TaskInterval(interval), m_State(STATE_READY)
, m_TaskExcuteDoTimeLeft(0), m_TaskSchuduleTime(0),m_TaskExcuteTime(0)
{

}

TaskDelegate::~TaskDelegate()
{

}

void TaskDelegate::UpdateTimeInfo( )
{
	m_TimeInfo.Update();
}

void TaskDelegate::UpdateDoWaitTime(int32_t elapse)
{
	m_TaskExcuteDoTimeLeft -= elapse;
}

void TaskDelegate::Excute( )
{
	SetState(STATE_EXECUTE);

	m_TaskExcuteTime = 0;
	__ENTER_FUNCTION_EX
		Do();
	__LEAVE_FUNCTION_EX
	m_TaskExcuteDoTimeLeft = m_TaskInterval;
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

void Task::Start()
{
	OnStartOk();
}

void Task::OnStartOk()
{
	SetState(TASK_START_OK);
}

void Task::Load()
{
	OnLoadOk();
}

void Task::OnLoadOk()
{
	SetState(TASK_LOAD_OK);
}

void Task::Shutdown()
{
	OnShutdownOk();
}

void Task::OnShutdownOk()
{
	SetState(TASK_SHUTDOWN_OK);
}

void Task::Save()
{
	OnSaveOk();
}

void Task::OnSaveOk()
{
	SetState(TASK_SAVE_OK);
}

TaskDelegatePtr Task::FetchTaskDelegate()
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
		ExcuteAllTaskStart();
		ExcuteAllTaskLoad();
		ExcuteAllTask();
		ExcuteAllTaskShutdown();
		ExcuteAllTaskSave();
	__LEAVE_FUNCTION
}

void TaskManager::Exit()
{
	__ENTER_FUNCTION
		Wait(600);
		m_TaskDelegatePtrList.Clear();
		m_TaskPtrVec.clear();
	__LEAVE_FUNCTION
}

void TaskManager::SetAllTaskState(int32_t state)
{
	for(int32_t i = 0; i < (int32_t)m_TaskPtrVec.size(); i++)
	{
		m_TaskPtrVec[i]->SetState(state);
	}
}

void TaskManager::ExcuteState(int32_t setState, int32_t checkState)
{
	SetAllTaskState(setState);

	while (true)
	{
		m_TimeInfo.Update();
		int32_t ElapseMilli = m_TimeInfo.Elapse();

		Tick(ElapseMilli);

		if(IsAllTaskInState(checkState))
		{
			break;
		}

		MySleep(100);
	}
}
void TaskManager::ExcuteAllTaskStart()
{
	this->ExcuteState(Task::TASK_START, Task::TASK_START_OK);
}
void TaskManager::ExcuteAllTaskLoad()
{
	this->ExcuteState(Task::TASK_LOAD, Task::TASK_LOAD_OK);
}
void TaskManager::ExcuteAllTaskShutdown()
{
	this->ExcuteState(Task::TASK_SHUTDOWN, Task::TASK_SHUTDOWN_OK);
}
void TaskManager::ExcuteAllTaskSave()
{
	this->ExcuteState(Task::TASK_SAVE, Task::TASK_SAVE_OK);
}
void TaskManager::Wait(int32_t sec)
{
	boost::xtime now_xt;
	xtime_get(&now_xt, boost::TIME_UTC_);
	now_xt.sec += sec;

	m_ThreadPoolPtr->wait(now_xt);
}
void TaskManager::ExcuteAllTask()
{
	SetAllTaskState(Task::TASK_EXECUTE);

	while (true)
	{
		m_TimeInfo.Update();
		int32_t ElapseMilli = m_TimeInfo.Elapse();

		Tick(ElapseMilli);

		if(IsShutdown())
		{
			break;
		}
	}

	Wait(300);
}
void TaskManager::Tick(int32_t elapse)
{
	__ENTER_FUNCTION_EX
		Tick_Task(elapse);
		Tick_TaskDelegate(elapse);
	__LEAVE_FUNCTION_EX
}
void TaskManager::Tick_Task(int32_t elapse)
{
	__ENTER_FUNCTION
		const int32_t maxTaskFetch = 256;
	int32_t fetchCounter = 0;
	for(int32_t i = 0; i < (int32_t)m_TaskPtrVec.size(); i++)
	{
		if( fetchCounter > maxTaskFetch) break;

		TaskDelegatePtr Ptr = m_TaskPtrVec[i]->FetchTaskDelegate();
		if( Ptr )
		{
			fetchCounter++;
			m_TaskDelegatePtrList.PushBack(Ptr);
		}
	}
	__LEAVE_FUNCTION
}

void TaskManager::Tick_TaskDelegate(int32_t elapse)
{
	__ENTER_FUNCTION
	
	Assert(m_ThreadPoolPtr);

	TaskDelegatePtr Ptr;
	m_TaskDelegatePtrList.ResetIterator();
	while ( m_TaskDelegatePtrList.PeekNext(Ptr))
	{
		switch (Ptr->GetState())
		{
		case TaskDelegate::STATE_READY:
			{
				Ptr->UpdateDoWaitTime(elapse);
				if( Ptr->CanExcuteNow() )
				{
					Ptr->SetState( TaskDelegate::STATE_SCHUDULE );
					m_ThreadPoolPtr->schedule( boost::bind( &TaskDelegate::Excute, Ptr) );
				}
			}
			break;
		case TaskDelegate::STATE_EXECUTE:
			Ptr->UpdateExcuteTime(elapse);
			break;
		case TaskDelegate::STATE_SCHUDULE:
			Ptr->UpdateSchuduleTime(elapse);
			break;
		default:
			AssertSpecialEx(false, "TaskDelegate unkonwn state.");
			break;
		}
	}

	__LEAVE_FUNCTION
}


