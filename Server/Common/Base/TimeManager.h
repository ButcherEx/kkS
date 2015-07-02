
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
	uint32_t		CurrentTime();
	uint32_t		RunTime();
	uint32_t		StartTime();
	void			SetTime();
private:
	bool			Init( ) ;
private:
	uint32_t		m_StartTime;
	uint32_t		m_CurrentTime;
	time_t			m_SetTime;
	tm				m_TM;
#ifdef __LINUX__
	struct timeval _tstart;
	struct timezone _tz;
#endif
};

extern TimeManager g_TimeManager;




#endif
