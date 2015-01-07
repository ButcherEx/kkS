#ifndef CMD_DEFINE_H
#define CMD_DEFINE_H

//...
struct tagCmd
{
	int32_t size; 
	uint32_t id;
};

typedef boost::shared_ptr<tagCmd> CmdPtr;


#define CMD_START(CMD)\
struct CMD : tagCmd {\
	CMD( ){\
	memset(this, 0, sizeof(*this));\
	size = sizeof(*this);\
	id=base::crc32(#CMD);}

#define CMD_END };


enum MSG_FLAG
{
	MSG_FLAG_POD		= 0,
	MSG_FLAG_GATHER		= 1 << 0,
	MSG_FLAG_COMPRESS	= 1 << 1,
	MSG_FLAG_GCBOTH		= MSG_FLAG_GATHER | MSG_FLAG_COMPRESS,
};

//...
struct NetworkConfig {
	NetworkConfig( ) 
	{
		threadPoolSize = 4;
		maxCmdSize = 512;
		sendBufferSize = 1024;
		recvBufferSize = 1024;
		maxConnectionSize = 4096;
		maxAcceptionExceptionSecond = 3;
#if defined(USE_SELF_POOL)
		maxCmdPoolSize = 64 * 1024;
		maxCmdPoolNumber = 8;
#endif
		ioFlag = MSG_FLAG_POD;
	}
	int32_t threadPoolSize;
	int32_t maxCmdSize;
	int32_t maxCmdPoolSize;
	int32_t sendBufferSize;
	int32_t recvBufferSize;
	int32_t maxConnectionSize;
	int32_t maxAcceptionExceptionSecond;
#if defined(USE_SELF_POOL)
	int32_t maxCmdPoolNumber;
#endif
	int32_t ioFlag;
};

#endif // CMD_DEFINE_H