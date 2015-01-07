


#ifndef __SERVERPLAYER_H__
#define __SERVERPLAYER_H__

#include "Base.h"
#include "Player.h"
#include "Config.h"


//���������ָ��������ķ������˳����ڵ�ǰ�����ڵ����ݱ���

class ServerPlayer : public Player
{
public :
	ServerPlayer( bool bIsServer=true ) ;
	~ServerPlayer( ) ;

	//���ݽ���
	virtual bool	ProcessInput( ) ;
	//���ݷ���
	virtual bool	ProcessOutput( ) ;
	//��Ϣ����
	virtual bool	ProcessCommand( bool Option = true ) ;

	//���
	virtual void	CleanUp( ) ;

	//�߼��ӿ�
	virtual bool	HeartBeat( uint32_t uTime=0 ) ;


public :
	//ͨ�ýӿ�

	virtual bool	IsLoginPlayer( ){ return false ; } ;
	virtual bool	IsServerPlayer( ){ return true ; } ;

	//���õ�ǰ����������ҵ�״̬
	void			SetPlayerStatus( uint32_t status ){ m_Status = status ; } ;

	//�жϵ�ǰ������������Ƿ���Ч
	virtual bool	IsValid( ) ;

	//��˷������˳�������Ϣ
	//�˽ӿڲ�֧���̼߳�ͬ�������ֻ���е�ǰ�߳�������ִ��
	virtual bool	SendPacket( Packet* pPacket ) ;

	virtual void	Encrypt_SC(CHAR* header, uint32_t uLen, uint32_t uBeginPlace){;}

	virtual void	DecryptHead_CS(CHAR* header){;}

	virtual void	Decrypt_CS(CHAR* header, uint32_t uLen, uint32_t uBeginPlace){;}
private :
	//��������״̬
	uint32_t			m_Status ;


};




#endif
