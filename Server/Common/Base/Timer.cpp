/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#include "Timer.h"
#if defined(__WINDOWS__)
#include <sys/timeb.h>
#endif


int64_t TimeUtil::UtcMilliseconds( )
{
	/* *
	*  miliseconds for January 1, 1970 (UTC)
	*/
	int32_t seconds;
	int32_t miliseconds;
	int64_t allmiliseconds;

	UtcTime(&seconds, &miliseconds);
	allmiliseconds = seconds * 1000 + miliseconds;
	return allmiliseconds;
}

void TimeUtil::UtcTime(int32_t *seconds, int32_t *milliseconds)
{
#if defined(__WINDOWS__)
	/* XXXX
	* _ftime is not the greatest int32erface here; GetSystemTimeAsFileTime
	* would give us better resolution, whereas something cobbled together
	* with GetTickCount could maybe give us monotonic behavior.
	*
	* Either way, I think this value might be skewed to ignore the
	* timezone, and just return local time.  That's not so good.
	*/
	struct _timeb tb;
	_ftime(&tb);
	*seconds = (int32_t) tb.time;
	*milliseconds = (int32_t) tb.millitm;
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	*seconds = tv.tv_sec;
	*milliseconds = tv.tv_usec/1000;
#endif
}

void TimeUtil::AddMillisecondsToNow(int32_t milliseconds, int32_t *sec, int32_t *ms)
{
	int32_t cur_sec, cur_ms, when_sec, when_ms;

	UtcTime(&cur_sec, &cur_ms);
	when_sec = cur_sec + (int32_t)(milliseconds/1000);
	when_ms = cur_ms + milliseconds%1000;
	if (when_ms >= 1000)
	{
		when_sec ++;
		when_ms -= 1000;
	}
	*sec = when_sec;
	*ms = when_ms;
}

uint32_t TimeUtil::TickCount( )
{
#if defined(__WINDOWS__)
	return (uint32_t)GetTickCount( );
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (uint32_t)(tv.tv_sec * 1000 + (int32_t)tv.tv_usec/1000);
#endif
}


 TIME64 TimeUtil::Create(uint8_t sec, uint8_t mint, uint8_t hour, uint8_t day, uint8_t month, uint16_t year, uint8_t wday)
 {
	 TIME64 var;
	 var.i64 = (int64_t)0;
	 var.sec = sec; var.min = mint; var.hour = hour;
	 var.day = day; var.mon = month; var.year = year - 1900;
	 if(wday == UNKNOWN_WEEK)
	 {
		 var.weekday = TimeUtil::WhichWeekday(var);
	 }
	 else
	 {
		 var.weekday = wday;
	 }
	 return var;
 }

 TIME64 defaultTime64( )
 {
	 return TimeUtil::Create(0,0,0,1,1,1900);
 }

/*int64_t time*/
TIME64 TimeUtil::Now()
{
    time_t now = time(NULL);
    struct tm t;
	localtime_r(&t, &now);
    TIME64 i64time = defaultTime64();
	i64time.weekday = t.tm_wday;
    i64time.sec = t.tm_sec;
    i64time.min = t.tm_min;
    i64time.hour = t.tm_hour;
    i64time.day = t.tm_mday;
    i64time.mon = t.tm_mon + 1;
    i64time.year = t.tm_year;

    return i64time;
}

int64_t TimeUtil::ToInt64(TIME64 curTime)
{
	int64_t i64 = 0;

	i64 = curTime.real_year; //[0,65535]
	i64 = i64 * 100;

	i64 += curTime.mon; //[1,12]
	i64 = i64 * 100;

	i64 += curTime.day;//[1,31]
	i64 = i64 * 100;

	i64 += curTime.hour; //[0,23]
	i64 = i64 * 100;

	i64 += curTime.min; //[0,59]
	i64 = i64 * 100;

	i64 += curTime.sec;

	return i64;
}

