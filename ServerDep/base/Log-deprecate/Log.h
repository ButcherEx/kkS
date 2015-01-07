#ifndef __LOG_H__
#define __LOG_H__

#include <PreCompier.h>
#if defined(HAVE_LIB_GFLAGS)
#include <gflags/gflags.h>
#endif
//////////////////////////////////////////////////////////////////////////
/*
* Level define
*/
#define LOG_LEVEL_INFO		(0x0000)
#define LOG_LEVEL_DEBUG		(0x0001)
#define LOG_LEVEL_WARNING	(0x0002)
#define LOG_LEVEL_ERROR		(0x0004)
#define LOG_LEVEL_FATAL		(0x0008)
const char* logLevelDesc(int32_t level);
//////////////////////////////////////////////////////////////////////////
#if defined(HAVE_LIB_GFLAGS)
//
//// Set whether log messages go to stderr instead of logfiles
//DECLARE_bool(logtostderr);
//
//// Set whether log messages go to stderr in addition to logfiles.
//DECLARE_bool(alsologtostderr);
//
//// Set color messages logged to stderr (if supported by terminal).
//DECLARE_bool(colorlogtostderr);
//
//// Log messages at a level >= this flag are automatically sent to
//// stderr in addition to log files.
//DECLARE_int32(stderrthreshold);
//
//// Set whether the log prefix should be prepended to each line of output.
//DECLARE_bool(log_prefix);
//
//// Log suppression level: messages logged at a lower level than this
//// are suppressed.
//DECLARE_int32(minloglevel);
//
//// Log messages at a level <= this flag are buffered.
//// Log messages at a higher level are flushed immediately.
//DECLARE_int32(logbuflevel);
//
//// Sets the maximum number of seconds which logs may be buffered for.
//DECLARE_int32(logbufsecs);

// If specified, logfiles are written into this directory instead of the
// default logging directory.
//DECLARE_string(log_dir);

//// Sets the maximum log file size (in MB).
//DECLARE_int32(max_log_size);
//
//// Sets whether to avoid logging to the disk if the disk is full.
//DECLARE_bool(stop_logging_if_full_disk);
#endif

//////////////////////////////////////////////////////////////////////////
void initLoggingUtilities(const char* argv0);
void hostName(bstd::string& hn);
void coloredWriteToStderr(int32_t severity,const char* message, size_t len);
const char* logDir();
const char* ProgramInvocationShortName();
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

template<int32_t _BufSize, int32_t _BlockSize>
class LogSink
{
	enum 
	{
		LOGNAME_LEN=64,
		LOGTIME_LEN=64,
	};
public:
	explicit LogSink<_BufSize>(const char* fileName)
	{
		bufferedSize_ = 0;
		zeroMemory(logFileName_, LOGNAME_LEN);
		snprintf(logFileName_, LOGNAME_LEN, "%s", fileName);
	}
	~ LogSink<_BufSize>(){ flush(); }
public:
	void init(int32_t param)
	{
		__ENTER_FUNCTION

		param_ = param;
		rebuildPath();

		__LEAVE_FUNCTION
	}
	void flush()
	{
		ScopedLock lock(lock_);
		flushUnlock();
	}
	void rebuildPath()
	{
		ScopedLock lock(lock_);

		base::TIME64 now  = base::TimeUtil::now();
		char logPath[MAX_PATH] = {0};
		snprintf(logPath, MAX_PATH, "./%s.%04d-%02d-%02d-%02d.log",
			logFileName_, now.year + 1900, now.mon, now.day, now.hour);
		logPath_ = logDir();
		logPath_ /= logPath;
	}
	void log(const char* log, bool bFlush)
	{
		__ENTER_FUNCTION

		if( !log ) return ;

		if( (bufferedSize_ + _BlockSize) >= _BufSize)
		{
			flushUnlock();
		}

		base::TIME64 now  = base::TimeUtil::now();
		char curTime[LOGTIME_LEN] = {0};
		snprintf(curTime, LOGTIME_LEN, "[%04d-%02d %02d:%02d:%02d]",
			now.year + 1900, now.mon, now.day, now.hour, now.sec);

		char szBlock[LOGBLOCK_LEN] = {0};
		int32_t len = snprintf(szBlock, LOGBLOCK_LEN, "%s (%s)\n", log, curTime);
		if( len > 0 )
		{
			if(bufferedSize_ > 0 && bufferedSize_ < _BufSize)
			{
				snprintf(logBuf_+bufferedSize_, _BufSize - bufferedSize_, "%s", szBlock);
			}
			bufferedSize_ += len;
		}

		__LEAVE_FUNCTION
	}
private:
	void flushUnlock()
	{
		__ENTER_FUNCTION_EX

			if(bufferedSize_ > 0)
			{
				bfs::fstream fs(logPath_, std::ios_base::app);
				if(fs.good())
				{
					fs << logBuf_;
					fs.close();
				}
				bufferedSize_ = 0;
				zeroMemory(logBuf_, _BufSize);
			}

		__LEAVE_FUNCTION_EX
	}
private:
	Mutex lock_;
	int32_t param_;
	int32_t bufferedSize_;
	bfs::path logPath_;
	char logFileName_[LOGNAME_LEN];
	char logBuf_[_BufSize];
};

