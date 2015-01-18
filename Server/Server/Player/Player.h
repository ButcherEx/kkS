//
// Player 是指已经通过网络连接进入服务器端程序的客户
//
//
//

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "BaseLib.h"
#include "Role.h"
#include "PlayerStatus.h"


//如果在一定时间内没有收到任何消息，则断开此客户端的网络连接
#define MAX_KICK_TIME 300000


class Player
{
public :
	Player( bool bIsServer=false ) ;
	~Player( ) ;

	virtual bool	ProcessInput( ) ;
	virtual bool	ProcessOutput( ) ;
	virtual bool	ProcessCommand( bool Option = true ) ;
	virtual bool	HeartBeat( uint32_t uTime=0 ) ;
	virtual bool	SendPacket( Packet* pPacket ) ;
public :
	//读取当前玩家的Socket类
	//网络连接接口
	Socket&		GetSocket(){ return m_Socket ; } ;

	//断开与当前玩家的网络连接
	virtual void			Disconnect( ) ;

	//判断当前玩家的网络连接是否有效
	virtual	bool			IsValid( ) ; 

	//清除当前玩家网络连接数据和缓存数据
	virtual	void			CleanUp( ) ;
protected :
	Role					m_Role;
	//网络连接句柄
	Socket					m_Socket ;

	//输入输出数据缓存
	SocketInputStream		m_SocketInputStream ;
	SocketOutputStream		m_SocketOutputStream ;
public:
	virtual uint32_t Handle(const PBMessage& rMsg) { return 0; };
	virtual uint32_t Handle(const CG_LOGIN& rMsg) { m_Role.Handle(rMsg); };
};

POOL_DECL(Player);

typedef TList<PlayerPtr> PlayerPtrList;
#endif
