//#include "stdafx.h"

#include "LoginMain.h"
#include "Login.h"
#include "CLAskCharList.h"
#if defined(__WINDOWS__)

struct NetworkInitialize
{
	NetworkInitialize()
	{
		WSADATA wsaData= {0};
		uint16_t dwNetworkVersion = MAKEWORD(2,2);
 		::WSAStartup(dwNetworkVersion, &wsaData);
 		
 		_MY_TRY
		{
			CG_LOGIN_PAK loginPak;
			loginPak.Msg.set_vtype(1);
			loginPak.Msg.set_gameversion(1);
			loginPak.Msg.set_programversion(1);
			loginPak.Msg.set_publicresourceversion(1);
			loginPak.Msg.set_maxpacketid(99);
			loginPak.Msg.set_forceenter(8);
			loginPak.Msg.set_deviceid("ma.CY5929");
			loginPak.Msg.set_devicetype("WINDOWS");
			loginPak.Msg.set_deviceversion("Win8.1");
			int32 byteSize  = loginPak.Msg.ByteSize();
			CHAR Buf[256] = {0};
			loginPak.Msg.SerializeToArray(Buf,sizeof Buf);
			printf("%s\n", Buf);
			
			Packet* pPacket = new CG_LOGIN_PAK;
			pPacket->rMsg.ParseFromString(Buf);
			pPacket->FreeOwn();
		}
		_MY_CATCH
		{

		}
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





