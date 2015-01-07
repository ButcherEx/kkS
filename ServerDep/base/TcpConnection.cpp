#include <TcpConnection.h>
#include <lzf/lzf.h>

BASE_NAME_SPACES

AtomicInt32 TcpConnection::sRecvCmdSizeAll;

TcpConnection::TcpConnection(basio::io_service& service, const bstd::string &name, const NetworkConfig& config)
:service_(service), socket_(service), name_(name)
,netConfig_(config)
,sendBuffer_(kPreHeadSize, config.sendBufferSize)
,recvBuffer_(kPreHeadSize, config.recvBufferSize)
,isAsynWriting_(0),isAsynReading_(0),isCalledDelCallbak_(0),recvBytes_(0)
{
	msgSendListSize_ = 0;
}

TcpConnection::~TcpConnection( )
{
}

bool TcpConnection::sendCmd(const tagCmd* cmd)
{
	_MY_TRY
	{
		if( connected() )
		{	
			if( cmd->size <= netConfig_.maxCmdSize )
			{
				if(!!cmdAllocateCallback_)
				{
					CmdPtr cmdPtr = cmdAllocateCallback_((int64_t)this, cmd->size);
					_Assert(cmdPtr, "Run out of memory.")
					if( cmdPtr )
					{
						memcpy(cmdPtr.get(), cmd, cmd->size);
						cmdSendList_.pushBack(cmdPtr);
						doWrite();
					}
					return true;
				}	
			}
		}
	}
	_MY_CATCH
	{
		shutdown( );
		return false;
	}
	

	return false;
}

CmdPtr TcpConnection::recvCmd( )
{
	CmdPtr cmdPtr;
	if( connected() )
	{//..
		cmdPtr = cmdRecvList_.popFront( );
	}
	return cmdPtr;
}

void TcpConnection::shutdown( )
{
	bsys::error_code ec;
	state_.set(kDisconnecting);
	socket_.shutdown(basio::ip::tcp::socket::shutdown_both, ec);
}

void TcpConnection::forceClose( )
{
	bsys::error_code ec;
	state_.set(kDisconnecting);
	socket_.close(ec);
}

void TcpConnection::noblocking(bool mode)
{
	bsys::error_code ec;
	socket_.non_blocking( mode, ec);
}

void TcpConnection::setReuse(bool on )
{
	bsys::error_code ec;
	basio::socket_base::reuse_address reuse(on);
	socket_.set_option( reuse, ec );
}
void TcpConnection::setTcpNodelay(bool on)
{
	bsys::error_code ec;
	basio::ip::tcp::no_delay no_delay(on);
	socket_.set_option(no_delay, ec);
}
void TcpConnection::setLinger(bool on, int32_t ms)
{
	bsys::error_code ec;
	basio::socket_base::linger linger(on, ms);
	socket_.set_option( linger, ec);
}

void TcpConnection::onEstablish( )
{
	state_.set(kConnected);
	if(newconnectionCallback_)newconnectionCallback_(shared_from_this());
	doRead( );
}

void TcpConnection::doRead( )
{
	if( isAsynReading_.compareAndSet(0, 1) == 0 )
	{
		basio::mutable_buffers_1 mb(recvBuffer_.beginBuffer(), 
			recvBuffer_.prependableBytes() + recvBuffer_.writableBytes());
		basio::async_read(socket_, mb, 
			boost::bind(&TcpConnection::readComletion, shared_from_this(), 
			basio::placeholders::error, basio::placeholders::bytes_transferred),
			make_custom_alloc_handler( recvHandlerAllocator_, boost::bind(&TcpConnection::handleRead, shared_from_this(), 
			basio::placeholders::error, basio::placeholders::bytes_transferred)));
	}
}

