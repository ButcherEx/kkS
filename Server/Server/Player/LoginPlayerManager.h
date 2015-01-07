
#ifndef __LOGINPLAYERMANAGER_H__
#define __LOGINPLAYERMANAGER_H__

#include "BaseLib.h"
#include "PlayerManager.h"
#include "GameDefine.h"


class LoginPlayerManager : public PlayerManager
{
public :
	LoginPlayerManager() ;
	~LoginPlayerManager() ;

	//ģ���ʼ���ӿ�
	bool				Init( ) ;
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
	//�����ӽ��մ���
	bool				AcceptNewConnection( ) ;
	//�߼��ӿ�
	virtual bool		HeartBeat( ) ;
	//��������Ϣ
	bool				ProcessCacheCommands( ) ;
	//��ȡ������Ϣ
	bool				RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint32_t& Flag ) ;
	//���µ��������С
	bool				ResizeCache( ) ;
	//ɾ����ЧPlayer����Ϣ
	bool				MovePacket( PlayerID_t PlayerID ) ;

public :
	//ͨ�ýӿ�

	//��Player���ݼ���ϵͳ��
	bool				AddPlayer( Player* pPlayer ) ;
	//��ӵ��fd�����������ݴӵ�ǰϵͳ�����
	bool				DelPlayer( PlayerID_t pid ) ;

	//�����쳣��Player��Ϣ���������ϵͳ�е���ϢҲ���
	//�Ͽ���ҵ�����
	bool				RemovePlayer( Player* pPlayer ) ;
	void				RemoveAllPlayer( ) ;

	//*********
	//*********
	//�˽ӿ�֧������ͬ�����������ڲ�ͬ�߳��ڵ���
	//�˽ӿ����첽ͨѶ��Ψһ�ӿ�
	//ע�⣺pPacket��Ϣ��Ҫ��g_pPacketFactoryManager�����������������ɾ��
	bool				SendPacket( Packet* pPacket, 
									PlayerID_t PlayerID, 
									uint32_t Flag=PF_NONE ) ;
	//*********
	//*********

private :
	//���������ķ�����Socket
	ServerSocket*		m_pServerSocket ;
	//���������ķ�����SOCKET���ֵ�������ݼ�m_pServerSocket��ӵ�е�SOCKET���ֵ��
	SOCKET				m_SocketID ;

	//
	//�����������
	enum{
		SELECT_BAK = 0,	//��ǰϵͳ��ӵ�е������������
		SELECT_USE = 1,	//����select���õľ������
		SELECT_MAX = 2, //�ṹʹ������
	};
	fd_set		m_ReadFDs[SELECT_MAX];
	fd_set		m_WriteFDs[SELECT_MAX];
	fd_set		m_ExceptFDs[SELECT_MAX];

	timeval		m_Timeout[SELECT_MAX];

	SOCKET		m_MinFD;
	SOCKET					m_MaxFD;

	int32_t						m_nFDSize ;
	//�����������
	//

	MyLock					m_Lock ;


	//��ǰ����Ϣ����
	ASYNC_PACKET*			m_PacketQue ;
	uint32_t					m_QueSize ;
	uint32_t					m_Head ;
	uint32_t					m_Tail ;

public :
	TID			m_ThreadID ;

};

extern LoginPlayerManager* g_pLoginPlayerManager ;

#endif
