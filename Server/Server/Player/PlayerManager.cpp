
#include "PlayerManager.h"
#include "Assertx.h"


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
	return true ;
	
__LEAVE_FUNCTION

	return false ;
}


void PlayerManager::RemovePlayer( PlayerPtr Ptr, int32_t reason )
{
__ENTER_FUNCTION

	Assert( Ptr ) ;
	std::remove(m_PlayerPtrList.Begin(), m_PlayerPtrList.End(), Ptr);
__LEAVE_FUNCTION
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
