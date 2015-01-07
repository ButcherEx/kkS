#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <PreCompier.h>
#include <Log/LogStream.h>
#if defined(HAVE_LIB_GFLAGS)
#include <gflags/gflags.h>
#endif

/*
* Level define
*/
#define LOG_LEVEL_INFO		(0x0000)
#define LOG_LEVEL_DEBUG		(0x0001)
#define LOG_LEVEL_WARNING	(0x0002)
#define LOG_LEVEL_ERROR		(0x0004)
#define LOG_LEVEL_FATAL		(0x0008)

#define NUM_SEVERITIES		(5)

#define LOG_FILE_OS			(0x0001)
#define LOG_FILE_PID		(0x0002)
#define LOG_FILE_DAY		(0x0004)
#define LOG_FILE_HOUR		(0x0008)
#define LOG_FILE_REALTIME	(0x0010)

/*
* ID define
*/
#ifndef LOG_MAX_ID 
#define LOG_MAX_ID 64
#endif

#define __STD_LOG	0
#define __INFO_LOG	1
#define __DEBUG_LOG	2
#define __WARN_LOG	3
#define __ERROR_LOG	4
#define __FATAL_LOG	5
#define __RSEV1_LOG	6
#define __RSEV2_LOG	7
#define __RSEV3_LOG	8
#define __LUA_LOG		9

#define LOG_RESERVED_ID_MAX 15

#ifndef LOG_SPLIT_MARK
#define LOG_SPLIT_MARK " "
#endif

#ifndef LOG_END_MARK
#define LOG_END_MARK "."
#endif

#define NEXT_LINE "\n"
#define NEXT_LINE_LEN (1)
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
const char* ProgramInvocationShortName();
void coloredWriteToStderr(int32_t severity,const char* message, size_t len);
//////////////////////////////////////////////////////////////////////////
#define LOG_FILE_BUFFER (16*1024)
#define LOG_FUSH_SIZE (8192)
class LogFile : boost::noncopyable
{
public:
	explicit LogFile( );
	~LogFile();
public:
	void open(const char* f, bool realTime = false);
	void open(const bfs::path& p, bool realTime = false);
	void append(int32_t level, const char* logline, const size_t len);
public:
	bool good() const 
	{
		if( fp_ != NULL )
		{
			return !ferror(fp_);
		}

		return false;
	}
	bool canFlush() const
	{
		return (good() && writtenBytes() > 0);
	}
	void forceFlush();
	size_t writtenBytes() const { return writtenBytes_; }
	int32_t fileFlag() const{ return fileFlag_; }
	void setFileFlag(int32_t flag){ fileFlag_ = flag; }
	void setId(int32_t id){ id_ = id; }
	bool isStdFile()const {return id_ == __STD_LOG; }
private:
	void _open(const char* f,bool realTime);
#ifdef BOOST_WINDOWS_API
	void _open(const wchar_t* f, bool realTime);
#endif
	void safeClose();
	void forceFlushUnlock();
private:
	Mutex lock_;
	FILE* fp_;
	char buffer_[LOG_FILE_BUFFER];
	bool realTimeFlag_;
	int32_t fileFlag_;
	size_t writtenBytes_;
	time_t lastflushTime_;
	int32_t id_;
};

//////////////////////////////////////////////////////////////////////////
class LogManager : public boost::noncopyable//: public boost::serialization::singleton<LogManager>
{
public:
	LogManager( ) {  }
	~LogManager() { flushall(); }
public:
	bool regLog(int32_t id, const bstd::string& fileName, int32_t fileFlag = LOG_FILE_HOUR, bool realTime = false);
	void genLogName(const bstd::string& fileName, bfs::path& p, int32_t fileFlag = LOG_FILE_HOUR);
	LogFile& logFile(int32_t id, int32_t level);
	const bstd::string& pid() { return pid_; }
	const bfs::path& logPath() { return logPath_; }
	const bfs::path& curPath() { return curPath_; }
public:
	//static LogManager& instance() { return LogManager::get_mutable_instance(); }
	bool initializeBeforeMain( );
	void forceFlush();
protected:
	void genLogPath();
	bool doRegLog(int32_t id, const bstd::string& fileName, int32_t fileFlag = LOG_FILE_HOUR, bool realTime = false);
public:
	bfs::path logPath_;
	bfs::path curPath_;
	LogFile logFiles_[LOG_MAX_ID];
	bstd::string pid_;
	Mutex lock_;
	static bstd::string sHostName;
};
#define sLogMgr Singleton<LogManager>::instance()

