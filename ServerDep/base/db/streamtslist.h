/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __STREAM_TSLIST__
#define __STREAM_TSLIST__

class MyStreamTSList : public MyNonCopyable
{
public:
	__forceinline MySqlStream *pop( );
	__forceinline VOID add(MySqlStream *stream);
	__forceinline INT size( );
public:
	VOID wait(INT millisecond);
	VOID active( );
public:
	MyStreamTSList( );
	~MyStreamTSList( );
private:
	EventHandle m_Event;
	MyTSList<MySqlStream> m_List;
};

/**/
MySqlStream *MyStreamTSList::pop( ){ return m_List.popHead( ); }
VOID MyStreamTSList::add(MySqlStream *stream){ m_List.addTail(stream); }
INT MyStreamTSList::size( ){ return m_List.size( ); }

#endif /* __STREAM_TSLIST__ */

