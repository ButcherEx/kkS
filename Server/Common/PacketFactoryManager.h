


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
	//外部调用通用接口

	//初始化接口
	bool				Init( ) ;
	//根据消息类型从内存里分配消息实体数据（允许多线程同时调用）
	Packet*				CreatePacket( PacketID_t packetID ) ;
	//根据消息类型取得对应消息的最大尺寸（允许多线程同时调用）
	uint32_t				GetPacketMaxSize( PacketID_t packetID ) ;
	//删除消息实体（允许多线程同时调用）
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
