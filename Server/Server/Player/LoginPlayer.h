

#ifndef __LOGINPLAYER_H__
#define __LOGINPLAYER_H__

#include "Player.h"

class LoginPlayer : public Player
{
public :
	LoginPlayer( ) ;
	~LoginPlayer( ) ;

	//��Ϣִ�нӿ�
	virtual bool		ProcessCommand( bool Option = true ) ;
	//���ݽ��սӿ�
	virtual bool		ProcessInput( ) ;
	//���ݷ��ͽӿ�
	virtual bool		ProcessOutput( ) ;

//�����ӿڣ����������߼�
	virtual bool		HeartBeat( uint32_t uTime=0 ) ;

	//�������
	virtual void		CleanUp( ) ;


public :
	//Ӧ�ýӿ�
	virtual bool		IsLoginPlayer( ){ return true; } ;
	virtual bool		IsServerPlayer( ){ return false ; } ;

	//���ӳɹ����ʼ����������
	void				Init( ) ;

	//���Player����һ����Ϣ��
	//�˽ӿ�ֻ���ڱ�ִ���߳��ڴ���������ͬ��������
	virtual bool		SendPacket( Packet* pPacket ) ;

	virtual void		Encrypt_SC(CHAR* header, uint32_t uLen, uint32_t uBeginPlace){ENCRYPT(header, uLen, LOGIN_TO_CLIENT_KEY, uBeginPlace)}

	virtual void		DecryptHead_CS(CHAR* header){ENCRYPT_HEAD(header, CLIENT_TO_LOGIN_KEY)}

	virtual void		Decrypt_CS(CHAR* header, uint32_t uLen, uint32_t uBeginPlace){ENCRYPT(header, uLen, CLIENT_TO_LOGIN_KEY, uBeginPlace)}

	//���״̬���á���ȡ�ӿ�
	void				SetPlayerStatus( uint32_t status ){ m_Status = status ; } ;
	uint32_t				GetPlayerStatus( ) { return m_Status ; } ;
	
	//�Ͽ��������ӣ����һ���Player����
	//�����ݱ����պ�������ϻᱻPlayerPool�����ȥʹ��
	bool				FreeOwn( ) ;
	virtual void		ResetKick( ) ;
	virtual void		Disconnect( ) ;
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
private :
	uint32_t					m_Status ;
	GUID_t					m_AccountGuid;				//�˺�guid ��billingsystem ���أ�ͨ����֤�Ժ�

public :
	//��Ϸ����
	uint32_t					m_KickTime ;		//�ж��Ƿ���Ҫ�ߵ���ҵļ�ʱ��
	uint32_t					m_LastSendTime ;	//�ϴη������ݵ�ʱ��
	uint32_t					m_ConnectTime;
	uint32_t					m_CurrentTime ;		//��ǰ�߼�ʱ��
	int32_t						m_LeftTimeToQuit ;	//ʣ�౻����˳���ʱ��
	bool					m_Dirty ;			//�˱�־��ʾ��ǰ�����Ѿ���Ч��
												//����Ҫ�����κ�״̬��Ϣ���������ݷ�����
};





#endif
