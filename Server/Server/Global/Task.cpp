/************************************************************************/
/*                                                                      */
/************************************************************************/

#include "BaseLib.h"
#include "Config.h"
#include "Task.h"
//////////////////////////////////////////////////////////////////////////
LOG_IMPL(ServerTask)
//////////////////////////////////////////////////////////////////////////

Invoker::Invoker(uint32_t interval, int32_t type, int32_t initState)
: m_Interval(interval), m_State(initState), m_Type(type)
, m_InvokeTimeLeft(0), m_SchuduleTime(0),m_ExcuteTime(0),m_IdleTime(0)
{

}

Invoker::~Invoker()
{

}

void Invoker::UpdateTimeInfo( )
{
	__ENTER_FUNCTION
	m_TimeInfo.Update();
	__LEAVE_FUNCTION
}

void Invoker::UpdateInvokeTimeLeft(int32_t elapse)
{
	__ENTER_FUNCTION
	m_InvokeTimeLeft -= elapse;
	__LEAVE_FUNCTION
}

void Invoker::Invoke( )
{
	__ENTER_FUNCTION
	SetState(STATE_EXECUTE);

	m_ExcuteTime = 0;
	__ENTER_FUNCTION_EX
		Do();
	__LEAVE_FUNCTION_EX
	m_InvokeTimeLeft = m_Interval;
	SetState( ( m_Type != TYPE_ACTIVE) ? STATE_IDLE : STATE_READY);
	__LEAVE_FUNCTION
}
//////////////////////////////////////////////////////////////////////////
Task::Task( )
{
	SetState(TASK_STOP);
}

Task::~Task()
{
	m_InvokerPtrList.Clear();
}


void Task::Tick(const TimeInfo& rTimeInfo)
{
	EventMgr::Update(rTimeInfo);
	Tick_State();
}

