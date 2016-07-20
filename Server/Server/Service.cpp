#include "BaseLib.h"
#include "Config.h"
#include "Service.h"
#include "CpuMemStat.h"


//////////////////////////////////////////////////////////////////////////
LOG_IMPL(ServiceMgrLog)
//////////////////////////////////////////////////////////////////////////
Invoker::Invoker(uint32 interval, uint32 lifeTime, int32 type, int32 initState)
: m_Interval(interval), m_LifeTime(lifeTime), m_LifeTimeLeft(lifeTime), m_State(initState), m_Type(type)
, m_InvokeTimeLeft(0), m_SchuduleTime(0), m_ExcuteTime(0), m_IdleTime(0)
{

}

Invoker::~Invoker()
{

}

void Invoker::UpdateTimeInfo()
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

void Invoker::Invoke()
{
	__ENTER_FUNCTION
		SetState(InvokerStatus::EXECUTE);

	m_ExcuteTime = 0;
	__ENTER_FUNCTION_EX
		Do();
	__LEAVE_FUNCTION_EX
		m_InvokeTimeLeft = m_Interval;
	m_LifeTimeLeft -= m_Interval;
	if (m_LifeTimeLeft <= 0) {
		SetState(InvokerStatus::STOP);
	} else {
		SetState((m_Type != InvokerType::ACTIVE) ? InvokerStatus::IDLE : InvokerStatus::READY);
	}

	__LEAVE_FUNCTION
}
//////////////////////////////////////////////////////////////////////////

Service::Service()
{
	SetState(ServiceStatus::STOP);
}

Service::~Service()
{
	m_InvokerPtrList.Clear();
}


void Service::Tick()
{
	TickState();
}

void Service::TickState()
{
	__ENTER_FUNCTION
		switch (m_State)
	{
		case ServiceStatus::START:
		{
			SetState(ServiceStatus::START_EXC);
			Start();
		}
		break;
		case ServiceStatus::LOAD:
		{
			SetState(ServiceStatus::LOAD_EXC);
			Load();
		}
		break;
		case ServiceStatus::SHUTDOWN:
		{
			SetState(ServiceStatus::SHUTDOWN_EXC);
			Shutdown();
		}
		break;
		case ServiceStatus::FINALSAVE:
		{
			SetState(ServiceStatus::FINALSAVE_EXC);
			FinalSave();
		}
		break;
		default:
			break;
	}
	__LEAVE_FUNCTION
}

void Service::Start()
{
	__ENTER_FUNCTION
		OnStartOk();
	__LEAVE_FUNCTION
}

void Service::OnStartOk()
{
	__ENTER_FUNCTION
		SetState(ServiceStatus::START_OK);
	__LEAVE_FUNCTION
}

void Service::Load()
{
	__ENTER_FUNCTION
		OnLoadOk();
	__LEAVE_FUNCTION
}

void Service::OnLoadOk()
{
	__ENTER_FUNCTION
		SetState(ServiceStatus::LOAD_OK);
	__LEAVE_FUNCTION
}

void Service::Shutdown()
{
	__ENTER_FUNCTION
		OnShutdownOk();
	__LEAVE_FUNCTION
}

void Service::OnShutdownOk()
{
	__ENTER_FUNCTION
		SetState(ServiceStatus::SHUTDOWN_OK);
	__LEAVE_FUNCTION
}

void Service::FinalSave()
{
	__ENTER_FUNCTION
		OnFinalSaveOk();
	__LEAVE_FUNCTION
}

void Service::OnFinalSaveOk()
{
	__ENTER_FUNCTION
		SetState(ServiceStatus::FINALSAVE_OK);
	__LEAVE_FUNCTION
}

void Service::Stop()
{
	__ENTER_FUNCTION

		__LEAVE_FUNCTION
}