//////////////////////////////////////////////////////////////////////////
const char* __logLevel(int32_t level);

//////////////////////////////////////////////////////////////////////////
#define LOG_BUFFER_SIZE (4*1024)
void __logPrint(int32_t id, int32_t level,const char* funcline, const char* fmt, ...);

//////////////////////////////////////////////////////////////////////////

#define LOG_STREAM_TIME_FMT()\
	char now[DATETIME_LEN+8] = {0};\
	base::TimeUtil::format(now, DATETIME_LEN+8, "(%Y-%m-%d %H:%M:%S)");

template<typename P1>
void __logStream(int32_t id,  int32_t level, const P1& p1)
{
	base::LogStream stream;
	LOG_STREAM_TIME_FMT();
	stream << __logLevel(level) << p1 << LOG_SPLIT_MARK << now << NEXT_LINE;

	LogFile& rFile = sLogMgr.logFile(id, level);
	rFile.append( level, stream.buffer().data(), stream.buffer().length());
}

template<typename P1, typename P2>
void __logStream(int32_t id,  int32_t level, const P1& p1, const P2& p2)
{
	base::LogStream stream;
	
	LOG_STREAM_TIME_FMT();
	stream << __logLevel(level) << p1 << LOG_SPLIT_MARK << p2 << now << NEXT_LINE;

	LogFile& rFile = sLogMgr.logFile(id, level);
	rFile.append( level, stream.buffer().data(), stream.buffer().length());
}

template<typename P1, typename P2, typename P3>
void __logStream(int32_t id,  int32_t level, const P1& p1, const P2& p2, const P3& p3)
{
	base::LogStream stream;
	LOG_STREAM_TIME_FMT();
	stream  << __logLevel(level)  << p1 << LOG_SPLIT_MARK 
		<< p2 << p3 << now << NEXT_LINE;

	LogFile& rFile = sLogMgr.logFile(id, level);
	rFile.append( level, stream.buffer().data(), stream.buffer().length());
}


template<typename P1, typename P2, typename P3, typename P4>
void __logStream(int32_t id,  int32_t level, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
	base::LogStream stream;
	LOG_STREAM_TIME_FMT();
	stream << __logLevel(level) << p1 << LOG_SPLIT_MARK 
		<< p2<< p3 << p4 << now << NEXT_LINE;

	LogFile& rFile = sLogMgr.logFile(id, level);
	rFile.append( level, stream.buffer().data(), stream.buffer().length());
}

