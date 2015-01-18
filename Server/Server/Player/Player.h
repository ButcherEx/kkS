//
// Player ��ָ�Ѿ�ͨ���������ӽ���������˳���Ŀͻ�
//
//
//

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "BaseLib.h"
#include "Role.h"
#include "PlayerStatus.h"


//�����һ��ʱ����û���յ��κ���Ϣ����Ͽ��˿ͻ��˵���������
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
	//��ȡ��ǰ��ҵ�Socket��
	//�������ӽӿ�
	Socket&		GetSocket(){ return m_Socket ; } ;

	//�Ͽ��뵱ǰ��ҵ���������
	virtual void			Disconnect( ) ;

	//�жϵ�ǰ��ҵ����������Ƿ���Ч
	virtual	bool			IsValid( ) ; 

	//�����ǰ��������������ݺͻ�������
	virtual	void			CleanUp( ) ;
protected :
	Role					m_Role;
	//�������Ӿ��
	Socket					m_Socket ;

	//����������ݻ���
	SocketInputStream		m_SocketInputStream ;
	SocketOutputStream		m_SocketOutputStream ;
public:
	virtual uint32_t Handle(const PBMessage& rMsg) { return 0; };
	virtual uint32_t Handle(const CG_LOGIN& rMsg) { m_Role.Handle(rMsg); };
};

POOL_DECL(Player);

typedef TList<PlayerPtr> PlayerPtrList;
#endif
