#include "BaseLib.h"

// boost::threadpool::fifo_pool thread_pool(10);
// 
// 
// void HellWorld()
// {
// 	printf("%s","boost::threadpool::fifo_pool.\n");
// }
// 
// struct ThreadPoolAutoT
// {
// 	ThreadPoolAutoT()
// 	{
// 		for(int32 i = 0; i < 100; i++)
// 		{
// 			thread_pool.schedule(&HellWorld);
// 		}
// 	}
// }__threadpool_t;

class StaticObject
{
public:
	StaticObject()
	{

	}

	~StaticObject()
	{

	}

	void Say()
	{
		printf("%s", "PoolObject Test.\n");
	}
};

POOL_DECL(StaticObject);
POOL_IMPL(StaticObject);


struct PoolTest
{
public:
	PoolTest()
	{
		const CHAR* message = "CG_ASK_LOGIN";
		uint32_t Id1 = MyCRC(message); // 0,2589650335 1,1179716420 2,3115250875
		uint32_t Id2 = MyCRC32(message); // 882901144
	//	uint32_t Id3 = MyCRC32_1(message); // 3822032882
		bool isEqual = (Id1 == Id2);
		Signle();
		Signle();
		Signle();
	}

	void Signle()
	{
		StaticObjectPtr object = POOL_NEW(StaticObject);
		object->Say();
	}
}__autoPool;


class Message
{
public:
	virtual void Serial()
	{
		printf("%s\n", __FUNCTION__);
	}

	virtual uint32_t ByteSize() const { return 1; }
};

class CG_LOGIN_MESSAGE: public Message
{
public:
	virtual void Serial()
	{
		printf("%s\n", __FUNCTION__);
	}
};

class CmdMessage
{
public:
	CmdMessage(Message& rMsg, const CHAR* Name) : m_refMessage(rMsg), m_PacketId(MyCRC(Name)){ }
	virtual ~CmdMessage(){}
	virtual Message& GetMsg( ) { return m_refMessage ; }
	virtual	uint32_t	GetPacketID( ) const { return m_PacketId; } 
	virtual	uint32_t	GetPacketSize( ) const { return m_refMessage.ByteSize(); } 
	virtual CmdMessage* Clone() = 0;
private:
	uint32_t m_PacketId;
	Message& m_refMessage;
};

#define CMD_TYPE(MSGTYPE) MSGTYPE##Cmd
#define CMD_DECL(MSGTYPE)\
class CMD_TYPE(MSGTYPE) : public CmdMessage { \
private: \
	MSGTYPE m_Msg; \
public: \
	CmdMessage* Clone() { return new CG_LOGIN_MESSAGECmd(); } \
public: \
	CMD_TYPE(MSGTYPE)( ) : CmdMessage(m_Msg, #MSGTYPE){ } \
};

CMD_DECL(CG_LOGIN_MESSAGE);

struct  MesssageFactory
{
public:
	MesssageFactory()
	{
		CmdMessage* pCmd = new CMD_TYPE(CG_LOGIN_MESSAGE);
		pCmd->GetMsg().Serial( );
		delete pCmd;

	}
} __autoMessageFac;

