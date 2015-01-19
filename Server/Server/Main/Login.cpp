//#include "stdafx.h"

#include "Login.h"

Login	g_Login;


Login::Login() : m_bExited(false)
{

}

Login::~Login()
{
}

bool	Login::Init()
{
	__ENTER_FUNCTION

	
	bool	bRet = false;

	bRet  = NewLogin();
	if(!bRet)
	{
		Assert(bRet);
		return false;
	}

	bRet = InitLogin();
	if(!bRet)
	{
		Assert(bRet);
		return false;
	}

	m_bExited = false;
	return true;

	__LEAVE_FUNCTION

	return false;
}

bool	Login::Loop()
{
	__ENTER_FUNCTION

	bool bRet = false ;

	Log::SaveLog( LOGIN_LOGFILE, "\r\nLoop..." ) ;

	//守护线程
	while(true)
	{
		MySleep(100);
	}

	return bRet;

	__LEAVE_FUNCTION

	return false;
}

bool	Login::Exit()
{
	__ENTER_FUNCTION

	
	Log::SaveLog( LOGIN_LOGFILE, "Begin delete..." ) ;


	Log::SaveLog( LOGIN_LOGFILE, "End delete..." ) ;
	
	m_bExited = true;

	return true;

	__LEAVE_FUNCTION

	return false;
}


/*
*	分配空间相关
*/
bool	Login::NewLogin()
{
	__ENTER_FUNCTION
	

	return true;

	__LEAVE_FUNCTION

		return false;
}

/*
*	数据赋值相关
*/
bool	Login::InitLogin()
{
	__ENTER_FUNCTION

	bool	bRet = false;
	int32_t		nTemp = 0;

	//________________________________________________________

	return true;

	__LEAVE_FUNCTION

	return false;
}

void	Login::Stop()
{
	__ENTER_FUNCTION


	__LEAVE_FUNCTION
}


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

LoginExceptionHandler::LoginExceptionHandler()
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

LoginExceptionHandler g_LoginExceptionHandler;
