//#include "stdafx.h"
// #pragma comment(lib, "vld")
// #include <vld.h>
#include "Main.h"
#include "Server.h"
#include "CpuMemStat.h"
//////////////////////////////////////////////////////////////////////////

int32_t main(int32_t argc, CHAR* argv[])
{	
	__ENTER_FUNCTION


	_MY_TRY
	{
		Ini ConfigFile("GameConfig.ini");
		int32_t interval = ConfigFile.ReadInt("Login", "PlayerSaveInterval");
		int32_t isopenAdvertisement = ConfigFile.ReadInt("Advertisement", "AdvertisementFirstTime");

		bool bRet =	g_InstancenManager.Init();
		Assert(bRet);

		bRet = g_Server.Init( );
		Assert(bRet);

		g_Server.Loop();

		g_Server.Exit();
	}
	_MY_CATCH
	{
		g_Server.Exit();
	}

	g_InstancenManager.Exit();

	return	0;

	__LEAVE_FUNCTION

	return -1;
}





