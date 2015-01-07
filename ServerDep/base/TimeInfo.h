#ifndef TIME_INFO_H
#define TIME_INFO_H

#include <PreCompier.h>

BASE_NAME_SPACES

class TimeInfo
{
public:
	TimeInfo();
public:
	void update();
public:
	bool diffYear()		const	{return m_bDiffYear;}
	bool diffMonth()	const	{return m_bDiffMonth;}
	bool diffDay()		const	{return m_bDiffDay;}
	bool diffHour()		const	{return m_bDiffHour;}
	bool diffMinute()	const	{return m_bDiffMinute;}
	bool diffSecond()	const	{return m_bDiffSecond;}
public:
	uint32_t elapse()	const	{return m_uTimeElapse;}
	uint32_t sysRunTime()const	{return m_uLastSysRunTime;}
private:
	uint32_t m_uLastTime;
	uint32_t m_uLastSysRunTime;
	uint32_t m_uTimeElapse;
	
	time_t m_nAnsiTime;

	int32_t m_nLastYear;
	int32_t m_nLastMonth;
	int32_t m_nLastDay;
	int32_t m_nLastHour;
	int32_t m_nLastMinute;
	int32_t m_nLastSecond;
	int32_t m_nLastWeek;

	bool m_bDiffYear;
	bool m_bDiffMonth;
	bool m_bDiffDay;
	bool m_bDiffHour;
	bool m_bDiffMinute;
	bool m_bDiffSecond;
	bool m_bDiffWeek;
};

BASE_NAME_SPACEE

#endif