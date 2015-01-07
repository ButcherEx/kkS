#include <Log/LogManager.h>

//////////////////////////////////////////////////////////////////////////
static const char* g_program_invocation_short_name = NULL;
void initLoggingUtilities(const char* argv0)
{
	const char* slash = strrchr(argv0, '/');
#if defined(__WINDOWS__)
	if (!slash)  slash = strrchr(argv0, '\\');
#endif
	g_program_invocation_short_name = slash ? slash + 1 : argv0;
}

const char* programInvocationShortName() 
{
	if (g_program_invocation_short_name != NULL) 
	{
		return g_program_invocation_short_name;
	} 
	else 
	{
		// TODO(hamaji): Use /proc/self/cmdline and so?
		return "UNKNOWN";
	}
}
//////////////////////////////////////////////////////////////////////////
#define LEVEL_STR_LEN (3)
const char* __logLevel(int32_t level)
{
	switch (level)
	{
	case LOG_LEVEL_INFO://(0x0000)
		return "[I]";
		break;
	case LOG_LEVEL_DEBUG://			(0x0001)
		return "[D]";
		break;
	case LOG_LEVEL_WARNING://		(0x0002)
		return "[W]";
		break;
	case LOG_LEVEL_ERROR://			(0x0004)
		return "[E]";
		break;
	case LOG_LEVEL_FATAL://			(0x0008)
		return "[F]";
		break;
	default:
		break;
	}
	return "[?]";
}
//////////////////////////////////////////////////////////////////////////
#if defined(HAVE_LIB_GFLAGS)
static bool BoolFromEnv(const char *varname, bool defval) {
	const char* const valstr = getenv(varname);
	if (!valstr) {
		return defval;
	}
	return memchr("tTyY1\0", valstr[0], 6) != NULL;
}

//DEFINE_bool(logtostderr, BoolFromEnv("GOOGLE_LOGTOSTDERR", false),
//				 "log messages go to stderr instead of logfiles");
DEFINE_bool(alsologtostderr, BoolFromEnv("GOOGLE_ALSOLOGTOSTDERR", true),
			"log messages go to stderr in addition to logfiles");
DEFINE_bool(colorlogtostderr, true,
			"color messages logged to stderr (if supported by terminal)");

// By default, errors (including fatal errors) get logged to stderr as
// well as the file.
//
// The default is ERROR instead of FATAL so that users can see problems
// when they run a program without having to look in another file.
DEFINE_int32(stderrthreshold,
			 LOG_LEVEL_DEBUG,
			 "log messages at or above this level are copied to stderr in "
			 "addition to logfiles.  This flag obsoletes --alsologtostderr.");
//DEFINE_bool(log_prefix, true,
//				 "Prepend the log prefix to the start of each log line");
//DEFINE_int32(minloglevel, 0, "Messages logged at a lower level than this don't "
//				  "actually get logged anywhere");
//DEFINE_int32(logbuflevel, 0,
//				  "Buffer log messages logged at this level or lower"
//				  " (-1 means don't buffer; 0 means buffer INFO only;"
//				  " ...)");
DEFINE_int32(logbufmsecs, 5000,
			 "Buffer log messages for at most this many milli-seconds");

// Compute the default value for --log_dir
static const char* DefaultLogDir() 
{
	const char* env;
	env = getenv("GOOGLE_LOG_DIR");
	if (env != NULL && env[0] != '\0') 
	{
		return env;
	}
	env = getenv("TEST_TMPDIR");
	if (env != NULL && env[0] != '\0') 
	{
		return env;
	}
#if defined(NDEBUG)
	return "log";
#else
	return "dbglog";
#endif
}
DEFINE_string(log_dir, DefaultLogDir(),
				   "If specified, logfiles are written into this directory instead "
				   "of the default logging directory.");
//DEFINE_int32(max_log_size, 1800,
//				  "approx. maximum log file size (in MB). A value of 0 will "
//				  "be silently overridden to 1.");
//DEFINE_bool(stop_logging_if_full_disk, false,
//				 "Stop attempting to log to disk if the disk is full.");

#endif

