

#ifndef __CONNECTMANAGER_H__
#define __CONNECTMANAGER_H__

#include "BaseLib.h"
#include "LoginPlayerManager.h"


//负责管理连接进入的客户端
class ConnectManager : public Thread
{
public :
	ConnectManager( ) ;
	~ConnectManager( ) ;

	//初始化模块
	bool			Init( ) ;
	//模块处理循环
	virtual void	run () ;
	//模块退出
	virtual void	stop( ){ m_Active = false ; } ;
	//模块退出处理
	void			Quit( ) ;

	//判断当前模块是否处于活动状态
	bool			IsActive( ){ return m_Active ; } ;
private :
	bool			m_Active ;//是否活动的标志


};

extern ConnectManager* g_pConnectManager ;

#endif