InvokerPtr Service::FetchInvoker()
{
	InvokerPtr taskPtr;
	__ENTER_FUNCTION
		m_InvokerPtrList.PopFront(taskPtr);
	__LEAVE_FUNCTION
		return taskPtr;
}
void Service::AddInvoker(InvokerPtr taskPtr)
{
	__ENTER_FUNCTION
		m_InvokerPtrList.PushBack(taskPtr);
	__LEAVE_FUNCTION
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

ServiceMgr::ServiceMgr()
{

}

ServiceMgr::~ServiceMgr()
{

}

bool ServiceMgr::Init(int32_t maxTask, int32_t maxThread)
{
	__ENTER_FUNCTION

		LOG_DEBUG(ServiceMgrLog, "Init TaskManager (maxTask:%d, maxThread:%d)...",
		maxTask, maxThread);

	Assert((maxTask >= 0 && maxThread));


	m_ServicePtrVec.resize(maxTask);

	m_ThreadPoolPtr = ThreadPoolPtr(new ThreadPool(maxThread));
	Assert(m_ThreadPoolPtr);

	LOG_DEBUG(ServiceMgrLog, "New ThreadPool(%d) ok", maxThread);

	return true;
	__LEAVE_FUNCTION

		return false;
}

bool ServiceMgr::Register(ServicePtr taskPtr)
{
	__ENTER_FUNCTION
		Assert(taskPtr);
	Assert((taskPtr->GetServiceID() >= 0));
	Assert((taskPtr->GetServiceID() < (int32_t)m_ServicePtrVec.size()));
	Assert(!m_ServicePtrVec[taskPtr->GetServiceID()]);

	LOG_DEBUG(ServiceMgrLog, "Register task:%d ok", taskPtr->GetServiceID());
	m_ServicePtrVec[taskPtr->GetServiceID()] = taskPtr;

	return true;
	__LEAVE_FUNCTION
		return false;
}

void ServiceMgr::Excute()
{
	__ENTER_FUNCTION

		LOG_DEBUG(ServiceMgrLog, "Init ...");
	ExcuteAllServiceInit();
	LOG_DEBUG(ServiceMgrLog, "Init Ok");

	LOG_DEBUG(ServiceMgrLog, "Start  ...");
	ExcuteAllServiceStart();
	LOG_DEBUG(ServiceMgrLog, "Start Ok");

	LOG_DEBUG(ServiceMgrLog, "Load ...");
	ExcuteAllServiceLoad();
	LOG_DEBUG(ServiceMgrLog, "Load Ok");

	LOG_DEBUG(ServiceMgrLog, "Excute ...");
	ExcuteAllService();
	LOG_DEBUG(ServiceMgrLog, "Excute Ok");

	LOG_DEBUG(ServiceMgrLog, "Shutdown ...");
	ExcuteAllServiceShutdown();
	LOG_DEBUG(ServiceMgrLog, "Shutdown Ok");

	LOG_DEBUG(ServiceMgrLog, "FinalSave ...");
	ExcuteAllServiceFinalSave();
	LOG_DEBUG(ServiceMgrLog, "FinalSave Ok");

	__LEAVE_FUNCTION
}

void ServiceMgr::Exit()
{
	__ENTER_FUNCTION
		LOG_DEBUG(ServiceMgrLog, "TaskManager::Exit ...");

	Wait(600);
	m_InvokerPtrList.Clear();
	m_ServicePtrVec.clear();

	LOG_DEBUG(ServiceMgrLog, "TaskManager::Exit Ok");
	__LEAVE_FUNCTION
}

void ServiceMgr::ExcuteAllServiceInit()
{
	__ENTER_FUNCTION
		for (int32_t i = 0; i < (int32_t)m_ServicePtrVec.size(); i++)
		{
			bool bRet = m_ServicePtrVec[i]->Init();
			Assert(bRet);
		}
	__LEAVE_FUNCTION
}
void ServiceMgr::SetAllServiceState(int32_t state)
{
	__ENTER_FUNCTION
		for (int32_t i = 0; i < (int32_t)m_ServicePtrVec.size(); i++)
		{
			m_ServicePtrVec[i]->SetState(state);
		}
	__LEAVE_FUNCTION
}

void ServiceMgr::SetAllInvokerState_MainThread(int32_t state)
{
	__ENTER_FUNCTION
		InvokerPtr Ptr;
	m_InvokerPtrList.ResetIterator();
	while (m_InvokerPtrList.PeekNext(Ptr))
	{
		Ptr->SetState(state);
	}
	__LEAVE_FUNCTION
}

void ServiceMgr::ExcuteState(int32_t setState, int32_t checkState)
{
	__ENTER_FUNCTION

		SetAllServiceState(setState);

	while (true)
	{
		m_TimeInfo.Update();
		int32_t ElapseMilli = m_TimeInfo.Elapse();

		Tick(ElapseMilli);

		if (IsAllTaskInState(checkState))
		{
			break;
		}

		MySleep(10);
	}
	__LEAVE_FUNCTION
}
void ServiceMgr::ExcuteAllServiceStart()
{
	__ENTER_FUNCTION
		this->ExcuteState(ServiceStatus::START, ServiceStatus::START_OK);
	__LEAVE_FUNCTION
}
void ServiceMgr::ExcuteAllServiceLoad()
{
	__ENTER_FUNCTION
		this->ExcuteState(ServiceStatus::LOAD, ServiceStatus::LOAD_OK);
	__LEAVE_FUNCTION
}
void ServiceMgr::ExcuteAllServiceShutdown()
{
	__ENTER_FUNCTION
		this->ExcuteState(ServiceStatus::SHUTDOWN, ServiceStatus::SHUTDOWN_OK);
	__LEAVE_FUNCTION
}
void ServiceMgr::ExcuteAllServiceFinalSave()
{
	__ENTER_FUNCTION
		this->ExcuteState(ServiceStatus::FINALSAVE, ServiceStatus::FINALSAVE_OK);
	__LEAVE_FUNCTION
}
void ServiceMgr::Wait(int32_t sec)
{
	__ENTER_FUNCTION
		LOG_DEBUG(ServiceMgrLog, "TaskManager::Wait(%d) ...", sec);

	boost::xtime now_xt;
	xtime_get(&now_xt, boost::TIME_UTC_);
	now_xt.sec += sec;

	m_ThreadPoolPtr->wait(now_xt);
	LOG_DEBUG(ServiceMgrLog, "TaskManager::Wait(%d) Ok", sec);
	__LEAVE_FUNCTION
}
void ServiceMgr::ExcuteAllService()
{
	__ENTER_FUNCTION
		SetAllServiceState(ServiceStatus::EXECUTE);

	int64_t checkShutdown = 0;
	while (true)
	{
		m_TimeInfo.Update();
		int32_t elapseMs = m_TimeInfo.Elapse();

		Tick(elapseMs);

		checkShutdown += elapseMs;
		if (checkShutdown >= 30 * 1000)
		{
			LogCpuMemStat("MZ");
			checkShutdown = 0;
			if (IsShouldShutdown())
			{
				break;
			}
		}



		MySleep(10);
	}

	Wait(300);
	__LEAVE_FUNCTION
}
void ServiceMgr::Tick(int32_t elapse)
{
	__ENTER_FUNCTION_EX
		Tick_Service(elapse);
	Tick_AllInvoker(elapse);
	Tick_FreeInvoker();
	__LEAVE_FUNCTION_EX
}
void ServiceMgr::Tick_Service(int32_t elapse)
{
	__ENTER_FUNCTION
		const int32_t maxTaskFetch = 64;
	for (int32_t i = 0; i < (int32_t)m_ServicePtrVec.size(); i++)
	{
		for (int32_t fetchIdx = 0; fetchIdx < maxTaskFetch; fetchIdx++)
		{
			InvokerPtr Ptr = m_ServicePtrVec[i]->FetchInvoker();
			if (Ptr)
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
void ServiceMgr::Tick_AllInvoker(int32_t elapse)
{
	__ENTER_FUNCTION

		Assert(m_ThreadPoolPtr);

	InvokerPtr Ptr;
	m_InvokerPtrList.ResetIterator();

	while (m_InvokerPtrList.PeekNext(Ptr))
	{
		switch (Ptr->GetState())
		{
		case InvokerStatus::IDLE:
		{
			Ptr->UpdateIdleTime(elapse);
		}
		break;
		case InvokerStatus::READY:
		{
			Ptr->ResetIdleTime();
			Ptr->UpdateInvokeTimeLeft(elapse);
			if (Ptr->CanExcuteNow())
			{
				Ptr->SetState(InvokerStatus::SCHUDULE);
				m_ThreadPoolPtr->schedule(boost::bind(&Invoker::Invoke, Ptr));
			}
		}
		break;
		case InvokerStatus::EXECUTE:
			Ptr->UpdateExcuteTime(elapse);
			break;
		case InvokerStatus::SCHUDULE:
			Ptr->UpdateSchuduleTime(elapse);
			break;
		case InvokerStatus::STOP:
			Ptr->Stop();
			break;
		default:
			AssertSpecialEx(false, "Invoker unkonwn state.");
			break;
		}
	}

	__LEAVE_FUNCTION
}
void ServiceMgr::Tick_FreeInvoker()
{
	__ENTER_FUNCTION
		TList<InvokerPtr>::TListIterator iter = m_InvokerPtrList.Begin();
	for (; iter != m_InvokerPtrList.End();){
		if ((*iter)->GetState() == InvokerStatus::STOP){
			iter = m_InvokerPtrList.Erase(iter);
		} else {
			++iter;
		}
	}

	__LEAVE_FUNCTION
}
void ServiceMgr::Tick_Logic(int32_t elapse)
{
	__ENTER_FUNCTION
		__LEAVE_FUNCTION
}

bool ServiceMgr::IsAllTaskInState(int32_t state)
{
	__ENTER_FUNCTION
		for (int32_t i = 0; i < (int32_t)m_ServicePtrVec.size(); i++)
		{
			Assert(m_ServicePtrVec[i]);
			if (m_ServicePtrVec[i]->GetState() != state) return false;
		}
	return true;
	__LEAVE_FUNCTION
		return false;
}

bool ServiceMgr::IsAllInvokerInState(int32_t state)
{
	__ENTER_FUNCTION
		InvokerPtr Ptr;
	m_InvokerPtrList.ResetIterator();
	while (m_InvokerPtrList.PeekNext(Ptr))
	{
		if (!Ptr->IsState(state)) return false;

	}
	return true;
	__LEAVE_FUNCTION
		return false;
}

bool ServiceMgr::IsShouldShutdown()
{
	__ENTER_FUNCTION_EX

		TIME64 now = TimeUtil::Now();
	CHAR shutdownFileName[32] = { 0 };

	int64_t nowInt64 = TimeUtil::ToInt64(now);
	tsnprintf(shutdownFileName, 32, "./shutdown.%lld", nowInt64 / 1000000);
	/*bfs::path shutdownfile = shutdownFileName;
	bsys::error_code ec;

	bool isRemoved = bfs::remove(shutdownfile, ec);
	AssertEx(!ec, ec.message().c_str());*/
	return false;

	__LEAVE_FUNCTION_EX
		return false;
}