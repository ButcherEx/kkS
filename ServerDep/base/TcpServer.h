#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <PreCompier.h>
#include <Allocator.h>
#include <TcpCmdPool.h>

BASE_NAME_SPACES

class TcpServer : boost::noncopyable
{
	enum StateE { kStopped, kStarting, kStarted, kStopping };
public:
	TcpServer(basio::io_service& service, basio::ip::tcp::endpoint addr, const bstd::string& name, const NetworkConfig& config);
	~TcpServer( );
public:
	void start( );
	void stop( );
public:
	void setConnectionCallback(const NewconnectionCallback& cb)
	{ newconnectionCallback_ = cb; }

	void setdelconnectionCallback(const DelconnectionCallback& cb)
	{ delconnectionCallback_ = cb; }

	void setMessageCallback(const MessageCallback& cb)
	{ messageCallback_ = cb; }

	void setnetworkConfig(const NetworkConfig& config ) { netConfig_ = config; }
	const NetworkConfig&  networkConfig( ) const { return netConfig_; }
private:
	bool isState(StateE se) { return state_.get() == se; }
private:
	void loop( );
	void acceptConnection( );
	void acceptExceptionHanlder(const bsys::error_code& ec);
	void handleAccept(const bsys::error_code& ec, const TcpConnectionPtr& conn);
	void newConnection(const TcpConnectionPtr& conn);
	void delConnection(const TcpConnectionPtr& conn);
private:
	TcpConnectionPtr allocate( );
#if defined(USE_SELF_POOL)
	void deallocate(TcpConnection *conn);
#endif
private:
	bstd::string name_;
	basio::io_service& service_;
	basio::ip::tcp::acceptor acceptor_;
	basio::ip::tcp::endpoint serverAddr_;
	NewconnectionCallback newconnectionCallback_;
	DelconnectionCallback delconnectionCallback_;
	MessageCallback messageCallback_;
	basio::deadline_timer acceptExceptionTimer_;

#if defined(USE_SELF_POOL)
	boost::scoped_ptr<TcpCmdPoolArray> tcpCmdPoolArray_;
#endif
	CmdAllocateCallback cmdAllocateCallback_;


#if defined(USE_SELF_POOL)
	typedef Allocator<1> ConnetionPool;
	boost::scoped_ptr<ConnetionPool> connectionPool_;
	RecyleConnectionfromCallback recyleCallback_;
#endif

	typedef bstd::map<bstd::string, TcpConnectionPtr> ConnectionMap;

	Mutex lock_;
	int32_t nextConnId_;
	NetworkConfig netConfig_;
	ConnectionMap connections_;
	AtomicInt32 state_;
	boost::scoped_ptr<ThreadPool> threadPool_;
};

BASE_NAME_SPACEE

#endif