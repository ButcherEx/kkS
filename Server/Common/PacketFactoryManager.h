


#ifndef __PACKETFACTORYMANAGER_H__
#define __PACKETFACTORYMANAGER_H__

#include "PacketFactory.h"
#include "PacketDefine.h"


class PacketFactoryManager 
{
public :
	
	PacketFactoryManager( ) ;
	
	~PacketFactoryManager( ) ;

private :
	void				AddFactory( PacketFactory* pFactory ) ;
	
public :
	//�ⲿ����ͨ�ýӿ�

	//��ʼ���ӿ�
	bool				Init( ) ;
	//������Ϣ���ʹ��ڴ��������Ϣʵ�����ݣ�������߳�ͬʱ���ã�
	Packet*				CreatePacket( PacketID_t packetID ) ;
	//������Ϣ����ȡ�ö�Ӧ��Ϣ�����ߴ磨������߳�ͬʱ���ã�
	uint32_t				GetPacketMaxSize( PacketID_t packetID ) ;
	//ɾ����Ϣʵ�壨������߳�ͬʱ���ã�
	void				RemovePacket( Packet* pPacket ) ;
	
	void				Lock( ){ m_Lock.lock() ; } ;
	void				Unlock( ){ m_Lock.unlock() ; } ;

private :
	
	PacketFactory **	m_Factories ;
	
	USHORT				m_Size ;

	MyLock				m_Lock ;

public :
	uint32_t*				m_pPacketAllocCount ;

};

extern PacketFactoryManager* g_pPacketFactoryManager ;







#endif
