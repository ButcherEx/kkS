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

		bool bRet = g_TimeManager.Init(); //时间管理器
		Assert(bRet);

		bRet = g_Config.Init(__argv[0]);
		Assert(bRet);

		srand(g_TimeManager.CurrentTime());

		LOGD(Debug, "main..." ) ;
		LOGD(Debug, "Login Starting... (%.10d)(%d)",
			g_TimeManager.Time2DWORD(), g_TimeManager.StartTime() ) ;

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


