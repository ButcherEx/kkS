//#include "stdafx.h"


#include "LoginPlayerManager.h"
#include "Config.h"
#include "Log.h"
#include "PlayerPool.h"
#include "TimeManager.h"
#include "Assertx.h"
#include "GameUtil.h"
#include "PlayerStatus.h"


#define ACCEPT_ONESTEP 50

#define MAX_LOGIN_PLAYER_AUTH_TIME	30000

LoginPlayerManager* g_pLoginPlayerManager = NULL ;

LoginPlayerManager::LoginPlayerManager( )
{
__ENTER_FUNCTION

	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_nFDSize = 0 ;

	m_PacketQue = new ASYNC_PACKET[MAX_CACHE_SIZE] ;
	Assert(m_PacketQue) ;
	m_QueSize = MAX_CACHE_SIZE ;
	m_Head = 0 ;
	m_Tail = 0 ;

__LEAVE_FUNCTION
}

LoginPlayerManager::~LoginPlayerManager( )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pServerSocket ) ;
	SAFE_DELETE_ARRAY( m_PacketQue ) ;

__LEAVE_FUNCTION
}

bool LoginPlayerManager::Init( )
{
__ENTER_FUNCTION

	
	int32_t LoginPort = 5555;
	
	m_pServerSocket = new ServerSocket( LoginPort) ;
	Assert( m_pServerSocket ) ;

	m_pServerSocket->setNonBlocking() ;

	m_SocketID = m_pServerSocket->getSOCKET() ;
	Assert( m_SocketID != INVALID_SOCKET ) ;

	FD_SET(m_SocketID , &m_ReadFDs[SELECT_BAK]);
	FD_SET(m_SocketID , &m_ExceptFDs[SELECT_BAK]);

	m_MinFD = m_MaxFD = m_SocketID;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	Log::SaveLog(LOGIN_LOGFILE,"LoginPlayerManager Start ServerSocket At Port: %d",LoginPort);


__LEAVE_FUNCTION

	return true ;
}

