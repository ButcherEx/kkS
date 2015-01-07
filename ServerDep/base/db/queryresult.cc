/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#include <core/sfcore.h>
#include <db/dbdef.h>
#include <mysql.h>
#include <db/field.h>
#include <db/queryresult.h>

static Field::FieldType ConvertType(enum_field_types mysqlType);


QueryResult::QueryResult(MYSQL_RES* result, INT nRows, INT nCols)
: m_Result(result), m_Rows(nRows), m_Cols(nCols)
{
	m_CurrentRow = new Field[m_Cols];

	MYSQL_FIELD* field = mysql_fetch_fields(m_Result);
	for(INT n = 0; n < m_Cols; n++) {
		m_CurrentRow[n].setALL(field[n].name, ConvertType(field[n].type), field[n].max_length);
	}
}

QueryResult::~QueryResult()
{
	delete[] m_CurrentRow;
	if( m_Result ) mysql_free_result(m_Result);
}

/*
** 注意：当生成QueryResult时，
** 上层需要立刻调用一次NextRow
** 以便定位到结果集的第一行
*/
BOOL QueryResult::nextRow()
{
	MYSQL_ROW row = mysql_fetch_row(m_Result);
	if( NULL == row )
		return FALSE;

	ULONG* uLen = mysql_fetch_lengths(m_Result);
	for(INT n = 0; n < m_Cols; n++) {
		m_CurrentRow[n].setValue(row[n]);
		m_CurrentRow[n].setLen(uLen[n]);
	}

	return TRUE;
}

Field::FieldType ConvertType(enum_field_types mysqlType) 
{
	switch ( mysqlType ) {
	case FIELD_TYPE_TIMESTAMP:
	case FIELD_TYPE_DATE:
	case FIELD_TYPE_TIME:
	case FIELD_TYPE_DATETIME:
	case FIELD_TYPE_YEAR:
	case FIELD_TYPE_STRING:
	case FIELD_TYPE_VAR_STRING:
	case FIELD_TYPE_SET:
	case FIELD_TYPE_NULL:
		return Field::EFT_STRING;
	case FIELD_TYPE_TINY:
	case FIELD_TYPE_SHORT:
	case FIELD_TYPE_LONG:
	case FIELD_TYPE_INT24:
	case FIELD_TYPE_LONGLONG:
	case FIELD_TYPE_ENUM:
		return Field::EFT_INTEGER;
	case FIELD_TYPE_DECIMAL:
	case FIELD_TYPE_FLOAT:
	case FIELD_TYPE_DOUBLE:
		return Field::EFT_FLOAT;
	case FIELD_TYPE_BLOB:
		return Field::EFT_BLOB;
	default:
		return Field::EFT_UNKNOWN;
	}
}

