 #include <TcpClient.h>
 #include <TcpConnection.h>

BASE_NAME_SPACES

 TcpClient::TcpClient(basio::io_service& service, basio::ip::tcp::endpoint addr, const bstd::string& name, const NetworkConfig& config)
 :service_(service), serverAddr_(addr), name_(name),netConfig_(config),state_(kDisconnected)
#if defined(USE_SELF_POOL)
 ,tcpCmdPoolArray_(new TcpCmdPoolArray(config.maxCmdPoolNumber, config.maxCmdPoolSize, config.maxCmdSize))
#endif
 {
#if defined(USE_SELF_POOL)
	cmdAllocateCallback_ = boost::bind(&TcpCmdPoolArray::allocate, tcpCmdPoolArray_.get(), _1, _2);
#else
	cmdAllocateCallback_ = boost::bind(cmdAllocate, _1, _2);
#endif
 }
 
 TcpClient::~TcpClient( )
 {
 
 }
 
 void TcpClient::connect( )
 {
	if( isState(kDisconnected) && !connection_ )
	{
		service_.reset();
		TcpConnectionPtr conn(znew TcpConnection(service_, "unknown#unknown", 
			netConfig_), zdeleter<TcpConnection>);
		
		state_.set(kConnecting);
		connection_ = conn;
  		conn->socket().async_connect(serverAddr_, 
  			boost::bind(&TcpClient::handleConnection, this, basio::placeholders::error, conn));
		loopThread_ = boost::thread(boost::bind(&basio::io_service::run, &service_));
	}
 }

 void TcpClient::reconnect()
 {
	if(isState(kDisconnected))
	{
		if(connection_) 
		{
			connection_->forceClose();
			connection_.reset( );

			if(loopThread_.joinable()) loopThread_.join();

			bsys::error_code ec;
			service_.run( ec );
			service_.reset();
		}
		connect( );
	}
 }

 
 void TcpClient::handleConnection(const bsys::error_code& ec, TcpConnectionPtr& conn)
 {
 	switch (!ec)
 	{
 	case true:
 		newConnection(conn);
 		break;
 	default:
		state_.set(kDisconnected);
 		break;
 	} 
 }
 
 void TcpClient::newConnection(TcpConnectionPtr& conn)
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
		 connection_ = conn;
		 connection_->setName(connName);
		 connection_->setLinger(true,0);
		 connection_->setTcpNodelay(true);
		 connection_->setMessageCallback(messageCallback_);
		 connection_->setConnectionCallback(newconnectionCallback_);
		 connection_->setCmdAllocateCallback(cmdAllocateCallback_);
		 connection_->setdelconnectionCallback(boost::bind(&TcpClient::delConnection, this, _1));
		 connection_->onEstablish( );
		 state_.set(kConnected);
	 }
	 _MY_CATCH
	 {
		 state_.set(kDisconnected);
	 }
	 
 }
 
 
 void TcpClient::disconnect( )
 {
	 if(connection_)
 		connection_->shutdown( );
 }
 
 void TcpClient::stop( )
 {
	 state_.set(kDisconnected);
	 if(connection_)
		connection_->forceClose();
	 loopThread_.join( );
 }

 void TcpClient::loop()
 {
	if(connection_)
	{
		_MY_TRY
		{
			bsys::error_code ec;
			service_.run(ec);

			_Assert(!ec, ec.message().c_str());
		}
		_MY_CATCH
		{

		}
		
	}
 }

 
 void TcpClient::delConnection(const TcpConnectionPtr& conn)
 {
	 _MY_TRY
	 {
		 if(delconnectionCallback_)delconnectionCallback_( conn );
		 connection_.reset( );
	 }
	 _MY_CATCH
	 {

	 }
	 

	state_.set(kDisconnected);
 }

BASE_NAME_SPACEE
