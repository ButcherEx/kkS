#include "BaseLib.h"
#include "PBMessage.pb.h"
#include "InstanceModule.h"
#include "LogDefine.h"
#include "Task.h"

InstanceManager g_InstancenManager;

bool InstanceManager::Init()
{
	__ENTER_FUNCTION

		bool bRet = g_Config.Init(__argv[0]);
		Assert(bRet);

		srand(g_TimeManager.CurrentTick());

		LOGD(ServerDebug, "main..." ) ;
		LOGD(ServerDebug, "Login Starting... (%lld)(%d)",
			g_TimeManager.CurrentTimeToInt64(), g_TimeManager.CurrentTick() ) ;

		CHAR currentDir[_MAX_PATH] = {0};
		tgetcwd(currentDir, _MAX_PATH);
		LOGD(ServerDebug, "Current Directory %s", currentDir);

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


