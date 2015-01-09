

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
