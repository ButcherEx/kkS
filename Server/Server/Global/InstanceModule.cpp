#include "BaseLib.h"
#include "PBMessage.pb.h"
#include "InstanceModule.h"
#include "LogDefine.h"
#include "Task.h"

InstanceManager g_InstancenManager;

bool InstanceManager::Init()
{
	__ENTER_FUNCTION

#if defined(__WINDOWS__)
		_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
#endif

		bool bRet = g_Config.Init(__argv[0]);
		Assert(bRet);

		srand(g_TimeManager.CurrentTick());

		LOGD(ServerDebug, "main..." ) ;
		LOGD(ServerDebug, "Login Starting... (%lld)(%d)",
			g_TimeManager.CurrentTimeToInt64(), g_TimeManager.CurrentTick() ) ;

		CHAR currentDir[_MAX_PATH] = {0};
		tgetcwd(currentDir, _MAX_PATH);
		LOGD(ServerDebug, "Current Directory %s", currentDir);

		doScriptTest("lua1.lua");
		doScriptTest("lua.lua");

		return bRet;
	__LEAVE_FUNCTION
		return false;
}

bool InstanceManager::Exit()
{
	__ENTER_FUNCTION
		::google::protobuf::ShutdownProtobufLibrary();
		return true;
	__LEAVE_FUNCTION
		return false;
}


