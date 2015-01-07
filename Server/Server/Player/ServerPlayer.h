


#ifndef __SERVERPLAYER_H__
#define __SERVERPLAYER_H__

#include "Base.h"
#include "Player.h"
#include "Config.h"


//服务器玩家指的是另外的服务器端程序在当前程序内的数据表现

class ServerPlayer : public Player
{
public :
	ServerPlayer( bool bIsServer=true ) ;
	~ServerPlayer( ) ;

	//数据接收
	virtual bool	ProcessInput( ) ;
	//数据发送
	virtual bool	ProcessOutput( ) ;
	//消息处理
	virtual bool	ProcessCommand( bool Option = true ) ;

	//清除
	virtual void	CleanUp( ) ;

	//逻辑接口
	virtual bool	HeartBeat( uint32_t uTime=0 ) ;


public :
	//通用接口

	virtual bool	IsLoginPlayer( ){ return false ; } ;
	virtual bool	IsServerPlayer( ){ return true ; } ;

	//设置当前服务器端玩家的状态
	void			SetPlayerStatus( uint32_t status ){ m_Status = status ; } ;

	//判断当前服务器端玩家是否有效
	virtual bool	IsValid( ) ;

	//向此服务器端程序发送消息
	//此接口不支持线程间同步，因此只能有当前线程来调用执行
	virtual bool	SendPacket( Packet* pPacket ) ;

	virtual void	Encrypt_SC(CHAR* header, uint32_t uLen, uint32_t uBeginPlace){;}

	virtual void	DecryptHead_CS(CHAR* header){;}

	virtual void	Decrypt_CS(CHAR* header, uint32_t uLen, uint32_t uBeginPlace){;}
private :
	//服务器端状态
	uint32_t			m_Status ;


};




#endif
