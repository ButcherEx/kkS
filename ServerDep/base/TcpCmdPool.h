#ifndef TCPCMD_POOL_H
#define TCPCMD_POOL_H

#include <PreCompier.h>
#include <Allocator.h>

BASE_NAME_SPACES

#if defined(USE_SELF_POOL)
typedef boost::function<void (void*)> cmdFreeCallback;

class TcpCmdPool
{
public:
	TcpCmdPool(int32_t poolSize, int32_t maxMsgSize)
		: allocator_(poolSize, minSizeOfMax(maxMsgSize))
	{
		cmdFreeCallback_ = boost::bind(&TcpCmdPool::deallocate, this, _1);
	}
	~TcpCmdPool( ){}

public:
	CmdPtr allocate(int32_t bytes )
	{
		void* p = allocator_.allocZ(bytes);
		if( p )
		{
			CmdPtr cmd( new(p)tagCmd, cmdFreeCallback_ );
			return cmd;
		}
		return CmdPtr();
	}

	CmdPtr tryAllocate(int32_t bytes )
	{
		void* p = allocator_.tryAllocZ(bytes);
		if( p )
		{
			CmdPtr cmd( new(p)tagCmd, cmdFreeCallback_ );
			return cmd;
		}
		return CmdPtr();
	}

	void deallocate(void *cmd)
	{
		if( cmd ) allocator_.freeZ( cmd );
	}

private:
	int32_t minSizeOfMax(int32_t maxMsgSize)
	{
		for(int32_t i = 0; i < 8; i++)
			maxMsgSize = maxMsgSize >> 1;

		if(maxMsgSize <= 64) maxMsgSize = 64;
		return maxMsgSize;
	}

public:
	Allocator<8, PolicyAlignPow2<8> > allocator_;
	cmdFreeCallback cmdFreeCallback_;
};

class TcpCmdPoolArray
{
public:
	TcpCmdPoolArray(int32_t poolNumber, int32_t poolSize, int32_t maxMsgSize)
	{
		for(int32_t i = 0; i < poolNumber; i++)
		{
			cmdPools_.push_back( CmdPoolPtr(new TcpCmdPool(poolSize, maxMsgSize)));
		}
	}

public:
	CmdPtr allocate(int64_t key, int32_t bytes)
	{
#if defined(USE_JEMALLOC)
		void* p = je_malloc(bytes);
		return CmdPtr(new(p)tagCmd, je_free);
#elif defined(USE_TCMALLOC)
		void* p = malloc(bytes);
		return CmdPtr(new(p)tagCmd, free);
#elif defined(SELF_POOL)
		int32_t idx = key % (int32_t)cmdPools_.size();
		return cmdPools_.at(idx)->allocate(bytes);
#else
		void* p = ::malloc(bytes);
		return CmdPtr(new(p)tagCmd, free);
#endif
	}



private:
	typedef boost::shared_ptr<TcpCmdPool> CmdPoolPtr;
	bstd::vector<CmdPoolPtr> cmdPools_;
};

#else
inline CmdPtr cmdAllocate(int64_t key, int32_t bytes)
{
	void* p = zmalloc(bytes);
	if( !!p ) return CmdPtr(new(p)tagCmd, zdeleter<tagCmd>);
	return CmdPtr();
}
#endif

BASE_NAME_SPACEE

#endif // TCPCMD_POOL_H