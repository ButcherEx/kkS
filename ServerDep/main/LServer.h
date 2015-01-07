#ifndef LSERVER_H
#define LSERVER_H

#include <PreCompier.h>
#include <TcpConnection.h>
#include <TcpServer.h>
#include <Script/Script.h>

class LServer
{
public:
	LServer(basio::ip::tcp::endpoint& addr, const NetworkConfig& config);
	~LServer( );
public:
	void tick( );
	void start();
	void stop( );
private:
	void delConnection( const TcpConnectionPtr& session );
	void newConnection( const TcpConnectionPtr& session );
	void messageCall(const TcpConnectionPtr& session, const tagCmd* cmd);
public:
	void regLog(bool diffHour, bool diffDay);
private:
	volatile int32_t stop_;
	volatile bool stoped_;
	basio::io_service service_;

	typedef bstd::map<bstd::string, TcpConnectionPtr> ConnectionMap;

	Mutex lock_;
	ConnectionMap connections_;
	boost::scoped_ptr<typename base::TcpServer> server_;
	boost::thread tickThread_;

	//progress_display display_;
};

#endif