#include <TcpServer.h>
#include <TcpConnection.h>

BASE_NAME_SPACES

TcpServer::TcpServer(basio::io_service& service, basio::ip::tcp::endpoint addr, const bstd::string& name, const NetworkConfig& config)
:name_(name), service_(service), acceptor_(service)
,netConfig_(config),serverAddr_(addr),state_(kStopped)
,threadPool_(new ThreadPool(config.threadPoolSize))
,acceptExceptionTimer_(service)
#if defined(USE_SELF_POOL)
,connectionPool_(new ConnetionPool(config.maxConnectionSize * sizeof(TcpConnection), sizeof(TcpConnection)))
,tcpCmdPoolArray_(new TcpCmdPoolArray(config.maxCmdPoolNumber, config.maxCmdPoolSize, config.maxCmdSize))
#endif
{
#if defined(USE_SELF_POOL)
	recyleCallback_ = boost::bind(&TcpServer::deallocate, this, _1);
	cmdAllocateCallback_ = boost::bind(&TcpCmdPoolArray::allocate, tcpCmdPoolArray_.get(), _1, _2);
#else
	cmdAllocateCallback_ = boost::bind(cmdAllocate, _1, _2);
#endif
}


TcpServer::~TcpServer( )
{
}
void TcpServer::start( )
{
	if( isState(kStopped) )
	{
		state_.set(kStarting);
		bsys::error_code ec;
		acceptor_.open(serverAddr_.protocol(), ec); 
		_Assert(!ec, ec.message().c_str());
#ifndef NOT_REUSE_ADDRESS
		acceptor_.set_option(basio::ip::tcp::acceptor::reuse_address(true), ec); 
		_Assert(!ec, ec.message().c_str());
#endif
		acceptor_.bind(serverAddr_, ec); 
		_Assert(!ec, ec.message().c_str());

		acceptor_.listen(basio::ip::tcp::acceptor::max_connections, ec);
		_Assert(!ec, ec.message().c_str());

		acceptConnection( );
		loop( );
		state_.set(kStarted);
	}
}

void TcpServer::loop( )
{
	_MY_TRY
	{
		for(int32_t i = 0; i < netConfig_.threadPoolSize; i++)
			threadPool_->schedule( boost::bind(&basio::io_service::run, &service_));
	}
	_MY_CATCH
	{

	}
	
}

void TcpServer::stop( )
{
	if( isState(kStarted ) )
	{
		state_.set(kStopping);

		//////////////////////////////////////////////////////////////////////////
		if( 1 )
		{
			LOGD("acceptExceptionTimer_.cancel...");
			bsys::error_code ec;
			acceptExceptionTimer_.cancel(ec);
			LOGD("acceptExceptionTimer_.cancel done.");
		}

		//////////////////////////////////////////////////////////////////////////
		if( 1 ) // stop acceptor
		{
			LOGD("acceptor_.close...");
			bsys::error_code ec;
			acceptor_.close(ec);
			LOGD("acceptor_.close done.");
		}

		//////////////////////////////////////////////////////////////////////////
		if( 1 )//fore all asyn-io return
		{
			LOGD("all connection->shutdown()...");
			ScopedLock lock(lock_);
			BOOST_AUTO(it, connections_.begin());
			for ( ;it != connections_.end(); ++it)
			{
				TcpConnectionPtr conn = it->second;
				conn->shutdown( );
			}
			LOGD("all connection->shutdown() done.");
		}

		LOGD("threadPool_->wait...");
		threadPool_->wait( );
		LOGD("threadPool_->wait done.");

		//////////////////////////////////////////////////////////////////////////
		if( 1 ) // reset
		{
			LOGD("service_.reset...");
			bsys::error_code ec;
			service_.run( ec );
			service_.reset( );
			LOGD("service_.reset done.");
		} 

		state_.set(kStopped);
	}
}
#if defined(USE_SELF_POOL)
TcpConnectionPtr TcpServer::allocate( )
{
	void *p = connectionPool_->allocZ( sizeof(TcpConnection) );
	if( p != NULL)
	{
		TcpConnectionPtr conn(new (p) TcpConnection(service_, "unknown#unknown",
			netConfig_), recyleCallback_);
		return conn;
	}
	return TcpConnectionPtr();
}
void TcpServer::deallocate(TcpConnection *conn)
{
	conn->~TcpConnection( );
	connectionPool_->freeZ(conn);
}
#else
TcpConnectionPtr TcpServer::allocate( )
{
	return TcpConnectionPtr(znew TcpConnection(service_, "unknown#unknown",
		netConfig_), zdeleter<TcpConnection>);
}
#endif


void TcpServer::acceptConnection( )
{
	if( isState(kStarted) || isState(kStarting) )
	{
		TcpConnectionPtr conn = allocate();
		if( !!conn )
		{
			acceptor_.async_accept( conn->socket(), 
				boost::bind(&TcpServer::handleAccept, this, basio::placeholders::error, conn) );
		}
		else
		{
			acceptExceptionTimer_.expires_from_now(boost::posix_time::seconds(netConfig_.maxAcceptionExceptionSecond));  
			acceptExceptionTimer_.async_wait(boost::bind(&TcpServer::acceptExceptionHanlder,
				this, basio::placeholders::error));  
		}
	}
}


void TcpServer::acceptExceptionHanlder(const bsys::error_code& ec)
{
	if(!ec)
	{
		acceptConnection( );
	}
}

void TcpServer::handleAccept(const bsys::error_code& ec, const  TcpConnectionPtr& conn)
{
	if( !ec )
	{
		newConnection( conn );
	}

	if( !isState(kStopping) )
	{
		acceptConnection( );
	}
}

void TcpServer::newConnection(const TcpConnectionPtr& conn)
{
	int32_t connectionSize = 0;
	{
		ScopedLock lock(lock_);
		connectionSize = (int32_t)connections_.size();
	}

	if( connectionSize < netConfig_.maxConnectionSize )
	{
		_MY_TRY
		{
			bsys::error_code ec;
			basio::ip::tcp::endpoint peerAddr = conn->socket().remote_endpoint(ec);
			_Assert(!ec, ec.message().c_str());

			basio::ip::tcp::endpoint localAddr = conn->socket().local_endpoint(ec);
			_Assert(!ec, ec.message().c_str());

			std::ostringstream oss;
			oss << name_ << ":" 
				<< peerAddr.address( ).to_string()
				<< "#" << peerAddr.port();
			bstd::string connName = oss.str( ).c_str();

			conn->setName(connName);
			conn->setLinger(true,0);
			conn->setTcpNodelay(true);
			conn->setMessageCallback( messageCallback_);
			conn->setCmdAllocateCallback(cmdAllocateCallback_);
			conn->setConnectionCallback( newconnectionCallback_ );
			conn->setdelconnectionCallback(boost::bind(&TcpServer::delConnection, this, _1));
			conn->onEstablish( );

			if ( 1 )
			{
				ScopedLock lock(lock_);
				connections_[connName] = conn;
			}
		}
		_MY_CATCH
		{

		}
		
	}
	else
	{
		conn->shutdown( );
	}
}

void TcpServer::delConnection(const TcpConnectionPtr& conn)
{
	if ( 1 )
	{
		ScopedLock lock(lock_);
		size_t n = connections_.erase(conn->name());
		(void)n;
		_Verify(n == 1, "duplicate connection name.");
	}

	if(delconnectionCallback_)delconnectionCallback_(conn); 
}

BASE_NAME_SPACEE