//
static void hostName(bstd::string& hn) 
{
#if defined(HAVE_SYS_UTSNAME_H)
	struct utsname buf;
	if (0 != uname(&buf)) {
		// ensure null termination on failure
		*buf.nodename = '\0';
	}
	hn = buf.nodename;
#elif defined(__WINDOWS__)
	char buf[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD len = MAX_COMPUTERNAME_LENGTH + 1;
	if (GetComputerNameA(buf, &len)) {
		hn = buf;
	} else {
		hn.clear();
	}
#else
# warning There is no way to retrieve the host name.
	hn = "(unknown)";
#endif
}

// Returns true iff terminal supports using colors in output.
static bool terminalSupportsColor() {
	bool term_supports_color = false;
#if defined(__WINDOWS__)
	// on Windows TERM variable is usually not set, but the console does
	// support colors.
	term_supports_color = true;
#else
	// On non-Windows platforms, we rely on the TERM variable.
	const char* const term = getenv("TERM");
	if (term != NULL && term[0] != '\0') {
		term_supports_color =
			!strcmp(term, "xterm") ||
			!strcmp(term, "xterm-color") ||
			!strcmp(term, "xterm-256color") ||
			!strcmp(term, "screen") ||
			!strcmp(term, "linux") ||
			!strcmp(term, "cygwin");
	}
#endif
	return term_supports_color;
}
enum GLogColor 
{
	COLOR_DEFAULT,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW
};

static GLogColor SeverityToColor(int32_t level) 
{
	GLogColor color = COLOR_DEFAULT;
	switch (level) {
	case LOG_LEVEL_INFO:
		color = COLOR_DEFAULT;
		break;
	case LOG_LEVEL_DEBUG:
		color = COLOR_GREEN;
		break;
	case LOG_LEVEL_WARNING:
		color = COLOR_YELLOW;
		break;
	case LOG_LEVEL_ERROR:
	case LOG_LEVEL_FATAL:
		color = COLOR_RED;
		break;
	default:
		// should never get here.
		assert(false);
	}
	return color;
}

#if defined(__WINDOWS__)

// Returns the character attribute for the given color.
WORD GetColorAttribute(GLogColor color) {
	switch (color) {
	case COLOR_RED:    return FOREGROUND_RED;
	case COLOR_GREEN:  return FOREGROUND_GREEN;
	case COLOR_YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN;
	default:           return 0;
	}
}

#else

// Returns the ANSI color code for the given color.
const char* GetAnsiColorCode(GLogColor color) {
	switch (color) {
	case COLOR_RED:     return "1";
	case COLOR_GREEN:   return "2";
	case COLOR_YELLOW:  return "3";
	case COLOR_DEFAULT:  return "";
	};
	return NULL; // stop warning about return type.
}

#endif  // __WINDOWS__

void coloredWriteToStderr(int32_t severity,
                                 const char* message, size_t len) {
  const GLogColor color =
      (terminalSupportsColor() && FLAGS_colorlogtostderr) ?
      SeverityToColor(severity) : COLOR_DEFAULT;

  // Avoid using cerr from this module since we may get called during
  // exit code, and cerr may be partially or fully destroyed by then.
  if (COLOR_DEFAULT == color) {
    fwrite(message, len, 1, stderr);
    return;
  }
#if defined(__WINDOWS__)
  const HANDLE stderr_handle = GetStdHandle(STD_ERROR_HANDLE);

  // Gets the current text color.
  CONSOLE_SCREEN_BUFFER_INFO buffer_info;
  GetConsoleScreenBufferInfo(stderr_handle, &buffer_info);
  const WORD old_color_attrs = buffer_info.wAttributes;

  // We need to flush the stream buffers into the console before each
  // SetConsoleTextAttribute call lest it affect the text that is already
  // printed but has not yet reached the console.
  fflush(stderr);
  SetConsoleTextAttribute(stderr_handle,
                          GetColorAttribute(color) | FOREGROUND_INTENSITY);
  fwrite(message, len, 1, stderr);
  fflush(stderr);
  // Restores the text color.
  SetConsoleTextAttribute(stderr_handle, old_color_attrs);
#else
  fprintf(stderr, "\033[0;3%sm", GetAnsiColorCode(color));
  fwrite(message, len, 1, stderr);
  fprintf(stderr, "\033[m");  // Resets the terminal to default.
#endif  // __WINDOWS__
}
//////////////////////////////////////////////////////////////////////////
LogFile::LogFile( )
:fp_(NULL)
,writtenBytes_(0)
,realTimeFlag_(false)
,fileFlag_(LOG_FILE_HOUR)
,id_(-1)
{
	lastflushTime_ = time(NULL);
}

LogFile::~LogFile()
{
	safeClose();
}
void LogFile::open(const char* f, bool realTime)
{
	ScopedLock l(lock_);
	_open(f, realTime);
	lastflushTime_ = time(NULL);
}

void LogFile::open(const bfs::path& p, bool realTime)
{
	ScopedLock l(lock_);
	_open(p.BOOST_FILESYSTEM_C_STR, realTime);
}

void LogFile::append(int32_t level, const char* logline, const size_t len)
{
	ScopedLock l(lock_);
	if(good())
	{
		size_t n = ::fwrite(logline, len, 1, fp_);
		if( n == 1 ) writtenBytes_ += len;
		if(realTimeFlag_ || writtenBytes_ > LOG_FUSH_SIZE
#if defined(HAVE_LIB_GFLAGS)
			|| FLAGS_logbufmsecs <= (time(NULL) - lastflushTime_)
#endif
		) 
		{
			forceFlushUnlock();
		}
	}
#if defined(HAVE_LIB_GFLAGS)
	if(FLAGS_alsologtostderr || isStdFile() )
	{
		if(FLAGS_stderrthreshold <= level || isStdFile() )
		{
			coloredWriteToStderr(level, logline, len);
		}
	}
#endif
}
void LogFile::forceFlush()
{
	ScopedLock l(lock_);
	forceFlushUnlock();
}

void LogFile::forceFlushUnlock()
{
	if(canFlush()) 
	{
		::fflush(fp_);
		lastflushTime_ = time(NULL);
		writtenBytes_ = 0;
	}
}
void LogFile::_open(const char* f,bool realTime)
{
	safeClose();
	fp_ = ::fopen(f, "a+");
	if( !fp_ )
	{
		LOGPE(__ERROR_LOG, "open log file failed:%s %s.", static_cast<const char*>((void*)f), strerror(errno));
	}
	_Assert(fp_ != NULL, f);
	realTimeFlag_ = realTime;
#if defined(__WINDOWS__)
	::setbuf(fp_, buffer_);
#else
	::setbuffer(fp_, buffer_, LOG_FILE_BUFFER);
#endif
}

#ifdef BOOST_WINDOWS_API
void LogFile::_open(const wchar_t* f, bool realTime)
{
	safeClose();
	fp_ = ::_wfopen(f, L"a+");
	if( !fp_ )
	{
		std::string asciif;
		bfs::path_traits::convert(f, asciif, bfs::path::codecvt());
		LOGPE(__ERROR_LOG, "open log file failed:%s %s.", asciif.c_str(), strerror(errno));
		_Verify(fp_ != NULL, asciif.c_str());
	}

	realTimeFlag_ = realTime;
	::setbuf(fp_, buffer_);
}
#endif

void LogFile::safeClose()
{
	if( fp_ != NULL)
	{
		::fflush(fp_);
		::fclose(fp_);
		fp_ = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
bstd::string LogManager::sHostName = "(unknown)";
bool LogManager::initializeBeforeMain( )
{
	//对当前的目录的操作  
	char pid[32] = {0};
	snprintf(pid, 32, "%d", getpid());

	pid_ = pid; 
	_MY_TRY
	{
		bsys::error_code ec;
		curPath_ = bfs::current_path(ec); //取得当前目录  
		_Assert(!ec, ec.message().c_str());

		hostName(LogManager::sHostName);
		genLogPath();
		//////////////////////////////////////////////////////////////////////////
		logFiles_[__STD_LOG].setId(__STD_LOG);
		//////////////////////////////////////////////////////////////////////////
		doRegLog(__INFO_LOG,	"info",		LOG_FILE_PID);
		doRegLog(__DEBUG_LOG, "debug",	LOG_FILE_PID);
		doRegLog(__WARN_LOG,	"warning",	LOG_FILE_PID);
		doRegLog(__ERROR_LOG, "error",	LOG_FILE_PID);
		doRegLog(__FATAL_LOG, "fatal",	LOG_FILE_PID);
		doRegLog(__LUA_LOG,	"lua",		LOG_FILE_PID);
		return true;
	}
	_MY_CATCH
	{
		return false;
	}

	return false;
}
void LogManager::genLogPath()
{
	__ENTER_FUNCTION
	{
#if defined(HAVE_LIB_GFLAGS)
		logPath_ = curPath_ / FLAGS_log_dir.c_str();
#else
		logPath_ = curPath_ / "runtime_log";
#endif
		bsys::error_code ec;
		if( !bfs::exists(logPath_, ec) )
		{
			boost::system::error_code ec;
			bfs::create_directories(logPath_, ec);
			_Assert(!ec, ec.message().c_str());
		}
	}
	__LEAVE_FUNCTION
}
bool LogManager::regLog(int32_t id, const bstd::string& fileName, int32_t fileFlag, bool realTime)
{
	if( id >= LOG_RESERVED_ID_MAX && id < LOG_MAX_ID)
	{
		return doRegLog(id, fileName, fileFlag, realTime);
	}
	else
	{

	}

	return false;
}
bool LogManager::doRegLog(int32_t id, const bstd::string& fileName, int32_t fileFlag, bool realTime)
{
	ScopedLock l(lock_);

	bfs::path log;
	genLogName(fileName, log, fileFlag);
	logFiles_[id].open(log, realTime);
	logFiles_[id].setFileFlag(fileFlag);
	logFiles_[id].setId(id);
	return logFiles_[id].good();
}
void LogManager::genLogName(const bstd::string& fileName, bfs::path& p, int32_t fileFlag)
{
#define FILE_NAME_LEN (128)
	char logfileName[FILE_NAME_LEN] = {0};
	char logsuffix[DATETIME_LEN] = {0};
	
	if(fileFlag&LOG_FILE_OS)
	{
		snprintf(logfileName, FILE_NAME_LEN, "%s.%s.log", 
			fileName.c_str(), programInvocationShortName());
	}
	else if(fileFlag&LOG_FILE_PID)
	{
		snprintf(logfileName, FILE_NAME_LEN, "%s.%s.%s.log", 
			fileName.c_str(), programInvocationShortName(), pid_.c_str());
	}
	else if(fileFlag&LOG_FILE_DAY)
	{
		base::TimeUtil::format(logsuffix, DATETIME_LEN, "%Y%m%d");
		snprintf(logfileName, FILE_NAME_LEN, "%s.%s.%s.log",
			fileName.c_str(), programInvocationShortName(), logsuffix);
	}
	else if(fileFlag&LOG_FILE_HOUR)
	{
		base::TimeUtil::format(logsuffix, DATETIME_LEN, "%Y%m%d%H");
		snprintf(logfileName, FILE_NAME_LEN, "%s.%s.%s-%s.log",  
			fileName.c_str(), programInvocationShortName(), logsuffix, pid_.c_str());
	}
	else
	{
		base::TimeUtil::format(logsuffix, DATETIME_LEN, "%Y%m%d-%H%M%S");
		snprintf(logfileName, FILE_NAME_LEN, "%s.%s.%s-%s.log",  
			fileName.c_str(), programInvocationShortName(), logsuffix, pid_.c_str());
	}


	p = logPath_ ;
	p /= logfileName;
}
LogFile& LogManager::logFile(int32_t id, int32_t level)
{
	if( id >= 0 && id < LOG_MAX_ID)
	{
		if( logFiles_[id].good() || logFiles_[id].isStdFile() ) 
			return logFiles_[id];
	}

	id = __INFO_LOG;
	switch (level)
	{ 
	case LOG_LEVEL_DEBUG://		(0x0001)
		id = __DEBUG_LOG;
		break;
	case LOG_LEVEL_WARNING://	(0x0002)
		id = __WARN_LOG;
		break;
	case LOG_LEVEL_ERROR://		(0x0004)
		id = __ERROR_LOG;
		break;
	case LOG_LEVEL_FATAL://		(0x0008)
		id = __FATAL_LOG;
		break;
	}
	return logFiles_[id];
}
void LogManager::forceFlush()
{
	_MY_TRY
	{
		for(int32_t i = 0; i<LOG_MAX_ID; i++)
		{
			if(logFiles_[i].canFlush()) logFiles_[i].forceFlush();
		}
	}
	_MY_CATCH
	{

	}
	
}
//////////////////////////////////////////////////////////////////////////
void __logPrint(int32_t id, int32_t level, const char* funcline, const char* fmt, ...)
{
	_MY_TRY
	{
		int32_t fillBytes = 0;
		char msg[LOG_BUFFER_SIZE] = {0};
		fillBytes = snprintf(msg, LOG_BUFFER_SIZE, "%s%s ", __logLevel(level), funcline);

		if( fillBytes < LOG_BUFFER_SIZE && fillBytes > 0)
		{
			va_list ap;
			va_start(ap,fmt);
			int32_t bytes = (int32_t)vsnprintf(msg+fillBytes, LOG_BUFFER_SIZE - fillBytes, fmt, ap);
			va_end(ap);
			if(bytes > 0 ) fillBytes += bytes;
		}

		if( fillBytes > 0 )
		{
			char now[DATETIME_LEN] = {0};
			base::TimeUtil::format(now, DATETIME_LEN, "%Y-%m-%d %H:%M:%S");

			if( fillBytes < LOG_BUFFER_SIZE )
			{
				int32_t bytes = snprintf(msg+fillBytes, LOG_BUFFER_SIZE - fillBytes,
					"(%s)\n", now);
				if( bytes > 0) fillBytes += bytes;
			}

			if(fillBytes > 0)
			{
				if(fillBytes < LOG_BUFFER_SIZE) msg[fillBytes] = 0;
				LogFile& rFile = sLogMgr.logFile(id, level);
				rFile.append(level, msg, fillBytes);
			}
		}
	}
	_MY_CATCH
	{
		char errorMsg[256]={0};
		LOGPE(__ERROR_LOG, "logPrint exception:%d %s", id, fmt);
		snprintf(errorMsg, 256, 
			"\n\\********%s logid=%d,fmt=%s********/\n", __FUNCTION__, id, fmt);
		coloredWriteToStderr(LOG_LEVEL_FATAL, errorMsg, 256);
	}
	
}
//////////////////////////////////////////////////////////////////////////
#ifdef LOG_UNIT_TEST
//////////////////////////////////////////////////////////////////////////
#define UTEST_LOG_MAX_WRITES (50*10000)
void logWriterMf(int32_t id)
{
	boost::timer t;
	for(int32_t i = 0; i < UTEST_LOG_MAX_WRITES; i++)
		LOGSI(id, *reinterpret_cast<uint32_t*>(&boost::this_thread::get_id()), " TYUVDF");
	LOGSI(__INFO_LOG, *reinterpret_cast<uint32_t*>(&boost::this_thread::get_id()), " time elapse(sec):", t.elapsed() );
}

void logWriterMP(int32_t id)
{
	boost::timer t;
	for(int32_t i = 0; i < UTEST_LOG_MAX_WRITES; i++)
		LOGI(id, "%d %s", boost::this_thread::get_id(), "TYUVDF");
	LOGSI(__INFO_LOG, *reinterpret_cast<uint32_t*>(&boost::this_thread::get_id()), "time elapse(sec):", t.elapsed() );
}

#define SHARE_LOG (16)

void logWriterMTShare(void*)
{
	sLogMgr.regLog(SHARE_LOG, "ShareWriter");

	boost::timer t;
	ThreadPool pool(5);
	for(int32_t i = 0; i < 10; i++)
		pool.schedule(boost::bind(logWriterMf, SHARE_LOG));
	pool.wait( );
	double elapse = t.elapsed();
	LOGSI(__INFO_LOG," Total Elapse:", elapse, ",opps:", 10*(UTEST_LOG_MAX_WRITES/elapse));
}

void logWriterMTExclucive(void*)
{
	for(int32_t i = 4; i < 14; i++)
	{
		char f[32] = {0};
		snprintf(f, 32, "%d-Exclusive", SHARE_LOG+i);
		sLogMgr.regLog(SHARE_LOG+i, f);
	}
	boost::timer t;
	ThreadPool pool(10);
	for(int32_t i = 4; i < 14; i++)
		pool.schedule(boost::bind(logWriterMf, SHARE_LOG+i));
	pool.wait( );
	double elapse = t.elapsed();
	LOGSI(__INFO_LOG," Total Elapse:", elapse, ",opps:", 10*(UTEST_LOG_MAX_WRITES/elapse));
}

void logWriterMTSharePrint(void*)
{
	sLogMgr.regLog(SHARE_LOG, "ShareWriterPrint");
	boost::timer t;
	ThreadPool pool(5);
	for(int32_t i = 0; i < 10; i++)
		pool.schedule(boost::bind(logWriterMP, SHARE_LOG));
	pool.wait( );
	double elapse = t.elapsed();
	LOGSI(__INFO_LOG," Total Elapse:", elapse, ",opps:", 10*(UTEST_LOG_MAX_WRITES/elapse));
}

void logWriterMTSharePrintExclucive(void*)
{
	for(int32_t i = 4; i < 14; i++)
	{
		char f[32] = {0};
		snprintf(f, 32, "%d-PrintExclusive", SHARE_LOG+i);
		sLogMgr.regLog(SHARE_LOG+i, f);
	}
	boost::timer t;
	ThreadPool pool(10);
	for(int32_t i = 4; i < 14; i++)
		pool.schedule(boost::bind(logWriterMP, SHARE_LOG+i));
	pool.wait( );
	double elapse = t.elapsed();
	LOGSI(__INFO_LOG," Total Elapse:", elapse, ",opps:", 10*(UTEST_LOG_MAX_WRITES/elapse));
}


AUTOTEST_IMP(LogStreamWriteMulti, logWriterMTShare);
AUTOTEST_IMP(LogStreamWriteExclucive, logWriterMTExclucive);
AUTOTEST_IMP(LogPrintWriteMulti, logWriterMTSharePrint);
AUTOTEST_IMP(LogPrintWriteExclucive, logWriterMTSharePrintExclucive);

#endif