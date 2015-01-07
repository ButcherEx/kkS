//
// Player ��ָ�Ѿ�ͨ���������ӽ���������˳���Ŀͻ�
//
//
//

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "BaseLib.h"
#include "PlayerStatus.h"



#define MAX_CACHE_SIZE 1024

//�����һ��ʱ����û���յ��κ���Ϣ����Ͽ��˿ͻ��˵���������
#define MAX_KICK_TIME 300000
//�Ŷ���Ϣ����ʱ��
#define MAX_TRUN_MESSAGE_TIME	5000 


enum PACKET_FLAG
{
	PF_NONE=0 ,
	PF_REMOVE ,
};

struct ASYNC_PACKET
{
	Packet*		m_pPacket ;
	PlayerID_t	m_PlayerID ;
	uint32_t		m_Flag ;

	ASYNC_PACKET( )
	{
		m_pPacket = NULL ;
		m_PlayerID = INVALID_ID ;
		m_Flag = PF_NONE ;
	};
	~ASYNC_PACKET( )
	{
		SAFE_DELETE( m_pPacket ) ;
		m_PlayerID = INVALID_ID ;
		m_Flag = PF_NONE ;
	};
};



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

	virtual void	Encrypt_SC(CHAR* header, uint32_t uLen, uint32_t uBeginPlace)=0;

	virtual void	DecryptHead_CS(CHAR* header)=0;

	virtual void	Decrypt_CS(CHAR* header, uint32_t uLen, uint32_t uBeginPlace)=0;

public :
	//ͨ�ýӿ�

	virtual bool	IsLoginPlayer( )=0 ;
	virtual bool	IsServerPlayer( )=0 ;
	//������ͨ���Ż������ж��ǲ���ʹ�ô�����������û���ʹ���ĸ�������������û�
	//INT				WhereThisPlayerFrom(void) const;

	//��ȡ�������PID�Ľӿڣ�PlayerID ����PlayerPool��������������ֵ
	//PlayerID���ڱ�ʶһ���ͻ�������������ϵͳ�е�����λ����Ϣ
	PlayerID_t	PlayerID(){ return m_PID ; } ;
	void		SetPlayerID( PlayerID_t id ){ m_PID = id ; } ;

	//��ȡ�������UID�Ľӿڣ�UserID ����UserPool��������������ֵ
	//UserID���ڱ�ʶ��ҵ���Ϸ�洢λ����Ϣ
	ID_t		UserID(){ return m_UID ; } ;
	void		SetUserID( ID_t id ){ m_UID = id ; } ;

	//��ȡ�������PMID�Ľӿڣ�PlayerManagerID ����PlayerManager��������������ֵ
	//PlayerManagerID���ڱ�ʶ��ǰ������ڵ�ĳ��PlayerManager�е�λ����Ϣ��ÿ��
	//PlayerManagerλ��һ���������߳��
	ID_t		PlayerManagerID(){ return m_PlayerManagerID ; } ;
	void		SetPlayerManagerID( ID_t id ){ m_PlayerManagerID = id ; } ;

	//��ȡ��ǰ��ҵ�Socket��
	//�������ӽӿ�
	Socket&		GetSocket(){ return m_Socket ; } ;

	//�Ͽ��뵱ǰ��ҵ���������
	virtual void			Disconnect( ) ;

	//�жϵ�ǰ��ҵ����������Ƿ���Ч
	virtual	bool			IsValid( ) ; 

	//�����ǰ��������������ݺͻ�������
	virtual	void			CleanUp( ) ;

	//�жϵ�ǰ������ݿ��Ƿ��ǿտ飻
	//����ǿտ飬����Ա�PlayerPool�����ȥ���ڽ���������
	bool					IsEmpty( ) { return m_IsEmpty ; }
	void					SetEmpty( bool opt = true ) { m_IsEmpty = opt ; }

	bool					IsDisconnect( ) { return m_IsDisconnect ; }
	void					SetDisconnect( bool opt = true ) { m_IsDisconnect = opt ; }

	virtual void			ResetKick( ) ;

private :
	bool					m_IsEmpty ;
	bool					m_IsDisconnect ;


protected :
	//Player ID
	//ͳһ��PlayerPool����
	PlayerID_t				m_PID ;	
	//User ID
	//ͳһ��UserPool����
	ID_t					m_UID ;	
	//��PlayerManager����player id������±�
	ID_t					m_PlayerManagerID ;

	//�������Ӿ��
	Socket					m_Socket ;

	//����������ݻ���
	SocketInputStream		m_SocketInputStream ;
	SocketOutputStream		m_SocketOutputStream ;



	BYTE					m_PacketIndex ;
};

#endif
