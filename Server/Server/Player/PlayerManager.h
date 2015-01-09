

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

	//逻辑接口
	virtual bool		HeartBeat( uint32_t uTime=0 ) ;

	//增加一个玩家
	bool				AddPlayer( PlayerPtr Ptr ) ;
	void				RemovePlayer( PlayerPtr Ptr, int32_t reason = -1 ) ;
public:
	uint32_t			GetPlayerNumber( ){ return m_nPlayers ; } ;
	bool				HasPlayer( ){ return m_nPlayers > 0 ; } ;
protected :
	//
	PlayerPtrList	m_PlayerPtrList;
	uint32_t		m_nPlayers ;
};




#endif
