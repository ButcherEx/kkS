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
#include <db/dbhandler.h>

MyDBHandler::MyDBHandler(  )
: m_Database(NULL)
{

}

MyDBHandler::~MyDBHandler( )
{

}

VOID MyDBHandler::attach(DataBase *db)
{
	m_Database = db;
}

DataBase *MyDBHandler::detach( )
{
	DataBase *db = m_Database;
	m_Database = NULL;
	return db;
}

BOOL MyDBHandler::commonSelect(
				  IN OUT VOID* &pBuffer, 
				  OUT INT *pNumber, 
				  DWORD Param1, 
				  DWORD Param2, 
				  SelectFormatFunc formatFunc, 
				  ProcResultFunc procsFunc
)
{
	BOOL retval = TRUE;

	MySqlStream *pStream = m_Database->getStream( );

	pStream->clear( );

	(formatFunc)(pStream, Param1, Param2);
	QueryResult *pResult = m_Database->query(pStream);
	if( !pResult || pResult->getRowCount( ) <= 0 ){
		*pNumber = 0; retval = FALSE;
	} else {
		(procsFunc)(pBuffer, pNumber, pResult);
	}

	m_Database->returnStream(pStream);
	m_Database->freeQueryResult(pResult);
	return retval;
}

BOOL MyDBHandler::commonUpdate(
				  DWORD Param1, 
				  CONST VOID *pBuffer, 
				  INT elmCnt, 
				  INT sizeOne, 
				  UpdateFormatFunc formatFunc, 
				  OUT VOID **ppBuffer
)
{
	if(elmCnt <=0 ) return TRUE;

	BOOL retval = TRUE;
	MySqlStream *pStream = m_Database->getStream( );
	
	CHAR *pData = (CHAR*)pBuffer;
	pStream->clear( );
	for(INT i = 0; i < elmCnt; i++){
		(formatFunc)(pStream, Param1, pData);
		if( !m_Database->query(pStream) ){
			retval = FALSE;
		} else {
			pData += sizeOne;
		}
	}

	m_Database->returnStream(pStream);

	if(ppBuffer) *ppBuffer = pData;

	return retval;
}

VOID MyDBHandler::commonUpdateLog(
					 CONST CHAR *tabePostFix, 
					 CONST VOID *pBuffer, 
					 updateFormatFunc2 formatFunc
)
{
	BOOL retval = TRUE;
	MySqlStream *pStream = m_Database->getStream( );
	pStream->clear( );
	(formatFunc)(pStream, tabePostFix, pBuffer);
	m_Database->addAsynUpdate(pStream);
}



