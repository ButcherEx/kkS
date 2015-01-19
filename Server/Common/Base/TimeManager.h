
#ifndef __TIMEMANAGER_H__
#define __TIMEMANAGER_H__

#include "Base.h"


class TimeManager
{
public :
	TimeManager( ) ;
	~TimeManager( ) ;

	bool			Init( ) ;

	//当前时间计数值，起始值根据系统不同有区别
	//返回的值为：微妙单位的时间值
	uint32_t			CurrentTime( ) ;
	//直接返回，不调用系统接口
	uint32_t			CurrentSavedTime( ){ return m_CurrentTime ; } ;
	//取得服务器端程序启动时的时间计数值
	uint32_t			StartTime( ){ return m_StartTime ; } ;

	//将当前的系统时间格式化到时间管理器里
	void			SetTime( ) ;

	// 得到标准时间
	time_t			GetANSITime( );

public :
	//***注意：
	//以下接口调用没有系统调用，只针对时间管理器内的数据
	//

	//取得设置时间时候的“年、月、日、小时、分、秒、星期的值”
	int32_t				GetYear( ){ return m_TM.tm_year+1900 ; } ;	//[1900,????]
	int32_t				GetMonth( ){ return m_TM.tm_mon ; } ;		//[0,11]
	int32_t				GetDay( ){ return m_TM.tm_mday ; } ;		//[1,31]
	int32_t				GetHour( ){ return m_TM.tm_hour ; } ;		//[0,23]
	int32_t				GetMinute( ){ return m_TM.tm_min ; } ;		//[0,59]
	int32_t				GetSecond( ){ return m_TM.tm_sec ; } ;		//[0,59]
	//取得当前是星期几；0表示：星期天，1～6表示：星期一～星期六
	uint32_t			GetWeek( ){ return m_TM.tm_wday ; } ;
	//将当前的时间（年、月、日、小时、分）转换成一个uint32_t来表示
	//例如：0,507,211,233 表示 "2005.07.21 12:33"
	uint32_t			Time2DWORD( ) ;
	//取得当前的日期[4bit 0-9][4bit 0-11][5bit 0-30][5bit 0-23][6bit 0-59][6bit 0-59]
	uint32_t			CurrentDate( ) ;
	//取得服务器启动后的运行时间（毫秒）
	uint32_t			RunTime( ){ 
		CurrentTime( ) ;
		return (m_CurrentTime-m_StartTime);  
	} ;
	uint16_t			RunTick( )
	{
		CurrentTime();
		return uint16_t(m_CurrentTime-m_StartTime);  
	};
	//取得两个日期时间的时间差（单位：毫秒）, Ret = Date2-Data1
	uint32_t			DiffTime( uint32_t Date1, uint32_t Date2 ) ;
	//将一个uint32_t的日期转换成一个tm结构
	void			ConvertUT( uint32_t Date, tm* TM ) ;
	//将一个tm结构转换成一个uint32_t的日期
	void			ConvertTU( tm* TM, uint32_t& Date ) ;
	//取得已天为单位的时间值, 千位数代表年份，其他三位代表时间（天数）
	uint32_t			GetDayTime( ) ;
	//得到当前是今天的什么时间2310表示23点10分
	uint16_t			GetTodayTime();
	bool			FormatTodayTime(uint16_t& nTime);


public :
	uint32_t			m_StartTime ;
	uint32_t			m_CurrentTime ;
	time_t			m_SetTime ;
	tm				m_TM ;
#ifdef __LINUX__
	struct timeval _tstart, _tend;
	struct timezone tz;
#endif



};

extern TimeManager g_TimeManager;




#endif
