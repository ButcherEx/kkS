/************************************************************************/
/*                                                                      */
/************************************************************************/

#include "BaseLib.h"
#include "Config.h"
#include "Task.h"
//////////////////////////////////////////////////////////////////////////
LOG_IMPL(ServerTask)
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
	__ENTER_FUNCTION
	m_TimeInfo.Update();
	__LEAVE_FUNCTION
}

void TaskDelegate::UpdateDoWaitTime(int32_t elapse)
{
	__ENTER_FUNCTION
	m_TaskExcuteDoTimeLeft -= elapse;
	__LEAVE_FUNCTION
}

void TaskDelegate::Excute( )
{
	__ENTER_FUNCTION
	SetState(STATE_EXECUTE);

	m_TaskExcuteTime = 0;
	__ENTER_FUNCTION_EX
		Do();
	__LEAVE_FUNCTION_EX
	m_TaskExcuteDoTimeLeft = m_TaskInterval;
	SetState(STATE_READY);
	__LEAVE_FUNCTION
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
	__ENTER_FUNCTION
	OnStartOk();
	__LEAVE_FUNCTION
}

void Task::OnStartOk()
{
	__ENTER_FUNCTION
	SetState(TASK_START_OK);
	__LEAVE_FUNCTION
}

void Task::Load()
{
	__ENTER_FUNCTION
	OnLoadOk();
	__LEAVE_FUNCTION
}

void Task::OnLoadOk()
{
	__ENTER_FUNCTION
	SetState(TASK_LOAD_OK);
	__LEAVE_FUNCTION
}

void Task::Shutdown()
{
	__ENTER_FUNCTION
	OnShutdownOk();
	__LEAVE_FUNCTION
}

void Task::OnShutdownOk()
{
	__ENTER_FUNCTION
	SetState(TASK_SHUTDOWN_OK);
	__LEAVE_FUNCTION
}

void Task::FinalSave()
{
	__ENTER_FUNCTION
	OnFinalSaveOk();
	__LEAVE_FUNCTION
}

void Task::OnFinalSaveOk()
{
	__ENTER_FUNCTION
	SetState(TASK_FINALSAVE_OK);
	__LEAVE_FUNCTION
}

TaskDelegatePtr Task::FetchTaskDelegate()
{
	TaskDelegatePtr taskPtr;
	__ENTER_FUNCTION
	m_TaskPtrList.PopFront(taskPtr);
	__LEAVE_FUNCTION
	return taskPtr;
}
void Task::AddTask(TaskDelegatePtr taskPtr)
{
	__ENTER_FUNCTION
	m_TaskPtrList.PushBack(taskPtr);
	__LEAVE_FUNCTION
}

//////////////////////////////////////////////////////////////////////////
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

		LOGD(ServerTask, "Init TaskManager(maxTask:%d, maxThread:%d)...", 
			maxTask, maxThread);

		Assert((maxTask && maxThread));

		m_ThreadPoolPtr = ThreadPoolPtr(new ThreadPool(maxThread));
		Assert(m_ThreadPoolPtr);
		LOGD(ServerTask, "New ThreadPool(%d) ok", maxThread);

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

	LOGD(ServerTask, "Register task:%d ok", taskPtr->GetTaskID());
	m_TaskPtrVec[taskPtr->GetTaskID()] = taskPtr;

	return true;
	__LEAVE_FUNCTION
		return false;
}

void TaskManager::Excute( )
{
	__ENTER_FUNCTION

		LOGD(ServerTask, "ExcuteAllTaskStart ...");
		ExcuteAllTaskStart();
		LOGD(ServerTask, "ExcuteAllTaskStart Ok");

		LOGD(ServerTask, "ExcuteAllTaskLoad ...");
		ExcuteAllTaskLoad();
		LOGD(ServerTask, "ExcuteAllTaskLoad Ok");

		LOGD(ServerTask, "ExcuteAllTask ...");
		ExcuteAllTask();
		LOGD(ServerTask, "ExcuteAllTask Ok");

		LOGD(ServerTask, "ExcuteAllTaskShutdown ...");
		ExcuteAllTaskShutdown();
		LOGD(ServerTask, "ExcuteAllTaskShutdown Ok");

		LOGD(ServerTask, "ExcuteAllTaskFinalSave ...");
		ExcuteAllTaskFinalSave();
		LOGD(ServerTask, "ExcuteAllTaskFinalSave Ok");

	__LEAVE_FUNCTION
}

