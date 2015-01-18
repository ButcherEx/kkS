

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
public:
	#define RMSG m_rMsg
public :
	Packet(PBMessage& msg, const CHAR* name);
	virtual ~Packet( );
	virtual	uint32_t	GetPacketID( ) const { return m_PacketId; } 
	virtual	uint32_t	GetPacketSize( ) const { return m_rMsg.ByteSize(); } 
	virtual Packet*		Clone() = 0;
	virtual uint32_t	Execute( Player* pPlayer ) = 0 ;
	virtual void		FreeOwn() { delete this; }
public:
	PBMessage& m_rMsg;
private:
	PacketID_t	m_PacketId;
};

template<class MsgType>
class PacketWrapper : public Packet
{
public: 
	PacketWrapper<MsgType>(const CHAR* name) : Packet(m_Msg, name){}

public:
	MsgType& GetMsg() { return m_Msg; }
	const MsgType& GetMsg( ) const { return m_Msg; }
	virtual uint32_t Execute( Player* pPlayer ) { return pPlayer->Handle(Msg); } 

#define MSG GetMsg()
private:
	MsgType m_Msg;
};

#define PACKET_DECL(MSGTYPE)\
class MSGTYPE##_PAK : public PacketWrapper<MSGTYPE>\
{\
public:\
	explicit MSGTYPE##_PAK(): PacketWrapper<MSGTYPE>(#MSGTYPE){}\
	Packet* Clone() { return new MSGTYPE##_PAK(); }\
};


#endif