//////////////////////////////////////////////////////////////////////////
#define LOG_INST(LOGTYPE) g##LOGTYPE##LogSink
#define LOG_DECL(LOFTYPE) extern LogSink<16*1024, 4*1024> LOG_INST(LOGTYPE)
#define LOG_INIT(LOGTYPE,PARAM) LOG_INST(LOGTYPE).init(PARAM)
#define LOG_FLUSH(LOGTYPE) LOG_INST(LOGTYPE).flush()
#define LOG_REBUILD(LOGTYPE) LOG_INST(LOGTYPE).rebuildPath()
//////////////////////////////////////////////////////////////////////////

class LogManager
{

};

//////////////////////////////////////////////////////////////////////////
template<typename _LogSink>
void _logPrintf(_LogSink& rSink, int32_t level, const char* func, int32_t line, const char* fmt, ...)
{
	__ENTER_FUNCTION_EX
	{
		char fmtBuffer[4096] = {0};
		int32_t writeBytes = snprintf(fmtBuffer, 4096,"[%s][%s][%d]",
			logLevelDesc(level), func, line);

		if(writeBytes > 0 && writeBytes < 4096)
		{
			va_list ap;
			va_start(ap,fmt);
			int32_t fmtBytes = (int32_t)vsnprintf(msg+writeBytes, LOG_BUFFER_SIZE - writeBytes, fmt, ap);
			va_end(ap);

			if(fmtBytes > 0) writeBytes += fmtBytes;
		}

		if(writeBytes>0) rSink.log(fmtBuffer, false);

	}
	__LEAVE_FUNCTION_EX
}

//////////////////////////////////////////////////////////////////////////
#define LOGPI(ID, ...) _logPrintf(ID, (LOG_LEVEL_INFO),		__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGPD(ID, ...) _logPrintf(ID, (LOG_LEVEL_DEBUG),	__FUNCTION__, __LINE__,	__VA_ARGS__)
#define LOGPW(ID, ...) _logPrintf(ID, (LOG_LEVEL_WARNING),	__FUNCTION__, __LINE__,	__VA_ARGS__)
#define LOGPE(ID, ...) _logPrintf(ID, (LOG_LEVEL_ERROR),	__FUNCTION__, __LINE__,	__VA_ARGS__)
#define LOGPF(ID, ...) _logPrintf(ID, (LOG_LEVEL_FATAL),	__FUNCTION__, __LINE__,	__VA_ARGS__)

#define LOGI LOGPI
#define LOGD LOGPD
#define LOGW LOGPW
#define LOGE LOGPE
#define LOGF LOGPF

//////////////////////////////////////////////////////////////////////////

#endif