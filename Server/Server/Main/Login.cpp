//#include "stdafx.h"

#include "Login.h"
#include "LogDefine.h"

Login	g_Login;


Login::Login() : m_bExited(false)
{

}

Login::~Login()
{
}

bool	Login::Init()
{
	__ENTER_FUNCTION

	
	bool	bRet = false;

	bRet  = NewLogin();
	if(!bRet)
	{
		Assert(bRet);
		return false;
	}

	bRet = InitLogin();
	if(!bRet)
	{
		Assert(bRet);
		return false;
	}

	m_bExited = false;
	return true;

	__LEAVE_FUNCTION

	return false;
}

bool	Login::Loop()
{
	__ENTER_FUNCTION

	bool bRet = false ;

	LOG_DEBUG(ServerDebug, "Loop..." ) ;

	//守护线程
	while(true)
	{
		MySleep(100);
	}

	return bRet;

	__LEAVE_FUNCTION

	return false;
}

bool	Login::Exit()
{
	__ENTER_FUNCTION

	
	LOG_DEBUG(ServerDebug,"Begin delete..." ) ;


	LOG_DEBUG(ServerDebug,"End delete..." ) ;
	
	m_bExited = true;

	return true;

	__LEAVE_FUNCTION

	return false;
}


/*
*	分配空间相关
*/
bool	Login::NewLogin()
{
	__ENTER_FUNCTION
	

	return true;

	__LEAVE_FUNCTION

		return false;
}

/*
*	数据赋值相关
*/
bool	Login::InitLogin()
{
	__ENTER_FUNCTION

	bool	bRet = false;
	int32_t		nTemp = 0;

	//________________________________________________________

	return true;

	__LEAVE_FUNCTION

	return false;
}

void	Login::Stop()
{
	__ENTER_FUNCTION


	__LEAVE_FUNCTION
}


