#include "BaseLib.h"
#include "PBMessage.pb.h"
#include "InstanceModule.h"

InstanceManager g_InstancenManager;

bool InstanceManager::Init()
{
	__ENTER_FUNCTION
		bool bRet = g_TimeManager.Init(); //ʱ�������
		Assert(bRet);

		bRet = g_Config.Init(__argv[0]);
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
		::google::protobuf::ShutdownProtobufLibrary();
	__LEAVE_FUNCTION
		return false;
}


