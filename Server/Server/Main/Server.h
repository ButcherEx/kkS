/************************************************************************/
/*                                                                      */
/************************************************************************/
#ifndef __SERVER_H__
#define __SERVER_H__

#include "BaseLib.h"
#include "Player.h"
#include "Role/Role.h"
#include "InstanceModule.h"
#include "LogDefine.h"
#include "EventMsg.h"
#include "EventMgr.h"
#include "Task.h"
#include "EventMsg_Test.h"

class Server
{
public:
	Server();
	~Server();
public:
	bool Init();
	void Loop();
	void Exit();
private:
	void Init_AllTask();
private:
	TaskManager m_MainTaskManager;
};

extern Server g_Server;
#endif