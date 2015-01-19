
#include "TimeInfo.h"
#include "Timer.h"


TimeInfo::TimeInfo()
{
	TIME64 now = TimeUtil::Now();
	m_nLastYear = now.year + 1900;
	m_nLastMonth = now.mon;
	m_nLastDay = now.day;
	m_nLastHour = now.hour;
	m_nLastMinute = now.min;
	m_nLastSecond = now.sec;
	m_nLastWeek = now.weekday;

	m_uLastSysRunTime = 0;
	m_nAnsiTime = time(NULL);
	m_uLastTime = TimeUtil::TickCount();

	Update();
}


void TimeInfo::Update()
{
	_MY_TRY
	{
		uint32_t nowMs = TimeUtil::TickCount();
		m_nAnsiTime = time(NULL);

		TIME64 now = TimeUtil::Now();
		now.year += 1900;

		m_uTimeElapse = nowMs - m_uLastTime;
		m_uLastSysRunTime += m_uTimeElapse;

		m_bDiffYear = (m_nLastYear != now.year);
		m_bDiffMonth = (m_bDiffYear || m_nLastMonth != now.mon);
		m_bDiffDay = (m_bDiffMonth || m_nLastDay != now.day);
		m_bDiffHour = (m_bDiffDay || m_nLastHour != now.hour);
		m_bDiffMinute = (m_bDiffHour || m_nLastMinute != now.min);
		m_bDiffSecond = (m_bDiffMinute || m_nLastSecond != now.sec);
		m_bDiffWeek = (m_nLastWeek == 0 && now.weekday == 1);

		m_uLastTime = nowMs;
		m_nLastYear = now.year;
		m_nLastMonth = now.mon;
		m_nLastDay = now.day;
		m_nLastHour = now.hour;
		m_nLastMinute = now.min;
		m_nLastSecond = now.sec;
		m_nLastWeek = now.weekday;
	}
	_MY_CATCH
	{

	}
	
}

