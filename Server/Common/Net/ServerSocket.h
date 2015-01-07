
#ifndef __SERVER_SOCKET__
#define __SERVER_SOCKET__

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////

#include "Socket.h"

//#ifdef __WINDOWS__
//	#pragma comment(lib, "../../Common/KeyDog.lib")
//	extern BOOL GetMacByCmd(char *lpszMac);
////#else
////	#pragma comment(lib, "../../Common/libCFG.a")
////	extern int		 CheckSystem();
//#endif
//////////////////////////////////////////////////////////////////////
//
// class ServerSocket
//
//////////////////////////////////////////////////////////////////////

class ServerSocket 
{
public :
	
	// constructor
	ServerSocket (uint32_t port, uint32_t backlog = 5) ;
	
	// destructor
	~ServerSocket () ;
	

//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
	
	// close socket
	void close () ;
	
	// accept new connection
	bool accept ( Socket& rSocket ) ;

	// get/set socket's linger status
    uint32_t getLinger () { return m_Socket.getLinger(); }
    void setLinger (uint32_t lingertime) { m_Socket.setLinger(lingertime); }
 
     // get/set socket's nonblocking status
    bool isNonBlocking () const { return m_Socket.isNonBlocking(); }
    void setNonBlocking (bool on = true) { m_Socket.setNonBlocking(on); }
 
    // get/set receive buffer size
    uint32_t getReceiveBufferSize () const { return m_Socket.getReceiveBufferSize(); }
    void setReceiveBufferSize (uint32_t size) { m_Socket.setReceiveBufferSize(size); }
 
    // get/set send buffer size
    uint32_t getSendBufferSize () const { return m_Socket.getSendBufferSize(); }
    void setSendBufferSize (uint32_t size) { m_Socket.setSendBufferSize(size); }
 
	SOCKET getSOCKET () { return m_Socket.getSOCKET(); }

//////////////////////////////////////////////////
// attributes
//////////////////////////////////////////////////
protected :
	
	// socket implementation object
	Socket m_Socket;




};


#endif
