#include <PreCompier.h>
#include <Exception.h>

//////////////////////////////////////////////////////////////////////////
#if defined(__LINUX__)
#include <Symbol/LinuxStackTrace.h>
#elif defined(__WINDOWS__)
#include <Symbol/SimpleSymbolEngine.h>
#include <Symbol/SimpleSymbolEngine.cpp>
#else
#error windows or linux required.
#endif

//////////////////////////////////////////////////////////////////////////
void stackTrace(std::ostream& os)
{
#if defined(__LINUX__)
	debug::stackTrace(os);
#elif defined(__WINDOWS__)
	CONTEXT context = {CONTEXT_FULL};
	::GetThreadContext( GetCurrentThread(), &context );
	SimpleSymbolEngine::instance().StackTrace( &context, os );
#else
#error windows or linux required.
#endif
}
//////////////////////////////////////////////////////////////////////////
static Mutex btDumpMutex;
static void dumpStack(const char* msg, bool throwException)
{
	try
	{
		ScopedLock lock(btDumpMutex);

#define FILE_NAME_LEN (128)
		char logfileName[FILE_NAME_LEN] = {0};
		char curTime[DATETIME_LEN] = {0};

		base::TimeUtil::format(curTime, DATETIME_LEN, "%Y%m%d-%H%M%S");
		bfs::path p = sLogConfigure.baseDir();
		snprintf(logfileName, FILE_NAME_LEN, "%s.%d.%s.%s.log",  
			sLogConfigure.invocationShortName().c_str(),
			getpid(), "dump", curTime);

		p /= logfileName;
#undef FILE_NAME_LEN

		bfs::fstream ss(p, std::ios_base::app);
		ss << msg;
		stackTrace(ss);
		ss.close();

		int32_t level = throwException ? LOG_LEVEL_FATAL : LOG_LEVEL_ERROR;
		if( sLogConfigure.isLogToStderr(level) )
		{
			coloredWriteToStderr(level, msg, strlen(msg));
			coloredWriteToStderr(level, "\n",strlen("\n"));
		}
	}
	catch(...)
	{

	}

	if(throwException) throw 1;
}
//////////////////////////////////////////////////////////////////////////
static void defaultSiganlHandler(int sig)
{
	try
	{
		dumpStack("SignalException", false);
	}
	catch(...)
	{

	}
}
//////////////////////////////////////////////////////////////////////////

void ExceptionHanlder::initialize( )
{

#if defined(__LINUX__)
	signal(SIGINT, defaultSiganlHandler);
	signal(SIGILL, defaultSiganlHandler);
	signal(SIGSEGV, defaultSiganlHandler);
	signal(SIGABRT, defaultSiganlHandler);
	signal(SIGHUP, defaultSiganlHandler);
	signal(SIGQUIT, defaultSiganlHandler);
	signal(SIGBUS, defaultSiganlHandler);
	signal(SIGKILL, SIG_IGN);
#endif
	signal(SIGTERM, defaultSiganlHandler);
	signal(SIGBREAK, defaultSiganlHandler);
}



//////////////////////////////////////////////////////////////////////////
#define ASSERT_BUF_LEN 10240
static Mutex _asertlogMutex;
void __error_assert(const char *file, const char *func, 
					int line, const char *exp, bool throwException)
{
	ScopedLock __lockThis(_asertlogMutex);
	static char assertBuf[ASSERT_BUF_LEN];

	try
	{
		base::TIME64 now = base::TimeUtil::now();
		//////////////////////////////////////////////////////////////////////////
		zeroMemory(assertBuf, ASSERT_BUF_LEN);
		snprintf(assertBuf, ASSERT_BUF_LEN, 
			"[%s][%s][%s][%d]%s(%04d-%02d-%02d %02d:%02d:%02d).",
			throwException ? "Assert" : "Verify",
			file, func, line, exp, now.year + 1900, 
			now.mon, now.day, now.hour, now.min, now.sec);

		//////////////////////////////////////////////////////////////////////////
#define FILE_NAME_LEN 128
		char fileName[FILE_NAME_LEN] = {0};
		snprintf(fileName, FILE_NAME_LEN, "%s%s.%04d-%02d-%02d-%02d.log",  
			sLogConfigure.filePrefix().c_str(), "assert",
			now.year + 1900, now.mon, now.day, now.hour);
#undef FILE_NAME_LEN

		bfs::path p = sLogConfigure.baseDir();
		p /= fileName;

		//////////////////////////////////////////////////////////////////////////
		bfs::fstream assertfs(p, std::ios_base::app);
		assertfs << assertBuf;
		assertfs << "\n";
		assertfs.close();
#if 0
		printf("%s:%s\n", 
			throwException ? "Assert" : "Verify", assertBuf);
#else
		int32_t level = throwException ? LOG_LEVEL_FATAL : LOG_LEVEL_ERROR;
		if( sLogConfigure.isLogToStderr(level) )
		{
			strcat(assertBuf, "\n");
			coloredWriteToStderr(level, assertBuf, ::strlen(assertBuf));
		}
#endif
	} 
	catch(...)
	{

	}
	if(throwException) throw 1;
}
//////////////////////////////////////////////////////////////////////////
#ifdef EXCEPTION_UNIT_TEST
void pointerError()
{
	__ENTER_FUNCTION_EX

	if(1)
	{
		int* i = znew int;
		zdeleter(i);
		*i = 0;
	}

	if(1)
	{
		int* p = 0;
		*p = 0;
	}

	if(1)
	{
		int* i = (int*)0xcccccccc;
		*i = 0;
	}

	__LEAVE_FUNCTION_EX
}
void overwriteArrayIndex()
{
	__ENTER_FUNCTION_EX

	int32_t intArray[2];
	for( int32_t i = 0; i < 4; i++)
	{
		_Assert(i<2,"out of range");
		intArray[i] = INT32_MAX;
	}

	__LEAVE_FUNCTION_EX
}
void divZero()
{
	__ENTER_FUNCTION_EX
	int i = 1;
	i -= 1;
	int k = 1 / i;
	__LEAVE_FUNCTION_EX
}
void _throwBase()
{
	__ENTER_FUNCTION
	{
		_Assert(false, "call function!");
	}
	__LEAVE_FUNCTION
}
void _callfunc()
{
	__ENTER_FUNCTION
	{
		_throwBase();
	}
	__LEAVE_FUNCTION
}
void callStack()
{
	__ENTER_FUNCTION_EX
	{
		_callfunc();
	}
	__LEAVE_FUNCTION_EX
}
void crashFunction(void* p)
{
	__ENTER_FUNCTION_EX

	pointerError();

	overwriteArrayIndex();

	divZero();

	divZero();

	callStack();

	__LEAVE_FUNCTION_EX

}

AUTOTEST_IMP(exceptionUnitTest, crashFunction);
#endif


