/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef __DUMP__
#define __DUMP__


/*
** _set_se_translator only for c++ ,translate SEH to c++ exception
**
**      To specify a custom translation function, call _set_se_translator -
** with the name of your translation function as its argument.
** The translator function that you write is called once -
** for each function invocation on the stack that has try blocks. 
** There is no default translator function.
**      Your translator function should do no more than throw a C++ typed exception. 
** If it does anything in addition to throwing (such as writing to a log file, 
** for example) your program might not behave as expected, 
** because the number of times the translator function is invoked is platform-dependent.
**     In a multithreaded environment, translator functions are maintained separately for each thread. 
** Each new thread needs to install its own translator function. 
** Thus, each thread is in charge of its own translation handling. 
** _set_se_translator is specific to one thread; another DLL can install a different translation function.
**
*/

BASE_NAME_SPACES

#if defined( __WINDOWS__ )

LONG WINAPI SEH_ExceptionFilter(EXCEPTION_POINTERS* pExp);

#if defined(__cplusplus)

class SE_Exception
{
public:
	SE_Exception(unsigned int u, EXCEPTION_POINTERS* pExp);
};

VOID SEH_Translation( unsigned int u, EXCEPTION_POINTERS* pExp );

#define __set_se_handler__	_set_se_translator( SEH_Translation );
#define __THREAD_PROTECTS__	_set_se_translator( SEH_Translation ); try { 
#define __THREAD_PROTECTE__ } catch ( SE_Exception ){ }
#define __PROCESS_PROTECT__	SetUnhandledExceptionFilter(SEH_ExceptionFilter);

#else /* __cplusplus  */

#define __PROCESS_PROTECT__ SetUnhandledExceptionFilter(SEH_ExceptionFilter);
#define __THREAD_PROTECTS__	
#define __THREAD_PROTECTE__ 

#endif /*__cplusplus*/

#else /* __WINDOWS__ */

#define __set_se_handler__
#define __PROCESS_PROTECT__ 
#define __THREAD_PROTECTS__	
#define __THREAD_PROTECTE__ 

#endif /* __WINDOWS__ */

BASE_NAME_SPACEE

#endif /** __DUMP__ */