void Task::Tick_State()
{
	__ENTER_FUNCTION
	switch (m_State)
	{
	case TASK_START:
		{
			SetState(TASK_START_EXC);
			Start();
		}
		break;
	case TASK_LOAD:
		{
			SetState(TASK_LOAD_EXC);
			Load();
		}
		break;
	case TASK_SHUTDOWN:
		{
			SetState(TASK_SHUTDOWN_EXC);
			Shutdown();
		}
		break;
	case TASK_FINALSAVE:
		{
			SetState(TASK_FINALSAVE_EXC);
			FinalSave();
		}
		break;
	default:
			break;
	}
	__LEAVE_FUNCTION
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
InvokerPtr Task::FetchInvoker()
{
	InvokerPtr taskPtr;
	__ENTER_FUNCTION
		m_InvokerPtrList.PopFront(taskPtr);
	__LEAVE_FUNCTION
		return taskPtr;
}
void Task::AddInvoker(InvokerPtr taskPtr)
{
	__ENTER_FUNCTION
		m_InvokerPtrList.PushBack(taskPtr);
	__LEAVE_FUNCTION
}

//////////////////////////////////////////////////////////////////////////

TaskManager::TaskManager()
{
	strncpy(m_Name, "unknown", TASKMANAGER_NAME_LEN);
}

TaskManager::~TaskManager()
{

}

bool TaskManager::Init(int32_t maxTask, int32_t maxThread)
{
	__ENTER_FUNCTION

		LOG_DEBUG(ServerTask, "Init TaskManager(%s)(maxTask:%d, maxThread:%d)...", 
			GetName(), maxTask, maxThread);

		Assert((maxTask >= 0 && maxThread));


		m_TaskPtrVec.resize(maxTask);

		m_ThreadPoolPtr = ThreadPoolPtr(new ThreadPool(maxThread));
		Assert(m_ThreadPoolPtr);

		LOG_DEBUG(ServerTask, "New ThreadPool(%d) ok", maxThread);

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

	LOG_DEBUG(ServerTask, "Register task:%d ok", taskPtr->GetTaskID());
	m_TaskPtrVec[taskPtr->GetTaskID()] = taskPtr;

	return true;
	__LEAVE_FUNCTION
		return false;
}

void TaskManager::Excute( )
{
	__ENTER_FUNCTION

		LOG_DEBUG(ServerTask, "Init %s ...", GetName());
		ExcuteAllTaskInit();
		LOG_DEBUG(ServerTask, "Init Ok");

		LOG_DEBUG(ServerTask, "Start %s ...", GetName());
		ExcuteAllTaskStart();
		LOG_DEBUG(ServerTask, "Start Ok");

		LOG_DEBUG(ServerTask, "Load %s...", GetName());
		ExcuteAllTaskLoad();
		LOG_DEBUG(ServerTask, "Load Ok");

		LOG_DEBUG(ServerTask, "Excute %s ...", GetName());
		ExcuteAllTask();
		LOG_DEBUG(ServerTask, "Excute Ok");

		LOG_DEBUG(ServerTask, "Shutdown %s ...", GetName());
		ExcuteAllTaskShutdown();
		LOG_DEBUG(ServerTask, "Shutdown Ok");

		LOG_DEBUG(ServerTask, "FinalSave %s ...", GetName());
		ExcuteAllTaskFinalSave();
		LOG_DEBUG(ServerTask, "FinalSave Ok");

	__LEAVE_FUNCTION
}

void TaskManager::Exit()
{
	__ENTER_FUNCTION
		LOG_DEBUG(ServerTask, "TaskManager::Exit ...");
		
		Wait(600);
		m_InvokerPtrList.Clear();
		m_TaskPtrVec.clear();

		LOG_DEBUG(ServerTask, "TaskManager::Exit Ok");
	__LEAVE_FUNCTION
}

void TaskManager::ExcuteAllTaskInit()
{
	__ENTER_FUNCTION
		for(int32_t i = 0; i < (int32_t)m_TaskPtrVec.size(); i++)
		{
			bool bRet = m_TaskPtrVec[i]->Init();
			Assert(bRet);
		}
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

void TaskManager::SetAllInvokerState_MainThread(int32_t state)
{
	__ENTER_FUNCTION
	InvokerPtr Ptr;
	m_InvokerPtrList.ResetIterator();
	while ( m_InvokerPtrList.PeekNext(Ptr))
	{
		Ptr->SetState(state);
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
	LOG_DEBUG(ServerTask, "TaskManager::Wait(%d) ...", sec);

	boost::xtime now_xt;
	xtime_get(&now_xt, boost::TIME_UTC_);
	now_xt.sec += sec;

	m_ThreadPoolPtr->wait(now_xt);
	LOG_DEBUG(ServerTask, "TaskManager::Wait(%d) Ok", sec);
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


		MySleep(1000);
	}

	Wait(300);
	__LEAVE_FUNCTION
}
void TaskManager::Tick(int32_t elapse)
{
	__ENTER_FUNCTION_EX
		Tick_Task(elapse);
		Tick_AllInvoker(elapse);
	__LEAVE_FUNCTION_EX
}
void TaskManager::Tick_Task(int32_t elapse)
{
	__ENTER_FUNCTION
	const int32_t maxTaskFetch = 64;
	for(int32_t i = 0; i < (int32_t)m_TaskPtrVec.size(); i++)
	{
		for(int32_t fetchIdx = 0; fetchIdx < maxTaskFetch; fetchIdx++)
		{
			InvokerPtr Ptr = m_TaskPtrVec[i]->FetchInvoker();
			if( Ptr )
			{
				m_InvokerPtrList.PushBack(Ptr);
			}
			else
			{
				break;
			}
		}
	}
	__LEAVE_FUNCTION
}
void TaskManager::Tick_AllInvoker(int32_t elapse)
{
	__ENTER_FUNCTION
	
	Assert(m_ThreadPoolPtr);

	InvokerPtr Ptr;
	m_InvokerPtrList.ResetIterator();
	while ( m_InvokerPtrList.PeekNext(Ptr))
	{
		switch (Ptr->GetState())
		{
		case Invoker::STATE_IDLE:
			{
				Ptr->UpdateIdleTime(elapse);
			}
			break;
		case Invoker::STATE_READY:
			{
				Ptr->ResetIdleTime() ;
				Ptr->UpdateInvokeTimeLeft(elapse);
				if( Ptr->CanExcuteNow() )
				{
					Ptr->SetState( Invoker::STATE_SCHUDULE );
					m_ThreadPoolPtr->schedule( boost::bind( &Invoker::Invoke, Ptr) );
				}
			}
			break;
		case Invoker::STATE_EXECUTE:
			Ptr->UpdateExcuteTime(elapse);
			break;
		case Invoker::STATE_SCHUDULE:
			Ptr->UpdateSchuduleTime(elapse);
			break;
		default:
			AssertSpecialEx(false, "Invoker unkonwn state.");
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

bool TaskManager::IsAllInvokerInState(int32_t state)
{
	__ENTER_FUNCTION
		InvokerPtr Ptr;
	m_InvokerPtrList.ResetIterator();
	while ( m_InvokerPtrList.PeekNext(Ptr))
	{
		if( !Ptr->IsState(state) ) return false;

	}
		return true;
	__LEAVE_FUNCTION
	return false;
}

bool TaskManager::IsShouldShutdown()
{
	__ENTER_FUNCTION_EX

		TIME64 now = TimeUtil::Now();
		CHAR shutdownFileName[32] = {0};

		int64_t nowInt64 = TimeUtil::ToInt64(now);
		tsnprintf(shutdownFileName, 32, "./shutdown.%lld", nowInt64/1000000);
		bfs::path shutdownfile = shutdownFileName;
		bsys::error_code ec;

		bool isRemoved = bfs::remove(shutdownfile, ec);
		AssertEx(!ec, ec.message().c_str());
		return isRemoved;
	
	__LEAVE_FUNCTION_EX
		return false;
}


