/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/


/*
** QueryResult�� - �ṩ���ݿ���������ع��ܣ��Խ��������������ݺ����Խ��й���
** 1. �ṩ����������ȡ����Ĺ��ܣ�ÿ���µ���ֵ����µ�����Field��
** 2. �ṩ���������ַ��������еĹ���
** 3. �ṩ��ѯ���ԵĹ��ܣ������������������ȵ�
** 4. �ṩ�α�Ĺ��ܣ����Զ�λ����Ӧ�кͶ�Ӧ�У�������ɣ�
** ##ע������
** 1. �ý����ֻ������ʹ��mysql_store_result()���صĽ����������mysql_use_reslut()�Ľ�������ڻ����ṩ֧��
** 2. ����mysql_use_resultʹ�õĽ������һ�㲻�Ƽ�ʹ�ã��������ṩ�ù��ܵļ���
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
