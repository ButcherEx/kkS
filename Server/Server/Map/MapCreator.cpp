/************************************************************************/
/*                                                                      */
/************************************************************************/
#include "LogDefine.h"
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
	
	Service::Tick(rTimeInfo);
	Assert(m_MapTaskManager.IsAllInvokerInState(InvokerStatus::IDLE));
	m_MapTaskManager.SetAllInvokerState_MainThread(InvokerStatus::READY);
	m_MapTaskManager.Tick(rTimeInfo.Elapse());
	while (!m_MapTaskManager.IsAllInvokerInState(InvokerStatus::IDLE));

	__LEAVE_FUNCTION
}

void MapCreator::Start()
{
	__ENTER_FUNCTION
	
	LOG_DEBUG(ServerService, "Start %s ...", m_MapTaskManager.GetName());
	
	m_MapTaskManager.ExcuteAllServiceStart();
	Service::OnStartOk();

	LOG_DEBUG(ServerService, "Start Ok");

	__LEAVE_FUNCTION
}

void MapCreator::Load()
{
	__ENTER_FUNCTION
		LOG_DEBUG(ServerService, "Load %s ...", m_MapTaskManager.GetName());
		m_MapTaskManager.ExcuteAllServiceLoad();
		Service::OnLoadOk();
		LOG_DEBUG(ServerService, "Load Ok");
	__LEAVE_FUNCTION
}

void MapCreator::Shutdown()
{
	__ENTER_FUNCTION
		LOG_DEBUG(ServerService, "Shutdown %s ...", m_MapTaskManager.GetName());
		m_MapTaskManager.ExcuteAllServiceShutdown();
		Service::OnShutdownOk();
		LOG_DEBUG(ServerService, "Shutdown Ok");
	__LEAVE_FUNCTION
}

void MapCreator::FinalSave()
{
	__ENTER_FUNCTION
		LOG_DEBUG(ServerService, "FinalSave %s ...", m_MapTaskManager.GetName());
		m_MapTaskManager.ExcuteAllServiceFinalSave();
		Service::OnFinalSaveOk( );
		LOG_DEBUG(ServerService, "FinalSave Ok");
	__LEAVE_FUNCTION
}


void MapCreator::Exit()
{
	__ENTER_FUNCTION
	
	m_MapTaskManager.Exit();

	__LEAVE_FUNCTION
}


