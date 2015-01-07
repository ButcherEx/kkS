//#include "stdafx.h"

#include "Login.h"
#include "ConnectManager.h"
#include "ThreadManager.h"
#include "PlayerPool.h"
#include "PacketFactoryManager.h"

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

	bRet = g_Config.Init( ) ;
	if(!bRet)
	{
		Assert(bRet);
		return false;
	}

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

	
	//���ӿͻ����߳�
	Log::SaveLog( LOGIN_LOGFILE, "g_pConnectManager->start( )..." ) ;
	g_pConnectManager->start();


	//����World �� BillingSystem ���߳�
	Log::SaveLog( LOGIN_LOGFILE, "g_pThreadManager->Start( )..." ) ;
	bRet = g_pThreadManager->Start( ) ;
	Assert( bRet ) ;
	
	//���ݿ⴦��
	//Log::SaveLog( LOGIN_LOGFILE, "g_pDataBaseManager->Start( )..." ) ;
	//g_pDataBaseManager->start();
	

	//�ػ��߳�
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

	SAFE_DELETE(g_pPlayerPool);
	Log::SaveLog( LOGIN_LOGFILE, "g_pPlayerPool delete...OK" ) ;
	

	SAFE_DELETE(g_pPacketFactoryManager);
	Log::SaveLog( LOGIN_LOGFILE, "g_pPacketFactoryManager delete...OK" ) ;
	
	SAFE_DELETE(g_pThreadManager);
	Log::SaveLog( LOGIN_LOGFILE, "g_pThreadManager delete...OK" ) ;
	
	
	SAFE_DELETE(g_pConnectManager);
	Log::SaveLog( LOGIN_LOGFILE, "g_pConnectManager delete...OK" ) ;
	

	Log::SaveLog( LOGIN_LOGFILE, "End delete..." ) ;
	
	m_bExited = true;

	return true;

	__LEAVE_FUNCTION

	return false;
}


/*
*	����ռ����
*/
bool	Login::NewLogin()
{
	__ENTER_FUNCTION
	
	g_pPlayerPool		=	new	  PlayerPool ;
	AssertEx( g_pPlayerPool,"����g_pPlayerPool ʧ��!");
	Log::SaveLog( LOGIN_LOGFILE, "new PlayerPool...OK" ) ;

	g_pPacketFactoryManager = new PacketFactoryManager ;
	AssertEx( g_pPacketFactoryManager,"����g_pFacketFactoryManager ʧ��!") ;
	Log::SaveLog( LOGIN_LOGFILE, "new PacketFactoryManager...OK" ) ;
	
	g_pThreadManager	=	 new ThreadManager();
	AssertEx( g_pThreadManager,"����g_pThreadManager ʧ��!");
	Log::SaveLog( LOGIN_LOGFILE, "new ThreadManager...OK" ) ;

	g_pConnectManager   =    new ConnectManager ;
	AssertEx( g_pConnectManager,"����g_pConnectManager ʧ��!");
	Log::SaveLog( LOGIN_LOGFILE, "new ConnectManager...OK" ) ;


	return true;

	__LEAVE_FUNCTION

		return false;
}

/*
*	���ݸ�ֵ���
*/
bool	Login::InitLogin()
{
	__ENTER_FUNCTION

	bool	bRet = false;
	int32_t		nTemp = 0;

	//________________________________________________________
	if( g_Config.m_ConfigInfo.m_SystemModel == 0 )
	{
		nTemp = 5;
	}
	else
	{
		nTemp = MAX_POOL_SIZE;
	}
	bRet = g_pPlayerPool->Init( nTemp ) ;
	Assert( bRet ) ;
	Log::SaveLog( LOGIN_LOGFILE, "g_pPlayerPool->Init()...OK" ) ;

	//
	bRet = g_pPacketFactoryManager->Init( ) ;
	Assert( bRet ) ;
	Log::SaveLog( LOGIN_LOGFILE, "g_pPacketFactoryManager->Init()...OK" ) ;
	
	//�Կͻ������������ĳ�ʼ��
	bRet = g_pConnectManager->Init( ) ;
	Assert( bRet ) ;
	Log::SaveLog( LOGIN_LOGFILE, "g_pConnectManager->Init()...OK" ) ;

	// �̹߳�����
	bRet = g_pThreadManager->Init( ) ;
	Assert( bRet ) ;
	Log::SaveLog( LOGIN_LOGFILE, "g_pThreadManager->Init()...OK" ) ;




	return true;

	__LEAVE_FUNCTION

	return false;
}

void	Login::Stop()
{
	__ENTER_FUNCTION

		if( g_pConnectManager )
		{
			g_pConnectManager->stop( ) ;
		}

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
