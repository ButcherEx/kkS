/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#include <core/sfcore.h>
#include <impl/sfimpl.h>
#include <db/dbdef.h>
#include <mysql.h>
#include <db/connection.h>
#include <db/field.h>
#include <db/queryresult.h>
#include <db/mysqlstream.h>
#include <db/streamtslist.h>
#include <db/database.h>

DataBase::DataBase() 
: m_nPort(0)
, m_fnWarning(NULL)
, m_nConNum(0)
, m_Con(0)
, m_bConLost(TRUE)
, m_hThreadAsynQuery(0)
, m_hThreadCheck(0)
, m_TerminateAsynQuery(FALSE)
, m_TerminateCheck(FALSE)
{
	Mybzero(m_Hostname, sizeof(m_Hostname));
	Mybzero(m_Username, sizeof(m_Username));
	Mybzero(m_Password, sizeof(m_Password));
	Mybzero(m_DatabaseName, sizeof(m_DatabaseName));
	Mybzero(m_LogFileName, sizeof(m_LogFileName));
}

DataBase::~DataBase()
{
	m_StreamPool.finalize( );
	finalize();
}


VOID DataBase::setParam(CONST  CHAR* szHost, 
						CONST  CHAR* szUser, 
						CONST  CHAR* szPassword, 
						CONST  CHAR* szDatabase, 
						INT nPort, INT nConNum)
{
	strncpy(m_Hostname, szHost, DB_HOST_LEN);
	strncpy(m_Username, szUser, DB_USER_LEN);
	strncpy(m_Password, szPassword, DB_PASS_LEN);
	strncpy(m_DatabaseName, szDatabase, DB_DATABASE_LEN);
	m_nPort = nPort;
	m_nConNum = nConNum;
}


BOOL DataBase::initialize(CONST  CHAR *strHost, 
					CONST  CHAR *strUser, 
					CONST  CHAR *strPassword, 
					CONST  CHAR *strDatabase, 
					INT nPort, INT nConNum/*=2*/,
					CONST CHAR *fName)
{
	if( nConNum <= 1 ) return FALSE;
	
	m_StreamPool.initialize( );
	setParam(strHost, strUser, 
		strPassword, strDatabase, nPort, nConNum);
	setLogFileName(fName);
	return start();
}

VOID DataBase::setLogFileName(CONST CHAR *fName)
{
#if defined(__WINDOWS__) && defined(_UNICODE)
	CHAR path[MAX_PATH] = {0};
	MyUnicodeToAnsi(path,MAX_PATH,__GLOGPATH__);
#else
	CONST CHAR *path = __GLOGPATH__;
#endif
	sprintf(m_LogFileName, "%s%s%s.(%d).%d.err", 
		path, m_DatabaseName,  fName ? fName : "", MyGetProcessID( ), rand( ));
}

BOOL DataBase::reconnect()
{
	finalize();	 
	return start(); 
}

