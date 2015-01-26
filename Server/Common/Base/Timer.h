/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "Base.h"

#pragma pack(push,8)
typedef union TIME64_t
{
    uint64_t i64;
    struct
    {
		uint8_t weekday;	/* reserve */
        uint8_t sec ;		/*0 - 59*/
        uint8_t min ;		/*0 - 59*/
        uint8_t hour ;		/* 0 - 23*/
        uint8_t day ;		/* 1 - 31 */
        uint8_t mon ;		/* 1 - 12 */
        uint16_t year;		/* years since 1900 */
#define real_year year+1900

    };
} TIME64;
#pragma pack(pop)

class TimeUtil
{
public:
static uint32_t TickCount( );
static int64_t UtcMilliseconds( );
static void UtcTime(int32_t *seconds, int32_t *milliseconds);
static void AddMillisecondsToNow(int32_t milliseconds, int32_t *sec, int32_t *ms);

public:
#define UNKNOWN_WEEK (0xff)
 static TIME64 InitTime64(uint8_t sec, uint8_t mint, uint8_t hour, uint8_t day, uint8_t month, uint16_t year, uint8_t wday = UNKNOWN_WEEK);
 static TIME64 Now();
 static int64_t  TimeToInt64(TIME64 curTime);
 static TIME64  Int64ToTime(int64_t i64Time);
/*
** this function return the seconds from current year 01-01 00:00:00
**/
//static int64_t _ElapseSecondsFromThisYear(TIME64 srcTime);
/*
** the return value of this function is likely the strcmp
** if @destTime is equal to srcTime return 0
** if @destTime is less than srcTime return -1
** if @destTime is greater than srcTime return the seconds , from srcTime to destTime
**/
static int64_t TimeCompare(TIME64 destTime, TIME64 srcTime);
static bool Timout(TIME64 curTime, TIME64 deadlineTime);
static TIME64 TimeAdd(TIME64 srcTime, int32_t nIncrementSecond);
static TIME64 TimeDec(TIME64 srcTime, int32_t nDecrementSecond);
/*
** @return value means:[0:sunday, 1:monday ... 6:saturday]
*/
static int32_t TimeWhichWeekday(TIME64 dw_time);

/*
@@ DATETIME_LEN min length of @buf
** @len >= DATETIME_LEN
** convert ae_time64_t to string(eg."1983-10-08 00:00:00")
*/
#define DATETIME_LEN 20
static int32_t TimeToDateTime(TIME64 srcTime, char *buf, int32_t len);
/*
* fmt = %Y%m%d%H%M%S(20080912132328)
*/
static void Format(char* buf, int32_t len, const char* fmt);

/*
** @len >= DATETIME_LEN
** convert string(eg. "1983-10-08 00:00:00" ) to ae_time64_t
*/
static int32_t TimeFromDateTime(TIME64 *t, const  char *buf, int32_t len);
static void TimeToTM(const TIME64* t, struct tm* m);
};

#endif /* __TIMER_H__*/

