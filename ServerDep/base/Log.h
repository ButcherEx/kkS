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
void clearStderrLineBuffer(FILE *file);
void coloredWriteToStderr(int32_t severity,const char* message, size_t len);
//////////////////////////////////////////////////////////////////////////
class LogConfigure : public boost::noncopyable
{
public:
	void initialize(const char* argv0, const char* directory = "./log", 
		bool prefixPid=false, bool prefixInvocation = false, bool prefixHost = false);

	const bfs::path& baseDir(){ return basePath_; }
	const bstd::string& hostName() { return hostName_; }
	const bstd::string& invocationShortName(){ return invocationName_; }
	const bstd::string& filePrefix(){ return filePrefix_; }
	bool isLogToStderr(int32_t level);
public:
	LogConfigure();
private:
	bfs::path basePath_;
	bstd::string hostName_;
	bstd::string invocationName_;
	bstd::string filePrefix_;
};
#define sLogConfigure Singleton<LogConfigure>::instance()
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
		bufferedSize_ = 0;
		init(fileName);
	}
	explicit LogSink<_BufSize, _BlockSize, _Tag>()
	{
		_Tag _tag;
		bufferedSize_ = 0;
		zeroMemory(logFileName_, LOGNAME_LEN);
		snprintf(logFileName_, LOGNAME_LEN, "%s", _tag.name());
		rebuildPath();
	}
	~ LogSink<_BufSize, _BlockSize, _Tag>(){ flush(); }
public:
	void init(const char* fileName, int32_t param)
	{
		__ENTER_FUNCTION
		param_ = param;
		zeroMemory(logFileName_, LOGNAME_LEN);
		snprintf(logFileName_, LOGNAME_LEN, "%s", fileName);
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
		
		zeroMemory(logFileCurName_, LOGNAME_LEN);
		snprintf(logFileCurName_, LOGNAME_LEN, "./%s%s.%04d-%02d-%02d-%02d.log",
			sLogConfigure.filePrefix().c_str(),
			logFileName_, now.year + 1900, now.mon, now.day, now.hour);

		logPath_ = sLogConfigure.baseDir();
		logPath_ /= logFileCurName_;
	}
	void log(int32_t level, const char* log, bool bFlush)
	{
		__ENTER_FUNCTION

		if( !log ) return ;

		ScopedLock lock(lock_);

		if( (bufferedSize_ + _BlockSize) >= _BufSize)
		{
			flushUnlock();
		}

		base::TIME64 now  = base::TimeUtil::now();
		char curTime[LOGTIME_LEN] = {0};
		snprintf(curTime, LOGTIME_LEN, "%04d-%02d-%02d %02d:%02d:%02d",
			now.year + 1900, now.mon, now.day, now.hour, now.min, now.sec);

		char szBlock[_BlockSize] = {0};
		int32_t len = snprintf(szBlock, _BlockSize, "%s (%s).\n", log, curTime);
		if( len > 0 )
		{
			if(bufferedSize_ >= 0 && bufferedSize_ < _BufSize)
			{
				snprintf(logBuf_+bufferedSize_, _BufSize - bufferedSize_, "%s", szBlock);
			}
			bufferedSize_ += len;

			if( sLogConfigure.isLogToStderr(level) ) coloredWriteToStderr(level, szBlock, len);
		}

		if(bFlush) flushUnlock();

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
				else
				{
					char errorDesc[64] = {0};
#if defined(__WINDOWS__)
					strerror_s(errorDesc, 128, errno);
#elif defined(__LINUX__)
					strerror_r(errno, errorDesc, 128);

#endif
					char assertMsg[256] = {0};
					snprintf(assertMsg, 256,"open file %s is failed(%s).",
						logFileCurName_, errorDesc);
					_Assert(0, errorDesc);
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
	char logFileCurName_[LOGNAME_LEN];
	char logBuf_[_BufSize];
};

//////////////////////////////////////////////////////////////////////////
#define LOG_DECL(LOGTYPE) class Tag##LOGTYPE{public: const char* name(){return #LOGTYPE;} }
#define LOG_INST(LOGTYPE) Singleton< LogSink<16*1024, 4*1024, Tag##LOGTYPE> >::instance()
#define LOG_IMPL(LOGTYPE, PARAM) LOG_INST(LOGTYPE).init(#LOGTYPE, PARAM)
#define LOG_FLUSH(LOGTYPE) LOG_INST(LOGTYPE).flush()
#define LOG_REBUILD(LOGTYPE) LOG_INST(LOGTYPE).rebuildPath()
//////////////////////////////////////////////////////////////////////////

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
			int32_t fmtBytes = (int32_t)vsnprintf(fmtBuffer+writeBytes, 4096 - writeBytes, fmt, ap);
			va_end(ap);

			if(fmtBytes > 0) writeBytes += fmtBytes;
		}

		if(writeBytes>0)
		{
			fmtBuffer[writeBytes%4096] = 0;
			rSink.log(level, fmtBuffer, false);
		}

	}
	__LEAVE_FUNCTION_EX
}

//////////////////////////////////////////////////////////////////////////
#define LOGPI(SINK, ...) _logPrintf(LOG_INST(SINK), (LOG_LEVEL_INFO),	__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGPD(SINK, ...) _logPrintf(LOG_INST(SINK), (LOG_LEVEL_DEBUG),	__FUNCTION__, __LINE__,	__VA_ARGS__)
#define LOGPW(SINK, ...) _logPrintf(LOG_INST(SINK), (LOG_LEVEL_WARNING),__FUNCTION__, __LINE__,	__VA_ARGS__)
#define LOGPE(SINK, ...) _logPrintf(LOG_INST(SINK), (LOG_LEVEL_ERROR),	__FUNCTION__, __LINE__,	__VA_ARGS__)
#define LOGPF(SINK, ...) _logPrintf(LOG_INST(SINK), (LOG_LEVEL_FATAL),	__FUNCTION__, __LINE__,	__VA_ARGS__)

#define LOGI(...) _logPrintf(LOG_INST(info),	(LOG_LEVEL_INFO),	__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOGD(...) _logPrintf(LOG_INST(debug),	(LOG_LEVEL_DEBUG),	__FUNCTION__, __LINE__,	__VA_ARGS__)
#define LOGW(...) _logPrintf(LOG_INST(warning), (LOG_LEVEL_WARNING),__FUNCTION__, __LINE__,	__VA_ARGS__)
#define LOGE(...) _logPrintf(LOG_INST(error),	(LOG_LEVEL_ERROR),	__FUNCTION__, __LINE__,	__VA_ARGS__)
#define LOGF(...) _logPrintf(LOG_INST(fatal),	(LOG_LEVEL_FATAL),	__FUNCTION__, __LINE__,	__VA_ARGS__)

//////////////////////////////////////////////////////////////////////////
LOG_DECL(info);
LOG_DECL(debug);
LOG_DECL(warning);
LOG_DECL(error);
LOG_DECL(fatal);
//////////////////////////////////////////////////////////////////////////
#ifdef LOG_UNIT_TEST
AUTOTEST_DEF(LogPrintWriteMulti);
AUTOTEST_DEF(LogPrintWriteExclucive);
#endif
//////////////////////////////////////////////////////////////////////////
#endif