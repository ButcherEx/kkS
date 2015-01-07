/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/


/*
** QueryResult类 - 提供数据库结果集的相关功能，对结果集所保存的数据和属性进行管理
** 1. 提供按行依序提取结果的功能，每行新的列值会更新到各个Field中
** 2. 提供按索引和字符串检索列的功能
** 3. 提供查询属性的功能，最大行数，最大列数等等
** 4. 提供游标的功能，可以定位到对应行和对应列（后续完成）
** ##注意事项
** 1. 该结果集只适用于使用mysql_store_result()返回的结果集，对于mysql_use_reslut()的结果集现在还不提供支持
** 2. 对于mysql_use_result使用的结果集，一般不推荐使用，后续会提供该功能的简单类
*/
//----------------------------------------------------------------------------------------------------

#ifndef __QUERYRESULT__
#define __QUERYRESULT__

class  QueryResult
{
public:
	BOOL nextRow();
public:
	__forceinline INT getRowCount() CONST ;
	__forceinline INT getFieldCount() CONST ;
	__forceinline CONST  Field* fetch() CONST ;
	__forceinline CONST  Field* operator [] (INT nIndex) CONST ;
public:
	QueryResult(MYSQL_RES* result, INT nRows, INT nCols);
	~QueryResult();
private:
	MYSQL_RES*		m_Result;
	INT				m_Rows;
	INT				m_Cols;
	Field*			m_CurrentRow;
};

INT QueryResult::getRowCount() CONST 		{ return m_Rows; }
INT QueryResult::getFieldCount() CONST 		{ return m_Cols; }
CONST  Field* QueryResult::fetch() CONST 		{ return m_CurrentRow; } 
CONST  Field* QueryResult::operator [] (INT nIndex) CONST   
{
	if(nIndex >= 0 && nIndex < m_Cols && m_CurrentRow != NULL) 
		return &m_CurrentRow[nIndex];
	return NULL;
}

#endif /* __QUERYRESULT__ */
