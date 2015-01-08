

#ifndef __PACKET_H__
#define __PACKET_H__

#include "Base.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "google/protobuf/message.h"

class Socket;
class Player;

#define GET_PACKET_INDEX(a) ((a)>>24)
#define SET_PACKET_INDEX(a,index) ((a)=(((a)&0xffffff)+((index)<<24)))
#define GET_PACKET_LEN(a) ((a)&0xffffff)
#define SET_PACKET_LEN(a,len) ((a)=((a)&0xff000000)+(len))
//��Ϣͷ�а�����PacketID_t-2�ֽڣ�uint32_t-4�ֽ��и�λһ���ֽ�Ϊ��Ϣ���кţ�����
//�����ֽ�Ϊ��Ϣ����
//ͨ��GET_PACKET_INDEX��GET_PACKET_LEN�꣬����ȡ��uint32_t�����������Ϣ���кźͳ���
//ͨ��SET_PACKET_INDEX��SET_PACKET_LEN�꣬��������uint32_t�����������Ϣ���кźͳ���
#define PACKET_HEADER_SIZE (sizeof(PacketID_t)+sizeof(uint16_t)+sizeof(uint32_t))


//Packet::Execute(...) �ķ���ֵ
enum PACKET_EXE
{
	PACKET_EXE_ERROR = 0 ,
	PACKET_EXE_BREAK ,
	PACKET_EXE_CONTINUE ,
	PACKET_EXE_NOTREMOVE ,
	PACKET_EXE_NOTREMOVE_ERROR ,
};

typedef ::google::protobuf::Message PBMessage;

class Packet
{
public :
	Packet(PBMessage& rMsg, const CHAR* name);
	virtual ~Packet( );
	virtual	uint32_t	GetPacketID( ) const { return m_PacketId; } 
	virtual	uint32_t	GetPacketSize( ) const { return m_rMessage.ByteSize(); } 
	virtual Packet*		Clone() = 0;
	virtual void		FreeOwn() { delete this; }
private:
	PacketID_t	m_PacketId;
	PBMessage& m_rMessage;
};
#define PACKET_CLASS(MSGTYPE) MSGTYPE##_PAK
#define PACKET_DECL(MSGTYPE)\
class PACKET_CLASS(MSGTYPE) : public Packet { \
public: \
	MSGTYPE Msg; \
public: \
	Packet* Clone() { return new PACKET_CLASS(MSGTYPE)(); } \
public: \
	PACKET_CLASS(MSGTYPE)( ) : Packet(Msg, #MSGTYPE){ } \
};


#endif
