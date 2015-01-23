//#include "stdafx.h"

#include "LoginMain.h"
#include "Login.h"
#include "Player.h"
#include "Role/Role.h"
#include "InstanceModule.h"
#include "LogDefine.h"

#if defined(__WINDOWS__)



struct NetworkInitialize
{
	NetworkInitialize()
	{
		WSADATA wsaData= {0};
		uint16_t dwNetworkVersion = MAKEWORD(2,2);
 		::WSAStartup(dwNetworkVersion, &wsaData);
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
	
	LOGI(Assert, "%s", "Fc");

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
		
		
	_MY_TRY
	{
		bool bRet =	g_InstancenManager.Init();
		Assert(0);

		bRet = g_Login.Init();
		Assert(bRet);

		bRet = g_Login.Loop();	
		Assert(bRet);

		bRet = g_Login.Exit();
		Assert(bRet);
	}
	_MY_CATCH
	{
		
	}

	g_InstancenManager.Exit();

	return	0;

	__LEAVE_FUNCTION

	return -1;
}





