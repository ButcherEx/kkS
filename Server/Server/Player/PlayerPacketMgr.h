#ifndef __PLAYER_PACKET_MGR_H__
#define __PLAYER_PACKET_MGR_H__

#include "BaseLib.h"

class Packet;
class Player;

typedef fastdelegate::FastDelegate1<Packet*, uint32_t> NETMSGHANDLER;

//-----------------------------------------------------------------------------
// �ͻ������������
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

	// ȡ����Ϣִ�д���
	uint32_t GetRecvCmdRunTimes(uint32_t dwMsgID);
protected:
	typedef struct tagPlayerCmd
	{
		bstd::string			strCmd;			// ������
		bstd::string			strDesc;		// ����
		uint32_t				dwSize;			// ��Ϣ��С
		NETMSGHANDLER			handler;		// ����ָ��
		volatile uint32_t		nTimes;			// ������Ĵ���
	} tagPlayerCmd;

	TMap<uint32_t, tagPlayerCmd*>	m_mapRecvProc;	// ������Ϣ�Ĵ���ͳ��
	TMap<uint32_t, tagPlayerCmd*>	m_mapSendProc;	// ������Ϣ�Ĵ���ͳ��
};

#endif

