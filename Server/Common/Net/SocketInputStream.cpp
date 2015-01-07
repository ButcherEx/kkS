//#include "stdafx.h"


#include "SocketInputStream.h"
#include "Packet.h"


SocketInputStream::SocketInputStream( Socket& sock, uint32_t BufferLen, uint32_t MaxBufferLen ) 
:m_rSocket(sock)
{
__ENTER_FUNCTION_FOXNET

	m_Head = 0 ;
	m_Tail = 0 ;
	m_BufferLen = BufferLen ;
	m_MaxBufferLen = MaxBufferLen ;
		
	m_Buffer = new CHAR[ m_BufferLen ];
	memset( m_Buffer, 0, m_BufferLen ) ;

__LEAVE_FUNCTION_FOXNET
}

SocketInputStream::~SocketInputStream( ) 
{
__ENTER_FUNCTION_FOXNET
		
	SAFE_DELETE_ARRAY(m_Buffer);
		
__LEAVE_FUNCTION_FOXNET
}

uint32_t SocketInputStream::Length( )const
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

//返回0表示没有读到数据
uint32_t SocketInputStream::Read( CHAR* buf, uint32_t len ) 
{
__ENTER_FUNCTION_FOXNET

//	Assert( buf != NULL );	
	
	if ( len == 0 )
		return 0 ;
		
	if ( len > Length() )
		return 0 ;
	
	if ( m_Head < m_Tail ) 
	{
		memcpy( buf, &m_Buffer[m_Head], len ) ;
	} 
	else 
	{
		uint32_t rightLen = m_BufferLen-m_Head ;
		if( len<=rightLen ) 
		{
			memcpy( buf, &m_Buffer[m_Head], len ) ;
		} 
		else 
		{
			memcpy( buf, &m_Buffer[m_Head], rightLen ) ;
			memcpy( &buf[rightLen], m_Buffer, len-rightLen ) ;
		}
	}

	m_Head = (m_Head+len)%m_BufferLen ;
	
	return len ;

__LEAVE_FUNCTION_FOXNET

	return 0 ;
}


bool SocketInputStream::ReadPacket( Packet* pPacket ) 
{
__ENTER_FUNCTION_FOXNET

	bool ret ;
		
	ret = Skip( PACKET_HEADER_SIZE ) ;
	if( !ret ) return false ;

	return pPacket->Read( *this ) ;
	
__LEAVE_FUNCTION_FOXNET

	return false ;
}


bool SocketInputStream::Peek( CHAR* buf, uint32_t len ) 
{
__ENTER_FUNCTION_FOXNET
			

	if( len==0 )
		return false ;
	
	if( len>Length() )
		return false ;

	if( m_Head<m_Tail ) 
	{
		memcpy( buf , &m_Buffer[m_Head] , len );

	} 
	else 
	{
		uint32_t rightLen = m_BufferLen-m_Head ;
		if( len<=rightLen ) 
		{
			memcpy( &buf[0], &m_Buffer[m_Head], len ) ;
		} 
		else 
		{
			memcpy( &buf[0], &m_Buffer[m_Head], rightLen ) ;
			memcpy( &buf[rightLen], &m_Buffer[0], len-rightLen ) ;
		}
	}
		
	return true ;

__LEAVE_FUNCTION_FOXNET

	return false ;
}


bool SocketInputStream::Find( CHAR* buf )
{
	__ENTER_FUNCTION_FOXNET

		bool IsFind = false;
		while ( Length() > PACK_COMPART_SIZE )
		{
			if( m_Head<m_Tail ) 
			{
				memcpy( buf , &m_Buffer[m_Head] , PACK_COMPART_SIZE );
			} 
			else 
			{
				uint32_t rightLen = m_BufferLen-m_Head ;
				if( PACK_COMPART_SIZE<=rightLen ) 
				{
					memcpy( &buf[0], &m_Buffer[m_Head], PACK_COMPART_SIZE ) ;
				} 
				else 
				{
					memcpy( &buf[0], &m_Buffer[m_Head], rightLen ) ;
					memcpy( &buf[rightLen], &m_Buffer[0], PACK_COMPART_SIZE-rightLen ) ;
				}
			}
			if (strstr(buf, PACK_COMPART) == NULL){
				m_Head++;
				if (m_Head >= m_BufferLen){
					m_Head -= m_BufferLen;
				}
				continue;
			}
			else{
				IsFind = true;
				break;
			}
		}

		if (!IsFind){
			return false;
		}

		m_Head += (uint32_t)PACK_COMPART_SIZE;
		if (m_Head >= m_BufferLen){
			m_Head -= m_BufferLen;
		}
		return true ;

		__LEAVE_FUNCTION_FOXNET

			return false ;
}

bool SocketInputStream::Skip( uint32_t len ) 
{
__ENTER_FUNCTION_FOXNET
		
	if( len == 0 )
		return false ;

	if( len>Length( ) )
		return false ;
	
	m_Head = (m_Head+len)%m_BufferLen ;

	return true ;

__LEAVE_FUNCTION_FOXNET

	return false ;
}

void SocketInputStream::Initsize( )
{
	m_Head = 0 ;
	m_Tail = 0 ;

	SAFE_DELETE_ARRAY( m_Buffer ) ;
		
	m_Buffer = new CHAR[DEFAULTSOCKETINPUTBUFFERSIZE] ;
	m_BufferLen = DEFAULTSOCKETINPUTBUFFERSIZE ;
		
	memset( m_Buffer, 0, m_BufferLen ) ;
}
	
