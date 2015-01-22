#include "BaseLib.h"
#include "ExceptionHandler.h"

#ifdef __LINUX__
void  DumpStack(int32 sig)
{
	void *	DumpArray[25];
	int	Size =	backtrace(DumpArray,25);
	char ** symbols = backtrace_symbols(DumpArray, Size);
	if(symbols)
	{
		if(Size>10) Size= 10;
		if(Size>0)
		{
			FILE* f = fopen( DUMP_LOG, "a" ) ;
			char threadinfo[256] = {0};
			tsnprintf(threadinfo, 256, "threadid = %d cause dump\r\n",MyGetCurrentThreadID());
			fprintf(f,"%s Signal=%d\r\n", threadinfo, sig);
			for(int	i=0;i<Size;i++)
			{
				printf("%s\r\n",symbols[i]);
				fwrite(symbols[i],1,strlen(symbols[i]),f);
				fwrite("\r\n",1,2,f);
			}
			fclose(f) ;
		}
		free(symbols);
	}
	else
	{
		FILE* f = fopen( DUMP_LOG, "a" ) ;
		char	buffer[256] = {0};
		char threadinfo[256] = {0};
		tsnprintf(threadinfo, 256, "threadid = %d cause dump\r\n",MyGetCurrentThreadID());
		fprintf(f,"%s Signal=%d\r\n", threadinfo, sig);
		fclose(f);
	}
}
#else
void  DumpStack(int32 sig, const CHAR* type)
{

}
#endif

void SignalHandler(int32_t sig)
{
	DumpStack(sig, "XFSZ exception:\r\n");
	exit(0);
}

ExceptionHandler::ExceptionHandler()
{
#ifdef __LINUX__
	signal(SIGSEGV, SignalHandler);
	signal(SIGFPE,  SignalHandler);
	signal(SIGILL,  SignalHandler);
	signal(SIGINT,  SignalHandler);  
	signal(SIGTERM, SignalHandler); 
	signal(SIGABRT, SignalHandler);
	signal(SIGXFSZ, SignalHandler);
#endif
}

ExceptionHandler g_ExceptionHandler;
