/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __CONNECTION__
#define __CONNECTION__

class Connection : public MyNonCopyable
{
	CMyLock		m_Lock;	// ²¢·¢Ëø
	MYSQL*		m_Mysql;
public:
	__forceinline VOID setMySql( MYSQL* mysql );
	__forceinline MYSQL* getMySql( );
	__forceinline MyLock* getLock( );
public:
	Connection( )	{ m_Mysql = NULL; }
	~Connection( )	{ }
public:
};

VOID Connection::setMySql( MYSQL* mysql ) { m_Mysql = mysql; }
MYSQL* Connection::getMySql( ) { return m_Mysql; }
MyLock* Connection::getLock( ) { return m_Lock.getLock( ); }

#endif /* __CONNECTION__ */

