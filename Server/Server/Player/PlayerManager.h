

#ifndef __PLAYERMANAGER_H__
#define __PLAYERMANAGER_H__

#include "Player.h"
#include "Socket.h"
#include "GameDefine.h"

//管理一群玩家功能的基本模块,一般为每个场景创建一个PlayerManager
//并且在一个独立的线程内执行
class PlayerManager
{
public :
	PlayerManager( ) ;
	~PlayerManager( ) ;

	//清除当前模块内的数据信息
	void				CleanUp( ) ;

	//网络侦测
	bool				Select( ) ;
	//数据接受接口
	bool				ProcessInputs( ) ;
	//数据发送接口
	bool				ProcessOutputs( ) ;
	//异常连接处理
	bool				ProcessExceptions( ) ;
	//消息执行
	bool				ProcessCommands( ) ;

	//逻辑接口
	virtual bool		HeartBeat( uint32_t uTime=0 ) ;

	//增加一个玩家
	bool				AddPlayer( PlayerPtr Ptr ) ;
	virtual void		OnAddPlayer(PlayerPtr Ptr, int32_t reason) {}
	void				RemovePlayer( PlayerPtr Ptr, int32_t reason = -1 ) ;
	virtual void		OnRemovePlayer(PlayerPtr ptr, int32_t reason) {}
public:
	uint32_t			GetPlayerNumber( ){ return m_nPlayers ; } ;
	bool				HasPlayer( ){ return m_nPlayers > 0 ; } ;
protected :

	fd_set		m_ReadFDs;
	fd_set		m_WriteFDs;
	fd_set		m_ExceptFDs;

	//
	PlayerPtrList	m_PlayerPtrList;
	uint32_t		m_nPlayers ;
};




#endif
