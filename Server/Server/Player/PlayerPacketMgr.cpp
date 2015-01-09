

#include "PlayerPacketMgr.h"


//-----------------------------------------------------------------------------
// construct
//-----------------------------------------------------------------------------
PlayerNetCmdMgr::PlayerNetCmdMgr()
{
	m_mapRecvProc.Clear();
	m_mapSendProc.Clear();
}

//-----------------------------------------------------------------------------
// destruct
//-----------------------------------------------------------------------------
PlayerNetCmdMgr::~PlayerNetCmdMgr()
{
	Destroy();
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
void PlayerNetCmdMgr::Destroy()
{
}

//-----------------------------------------------------------------------------
// ��ӡ���е�������Ϣͳ����Ϣ��log
//-----------------------------------------------------------------------------
void PlayerNetCmdMgr::LogAllMsg()
{
	tagPlayerCmd* pCmd = NULL;

/*	
	m_pLog->Write(_T("\r\n\r\n"));

	// ���ȴ�ӡ���еĿͻ�����Ϣ
	m_pLog->Write(_T("Client Msg Statistics:\r\n"));

	m_mapRecvProc.ResetIterator();
	while( m_mapRecvProc.PeekNext(pCmd) )
	{
		m_pLog->Write(_T("%s\t\t%u\r\n"), m_pUtil->Unicode8ToUnicode(pCmd->strCmd.c_str()), pCmd->nTimes);
	}
	m_pLog->Write(_T("\r\n\r\n"));

	// �ٴ�ӡ�������˵���Ϣ
	m_pLog->Write(_T("Server Msg Statistics:\r\n"));

	m_mapSendProc.ResetIterator();
	while( m_mapSendProc.PeekNext(pCmd) )
	{
		m_pLog->Write(_T("%s\t\t%u\r\n"), m_pUtil->Unicode8ToUnicode(pCmd->strCmd.c_str()), pCmd->nTimes);
	}
	m_pLog->Write(_T("\r\n\r\n"));
*/
}

//-----------------------------------------------------------------------------
// ע�������Ϣ
//-----------------------------------------------------------------------------
bool PlayerNetCmdMgr::RegisterRecvProc(const CHAR* szCmd, NETMSGHANDLER fp, const CHAR* szDesc, uint32_t dwSize)
{
	uint32_t dwID = MyCRC(szCmd);

	tagPlayerCmd* pCmd = NULL;
	m_mapRecvProc.Peek(dwID, pCmd);

	if( pCmd )
	{
		if( pCmd->strCmd != szCmd )
		{
			Assert(0);	// ��������ӵ����ͬ��CRC
			return false;
		}
	}
	else
	{
		pCmd = new tagPlayerCmd;
		pCmd->nTimes = 0;
		pCmd->dwSize = dwSize;
		pCmd->handler = fp;
		pCmd->strCmd = szCmd;
		pCmd->strDesc = szDesc;
		m_mapRecvProc.Add(dwID, pCmd);
	}

	return true;
}

//------------------------------------------------------------------------------
// ע�ᷢ����Ϣ
//------------------------------------------------------------------------------
bool PlayerNetCmdMgr::RegisterSendProc(LPCSTR szCmd)
{
	uint32_t dwID = MyCRC(szCmd);

	tagPlayerCmd* pCmd = NULL;
	
	m_mapSendProc.Peek(dwID, pCmd);

	if( pCmd )
	{
		if( pCmd->strCmd != szCmd )
		{
			Assert(0);
			return false;
		}
	}
	else
	{
		pCmd = new tagPlayerCmd;
		pCmd->nTimes = 0;
		pCmd->dwSize = 0;
		pCmd->handler = NULL;
		pCmd->strCmd = szCmd;
		m_mapSendProc.Add(dwID, pCmd);
	}

	return true;
}

//------------------------------------------------------------------------------
// ȡ��ע��
//------------------------------------------------------------------------------
void PlayerNetCmdMgr::UnRegisterAll()
{
	tagPlayerCmd* pCmd = NULL;

	m_mapRecvProc.ResetIterator();
	while( m_mapRecvProc.PeekNext(pCmd) )
	{
		SAFE_DELETE(pCmd);
	}
	m_mapRecvProc.Clear();

	m_mapSendProc.ResetIterator();
	while( m_mapSendProc.PeekNext(pCmd) )
	{
		SAFE_DELETE(pCmd);
	}
	m_mapSendProc.Clear();
}

//------------------------------------------------------------------------------
// �õ�ĳ����ϢID��Ӧ�Ĵ�����
//------------------------------------------------------------------------------
NETMSGHANDLER PlayerNetCmdMgr::GetHandler(Packet* pMsg, uint32_t nMsgSize)
{
	tagPlayerCmd* pCmd = NULL;
	
	m_mapRecvProc.Peek(pMsg->GetPacketID(), pCmd);
	if( !pCmd )
	{
		//IMSG(_T("Unknow player command recved[<cmdid>%u <size>%d]\r\n"), pMsg->dwID, nMsgSize);
		return NULL;
	}
/*
	if( pMsg->dwSize != nMsgSize || nMsgSize > GT_MAX_PACKAGE_LEN || nMsgSize < pCmd->dwSize )
	{
		IMSG(_T("Invalid net command size[<cmd>%u <size>%d]\r\n"), pMsg->dwID, pMsg->dwSize);
		return NULL;
	}
*/

	InterlockedExchangeAdd((LPLONG)&pCmd->nTimes, 1);

	return pCmd->handler;
}

//------------------------------------------------------------------------------------------
// ִ����Ϣ������
//------------------------------------------------------------------------------------------
bool PlayerNetCmdMgr::HandleCmd(Packet* pMsg, uint32_t nMsgSize)
{

	NETMSGHANDLER fp = GetHandler(pMsg, nMsgSize);
	if( NULL == fp ) return false;

	fp(pMsg);

	return true;
}

//-------------------------------------------------------------------------------------------
// �������˵ķ�������
//-------------------------------------------------------------------------------------------
void PlayerNetCmdMgr::CountServerMsg(uint32_t dwMsgID)
{
	tagPlayerCmd* pCmd = NULL;
	m_mapSendProc.Peek(dwMsgID, pCmd);

	if( pCmd )
	{
		InterlockedExchangeAdd((LPLONG)&pCmd->nTimes, 1);
	}
}

//-------------------------------------------------------------------------------------------
// ȡ�ý�������ִ�д���
//-------------------------------------------------------------------------------------------
uint32_t PlayerNetCmdMgr::GetRecvCmdRunTimes( uint32_t dwMsgID )
{
	tagPlayerCmd* pCmd = NULL;
	m_mapSendProc.Peek(dwMsgID, pCmd);

	if (pCmd)
	{
		return pCmd->nTimes;
	}

	return 0;
}
