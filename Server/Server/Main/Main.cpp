//#include "stdafx.h"
// #pragma comment(lib, "vld")
// #include <vld.h>
#include "Main.h"
#include "Server.h"
//////////////////////////////////////////////////////////////////////////
class GroupMgr : public EventMgr
{
public:
	virtual void Handle(JoinWolrd& rMsg)
	{
		fprintf(stderr, "%s\n", rMsg.m_Message);
	}
};

GroupMgr g_GroupMgr;

void EventMgr_Test()
{
	if(1)//EventMgr/Msg Test
	{
		TimeInfo timeInfo;
		timeInfo.Update();
		EventMsgPtr joinMsg = POOL_NEW(JoinWolrd);
		EventMsgPtr unhandleMsg = POOL_NEW(UnHandledMsg);

		g_GroupMgr.AddEvent(INVALID_GUID, joinMsg);
		g_GroupMgr.AddEvent(INVALID_GUID, unhandleMsg);
		g_GroupMgr.Update(timeInfo);
	}
}
//////////////////////////////////////////////////////////////////////////
int32_t main(int32_t argc, CHAR* argv[])
{	

// #if defined(__WINDOWS__)
// 	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
// #endif

	__ENTER_FUNCTION

	_MY_TRY
	{
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





