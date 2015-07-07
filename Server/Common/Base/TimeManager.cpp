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
#if defined( __WINDOWS__ )
	m_StartTime = TimeUtil::TickCount();
	m_CurrentTime = TimeUtil::TickCount();
#elif defined(__LINUX__)
	m_SetTime = 0;
	m_CurrentTime = 0;
	gettimeofday(&_tstart, NULL);
#endif
	SetTime();
	return true ;

	__LEAVE_FUNCTION

		return false ;
}

uint32_t TimeManager::SysRuntime()
{
#if defined( __WINDOWS__ )
	m_CurrentTime = TimeUtil::TickCount();
#elif defined(__LINUX__)
	struct timeval _tend;
	gettimeofday(&tend, NULL);
	double t1, t2;
	t1 = (double)(_tstart.tv_sec * 1000 + (double)_tstart.tv_usec/1000);
	t2 = (double)(_tend.tv_sec * 1000 + (double)_tend.tv_usec/1000);
	m_CurrentTime = (tuint32)(t2-t1);
#endif
	return m_CurrentTime;
}

uint32_t TimeManager::Runtime()
{
	SysRuntime();
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

