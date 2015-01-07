//#include "stdafx.h"

#include "LoginMain.h"
#include "Login.h"
#include "PBMessage.pb.h"
#if defined(__WINDOWS__)
struct NetworkInitialize
{
	NetworkInitialize()
	{
		WSADATA wsaData= {0};
		uint16_t dwNetworkVersion = MAKEWORD(2,2);
 		::WSAStartup(dwNetworkVersion, &wsaData);
 		::CG_LOGIN msg;
 		
		msg.set_vtype(1);
		msg.set_gameversion(1);
		msg.set_programversion(1);
		msg.set_publicresourceversion(1);
		msg.set_maxpacketid(99);
		msg.set_forceenter(8);
		msg.set_deviceid("ma.CY5929");
		msg.set_devicetype("WINDOWS");
		//msg.set_deviceversion("Win8.1");
		int32 byteSize  = msg.ByteSize();
		CHAR Buf[256] = {0};
		msg.SerializeToArray(Buf,sizeof Buf);
		printf("%s", Buf);



	}

	~NetworkInitialize()
	{
		::WSACleanup();
	}

} __AutoNetworkInitialize;
#endif

int32_t main(int32_t argc, CHAR* argv[])
{
#if defined(__WINDOWS__)
	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	__ENTER_FUNCTION
	
		if( argc>1 )
		{
			for( int i=1; i<argc; i++ )
			{
				if( strcmp(argv[i],"-ignoreassert")==0 )
				{
					g_Command_Assert=1 ;
				}
				else if( strcmp(argv[i],"-retryassert")==0 )
				{
					g_Command_Assert=2 ;
				}

				if( strcmp(argv[i],"-ignoremessagebox")==0 )
				{
					g_Command_IgnoreMessageBox=true ;
				}
			}
		}	
		
		
	//时间管理器
	g_pTimeManager	=	new TimeManager;
	g_pTimeManager->Init();
			
	Log::SaveLog( "./Log/Login.log", "\r\n(###) main..." ) ;
	g_pTimeManager->SetTime( ) ;
		
	Log::SaveLog( "./Log/Login.log", "Login Starting... (%.10d)(%d)",
	g_pTimeManager->Time2DWORD(),
	g_pTimeManager->StartTime() ) ;
	srand(g_pTimeManager->CurrentTime());
	bool	bRet  =		g_Login.Init();
	Assert(bRet);
		
	bRet	=			g_Login.Loop();	
	Assert(bRet);

	bRet	=			g_Login.Exit();
	Assert(bRet);

	return	0;

	__LEAVE_FUNCTION

	return -1;
}





