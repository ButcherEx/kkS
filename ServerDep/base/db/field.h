/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __FIELD__
#define __FIELD__

//----------------------------------------------------------------------------------------------------
/**	Filed类 - 提供数据库结构集中当前某一行某一列的当前属性
1. 所有列的当前值均由CHAR*存储，并提供按特定类型提取数据的功能，对BOLB字段做单独处理
2. 提供外部查询的列属性，列最大程度，当前行当前列的长度，是否为NULL，字段类型等等
3. 对Unicode字符串提供支持，只允许数据表中存储utf8类型的字符串，所以m_value用CHAR*表示
*/
//----------------------------------------------------------------------------------------------------

class  Field
{
public:
	typedef enum FieldType {
		EFT_UNKNOWN	= 0,			// UNKNOWN
		EFT_INTEGER	= 1,			// integer
		EFT_STRING	= 2,			// string
		EFT_FLOAT	= 3,			// float
		EFT_BLOB	= 4				// blob
	} FieldType;
public:
	__forceinline VOID setValue(CHAR* value);
	__forceinline VOID setName(CHAR* name);
	__forceinline VOID setType(FieldType type);
	__forceinline VOID setLen(ULONG len);
	__forceinline VOID setMaxLen(ULONG maxLen);
	__forceinline VOID setALL(CHAR* name, FieldType type, ULONG maxLen);
public:
	__forceinline FieldType getType() CONST;
	__forceinline BOOL isNull()			CONST;
	__forceinline CONST CHAR* getName()	CONST;
	__forceinline CONST CHAR* getString() CONST;
	__forceinline ULONG	getLen()	CONST;
	__forceinline ULONG	getMaxLen() CONST;
	__forceinline BOOL	getBool()	CONST;
	__forceinline INT	getInt()	CONST;
	__forceinline DWORD	getDword()	CONST;
	__forceinline LLONG	getLLong()	CONST;
	__forceinline BYTE	getByte()	CONST;
	__forceinline SHORT	getShort()	CONST;
	__forceinline FLOAT	getFloat()	CONST;
	__forceinline DOUBLE getDouble() CONST;
public:
	__forceinline CONST Field& operator >> (INT& i) CONST;
	__forceinline CONST Field& operator >> (DWORD& dw) CONST;
	__forceinline CONST Field& operator >> (LLONG& ll) CONST;
	__forceinline CONST Field& operator >> (BYTE& by) CONST;
	__forceinline CONST Field& operator >> (SHORT& sh) CONST;
	__forceinline CONST Field& operator >> (FLOAT& f) CONST;
	__forceinline CONST Field& operator >> (DOUBLE& d) CONST;
public:
	INT getTCHAR(TCHAR* value, INT nLen) CONST;
	BOOL getBlob(VOID* buffer, ULONG nLen) CONST;
	mystring getUnicodeString() CONST;
public:
	Field();
	Field(CHAR* value, CHAR* name, ULONG len, ULONG maxLen, FieldType type);
	Field(Field& field);
	~Field() {}
private:
	CHAR*		m_Value;	
	CHAR*		m_Name;				
	DWORD		m_NameCrc;			
	FieldType	m_Type;				
	DWORD		m_Len;		
	DWORD		m_MaxLen;
};

VOID Field::setValue(CHAR* value)	{ m_Value = value; }
VOID Field::setName(CHAR* name)		{ m_Name = name; }	
VOID Field::setType(FieldType type)	{ m_Type = type; }
VOID Field::setLen(ULONG len)		{ m_Len = len; }
VOID Field::setMaxLen(ULONG maxLen)	{ m_MaxLen = maxLen; }
VOID Field::setALL(CHAR* name, FieldType type, ULONG maxLen) { setName(name); setType(type); setMaxLen(maxLen); }
Field::FieldType Field::getType()	CONST			{ return m_Type; }
BOOL Field::isNull()				CONST			{ return m_Value == NULL; }
CONST CHAR* Field::getName()		CONST			{ return m_Name; }
CONST CHAR* Field::getString()		CONST 			{ return m_Value; }
ULONG Field::getLen()				CONST			{ return m_Len; }
ULONG Field::getMaxLen()			CONST			{ return m_MaxLen; }
BOOL Field::getBool()				CONST			{ return m_Value ? (BOOL)atoi(m_Value) : FALSE; }
INT Field::getInt()					CONST			{ return m_Value ? static_cast<INT>(atoi(m_Value)) : 0; }
DWORD Field::getDword()				CONST			{ return m_Value ? static_cast<DWORD>(_Myatoi64(m_Value, 10)) : 0; }
LLONG Field::getLLong()				CONST			{ return m_Value ? static_cast<LLONG>(_Myatoi64(m_Value, 10)) : 0; }
BYTE Field::getByte()				CONST			{ return m_Value ? static_cast<BYTE>(_Myatoi64(m_Value, 10)) : 0; }
SHORT Field::getShort()				CONST			{ return m_Value ? static_cast<SHORT>(_Myatoi64(m_Value, 10)) : 0; }
FLOAT Field::getFloat()				CONST			{ return m_Value ? static_cast<FLOAT>(atof(m_Value)) : 0.0f; }
DOUBLE Field::getDouble()			CONST			{ return m_Value ? static_cast<DOUBLE>(atof(m_Value)) : 0.0;}
CONST Field& Field::operator >> (INT& i)	CONST	{ i	 = getInt( );    return *this;  }
CONST Field& Field::operator >> (DWORD& dw)	CONST	{ dw = getDword( );  return *this;  }
CONST Field& Field::operator >> (LLONG& ll)	CONST	{ ll = getLLong( );  return *this;  }
CONST Field& Field::operator >> (BYTE& by)	CONST	{ by = getByte( );   return *this;  }
CONST Field& Field::operator >> (SHORT& sh)	CONST	{ sh = getByte( );   return *this;  }
CONST Field& Field::operator >> (FLOAT& f)	CONST	{ f  = getFloat( );  return *this;  }
CONST Field& Field::operator >> (DOUBLE& d)	CONST	{ d  = getDouble( ); return *this;  }

#endif /* __FIELD__ */

