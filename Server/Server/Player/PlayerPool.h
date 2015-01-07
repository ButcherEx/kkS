

#ifndef __PLAYERPOOL_H__
#define __PLAYERPOOL_H__

#include "Base.h"
#include "LoginPlayer.h"
#include "GameDefine.h"



//����������ݵ����ݳ�
//

class PlayerPool
{

	
public :
	PlayerPool( ) ;
	~PlayerPool( ) ;

	//��ʼ�����ݳ�
	bool				Init( uint32_t MaxPlayerCount = MAX_POOL_SIZE ) ;
	
	//ȡ��ShareMem�ĳ�ʼ��
	//BOOL				PostInit();


	//������ҵ�PlayerIDȡ������ָ��
	LoginPlayer*			GetPlayer( PlayerID_t PlayerID )
	{
		if( PlayerID==INVALID_ID || PlayerID>=(PlayerID_t)m_MaxPlayerCount || PlayerID<0 )
			return NULL ;
		return &(m_pPlayer[PlayerID]) ;
	};

	//�ӿ������ݳ������һ���������
	LoginPlayer*			NewPlayer( ) ;
	LoginPlayer*			NewPlayer(PlayerID_t PlayerID);
	//��һ�����������ջ���
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
