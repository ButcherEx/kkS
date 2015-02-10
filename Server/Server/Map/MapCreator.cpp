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
void MapCreator::Excute()
{
	__ENTER_FUNCTION
		m_MapTaskManager.Excute( );
	__LEAVE_FUNCTION
}
void MapCreator::Exit()
{
	__ENTER_FUNCTION
		m_MapTaskManager.Exit();
	__LEAVE_FUNCTION
}
int32_t MapCreator::GetTaskID()
{
		return 1;
}

