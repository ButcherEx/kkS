#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "BaseLib.h"

class Login
{
public:
	Login();
	~Login();

public:
	//Login 服务器初始化
	bool		Init();

	//Login 服务器主循环
	bool		Loop();

	//Login 服务器退出
	bool		Exit();


	//停止模块执行，当调用此接口后，会将所有拥有线程的模块设置为不活动状态
	//当各个线程的循环调用下一此循环操作后就会退出
	void		Stop( ) ;

protected:
	//分配相关数据空间
	bool		NewLogin();
	//初始化相关数据空间
	bool		InitLogin();
	bool		m_bExited;
};


extern Login	g_Login;

class LoginExceptionHandler
{
public:
	LoginExceptionHandler();
	//void INTHandler(INT);
	//void TERMHandler(INT);
	//void ABORTHandler(INT);
	//void ILLHandler(INT);
	//void FPEHandler(INT);
	//void SEGHandler(INT);
	//void XFSZHandler(INT);
};

extern LoginExceptionHandler g_LoginExceptionHandler;


#endif