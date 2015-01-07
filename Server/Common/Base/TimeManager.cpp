#include "TimeManager.h"
#include "Assertx.h"

TimeManager*	g_pTimeManager=NULL ;

TimeManager::TimeManager( )
{
__ENTER_FUNCTION

	m_CurrentTime = 0 ;

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

#if defined(__WINDOWS__)
	m_StartTime = GetTickCount() ;
	m_CurrentTime = GetTickCount() ;
#elif defined(__LINUX__)
	m_StartTime		= 0;
	m_CurrentTime	= 0;
	gettimeofday(&_tstart, &tz);
#endif
	SetTime( ) ;

	return true ;

__LEAVE_FUNCTION

	return false ;
}

uint32_t TimeManager::CurrentTime( )
{
__ENTER_FUNCTION

#if defined(__WINDOWS__)
	m_CurrentTime = GetTickCount() ;
#elif defined(__LINUX__)
	gettimeofday(&_tend,&tz);
	double t1, t2;
	t1 =  (double)_tstart.tv_sec*1000 + (double)_tstart.tv_usec/1000;
	t2 =  (double)_tend.tv_sec*1000 + (double)_tend.tv_usec/1000;
	m_CurrentTime = (uint32_t)(t2-t1);
#endif

	return m_CurrentTime ;


__LEAVE_FUNCTION

	return 0 ;
}

uint32_t	TimeManager::CurrentDate()
{
	__ENTER_FUNCTION

	SetTime( ) ;
	uint32_t Date;
	ConvertTU(&m_TM,Date);

	return Date;

	__LEAVE_FUNCTION

		return 0;
}


void TimeManager::SetTime( )
{
__ENTER_FUNCTION

	time( &m_SetTime ) ;
	tm* ptm = localtime( &m_SetTime ) ;
	m_TM = *ptm ;

__LEAVE_FUNCTION
}

// 得到标准时间
time_t TimeManager::GetANSITime( )
{
__ENTER_FUNCTION

	SetTime();

__LEAVE_FUNCTION

	return m_SetTime;
}

uint32_t TimeManager::Time2DWORD( )
{
__ENTER_FUNCTION

	SetTime( ) ;

	uint32_t uRet=0 ;

	uRet += GetYear( ) ;
	uRet -= 2000 ;
	uRet =uRet*100 ;

	uRet += GetMonth( )+1 ;
	uRet =uRet*100 ;

	uRet += GetDay( ) ;
	uRet =uRet*100 ;

	uRet += GetHour( ) ;
	uRet =uRet*100 ;

	uRet += GetMinute( ) ;

	return uRet ;

__LEAVE_FUNCTION

	return 0 ;
}

uint32_t TimeManager::DiffTime( uint32_t Date1, uint32_t Date2 )
{
__ENTER_FUNCTION

	tm S_D1, S_D2 ;
	ConvertUT( Date1, &S_D1 ) ;
	ConvertUT( Date2, &S_D2 ) ;
	time_t t1,t2 ;
	t1 = mktime(&S_D1) ;
	t2 = mktime(&S_D2) ;
	uint32_t dif = (uint32_t)(difftime(t2,t1)*1000) ;
	return dif ;

__LEAVE_FUNCTION

	return 0 ;
}

void TimeManager::ConvertUT( uint32_t Date, tm* TM )
{
__ENTER_FUNCTION

	Assert(TM) ;
	memset( TM, 0, sizeof(tm) ) ;
	TM->tm_year = (Date>>26)&0xf ;
	TM->tm_mon  = (Date>>22)&0xf ;
	TM->tm_mday = (Date>>17)&0x1f ;
	TM->tm_hour = (Date>>12)&0x1f ;
	TM->tm_min  = (Date>>6) &0x3f ;
	TM->tm_sec  = (Date)    &0x3f ;

__LEAVE_FUNCTION
}

void TimeManager::ConvertTU( tm* TM, uint32_t& Date )
{
__ENTER_FUNCTION

	Assert( TM ) ;
	Date = 0 ;
	Date += (TM->tm_yday%10) & 0xf ;
	Date = (Date<<4) ;
	Date += TM->tm_mon & 0xf ;
	Date = (Date<<4) ;
	Date += TM->tm_mday & 0x1f ;
	Date = (Date<<5) ;
	Date += TM->tm_hour & 0x1f ;
	Date = (Date<<5) ;
	Date += TM->tm_min & 0x3f ;
	Date = (Date<<6) ;
	Date += TM->tm_sec & 0x3f ;

__LEAVE_FUNCTION
}

uint32_t TimeManager::GetDayTime( )
{
__ENTER_FUNCTION

	time_t st ;
	time( &st ) ;
	tm* ptm = localtime( &m_SetTime ) ;

	uint32_t uRet=0 ;

	uRet  = (ptm->tm_year-100)*1000 ;
	uRet += ptm->tm_yday ;

	return uRet ;

__LEAVE_FUNCTION

	return 0 ;
}

uint16_t TimeManager::GetTodayTime()
{
__ENTER_FUNCTION
	time_t st ;
	time( &st ) ;
	tm* ptm = localtime( &m_SetTime ) ;

	uint16_t uRet=0 ;

	uRet  = ptm->tm_hour*100 ;
	uRet += ptm->tm_min ;

	return uRet ;

__LEAVE_FUNCTION

return 0 ;
}

bool TimeManager::FormatTodayTime(uint16_t& nTime)
{
__ENTER_FUNCTION
	bool ret = false;

	uint16_t wHour = nTime / 100;
	uint16_t wMin = nTime % 100;
	uint16_t wAddHour = 0;
	if( wMin > 59 )
	{
		wAddHour = wMin / 60;
		wMin = wMin % 60;
	}
	wHour += wAddHour;
	if( wHour > 23 )
	{
		ret = true;
		wHour = wHour % 60;
	}

	return ret;

__LEAVE_FUNCTION

return false ;
}
