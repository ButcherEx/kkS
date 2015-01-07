//#include "stdafx.h"



#include "ServerPlayer.h"
#include "PacketFactoryManager.h"
#include "Assertx.h"


ServerPlayer::ServerPlayer( bool bIsServer ) : Player( bIsServer ) 
{
__ENTER_FUNCTION

	m_Status = 0 ;

__LEAVE_FUNCTION
}

ServerPlayer::~ServerPlayer( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

bool ServerPlayer::ProcessCommand( bool Option )
{
__ENTER_FUNCTION

	return Player::ProcessCommand( Option ) ;

__LEAVE_FUNCTION

	return false ;
}

bool ServerPlayer::ProcessInput( )
{
__ENTER_FUNCTION

	return Player::ProcessInput( ) ;

__LEAVE_FUNCTION

	return false ;
}

bool ServerPlayer::ProcessOutput( )
{
__ENTER_FUNCTION

	return Player::ProcessOutput( ) ;

__LEAVE_FUNCTION

	return false ;
}

bool ServerPlayer::SendPacket( Packet* pPacket )
{
__ENTER_FUNCTION

	return Player::SendPacket( pPacket ) ;

__LEAVE_FUNCTION

	return false ;
}

bool ServerPlayer::IsValid( )
{
__ENTER_FUNCTION

	if( !Player::IsValid( ) )
		return false ;

	return true ;

__LEAVE_FUNCTION

	return false ;
}

void ServerPlayer::CleanUp( )
{
__ENTER_FUNCTION

	Player::CleanUp( ) ;

__LEAVE_FUNCTION
}

bool ServerPlayer::HeartBeat( uint32_t uTime )
{
__ENTER_FUNCTION


	return Player::HeartBeat(uTime ) ;

__LEAVE_FUNCTION

	return false ;
}