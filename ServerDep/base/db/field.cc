/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#include <core/sfcore.h>
#include <db/dbdef.h>
#include <db/field.h>

Field::Field( ) 
: m_Value(NULL)
, m_Name(NULL)
, m_Type(EFT_UNKNOWN)
, m_Len(0)
, m_MaxLen(0) {

}

inline Field::Field(Field& field) 
: m_Value(field.m_Value)
, m_Name(field.m_Name)
, m_Type(field.m_Type)
, m_Len(field.m_Len)
, m_MaxLen(field.m_MaxLen) {

}

inline Field::Field(CHAR* value, CHAR* name, ULONG len, ULONG maxLen, FieldType type)
: m_Value(value)
, m_Name(name)
, m_Type(type)
, m_Len(len)
, m_MaxLen(maxLen) {

}

INT Field::getTCHAR(TCHAR* value, INT nLen) CONST  {
	if( !value || !m_Value || EFT_BLOB == m_Type || EFT_UNKNOWN == m_Type )
		return 0;

#if defined(_UNICODE) && defined(__WINDOWS__) 
	return MultiByteToWideChar(CP_UTF8, 0, m_Value, -1, value, nLen);
#else
	if( nLen < ((INT)m_Len + 1) ) return 0;
	memcpy(value, m_Value, m_Len+1);
	return m_Len+1;
#endif
}


mystring Field::getUnicodeString() CONST  {
	if( NULL == m_Value || EFT_BLOB == m_Type || EFT_UNKNOWN == m_Type )
		return mystring(_T(""));

#if defined(_UNICODE) && defined(__WINDOWS__) 
	INT nDesiredLen = MultiByteToWideChar(CP_UTF8, 0, m_Value, -1, NULL, 0);
	if( nDesiredLen == 0 ) return mystring(_T(""));
	TCHAR* szResultValue = new TCHAR[nDesiredLen];
	if( !MultiByteToWideChar(CP_UTF8, 0, m_Value, -1, szResultValue, nDesiredLen) ) {
		delete []szResultValue;
		return mystring(_T(""));
	}

	mystring str(szResultValue);
	delete[] szResultValue;
	return str;
#else
	return mystring(m_Value);
#endif
}

/*
** TRUE if len <= m_Len ,otherwise FALSE
*/
BOOL Field::getBlob(VOID* buffer, ULONG len) CONST 
{
	if( len < m_Len ) return FALSE;

	memcpy(buffer, m_Value, m_Len);
	return TRUE;
}

