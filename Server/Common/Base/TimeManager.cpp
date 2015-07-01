#include "TimeManager.h"
#include "Assertx.h"

TimeManager g_TimeManager;

TimeManager::TimeManager( )
{
__ENTER_FUNCTION

	Init();

__LEAVE_FUNCTION
}

TimeManager::~TimeManager( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

bool TimeManager::Init( )
{
	__ENTER_FUNCTION
	m_StartTime = TimeUtil::TickCount();
	m_CurrentTime = TimeUtil::TickCount();

	return true ;

	__LEAVE_FUNCTION

		return false ;
}

uint32_t TimeManager::CurrentTime()
{
	m_CurrentTime = TimeUtil::TickCount();

	return m_CurrentTime;
}

uint32_t TimeManager::RunTime()
{
	CurrentTime();
	if( m_CurrentTime < m_StartTime )
	{
		return ((uint32_t)0xffffffff-m_StartTime) + m_CurrentTime;
	}
	return m_CurrentTime - m_StartTime;
}

uint32_t TimeManager::StartTime()
{
	return m_StartTime;
}

void TimeManager::SetTime()
{
	time_t nowTime = time(NULL);
	tm curTm;
	localtime_r(&curTm, &nowTime);

	m_SetTime = nowTime;
	memcpy(&m_TM, &curTm, sizeof(tm));
}

