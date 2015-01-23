
#ifndef __LOG_H__
#define __LOG_H__

#include "Base.h"
#include "Assertx.h"

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


//////////////////////////////////////////////////////////////////////////
template<int32_t _BufSize, int32_t _BlockSize, class _Tag>
class LogSink
{
	enum 
	{
		LOGNAME_LEN=64,
		LOGTIME_LEN=64,
	};
public:
	explicit LogSink<_BufSize, _BlockSize, _Tag>(const char* fileName)
	{
		m_BufferedSize = 0;
		Init(fileName);
	}
	explicit LogSink<_BufSize, _BlockSize, _Tag>()
	{
		_Tag _tag;
		m_BufferedSize = 0;
		zeroMemory(m_LogFileName, LOGNAME_LEN);
		tsnprintf(m_LogFileName, LOGNAME_LEN, "%s", _tag.name());
		RebuildPath();
	}
	~ LogSink<_BufSize, _BlockSize, _Tag>(){ Flush(); }
public:
	void Init(const CHAR* fileName, int32_t param)
	{
		__ENTER_FUNCTION
		m_Parameter = param;
		zeroMemory(m_LogFileName, LOGNAME_LEN);
		tsnprintf(m_LogFileName, LOGNAME_LEN, "%s", fileName);
		RebuildPath();
		__LEAVE_FUNCTION
	}
	void Flush()
	{
		AutoLock_T lock(m_FileLock);
		_flushUnlock();
	}
	void RebuildPath()
	{
		AutoLock_T lock(m_FileLock);

		TIME64 now  = TimeUtil::Now();

		zeroMemory(m_LogFileCurName, LOGNAME_LEN);
		tsnprintf(m_LogFileCurName, LOGNAME_LEN, "./%s%s.%04d-%02d-%02d-%02d.log",
			g_Config.m_LogConfig.m_FilePrefix.c_str(),
			m_LogFileName, now.year+1900, now.mon, now.day, now.hour);

		m_LogPath = g_Config.m_LogConfig.m_LogDir.c_str();
		m_LogPath /= m_LogFileCurName;
	}
	void SaveLog(const CHAR* log, bool bFlush)
	{
		__ENTER_FUNCTION

			if( !log ) return ;

		AutoLock_T lock(m_FileLock);

		if( (m_BufferedSize + _BlockSize) >= _BufSize)
		{
			_flushUnlock();
		}

		TIME64 now  = TimeUtil::Now();
		char curTime[LOGTIME_LEN] = {0};
		tsnprintf(curTime, LOGTIME_LEN, "%04d-%02d-%02d %02d:%02d:%02d",
			now.year+1900, now.mon, now.day, now.hour, now.min, now.sec);

		char szBlock[_BlockSize] = {0};
		int32_t len = tsnprintf(szBlock, _BlockSize, "%s(%s)\r\n", log, curTime);
		if( len > 0 )
		{
			if(m_BufferedSize >= 0 && m_BufferedSize < _BufSize)
			{
				tsnprintf(m_LogBuffer+m_BufferedSize, _BufSize - m_BufferedSize, "%s", szBlock);
			}
			m_BufferedSize += len;

		//	if( sLogConfigure.isLogToStderr(level) ) coloredWriteToStderr(level, szBlock, len);
		}

		if(bFlush) _flushUnlock();

		__LEAVE_FUNCTION
	}
private:
	void _flushUnlock()
	{
		__ENTER_FUNCTION_EX

			if(m_BufferedSize > 0)
			{
				bfs::fstream fs(m_LogPath, std::ios_base::app);
				if(fs.good())
				{
					fs << m_LogBuffer;
					fs.close();

				}
				else
				{
					char errorDesc[64] = {0};
#if defined(__WINDOWS__)
					strerror_s(errorDesc, 128, errno);
#elif defined(__LINUX__)
					strerror_r(errno, errorDesc, 128);

#endif
					char assertMsg[256] = {0};
					tsnprintf(assertMsg, 256,"open file %s is failed(%s).",
						m_LogFileCurName, errorDesc);
					AssertEx(0, errorDesc);
				}
				m_BufferedSize = 0;
				zeroMemory(m_LogBuffer, _BufSize);
			}

			__LEAVE_FUNCTION_EX
	}
private:
	MyLock m_FileLock;
	int32_t m_Parameter;
	int32_t m_BufferedSize;
	bfs::path m_LogPath;
	char m_LogFileName[LOGNAME_LEN];
	char m_LogFileCurName[LOGNAME_LEN];
	char m_LogBuffer[_BufSize];
};

