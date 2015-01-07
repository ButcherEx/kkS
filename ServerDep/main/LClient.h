#ifndef LCLIENT_H
#define LCLIENT_H

#include <PreCompier.h>
#include <TcpConnection.h>
#include <TcpServer.h>
#include <Script/Script.h>
#include <LProtocol.h>

class LClient
{
public:
	LClient(basio::ip::tcp::endpoint& addr,const NetworkConfig& config);
	~LClient( );
public:
	void tick( );
	void start();
	void stop( );
private:
	void delConnection( const TcpConnectionPtr& session );
	void newConnection( const TcpConnectionPtr& session );
	void messageCall(const TcpConnectionPtr& session, const tagCmd* cmd);
protected:
	void regLog(bool diffHour, bool diffDay);
private:
	int32_t flag_;
	volatile int32_t connLost_;
	volatile int32_t stop_;
	volatile int32_t stoped_;
	basio::io_service service_;
	TcpConnectionPtr connection_;
	boost::scoped_ptr<base::TcpClient> client_;
	boost::thread loopThread_;
	boost::thread tickThread_;

	int32_t reconnectMillisec;
	boost::progress_display display_;
};

#endif