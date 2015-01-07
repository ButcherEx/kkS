

#ifndef __LOGINPLAYER_H__
#define __LOGINPLAYER_H__

#include "Player.h"

class LoginPlayer : public Player
{
public :
	LoginPlayer( ) ;
	~LoginPlayer( ) ;

	//消息执行接口
	virtual bool		ProcessCommand( bool Option = true ) ;
	//数据接收接口
	virtual bool		ProcessInput( ) ;
	//数据发送接口
	virtual bool		ProcessOutput( ) ;

//心跳接口，处理所有逻辑
	virtual bool		HeartBeat( uint32_t uTime=0 ) ;

	//清除数据
	virtual void		CleanUp( ) ;


public :
	//应用接口
	virtual bool		IsLoginPlayer( ){ return true; } ;
	virtual bool		IsServerPlayer( ){ return false ; } ;

	//连接成功后初始化基本数据
	void				Init( ) ;

	//向此Player发送一个消息包
	//此接口只能在本执行线程内处理（无数据同步能力）
	virtual bool		SendPacket( Packet* pPacket ) ;

	virtual void		Encrypt_SC(CHAR* header, uint32_t uLen, uint32_t uBeginPlace){ENCRYPT(header, uLen, LOGIN_TO_CLIENT_KEY, uBeginPlace)}

	virtual void		DecryptHead_CS(CHAR* header){ENCRYPT_HEAD(header, CLIENT_TO_LOGIN_KEY)}

	virtual void		Decrypt_CS(CHAR* header, uint32_t uLen, uint32_t uBeginPlace){ENCRYPT(header, uLen, CLIENT_TO_LOGIN_KEY, uBeginPlace)}

	//玩家状态设置、读取接口
	void				SetPlayerStatus( uint32_t status ){ m_Status = status ; } ;
	uint32_t				GetPlayerStatus( ) { return m_Status ; } ;
	
	//断开网络连接，并且回收Player数据
	//当数据被回收后可能马上会被PlayerPool分配出去使用
	bool				FreeOwn( ) ;
	virtual void		ResetKick( ) ;
	virtual void		Disconnect( ) ;
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
private :
	uint32_t					m_Status ;
	GUID_t					m_AccountGuid;				//账号guid 由billingsystem 返回，通过验证以后

public :
	//游戏数据
	uint32_t					m_KickTime ;		//判断是否需要踢掉玩家的计时器
	uint32_t					m_LastSendTime ;	//上次发送数据的时间
	uint32_t					m_ConnectTime;
	uint32_t					m_CurrentTime ;		//当前逻辑时间
	int32_t						m_LeftTimeToQuit ;	//剩余被清除退出的时间
	bool					m_Dirty ;			//此标志表示当前连接已经无效，
												//不需要发送任何状态消息给世界数据服务器
};





#endif
