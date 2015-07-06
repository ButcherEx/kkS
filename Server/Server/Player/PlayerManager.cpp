
#include "PlayerManager.h"
#include "Assertx.h"
#include "LogDefine.h"

PlayerManager::PlayerManager( )
{
__ENTER_FUNCTION

	m_nPlayers = 0 ;
	m_PlayerPtrList.Clear();

__LEAVE_FUNCTION
}

PlayerManager::~PlayerManager( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

void PlayerManager::CleanUp( )
{
__ENTER_FUNCTION

	m_nPlayers = 0 ;
	m_PlayerPtrList.Clear();

__LEAVE_FUNCTION
}

bool PlayerManager::AddPlayer( PlayerPtr Ptr )
{
__ENTER_FUNCTION

	Assert( Ptr ) ;

	m_PlayerPtrList.PushBack(Ptr);
	OnAddPlayer(Ptr, 0);
	return true ;
	
__LEAVE_FUNCTION

	return false ;
}


void PlayerManager::RemovePlayer( PlayerPtr Ptr, int32_t reason )
{
__ENTER_FUNCTION

	Assert( Ptr ) ;
	PlayerPtrList::TListIterator iter = std::remove(m_PlayerPtrList.Begin(), m_PlayerPtrList.End(), Ptr);
	if( iter != m_PlayerPtrList.End()) OnRemovePlayer(Ptr, reason);
	
__LEAVE_FUNCTION
}

bool PlayerManager::Select( )
{
	__ENTER_FUNCTION

	if( m_PlayerPtrList.Empty()) return true;

	FD_ZERO(&m_ReadFDs);
	FD_ZERO(&m_WriteFDs);
	FD_ZERO(&m_ExceptFDs);

	int32_t		maxFD = INVALID_SOCKET;

	PlayerPtr Ptr;
	m_PlayerPtrList.ResetIterator();
	while (m_PlayerPtrList.PeekNext(Ptr))
	{
		Assert(Ptr);
		SOCKET sock = Ptr->GetSocket().getSOCKET();
		Assert(sock != INVALID_SOCKET);
		FD_SET(sock, &m_ReadFDs);
		FD_SET(sock, &m_WriteFDs);
		FD_SET(sock, &m_ExceptFDs);
		if(maxFD < (int32_t)sock) maxFD = sock;
	}
	

	_MY_TRY 
	{
		timeval	tv; 
		tv.tv_sec = 0; tv.tv_usec = 0;
		int32_t iRet = SocketAPI::select_ex((int32_t)maxFD+1 , 
			&m_ReadFDs, &m_WriteFDs, &m_ExceptFDs, &tv);
		Assert( iRet!=SOCKET_ERROR ) ;
	} 
	_MY_CATCH
	{
		LOG_ERROR(ServerError,"ERROR: IncomingPlayerManager::Select( )..." ) ;
	}

	return true ;

	__LEAVE_FUNCTION

		return false ;
}

bool PlayerManager::ProcessInputs( )
{
	__ENTER_FUNCTION

	if( m_PlayerPtrList.Empty()) return true;

	PlayerPtr Ptr;
	m_PlayerPtrList.ResetIterator();
	while (m_PlayerPtrList.PeekNext(Ptr))
	{
		Assert(Ptr);
		SOCKET sock = Ptr->GetSocket().getSOCKET();
		Assert(sock != INVALID_SOCKET);

		if( FD_ISSET( sock, &m_ReadFDs ) )
		{
			if( Ptr->GetSocket().isSockError() )
			{//连接出现错误
				RemovePlayer( Ptr ) ;
			}
			else
			{//连接正常
				_MY_TRY
				{
					if( !Ptr->ProcessInput( ) )
					{
						RemovePlayer( Ptr ) ;
					}
				}
				_MY_CATCH
				{
					RemovePlayer( Ptr ) ;
				}
			}
		}
	}

	return true ;

	__LEAVE_FUNCTION

		return false ;
}

bool PlayerManager::ProcessOutputs( )
{
	__ENTER_FUNCTION

	if( m_PlayerPtrList.Empty()) return true;

	PlayerPtr Ptr;
	m_PlayerPtrList.ResetIterator();
	while (m_PlayerPtrList.PeekNext(Ptr))
	{
		Assert(Ptr);
		SOCKET sock = Ptr->GetSocket().getSOCKET();
		Assert(sock != INVALID_SOCKET);

		if( FD_ISSET( sock, &m_WriteFDs ) )
		{
			if( Ptr->GetSocket().isSockError() )
			{//连接出现错误
				RemovePlayer( Ptr ) ;
			}
			else
			{//连接正常
				_MY_TRY
				{
					if( !Ptr->ProcessOutput( ) )
					{
						RemovePlayer( Ptr ) ;
					}
				}
				_MY_CATCH
				{
					RemovePlayer( Ptr ) ;
				}
			}
		}
	}

	return true ;

	__LEAVE_FUNCTION

		return false ;
}

bool PlayerManager::ProcessExceptions( )
{
	__ENTER_FUNCTION


	if( m_PlayerPtrList.Empty()) return true;

	PlayerPtr Ptr;
	m_PlayerPtrList.ResetIterator();
	while (m_PlayerPtrList.PeekNext(Ptr))
	{
		Assert(Ptr);
		SOCKET sock = Ptr->GetSocket().getSOCKET();
		Assert(sock != INVALID_SOCKET);

		if( FD_ISSET( sock, &m_ExceptFDs ) )
		{
			RemovePlayer( Ptr ) ;
		}
	}

	return true ;

	__LEAVE_FUNCTION

		return false ;
}

bool PlayerManager::ProcessCommands( )
{
	__ENTER_FUNCTION

	if( m_PlayerPtrList.Empty()) return true;

	PlayerPtr Ptr;
	m_PlayerPtrList.ResetIterator();
	while (m_PlayerPtrList.PeekNext(Ptr))
	{
		Assert(Ptr);
		SOCKET sock = Ptr->GetSocket().getSOCKET();
		Assert(sock != INVALID_SOCKET);

		if( Ptr->GetSocket().isSockError() )
		{//连接出现错误
			RemovePlayer( Ptr ) ;
		}
		else
		{//连接正常
			_MY_TRY
			{
				if( !Ptr->ProcessCommand( ) )
				{
					RemovePlayer( Ptr ) ;
				}
			}
			_MY_CATCH
			{
				RemovePlayer( Ptr ) ;
			}
		}
	}

	return true ;

	__LEAVE_FUNCTION

		return false ;
}


bool PlayerManager::HeartBeat( uint32_t uTime )
{
__ENTER_FUNCTION

/*
	BOOL ret ;

	uint32_t nPlayerCount = GetPlayerNumber() ;
	for( uint32_t i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i] == INVALID_ID )
			continue ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			return FALSE ;
		}
		ret = pPlayer->HeartBeat( ) ;
		Assert( ret ) ;
	}
*/

	return true ;

__LEAVE_FUNCTION

	return false ;
}
