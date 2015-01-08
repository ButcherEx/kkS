//
//文件名称：	SocketInputStream.h
//功能描述：	消息数据的接收缓存，提供数据的接收和格式化读取功能
//				
//
//


#ifndef __SOCKETINPUTSTREAM_H__
#define __SOCKETINPUTSTREAM_H__

#include "Socket.h"

//初始化的接收缓存长度
#define DEFAULTSOCKETINPUTBUFFERSIZE 64*1024
//最大可以允许的缓存长度，如果超过此数值，则断开连接
#define DISCONNECTSOCKETINPUTSIZE 96*1024

class SocketInputStream
{
public :
	SocketInputStream( Socket& sock, 
					   uint32_t BufferSize = DEFAULTSOCKETINPUTBUFFERSIZE,
					   uint32_t MaxBufferSize = DISCONNECTSOCKETINPUTSIZE ) ;
	virtual ~SocketInputStream( ) ;

public :
	uint32_t	Fill( ) ;

	uint32_t	Read( CHAR* buf, uint32_t len ) ;

	bool		Peek( CHAR* buf, uint32_t len ) ;
	bool		Find( CHAR* buf ) ;
	bool		Skip( uint32_t len ) ;
	
	void		Initsize( ) ;
	void		CleanUp( ) ;
	bool		Resize( int32_t size ) ;
	bool		IsEmpty( )const { return m_Head==m_Tail; }

	CHAR*		GetBuff(){return m_Buffer;}

	uint32_t	Capacity( )const { return m_BufferLen; }
	uint32_t	Length( )const ;
	uint32_t	Size( )const { return Length(); }
	uint32_t	GetHead(){return m_Head;}
	uint32_t	GetTail(){return m_Tail;}
	uint32_t	GetBuffLen(){return m_BufferLen;}
private :
	CHAR*		m_Buffer ;
	Socket		m_rSocket ;

	uint32_t	m_Head ;
	uint32_t	m_Tail ;
	uint32_t	m_BufferLen ;
	uint32_t	m_MaxBufferLen ;
};




#endif
