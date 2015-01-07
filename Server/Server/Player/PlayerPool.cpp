//#include "stdafx.h"



#include "PlayerPool.h"
#include "Assertx.h"

PlayerPool* g_pPlayerPool=NULL ;

PlayerPool::PlayerPool( )
{
__ENTER_FUNCTION

	m_pPlayer = NULL ;
	m_Position = 0 ;
	m_PlayerCount = 0 ;
	m_MaxPlayerCount = 0 ;

__LEAVE_FUNCTION
}

PlayerPool::~PlayerPool( ) 
{
__ENTER_FUNCTION

	SAFE_DELETE_ARRAY( m_pPlayer ) ;


__LEAVE_FUNCTION
}

bool PlayerPool::Init( uint32_t MaxPlayerCount )
{
__ENTER_FUNCTION

	m_pPlayer = new LoginPlayer[MaxPlayerCount] ;
	Assert( m_pPlayer ) ;

	for( uint32_t i=0; i<MaxPlayerCount; i++ )
	{
		m_pPlayer[i].SetPlayerID( (PlayerID_t)i ) ;
		m_pPlayer[i].SetEmpty(true) ;
	}
	m_Position = 0 ;
	m_PlayerCount = MaxPlayerCount ;
	m_MaxPlayerCount = MaxPlayerCount ;

	return true ;

__LEAVE_FUNCTION

	return false ;
}



LoginPlayer* PlayerPool::NewPlayer( )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	int32_t iRet = 0 ;
	for( uint32_t i=0; i<m_MaxPlayerCount; i++ )
	{
		if( m_pPlayer[m_Position].IsEmpty() )
		{
			iRet = m_Position ;
			m_pPlayer[m_Position].SetEmpty( false ) ;

			m_Position ++ ;
			if( m_Position >= m_MaxPlayerCount ) 
				m_Position = 0 ;

			m_PlayerCount -- ;

			return &(m_pPlayer[iRet]) ;
		}

		m_Position ++ ;
		if( m_Position >= m_MaxPlayerCount ) 
			m_Position = 0 ;
	}

	return NULL ;

__LEAVE_FUNCTION

	return NULL ;
}

LoginPlayer* PlayerPool::NewPlayer( PlayerID_t PlayerID)
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if(PlayerID>=(PlayerID_t)m_MaxPlayerCount)
	{
		return NULL;
	}


	if(m_pPlayer[PlayerID].IsEmpty())
	{
		LoginPlayer* pPlayer = &(m_pPlayer[PlayerID]);
		pPlayer->SetEmpty(false);
		m_PlayerCount--;
		m_Position = PlayerID;
		m_Position ++ ;
		if( m_Position >= m_MaxPlayerCount ) 
			m_Position = 0 ;

		return pPlayer;

	}

	return NULL ;
	__LEAVE_FUNCTION
	return NULL ;
}

void PlayerPool::DelPlayer( PlayerID_t PlayerID )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( (uint32_t)PlayerID >= m_MaxPlayerCount )
	{
		Assert( false ) ;
		return ;
	}

	m_pPlayer[PlayerID].SetEmpty( true ) ;
	

	m_PlayerCount ++ ;

	return ;

__LEAVE_FUNCTION

	return ;
}

