

#ifndef __PACKET_H__
#define __PACKET_H__

#include "Base.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

class Socket;
class Player;

#define GET_PACKET_INDEX(a) ((a)>>24)
#define SET_PACKET_INDEX(a,index) ((a)=(((a)&0xffffff)+((index)<<24)))
#define GET_PACKET_LEN(a) ((a)&0xffffff)
#define SET_PACKET_LEN(a,len) ((a)=((a)&0xff000000)+(len))
//消息头中包括：PacketID_t-2字节；uint32_t-4字节中高位一个字节为消息序列号，其余
//三个字节为消息长度
//通过GET_PACKET_INDEX和GET_PACKET_LEN宏，可以取得uint32_t数据里面的消息序列号和长度
//通过SET_PACKET_INDEX和SET_PACKET_LEN宏，可以设置uint32_t数据里面的消息序列号和长度
#define PACKET_HEADER_SIZE (sizeof(PacketID_t)+sizeof(uint16_t)+sizeof(uint32_t))


//Packet::Execute(...) 的返回值
enum PACKET_EXE
{
	PACKET_EXE_ERROR = 0 ,
	PACKET_EXE_BREAK ,
	PACKET_EXE_CONTINUE ,
	PACKET_EXE_NOTREMOVE ,
	PACKET_EXE_NOTREMOVE_ERROR ,
};

class Packet
{
public :
	BYTE			m_Index ;
	BYTE			m_Status ;

public :
	Packet( ) ;
	virtual ~Packet( ) ;
	virtual void	CleanUp( ){} ;
	virtual bool	Read( SocketInputStream& iStream ) = 0 ;
	virtual bool	Write( SocketOutputStream& oStream ) const = 0;
	virtual	PacketID_t	GetPacketID( ) const = 0 ;
	virtual	uint32_t	GetPacketSize( ) const = 0 ;
	BYTE				GetPacketIndex( ) const { return m_Index ; } ;
	void				SetPacketIndex( BYTE Index ){ m_Index = Index ; } ;
};



#endif
