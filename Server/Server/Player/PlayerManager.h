

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

	//�������
	bool				Select( ) ;
	//���ݽ��ܽӿ�
	bool				ProcessInputs( ) ;
	//���ݷ��ͽӿ�
	bool				ProcessOutputs( ) ;
	//�쳣���Ӵ���
	bool				ProcessExceptions( ) ;
	//��Ϣִ��
	bool				ProcessCommands( ) ;

	//�߼��ӿ�
	virtual bool		HeartBeat( uint32_t uTime=0 ) ;

	//����һ�����
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