//a return value of 0 indicates that the read operation is complete. a non-zero value indicates the maximum number
//of bytes to be read on the next call to the stream's async_read_some function. ---basio::async_read
//read as many as possible to reduce async call-back(TcpConnection::handleRead).
size_t TcpConnection::readComletion(const bsys::error_code& ec, size_t bytes_transferred)
{
	size_t left2ReadBytes = 0;
	if( !ec )
	{
		if(bytes_transferred < kPreHeadSize)
		{
			left2ReadBytes = kPreHeadSize - bytes_transferred;
		}
		else
		{
			const void *data = recvBuffer_.beginBuffer();
			int32_t msgLenNet = *static_cast<const int32_t*>(data);
			int32_t msgLenHost = networkToHost32( msgLenNet );
			if( msgLenHost <= netConfig_.maxCmdSize && msgLenHost > 0) 
			{
				if( (int32_t)bytes_transferred < (msgLenHost + kPreHeadSize) ) 
					left2ReadBytes = msgLenHost  + kPreHeadSize - bytes_transferred;
				else
				{
					recvBuffer_.hasWritten( msgLenHost );
					recvBuffer_.hasPrepend( kPreHeadSize );
				}
			}
			else
			{
				recvBuffer_.retrieveAll( );
			}
		}
	}

	return left2ReadBytes;
}
void TcpConnection::handleRead(const bsys::error_code& ec, size_t bytes_transferred)
{
	bool result = (ec || recvBuffer_.readableBytes() == 0);
	_MY_TRY
	{
		int32_t lr = isAsynReading_.subAndGet( );
		_Assert(!lr, "isAsynReading_ multi-thread!!!");

		switch ( result )
		{
		case true:
			onDestroy(ec);
			break;
		default:
			recvBytes_.addAndGet( (int32_t)bytes_transferred );
			decode( );
			doRead( );
			break;
		}
	}
	_MY_CATCH
	{
		forceClose( );
	}
	

}
void TcpConnection::decode( )
{
	StackAllocator allocator;
	PreHead* head = static_cast<PreHead*>((void*)recvBuffer_.peek());
	head->size = networkToHost32( head->size );
	bool compressed = (head->flag & MSG_FLAG_COMPRESS) == MSG_FLAG_COMPRESS;
	recvBuffer_.retrieve(kPreHeadSize);

	//--decompress
	void* outBuf = allocator.allocate( head->original );

	if( compressed ){
		int32_t decompressedSize = ::lzf_decompress(recvBuffer_.peek(),
			head->size, outBuf, head->original);
		_Assert(decompressedSize == head->original, "");
	}else {
		memcpy(outBuf, recvBuffer_.peek(), head->original);
	}

	recvBuffer_.retrieveAll();
	//-- scatter
	int32_t dealBytes = 0;
	for( ; dealBytes < head->original; )
	{
		tagCmd* cmd = static_cast<tagCmd*>((void*)((char*)outBuf+dealBytes));
		if( !!messageCallback_ )
		{
			dealBytes += cmd->size;
			messageCallback_(shared_from_this(), cmd);
			recvCmdSize_.addAndGet( );
			sRecvCmdSizeAll.addAndGet( );
		}
		else if(!!cmdAllocateCallback_)
		{
			CmdPtr msg = cmdAllocateCallback_((int64_t)this, cmd->size );
			if(msg)
			{
				memcpy(msg.get(), cmd, cmd->size);
				cmdRecvList_.pushBack(msg);
				dealBytes += cmd->size;
				recvCmdSize_.addAndGet( );
				sRecvCmdSizeAll.addAndGet( );
			}
			else
			{
				allocator.deallocate(outBuf);
				_Assert(false, "Decode run out of memory.");
			}
		}
		else
		{
			break;
		}
	}

	allocator.deallocate(outBuf);
}

