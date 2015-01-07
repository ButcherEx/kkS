#ifndef TCP_CLINET_H
#define TCP_CLINET_H

#include <PreCompier.h>
#include <TcpCmdPool.h>

BASE_NAME_SPACES


class TcpClient : boost::noncopyable
{
public:
	TcpClient(basio::io_service& service, basio::ip::tcp::endpoint addr, const bstd::string& name, const NetworkConfig& config);
	~TcpClient( );
public:
	void connect();
	void reconnect();
	void disconnect();
	void stop();
public:
	void setConnectionCallback(const NewconnectionCallback& cb)
	{ newconnectionCallback_ = cb; }

	void setdelconnectionCallback(const DelconnectionCallback& cb)
	{ delconnectionCallback_ = cb; }


	void setMessageCallback(const MessageCallback& cb)
	{ messageCallback_ = cb; }

	void setnetworkConfig(const NetworkConfig& config ) { netConfig_ = config; }
	const NetworkConfig&  networkConfig( ) const { return netConfig_; }

	TcpConnectionPtr connection( ) { return connection_; }
private:
	void loop();
	void newConnection(TcpConnectionPtr& conn);
	void delConnection(const TcpConnectionPtr& conn);
	void handleConnection(const bsys::error_code& ec, TcpConnectionPtr& conn);
private:
	bstd::string name_;
	basio::io_service& service_;
	basio::ip::tcp::endpoint serverAddr_;
	TcpConnectionPtr connection_;
	NewconnectionCallback newconnectionCallback_;
	DelconnectionCallback delconnectionCallback_;
	MessageCallback messageCallback_;

#if defined(USE_SELF_POOL)
	boost::scoped_ptr<TcpCmdPoolArray> tcpCmdPoolArray_;
#endif
	CmdAllocateCallback cmdAllocateCallback_;

	NetworkConfig netConfig_;
	boost::thread loopThread_;

	enum States { kDisconnected, kConnecting, kConnected };
	bool isState(States se) { return state_.get() == se;};
//	boost::atomic_int8_t state_;
	AtomicInt32 state_;
};
BASE_NAME_SPACEE


#endif