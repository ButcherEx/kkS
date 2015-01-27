#include "BaseLib.h"
#include "PBMessage.pb.h"
#include "InstanceModule.h"
#include "LogDefine.h"

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

		LOGD(Debug, "main..." ) ;
		LOGD(Debug, "Login Starting... (%lld)(%d)",
			g_TimeManager.CurrentTimeToInt64(), g_TimeManager.CurrentTick() ) ;

		//bsys::error_code ec;
		// basePath_ = bfs::current_path(ec); //取得当前目录  
		CHAR currentDir[_MAX_PATH] = {0};
		tgetcwd(currentDir, _MAX_PATH);
		LOGD(Debug, "Current Directory %s", currentDir);

		return bRet;
	__LEAVE_FUNCTION
		return false;
}

bool InstanceManager::Exit()
{
	__ENTER_FUNCTION
		::google::protobuf::ShutdownProtobufLibrary();
	__LEAVE_FUNCTION
		return false;
}