BOOL DataBase::start()
{
	m_LogFile = fopen(m_LogFileName, "ab+");
	if( !m_LogFile ){
		LogEx(stdout, LOG_LEVEL_ERROR,
			_T("Create file %s failed.%s.\n"),
			m_LogFileName, Mystrerror(errno));
		return FALSE;
	}

	// 建立连接
	m_Con = new Connection[m_nConNum];

	for( INT n = 0; n < m_nConNum; n++ ) {

		MYSQL* temp = mysql_init(NULL);
		if( mysql_options(temp, 
				MYSQL_SET_CHARSET_NAME, 
				"utf8") )
		{
			fprintf(m_LogFile, "mysql_options:%s.\n", mysql_error(temp));
			return FALSE;
		}

		my_bool my_true = true;
		if( mysql_options(temp, 
				MYSQL_OPT_RECONNECT, 
				&my_true) )
		{
			
			fprintf(m_LogFile, "mysql_options(MYSQL_OPT_RECONNECT):%s.\n", mysql_error(temp));
			return FALSE;
		}

		if( !mysql_real_connect(temp, 
					m_Hostname, 
					m_Username, 
					m_Password, 
					m_DatabaseName, 
					m_nPort, 
					NULL, 
					0) )
		{
			fprintf(m_LogFile, "mysql_real_connect:%s.\n", mysql_error(temp));
			return FALSE;
		}

		m_Con[n].setMySql( temp );
	}


	m_TerminateAsynQuery = FALSE;

	INT nRet = MyCreateThreadEx0(this, &DataBase::ThreadAsynQuery);
	if( 0 == nRet || -1 == nRet ){
		return FALSE;
	}

	m_hThreadAsynQuery = (ThreadHandle)nRet;

	m_TerminateCheck = FALSE;
	nRet = MyCreateThreadEx0(this, &DataBase::ThreadCheck);
	if( 0 == nRet || -1 == nRet ) return FALSE;
	m_hThreadCheck = (ThreadHandle)nRet;


	m_bConLost = FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------------------------------------------
// 关闭
//---------------------------------------------------------------------------------------------------------------
VOID DataBase::finalize()
{
	m_TerminateAsynQuery = TRUE;
	m_TerminateCheck = TRUE;

	MyThreadJoin(m_hThreadAsynQuery);
	MyThreadJoin(m_hThreadCheck);
	MyThreadClose(m_hThreadAsynQuery);
	MyThreadClose(m_hThreadCheck);

	// 关闭所有连接
	if( m_Con ){
		for(INT n = 0; n < m_nConNum; n++) {
			if( m_Con[n].getMySql( ) ) {
				mysql_close(m_Con[n].getMySql( ));
			}
		}
		SAFE_DEL_ARRAY(m_Con);
	}

	// 关闭log
	if( m_LogFile ){
		fclose(m_LogFile);
	}
}


Connection* DataBase::getFreeConnection()
{
	DWORD n = 0;
	while(TRUE) {
		Connection* con = &m_Con[((n++) % m_nConNum)];
		if( LOCK_TRY_REQUIRE( con->getLock( ) ) )
			return con;
	}

	return NULL;
}


VOID DataBase::returnConnection(Connection* con)
{
	if( con ){
		LOCK_RELEASE( con->getLock( ) );
	}
}


QueryResult* DataBase::query(CONST  CHAR* szStr)
{
	QueryResult* pRet = NULL;

	if( szStr ) {
		Connection* con = getFreeConnection();
		if( sendQuery(con, szStr, (INT)strlen(szStr)) ) {
			pRet = storeQueryResult(con);
		}

		returnConnection(con);
	}

	return pRet;
}

QueryResult* DataBase::query(CONST  MySqlStream* pStream)
{
	QueryResult* pRet = NULL;
	if( pStream && pStream->getBuf() && pStream->getBufLen() ){
		// 任意获得一条连接
		Connection* con = getFreeConnection();
		if( sendQuery(con, pStream->getBuf(), pStream->getBufLen( ) ) ) {
			pRet = storeQueryResult(con);
		}

		returnConnection(con);
	}
	return pRet;
}

QueryResult* DataBase::waitQuery(CONST  CHAR* szStr, Connection* con)
{
	QueryResult* pRet = NULL;
	if( szStr && con ){
		if( sendQuery(con, szStr, (INT)strlen(szStr)) ){
			pRet = storeQueryResult(con);
		}
	}
	return pRet;
}

QueryResult* DataBase::waitQuery(CONST  MySqlStream* pStream, Connection* con)
{
	QueryResult* pRet = NULL;
	if( pStream && pStream->getBuf() && pStream->getBufLen() && con ){
		if( sendQuery(con, pStream->getBuf(), pStream->getBufLen()) ){
			pRet = storeQueryResult(con);
		}
	}
	return pRet;
}


BOOL DataBase::execute(CONST  CHAR* szStr)
{
	BOOL bRet = FALSE;

	if( szStr ) {
		// 任意获得一条连接
		Connection* con = getFreeConnection();
		bRet = sendQuery(con, szStr, (INT)strlen(szStr));
		returnConnection(con);
	}

	return bRet;
}

BOOL DataBase::execute(CONST  MySqlStream* pStream)
{
	BOOL bRet = FALSE;

	if( pStream && pStream->getBuf()&& pStream->getBufLen() ){
		// 任意获得一条连接
		Connection* con = getFreeConnection();
		BOOL bRet = sendQuery(con, pStream->getBuf(), pStream->getBufLen());
		returnConnection(con);
	}

	return bRet;
}

BOOL DataBase::waitExecute(CONST  CHAR* szStr, Connection* con)
{
	BOOL bRet = FALSE;

	if( szStr && con ) {
		bRet = sendQuery(con, szStr, (INT)strlen(szStr));
	}

	return bRet;

}

BOOL DataBase::waitExecute(CONST  MySqlStream* pStream, Connection* con)
{
	BOOL bRet = FALSE;

	if( pStream && pStream->getBuf() && pStream->getBufLen() && con ){
		bRet = sendQuery(con, pStream->getBuf(), pStream->getBufLen());
	}

	return bRet;
}


INT DataBase::checkExecute(CONST  CHAR* szStr)
{
	INT retval = -1;
	if( szStr ){
		// 任意获得一条连接
		Connection* con = getFreeConnection();
		if( sendQuery(con, szStr, (INT)strlen(szStr)) ){
			retval = (INT)mysql_affected_rows(con->getMySql( ));
		}
		returnConnection(con);
	}

	return retval;
}

INT DataBase::checkExecute(CONST  MySqlStream* pStream)
{
	INT retval = -1;
	if( pStream && pStream->getBuf() && pStream->getBufLen() ){
		// 任意获得一条连接
		Connection* con = getFreeConnection();
		if( sendQuery(con, pStream->getBuf(), pStream->getBufLen()) ){
			retval = (INT)mysql_affected_rows(con->getMySql( )); 
		}
		returnConnection(con);
	}

	return retval;
}

INT DataBase::checkWaitExecute(CONST  CHAR* szStr, Connection* con)
{
	BOOL bRet = FALSE;

	if( szStr && con ) {
		bRet = sendQuery(con, szStr, (INT)strlen(szStr));
	}

	return (bRet ? (INT)mysql_affected_rows(con->getMySql( )) : -1);
}

INT DataBase::checkWaitExecute(CONST  MySqlStream *pStream, Connection* con)
{
	BOOL bRet = FALSE;
	
	if( pStream && pStream->getBuf() && pStream->getBufLen() && con ){
		bRet = sendQuery(con, pStream->getBuf(), pStream->getBufLen());
	}

	return (bRet ? (INT)mysql_affected_rows(con->getMySql( )) : -1);
}

/**/
BOOL DataBase::beginTransaction(Connection* con)
{
	return waitExecute("START TRANSACTION", con);
}

BOOL DataBase::endTransaction(Connection* con)
{
	return waitExecute("COMMIT", con);
}

BOOL DataBase::rollback(Connection* con)
{
	return waitExecute("ROLLBACK", con);
}


BOOL DataBase::sendQuery(Connection* con, CONST  CHAR* szSql, INT nLen, BOOL bSelf/* =FALSE */)
{
	INT nResult = mysql_real_query(con->getMySql( ), szSql, nLen);
	if( nResult != 0 && bSelf == FALSE ) {
		if( handleError(con, mysql_errno(con->getMySql( )), szSql) ) {
			// 如果因为连接丢失
			return sendQuery(con, szSql, nLen, FALSE);
		} else {
			return sendQuery(con, szSql, nLen, TRUE);
		}
	}

	return (nResult == 0 ? TRUE : FALSE);
}


QueryResult* DataBase::storeQueryResult(Connection* con)
{
	QueryResult* pResult = NULL;

	if( con ){
		MYSQL_RES* pRes = mysql_store_result(con->getMySql( ));
		INT nRows = (INT)mysql_affected_rows(con->getMySql( ));
		INT nFields = (INT)mysql_field_count(con->getMySql( ));

		if( 0 == nFields || NULL == pRes ){
			if( pRes != NULL ) mysql_free_result(pRes);
		} else {
			pResult = new QueryResult(pRes, nRows, nFields);
			pResult->nextRow();
		}
	}

	return pResult;
}

// 处理sql语句错误
BOOL DataBase::handleError(Connection* con, DWORD dwErrorCode, CONST  CHAR* szSql)
{
	if( !con ) return FALSE;
	if( m_LogFile ){
		fprintf(m_LogFile, "ErrorCode: %u.\n", dwErrorCode);
		fprintf(m_LogFile, "ErrorDesc: %s.\n", mysql_error(con->getMySql( )));
		fprintf(m_LogFile, "SqlStream: %s.\n\n", szSql);
	}


	switch(dwErrorCode) {
	case 2006:  // Mysql server has gone away
	case 2008:  // Client ran out of memory
	case 2013:  // Lost connection to sql server during query
	case 2055:{  // Lost connection to sql server - system error
			return reconnect(con);
		}
		break;
	}

	return FALSE;
}

// 重连机制
BOOL DataBase::reconnect(Connection* con)
{
	if( !con ) return FALSE;

	my_bool my_true = true;
	MYSQL* temp = mysql_init(NULL);
	mysql_options(temp, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_options(temp, MYSQL_OPT_RECONNECT, &my_true);

	if( !mysql_real_connect(temp, m_Hostname, m_Username, m_Password, m_DatabaseName, m_nPort, NULL , 0) )
	{
		mysql_close(temp);

		// 设置连接断开
		m_bConLost = TRUE;

		// 这里需要给上层报告，调用上层的回调函数
		if( m_fnWarning ) (*m_fnWarning)(this, EDBE_System, 0);
		return FALSE;
	}

	if( con->getMySql() != NULL )
		mysql_close(con->getMySql( ));
	con->setMySql( temp );

	return TRUE;
}

// 非阻塞数据库操作函数
ThreadRetType DataBase::ThreadAsynQuery()
{
	// 为该线程提供一条固定的连接
	Connection* con = getFreeConnection();
	if( NULL == con ) return (ThreadRetType)0;

	while( FALSE == m_TerminateAsynQuery ) {
		while(TRUE) {
			MySqlStream* pStream = m_AsynStreamQueue.pop();
			if( !pStream ) break;

			waitExecute(pStream, con);
			returnStream(pStream);
		}

		m_AsynStreamQueue.wait( 120 );
	}

	while(TRUE) {
		// 释放掉可能还剩余的内容
		MySqlStream* pStream = m_AsynStreamQueue.pop();
		if( !pStream ) break;

		waitExecute(pStream, con);
		returnStream(pStream);
	}

	// 交还连接
	returnConnection(con);
	
	return (ThreadRetType)0;
}


ThreadRetType DataBase::ThreadCheck()
{
	while( FALSE == m_TerminateCheck ){
		INT nQueryNum = m_AsynStreamQueue.size( );
		if( nQueryNum >= WARNING_QUEUE_NUM ){
			if( m_fnWarning ) (*m_fnWarning)(this, EDBE_QueueFull, nQueryNum);
		}
			
		INT nPoolNum = m_StreamPool.getAllocSize( );
		if( nPoolNum >= WARNING_POOL_NUM ) {
			if( m_fnWarning ) (*m_fnWarning)(this, EDBE_PoolFull, nPoolNum);
		} 

		MySleep(2000);
	}

	return (ThreadRetType)0;
}

