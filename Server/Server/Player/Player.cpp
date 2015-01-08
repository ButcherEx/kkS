//#include "stdafx.h"

#include "Player.h"
#include "PacketFactoryManager.h"

using namespace Packets ;

Player::Player( bool bIsServer )
:m_SocketInputStream(m_Socket) ,m_SocketOutputStream(m_Socket)
{
__ENTER_FUNCTION

	m_PID = INVALID_ID ;
	m_UID = INVALID_ID ;
	m_PlayerManagerID = INVALID_ID ;
	m_IsEmpty		= true ;
	m_IsDisconnect	= false ;
	m_PacketIndex	 = 0 ;

__LEAVE_FUNCTION
}

Player::~Player( )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

void Player::CleanUp( )
{
__ENTER_FUNCTION

	m_Socket.close() ;
	m_SocketInputStream.CleanUp() ;
	m_SocketOutputStream.CleanUp() ;
	SetPlayerManagerID( INVALID_ID ) ;
	SetUserID( INVALID_ID ) ;
	m_PacketIndex = 0 ;
	SetDisconnect(false) ;


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

	if( IsDisconnect() )
		return true ;

	_MY_TRY 
	{
		uint32_t ret = m_SocketInputStream.Fill( ) ;
		if( (int32_t)ret <= SOCKET_ERROR )
		{
			Log::SaveLog( "./Log/LoginError.log", "[%d] m_pSocketInputStream->Fill ret:%d %s", 
				g_pTimeManager->Time2DWORD(), (int32_t)ret, MySocketError() ) ;
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

	bool ret ;

	CHAR header[PACKET_HEADER_SIZE];
	PacketID_t packetID;
	uint32_t packetuint, packetSize, packetIndex;
	uint16_t packetTick;
	Packet* pPacket = NULL ;

	if( IsDisconnect( ) )
		return true ;

	_MY_TRY
	{
		if( Option ) 
		{//ִ�в���ѡ�����
		}

		for( ;; )
		{
			if( !m_SocketInputStream.Peek(&header[0], PACKET_HEADER_SIZE) )
			{//���ݲ��������Ϣͷ
				break ;
			}

			this->DecryptHead_CS(header);//���ͷ����

			memcpy( &packetID, &header[0], sizeof(PacketID_t) ) ;	
			memcpy( &packetTick, &header[sizeof(uint16_t)], sizeof(uint16_t) ) ;
			memcpy( &packetuint, &header[sizeof(uint16_t)+sizeof(PacketID_t)], sizeof(uint32_t) );
			packetSize = GET_PACKET_LEN(packetuint) ;
			packetIndex = GET_PACKET_INDEX(packetuint) ;

			if( packetID >= (PacketID_t)PACKET_MAX )
			{//��Ч����Ϣ����
				Assert( false ) ;
				return false ;
			}

			//��Ϣ���ܴ���--Begin
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
			//��Ϣ���ܴ���--End

			_MY_TRY
			{

				if( m_SocketInputStream.Length()<PACKET_HEADER_SIZE+packetSize )
				{//��Ϣû�н���ȫ
					break;
				}

				if( packetSize>g_pPacketFactoryManager->GetPacketMaxSize(packetID) )
				{//��Ϣ�Ĵ�С�����쳣���յ�����Ϣ��Ԥ������Ϣ�����ֵ��Ҫ��
					Assert( false ) ;
//					m_pSocketInputStream->Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return false ;
				}

				Packet* pPacket = g_pPacketFactoryManager->CreatePacket( packetID ) ;
				if( pPacket==NULL )
				{//���ܷ��䵽�㹻���ڴ�
					Assert( false ) ;
//					m_pSocketInputStream->Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return false ;
				}

				//������Ϣ���к�
				pPacket->SetPacketIndex( packetIndex ) ;

				ret = m_SocketInputStream.ReadPacket( pPacket ) ;
				if( ret==false )
				{//��ȡ��Ϣ���ݴ���
					Assert( false ) ;
					g_pPacketFactoryManager->RemovePacket( pPacket ) ;
					return false ;
				}

				bool bNeedRemove = true ;

				_MY_TRY
				{
					//����m_KickTime��Ϣ��m_KickTime��Ϣ�е�ֵΪ�ж��Ƿ���Ҫ�ߵ�
					//�ͻ��˵�����
					this->ResetKick( ) ;

					uint32_t uret = 0;// pPacket->Execute( this ) ;
					if( uret==PACKET_EXE_ERROR )
					{//�����쳣���󣬶Ͽ����������
						if( pPacket ) 
							g_pPacketFactoryManager->RemovePacket( pPacket ) ;
						return false ;
					}
					else if( uret==PACKET_EXE_BREAK )
					{//��ǰ��Ϣ�Ľ���ִ�н�ֹͣ
					 //ֱ���¸�ѭ��ʱ�ż����Ի����е����ݽ�����Ϣ��ʽ
					 //����ִ�С�
					 //����Ҫ���ͻ��˵�ִ�д�һ������ת�Ƶ�����һ������ʱ��
					 //��Ҫ�ڷ���ת����Ϣ��ִ���ڱ��߳���ֹͣ��
						if( pPacket ) 
							g_pPacketFactoryManager->RemovePacket( pPacket ) ;
						break ;
					}
					else if( uret==PACKET_EXE_CONTINUE )
					{//��������ʣ�µ���Ϣ
					}
					else if( uret==PACKET_EXE_NOTREMOVE )
					{//��������ʣ�µ���Ϣ�����Ҳ����յ�ǰ��Ϣ
						bNeedRemove = false ;
					}
					else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
					{
						return false ;
					}
					else
					{//δ֪�ķ���ֵ
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



	return true ;

__LEAVE_FUNCTION

	return false ;
}

bool Player::ProcessOutput( )
{
__ENTER_FUNCTION

	if( IsDisconnect( ) )
		return true ;

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
			Log::SaveLog( "./Log/LoginError.log", "[%d] m_pSocketOutputStream->Flush ret:%d %s", 
				g_pTimeManager->Time2DWORD(), (int32_t)ret, MySocketError() ) ;
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

	if( IsDisconnect( ) )
		return true ;


		pPacket->SetPacketIndex( m_PacketIndex++ ) ;

		PacketID_t packetID = pPacket->GetPacketID() ;
		uint32_t w = m_SocketOutputStream.Write( PACK_COMPART , PACK_COMPART_SIZE ) ;

		uint32_t t_uTail_Begin = m_SocketOutputStream.GetTail();//��ѯ��ǰ��βλ�á���¼д��ǰλ��

		w = m_SocketOutputStream.Write( (CHAR*)&packetID , sizeof(PacketID_t) ) ;

		uint16_t packetTick = g_pTimeManager->RunTick();
		w = m_SocketOutputStream.Write( (CHAR*)&packetTick , sizeof(uint16_t) ) ;

		uint32_t packetUINT ;
		uint32_t packetSize = pPacket->GetPacketSize( ) ;
		uint32_t packetIndex = pPacket->GetPacketIndex( ) ;

		SET_PACKET_INDEX(packetUINT, packetIndex) ;
		SET_PACKET_LEN(packetUINT, packetSize) ;

		w = m_SocketOutputStream.Write( (CHAR*)&packetUINT, sizeof(uint32_t) ) ;

		bool ret = pPacket->Write( m_SocketOutputStream ) ;

		uint32_t t_uTail_End = m_SocketOutputStream.GetTail();//��ѯ��ǰ��βλ�á���¼д����λ��

		//��Ϣ���ܴ���--Begin
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
		//��Ϣ���ܴ���--End

		//BOOL ret = m_pSocketOutputStream->WritePacket( pPacket ) ;
		Assert( ret ) ;
		//Log::SaveLog( "./Log/LoginDebug.log", "SendPacket! SOCKET=%d, ID=%d", 
		//	m_pSocket->getSOCKET(), pPacket->GetPacketID() ) ;
		


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

void Player::ResetKick( )
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}
//INT Player::WhereThisPlayerFrom(void) const
//{
//	if(NULL!=m_pSocket)
//	{
//		return g_Config.m_InternalIpofProxy.WhereThisIpFrom(m_pSocket->m_Host);
//	}
//	return INVALID_ISP;
//}
//CHAR* ProxyTool_GetCorrespondingIP(Player const* pPlayer, ID_t ServerID)
//{
//	__ENTER_FUNCTION
//	_SERVER_DATA* pData = g_pServerManager->FindServerInfo( ServerID );
//	if(NULL==pData)
//	{
//		CHAR szLog[1024];
//		tsnprintf(szLog, sizeof(szLog), "[ProxyTool_GetCorrespondingIP] Error: Can't found the specific server(%d).", ServerID);
//		szLog[sizeof(szLog)-1] = '\0';
//		AssertEx(pData, szLog);
//	}
//	INT nIsp = pPlayer->WhereThisPlayerFrom();
//	if(INVALID_ISP==nIsp)
//	{
//		return pData->m_IP0;
//	}
//	else
//	{
//		if(TRUE==RangeCheckForIndex_Assert(nIsp, 0, NUM_OF_ISP-1, "ProxyTool_GetCorrespondingIP"))
//		{
//			PROXY_DATA& rProxy = pData->m_aProxy[nIsp];
//			if(TRUE==rProxy.m_bEnabled)
//			{
//				return rProxy.m_szIP;
//			}
//			return pData->m_IP0;
//		}
//		else
//		{
//			return pData->m_IP0;
//		}
//	}
//	__LEAVE_FUNCTION
//	return NULL;
//}
//uint32_t  ProxyTool_GetCorrespondingPort(Player const* pPlayer, ID_t ServerID)
//{
//	__ENTER_FUNCTION
//	_SERVER_DATA* pData = g_pServerManager->FindServerInfo( ServerID );
//	if(NULL==pData)
//	{
//		CHAR szLog[1024];
//		tsnprintf(szLog, sizeof(szLog), "[ProxyTool_GetCorrespondingPort] Error: Can't found the specific server(%d).", ServerID);
//		szLog[sizeof(szLog)-1] = '\0';
//		AssertEx(pData, szLog);
//	}
//	INT nIsp = pPlayer->WhereThisPlayerFrom();
//	if(INVALID_ISP==nIsp)
//	{
//		return pData->m_Port0;
//	}
//	else
//	{
//		if(TRUE==RangeCheckForIndex_Assert(nIsp, 0, NUM_OF_ISP-1, "ProxyTool_GetCorrespondingIP"))
//		{
//			PROXY_DATA& rProxy = pData->m_aProxy[nIsp];
//			if(TRUE==rProxy.m_bEnabled)
//			{
//				return rProxy.m_nPort;
//			}
//			return pData->m_Port0;
//		}
//		else
//		{
//			return pData->m_Port0;
//		}
//	}
//	__LEAVE_FUNCTION
//	return NULL;
//}
