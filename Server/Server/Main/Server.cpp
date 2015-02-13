#include "Server.h"
#include "DBMainTask.h"
#include "MapCreator.h"

//////////////////////////////////////////////////////////////////////////
Server g_Server;
//////////////////////////////////////////////////////////////////////////
Server::Server()
{

}

Server::~Server()
{

}

bool Server::Init()
{
	__ENTER_FUNCTION
	
	//////////////////////////////////////////////////////////////////////////
	Init_AllTask( );
	//////////////////////////////////////////////////////////////////////////
	return true;
	__LEAVE_FUNCTION
	return false;
}

void Server::Init_AllTask()
{
	__ENTER_FUNCTION

	//////////////////////////////////////////////////////////////////////////
	m_MainTaskManager.SetName("MainTaskManager");
	//////////////////////////////////////////////////////////////////////////
	bool bRet = m_MainTaskManager.Init(TaskDefine::MAX_TASK, g_Config.m_LogConfig.m_ThreadNum);
	Assert(bRet);
	//////////////////////////////////////////////////////////////////////////
	m_MainTaskManager.Register( TaskPtr(new MapCreator() ) );
	m_MainTaskManager.Register( TaskPtr(new DBMainTask() ) );

	//////////////////////////////////////////////////////////////////////////
	__LEAVE_FUNCTION
}

void Server::Loop()
{
	__ENTER_FUNCTION
		m_MainTaskManager.Excute();
	__LEAVE_FUNCTION
}
void Server::Exit()
{
	__ENTER_FUNCTION
		m_MainTaskManager.Exit();
	__LEAVE_FUNCTION
}
