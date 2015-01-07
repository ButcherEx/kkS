//#include "stdafx.h"


#include "PacketFactoryManager.h"
#include "Assertx.h"

//CL && LC
#if defined (_FOX_CLIENT) && defined (_FOX_LOGIN)
//CL
#include "CLAskCharList.h"


#endif
using namespace Packets;


PacketFactoryManager* g_pPacketFactoryManager = NULL ;


PacketFactoryManager::PacketFactoryManager( ) 
{
__ENTER_FUNCTION

	m_Factories = NULL ;
	m_Size = PACKET_MAX ;

	Assert( m_Size>0 ) ;
	
	m_Factories = new PacketFactory*[ m_Size ];
	Assert( m_Factories ) ;
	m_pPacketAllocCount = new uint32_t[m_Size] ;
	Assert( m_pPacketAllocCount ) ;
	
	for( int32_t i=0; i<m_Size; i++ ) 
	{
		m_Factories[i] = NULL ;
		m_pPacketAllocCount[i] = 0 ;
	}
			
__LEAVE_FUNCTION
}

PacketFactoryManager::~PacketFactoryManager( ) 
{
__ENTER_FUNCTION
		
	Assert( m_Factories!=NULL ) ;

	for( int32_t i=0; i<m_Size; i++ ) 
	{
		SAFE_DELETE(m_Factories[i]) ;
	}
	
	SAFE_DELETE_ARRAY(m_Factories) ;
	SAFE_DELETE_ARRAY(m_pPacketAllocCount) ;
			
__LEAVE_FUNCTION
}

bool PacketFactoryManager::Init( )
{
__ENTER_FUNCTION
//  此处添加每种消息的Factory信息
//例如：
//	AddFactory( new CGAttackFactory() ) ;
//  .....
//CL && LC
#if defined (_FOX_CLIENT) && defined (_FOX_LOGIN)
//CL
	AddFactory( new CLAskCharListFactory()) ;

#endif



	return true ;

__LEAVE_FUNCTION

	return false ;
}

void PacketFactoryManager::AddFactory( PacketFactory* pFactory ) 
{
__ENTER_FUNCTION
		
	if( m_Factories[pFactory->GetPacketID()]!=NULL ) 
	{//重复设定
		Assert( false ) ;
		return ;
	}
	
	m_Factories[pFactory->GetPacketID()] = pFactory ;
			
__LEAVE_FUNCTION
}

Packet* PacketFactoryManager::CreatePacket( PacketID_t packetID ) 
{
__ENTER_FUNCTION

	if( packetID>=m_Size || m_Factories[packetID]==NULL ) 
	{
		Assert(false) ;
		return NULL ;
	}

	Packet* pPacket = NULL ;
	Lock() ;
	_MY_TRY
	{
		pPacket = m_Factories[packetID]->CreatePacket();
		m_pPacketAllocCount[packetID]++ ;
	}
	_MY_CATCH
	{
		pPacket = NULL ;
	}
	Unlock() ;

	return pPacket ;
			
__LEAVE_FUNCTION

	return NULL ;
}

uint32_t PacketFactoryManager::GetPacketMaxSize( PacketID_t packetID ) 
{
__ENTER_FUNCTION


	if( packetID>=m_Size || m_Factories[packetID]==NULL ) 
	{
		char buff[256] = {0};
		sprintf(buff,"PacketID= %d 消息没有注册到PacketFactoryManager上",packetID);
		AssertEx(false,buff) ;
		return 0 ;
	}

	Lock() ;
	uint32_t iRet = m_Factories[packetID]->GetPacketMaxSize( ) ;
	Unlock() ;

	return iRet ;
			
__LEAVE_FUNCTION

	return 0 ;
}

void PacketFactoryManager::RemovePacket( Packet* pPacket )
{
__ENTER_FUNCTION

	if( pPacket==NULL )
	{
		Assert(false) ;
		return ;
	}

	PacketID_t packetID = pPacket->GetPacketID() ;
	if( packetID>=m_Size ) 
	{
		Assert(false) ;
		return ;
	}

	Lock() ;
	_MY_TRY
	{
		SAFE_DELETE( pPacket ) ;
		m_pPacketAllocCount[packetID] -- ;
	}
	_MY_CATCH
	{
	}
	Unlock() ;
	return ;

__LEAVE_FUNCTION

	return ;
}



