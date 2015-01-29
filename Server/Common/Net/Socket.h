/************************************************************************/
/*                                                                      */
/************************************************************************/
#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "SocketAPI.h"

class Socket 
{
//////////////////////////////////////////////////
// constructor/destructor
//////////////////////////////////////////////////
public :
	
	// constructor
	Socket () ;
	Socket (const CHAR* host, uint32_t port) ;
	
	// destructor
	virtual ~Socket () ;

	
//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
	bool create() ;
	
	// close connection
	void close () ;
	
	// try connect to remote host
	bool connect () ;
	bool connect (const CHAR* host, uint32_t port) ;

	// close previous connection and connect to another socket
	bool reconnect (const CHAR* host, uint32_t port) ;
	
	// send data to peer
	uint32_t send (const void* buf, uint32_t len, uint32_t flags = 0) ;
	
	// receive data from peer
	uint32_t receive (void* buf, uint32_t len, uint32_t flags = 0) ;
	
	uint32_t available ()const ;

	SOCKET accept( struct sockaddr* addr, uint32_t* addrlen ) ;

	bool bind( ) ;
	bool bind( uint32_t port ) ;

	bool listen( int32_t backlog ) ;

//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
 
    // get/set socket's linger status
    uint32_t getLinger ()const ;
    bool setLinger (uint32_t lingertime) ;

	bool isReuseAddr ()const ;
	bool setReuseAddr (bool on = true) ;

	// get is Error
    uint32_t getSockError()const ;
 
    // get/set socket's nonblocking status
    bool isNonBlocking ()const ;
    bool setNonBlocking (bool on = true) ;
 
    // get/set receive buffer size
    uint32_t getReceiveBufferSize ()const ;
    bool setReceiveBufferSize (uint32_t size) ;
 
    // get/set send buffer size
    uint32_t getSendBufferSize ()const ;
    bool setSendBufferSize (uint32_t size) ;
 
	uint32_t getPort ()const ;
	IP_t getHostIP ()const ;

	// check if socket is valid
	bool isValid ()const ;

	// get socket descriptor
	SOCKET getSOCKET ()const ;

	bool isSockError()const ;

public :

	SOCKET m_SocketID;
	
	// socket address structure
	SOCKADDR_IN m_SockAddr;
	
	// peer host
	CHAR m_Host[IP_SIZE];
	
	// peer port
	uint32_t m_Port;


};

#endif
