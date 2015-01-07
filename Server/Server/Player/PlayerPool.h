

#ifndef __PLAYERPOOL_H__
#define __PLAYERPOOL_H__

#include "Base.h"
#include "LoginPlayer.h"
#include "GameDefine.h"



//分配玩家数据的数据池
//

class PlayerPool
{

	
public :
	PlayerPool( ) ;
	~PlayerPool( ) ;

	//初始化数据池
	bool				Init( uint32_t MaxPlayerCount = MAX_POOL_SIZE ) ;
	
	//取得ShareMem的初始化
	//BOOL				PostInit();


	//根据玩家的PlayerID取得数据指针
	LoginPlayer*			GetPlayer( PlayerID_t PlayerID )
	{
		if( PlayerID==INVALID_ID || PlayerID>=(PlayerID_t)m_MaxPlayerCount || PlayerID<0 )
			return NULL ;
		return &(m_pPlayer[PlayerID]) ;
	};

	//从空闲数据池里分配一个玩家数据
	LoginPlayer*			NewPlayer( ) ;
	LoginPlayer*			NewPlayer(PlayerID_t PlayerID);
	//将一个玩家数据清空回收
	void				DelPlayer( PlayerID_t PlayerID ) ;

private :
	LoginPlayer*		m_pPlayer ;
	uint32_t				m_Position ;

	MyLock				m_Lock ;
	uint32_t				m_PlayerCount ;

	uint32_t				m_MaxPlayerCount ;


	
	void				SetPosition(uint32_t pos);



};


extern PlayerPool* g_pPlayerPool ;


#endif
