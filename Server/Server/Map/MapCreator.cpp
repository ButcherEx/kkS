/************************************************************************/
/*                                                                      */
/************************************************************************/

#include "MapCreator.h"
#include "Config.h"

bool MapCreator::Init()
{
	__ENTER_FUNCTION

	//////////////////////////////////////////////////////////////////////////
	m_MapTaskManager.SetName("MapCreator");
	//////////////////////////////////////////////////////////////////////////
	bool bRet = m_MapTaskManager.Init(0, 10);
	Assert(bRet);
	//////////////////////////////////////////////////////////////////////////

	InvokerPtr Ptr ( new MapCreatorInvoker(*this) );
	AddInvoker(Ptr);

	//////////////////////////////////////////////////////////////////////////
	return true;

	__LEAVE_FUNCTION
	return false;
}


void MapCreator::InitAllMapMgr()
{
	__ENTER_FUNCTION

	MapMgrPtr mmPtr(new MapMgr);
	Assert(mmPtr);

	m_MapMgrVec.push_back(mmPtr);

	__LEAVE_FUNCTION
}

void MapCreator::Tick(const TimeInfo& rTimeInfo)
{
	__ENTER_FUNCTION
	
	Task::Tick(rTimeInfo);
	Assert(m_MapTaskManager.IsAllInvokerInState(Invoker::STATE_IDLE));
	m_MapTaskManager.SetAllInvokerState_MainThread(Invoker::STATE_READY);
	m_MapTaskManager.Tick(rTimeInfo.Elapse());
	while (!m_MapTaskManager.IsAllInvokerInState(Invoker::STATE_IDLE));

	__LEAVE_FUNCTION
}

void MapCreator::Start()
{
	__ENTER_FUNCTION
	
	LOG_DEBUG(ServerTask, "Start %s ...", m_MapTaskManager.GetName());
	
	m_MapTaskManager.ExcuteAllTaskStart();
	Task::OnStartOk();

	LOG_DEBUG(ServerTask, "Start Ok");

	__LEAVE_FUNCTION
}

void MapCreator::Load()
{
	__ENTER_FUNCTION
		LOG_DEBUG(ServerTask, "Load %s ...", m_MapTaskManager.GetName());
		m_MapTaskManager.ExcuteAllTaskLoad();
		Task::OnLoadOk();
		LOG_DEBUG(ServerTask, "Load Ok");
	__LEAVE_FUNCTION
}

void MapCreator::Shutdown()
{
	__ENTER_FUNCTION
		LOG_DEBUG(ServerTask, "Shutdown %s ...", m_MapTaskManager.GetName());
		m_MapTaskManager.ExcuteAllTaskShutdown();
		Task::OnShutdownOk();
		LOG_DEBUG(ServerTask, "Shutdown Ok");
	__LEAVE_FUNCTION
}

void MapCreator::FinalSave()
{
	__ENTER_FUNCTION
		LOG_DEBUG(ServerTask, "FinalSave %s ...", m_MapTaskManager.GetName());
		m_MapTaskManager.ExcuteAllTaskFinalSave();
		Task::OnFinalSaveOk( );
		LOG_DEBUG(ServerTask, "FinalSave Ok");
	__LEAVE_FUNCTION
}


void MapCreator::Exit()
{
	__ENTER_FUNCTION
	
	m_MapTaskManager.Exit();

	__LEAVE_FUNCTION
}


