#include "Log.h"
//////////////////////////////////////////////////////////////////////////
const char* logLevelDesc(int32_t level)
{
	switch (level)
	{
	case LOG_LEVEL_INFO: 		
		return "I";
		break;
	case LOG_LEVEL_DEBUG:
		return "D";
		break;
	case LOG_LEVEL_WARNING:
		return "W";
		break;
	case LOG_LEVEL_ERROR:
		return "E";
		break;
	case LOG_LEVEL_FATAL:
		return "F";
		break;
	default:
		break;
	}
	return "?";
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

const char* logDir()
{
#if defined(HAVE_LIB_GFLAGS)
	return FLAGS_log_dir.c_str();
#else
	return "log"
#endif
}

static void HostName(bstd::string& hn) 
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
//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
void coloredWriteToStderr(int32_t severity, const char* message, size_t len) {
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
void clearStderrLineBuffer(FILE *file)
{
#if defined(__WINDOWS__)
	HANDLE stdIOHandle;
	COORD coor;
	CONSOLE_SCREEN_BUFFER_INFO info;

	stdIOHandle = (HANDLE)_get_osfhandle(fileno(file));
	if(stdIOHandle == INVALID_HANDLE_VALUE)
		return ;

	if(!GetConsoleScreenBufferInfo(stdIOHandle,&info))
		return ;

	coor = info.dwCursorPosition;
	if(coor.Y <= 0) return ;

	coor.X = 0;
	SetConsoleCursorPosition(stdIOHandle, coor);
#else
	fprintf(file, "\033[0;0H");
	fprintf(file, "\033[K");
#endif
}
//////////////////////////////////////////////////////////////////////////
LogConfigure::LogConfigure()
{
	bsys::error_code ec;
	basePath_ = bfs::current_path(ec); //取得当前目录  
	_Verify(!ec, ec.message().c_str());
}
void LogConfigure::initialize(const char* argv0, const char* directory, 
				bool prefixPid, bool prefixInvocation, bool prefixHost)
{
	//////////////////////////////////////////////////////////////////////////
	if( argv0 != NULL)
	{
		const char* slash = strrchr(argv0, '/');
#if defined(__WINDOWS__)
		if (!slash)  slash = strrchr(argv0, '\\');
#endif
		invocationName_ = slash ? slash + 1 : argv0;
	}
	else
	{
		invocationName_ = "UNKNOWN";
	}

	//////////////////////////////////////////////////////////////////////////
	if(directory != NULL) basePath_ /= directory;

	bsys::error_code ec;
	if( !bfs::exists(basePath_, ec) )
	{
		ec.clear();
		bfs::create_directories(basePath_, ec);
		_Assert(!ec, ec.message().c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	HostName(hostName_);

	//////////////////////////////////////////////////////////////////////////
	if(prefixPid)
	{
		char pid[32] = {0};
		snprintf(pid, 32, "%d.", getpid());
		filePrefix_ += pid;
	}

	if(prefixInvocation)
	{
		filePrefix_ += invocationName_;
		filePrefix_ += ".";
	}

	if(prefixHost)
	{
		filePrefix_ += hostName_;
		filePrefix_ += ".";
	}
}

bool LogConfigure::isLogToStderr(int32_t level)
{
#if defined(HAVE_LIB_GFLAGS)
	if(FLAGS_alsologtostderr)
	{
		if(level >= FLAGS_stderrthreshold)
		{
			return true;
		}
	}
#endif
	return (level >= LOG_LEVEL_ERROR);
}
//////////////////////////////////////////////////////////////////////////
#ifdef LOG_UNIT_TEST
LOG_DECL(ShareLog);
LOG_DECL(Exclucive1);
LOG_DECL(Exclucive2);
LOG_DECL(Exclucive3);
LOG_DECL(Exclucive4);
LOG_DECL(Exclucive5);
//////////////////////////////////////////////////////////////////////////
#define UTEST_LOG_MAX_WRITES (50*10000)
#define UTEST_LOG_POOL_THREAD_NUM (5)
#define UTEST_LOG_WORK_THREAD_NUM (5)
void ShareLogPrintf()
{
	boost::timer t;
	for(int32_t i = 0; i < UTEST_LOG_MAX_WRITES; i++)
		LOGPI(ShareLog, "%d %s", boost::this_thread::get_id(), "TYUVDF");
	LOGD("Share thread id=%u,time elapse(sec):%f",
		*static_cast<uint32_t*>((void*)&boost::this_thread::get_id()), t.elapsed() );
}

void ShareWriter(void*)
{
	boost::timer t;
	ThreadPool pool(UTEST_LOG_POOL_THREAD_NUM);
	for(int32_t i = 0; i < UTEST_LOG_WORK_THREAD_NUM; i++)
		pool.schedule(boost::bind(ShareLogPrintf));
	pool.wait( );
	double elapse = t.elapsed();
	LOGD("Share Total Elapse:%f,opps:%f", elapse, UTEST_LOG_WORK_THREAD_NUM*(UTEST_LOG_MAX_WRITES/elapse));
}


#define ExcluciveFunctionDef(LOGDECL)\
void LOGDECL##Printf()\
{\
	boost::timer t;\
	for(int32_t i = 0; i < UTEST_LOG_MAX_WRITES; i++)\
		LOGPI(LOGDECL, "%d %s", boost::this_thread::get_id(), "TYUVDF");\
	LOGD("Exclucive ,thread id=%u,time elapse(sec):%f",\
		*static_cast<uint32_t*>((void*)&boost::this_thread::get_id()), t.elapsed() );\
}

ExcluciveFunctionDef(Exclucive1)
ExcluciveFunctionDef(Exclucive2)
ExcluciveFunctionDef(Exclucive3)
ExcluciveFunctionDef(Exclucive4)
ExcluciveFunctionDef(Exclucive5)

#define POOL_SCHEDULE(POOL,LOGDECL)\
	POOL.schedule(boost::bind(LOGDECL##Printf));

void ExcluciveWriter(void*)
{
	boost::timer t;
	ThreadPool pool(UTEST_LOG_POOL_THREAD_NUM);
	
	POOL_SCHEDULE(pool, Exclucive1);
	POOL_SCHEDULE(pool, Exclucive2);
	POOL_SCHEDULE(pool, Exclucive3);
	POOL_SCHEDULE(pool, Exclucive4);
	POOL_SCHEDULE(pool, Exclucive5);

	pool.wait( );
	double elapse = t.elapsed();
	LOGD("Exclucive Total Elapse:%f,opps:%f", elapse, UTEST_LOG_WORK_THREAD_NUM*(UTEST_LOG_MAX_WRITES/elapse));
}

AUTOTEST_IMP(LogPrintWriteMulti, ShareWriter);
AUTOTEST_IMP(LogPrintWriteExclucive, ExcluciveWriter);

#endif
//////////////////////////////////////////////////////////////////////////