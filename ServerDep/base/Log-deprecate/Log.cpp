#include "Log.h"
//////////////////////////////////////////////////////////////////////////
#define LEVEL_STR_LEN (3)
const char* logLevelDesc(int32_t level)
{
	switch (level)
	{
	case LOG_LEVEL_INFO: 		
		return "[I]";
		break;
	case LOG_LEVEL_DEBUG:
		return "[D]";
		break;
	case LOG_LEVEL_WARNING:
		return "[W]";
		break;
	case LOG_LEVEL_ERROR:
		return "[E]";
		break;
	case LOG_LEVEL_FATAL:
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

const char* logDir()
{
#if defined(HAVE_LIB_GFLAGS)
	return FLAGS_log_dir.c_str();
#else
	return "log"
#endif
}

//
void hostName(bstd::string& hn) 
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
//////////////////////////////////////////////////////////////////////////
static const char* g_program_invocation_short_name = NULL;
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
void initLoggingUtilities(const char* argv0)
{
	const char* slash = strrchr(argv0, '/');
#if defined(__WINDOWS__)
	if (!slash)  slash = strrchr(argv0, '\\');
#endif
	g_program_invocation_short_name = slash ? slash + 1 : argv0;
}

//////////////////////////////////////////////////////////////////////////