TIME64  TimeUtil::FromInt64(int64_t i64Time)
{

	TIME64 curTime;
	int64_t i64 = i64Time;
	
	curTime.sec		= (i64 % 100);
	curTime.min		= ((i64/100)%100);
	curTime.hour	= ((i64/10000)%100);
	curTime.day		= ((i64/1000000)%100);
	curTime.mon		= (uint8_t)((i64/100000000)%100);
	curTime.year	= (uint16_t)((i64/10000000000)%65536) - 1900;
	curTime.weekday = TimeUtil::WhichWeekday(curTime);

	return curTime;
}


#define HOUR_SEC					(60*60)
#define HOURS_SEC(hour)				((hour) * HOUR_SEC)
#define DAY_SEC						(24*60*60)
#define DAYS_SEC(day)				((day) * DAY_SEC)
#define MyTimeIsLeapYear(year)		(!((year) % 4) && ((year) % 100) || !((year) % 400))

static int32_t mon_day[13]		= {0,31,28,31,30,31,30,31,31,30,31,30,31};
static int32_t mon_day_leap[13]	= {1,31,29,31,30,31,30,31,31,30,31,30,31};

static int32_t* _MyTimeGetMonthDayOfYear(int32_t year)
{
    return MyTimeIsLeapYear(year) ? mon_day_leap : mon_day;
}

int64_t _ElapseSecondsFromThisYear(TIME64 srcTime)
{
    int64_t srcRun = 0;
    int32_t i;
    const  int32_t *mon_day_real;

    mon_day_real =  _MyTimeGetMonthDayOfYear((srcTime.real_year));

    for(i = 1; i < srcTime.mon; i++)
    {
        srcRun += DAYS_SEC(mon_day_real[i]);
    }

    srcRun += DAYS_SEC(srcTime.day - 1);
    srcRun += HOURS_SEC(srcTime.hour);
    srcRun += srcTime.min * 60 ;
    srcRun += srcTime.sec;
    return srcRun;
}

int64_t TimeUtil::Diff(TIME64 destTime, TIME64 srcTime)
{
    if(destTime.i64 == srcTime.i64)
        return 0;

    if( 1 )
    {
        int32_t i;
        int64_t srcRun;
        int64_t dstRun;
        srcRun = _ElapseSecondsFromThisYear(srcTime);
        dstRun = _ElapseSecondsFromThisYear(destTime);
		if(srcTime.year <= destTime.year) 
		{
			for(i = (int32_t)srcTime.year ; i < (int32_t)destTime.year; i++) 
			{
				dstRun += MyTimeIsLeapYear(i+1900) ?
                      DAYS_SEC(366) : DAYS_SEC(365);
			}
		} 
		else 
		{
			for(i = (int32_t)destTime.year; i < (int32_t)srcTime.year; i++) 
			{
				srcRun += MyTimeIsLeapYear(i+1900) ?
                      DAYS_SEC(366) : DAYS_SEC(365);
			}
		}

        return srcRun > dstRun ? (-1) : (dstRun - srcRun);
    }
}

bool TimeUtil::Timout(TIME64 curTime, TIME64 deadlineTime)
{
    int64_t diff = Diff(deadlineTime, curTime);
    return diff <= 0 ? true : false;
}

TIME64 TimeUtil::Add(TIME64 srcTime, int32_t nIncrementSecond)
{
    TIME64 dstTime;
    int64_t srcRun;
    int64_t srcForRun;
    int32_t days;
    int32_t secs;
    const  int32_t *mon_day_real;

    srcRun = _ElapseSecondsFromThisYear(srcTime);
    srcForRun = srcRun + nIncrementSecond;

    days = (int32_t) (srcForRun / DAY_SEC);
    secs = srcForRun % DAY_SEC;

    dstTime.year = srcTime.year;
    for( ; ; )
    {
        int32_t yearday = MyTimeIsLeapYear((dstTime.real_year)) ?
                        366 : 365;
        if(days < yearday) break;
        dstTime.year++;
        days -= yearday;
    }

    dstTime.mon = 1;
    dstTime.day = 1;

    mon_day_real =  _MyTimeGetMonthDayOfYear((dstTime.real_year));
    for( ; ; )
    {
        if(days < mon_day_real[dstTime.mon])
        {
            dstTime.day += days;
            break;
        }

        days -= mon_day_real[dstTime.mon];
        dstTime.mon++;
    }

    dstTime.hour = secs / (HOUR_SEC);
    dstTime.min = (secs / 60) % 60;
    dstTime.sec = secs % 60;

    return dstTime;
}