void TcpConnection::doWrite( )
{
	if( cmdSendList_.sizeUnSafe() )
	{
		// .. double check
		if( isAsynWriting_.compareAndSet(0, 1) == 0 )
		{
			// ...bug.. fixMe MultiThreads: Logic|IoThread
			if( cmdSendList_.sizeUnSafe() )
			{
				if( sendBuffer_.readableBytes() <= 0) 
					encode( );

				async_write(socket_, basio::const_buffers_1(sendBuffer_.peek(),sendBuffer_.readableBytes()), 
					make_custom_alloc_handler( sendHandlerAllocator_, 
					boost::bind(&TcpConnection::handleWrite, shared_from_this(),
					basio::placeholders::error, basio::placeholders::bytes_transferred)));
			} 
			else
			{
				int32_t res = isAsynWriting_.compareAndSet(1, 0);
				_Assert(res == 1, "isAsynWriting_ multi-thread!!!");
			}
		}
	}
}

int32_t TcpConnection::encode( )
{
	//--
	// 使用静态buf，64k 使用栈空间，否则从堆上分配 windows下栈空间 1M
	PreHead head = {0,netConfig_.ioFlag,0,0};
	int32_t bytesAll = 0, bytesOriginal = 0;
	StackAllocator allocator;

	char *buf = static_cast<char*>(allocator.allocate( netConfig_.sendBufferSize ));

	//-- gather
	bool fill = true;
	for( ; fill; fill = ((netConfig_.ioFlag & MSG_FLAG_GATHER) == MSG_FLAG_GATHER ))
	{
		CmdPtr cmdPtr = cmdSendList_.popFront( );
		if(!cmdPtr) break;
		if(cmdPtr->size + bytesOriginal + kPreHeadSize > netConfig_.sendBufferSize )
		{
			cmdSendList_.pushFront(cmdPtr);
			break;
		}

		memcpy(buf+bytesOriginal, cmdPtr.get(), cmdPtr->size);
		bytesOriginal += cmdPtr->size;
	}

	//-- compress
	bool compressed = (netConfig_.ioFlag & MSG_FLAG_COMPRESS) == MSG_FLAG_COMPRESS;
	if( compressed && bytesOriginal > 0 )
	{
		bytesAll = lzf_compress(buf, bytesOriginal, sendBuffer_.beginWrite(), sendBuffer_.writableBytes());		
		if( bytesAll == 0 ) compressed = false;
	}

	if( !compressed && bytesOriginal > 0 )
	{
		bytesAll = bytesOriginal;
		head.flag &= ~MSG_FLAG_COMPRESS;
		sendBuffer_.append( buf, bytesOriginal);
	}
	else if( compressed )
	{
		sendBuffer_.hasWritten(bytesAll);
	}

	head.original = bytesOriginal;
	head.size = hostToNetwork32(bytesAll);
	sendBuffer_.prepend( &head, kPreHeadSize );

	allocator.deallocate(buf);
	msendBytes_.addAndGet( (kPreHeadSize+bytesOriginal) );

	return bytesOriginal>0 ? (kPreHeadSize + bytesAll) : 0;
}

void TcpConnection::handleWrite(const bsys::error_code& ec, size_t bytes_transferred)
{
	_MY_TRY
	{
		bool result = (!!ec);

		switch ( result )
		{
		case true:
			isAsynWriting_.subAndGet( );
			onDestroy(ec);
			break;
		default:
			sendBytes_.addAndGet((int32_t)bytes_transferred);
			sendBuffer_.retrieve((int32_t)bytes_transferred);
			_Assert(sendBuffer_.readableBytes() == 0, "");
			isAsynWriting_.subAndGet( ) ;
			doWrite( );
			break;
		}
	}
	_MY_CATCH
	{
		forceClose( );
	}
	
}

void TcpConnection::onDestroy(const bsys::error_code& ec)
{
	int32_t n = cmdSendList_.size();
	if(isCalledDelCallbak_.compareAndSet(0, 1) == 0)
	{
		state_.set( kDisconnecting );
		TcpConnectionPtr guardThis(shared_from_this());
		if(delconnectionCallback_)delconnectionCallback_( guardThis );
	}
}

BASE_NAME_SPACEE
