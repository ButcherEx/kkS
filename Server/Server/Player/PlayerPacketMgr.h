#ifndef __PLAYER_PACKET_MGR_H__
#define __PLAYER_PACKET_MGR_H__

#include "BaseLib.h"

class Packet;
class Player;

typedef fastdelegate::FastDelegate1<Packet*, uint32_t> NETMSGHANDLER;

//-----------------------------------------------------------------------------
// 客户端命令管理器
//-----------------------------------------------------------------------------
class PlayerNetCmdMgr
{
public:
	PlayerNetCmdMgr();
	~PlayerNetCmdMgr();

	void Init();
	void Destroy();
	void LogAllMsg();

	bool RegisterRecvProc(const CHAR* szCmd, NETMSGHANDLER fp, const CHAR* szDesc, uint32_t dwSize);
	bool RegisterSendProc(LPCSTR szCmd);
	void UnRegisterAll();

	NETMSGHANDLER GetHandler(Packet* pMsg, uint32_t nMsgSize);
	void CountServerMsg(uint32_t dwMsgID);

	bool HandleCmd(Packet* pMsg, uint32_t nMsgSize);

	// 取得消息执行次数
	uint32_t GetRecvCmdRunTimes(uint32_t dwMsgID);
protected:
	typedef struct tagPlayerCmd
	{
		bstd::string			strCmd;			// 命令名
		bstd::string			strDesc;		// 描述
		uint32_t				dwSize;			// 消息大小
		NETMSGHANDLER			handler;		// 函数指针
		volatile uint32_t		nTimes;			// 此命令的次数
	} tagPlayerCmd;

	TMap<uint32_t, tagPlayerCmd*>	m_mapRecvProc;	// 接收消息的处理及统计
	TMap<uint32_t, tagPlayerCmd*>	m_mapSendProc;	// 发送消息的处理及统计
};

#endif

