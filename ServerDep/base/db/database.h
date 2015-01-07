/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __DATABASE__
#define __DATABASE__

/**	
** Database�� - �ṩ���ݿ�����࣬�������ӵĽ�����ά�������ֲ�ѯ�����²������Ƶȵ�
** 1. Database���������ݿ�һһ��Ӧ������ϲ�Ӧ�ó�����Ҫ����⣬Ҫ�������Database����
** 2. ���ӳػ��ƣ�������̵߳Ĳ�������
** 3. �ṩ�����ݿ����Ӳ����Լ���ǰ���ݿ�״̬��Ϣ����ȡ
** 4. �ṩ���ֲ�ѯ��������ʽ��������ʽ�������ص�ʽ�������޷��ز���
** 5. �ṩ�򵥵���������ƣ��պ��������������֧��
*/

class DataBase : public MyNonCopyable
{
public:
	typedef enum
	{
		EDBE_System,		// mysql����������
		EDBE_QueueFull,		// ���������ݹ��࣬�鿴���ݿ��Ƿ�����
		EDBE_PoolFull,		// �ײ�Stream�ط�����ڴ������
	} EDBError;

public:
	DataBase();
	~DataBase();
public:
	VOID setWarningCallBack(WARNINGCALLBACK *fnWarning) { m_fnWarning = *fnWarning; }
public:

	BOOL initialize(CONST  CHAR *strHost, CONST  CHAR *strUser, CONST  CHAR *strPassword, CONST  CHAR *strDatabase, INT nPort, INT nConNum=2, CONST CHAR *fName = NULL);
	VOID finalize();

	//---------------------------------------------------------------------------
	// �������
	//---------------------------------------------------------------------------
	Connection* getFreeConnection();
	VOID returnConnection(Connection* con);

	//---------------------------------------------------------------------------
	// �鿴���Ӷ�ʧ������
	//---------------------------------------------------------------------------
	BOOL reconnect();
	BOOL isConnLost()		{ return m_bConLost; }


	//---------------------------------------------------------------------------
	// Stream���
	//---------------------------------------------------------------------------
	MySqlStream* getStream() { return m_StreamPool.get(); }
	VOID returnStream(MySqlStream* pStream) { m_StreamPool.add(pStream); }

	//----------------------------------------------------------------------------
	// ��ѯ���
	//----------------------------------------------------------------------------
	QueryResult* query(CONST  CHAR* szStr);
	QueryResult* query(CONST  MySqlStream* pStream);
	QueryResult* waitQuery(CONST  CHAR* szStr, Connection* con);
	QueryResult* waitQuery(CONST  MySqlStream* pStream, Connection* con);

	//-----------------------------------------------------------------------------
	// �������
	//-----------------------------------------------------------------------------
	BOOL execute(CONST  CHAR* szStr);
	BOOL execute(CONST  MySqlStream* pStream);
	BOOL waitExecute(CONST  CHAR* szStr, Connection* con);
	BOOL waitExecute(CONST  MySqlStream* pStream, Connection* con);

	//-----------------------------------------------------------------------------
	// ���������
	//-----------------------------------------------------------------------------
	INT checkExecute(CONST  CHAR* szStr);
	INT checkExecute(CONST  MySqlStream* pStream);
	INT checkWaitExecute(CONST  CHAR* szStr, Connection* con);
	INT checkWaitExecute(CONST  MySqlStream* pStream, Connection* con);

	//------------------------------------------------------------------------------
	// ��¼�����
	//------------------------------------------------------------------------------
	VOID freeQueryResult(QueryResult* pRet) { SAFE_DEL(pRet); }

	//------------------------------------------------------------------------------
	// �첽�������
	//------------------------------------------------------------------------------
	VOID addAsynUpdate(MySqlStream* pStream) { m_AsynStreamQueue.add(pStream); }

	//------------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------------
	BOOL beginTransaction(Connection* con);
	BOOL endTransaction(Connection* con);
	BOOL rollback(Connection* con);

	//-------------------------------------------------------------------------------
	// ϵͳ��Ϣ���
	//-------------------------------------------------------------------------------
	CONST  INT getPort()			CONST  { return m_nPort; }
	CONST  CHAR *getHostName()		CONST  { return m_Hostname; }
	CONST  CHAR *getUserName()		CONST  { return m_Username; }
	CONST  CHAR *getPassword()		CONST  { return m_Password; }
	CONST  CHAR *getDatabaseName()	CONST  { return m_DatabaseName; }

	//-------------------------------------------------------------------------------
	// ����ʱ��Ϣ���
	//-------------------------------------------------------------------------------
 	INT getFreeStreamSize() { return m_StreamPool.getFreeSize(); }
 	INT getAllStreamSize()	{ return m_StreamPool.getAllocSize( ); }
	INT getUnhandledAsynStreamNum() { return m_AsynStreamQueue.size(); }
private:
	VOID setParam(CONST  CHAR* szHost, CONST  CHAR* szUser, CONST  CHAR* szPassword, CONST  CHAR* szDatabase, INT nPort, INT nConNum = 2);
private:
	BOOL start();
	VOID setLogFileName(CONST CHAR *fName);
private:
	BOOL handleError(Connection* con, DWORD dwErrorCode, CONST  CHAR* szSql);
	BOOL reconnect(Connection* con);

	BOOL sendQuery(Connection* con, CONST  CHAR* szSql, INT nLen, BOOL bSelf=FALSE);
	QueryResult* storeQueryResult(Connection* con);

private:
	ThreadRetType ThreadAsynQuery();
	ThreadRetType ThreadCheck();
private:
	CHAR m_Hostname[DB_HOST_LEN];				// mysql����������
	CHAR m_Username[DB_USER_LEN];				// �û���
	CHAR m_Password[DB_PASS_LEN];				// ����
	CHAR m_DatabaseName[DB_DATABASE_LEN];			// ���ݿ���
	INT m_nPort;				// mysql�������˿ں�

	WARNINGCALLBACK *m_fnWarning;			// ֪ͨ�ϲ㱨���Ļص�����

	FILE *m_LogFile;
	CHAR m_LogFileName[MAX_PATH];

	Connection* m_Con;					// ���ӳ�
	INT m_nConNum;				// ���ӳص���������
	BOOL m_bConLost;				// ���Ӷ�ʧ

	MyObjPool<MySqlStream> m_StreamPool;			// �յ�sql����
	MyStreamTSList m_AsynStreamQueue;		// �������Ĳ���������

	ThreadHandle m_hThreadAsynQuery;		// �첽�����߳�
	ThreadHandle m_hThreadCheck;			// �������ʱϵͳ��Ϣ

	VOLATILE BOOL				m_TerminateAsynQuery;	// �첽�����߳̽�����ʶλ
	VOLATILE BOOL				m_TerminateCheck;		// ����߳̽�����ʶλ
};


#endif /* __DATABASE__ */

