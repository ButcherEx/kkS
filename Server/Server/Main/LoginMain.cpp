//#include "stdafx.h"

#include "LoginMain.h"
#include "Login.h"
#include "Player.h"
#include "Role/Role.h"
#include "InstanceModule.h"
#include "LogDefine.h"
#include "Task.h"

int32_t main(int32_t argc, CHAR* argv[])
{	
	__ENTER_FUNCTION
		
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





