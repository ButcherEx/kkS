//#include "stdafx.h"


#include "SocketOutputStream.h"
#include "Packet.h"


SocketOutputStream::SocketOutputStream( Socket& sock, uint32_t BufferLen, uint32_t MaxBufferLen ) 
:m_rSocket(sock)
{
__ENTER_FUNCTION_FOXNET
	
	m_BufferLen = BufferLen ;
	m_MaxBufferLen = MaxBufferLen ;
	m_Head = 0 ;
	m_Tail = 0 ;
	
	m_Buffer = new CHAR[ m_BufferLen ];
	memset( m_Buffer, 0, m_BufferLen ) ;

__LEAVE_FUNCTION_FOXNET
}

SocketOutputStream::~SocketOutputStream( ) 
{
__ENTER_FUNCTION_FOXNET
		
	SAFE_DELETE_ARRAY(m_Buffer);
		
__LEAVE_FUNCTION_FOXNET
}

uint32_t SocketOutputStream::Length( )const
{
__ENTER_FUNCTION_FOXNET

	if( m_Head<m_Tail )
		return m_Tail-m_Head;
	
	else if( m_Head>m_Tail ) 
		return m_BufferLen-m_Head+m_Tail ;
	
	return 0 ;

__LEAVE_FUNCTION_FOXNET

	return 0 ;
}

uint32_t SocketOutputStream::Write( const CHAR* buf, uint32_t len ) 
{
__ENTER_FUNCTION_FOXNET

		//					//
        //     T  H			//    H   T			LEN=10
        // 0123456789		// 0123456789
        // abcd...efg		// ...abcd...
        //					//
			
	uint32_t nFree = ( (m_Head<=m_Tail)?(m_BufferLen-m_Tail+m_Head-1):(m_Head-m_Tail-1) ) ;

	if( len>=nFree )
	{
		if( !Resize( len-nFree+1 ) )
			return 0 ;
	}
		
	if( m_Head<=m_Tail ) 
	{	
		if( m_Head==0 ) 
		{
			nFree = m_BufferLen - m_Tail - 1;
			memcpy( &m_Buffer[m_Tail], buf, len ) ;
		} 
		else 
		{
			nFree = m_BufferLen-m_Tail ;
			if( len<=nFree )
			{
				memcpy( &m_Buffer[m_Tail], buf, len ) ;
			}
			else 
			{
				memcpy( &m_Buffer[m_Tail], buf, nFree ) ;
				memcpy( m_Buffer, &buf[nFree], len-nFree ) ;
			}
		}
	} 
	else 
	{	
		memcpy( &m_Buffer[m_Tail], buf, len ) ;
	}
	
	m_Tail = (m_Tail+len)%m_BufferLen ;
		
	return len;
	
__LEAVE_FUNCTION_FOXNET

	return 0 ;
}


//BOOL SocketOutputStream::WritePacket( const Packet* pPacket )
//{
//__ENTER_FUNCTION_FOXNET
//		
//	PacketID_t packetID = pPacket->GetPacketID() ;
//	uint32_t w = Write( (CHAR*)&packetID , sizeof(PacketID_t) ) ;
////	Assert( w!=0 ) ;
//
//	uint32_t packetUINT ;
//	
//	uint32_t packetSize = pPacket->GetPacketSize( ) ;
//	uint32_t packetIndex = pPacket->GetPacketIndex( ) ;
//
//	SET_PACKET_INDEX(packetUINT, packetIndex) ;
//	SET_PACKET_LEN(packetUINT, packetSize) ;
//
//	w = Write( (CHAR*)&packetUINT, sizeof(uint32_t) ) ;
////	Assert( w!=0 ) ;
//	
//	return pPacket->Write( *this ) ;
//
//__LEAVE_FUNCTION_FOXNET
//
//	return FALSE ;
//}

void SocketOutputStream::Initsize( )
{
	m_Head = 0 ;
	m_Tail = 0 ;

	SAFE_DELETE_ARRAY( m_Buffer ) ;
		
	m_Buffer = new CHAR[DEFAULTSOCKETOUTPUTBUFFERSIZE] ;
		
	m_BufferLen = DEFAULTSOCKETOUTPUTBUFFERSIZE ;
		
	memset( m_Buffer, 0, m_BufferLen ) ;
}

uint32_t SocketOutputStream::Flush( ) 
{
__ENTER_FUNCTION_FOXNET

	uint32_t nFlushed = 0;
	uint32_t nSent = 0;
	uint32_t nLeft;

	if( m_BufferLen>m_MaxBufferLen )
	{
		//如果单个客户端的缓存太大，则重新设置缓存，并将此客户端断开连接
		Initsize( ) ;
		return SOCKET_ERROR-1 ;
	}

#if defined(__WINDOWS__)
	uint32_t flag = MSG_DONTROUTE ;
#elif defined(__LINUX__)
	uint32_t flag = MSG_NOSIGNAL ;
#endif
	
	_MY_TRY 
	{
		if ( m_Head < m_Tail ) 
		{
			nLeft = m_Tail - m_Head;
	
			while ( nLeft > 0 ) 
			{
				nSent = m_rSocket.send( &m_Buffer[m_Head] , nLeft , flag );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-2 ;
				if (nSent==0) return 0;
				
				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
		} 
		else if( m_Head > m_Tail ) 
		{
			nLeft = m_BufferLen - m_Head;
	
			while ( nLeft > 0 ) 
			{
				nSent = m_rSocket.send( &m_Buffer[m_Head] , nLeft , flag );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-3 ;
				if (nSent==0) return 0;
				
				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
			
	
			m_Head = 0;			
			nLeft = m_Tail;
	
			while ( nLeft > 0 ) 
			{
				nSent = m_rSocket.send( &m_Buffer[m_Head] , nLeft , flag );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-4 ;
				if (nSent==0) return 0;
				
				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
		}
	
		if ( m_Head != m_Tail ) 
		{
		}
		
	}
	_MY_CATCH
	{
		if( nSent>0 )
		{
			m_Head += nSent;
		}
	} 

	m_Head = m_Tail = 0 ;

	return nFlushed;

__LEAVE_FUNCTION_FOXNET

	return 0 ;
}

bool SocketOutputStream::Resize( int32_t size )
{
__ENTER_FUNCTION_FOXNET
		

	int32_t orgSize = size;

	size = max( size, (int)(m_BufferLen>>1) ) ;
	uint32_t newBufferLen = m_BufferLen+size ;
	uint32_t len = Length( ) ;
	
	if( size<0 ) 
	{
		if( newBufferLen<len )
			return false ;
	} 
	
	CHAR * newBuffer = new CHAR[ newBufferLen ] ;
	if( newBuffer==NULL )
		return false ;
		
	if( m_Head<m_Tail ) 
	{
		memcpy( newBuffer, &m_Buffer[m_Head], m_Tail-m_Head ) ;
	} 
	else if( m_Head>m_Tail ) 
	{
		memcpy( newBuffer, &m_Buffer[m_Head] , m_BufferLen-m_Head );
		memcpy( &newBuffer[m_BufferLen-m_Head], m_Buffer, m_Tail );
	}
		
	delete [] m_Buffer;
		
	m_Buffer = newBuffer;
	m_BufferLen = newBufferLen;
	m_Head = 0;
	m_Tail = len;	

	return true ;

__LEAVE_FUNCTION_FOXNET

	return false ;
}

void SocketOutputStream::CleanUp( )
{
__ENTER_FUNCTION_FOXNET

	m_Head = 0 ;
	m_Tail = 0 ;

__LEAVE_FUNCTION_FOXNET
}






