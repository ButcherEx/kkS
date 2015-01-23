//#include "stdafx.h"


//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "ServerSocket.h"


ServerSocket::ServerSocket ( uint32_t port , uint32_t backlog ) 
{
	

		bool ret = false ;

#define KEY_MAC 12
#ifdef __WINDOWS__

//#ifndef GAME_CLIENT
//	char lpszMac[128] = {0};
//	char lpszKey[128] = {0};
//	Ini ini( FILE_SERVER_INFO ) ;
//	ini.ReadText( "KeyDog", "Key", lpszKey, sizeof(lpszKey) ) ;
//	GetMacByCmd(lpszMac);
//	int uret = strncmp(lpszKey, lpszMac,KEY_MAC);
//	if (0 != uret)
//	{
//		while (1)
//		{
//			MessageBox(NULL,"���кŴ���Ϊ�˲����˶��ѣ���ʾ��һ�£�","���ش���!",MB_ICONERROR);//��ʽʱע�͵�
//			//double *d = new double[10000];//��ʽʱ��
//		}
//		//throw 1 ;//��ʽʱ��
//	}
//	else
//	{
//		;
//	}
//#else
//	//if(!CheckSystem())
//	//	throw 1;
//#endif

#endif
	
	// create socket
	ret = m_Socket.create( ) ;
	if( ret==false )
		throw 1 ;
//	Assert( ret ) ;
	
	// reuse address before Bind()
	// ���ε� �ϱ� ���� �ּ� ����� �ý��ۿ� �˷��� �Ѵ�.
	ret = m_Socket.setReuseAddr( ) ;
	if( ret==false )
		throw 1 ;
//	Assert( ret ) ;
	
	// bind address to socket
	// �̹� port�� m_Impl�� ����Ǿ� �����Ƿ�, �Ķ���;��� Bind()�� ȣ���ص� �ȴ�.
	ret = m_Socket.bind( port ) ;
	if( ret==false )
		throw 1 ;
//	Assert( ret ) ;

//	m_Impl->setSendBufferSize( 9000 );
//	m_Impl->setReceiveBufferSize( 9000 );
	
	// set listening queue size
	ret = m_Socket.listen( backlog ) ;
	if( ret==false )
		throw 1 ;
//	Assert( ret ) ;

	

}

ServerSocket::~ServerSocket () 
{
	

		m_Socket.close();

	
}
	
void ServerSocket::close ()
{
	
		

	m_Socket.close( ) ;

	
}

bool ServerSocket::accept ( Socket& rSocket ) 
{
	
	
	uint32_t addrlen = sizeof(SOCKADDR_IN) ;
	rSocket.close( ) ;

	rSocket.m_SocketID = m_Socket.accept( (struct sockaddr *)(&(rSocket.m_SockAddr)), &addrlen ) ;
	if( rSocket.m_SocketID == INVALID_SOCKET )
		return false ;

	rSocket.m_Port = ntohs( rSocket.m_SockAddr.sin_port ) ;
	strncpy( rSocket.m_Host, inet_ntoa(rSocket.m_SockAddr.sin_addr), IP_SIZE-1 ) ;

	return true;

	

	return false ;
}










