#include <LClient.h>
#include <LServer.h>
#if defined(HAS_GFLAGS)
#include <gflags/gflags.h>

#endif
#include <Test.cpp>
void uCall( )
{
	boost_ini();
	boost_xml();
	container_test();
#ifdef LOG_UNIT_TEST
	AUTOTEST_RUN(LogPrintWriteMulti,NULL);
	AUTOTEST_RUN(LogPrintWriteExclucive,NULL);
#endif

#ifdef SCRIPT_UNIT_TEST
	AUTOTEST_RUN(luaUnitTest, "./lua.lua");
#endif // SCRIPT_UNIT_TEST

#ifdef TIME_UNIT_TEST
	AUTOTEST_RUN(timeUitlUnitTest, NULL);
#endif

#ifdef EXCEPTION_UNIT_TEST
	AUTOTEST_RUN(exceptionUnitTest, NULL);
#endif
}

#if defined(HAVE_LIB_GFLAGS)
DEFINE_bool(isserver, false, "run in server mode.");
#endif 
//
//using namespace zsummer::log4z;
////! multi logger id
//LoggerId g_lgMySql;
//LoggerId g_lgNet;
//LoggerId g_lgMoniter;
//
//#define  LOG_CONTENT_LINUXFMT "char:%c, unsigned char:%u, short:%d, unsigned short:%u, int:%d, unsigned int:%u, long long:%lld, unsigned long long:%llu, \
//											float:%f, double:%lf, string:%s, void*:%x, const void*:%x, constant:%d, constant:%lf, bool:%d", \
//											'c', (unsigned char) 'c', (short)-1, (unsigned short)-1, \
//											(int)-1, (unsigned int)-1, (long long)-1, (unsigned long long) - 1, \
//											(float)-1.234567, (double)-2.34566, "fffff", \
//											32423324, 234, 1000, 100.12345678, true
//
//void log4zTest()
//{
//	//! ---------
//	g_lgMySql = ILog4zManager::GetInstance()->CreateLogger("MySql" );
//	g_lgNet = ILog4zManager::GetInstance()->CreateLogger("NetWork" );
//	g_lgMoniter = ILog4zManager::GetInstance()->CreateLogger("Monitor" );
//	ILog4zManager::GetInstance()->Config("config.cfg");
//	//! ---------
//	ILog4zManager::GetInstance()->SetLoggerDisplay(g_lgMySql, false);
//	ILog4zManager::GetInstance()->SetLoggerDisplay(g_lgNet, false);
//	ILog4zManager::GetInstance()->SetLoggerDisplay(g_lgMoniter, false);
//
//	//! ---------
//	ILog4zManager::GetInstance()->Start();
//	
//	LOG_ERROR(g_lgMySql, LOG_CONTENT_LINUXFMT);
//	LOG_FATAL(g_lgNet, LOG_CONTENT_LINUXFMT);
//	LOG_WARN(g_lgMoniter, LOG_CONTENT_LINUXFMT);
//	LOG_WARN(LOG4Z_MAIN_LOGGER_ID, LOG_CONTENT_LINUXFMT);
//	
//
//}

LOG_DECL(pingpong);

int main(int argn, char** argv)
{
	__ENTER_FUNCTION
	{

		base::TimeInfo info;

		printf("ParseCommandLineFlags...\n");
		google::ParseCommandLineFlags(&argn, &argv, true);
		printf("ParseCommandLineFlags done.\n");
		printf("Singleton<ExceptionHanlder>::instance().initialize()...\n");
		Singleton<ExceptionHanlder>::instance().initialize();
		printf("Singleton<ExceptionHanlder>::instance().initialize() done.\n");

		printf("Singleton<LogConfigure>::instance().initialize(argv[0])...\n");
		Singleton<LogConfigure>::instance().initialize(argv[0]);
		printf("Singleton<LogConfigure>::instance().initialize(argv[0])done.\n");
		info.update();

		LOGI("Info Printf test.");
		LOGD("Debug Printf test.");
		LOGW("Warning Printf test.");
		LOGE("Error Printf test!");
		LOGF("Fatal Printf test!");

		uCall();

		bsys::error_code ec;
		basio::ip::tcp::endpoint addr(basio::ip::address::from_string("127.0.0.1", ec), 2251);

#define PINGDONG_MS (50*1000)
		boost::timer t;
#if defined(HAVE_LIB_GFLAGS)
		if(!FLAGS_isserver)
#else
		if(argn < 2)
#endif
		{
			LOGD("%s", "running in client mode. starting connecting.");
			NetworkConfig config;
			config.ioFlag = MSG_FLAG_COMPRESS;
			LClient lc(addr, config);
			lc.start();
			LOGD("%s", "client is started.");
			base::thisThreadSleep( PINGDONG_MS );
			lc.stop( );
		}
		else
		{
			LOGD("%s", "running in server mode.");
			NetworkConfig config;
			config.ioFlag = MSG_FLAG_COMPRESS;
			LServer ls(addr, config);
			ls.start();
			LOGD("%s", "server is started.start accept connection.");
			base::thisThreadSleep( PINGDONG_MS );
			ls.stop( );
		}

		double elapse = t.elapsed();
		int32_t cmdSizeAll = base::TcpConnection::sRecvCmdSizeAll.get();
		LOGPD(pingpong, "running %0.5f seconds , %d ops, %f opps....", 
			elapse, cmdSizeAll, cmdSizeAll / elapse);
	}
	__LEAVE_FUNCTION

	return 0;
}