TIME64 TimeUtil::Dec(TIME64 srcTime, int32_t nDecrementSecond)
{
    int64_t srcRun;
    int64_t left;
    int32_t yearsday;
    int32_t days;
    int32_t secs;
    TIME64 dstTime;
    const  int32_t *mon_day_real;

    srcRun = _ElapseSecondsFromThisYear(srcTime);
    yearsday = MyTimeIsLeapYear((srcTime.real_year)) ? 366 : 365;
    left = DAYS_SEC(yearsday) - srcRun + nDecrementSecond;

    days = (int32_t)(left / DAY_SEC);
    secs = left % DAY_SEC;

    dstTime.year = srcTime.year;
    for( ; ; )
    {
        yearsday = MyTimeIsLeapYear((dstTime.real_year)) ?
                         366 : 365;
        if(days < yearsday) break;
        dstTime.year--;
        days -= yearsday;
    }

    days = yearsday - days - 1;
	if( secs == 0 ) { 
		days += 1; 
	} else {
		secs = DAY_SEC - secs;
	}

    mon_day_real =  _MyTimeGetMonthDayOfYear((dstTime.real_year));
    dstTime.mon = 1;
    dstTime.day = 1;
    for( ; ; )
    {
        if( days < mon_day_real[dstTime.mon])
        {
            dstTime.day += days;
            break;
        }

        days -= mon_day_real[dstTime.mon];
        dstTime.mon++;
    }

    dstTime.hour = secs / HOUR_SEC;
    dstTime.min = (secs / 60) % 60;
    dstTime.sec = secs % 60;

    return dstTime;
}

int32_t TimeUtil::WhichWeekday(TIME64 dw_time)
{
    int32_t i;
    int32_t days = 0;
    int32_t nYear = (int32_t)dw_time.real_year;
    const  int32_t *mon_day_real;

    mon_day_real =  _MyTimeGetMonthDayOfYear(dw_time.real_year);
    for( i = 1; i < dw_time.mon; i++ )
        days += mon_day_real[i];

    days += dw_time.day;

    // x-1+[(x-1)/4]-[(x-1)/100]+[(x-1)/400]+c
    return ( (nYear-1) + (nYear-1)/4 - (nYear-1)/100 + (nYear-1)/400 + days ) % 7;
}

int32_t TimeUtil::ToDateTime(TIME64 srcTime, char *buf, int32_t len)
{

    if(len < DATETIME_LEN)
        return -1;

    tsnprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d0",
            srcTime.real_year, srcTime.mon, srcTime.day,
            srcTime.hour, srcTime.min, srcTime.sec );

    return 0;
}

void TimeUtil::Format(char* buf, int32_t len, const char* fmt)
{
	struct tm t;
	time_t now = time(NULL);
	localtime_r(&t, &now);
	strftime(buf,len, fmt, &t);
}

int32_t TimeUtil::FromDateTime(TIME64 *t, const  char *buf, int32_t len)
{
    if(len < DATETIME_LEN || !t)
        return -1;

    if( 1 )
    {
        CHAR d[DATETIME_LEN];
        memcpy(d, buf, DATETIME_LEN);
        d[4] = d[7] = d[10] = d[13] = d[16] = d[19] = '0';

        t->year = atoi(d) - 1900;
        t->mon = atoi(d+5);
        t->day = atoi(d+8);
        t->hour = atoi(d+11);
        t->min = atoi(d+14);
        t->sec = atoi(d+17);
    }

    return 0;
}

void TimeUtil::ToTM(const TIME64* t, struct tm* m)
{
	if( t != NULL && m != NULL)
	{
		m->tm_sec =  t->sec;
		m->tm_min =  t->min;
		m->tm_hour = t->hour;
		m->tm_mday = t->day;
		m->tm_mon =  t->mon - 1;
		m->tm_year = t->year;
	}
}