//////////////////////////////////////////////////////////////////////////
#define LOG_INST(LOGTYPE)		g_LogSink_##LOGTYPE
#define LOG_IMPL(LOGTYPE)		LogSink<16*1024, 4*1024, Tag##LOGTYPE> LOG_INST(LOGTYPE);
#define LOG_FLUSH(LOGTYPE)		LOG_INST(LOGTYPE).Flush()
#define LOG_REBUILD(LOGTYPE)	LOG_INST(LOGTYPE).RebuildPath()
#define LOG_DECL(LOGTYPE)		class Tag##LOGTYPE{public: const char* name(){return #LOGTYPE;} };\
								extern  LogSink<16*1024, 4*1024, Tag##LOGTYPE> LOG_INST(LOGTYPE)
//////////////////////////////////////////////////////////////////////////
template<typename _LogSink>
void _LogSinkPrint(_LogSink& rSink, const CHAR* keyVal, const CHAR* func, int32_t line, const CHAR* fmt, ...)
{
	__ENTER_FUNCTION_EX
	{
		CHAR fmtBuffer[4096] = {0};
		int32_t writeBytes = tsnprintf(fmtBuffer, 4096,"[%s][%s][%d]",
			keyVal, func, line);

		if(writeBytes > 0 && writeBytes < 4096)
		{
			va_list ap;
			va_start(ap,fmt);
			int32_t fmtBytes = (int32_t)vsnprintf(fmtBuffer+writeBytes, 4096 - writeBytes, fmt, ap);
			va_end(ap);

			if(fmtBytes > 0) writeBytes += fmtBytes;
		}

		if(writeBytes>0)
		{
			fmtBuffer[writeBytes%4096] = 0;
			rSink.SaveLog(fmtBuffer, false);
		}

	}
	__LEAVE_FUNCTION_EX
}


//////////////////////////////////////////////////////////////////////////
#define LOG_SAVE(SINK, DescStr, ...) _LogSinkPrint(LOG_INST(SINK),	DescStr, __FUNCTION__, __LINE__, __VA_ARGS__)
// #define LOGPD(SINK, ...) _LogSinkPrint(LOG_INST(SINK), (LOG_LEVEL_DEBUG),	__FUNCTION__, __LINE__,	__VA_ARGS__)
// #define LOGPW(SINK, ...) _LogSinkPrint(LOG_INST(SINK), (LOG_LEVEL_WARNING),__FUNCTION__, __LINE__,	__VA_ARGS__)
// #define LOGPE(SINK, ...) _LogSinkPrint(LOG_INST(SINK), (LOG_LEVEL_ERROR),	__FUNCTION__, __LINE__,	__VA_ARGS__)
// #define LOGPF(SINK, ...) _LogSinkPrint(LOG_INST(SINK), (LOG_LEVEL_FATAL),	__FUNCTION__, __LINE__,	__VA_ARGS__)
// 
// #define LOGI(...) _LogSinkPrint(LOG_INST(info),	(LOG_LEVEL_INFO),	__FUNCTION__, __LINE__, __VA_ARGS__)
// #define LOGD(...) _LogSinkPrint(LOG_INST(debug),	(LOG_LEVEL_DEBUG),	__FUNCTION__, __LINE__,	__VA_ARGS__)
// #define LOGW(...) _LogSinkPrint(LOG_INST(warning), (LOG_LEVEL_WARNING),__FUNCTION__, __LINE__,	__VA_ARGS__)
// #define LOGE(...) _LogSinkPrint(LOG_INST(error),	(LOG_LEVEL_ERROR),	__FUNCTION__, __LINE__,	__VA_ARGS__)
// #define LOGF(...) _LogSinkPrint(LOG_INST(fatal),	(LOG_LEVEL_FATAL),	__FUNCTION__, __LINE__,	__VA_ARGS__)

#endif
