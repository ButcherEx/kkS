//--------------------------------------------------------------------------------
// Filename   : Assert.cpp 
//--------------------------------------------------------------------------------

#include "Assertx.h"
#include "Timer.h"

int	g_Command_Assert=0 ;//控制参数，0:会通过弹出对话框让用户选择(缺省值)
						//			1:忽略
						//			2:继续抛出异常用于获取运行堆栈
int g_Command_IgnoreMessageBox=false ;//控制参数，跳过MyMessageBox的中断

//////////////////////////////////////////////////////////////////////////
#define ASSERT_BUF_LEN 10240
static MyLock _asertlogMutex;
void __assert__(const CHAR *file, const CHAR *func, int32_t line, const CHAR *exp, bool throwException)
{
	AutoLock_T __lockThis(_asertlogMutex);
	static char assertBuf[ASSERT_BUF_LEN];

	try
	{
		TIME64 now = TimeUtil::Now();
		//////////////////////////////////////////////////////////////////////////
		zeroMemory(assertBuf, ASSERT_BUF_LEN);
		tsnprintf(assertBuf, ASSERT_BUF_LEN, 
			"[%s][%s][%s][%d]%s(%04d-%02d-%02d %02d:%02d:%02d).",
			throwException ? "Assert" : "Verify",
			file, func, line, exp, now.year + 1900, 
			now.mon, now.day, now.hour, now.min, now.sec);

		//////////////////////////////////////////////////////////////////////////
#define FILE_NAME_LEN 128
		char fileName[FILE_NAME_LEN] = {0};
		tsnprintf(fileName, FILE_NAME_LEN, "%s%s.%04d-%02d-%02d-%02d.log",  
			/*sLogConfigure.filePrefix().c_str()*/"", "assert",
			now.year + 1900, now.mon, now.day, now.hour);
#undef FILE_NAME_LEN

		bfs::path p = /*sLogConfigure.baseDir()*/"log";
		p /= fileName;

		//////////////////////////////////////////////////////////////////////////
		bfs::fstream assertfs(p, std::ios_base::app);
		assertfs << assertBuf;
		assertfs << "\n";
		assertfs.close();
// #if 0
// 		printf("%s:%s\n", 
// 			throwException ? "Assert" : "Verify", assertBuf);
// #else
// 		int32_t level = throwException ? LOG_LEVEL_FATAL : LOG_LEVEL_ERROR;
// 		if( sLogConfigure.isLogToStderr(level) )
// 		{
// 			strcat(assertBuf, "\n");
// 			coloredWriteToStderr(level, assertBuf, ::strlen(assertBuf));
// 		}
// #endif
	} 
	catch(...)
	{

	}
	if(throwException) throw 1;
}

// void __show__( const CHAR* szTemp )
// {
// 
// #ifdef __LINUX__
// 	printf("Assert:%s",szTemp);
// #endif
// 
// 	//保存日志
// #ifndef GAME_CLIENT
// 	FILE* f = fopen( "./Log/assert.log", "a" ) ;
// 	fwrite( szTemp, 1, strlen(szTemp), f ) ;
// 	fwrite( "\r\n", 1, 2, f ) ;
// 	fclose(f) ;
// #endif
// 
// #if defined(__WINDOWS__)
// 	static MyLock lock ;
// 	if( g_Command_Assert!=1 )
// 	{
// 		AutoLock_T _Lock(lock) ;
// 		int32_t iRet = ::MessageBoxA( NULL, szTemp, "异常", MB_OK ) ;
// 	}
// #elif defined(__LINUX__)
// 	
// #endif
// 
// #ifdef GAME_CLIENT
// 	throw(std::string(szTemp));
// #else
// 	throw(1);
// #endif
// }
// 
// void __messagebox__(const CHAR*msg )
// {
// 	if( g_Command_IgnoreMessageBox )
// 		return ;
// #if defined(__WINDOWS__)
// 	::MessageBoxA( NULL, msg, "信息", MB_OK ) ;
// #elif defined(__LINUX__)
// #endif
// }
// //--------------------------------------------------------------------------------
// //
// // __assert__
// //
// //
// //--------------------------------------------------------------------------------
// void __assert__ ( const CHAR * file , uint32_t line , const CHAR * func , const CHAR * expr )
// {
// 	CHAR szTemp[1024] = {0};
// 	
// #ifdef __LINUX__ //换个格式
// 	sprintf( szTemp, "[%s][%d][%s][%s]\n", file, line, func, expr ) ;
// #else
// 	sprintf( szTemp, "[%s][%d][%s][%s]", file, line, func, expr ) ;
// #endif
// 	__show__(szTemp) ;
// }
// 
// void __assertex__ ( const CHAR * file , uint32_t line , const CHAR * func , const CHAR * expr ,const CHAR* msg)
// {
// 	CHAR szTemp[1024] = {0};
// 	
// #ifdef __LINUX__
// 	sprintf( szTemp, "[%s][%d][%s][%s]\n[%s]\n", file, line, func, expr ,msg ) ;
// #else
// 	sprintf( szTemp, "[%s][%d][%s][%s]\n[%s]", file, line, func, expr ,msg ) ;
// #endif
// 	__show__(szTemp) ;
// }
// 
// void __assertspecial__ ( const CHAR * file , uint32_t line , const CHAR * func , const CHAR * expr ,const CHAR* msg)
// {
// 	CHAR szTemp[1024] = {0};
// 	
// #ifdef __LINUX__
// 	sprintf( szTemp, "S[%s][%d][%s][%s]\n[%s]\n", file, line, func, expr ,msg ) ;
// #else
// 	sprintf( szTemp, "S[%s][%d][%s][%s]\n[%s]", file, line, func, expr ,msg ) ;
// #endif
// 	__show__(szTemp) ;
// }
// //--------------------------------------------------------------------------------
// //
// //
// //--------------------------------------------------------------------------------
// void __protocol_assert__ ( const CHAR * file , uint32_t line , const CHAR * func , const CHAR * expr )
// {
// 	printf( "[%s][%d][%s][%s]", file, line, func, expr ) ;
// }