bool LoginPlayerManager::Select( )
{
__ENTER_FUNCTION

	m_Timeout[SELECT_USE].tv_sec  = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE]   = m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE]  = m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

	MySleep(100) ;

	_MY_TRY 
	{
		int32_t iRet = SocketAPI::select_ex(	(int32_t)m_MaxFD+1 , 
											&m_ReadFDs[SELECT_USE] , 
											&m_WriteFDs[SELECT_USE] , 
											&m_ExceptFDs[SELECT_USE] , 
											&m_Timeout[SELECT_USE] ) ;
		Assert( iRet!=SOCKET_ERROR ) ;
	} 
	_MY_CATCH
	{
		Log::SaveLog( LOGIN_LOGFILE, "ERROR: IncomingPlayerManager::Select( )..." ) ;
	}

	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::RemovePlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	bool ret = false ;
	
	//第一步：清除PlayerManager中的信息
	ret = DelPlayer( pPlayer->PlayerID() ) ;
	Assert( ret ) ;

	//第二步：清除PlayerPool中的信息，注意此步骤必须放在最后，
	//当调用此操作后，当前Player就有可能会被马上分配给新接入玩家
	((LoginPlayer*)pPlayer)->FreeOwn( ) ;


	Log::SaveLog( LOGIN_LOGFILE, "LoginPlayerManager::RemovePlayer(PID:%d)...OK", pPlayer->PlayerID() ) ;

	return ret ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::ProcessInputs( )
{
__ENTER_FUNCTION

	bool ret = false ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return true ;
	}

	//新连接接入：
	if( FD_ISSET(m_SocketID,&m_ReadFDs[SELECT_USE]) )
	{
		for( int32_t i=0; i<ACCEPT_ONESTEP; i++ )
		{
			if( !AcceptNewConnection() )
				break;
		}
	}

	//数据读取
	uint32_t nPlayerCount = GetPlayerNumber() ;
	for( uint32_t i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket().getSOCKET() ;
		if( s == m_SocketID )
			continue ;

		if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket().isSockError() )
			{//连接出现错误
				RemovePlayer( pPlayer ) ;
			}
			else
			{//连接正常
				_MY_TRY
				{
					ret = pPlayer->ProcessInput( ) ;
					if( !ret )
					{
						RemovePlayer( pPlayer ) ;
					}
				}
				_MY_CATCH
				{
					RemovePlayer( pPlayer ) ;
				}
			}
		}
	}


	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::ProcessOutputs( )
{
__ENTER_FUNCTION

	bool ret = false ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return true ;
	}

	//数据发送
	uint32_t nPlayerCount = GetPlayerNumber() ;
	for( uint32_t i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket().getSOCKET() ;
		if( s == m_SocketID )
			continue ;

		if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket().isSockError() )
			{//连接出现错误
				RemovePlayer( pPlayer ) ;
			}
			else
			{//连接正常
				_MY_TRY
				{
					ret = pPlayer->ProcessOutput( ) ;
					if( !ret )
					{
						RemovePlayer( pPlayer ) ;
					}
				}
				_MY_CATCH
				{
					RemovePlayer( pPlayer ) ;
				}
			}
		}
	}

	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::ProcessExceptions( )
{
__ENTER_FUNCTION

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return true ;
	}

	uint32_t nPlayerCount = GetPlayerNumber() ;
	for( uint32_t i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		//某个玩家断开网络连接

		LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket().getSOCKET() ;
		if( s == m_SocketID )
		{//侦听句柄出现问题，难。。。
			Assert( false ) ;
			continue ;
		}

		if( FD_ISSET( s, &m_ExceptFDs[SELECT_USE] ) )
		{
			RemovePlayer( pPlayer ) ;
		}
	}





	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::ProcessCommands( )
{
__ENTER_FUNCTION

	bool ret ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return true ;
	}

	uint32_t nPlayerCount = GetPlayerNumber() ;
	for( uint32_t i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket().getSOCKET() ;
		if( s == m_SocketID )
			continue ;

		if( pPlayer->GetSocket().isSockError() )
		{//连接出现错误
			RemovePlayer( pPlayer ) ;
		}
		else
		{//连接正常
			_MY_TRY
			{
				ret = pPlayer->ProcessCommand( false ) ;
				if( !ret )
				{
					RemovePlayer( pPlayer ) ;
				}
			}
			_MY_CATCH
			{
				RemovePlayer( pPlayer ) ;
			}
		}
	}


	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::AcceptNewConnection( )
{
__ENTER_FUNCTION

	int32_t iStep = 0 ;
	bool ret = false ;

	//从玩家池中找出一个空闲的玩家数据集
	LoginPlayer* client = g_pPlayerPool->NewPlayer() ;
//	Assert( client ) ;
	if( client==NULL )
		return false ;

	iStep = 5 ;
	client->CleanUp( ) ;

	int32_t fd = INVALID_SOCKET ;

	iStep = 10 ;
	_MY_TRY
	{
		//接受客户端接入Socket句柄
		ret = m_pServerSocket->accept( client->GetSocket() ) ;
		if( !ret )
		{
			client->CleanUp() ;
			g_pPlayerPool->DelPlayer( client->PlayerID() ) ;
			return false ;
		}
	}
	_MY_CATCH
	{
		iStep += 1000 ;
		goto EXCEPTION ;
	}

	_MY_TRY
	{
		iStep = 30 ;
		fd = (int32_t)client->GetSocket().getSOCKET();
		if( fd == INVALID_SOCKET )
		{
			Assert(false) ;
			goto EXCEPTION ;
		}

		iStep = 40 ;
		ret = client->GetSocket().setNonBlocking() ;
		if( !ret )
		{
			Assert(false) ;
			goto EXCEPTION ;
		}

		iStep = 50 ;
		if( client->GetSocket().getSockError() )
		{
			Assert(false) ;
			goto EXCEPTION ;
		}

		iStep = 60 ;
		ret = client->GetSocket().setLinger(0) ;
		if( !ret )
		{	
			Assert(false) ;
			goto EXCEPTION ;
		}

		iStep = 70 ;
		//初始化基本玩家信息
		client->Init( ) ;
		//设置当前客户端连接的状态
		client->SetPlayerStatus( PS_LOGIN_CONNECT ) ;
		client->m_ConnectTime = g_pTimeManager->CurrentTime();

		iStep = 80 ;
		_MY_TRY
		{
			ret = AddPlayer( client ) ;
			if( !ret )
			{
				Assert(false) ;
				goto EXCEPTION ;
			}
		}
		_MY_CATCH
		{
			iStep += 10000 ;
			goto EXCEPTION ;
		}
	}
	_MY_CATCH
	{
		iStep += 100000 ;
	}

	Log::SaveLog( LOGIN_LOGFILE, "AcceptNewConnection(SOCKET=%d, IP=%s)...OK", 
		client->GetSocket().getSOCKET(), client->GetSocket().m_Host ) ;

	return true ;


EXCEPTION:
	Log::SaveLog( LOGIN_LOGFILE, "AcceptNewConnection(SOCKET=%d,IP=%s,Step=%d)...Fails", 
		client->GetSocket().getSOCKET(), client->GetSocket().m_Host,iStep) ;

	client->CleanUp() ;
	g_pPlayerPool->DelPlayer( client->PlayerID() ) ;
	
	

	return false ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::AddPlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	if( m_nFDSize>=FD_SETSIZE )
	{//已经超出能够检测的网络句柄最大数；
		Assert(false) ;
		return false ;
	}

	bool ret = PlayerManager::AddPlayer( pPlayer ) ;
	if( !ret )
	{
		Assert(false) ;
		return false ;
	}

	//在IncomingPlayerManager模块中是没有场景ID值的
	LoginPlayer* pLoginPlayer = (LoginPlayer*)pPlayer ;
//	pLoginPlayer->SetSceneID( INVALID_ID ) ;

	SOCKET fd = pPlayer->GetSocket().getSOCKET() ;
	Assert( fd != INVALID_SOCKET ) ;

	m_MinFD = min(fd , m_MinFD);
	m_MaxFD = max(fd , m_MaxFD);

	FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);

	m_nFDSize++ ;

	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::DelPlayer( PlayerID_t pid )
{
__ENTER_FUNCTION

	Assert( pid!=INVALID_ID ) ;
	LoginPlayer* pLoginPlayer = g_pPlayerPool->GetPlayer(pid) ;
	Assert( pLoginPlayer ) ;

	SOCKET fd = pLoginPlayer->GetSocket().getSOCKET() ;

	Assert( m_MinFD!=INVALID_SOCKET ) ;
	Assert( m_MaxFD!=INVALID_SOCKET ) ;
	if( fd==INVALID_SOCKET ) 
	{
		Assert(false) ;
	}

	if( fd==m_MinFD ) 
	{
		SOCKET s = m_MaxFD ;
		uint32_t nPlayerCount = GetPlayerNumber() ;
		for( uint32_t i=0; i<nPlayerCount; i++ )
		{
			if( m_pPlayers[i]==INVALID_ID )
				continue ;

			LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
			Assert( pPlayer ) ;
			if( pPlayer==NULL )
				continue ;


			SOCKET temp = pPlayer->GetSocket().getSOCKET() ;
			if( temp == fd )
				continue ;
			if( temp==INVALID_SOCKET )
				continue ;

			if( temp < s )
			{
				s = temp ;
			}
		}

		if( m_MinFD == m_MaxFD )
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET ;
		}
		else
		{
			if( s > m_SocketID )
			{
				m_MinFD = m_SocketID ;
			}
			else
			{
				m_MinFD = s ;
			}
		}
	} 
	else if( fd==m_MaxFD ) 
	{
		SOCKET s = m_MinFD ;
		uint32_t nPlayerCount = GetPlayerNumber() ;
		for( uint32_t i=0; i<nPlayerCount; i++ )
		{
			if( m_pPlayers[i]==INVALID_ID )
				continue ;

			LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
			Assert( pPlayer ) ;
			if( pPlayer==NULL )
				continue ;


			SOCKET temp = pPlayer->GetSocket().getSOCKET() ;
			if( temp == fd )
				continue ;
			if( temp==INVALID_SOCKET )
				continue ;

			if( temp > s )
			{
				s = temp ;
			}
		}

		if( m_MaxFD == m_MinFD )
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET ;
		}
		else
		{
			if( s < m_SocketID )
			{
				m_MaxFD = m_SocketID ;
			}
			else
			{
				m_MaxFD = s ;
			}
		}
	}

	FD_CLR(fd , &m_ReadFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ReadFDs[SELECT_USE]);
	FD_CLR(fd , &m_WriteFDs[SELECT_BAK]);
	FD_CLR(fd , &m_WriteFDs[SELECT_USE]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_USE]);

	m_nFDSize-- ;
	Assert( m_nFDSize>=0 ) ;

	PlayerManager::RemovePlayer( pid ) ;

	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::HeartBeat( )
{
__ENTER_FUNCTION

	bool ret ;

	uint32_t uTime = g_pTimeManager->CurrentTime() ;
	
	uint32_t nPlayerCount = GetPlayerNumber() ;
	for( uint32_t i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i] == INVALID_ID )
			continue ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		if( pPlayer==NULL )
		{
			Assert(false) ;
			return false ;
		}

		ret = pPlayer->HeartBeat(uTime ) ;
		if( !ret )
		{//如果逻辑操作返回失败，则需要断开当前连接
			ret = RemovePlayer( pPlayer ) ;
			Assert( ret ) ;
		}
		else
		{
			LoginPlayer* pLoginPlayer = (LoginPlayer*)pPlayer;
			Assert(pLoginPlayer);

			if(pLoginPlayer->GetPlayerStatus() == PS_LOGIN_CONNECT)
			{
				if(uTime-pLoginPlayer->m_ConnectTime>MAX_LOGIN_PLAYER_AUTH_TIME)
				{
					ret = RemovePlayer(pPlayer);
					Assert( ret ) ;
				}
			}
		}
		
	}


	return true ;

