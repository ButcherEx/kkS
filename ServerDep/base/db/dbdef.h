/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __DB_DEFINE__
#define __DB_DEFINE__

#include <string>
#if defined(_UNICODE) && defined(__WINDOWS__)

typedef std::wstring mystring;
#else /* defined(_UNICODE) && defined(__WINDOWS__) */
typedef std::string mystring;
#endif /* defined(_UNICODE) && defined(__WINDOWS__) */

#ifndef __WINDOWS__
#undef __WIN__
#endif

#ifndef _CVTBUFSIZE
#define _CVTBUFSIZE (309+40) /* # of digits in max. dp value + slop */
#endif

// 异步操作队列的容量如果超过该值，则警报
CONST  INT WARNING_QUEUE_NUM = 1000;	
// 底层Mystream池的容量超过这个限度，则警报
CONST  INT WARNING_POOL_NUM = 20000;	
// sqlstream 出示内存大小
CONST  INT SQL_STREAM_SIZE = 1024 * 2;


CONST  INT DB_HOST_LEN = 32;
CONST  INT DB_USER_LEN = 32;
CONST  INT DB_PASS_LEN = 128;
CONST  INT DB_DATABASE_LEN = 128;

class DataBase;
typedef VOID WARNINGCALLBACK(DataBase*, INT reason, INT param);
#pragma comment(lib,"libmysql.lib")

#endif /* __DB_DEFINE__ */

