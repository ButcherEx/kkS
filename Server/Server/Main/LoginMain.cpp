//#include "stdafx.h"

#include "LoginMain.h"
#include "Login.h"
#include "Player.h"
#include "Role/Role.h"
#include "InstanceModule.h"
#include "LogDefine.h"

int32_t main(int32_t argc, CHAR* argv[])
{
#if defined(__WINDOWS__)
	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	__ENTER_FUNCTION_EX
		
	_MY_TRY
	{
		bool bRet =	g_InstancenManager.Init();
		Assert(bRet);

		bRet = g_Login.Init();
		Assert(bRet);

		bRet = g_Login.Loop();	
		Assert(bRet);

		bRet = g_Login.Exit();
		Assert(bRet);
	}
	_MY_CATCH
	{
		
	}

	g_InstancenManager.Exit();

	return	0;

	__LEAVE_FUNCTION_EX

	return -1;
}





