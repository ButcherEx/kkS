#include <LServer.h>

LServer::LServer(basio::ip::tcp::endpoint& addr, const NetworkConfig& config)
:stop_(0), stoped_(false), server_(new base::TcpServer(service_, addr, "LS", config))//,display_(MSGTRANS)
{

}
LServer::~LServer( )
{

}

void LServer::tick( )
{
	base::TimeInfo info;
	while(!stop_)
	{
		if ( 1 )
		{
			ScopedLock lock(lock_);
			BOOST_AUTO(it, connections_.begin());
			for ( ;it != connections_.end(); ++it)
			{
				CmdPtr msg = it->second->recvCmd( );
				for( ; msg; msg = it->second->recvCmd())
				{
					it->second->sendCmd(msg.get());
				}
			}
		}
		base::thisThreadSleep(1);
		
		info.update();
		if(info.diffDay() || info.diffHour())
		{
			regLog(info.diffHour(), info.diffDay());
		}

		if(((info.sysRunTime()/1000)%5) == 0)
		{
			
		}
	}
}

void LServer::regLog(bool diffHour, bool diffDay)
{

}

void LServer::start()
{
	regLog(true, true);

	server_->setMessageCallback(boost::bind(&LServer::messageCall, this, _1, _2));
	server_->setConnectionCallback( boost::bind(&LServer::newConnection, this, _1));
	server_->setdelconnectionCallback( boost::bind(&LServer::delConnection, this, _1));
	tickThread_ = boost::thread(boost::bind(&LServer::tick, this));
	server_->start( );
}

void LServer::stop( )
{
	stop_ = 1;
	tickThread_.join( );
	stoped_ = true;

	server_->stop( );
	connections_.clear();
}

void LServer::messageCall(const TcpConnectionPtr& session, const tagCmd* cmd)
{
	session->sendCmd(cmd);
}

void LServer::delConnection( const TcpConnectionPtr& session )
{
	if ( 1 )
	{
		ScopedLock lock(lock_);
		connections_.erase(session->name());
		LOGD("Byte-%s", session->name().c_str());
	}
}
void LServer::newConnection( const TcpConnectionPtr& session )
{
	if ( 1 )
	{
		ScopedLock lock(lock_);
		connections_[session->name()] = session;
		LOGD("Hello-%s", session->name().c_str());
	}
}


