//#include "stdafx.h"
//////////////////////////////////////////////////////////////////////
// 
// Socket.cpp
// 
// 
//////////////////////////////////////////////////////////////////////


#include "Socket.h"


#if defined(__WINDOWS__)

struct network_initializer
{
	network_initializer()
	{
		WSADATA wsaData= {0};
		uint16_t dwNetworkVersion = MAKEWORD(2,2);
		::WSAStartup(dwNetworkVersion, &wsaData);
	}
	~network_initializer()
	{
		::WSACleanup();
	}

} __auto_network_initializer;
#endif


Socket::Socket () 
{ 

	m_SocketID = INVALID_SOCKET ;
	memset( &m_SockAddr, 0, sizeof(SOCKADDR_IN) ) ;
	memset( m_Host, 0, IP_SIZE ) ;
	m_Port = 0 ;
	
	
}

Socket::Socket ( const CHAR* host , uint32_t port ) 
{ 
	strncpy( m_Host, host, IP_SIZE-1 ) ;
	m_Port = port ;

	create() ;	
}

Socket::~Socket () 
{ 
	

	close() ;
	
	
}

bool Socket::create( )
{
	

	m_SocketID = SocketAPI::socket_ex( AF_INET , SOCK_STREAM , 0 );
    
	memset( &m_SockAddr , 0 , sizeof(m_SockAddr) );
	
	m_SockAddr.sin_family = AF_INET;

	if( isValid() )
		return true ;
	else 
		return false ;

	

	return false ;
}

// close previous connection and connect to another server socket
bool Socket::reconnect ( const CHAR* host , uint32_t port )
{
	

	// delete old socket impl object
	close();

	// create new socket impl object
	strncpy( m_Host, host, IP_SIZE-1 ) ;
	m_Port = port ;

	create() ;

	// try to connect
	return connect();	

	

	return false ;
}

void Socket::close () 
{ 
	

//	if( !isSockError() ) 
	if( isValid() && !isSockError() ) 
	{
		_MY_TRY 
		{
			SocketAPI::closesocket_ex( m_SocketID );
		} 
		_MY_CATCH
		{
		}
	}

	m_SocketID = INVALID_SOCKET ;
	memset( &m_SockAddr, 0, sizeof(SOCKADDR_IN) ) ;
	memset( m_Host, 0, IP_SIZE ) ;
	m_Port = 0 ;

	
}
	
bool Socket::connect () 
{ 
	

	m_SockAddr.sin_addr.s_addr = inet_addr( m_Host );
	
	// set sockaddr's port
	m_SockAddr.sin_port = htons(m_Port);
	
	// try to connect to peer host
	bool result = SocketAPI::connect_ex( m_SocketID , (const struct sockaddr *)&m_SockAddr , sizeof(m_SockAddr) );
	if( result)
		return true ;
	else
		return false ;

	

	return 0 ;
}

bool Socket::connect (const CHAR* host, uint32_t port) 
{ 
	

	strncpy( m_Host, host, IP_SIZE-1 ) ;
	m_Port = port ;

	return connect() ;

	

	return 0 ;
}

uint32_t Socket::send (const void* buf, uint32_t len, uint32_t flags) 
{ 
	

	return SocketAPI::send_ex( m_SocketID , buf , len , flags );

	

	return 0 ;
}

uint32_t Socket::receive (void* buf, uint32_t len, uint32_t flags) 
{ 
	

	return SocketAPI::recv_ex( m_SocketID , buf , len , flags );

	

	return 0 ;
}

uint32_t Socket::available ()const
{ 
	

	return SocketAPI::availablesocket_ex( m_SocketID );

	

	return 0 ;
}

uint32_t Socket::getLinger ()const 
{ 
	

	struct linger ling;
	uint32_t len = sizeof(ling);
	
	SocketAPI::getsockopt_ex( m_SocketID , SOL_SOCKET , SO_LINGER , &ling , &len );
	
	return ling.l_linger;

	

	return 0 ;
}

SOCKET Socket::accept( struct sockaddr* addr, uint32_t* addrlen )
{
	

	return SocketAPI::accept_ex( m_SocketID, addr, addrlen ) ;

	

	return INVALID_SOCKET ;
}