template<typename P1, typename P2, typename P3, typename P4, typename P5>
void __logStream(int32_t id,  int32_t level, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
{
	base::LogStream stream;
	LOG_STREAM_TIME_FMT();
	stream << __logLevel(level) << p1 << LOG_SPLIT_MARK 
		<< p2 << p3 << p4 << p5 << now << NEXT_LINE;

	LogFile& rFile = sLogMgr.logFile(id, level);
	rFile.append( level, stream.buffer().data(), stream.buffer().length());
}

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
void __logStream(int32_t id,  int32_t level, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6)
{
	base::LogStream stream;
	LOG_STREAM_TIME_FMT();
	stream << __logLevel(level) << p1  << LOG_SPLIT_MARK 
		<< p2 << p3 << p4 << p5 << p6 << now << NEXT_LINE;

	LogFile& rFile = sLogMgr.logFile(id, level);
	rFile.append( level, stream.buffer().data(), stream.buffer().length());
}

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
void __logStream(int32_t id,  int32_t level, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7)
{
	base::LogStream stream;
	LOG_STREAM_TIME_FMT();
	stream << __logLevel(level) << p1 << LOG_SPLIT_MARK 
		<< p2 << p3 << p4 << p5 << p6 << p7<< now << NEXT_LINE;

	LogFile& rFile = sLogMgr.logFile(id, level);
	rFile.append( level, stream.buffer().data(), stream.buffer().length());
}

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
void __logStream(int32_t id,  int32_t level, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8)
{
	base::LogStream stream;
	LOG_STREAM_TIME_FMT();
	stream << __logLevel(level) << p1 << LOG_SPLIT_MARK
		<< p2 << p3 << p4 << p5 << p6 << p7 << p8 << now << NEXT_LINE;

	LogFile& rFile = sLogMgr.logFile(id, level);
	rFile.append( level, stream.buffer().data(), stream.buffer().length());
}

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
void __logStream(int32_t id,  int32_t level, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9)
{
	base::LogStream stream;
	LOG_STREAM_TIME_FMT();
	stream << __logLevel(level) << p1 << LOG_SPLIT_MARK 
		<< p2 << p3 << p4 << p5 << p6 << p7 << p8 << p9 << now << NEXT_LINE;

	LogFile& rFile = sLogMgr.logFile(id, level);
	rFile.append( level, stream.buffer().data(), stream.buffer().length());
}

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
void __logStream(int32_t id,  int32_t level, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10)
{
	base::LogStream stream;
	LOG_STREAM_TIME_FMT();
	stream << __logLevel(level) << p1 << LOG_SPLIT_MARK
		<< p2  << p3 << p4 << p5 << p6 << p7 << p8 << p9 << p10 << now << NEXT_LINE;

	LogFile& rFile = sLogMgr.logFile(id, level);
	rFile.append( level, stream.buffer().data(), stream.buffer().length());
}

#define LOGSI(ID, ...) __logStream(ID, (LOG_LEVEL_INFO),	__FL__, __VA_ARGS__)
#define LOGSD(ID, ...) __logStream(ID, (LOG_LEVEL_DEBUG),	__FL__,	__VA_ARGS__)
#define LOGSW(ID, ...) __logStream(ID, (LOG_LEVEL_WARNING),	__FL__,	__VA_ARGS__)
#define LOGSE(ID, ...) __logStream(ID, (LOG_LEVEL_ERROR),	__FL__,	__VA_ARGS__)
#define LOGSF(ID, ...) __logStream(ID, (LOG_LEVEL_FATAL),	__FL__,	__VA_ARGS__)

#define LOGPI(ID, ...) __logPrint(ID, (LOG_LEVEL_INFO),		__FL__, __VA_ARGS__)
#define LOGPD(ID, ...) __logPrint(ID, (LOG_LEVEL_DEBUG),	__FL__,	__VA_ARGS__)
#define LOGPW(ID, ...) __logPrint(ID, (LOG_LEVEL_WARNING),	__FL__,	__VA_ARGS__)
#define LOGPE(ID, ...) __logPrint(ID, (LOG_LEVEL_ERROR),	__FL__,	__VA_ARGS__)
#define LOGPF(ID, ...) __logPrint(ID, (LOG_LEVEL_FATAL),	__FL__,	__VA_ARGS__)

#define LOGI LOGPI
#define LOGD LOGPD
#define LOGW LOGPW
#define LOGE LOGPE
#define LOGF LOGPF

#ifdef LOG_UNIT_TEST
AUTOTEST_DEF(LogStreamWriteMulti);
AUTOTEST_DEF(LogStreamWriteExclucive);
AUTOTEST_DEF(LogPrintWriteMulti);
AUTOTEST_DEF(LogPrintWriteExclucive);
#endif

#endif // LOG_MANAGER_H