__LEAVE_FUNCTION

	return false ;
}


void LoginPlayerManager::RemoveAllPlayer( )
{
__ENTER_FUNCTION

	uint32_t nPlayerCount = GetPlayerNumber() ;
	for( uint32_t i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[0] == INVALID_ID )
			break ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[0]) ;
		if( pPlayer==NULL )
		{
			Assert(false) ;
			break ;
		}
	
		RemovePlayer( pPlayer ) ;
	}

__LEAVE_FUNCTION
}


bool LoginPlayerManager::ProcessCacheCommands( )
{
__ENTER_FUNCTION

	bool ret = false ;

	for( uint32_t i=0; i<m_QueSize; i++ )
	{
		Packet* pPacket = NULL ;
		PlayerID_t PlayerID ;
		uint32_t Flag ;

		ret = RecvPacket( pPacket, PlayerID, Flag ) ;
		if( !ret )
			break ;

		Assert( pPacket ) ;

		if( Flag==PF_REMOVE )
		{
			pPacket->FreeOwn();
			continue ;
		}

		bool bNeedRemove = true ;

		if( PlayerID==INVALID_ID )
		{
			_MY_TRY
			{
			//	pPacket->Execute(NULL) ;
			}
			_MY_CATCH
			{
			}
		}
		else
		{
			_MY_TRY
			{
				Player* pPlayer = g_pPlayerPool->GetPlayer( PlayerID ) ;
				Assert( pPlayer ) ;
				uint32_t uret = 0; //pPacket->Execute(pPlayer) ;
				if( uret == PACKET_EXE_ERROR )
				{
					RemovePlayer( pPlayer ) ;
					MovePacket( PlayerID ) ;
				}
				else if( uret == PACKET_EXE_BREAK )
				{
				}
				else if( uret == PACKET_EXE_CONTINUE )
				{
				}
				else if( uret == PACKET_EXE_NOTREMOVE )
				{
					bNeedRemove = false ;
				}
				else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
				{
					bNeedRemove = false ;

					RemovePlayer( pPlayer ) ;
					MovePacket( PlayerID ) ;
				}
			}
			_MY_CATCH
			{
			}
		}

		//回收消息
		if( bNeedRemove ) pPacket->FreeOwn();
	}


	
	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::MovePacket( PlayerID_t PlayerID )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	uint32_t Cur = m_Head ;

	for( uint32_t i=0; i<m_QueSize; i++ )
	{
		if( m_PacketQue[Cur].m_pPacket == NULL )
			break ;

		if( m_PacketQue[Cur].m_PlayerID == PlayerID )
		{
			m_PacketQue[Cur].m_Flag = PF_REMOVE ;
		}

		Cur ++ ;
		if( Cur>=m_QueSize ) Cur = 0 ;
	}

	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::ResizeCache( )
{
__ENTER_FUNCTION

	ASYNC_PACKET* pNew = new ASYNC_PACKET[m_QueSize+MAX_CACHE_SIZE] ;
	if( pNew==NULL )
		return false ;

	memcpy( pNew, &(m_PacketQue[m_Head]), sizeof(ASYNC_PACKET)*(m_QueSize-m_Head) ) ;
	if( m_Head!=0 )
	{
		memcpy( &(pNew[m_QueSize-m_Head]), &(m_PacketQue[0]), sizeof(ASYNC_PACKET)*(m_Head) ) ;
	}

	memset( m_PacketQue, 0, sizeof(ASYNC_PACKET)*m_QueSize ) ;
	SAFE_DELETE_ARRAY( m_PacketQue ) ;
	m_PacketQue = pNew ;

	m_Head = 0 ;
	m_Tail = m_QueSize ;
	m_QueSize = m_QueSize+MAX_CACHE_SIZE ;

	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::SendPacket( Packet* pPacket, PlayerID_t PlayerID, uint32_t Flag )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_PacketQue[m_Tail].m_pPacket )
	{//缓冲区满
		bool ret = ResizeCache( ) ;
		Assert( ret ) ;
	}

	m_PacketQue[m_Tail].m_pPacket = pPacket ;
	m_PacketQue[m_Tail].m_PlayerID = PlayerID ;
	m_PacketQue[m_Tail].m_Flag = Flag ;

	m_Tail ++ ;
	if( m_Tail>=m_QueSize ) 
		m_Tail = 0 ;

	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool LoginPlayerManager::RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint32_t& Flag )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_PacketQue[m_Head].m_pPacket==NULL )
	{//缓冲区中没有消息
		return false ;
	}

	pPacket = m_PacketQue[m_Head].m_pPacket ;
	PlayerID = m_PacketQue[m_Head].m_PlayerID ;
	Flag = m_PacketQue[m_Head].m_Flag ;

	m_PacketQue[m_Head].m_pPacket = NULL ;
	m_PacketQue[m_Head].m_PlayerID = INVALID_ID ;
	m_PacketQue[m_Head].m_Flag = PF_NONE ;

	m_Head ++ ;
	if( m_Head>=m_QueSize ) 
		m_Head = 0 ;

	return true ;

__LEAVE_FUNCTION

	return false ;
}


