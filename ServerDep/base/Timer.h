/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __TIMER__
#define __TIMER__

#include <PreCompier.h>
#include <EasyUnitTest.h>


BASE_NAME_SPACES

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
    };
} TIME64;
#pragma pack(pop)

class TimeUtil
{
public:
static uint32_t tickCount( );
static int64_t utcMilliseconds( );
static void utcTime(int32_t *seconds, int32_t *milliseconds);
static void addMillisecondsToNow(int32_t milliseconds, int32_t *sec, int32_t *ms);

public:
#define UNKNOWN_WEEK (0xff)
 static TIME64 initTime64(uint8_t sec, uint8_t mint, uint8_t hour, uint8_t day, uint8_t month, uint16_t year, uint8_t wday = UNKNOWN_WEEK);
 static TIME64 now();
/*
** this function return the seconds from current year 01-01 00:00:00
**/
static int64_t elapseSecondsFromThisYear(TIME64 srcTime);
/*
** the return value of this function is likely the strcmp
** if @destTime is equal to srcTime return 0
** if @destTime is less than srcTime return -1
** if @destTime is greater than srcTime return the seconds , from srcTime to destTime
**/
static int64_t timeCompare(TIME64 destTime, TIME64 srcTime);
static bool timout(TIME64 curTime, TIME64 deadlineTime);
static TIME64 timeAdd(TIME64 srcTime, int32_t nIncrementSecond);
static TIME64 timeDec(TIME64 srcTime, int32_t nDecrementSecond);
/*
** @return value means:[0:sunday, 1:monday ... 6:saturday]
*/
static int32_t timeWhichWeekday(TIME64 dw_time);

/*
@@ DATETIME_LEN min length of @buf
** @len >= DATETIME_LEN
** convert ae_time64_t to string(eg."1983-10-08 00:00:00")
*/
#define DATETIME_LEN 20
static int32_t timeToDateTime(TIME64 srcTime, char *buf, int32_t len);
/*
* fmt = %Y%m%d%H%M%S(20080912132328)
*/
static void format(char* buf, int32_t len, const char* fmt);

/*
** @len >= DATETIME_LEN
** convert string(eg. "1983-10-08 00:00:00" ) to ae_time64_t
*/
static int32_t timeFromDateTime(TIME64 *t, const  char *buf, int32_t len);
static void timeToTM(const TIME64* t, struct tm* m);
};

BASE_NAME_SPACEE

/*
**
*/
#ifdef TIME_UNIT_TEST
AUTOTEST_DEF(timeUitlUnitTest);
#endif

#endif /* __TIMER__*/

