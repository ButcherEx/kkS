
#include "Player.h"
#include "LogDefine.h"

POOL_IMPL(Player);

Player::Player( bool bIsServer )
:m_Role(*this)
,m_SocketInputStream(m_Socket) 
,m_SocketOutputStream(m_Socket)
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

Player::~Player( )
{
__ENTER_FUNCTION
	CleanUp();
__LEAVE_FUNCTION
}

void Player::CleanUp( )
{
__ENTER_FUNCTION
	m_Socket.close() ;
	m_SocketInputStream.CleanUp() ;
	m_SocketOutputStream.CleanUp() ;
__LEAVE_FUNCTION
}

void Player::Disconnect( )
{
__ENTER_FUNCTION

	_MY_TRY
	{
		m_Socket.close() ;
	}
	_MY_CATCH
	{
	}

__LEAVE_FUNCTION
}

bool Player::IsValid( )
{
__ENTER_FUNCTION

	return m_Socket.isValid() ;

__LEAVE_FUNCTION

	return false ;
}

bool Player::ProcessInput( )
{
__ENTER_FUNCTION

	_MY_TRY 
	{
		uint32_t ret = m_SocketInputStream.Fill( ) ;
		if( (int32_t)ret <= SOCKET_ERROR )
		{
			LOGE(ServerError, "[%lld] m_pSocketInputStream->Fill ret:%d %s", 
				g_TimeManager.CurrentTimeToInt64(), (int32_t)ret, MySocketError() ) ;
			return false ;
		}
	} 
	_MY_CATCH
	{
		return false ;
	}


	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool Player::ProcessCommand( bool Option )
{
__ENTER_FUNCTION

/*
	bool ret ;

	CHAR header[PACKET_HEADER_SIZE];
	PacketID_t packetID;
	uint32_t packetuint, packetSize, packetIndex;
	uint16_t packetTick;
	Packet* pPacket = NULL ;

	_MY_TRY
	{
		if( Option ) 
		{//执行部分选项操作
		}

		for( ;; )
		{
			if( !m_SocketInputStream.Peek(&header[0], PACKET_HEADER_SIZE) )
			{//数据不能填充消息头
				break ;
			}

			this->DecryptHead_CS(header);//封包头解密

			memcpy( &packetID, &header[0], sizeof(PacketID_t) ) ;	
			memcpy( &packetTick, &header[sizeof(uint16_t)], sizeof(uint16_t) ) ;
			memcpy( &packetuint, &header[sizeof(uint16_t)+sizeof(PacketID_t)], sizeof(uint32_t) );
			packetSize = GET_PACKET_LEN(packetuint) ;
			packetIndex = GET_PACKET_INDEX(packetuint) ;

			if( packetID >= (PacketID_t)PACKET_MAX )
			{//无效的消息类型
				Assert( false ) ;
				return false ;
			}

			//消息解密处理--Begin
			{
				uint32_t t_uSize = packetSize+PACKET_HEADER_SIZE;
				uint32_t t_uHead = m_SocketInputStream.GetHead();
				uint32_t t_uTail = m_SocketInputStream.GetTail();
				uint32_t t_uBufferLen = m_SocketInputStream.GetBuffLen();
				CHAR* t_szBuffer = m_SocketInputStream.GetBuff();
				if ( t_uHead < t_uTail ) 
				{
					this->Decrypt_CS(&t_szBuffer[t_uHead], t_uSize, 0);
				} 
				else 
				{
					uint32_t rightLen = t_uBufferLen - t_uHead ;
					if( t_uSize <= rightLen ) 
					{
						this->Decrypt_CS(&t_szBuffer[t_uHead], t_uSize, 0);
					} 
					else 
					{
						this->Decrypt_CS(&t_szBuffer[t_uHead], rightLen, 0);
						this->Decrypt_CS(t_szBuffer, t_uSize-rightLen, rightLen);
					}
				}
			}
			//消息解密处理--End

			_MY_TRY
			{

				if( m_SocketInputStream.Length()<PACKET_HEADER_SIZE+packetSize )
				{//消息没有接收全
					break;
				}

				if( packetSize>g_pPacketFactoryManager->GetPacketMaxSize(packetID) )
				{//消息的大小出现异常，收到的消息比预定义消息的最大值还要大
					Assert( false ) ;
//					m_pSocketInputStream->Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return false ;
				}

				Packet* pPacket = g_pPacketFactoryManager->CreatePacket( packetID ) ;
				if( pPacket==NULL )
				{//不能分配到足够的内存
					Assert( false ) ;
//					m_pSocketInputStream->Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return false ;
				}

				//设置消息序列号
				pPacket->SetPacketIndex( packetIndex ) ;

				ret = m_SocketInputStream.ReadPacket( pPacket ) ;
				if( ret==false )
				{//读取消息内容错误
					Assert( false ) ;
					g_pPacketFactoryManager->RemovePacket( pPacket ) ;
					return false ;
				}

				bool bNeedRemove = true ;

				_MY_TRY
				{
					//修正m_KickTime信息，m_KickTime信息中的值为判断是否需要踢掉
					//客户端的依据
					this->ResetKick( ) ;

					uint32_t uret = 0;// pPacket->Execute( this ) ;
					if( uret==PACKET_EXE_ERROR )
					{//出现异常错误，断开此玩家连接
						if( pPacket ) 
							g_pPacketFactoryManager->RemovePacket( pPacket ) ;
						return false ;
					}
					else if( uret==PACKET_EXE_BREAK )
					{//当前消息的解析执行将停止
					 //直到下个循环时才继续对缓存中的数据进行消息格式
					 //化和执行。
					 //当需要将客户端的执行从一个场景转移到另外一个场景时：
					 //需要在发送转移消息后将执行在本线程中停止。
						if( pPacket ) 
							g_pPacketFactoryManager->RemovePacket( pPacket ) ;
						break ;
					}
					else if( uret==PACKET_EXE_CONTINUE )
					{//继续解析剩下的消息
					}
					else if( uret==PACKET_EXE_NOTREMOVE )
					{//继续解析剩下的消息，并且不回收当前消息
						bNeedRemove = false ;
					}
					else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
					{
						return false ;
					}
					else
					{//未知的返回值
						Assert(false) ;
					}
				}
				_MY_CATCH
				{
				}

				if( pPacket && bNeedRemove ) 
					g_pPacketFactoryManager->RemovePacket( pPacket ) ;
			}
			_MY_CATCH
			{
				return false;
			}
		}
	}
	_MY_CATCH
	{
	}


*/
	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool Player::ProcessOutput( )
{
__ENTER_FUNCTION

	_MY_TRY
	{
		uint32_t size = m_SocketOutputStream.Length() ;
		if( size==0 )
		{
			return true ;
		}

		uint32_t ret = m_SocketOutputStream.Flush( ) ;
		if( (int32_t)ret <= SOCKET_ERROR )
		{
			LOGE(ServerError,"[%lld] m_pSocketOutputStream->Flush ret:%d %s", 
				g_TimeManager.CurrentTimeToInt64(), (int32_t)ret, MySocketError() ) ;
			return false ;
		}
	} 
	_MY_CATCH
	{
		return false ;
	}

	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool Player::SendPacket( Packet* pPacket )
{
__ENTER_FUNCTION

#if 0
		pPacket->SetPacketIndex( m_PacketIndex++ ) ;

		PacketID_t packetID = pPacket->GetPacketID() ;
		uint32_t w = m_SocketOutputStream.Write( PACK_COMPART , PACK_COMPART_SIZE ) ;

		uint32_t t_uTail_Begin = m_SocketOutputStream.GetTail();//查询当前包尾位置。记录写包前位置

		w = m_SocketOutputStream.Write( (CHAR*)&packetID , sizeof(PacketID_t) ) ;

		uint16_t packetTick = g_TimeManager.RunTick();
		w = m_SocketOutputStream.Write( (CHAR*)&packetTick , sizeof(uint16_t) ) ;

		uint32_t packetUINT ;
		uint32_t packetSize = pPacket->GetPacketSize( ) ;
		uint32_t packetIndex = pPacket->GetPacketIndex( ) ;

		SET_PACKET_INDEX(packetUINT, packetIndex) ;
		SET_PACKET_LEN(packetUINT, packetSize) ;

		w = m_SocketOutputStream.Write( (CHAR*)&packetUINT, sizeof(uint32_t) ) ;

		bool ret = pPacket->Write( m_SocketOutputStream ) ;

		uint32_t t_uTail_End = m_SocketOutputStream.GetTail();//查询当前包尾位置。记录写包后位置

		//消息加密处理--Begin
		{
			uint32_t t_uSize = t_uTail_End - t_uTail_Begin;
			uint32_t t_uHead = m_SocketOutputStream.GetHead();
			uint32_t t_uTail = m_SocketOutputStream.GetTail();
			uint32_t t_uBufferLen = m_SocketOutputStream.GetBuffLen();
			CHAR* t_szBuffer = m_SocketOutputStream.GetBuff();
			if ( t_uHead < t_uTail ) 
			{
				this->Encrypt_SC(&(t_szBuffer[t_uTail_Begin]), t_uSize, 0);
			} 
			else
			{
				uint32_t rightLen = t_uBufferLen - t_uHead ;
				if( t_uSize <= rightLen ) 
				{
					this->Encrypt_SC(&(t_szBuffer[t_uTail_Begin]), t_uSize, 0);
				} 
				else 
				{
					this->Encrypt_SC(&(t_szBuffer[t_uTail_Begin]), rightLen, 0);
					this->Encrypt_SC(t_szBuffer, t_uSize-rightLen, rightLen);
				}
			}
		}
		//消息加密处理--End

		//BOOL ret = m_pSocketOutputStream->WritePacket( pPacket ) ;
		Assert( ret ) ;
		//LOG_SAVE(Error, "SendPacket! SOCKET=%d, ID=%d", 
		//	m_pSocket->getSOCKET(), pPacket->GetPacketID() ) ;
		
#endif

	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool Player::HeartBeat( uint32_t uTime )
{
__ENTER_FUNCTION



	return true ;

__LEAVE_FUNCTION

	return false ;
}

uint32_t Player::HandlePacket(const CG_LOGIN& rMsg) 
{ 
	return m_Role.HandlePacket(rMsg); 
}
