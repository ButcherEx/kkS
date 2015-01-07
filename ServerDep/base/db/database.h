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
** Database类 - 提供数据库操作类，包括连接的建立及维护，各种查询及更新操作机制等等
** 1. Database对象与数据库一一对应，如果上层应用程序需要多个库，要建立多个Database对象
** 2. 连接池机制，方便多线程的并发操作
** 3. 提供对数据库连接参数以及当前数据库状态信息的提取
** 4. 提供三种查询及操作方式——阻塞式操作，回调式操作及无返回操作
** 5. 提供简单的事务处理机制，日后添加完整事务处理支持
*/

class DataBase : public MyNonCopyable
{
public:
	typedef enum
	{
		EDBE_System,		// mysql服务器出错
		EDBE_QueueFull,		// 队列中内容过多，查看数据库是否阻塞
		EDBE_PoolFull,		// 底层Stream池分配的内存过多了
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
	// 连接相关
	//---------------------------------------------------------------------------
	Connection* getFreeConnection();
	VOID returnConnection(Connection* con);

	//---------------------------------------------------------------------------
	// 查看连接丢失及重连
	//---------------------------------------------------------------------------
	BOOL reconnect();
	BOOL isConnLost()		{ return m_bConLost; }


	//---------------------------------------------------------------------------
	// Stream相关
	//---------------------------------------------------------------------------
	MySqlStream* getStream() { return m_StreamPool.get(); }
	VOID returnStream(MySqlStream* pStream) { m_StreamPool.add(pStream); }

	//----------------------------------------------------------------------------
	// 查询相关
	//----------------------------------------------------------------------------
	QueryResult* query(CONST  CHAR* szStr);
	QueryResult* query(CONST  MySqlStream* pStream);
	QueryResult* waitQuery(CONST  CHAR* szStr, Connection* con);
	QueryResult* waitQuery(CONST  MySqlStream* pStream, Connection* con);

	//-----------------------------------------------------------------------------
	// 操作相关
	//-----------------------------------------------------------------------------
	BOOL execute(CONST  CHAR* szStr);
	BOOL execute(CONST  MySqlStream* pStream);
	BOOL waitExecute(CONST  CHAR* szStr, Connection* con);
	BOOL waitExecute(CONST  MySqlStream* pStream, Connection* con);

	//-----------------------------------------------------------------------------
	// 检测操作相关
	//-----------------------------------------------------------------------------
	INT checkExecute(CONST  CHAR* szStr);
	INT checkExecute(CONST  MySqlStream* pStream);
	INT checkWaitExecute(CONST  CHAR* szStr, Connection* con);
	INT checkWaitExecute(CONST  MySqlStream* pStream, Connection* con);

	//------------------------------------------------------------------------------
	// 记录集相关
	//------------------------------------------------------------------------------
	VOID freeQueryResult(QueryResult* pRet) { SAFE_DEL(pRet); }

	//------------------------------------------------------------------------------
	// 异步操作相关
	//------------------------------------------------------------------------------
	VOID addAsynUpdate(MySqlStream* pStream) { m_AsynStreamQueue.add(pStream); }

	//------------------------------------------------------------------------------
	// 事物相关
	//------------------------------------------------------------------------------
	BOOL beginTransaction(Connection* con);
	BOOL endTransaction(Connection* con);
	BOOL rollback(Connection* con);

	//-------------------------------------------------------------------------------
	// 系统信息相关
	//-------------------------------------------------------------------------------
	CONST  INT getPort()			CONST  { return m_nPort; }
	CONST  CHAR *getHostName()		CONST  { return m_Hostname; }
	CONST  CHAR *getUserName()		CONST  { return m_Username; }
	CONST  CHAR *getPassword()		CONST  { return m_Password; }
	CONST  CHAR *getDatabaseName()	CONST  { return m_DatabaseName; }

	//-------------------------------------------------------------------------------
	// 运行时信息相关
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
	CHAR m_Hostname[DB_HOST_LEN];				// mysql服务器主机
	CHAR m_Username[DB_USER_LEN];				// 用户名
	CHAR m_Password[DB_PASS_LEN];				// 密码
	CHAR m_DatabaseName[DB_DATABASE_LEN];			// 数据库名
	INT m_nPort;				// mysql服务器端口号

	WARNINGCALLBACK *m_fnWarning;			// 通知上层报警的回调函数

	FILE *m_LogFile;
	CHAR m_LogFileName[MAX_PATH];

	Connection* m_Con;					// 连接池
	INT m_nConNum;				// 连接池的连接数量
	BOOL m_bConLost;				// 连接丢失

	MyObjPool<MySqlStream> m_StreamPool;			// 空的sql语句池
	MyStreamTSList m_AsynStreamQueue;		// 非阻塞的操作语句队列

	ThreadHandle m_hThreadAsynQuery;		// 异步操作线程
	ThreadHandle m_hThreadCheck;			// 检测运行时系统信息

	VOLATILE BOOL				m_TerminateAsynQuery;	// 异步操作线程结束标识位
	VOLATILE BOOL				m_TerminateCheck;		// 检测线程结束标识位
};


#endif /* __DATABASE__ */

