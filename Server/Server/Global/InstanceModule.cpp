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

		srand((uint32_t)TimeUtil::AnsiTime());

		LOG_DEBUG(ServerDebug, "main..." ) ;
		LOG_DEBUG(ServerDebug, "Login Starting... (%u)(%u)",
			g_TimeManager.CurrentTime(), g_TimeManager.RunTime() ) ;

		CHAR currentDir[_MAX_PATH] = {0};
		tgetcwd(currentDir, _MAX_PATH);
		LOG_DEBUG(ServerDebug, "Current Directory %s", currentDir);

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


