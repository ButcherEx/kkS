

#ifndef __THREADMANAGER_H__
#define __THREADMANAGER_H__

#include "BaseLib.h"
#include "ServerThread.h"



//除了主线程以外，其他线程都由此模块来调度
//Login 端的 ThreadManager 取消ThreadPool
//因为不需要跑场景 
class ThreadManager
{
public :
	ThreadManager( ) ;
	~ThreadManager( ) ;

	//初始化
	bool				Init( ) ;
	//启动所有线程
	bool				Start( ) ;
	//停止所有线程
	bool				Stop( ) ;
	
	//取得当前的服务器线程
	ServerThread*		GetServerThread(){ 
		return m_pServerThread ; 
	} ;
	//取得当前运行的线程总数
	uint32_t				GetTotalThreads(){ return m_nThreads ; } ;
protected :
	ServerThread*		m_pServerThread ;
	uint32_t				m_nThreads ;

};
extern ThreadManager*	g_pThreadManager ;




#endif
