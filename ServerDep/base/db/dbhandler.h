/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __DB_HANDLER__
#define __DB_HANDLER__

/*
** 数据库处理类，执行查询、更新、删除、替换(replace into)操作
*** COMMON FUNCTION **
** 1.有结果集返回: CommonSelect (查询)
** 2.无结果集返回: CommonUpdate (更新、删除、替换)
** 3. 无结果集异步日志操作: CommonUpdateLog(更新、删除、替换)
*** NOTICE ***
** 1.所需要的内存全由外部调用者分配并管理，此类只使用不管理
** 2.获得数据库返回的结果集，可构造不定长的网络数据包
** 3.向数据库写入或者更新多条数据时，每条数据的大小必须一致
** 4.调用CommonUpdate只需提供一个格式化相应字段形如UpdateFormatFunc的函数 
** 5.调用CommonSelect出了第四条所需函数还需要提供一个处理结果集形如ProcResultFunc的函数
** 6.日志实时性不高,都采用异步方式需要形如updateFormatFunc2的函数
*/

// 数据库处理函数
typedef fastdelegate::FastDelegate3<MySqlStream*, DWORD, DWORD, VOID> SelectFormatFunc;
typedef fastdelegate::FastDelegate3<MySqlStream*, DWORD, VOID*, VOID> UpdateFormatFunc;
typedef fastdelegate::FastDelegate3<MySqlStream*, CONST CHAR*, CONST VOID*, VOID> updateFormatFunc2;
typedef fastdelegate::FastDelegate3<VOID*&, INT*, QueryResult*, VOID> ProcResultFunc;
#define MakeDelegateOfDBHandler(func) fastdelegate::MakeDelegate(this, func)

class MyDBHandler : public MyNonCopyable
{
public:
	VOID attach(DataBase *db);
	DataBase *detach( );
public:
	// 请求结果集,同步操作
	BOOL commonSelect(
		IN OUT VOID* &pBuffer, 
		OUT INT *pNumber, 
		DWORD Param1, 
		DWORD Param2, 
		SelectFormatFunc formatFunc, 
		ProcResultFunc procsFunc
	);

	// 更新数据(删除/插入/更新),不需结果,同步操作
	BOOL commonUpdate(
		DWORD Param1, 
		CONST VOID *pBuffer, 
		INT elmCnt, 
		INT sizeOne, 
		UpdateFormatFunc formatFunc, 
		OUT VOID **ppBuffer = NULL
	);

	// 日志操作,不需结果集,异步操作(删除/插入/更新)
	VOID commonUpdateLog(
		CONST CHAR *tabePostFix, 
		CONST VOID *pBuffer, 
		updateFormatFunc2 formatFunc
	);
public:
	MyDBHandler( );
	virtual ~MyDBHandler( );
private:
	DataBase *m_Database;
};

#endif /* __DB_HANDLER__ */





