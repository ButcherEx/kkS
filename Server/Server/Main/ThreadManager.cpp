//#include "stdafx.h"


#include "ThreadManager.h"



ThreadManager*	g_pThreadManager=NULL ;

ThreadManager::ThreadManager( )
{
__ENTER_FUNCTION

	m_pServerThread = new ServerThread ;
	Assert( m_pServerThread ) ;

	m_nThreads = 0 ;

__LEAVE_FUNCTION
}

ThreadManager::~ThreadManager( )
{
__ENTER_FUNCTION
	
	SAFE_DELETE( m_pServerThread) ;

__LEAVE_FUNCTION
}

bool ThreadManager::Init( )
{
__ENTER_FUNCTION

	bool ret = false ;

	if( m_pServerThread->IsActive() )
	{
		m_nThreads ++ ;
	}

	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool ThreadManager::Start( )
{
__ENTER_FUNCTION

	m_pServerThread->start() ;

	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool ThreadManager::Stop( )
{
__ENTER_FUNCTION

	if( m_pServerThread )
	{
		m_pServerThread->stop( ) ;
	}

	return true;

__LEAVE_FUNCTION

	return false ;
}

