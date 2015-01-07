//
//文件名称：	SocketAPI.h
//功能描述：	对网络socket进行一个简单的封装操作，针对不同的操作系统
//				实现相同接口的调用
//修改情况：	2005-03-22 Ver 1.0.0 完成基本功能
//				
//
//
#ifndef __SOCKET_API_H__
#define __SOCKET_API_H__

// include files
#include "Base.h"


#if defined(__WINDOWS__)
#include <WinSock.h>
#elif defined(__LINUX__)
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#endif

#define _ESIZE 256

//////////////////////////////////////////////////
//
// 
// 
// 
//
//////////////////////////////////////////////////
#if defined(__LINUX__)

typedef		int32_t		SOCKET;
#define     INVALID_SOCKET   -1
#define		SOCKET_ERROR	 -1

#endif

static const int32_t SOCKET_ERROR_WOULDBLOCK = -100;

typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
static const uint32_t szSOCKADDR_IN = sizeof(SOCKADDR_IN);

//////////////////////////////////////////////////////////////////////
//
// Platform Independent Socket API Collection (exception based)
//
//////////////////////////////////////////////////////////////////////

namespace SocketAPI 
{

	//
	// exception version of socket ()
	//
	SOCKET socket_ex (int32_t domain, int32_t type, int32_t protocol) ;


	//
	// exception version of bind ()
	//
	bool bind_ex (SOCKET s, const struct sockaddr* name, uint32_t namelen) ;


	//
	// exception version of connect ()
	//
	bool connect_ex (SOCKET s, const struct sockaddr* name, uint32_t namelen) ;

	//
	// exception version of listen ()
	//
	bool listen_ex (SOCKET s, uint32_t backlog) ;


	//
	// exception version of accept ()
	//
	SOCKET accept_ex (SOCKET s, struct sockaddr* addr, uint32_t* addrlen) ;


	//
	// exception version of getsockopt ()
	//
	bool getsockopt_ex (SOCKET s, int32_t level, int32_t optname, void* optval, uint32_t* optlen) ;

	uint32_t getsockopt_ex2 (SOCKET s, int32_t level, int32_t optname, void* optval, uint32_t* optlen) ;

	//
	// exception version of setsockopt ()
	//
	bool setsockopt_ex (SOCKET s, int32_t level, int32_t optname, const void* optval, uint32_t optlen) ;

	//
	// exception version of send()
	//
	uint32_t send_ex (SOCKET s, const void* buf, uint32_t len, uint32_t flags) ;


	//
	// exception version of sendto()
	//
	uint32_t sendto_ex (SOCKET s, const void* buf, int32_t len, uint32_t flags, const struct sockaddr* to, int32_t tolen) ;

	//
	// exception version of recv()
	//
	uint32_t recv_ex (SOCKET s, void* buf, uint32_t len, uint32_t flags) ;


	//
	// exception version of recvfrom()
	//
	uint32_t recvfrom_ex (SOCKET s, void* buf, int32_t len, uint32_t flags, struct sockaddr* from, uint32_t* fromlen) ;
	 

	//
	// exception version of closesocket() 
	//
	// *CAUTION*
	//
	// in UNIX, close() used instead
	//
	bool closesocket_ex (SOCKET s) ;


	//
	// exception version of ioctlsocket()
	//
	// *CAUTION*
	//
	// in UNIX, ioctl() used instead
	//
	bool ioctlsocket_ex (SOCKET s, LONG cmd, ULONG* argp) ;


	//
	// check if socket is nonblocking mode
	//
	bool getsocketnonblocking_ex (SOCKET s) ;


	//
	// make socket nonblocking mode
	//
	bool setsocketnonblocking_ex (SOCKET s, bool on) ;


	//
	// get amount of data in socket input buffer
	//
	uint32_t availablesocket_ex (SOCKET s) ;


	//
	// exception version of shutdown()
	//
	bool shutdown_ex (SOCKET s, uint32_t how) ;


	//
	// exception version of select()
	//
	int32_t select_ex (int32_t maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout) ;


};//end of namespace 



#endif
