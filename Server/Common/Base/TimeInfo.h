#ifndef __TIME_INFO_H__
#define __TIME_INFO_H__

#include "Base.h"

class TimeInfo
{
public:
	TimeInfo();
public:
	void Update();
public:
	bool DiffYear()		const	{return m_bDiffYear;}
	bool DiffMonth()	const	{return m_bDiffMonth;}
	bool DiffDay()		const	{return m_bDiffDay;}
	bool DiffHour()		const	{return m_bDiffHour;}
	bool DiffMinute()	const	{return m_bDiffMinute;}
	bool DiffSecond()	const	{return m_bDiffSecond;}
public:
	uint32_t Elapse()	const	{return m_uTimeElapse;}
	uint32_t SysRunTime()const	{return m_uLastSysRunTime;}
private:
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


#endif // __TIME_INFO_H__