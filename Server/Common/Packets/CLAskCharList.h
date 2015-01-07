#ifndef __CLASKCHARLIST_H__
#define __CLASKCHARLIST_H__

#include "BaseLib.h"
#include "PacketFactory.h"
#include "GameDefine.h"
#include "PacketDefine.h"


namespace Packets
{


	class CLAskCharList : public Packet 
	{
	public:
		CLAskCharList( ){} ;
		virtual ~CLAskCharList( ){} ;

		//公用继承接口
		virtual bool			Read( SocketInputStream& iStream ) ;
		virtual bool			Write( SocketOutputStream& oStream ) const ;
		virtual uint32_t			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKCHARLIST;}
		virtual uint32_t			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT;
		}

	public:
		//使用数据接口

		const	CHAR*			GetAccount()	const;
		void					SetAccount(const CHAR*	pAccount);

		PlayerID_t				GetPlayerID() const
		{
			return PlayerID;
		}

		void					SetPlayerID(PlayerID_t	 pID)
		{
			PlayerID = pID;
		}	

	private:
		//数据

		//用户名称
		CHAR					szAccount[MAX_ACCOUNT+1];
		//玩家池id，客户端不用填写
		PlayerID_t				PlayerID;
	};

	class CLAskCharListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskCharList() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKCHARLIST ; }
		uint32_t		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT;
		}
	};


	class CLAskCharListHandler 
	{
	public:
		static uint32_t Execute( CLAskCharList* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif