//#include "stdafx.h"

#include "LoginMain.h"
#include "Login.h"
#include "Player.h"
#include "Role/Role.h"
#include "InstanceModule.h"
#include "LogDefine.h"

int32_t main(int32_t argc, CHAR* argv[])
{	
	__ENTER_FUNCTION
		
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

	__LEAVE_FUNCTION

	return -1;
}





