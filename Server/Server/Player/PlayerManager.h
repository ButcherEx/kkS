

#ifndef __PLAYERMANAGER_H__
#define __PLAYERMANAGER_H__

#include "Player.h"
#include "Socket.h"
#include "GameDefine.h"

//����һȺ��ҹ��ܵĻ���ģ��,һ��Ϊÿ����������һ��PlayerManager
//������һ���������߳���ִ��
class PlayerManager
{
public :
	PlayerManager( ) ;
	~PlayerManager( ) ;

	//�����ǰģ���ڵ�������Ϣ
	void				CleanUp( ) ;

	//�߼��ӿ�
	virtual bool		HeartBeat( uint32_t uTime=0 ) ;

	//����һ�����
	bool				AddPlayer( Player* pPlayer ) ;

	bool				AddPlayer( Player* pPlayer,uint32_t MaxPlayer);
	//ɾ��һ�����

	void				RemovePlayer( PlayerID_t pid ) ;

	PlayerID_t*			GetPlayers( ){ return m_pPlayers ; } ;

	uint32_t			GetPlayerNumber( ){ return m_nPlayers ; } ;
	bool				HasPlayer( ){ return m_nPlayers > 0 ; } ;



	bool				AddPlayer( PlayerID_t pid ) ;		//δ�ṩ���ܽӿ�

protected :
	//
	PlayerID_t	m_pPlayers[MAX_PLAYER] ;
	uint32_t		m_nPlayers ;
	


};




#endif
