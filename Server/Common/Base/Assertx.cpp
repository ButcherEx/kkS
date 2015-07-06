//--------------------------------------------------------------------------------
// Filename   : Assert.cpp 
//--------------------------------------------------------------------------------

#include "Assertx.h"
#include "Timer.h"
#include "Config.h"

int	g_Command_Assert=0 ;//控制参数，0:会通过弹出对话框让用户选择(缺省值)
						//			1:忽略
						//			2:继续抛出异常用于获取运行堆栈
int g_Command_IgnoreMessageBox=false ;//控制参数，跳过MyMessageBox的中断

//////////////////////////////////////////////////////////////////////////
#define ASSERT_BUF_LEN 10240
void __assert__(const CHAR *func, int32_t line, const CHAR *exp, bool throwException)
{
	static MyLock _asertlogMutex;

	AutoLock_T __lockThis(_asertlogMutex);
	static char assertBuf[ASSERT_BUF_LEN];

	try
	{
		TIME64 now = TimeUtil::Now();
		//////////////////////////////////////////////////////////////////////////
		zeroMemory(assertBuf, ASSERT_BUF_LEN);
		tsnprintf(assertBuf, ASSERT_BUF_LEN, 
			"(###)[%s][%d]%s(%04d-%02d-%02d %02d:%02d:%02d)\n",
			func, line, exp, now.real_year, 
			now.mon, now.day, now.hour, now.min, now.sec);

		//////////////////////////////////////////////////////////////////////////
#define FILE_NAME_LEN 128
		CHAR fileName[FILE_NAME_LEN] = {0};
		tsnprintf(fileName, FILE_NAME_LEN, "%s.%04d-%02d-%02d-%02d.log",  
			"Assert", now.real_year, now.mon, now.day, now.hour);
#undef FILE_NAME_LEN

		CHAR filePath[_MAX_PATH] = {0};
		tsnprintf(filePath, _MAX_PATH, "RunTime/Log/%s", fileName);
		

		//////////////////////////////////////////////////////////////////////////
		FILE *fp = fopen(filePath, "a+");
		if( fp != NULL )
		{
			fwrite(assertBuf, 1, ::strlen(assertBuf), fp);
			fclose(fp);
		}


		if(g_Config.m_LogConfig.m_LogAssert2Stderr)
		{
			coloredWriteToStderr(assertBuf, ::strlen(assertBuf), COLOR_RED);
		}
	} 
	catch(...)
	{

	}
	if(throwException) throw 1;
}
