//#include "stdafx.h"

#include "Log.h"
#include "TimeManager.h"
//#include "Config.h"
#include "Assertx.h"
#include "GameUtil.h"

CHAR* g_pLogFileName[] =
{
	"./Log/login",		//0		LOG_FILE_0
	"./Log/debug",		//1		LOG_FILE_1
	"./Log/error",		//2		LOG_FILE_2
	"./Log/functions",	//3		LOG_FILE_3
	"./Log/world",		//4		LOG_FILE_4
	"./Log/item",		//5		LOG_FILE_5
	"./Log/send",		//6		LOG_FILE_6
	"./Log/money",		//7		LOG_FILE_7
	"./Log/pet",		//8		LOG_FILE_8
	"./Log/skill",		//9		LOG_FILE_9
	"./Log/xinfa",		//10	LOG_FILE_10
	"./Log/ability",	//11	LOG_FILE_11
	"./Log/chat",		//12	LOG_FILE_12
	"./Log/mission",	//13	LOG_FILE_13
	'\0'
};

MyLock g_log_lock ;

Log* g_pLog=NULL ;

Log::Log( )
{
__ENTER_FUNCTION

	for( int32_t i=0; i<LOG_FILE_NUMBER; i++ )
	{
		m_LogCache[i] = NULL ;
		m_LogPos[i] = 0 ;
	}
	m_CacheSize = 0 ;
	m_DayTime= 0 ;

__LEAVE_FUNCTION
}

Log::~Log( )
{
__ENTER_FUNCTION

	for( int32_t i=0; i<LOG_FILE_NUMBER; i++ )
	{
		SAFE_DELETE_ARRAY(m_LogCache[i])  ;
	}
	m_CacheSize = 0 ;

__LEAVE_FUNCTION
}

bool Log::Init( )
{
__ENTER_FUNCTION

	m_CacheSize = DEFAULT_LOG_CACHE_SIZE ;
	//最小化内存占用
	//______________________

for( int32_t i=0; i<LOG_FILE_NUMBER; i++ )
{
	m_LogCache[i] = new CHAR[m_CacheSize] ;
	if( m_LogCache[i] == NULL )
	{
		return false ;
	}
	m_LogPos[i] = 0 ;
}

	//______________________
	m_DayTime = g_TimeManager.GetDayTime() ;


	return true ;

__LEAVE_FUNCTION

	return false ;
}

void Log::FastSaveLog( int32_t logid, CHAR* msg, ... )
{
__ENTER_FUNCTION

	if( logid<0 || logid >=LOG_FILE_NUMBER )
		return ;

	CHAR buffer[2048];
	va_list argptr;

	try
	{
		va_start(argptr, msg);
		vsprintf(buffer,msg,argptr);
		va_end(argptr);

		CHAR szTime[64] ;
		sprintf( szTime, " (%d)(T=%.4f)\r\n", 
			MyGetCurrentThreadID(),
			(float)(g_TimeManager.RunTime())/1000.0 ) ;
		strcat( buffer, szTime ) ;
	}
	catch(...)
	{
		Assert(false) ;
		return ;
	}

	int32_t iLen = (int32_t)strlen(buffer) ;
	if( iLen<=0 )
		return ;

	//最小化内存占用
	//______________________
	if( 1 )
	{
		CHAR szName[_MAX_PATH] ;
		GetLogName( logid, szName ) ;

		SaveLog( szName, buffer );
		return;
	}
	//______________________

	AutoLock_T Lock(m_LogLock[logid]);
	try
	{
		memcpy( m_LogCache[logid]+m_LogPos[logid], buffer, iLen ) ;
		m_LogPos[logid] += iLen ;
		printf( buffer ) ;
	}
	catch(...)
	{
	}

	

	if( m_LogPos[logid] > (DEFAULT_LOG_CACHE_SIZE*2)/3 )
	{
		FlushLog( logid ) ;
	}
	return ;

__LEAVE_FUNCTION

	return ;
}

void Log::FlushLog( int32_t logid )
{
__ENTER_FUNCTION

	CHAR szName[_MAX_PATH] ;
	GetLogName( logid, szName ) ;

	AutoLock_T Lock(m_LogLock[logid]);
	try
	{
		FILE* f = fopen( szName, "ab" ) ;
		fwrite( m_LogCache[logid], 1, m_LogPos[logid], f ) ;
		fclose(f) ;
		m_LogPos[logid] = 0 ;
	}
	catch(...)
	{
	}

	return ;

__LEAVE_FUNCTION
}

void Log::FlushLog_All( )
{
__ENTER_FUNCTION

	for( int32_t i=0; i<LOG_FILE_NUMBER; i++ )
	{
		FlushLog(i) ;
	}

__LEAVE_FUNCTION
}


void Log::GetLogName( int32_t logid, CHAR* szName )
{
__ENTER_FUNCTION

	sprintf( szName, "%s_%d.log", g_pLogFileName[logid], m_DayTime ) ;

__LEAVE_FUNCTION
}

void Log::SaveLog( CHAR* filename, CHAR* msg, ... )
{
__ENTER_FUNCTION

	AutoLock_T Lock(g_log_lock);

	CHAR buffer[2048];
	memset( buffer, 0, 2048 ) ;

	va_list argptr;

	_MY_TRY
	{
		va_start(argptr, msg);
		vsprintf(buffer,msg,argptr);
		va_end(argptr);

		CHAR szTime[64]= {0};
		sprintf( szTime, " (%d)(T=%.4f)\r\n", 
			MyGetCurrentThreadID(), (float)(g_TimeManager.RunTime())/1000.0 ) ;
		strcat( buffer, szTime ) ;

		FILE* f = fopen( filename, "ab" ) ;
		fwrite( buffer, 1, strlen(buffer), f ) ;
		fclose(f) ;

		printf( buffer ) ;
	}
	_MY_CATCH
	{
		printf("a big error here");
	}


	return ;

__LEAVE_FUNCTION

	return ;
}

void Log::RemoveLog( CHAR* filename )
{
__ENTER_FUNCTION

	AutoLock_T Lock(g_log_lock);

	FILE* f = fopen( filename, "w" ) ;
	fclose(f) ;

;
	return ;

__LEAVE_FUNCTION

	return ;
}

