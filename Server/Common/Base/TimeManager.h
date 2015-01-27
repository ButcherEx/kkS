
#ifndef __TIMEMANAGER_H__
#define __TIMEMANAGER_H__

#include "Base.h"
#include "Timer.h"

class TimeManager
{
public :
	TimeManager( ) ;
	~TimeManager( ) ;
public:
	uint32_t		SysTick();
	uint32_t		RunTick();
	TIME64			StartTime();
	int64_t			StartTimeToInt64();
	TIME64			CurrentTime();
	int64_t			CurrentTimeToInt64();
	uint32_t		StartTick();
	uint32_t		CurrentTick();
private:
	bool			Init( ) ;
private:
	MyLock			m_Lock;
	uint32_t		m_StartTickCount;
	uint32_t		m_CurrentTickCount;
	TIME64			m_StartTime;
	TIME64			m_CurrentTime;
};

extern TimeManager g_TimeManager;




#endif
