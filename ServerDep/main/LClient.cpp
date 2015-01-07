#include <LClient.h>

LClient::LClient(basio::ip::tcp::endpoint& addr, const NetworkConfig& config)
:stop_(0), stoped_(0), connLost_(1),reconnectMillisec(1*1000)
,client_(new base::TcpClient( service_, addr, "LClient", config)),display_(MAX_MSG_SEND)
{

}


LClient::~LClient( )
{

}

void LClient::regLog(bool diffHour, bool diffDay)
{

}

void floodSend(TcpConnectionPtr& session)
{
	tagHello hello;
	strncpy(hello.msg, "Copyright 2010 by backkom. All rights reserved."
		"This software is the confidential and proprietary information of backkom."
		"('Confidential Information'). You shall not disclose such Confidential -"
		"Information and shall use it only in accordance with the terms of the - "
		"license agreement you entered into with backkom", 128);
	hello.size = rand() % 128 + 8;
	session->sendCmd(&hello);
}

void LClient::tick( )
{
	int32_t connctime = 0;
	base::TimeInfo info;
	while(!stop_)
	{
		if(connection_)
		{
			if(connLost_)
			{
				connection_.reset();
				client_->reconnect();
			}
			else
			{
				CmdPtr msg = connection_->recvCmd( );
				(void)msg;
				//floodSend(connection_);
				//if(!!msg)connection_->sendCmd(msg.get());
			}
		}
		else 
		{
			connctime += 5000;
			if( connctime > 15000)
			{
				client_->reconnect();
			}
		}


		base::thisThreadSleep(10);
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

void LClient::stop( )
{
	stop_ = 1;
	tickThread_.join( );
	stoped_ = 1;
	client_->stop( );
	connection_.reset( );
}

void LClient::start()
{
	client_->setMessageCallback(boost::bind(&LClient::messageCall, this, _1, _2));
	client_->setConnectionCallback( boost::bind(&LClient::newConnection, this, _1));
	client_->setdelconnectionCallback( boost::bind(&LClient::delConnection, this, _1));
	client_->connect( );
	tickThread_ = boost::thread(boost::bind(&LClient::tick, this));
}

void LClient::delConnection( const TcpConnectionPtr& session )
{
	connLost_ = 1;
	LOGD("Byte-%s", session->name().c_str());
}

void LClient::newConnection( const TcpConnectionPtr& session )
{
	connLost_ = 0;
	connection_ = session;
	if( connection_ )
	{
		tagHello hello;
		strncpy(hello.msg, "Copyright 2010 by backkom. All rights reserved."
			"This software is the confidential and proprietary information of backkom."
			"('Confidential Information'). You shall not disclose such Confidential -"
			"Information and shall use it only in accordance with the terms of the - "
			"license agreement you entered into with backkom", 128);
		connection_->sendCmd(&hello);
		++display_;
	}
	LOGD("Hello-%s", session->name().c_str());
}

void LClient::messageCall(const TcpConnectionPtr& session, const tagCmd* cmd)
{
	tagHello hello;
	strncpy(hello.msg, "Copyright 2010 by backkom. All rights reserved."
		"This software is the confidential and proprietary information of backkom."
		"('Confidential Information'). You shall not disclose such Confidential -"
		"Information and shall use it only in accordance with the terms of the - "
		"license agreement you entered into with backkom", 128);
	hello.size = rand() % 128 + 8;
	session->sendCmd(&hello);
	++display_;
}



