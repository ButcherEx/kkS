//#include "stdafx.h"



#include "LoginPlayer.h"
#include "PlayerPool.h"


LoginPlayer::LoginPlayer( )
{
__ENTER_FUNCTION

	m_Status = PS_LOGIN_EMPTY ;
	m_LastSendTime	 = 0 ;
	m_ConnectTime	 = 0 ;
	m_KickTime		 = 0 ;
	m_CurrentTime	 = 0 ;
	m_LeftTimeToQuit = 0 ;
	m_AccountGuid	 = 0 ;	

__LEAVE_FUNCTION
}

LoginPlayer::~LoginPlayer( )
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}

void LoginPlayer::CleanUp( )
{
__ENTER_FUNCTION

	m_Status = PS_LOGIN_EMPTY ;
	m_KickTime		 =  0;
	m_LastSendTime   =  0;
	m_ConnectTime	 =	0;
	m_CurrentTime	 =	0;
	m_LeftTimeToQuit =	0;
	m_AccountGuid	 =	0;
	
	ResetKick( ) ;
	return Player::CleanUp( ) ;

__LEAVE_FUNCTION
}

bool LoginPlayer::ProcessCommand( bool Option )
{
__ENTER_FUNCTION

	return Player::ProcessCommand( Option ) ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayer::ProcessInput( )
{
__ENTER_FUNCTION

	return Player::ProcessInput( ) ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayer::ProcessOutput( )
{
__ENTER_FUNCTION

	return Player::ProcessOutput( ) ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayer::SendPacket( Packet* pPacket )
{
__ENTER_FUNCTION

	return Player::SendPacket( pPacket ) ;

__LEAVE_FUNCTION

	return false ;
}


bool LoginPlayer::HeartBeat( uint32_t uTime )
{
__ENTER_FUNCTION

	if( IsDisconnect() )
	{
		m_LeftTimeToQuit -= (uTime-m_CurrentTime) ;
		if( m_LeftTimeToQuit<0 )
		{//真正执行退出处理
  		  return false ;
		}
	}

	m_CurrentTime =uTime ;

	//如果Player在一定时间内没有收到任何消息，则断开客户端连接
	//在Player处理消息前会执行ResetKick函数修正m_KickTime信息
	if(uTime>m_KickTime+MAX_KICK_TIME && GetPlayerStatus() != PS_LOGIN_PROCESS_TURN)
	{
		Log::SaveLog( LOGIN_LOGFILE, "ERROR: Player::HeartBeat Didn't recv message for too long time. Kicked!" ) ;
		return false ;
	}
	
	if(GetPlayerStatus() == PS_LOGIN_PROCESS_TURN)  //排队状态的玩家，每格MAX_TRUN_MESSAGE_TIME
													//时间向客户端发送排队消息
	{
// 		if(uTime>m_LastSendProcessTurn+MAX_TRUN_MESSAGE_TIME)
// 		{
// 			m_LastSendProcessTurn = uTime;
// 		}

	}

	return Player::HeartBeat(uTime) ;

__LEAVE_FUNCTION

	return false ;
}


bool LoginPlayer::FreeOwn( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

	g_pPlayerPool->DelPlayer( PlayerID() ) ;

	return true ;

__LEAVE_FUNCTION

	return false ;
}



void LoginPlayer::Init( )
{
__ENTER_FUNCTION

	SetDisconnect( false ) ;
	ResetKick() ;//开始踢出计时
	

__LEAVE_FUNCTION
}




void LoginPlayer::Disconnect( )
{
__ENTER_FUNCTION

	_MY_TRY
	{
		m_LeftTimeToQuit = g_Config.m_ConfigInfo.m_DisconnectTime ;
		SetDisconnect( true ) ;
	}
	_MY_CATCH
	{
	}

	Player::Disconnect( ) ;

__LEAVE_FUNCTION
}

void LoginPlayer::ResetKick( )
{
__ENTER_FUNCTION

	m_KickTime = g_pTimeManager->CurrentSavedTime() ;

	Player::ResetKick( ) ;

__LEAVE_FUNCTION
}