uint32_t SocketInputStream::Fill( ) 
{
__ENTER_FUNCTION_FOXNET

	uint32_t nFilled = 0 ;
	uint32_t nReceived = 0 ;
	uint32_t nFree = 0 ;

	if ( m_Head <= m_Tail ) 
	{
		if ( m_Head == 0 ) 
		{
			//
			// H   T		LEN=10
			// 0123456789
			// abcd......
			//

			nReceived = 0 ;
			nFree = m_BufferLen-m_Tail-1 ;
			if( nFree != 0 )
			{
				nReceived = m_rSocket.receive( &m_Buffer[m_Tail] , nFree );
				if (nReceived==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nReceived==SOCKET_ERROR) return SOCKET_ERROR-1 ;
				if (nReceived==0) return SOCKET_ERROR-2 ;

				m_Tail += nReceived;
				nFilled += nReceived;
			}

			if( nReceived == nFree ) 
			{
				uint32_t available = m_rSocket.available();
				if ( available > 0 ) 
				{
					if( (m_BufferLen+available+1)>m_MaxBufferLen )
					{
						Initsize( ) ;
						return SOCKET_ERROR-3 ;
					}
					if( !Resize( available+1 ) )
						return 0 ;

					nReceived = m_rSocket.receive( &m_Buffer[m_Tail] , available );
					if (nReceived==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
					if (nReceived==SOCKET_ERROR) return SOCKET_ERROR-4 ;
					if (nReceived==0) return SOCKET_ERROR-5;

					m_Tail += nReceived;
					nFilled += nReceived;
				}
			}
		} 
		else 
		{
			//
			//    H   T		LEN=10
			// 0123456789
			// ...abcd...
			//

			nFree = m_BufferLen-m_Tail ;
			nReceived = m_rSocket.receive( &m_Buffer[m_Tail], nFree );
			if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
			if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR-6 ;
			if( nReceived==0 ) return SOCKET_ERROR-7 ;

			m_Tail = (m_Tail+nReceived)%m_BufferLen ;
			nFilled += nReceived ;

			if( nReceived==nFree ) 
			{
//				Assert( m_Tail == 0 );

				nReceived = 0 ;
				nFree = m_Head-1 ;
				if( nFree!=0 )
				{
					nReceived = m_rSocket.receive( &m_Buffer[0] , nFree );
					if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
					if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR -8;
					if( nReceived==0 ) return SOCKET_ERROR-9 ;

					m_Tail += nReceived;
					nFilled += nReceived;
				}

				if( nReceived==nFree ) 
				{
					uint32_t available = m_rSocket.available();
					if ( available > 0 ) 
					{
						if( (m_BufferLen+available+1)>m_MaxBufferLen )
						{
							Initsize( ) ;
							return SOCKET_ERROR-10 ;
						}
						if( !Resize( available+1 ) )
							return 0 ;

						nReceived = m_rSocket.receive( &m_Buffer[m_Tail] , available );
						if (nReceived==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
						if (nReceived==SOCKET_ERROR) return SOCKET_ERROR-11 ;
						if (nReceived==0) return SOCKET_ERROR-12;

						m_Tail += nReceived;
						nFilled += nReceived;
					}
				}
			}
		}

	} 
	else 
	{	
		//
        //     T  H		LEN=10
        // 0123456789
        // abcd...efg
        //

		nReceived = 0 ;
		nFree = m_Head-m_Tail-1 ;
		if( nFree!=0 )
		{
			nReceived = m_rSocket.receive( &m_Buffer[m_Tail], nFree ) ;
			if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
			if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR-13 ;
			if( nReceived==0 ) return SOCKET_ERROR-14 ;

			m_Tail += nReceived ;
			nFilled += nReceived ;
		}
		if( nReceived==nFree ) 
		{
			uint32_t available = m_rSocket.available( ) ;
			if ( available>0 ) 
			{
				if( (m_BufferLen+available+1)>m_MaxBufferLen )
				{
					Initsize( ) ;
					return SOCKET_ERROR-15 ;
				}
				if( !Resize( available+1 ) )
					return 0 ;

				nReceived = m_rSocket.receive( &m_Buffer[m_Tail], available ) ;
				if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
				if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR-16 ;
				if( nReceived==0 ) return SOCKET_ERROR-17 ;

				m_Tail += nReceived ;
				nFilled += nReceived ;
			}
		}
	}

	return nFilled ;

__LEAVE_FUNCTION_FOXNET

	return 0 ;
}

bool SocketInputStream::Resize( int32_t size )
{
__ENTER_FUNCTION_FOXNET
			
	size = max(size, (int)(m_BufferLen>>1));
	uint32_t newBufferLen = m_BufferLen + size;
	uint32_t len = Length();
	
	if ( size < 0 ) 
	{
		if ( newBufferLen < len )
			return false ;		
	} 
	
	CHAR * newBuffer = new CHAR[ newBufferLen ];

	if ( m_Head < m_Tail ) 
	{
		memcpy( newBuffer , &m_Buffer[m_Head] , m_Tail - m_Head );
	} 
	else if ( m_Head > m_Tail ) 
	{
		memcpy( newBuffer , &m_Buffer[m_Head] , m_BufferLen - m_Head );
		memcpy( &newBuffer[ m_BufferLen - m_Head ] , m_Buffer , m_Tail );
	}
		
	delete [] m_Buffer ;
		
	m_Buffer = newBuffer ;
	m_BufferLen = newBufferLen ;
	m_Head = 0 ;
	m_Tail = len ;

	return true ;

__LEAVE_FUNCTION_FOXNET

	return false ;
}

void SocketInputStream::CleanUp( )
{
__ENTER_FUNCTION_FOXNET

	m_Head = 0 ;
	m_Tail = 0 ;

__LEAVE_FUNCTION_FOXNET
}