void TaskManager::Exit()
{
	__ENTER_FUNCTION
		LOGD(ServerTask, "TaskManager::Exit ...");
		
		Wait(600);
		m_TaskDelegatePtrList.Clear();
		m_TaskPtrVec.clear();

		LOGD(ServerTask, "TaskManager::Exit Ok");
	__LEAVE_FUNCTION
}

void TaskManager::SetAllTaskState(int32_t state)
{
	__ENTER_FUNCTION
	for(int32_t i = 0; i < (int32_t)m_TaskPtrVec.size(); i++)
	{
		m_TaskPtrVec[i]->SetState(state);
	}
	__LEAVE_FUNCTION
}

void TaskManager::ExcuteState(int32_t setState, int32_t checkState)
{
	__ENTER_FUNCTION

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
	__LEAVE_FUNCTION
}
void TaskManager::ExcuteAllTaskStart()
{
	__ENTER_FUNCTION
	this->ExcuteState(Task::TASK_START, Task::TASK_START_OK);
	__LEAVE_FUNCTION
}
void TaskManager::ExcuteAllTaskLoad()
{
	__ENTER_FUNCTION
	this->ExcuteState(Task::TASK_LOAD, Task::TASK_LOAD_OK);
	__LEAVE_FUNCTION
}
void TaskManager::ExcuteAllTaskShutdown()
{
	__ENTER_FUNCTION
	this->ExcuteState(Task::TASK_SHUTDOWN, Task::TASK_SHUTDOWN_OK);
	__LEAVE_FUNCTION
}
void TaskManager::ExcuteAllTaskFinalSave()
{
	__ENTER_FUNCTION
	this->ExcuteState(Task::TASK_FINALSAVE, Task::TASK_FINALSAVE_OK);
	__LEAVE_FUNCTION
}
void TaskManager::Wait(int32_t sec)
{
	__ENTER_FUNCTION
	LOGD(ServerTask, "TaskManager::Wait(%d) ...", sec);

	boost::xtime now_xt;
	xtime_get(&now_xt, boost::TIME_UTC_);
	now_xt.sec += sec;

	m_ThreadPoolPtr->wait(now_xt);
	LOGD(ServerTask, "TaskManager::Wait(%d) Ok", sec);
	__LEAVE_FUNCTION
}
void TaskManager::ExcuteAllTask()
{
	__ENTER_FUNCTION
	SetAllTaskState(Task::TASK_EXECUTE);

	int64_t checkShutdown = 0;
	while (true)
	{
		m_TimeInfo.Update();
		int32_t elapseMs = m_TimeInfo.Elapse();

		Tick(elapseMs);

		checkShutdown += elapseMs;
		if( checkShutdown >= 30*1000)
		{
			checkShutdown = 0;
			if(IsShouldShutdown())
			{
				break;
			}
		}


		MySleep(10);
	}

	Wait(300);
	__LEAVE_FUNCTION
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
void TaskManager::Tick_Logic(int32_t elapse)
{
	__ENTER_FUNCTION
	__LEAVE_FUNCTION
}

bool TaskManager::IsAllTaskInState(int32_t state)
{
	__ENTER_FUNCTION
	for(int32_t i = 0; i < (int32_t)m_TaskPtrVec.size(); i++)
	{
		Assert(m_TaskPtrVec[i]);
		if (m_TaskPtrVec[i]->GetState() != state) return false;
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}
bool TaskManager::IsShouldShutdown()
{
	__ENTER_FUNCTION

		TIME64 now = TimeUtil::Now();
		CHAR shutdownFileName[32] = {0};

		int64_t nowInt64 = TimeUtil::ToInt64(now);
		tsnprintf(shutdownFileName, 32, "./shutdown.%lld", nowInt64/1000000);
		bfs::path shutdownfile = shutdownFileName;
		bsys::error_code ec;

		bool isRemoved = bfs::remove(shutdownfile, ec);
		AssertEx(!ec, ec.message().c_str());
		return isRemoved;
	
	__LEAVE_FUNCTION
		return false;
}


