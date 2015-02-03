//#include "stdafx.h"
// #pragma comment(lib, "vld")
// #include <vld.h>
#include "LoginMain.h"
#include "Login.h"
#include "Player.h"
#include "Role/Role.h"
#include "InstanceModule.h"
#include "LogDefine.h"
#include "Task.h"
#include "EventMsg.h"
#include "EventMgr.h"
#include "EventMsg_Test.h"

class GroupMgr : public EventMgr
{
public:
	virtual void HandleMessage(JoinWolrd& rMsg)
	{
		fprintf(stderr, "%s", rMsg.m_Message);
	}
};

GroupMgr g_GroupMgr;

int32_t main(int32_t argc, CHAR* argv[])
{	

// #if defined(__WINDOWS__)
// 	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
// #endif


	__ENTER_FUNCTION

	if(1)//EventMgr/Msg Test
	{
		EventMsgPtr joinMsg = POOL_NEW(JoinWolrd);
		EventMsgPtr unhandleMsg = POOL_NEW(UnHandledMsg);

		g_GroupMgr.AddEvent(INVALID_GUID, joinMsg);
		g_GroupMgr.AddEvent(INVALID_GUID, unhandleMsg);
		g_GroupMgr.Update();
	}
	_MY_TRY
	{
		bool bRet =	g_InstancenManager.Init();
		Assert(bRet);

		bRet = g_TaskManager.Init(10, g_Config.m_LogConfig.m_ThreadNum);
		Assert(bRet);

		g_TaskManager.Excute();	

		g_TaskManager.Exit();
	}
	_MY_CATCH
	{
		
	}

	g_InstancenManager.Exit();

	return	0;

	__LEAVE_FUNCTION

	return -1;
}





