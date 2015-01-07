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
#include <db/mysqlstream.h>
#include <db/streamtslist.h>

VOID MyStreamTSList::wait(INT millisecond)
{
	MyWaitEvent(&m_Event, millisecond);
}

VOID MyStreamTSList::active( )
{
	MyActiveEvent(&m_Event);
}

MyStreamTSList::MyStreamTSList( )
{
	MyCreateEvent(&m_Event, FALSE, TRUE, 1);
}

MyStreamTSList::~MyStreamTSList( )
{
	MyCloseEvent(&m_Event);
}