bool Socket::setLinger (uint32_t lingertime) 
{
	

	struct linger ling;
	
	ling.l_onoff = lingertime > 0 ? 1 : 0;
	ling.l_linger = lingertime;
	
	return SocketAPI::setsockopt_ex( m_SocketID , SOL_SOCKET , SO_LINGER , &ling , sizeof(ling) );

	

	return false ;
}

uint32_t Socket::getSockError()const 
{ 
	

	return isSockError(); 

	

	return 0 ;
}

bool Socket::isNonBlocking ()const 
{ 
	

	return SocketAPI::getsocketnonblocking_ex( m_SocketID );

	

	return false ;
}

bool Socket::setNonBlocking (bool on ) 
{ 
	

	return SocketAPI::setsocketnonblocking_ex( m_SocketID , on );

	

	return false ;
}

uint32_t Socket::getReceiveBufferSize ()const 
{ 
	

	uint32_t ReceiveBufferSize;
	uint32_t size = sizeof(ReceiveBufferSize);

	SocketAPI::getsockopt_ex( m_SocketID , SOL_SOCKET , SO_RCVBUF , &ReceiveBufferSize, &size );
	
	return ReceiveBufferSize;

	

	return 0 ;
}

bool Socket::setReceiveBufferSize (uint32_t size)
{ 
	

	return (bool)(SocketAPI::setsockopt_ex( m_SocketID , SOL_SOCKET , SO_RCVBUF , &size, sizeof(uint32_t) ) ) ;

	

	return false ;
}

uint32_t Socket::getSendBufferSize ()const 
{ 
	

	uint32_t SendBufferSize;
	uint32_t size = sizeof(SendBufferSize);

	SocketAPI::getsockopt_ex( m_SocketID , SOL_SOCKET , SO_SNDBUF , &SendBufferSize, &size );
	
	return SendBufferSize;

	

	return 0 ;
}

bool Socket::setSendBufferSize (uint32_t size)
{ 
	

	return (bool)(SocketAPI::setsockopt_ex( m_SocketID , SOL_SOCKET , SO_SNDBUF , &size, sizeof(uint32_t) ) );

	

	return false ;
}

uint32_t Socket::getPort ()const 
{ 
	

	return m_Port; 

	
}

IP_t Socket::getHostIP ()const 
{ 
	

	return (IP_t)(m_SockAddr.sin_addr.s_addr);; 

	
}

bool Socket::isValid ()const 
{
	

	return m_SocketID!=INVALID_SOCKET; 

	
}

SOCKET Socket::getSOCKET ()const 
{ 
	

	return m_SocketID; 

	
}

bool Socket::isSockError()const
{
	

	int32_t error;
	uint32_t len = sizeof(error);
	
	int32_t Result = SocketAPI::getsockopt_ex2( m_SocketID , SOL_SOCKET , SO_ERROR , &error, &len );

	if( Result == 0 ) 
		return false;
	else 			  
		return true;

	

	return false ;
}

bool Socket::bind( )
{
	

	m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_SockAddr.sin_port        = htons(m_Port);

	bool result = SocketAPI::bind_ex( m_SocketID , (const struct sockaddr *)&m_SockAddr , sizeof(m_SockAddr) ) ;
	if( result )
		return true ;
	else
		return false ;

	

	return 0 ;
}

bool Socket::bind( uint32_t port )
{
	

	m_Port = port ;

	m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_SockAddr.sin_port        = htons(m_Port);

	bool result = SocketAPI::bind_ex( m_SocketID , (const struct sockaddr *)&m_SockAddr , sizeof(m_SockAddr) );
	if( result )
		return true ;
	else
		return false ;

	

	return 0 ;
}

bool Socket::listen( int32_t backlog )
{
	
		
	return SocketAPI::listen_ex( m_SocketID , backlog );

	

	return false ;
}

bool Socket::isReuseAddr ()const
{
	 
		
	int32_t reuse;
	uint32_t len = sizeof(reuse);
	
	SocketAPI::getsockopt_ex( m_SocketID , SOL_SOCKET , SO_REUSEADDR , &reuse , &len );
	
	return reuse == 1;
	
	

	return 0 ;
}

bool Socket::setReuseAddr ( bool on )
{
	 

	int32_t opt = on == true ? 1 : 0;
	
	return SocketAPI::setsockopt_ex( m_SocketID , SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt) );
	
	

	return true ;
}

