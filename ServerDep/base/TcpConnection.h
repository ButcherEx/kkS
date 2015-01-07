#ifndef TCP_CONNCTION_H
#define TCP_CONNCTION_H

#include <PreCompier.h>
#include <Buffer.h>
#include <AtomicInt32.h>
#include <TcpCmdPool.h>

BASE_NAME_SPACES

typedef TSList<CmdPtr> TSCmdList;

class TcpConnection : boost::noncopyable,
		public boost::enable_shared_from_this<TcpConnection>
{
	enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
public:
	class PreHead{public: int32_t size, flag, original, reserved; } ;
	const static int32_t kPreHeadSize = sizeof(PreHead);
	const static int32_t kMSGHeadSize = kPreHeadSize;
	const static int32_t kStackAllocSize = 8 * 1024;
public:
	TcpConnection(basio::io_service& service, const bstd::string &name, const NetworkConfig& config);
	~TcpConnection( );
public:
	bool sendCmd(const tagCmd* cmd);
	CmdPtr recvCmd( );
public:
	void setConnectionCallback(const NewconnectionCallback& cb)
	{ newconnectionCallback_ = cb; }

	void setCmdAllocateCallback(const CmdAllocateCallback& cb)
	{ cmdAllocateCallback_ = cb; }

	void setdelconnectionCallback(const DelconnectionCallback& cb)
	{ delconnectionCallback_ = cb; }

	void setMessageCallback(const MessageCallback& cb)
	{ messageCallback_ = cb; }

	void setState(StateE eState) { state_.set(eState); }
	bool connected( ) { return state_.get() == kConnected; }
	basio::ip::tcp::socket& socket( ) { return socket_; }
public:
	void shutdown( );
	void forceClose( );
	void noblocking(bool mode);
	void setReuse(bool on );
	void setTcpNodelay(bool on);
	void setLinger(bool on, int32_t ms=0);
	void onEstablish( );
	void setName(const bstd::string &name) { name_ = name; }
	const bstd::string& name( ) const { return name_; }
	int32_t recvListSize( )  { return cmdRecvList_.size(); }
	int32_t sendListSize( )  { return cmdSendList_.size(); }
private:
	void doRead( );
	void decode( );
	void doWrite( );
	int32_t encode( );
	void handleRead(const bsys::error_code& ec, size_t bytes_transferred);
	void handleWrite(const bsys::error_code& ec, size_t bytes_transferred);
private:
	void onDestroy(const bsys::error_code& ec);
	size_t readComletion(const bsys::error_code& ec, size_t bytes_transferred);
private:

	typedef HandlerAllocator<kStackAllocSize> StackAllocator;
	typedef HandlerAllocator<160> IoHandlerAllcator;

	bstd::string	name_;
	basio::io_service& service_;
	basio::ip::tcp::socket socket_;
	CmdAllocateCallback cmdAllocateCallback_;
	MessageCallback messageCallback_;
	NewconnectionCallback newconnectionCallback_;
	DelconnectionCallback delconnectionCallback_;
	IoHandlerAllcator recvHandlerAllocator_;
	IoHandlerAllcator sendHandlerAllocator_;
	AtomicInt32 state_;
	AtomicInt32 isAsynWriting_;
	AtomicInt32 isAsynReading_;
	AtomicInt32 isCalledDelCallbak_;

	Buffer sendBuffer_;
	Buffer recvBuffer_;

	//---
	NetworkConfig netConfig_;

	//--
	TSCmdList cmdRecvList_;
	TSCmdList cmdSendList_;

	// ...
	AtomicInt32 recvBytes_;
	AtomicInt32 sendBytes_;
	AtomicInt32 msendBytes_;
	AtomicInt32 recvCmdSize_;
	AtomicInt32 msgSendListSize_;
public:
	static AtomicInt32 sRecvCmdSizeAll;
};

BASE_NAME_SPACEE


#endif