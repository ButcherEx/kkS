//--------------------------------------------------------------------------------
// Filename   : Assert.cpp 
//--------------------------------------------------------------------------------

#include "Assertx.h"
#include "Timer.h"
#include "Config.h"

int	g_Command_Assert=0 ;//���Ʋ�����0:��ͨ�������Ի������û�ѡ��(ȱʡֵ)
						//			1:����
						//			2:�����׳��쳣���ڻ�ȡ���ж�ջ
int g_Command_IgnoreMessageBox=false ;//���Ʋ���������MyMessageBox���ж�

//////////////////////////////////////////////////////////////////////////
#define ASSERT_BUF_LEN 10240
void __assert__(const CHAR *file, const CHAR *func, int32_t line, const CHAR *exp, bool throwException)
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
			"(###)[%s][%s][%d]%s(%04d-%02d-%02d %02d:%02d:%02d).",
			file, func, line, exp, now.real_year, 
			now.mon, now.day, now.hour, now.min, now.sec);

		//////////////////////////////////////////////////////////////////////////
#define FILE_NAME_LEN 128
		char fileName[FILE_NAME_LEN] = {0};
		tsnprintf(fileName, FILE_NAME_LEN, "%s%s.%04d-%02d-%02d-%02d.log",  
			g_Config.m_LogConfig.m_FilePrefix.c_str(), "Assert",
			now.real_year, now.mon, now.day, now.hour);
#undef FILE_NAME_LEN

		bfs::path p = g_Config.m_LogConfig.m_LogDir.c_str();
		p /= fileName;

		//////////////////////////////////////////////////////////////////////////
		bfs::fstream assertfs(p, std::ios_base::app);
		assertfs << assertBuf;
		assertfs << "\n";
		assertfs.close();


		if(g_Config.m_LogConfig.m_LogAssert2Stderr)
		{
			strcat(assertBuf, "\n");
			coloredWriteToStderr(assertBuf, ::strlen(assertBuf), COLOR_RED);
		}
	} 
	catch(...)
	{

	}
	if(throwException) throw 1;
}
