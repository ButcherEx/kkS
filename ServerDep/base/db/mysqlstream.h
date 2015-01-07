/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __SQL_STREAM__
#define __SQL_STREAM__

class  MySqlStream : public MyNonCopyable
{
public:
	MySqlStream& fillString(CONST  CHAR* p, Connection* con=NULL);
#ifdef __WINDOWS__
	MySqlStream& fillString(CONST  WCHAR* p, Connection* con=NULL);
#endif
	MySqlStream& fillBlob(CONST  VOID* p, INT size, Connection* con);
	MySqlStream& fillBlob(VOID* p, INT size);	
public:
	MySqlStream& operator << (CONST  CHAR *p);
	MySqlStream& operator << (CONST  INT p);
	MySqlStream& operator << (CONST  DWORD p);
	MySqlStream& operator << (CONST  LLONG p);
	MySqlStream& operator << (CONST  DOUBLE p);
	MySqlStream& operator << (CONST  FLOAT p);
	MySqlStream& operator << (CONST  WORD p);
	MySqlStream& operator << (CONST  BYTE p);
public:
	MySqlStream& setCopyItem(CONST  CHAR* table1, CONST  CHAR* fields, CONST  CHAR* table2);
	MySqlStream& setSelect(CONST  CHAR* table, CONST  CHAR* fields);
	MySqlStream& setUpdate(CONST  CHAR* table);
	MySqlStream& setInsert(CONST  CHAR* table);
	MySqlStream& setInsert(CONST  CHAR* table, CONST  CHAR* postfix);
	MySqlStream& setReplace(CONST  CHAR* table);
	MySqlStream& setDelete(CONST  CHAR* table);
	MySqlStream& setLimit(INT nNum, INT nBegin = 0);
	MySqlStream& setWhere();
public:
	__forceinline VOID clear( );
	__forceinline DWORD getBufLen( ) CONST ;
	__forceinline BOOL isEmpty( ) CONST ;
	__forceinline CONST  CHAR* getBuf( ) CONST ; 
public:
	MySqlStream(INT initSize = SQL_STREAM_SIZE);
	~MySqlStream( );
private:
	__forceinline VOID setEnd(INT len);
	__forceinline VOID end( );
private:
	VOID grow(INT size);
	BOOL seek(INT nPos);
	MySqlStream& fill(CONST  CHAR* p, INT size, Connection* con = NULL);
private:
	CHAR*		m_Buf;
	INT			m_BufLen;
	INT			m_Pos;
	INT			m_InitSize;
public:
	MySqlStream *next;
	MySqlStream *prev;
};
VOID MySqlStream::setEnd(INT len)  {m_Pos += len; end( );}
VOID MySqlStream::end()			{ m_Buf[m_Pos] = '\0'; }
VOID MySqlStream::clear()			{ m_Pos = 0; end(); }
DWORD MySqlStream::getBufLen()		CONST 	{ return m_Pos; }
BOOL MySqlStream::isEmpty()		CONST 	{ return m_Pos == 0; }
CONST  CHAR*	MySqlStream::getBuf()	CONST 	{ return m_Buf; }

#endif /* __SQL_STREAM__ */

