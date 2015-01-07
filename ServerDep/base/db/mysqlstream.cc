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
/***/
MySqlStream& MySqlStream::operator << (CONST  CHAR *p)
{
	INT len = (INT)strlen(p);
	fill(p, len);
	return *this;
}

MySqlStream& MySqlStream::operator << (CONST  INT p)
{
	CHAR szBuf[64] = {0};
	_Myitoa(p, szBuf, 64);

	INT nLen = (INT)strlen(szBuf);
	grow(nLen);
	memcpy(m_Buf + m_Pos, szBuf, nLen);
	setEnd(nLen);
	return *this;
}
MySqlStream& MySqlStream::operator << (CONST  DWORD p)
{
	CHAR szBuf[64] = {0};
	_Myi64toa(p, szBuf, 64);

	INT nLen = (INT)strlen(szBuf);
	grow(nLen);
	memcpy(m_Buf + m_Pos, szBuf, nLen);
	setEnd(nLen);
	return *this;

}

MySqlStream& MySqlStream::operator << (CONST  FLOAT p)
{ 
	return operator << (static_cast<DOUBLE>(p)); 
}

MySqlStream& MySqlStream::operator << (CONST  WORD p)
{ 
	return operator << (static_cast<DWORD>(p));
}

MySqlStream& MySqlStream::operator << (CONST  BYTE p) 
{ 
	return operator << (static_cast<DWORD>(p));
}

MySqlStream& MySqlStream::operator << (CONST  LLONG p)
{
	CHAR szBuf[96] = {0};
	_Myi64toa(p, szBuf, 10);

	INT nLen = (INT)strlen(szBuf);
	grow(nLen);
	memcpy(m_Buf + m_Pos, szBuf, nLen);
	setEnd(nLen);
	return *this;

}

MySqlStream& MySqlStream::operator << (CONST  DOUBLE p)
{
	CHAR szBuf[_CVTBUFSIZE] = {0};
	gcvt(p, 5, szBuf);

	INT nLen = (INT)strlen(szBuf);
	grow(nLen);
	memcpy(m_Buf + m_Pos, szBuf, nLen);
	setEnd(nLen);
	return *this;
}


/**/
MySqlStream& MySqlStream::fillString(CONST  CHAR* p, Connection* con)
{
	if( NULL == p || '\0' == p[0] )
		return (*this);

	// ×ªÒå×Ö·û´®
	INT nStrlen = (INT)strlen(p);
	fill(p, nStrlen, con);
	return *this;
}

#ifdef __WINDOWS__
MySqlStream& MySqlStream::fillString(CONST  WCHAR* p, Connection* con)
{
	if( NULL == p || 0 == p[0] )
		return (*this);

	// ×Ö·û´®×ª»»
	INT nDesiredLen = WideCharToMultiByte(CP_UTF8, 0, p, -1, NULL, 0, NULL, NULL);
	if( 0 == nDesiredLen ) return (*this);

	CHAR* szTemp = new CHAR[nDesiredLen];
	if( !WideCharToMultiByte(CP_UTF8, 0, p, -1, szTemp, nDesiredLen, NULL, NULL) ){
		SAFE_DEL_ARRAY(szTemp);
		return (*this);
	}

	INT nStrlen = (INT)strlen(szTemp);
	fill(szTemp, nStrlen, con);
	SAFE_DEL_ARRAY(szTemp);

	return (*this);
}
#endif


MySqlStream& MySqlStream::fillBlob(CONST  VOID* p, INT size, Connection* con)
{
	if( NULL == p || size <= 0 )	return (*this);

	if( NULL == con ) return (*this);

	fill((CONST  CHAR*)p, size, con);
	return *this;
}


MySqlStream& MySqlStream::fillBlob(VOID* p, INT size)
{
	fill((CONST  CHAR*)p, size, NULL);
	return *this;
}

/**/
MySqlStream& MySqlStream::setCopyItem(CONST  CHAR* table1, CONST  CHAR* fields, CONST  CHAR* table2)
{
	clear();
	(*this) << "insert into " << table1;
	(*this) << "select " << fields << " from " << table2;
	return *this;
}

MySqlStream& MySqlStream::setSelect(CONST  CHAR* table, CONST  CHAR* fields)
{
	clear();
	(*this) << "select " << fields << " from " << table;
	return *this;
}

MySqlStream& MySqlStream::setUpdate(CONST  CHAR* table)
{
	clear();
	(*this) << "update " << table << " set ";
	return *this;
}

MySqlStream& MySqlStream::setInsert(CONST  CHAR* table)
{
	clear();
	(*this) << "insert into " << table << " set ";
	return *this;
}

MySqlStream& MySqlStream::setInsert(CONST  CHAR* table, CONST  CHAR* postfix)
{
	clear();
	(*this) << "insert into " << table << postfix << " set ";
	return *this;
}

MySqlStream& MySqlStream::setReplace(CONST  CHAR* table)
{
	clear();
	(*this) << "replace into " << table << " set ";
	return *this;
}

MySqlStream& MySqlStream::setDelete(CONST  CHAR* table)
{
	clear();
	(*this) << "delete from " << table;
	return *this;
}

MySqlStream& MySqlStream::setLimit(INT nNum, INT nBegin)
{
	(*this) << " limit ";
	if( nBegin > 0 ) (*this) << nBegin << (",");
	(*this) << nNum;

	return *this;
}

MySqlStream& MySqlStream::setWhere()
{
	(*this) << " where ";
	return *this;
}

/**/
MySqlStream::MySqlStream(INT initSize)
: m_BufLen(initSize)
, m_InitSize(initSize)
, m_Pos(0)
, next(0)
, prev(0)
{
	m_Buf = new CHAR[m_InitSize];
	if( NULL == m_Buf ) { abort(); }
	m_Buf[0] = 0;
}


MySqlStream::~MySqlStream()
{
	SAFE_DEL_ARRAY(m_Buf);
}

BOOL MySqlStream::seek(INT nPos)
{
	if( nPos < m_BufLen )
	{
		m_Pos = nPos;
		end();
		return TRUE;
	}
	return FALSE;
}


VOID MySqlStream::grow(INT size)
{
	if( size <= 0 ) size = m_InitSize;

	if( m_Pos + size < m_BufLen )
		return;

	INT nTmpLen = m_BufLen  + size;
	CHAR* pTmp = new CHAR[nTmpLen];
	if( m_Buf ) {
		memcpy(pTmp, m_Buf, m_BufLen);
		SAFE_DEL_ARRAY(m_Buf);
	}

	m_Buf = pTmp;
	m_BufLen = nTmpLen;
}

MySqlStream& MySqlStream::fill(CONST  CHAR* p, INT size, Connection* con)
{
	if( NULL != con ) {
		INT nStrlen = size;
		INT nGrowSize = (nStrlen << 1) + 1;
		grow(nGrowSize);
		CHAR* pRet = m_Buf + m_Pos;
		memset(pRet, 0, nGrowSize);
		INT nLen = mysql_real_escape_string(con->getMySql( ), pRet, p, nStrlen);	
		setEnd(nLen);
	} else {
		grow(size);
		CHAR* pRet = m_Buf + m_Pos;
		strcat(pRet, p);
		setEnd(size);;
	}

	return *this;
}

