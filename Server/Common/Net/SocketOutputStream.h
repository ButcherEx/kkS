
//
//文件名称：	SocketOutputStream.h
//功能描述：	消息数据的输出缓存，提供数据的格式化写入和发送功能
//				
//				
//
//

#ifndef __SOCKETOUTPUTSTREAM_H__
#define __SOCKETOUTPUTSTREAM_H__

#include "Socket.h"



//初始化的发送缓存长度
#define DEFAULTSOCKETOUTPUTBUFFERSIZE 8192
//最大可以允许的缓存长度，如果超过此数值，则断开连接
#define DISCONNECTSOCKETOUTPUTSIZE 100*1024

class SocketOutputStream 
{
public :
	SocketOutputStream( Socket& sock, 
						uint32_t BufferSize = DEFAULTSOCKETOUTPUTBUFFERSIZE,
						uint32_t MaxBufferSize = DISCONNECTSOCKETOUTPUTSIZE ) ;
	virtual ~SocketOutputStream( ) ;
public :

	void		Initsize( ) ;
	void		CleanUp( ) ;
	bool		Resize( int32_t size ) ;
	CHAR*		GetBuffer( )const	{ return m_Buffer ; }
	CHAR*		GetBuff()			{return m_Buffer;}
	CHAR*		GetTail()const		{ return &(m_Buffer[m_Tail]) ; }
    bool		IsEmpty()const		{ return m_Head==m_Tail ; }

	uint32_t	Write( const CHAR* buf, uint32_t len ) ;
	uint32_t	Flush() ;
	uint32_t	GetHead(){return m_Head;}
	uint32_t	GetTail(){return m_Tail;}
	uint32_t	GetBuffLen(){return m_BufferLen;}
	uint32_t	Length( )const ;
	uint32_t	Size( )const { return Length( ) ; }
	int32_t		Capacity ()const { return m_BufferLen ; }
protected :
	
	Socket&		m_rSocket ;
	
	CHAR*		m_Buffer ;
	uint32_t	m_BufferLen ;
	uint32_t	m_MaxBufferLen ;
	
	uint32_t	m_Head ;
	uint32_t	m_Tail ;
};



#endif
