/************************************************************************/
/*                                                                      */
/************************************************************************/

#include "MapCreator.h"

bool MapCreator::Init()
{
	__ENTER_FUNCTION
	
	m_MapTaskManager.Init(0, 10);
	return true;

	__LEAVE_FUNCTION
	return false;
}


void MapCreator::Tick(const TimeInfo& rTimeInfo)
{
	__ENTER_FUNCTION
	
	Task::Tick(rTimeInfo);
	m_MapTaskManager.Tick(rTimeInfo.Elapse());

	__LEAVE_FUNCTION
}

void MapCreator::Start()
{
	__ENTER_FUNCTION
		m_MapTaskManager.ExcuteAllTaskStart();
		Task::OnStartOk();
	__LEAVE_FUNCTION
}

void MapCreator::Load()
{
	__ENTER_FUNCTION
		m_MapTaskManager.ExcuteAllTaskLoad();
		Task::OnLoadOk();
	__LEAVE_FUNCTION
}

void MapCreator::Shutdown()
{
	__ENTER_FUNCTION
		m_MapTaskManager.ExcuteAllTaskShutdown();
		Task::OnShutdownOk();
	__LEAVE_FUNCTION
}

void MapCreator::FinalSave()
{
	__ENTER_FUNCTION
		m_MapTaskManager.ExcuteAllTaskFinalSave();
		Task::OnFinalSaveOk( );
	__LEAVE_FUNCTION
}


void MapCreator::Exit()
{
	__ENTER_FUNCTION
	
	m_MapTaskManager.Exit();

	__LEAVE_FUNCTION
}


