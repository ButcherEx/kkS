

#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include "BaseLib.h"

//用来处理服务器之间数据通讯的线程

class ServerThread : public Thread
{
public :
	ServerThread( ) ;
	~ServerThread( ) ;

	//主循环
	virtual void	run( ) ;

	//停止线程执行
	virtual void	stop( ) { m_Active = false ; } ;

	//判断当前线程是否激活
	bool			IsActive( ){ return m_Active ; } ;



private :
	bool		m_Active ;


};





#endif
