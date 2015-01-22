#include "BaseLib.h"
#include "ExceptionHandler.h"

void INTHandler(int32_t)
{
	DumpStack("INT exception:\r\n");
	exit(0);
}
void TERMHandler(int32_t)
{
	DumpStack("TERM exception:\r\n");
	exit(0);

}
void ABORTHandler(int32_t)
{
	DumpStack("ABORT exception:\r\n");
	exit(0);
}

void ILLHandler(int32_t)
{
	DumpStack("ILL exception:\r\n");
	exit(0);
}

void FPEHandler(int32_t)
{
	DumpStack("FPE exception:\r\n");
	exit(0);
}

void SEGHandler(int32_t)
{
	DumpStack("SEG exception:\r\n");
	exit(0);
}
void XFSZHandler(int32_t)
{
	DumpStack("XFSZ exception:\r\n");
	exit(0);
}

ExceptionHandler::ExceptionHandler()
{
#ifdef __LINUX__
	signal(SIGSEGV, SEGHandler);
	signal(SIGFPE,  FPEHandler);
	signal(SIGILL,  ILLHandler);
	signal(SIGINT,  INTHandler);  
	signal(SIGTERM, TERMHandler); 
	signal(SIGABRT, ABORTHandler);
	signal(SIGXFSZ, XFSZHandler);
#endif
}

ExceptionHandler g_ExceptionHandler;
