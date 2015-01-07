
#ifndef __LOG_H__
#define __LOG_H__

#include "Base.h"

enum LOG_FILE_NAME_ID
{
	LOG_FILE_0 =0 ,
	LOG_FILE_1 =1 ,
	LOG_FILE_2 =2 ,
	LOG_FILE_3 =3 ,
	LOG_FILE_4 =4 ,
	LOG_FILE_5 =5 ,
	LOG_FILE_6 =6 ,
	LOG_FILE_7 =7 ,
	LOG_FILE_8 =8 ,
	LOG_FILE_9 =9 ,
	LOG_FILE_10 =10,
	LOG_FILE_11 =11,
	LOG_FILE_12 =12,
	LOG_FILE_13 =13,

	LOG_FILE_NUMBER ,
};

#define LOGIN_LOGFILE					"./Log/Login.log"
#define BILLING_LOGFILE					"./Log/Billing.log"
#define SERVER_LOGFILE					"./Log/Debug.log"
#define SERVER_ERRORFILE				"./Log/Error.log"
#define SERVER_FUNCTIONFILE				"./Log/Functions.log"
#define WORLD_LOGFILE					"./Log/world.log"
#define CONFIG_LOGFILE					"./Log/config.log"
#define ASSERT_LOGFILE					"./Log/assert.log"
#define RECYCLEPLAYER_LOGFILE			"./Log/RecyclePlayer.log"


#define DEFAULT_LOG_CACHE_SIZE 1024*1024*4
class Log
{
public :
	Log( ) ;
	~Log( ) ;

	bool			Init( ) ;

	//向日志缓存写入日志信息
	void			FastSaveLog( int32_t logid, CHAR* msg, ... ) ;

	//将日志内存数据写入文件
	void			FlushLog( int32_t logid ) ;

	//取得日志有效数据大小
	int32_t				GetLogSize( int32_t logid ){ return m_LogPos[logid] ; }

	//取得保存日志的文件名称
	void			GetLogName( int32_t logid, CHAR* szName ) ;

	//刷新每个日志文件
	void			FlushLog_All( ) ;

	//取得日期天数
	uint32_t			GetDayTime( ){ return m_DayTime ; }
	//设置日期天数
	void			SetDayTime( uint32_t daytime ){ m_DayTime = daytime ; }


	//支持异步写入操作的日志写入
	static void		SaveLog( CHAR* filename, CHAR* msg, ... ) ;	
	//删除日志内容
	static void		RemoveLog( CHAR* filename ) ;


private :
	CHAR*			m_LogCache[LOG_FILE_NUMBER] ;	//日志内存区
	int32_t			m_LogPos[LOG_FILE_NUMBER] ;		//日志当前有效数据位置
	MyLock			m_LogLock[LOG_FILE_NUMBER] ;	//日志读写锁
	int32_t			m_CacheSize ;
	uint32_t		m_DayTime ;
};

#if defined __LINUX__
	#define SaveCodeLog()	(Log::SaveLog( SERVER_ERRORFILE, "%s %d %s", __FILE__,__LINE__,__PRETTY_FUNCTION__ ))
#else
	#define SaveCodeLog()	(Log::SaveLog( SERVER_ERRORFILE, "%s %d %s", __FILE__,__LINE__,__FUNCTION__ ))
#endif

extern Log* g_pLog ;

#endif
