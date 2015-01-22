#include "BaseLib.h"
#include "InstanceModule.h"

InstanceManager g_InstancenManager;

bool InstanceManager::Init()
{
	__ENTER_FUNCTION
		bool bRet = g_TimeManager.Init(); //时间管理器
		Assert(bRet);

		bRet = g_Config.Init( );
		Assert(bRet);

		srand(g_TimeManager.CurrentTime());

		Log::SaveLog( "./Log/Login.log", "\r\n(###) main..." ) ;
		Log::SaveLog( "./Log/Login.log", "Login Starting... (%.10d)(%d)",
			g_TimeManager.Time2DWORD(), g_TimeManager.StartTime() ) ;

		return bRet;
	__LEAVE_FUNCTION
		return false;
}

bool InstanceManager::Exit()
{
	__ENTER_FUNCTION
		
	__LEAVE_FUNCTION
		return false;
}


