/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#include <PreCompier.h>
#ifdef __WINDOWS__
#include <DbgHelp.h>
#include <psapi.h>	
#include <eh.h>
#include <windump.h>



#pragma comment(lib, "dbghelp.lib")

BASE_NAME_SPACES

static void _CreateWinDumpFile(unsigned int u, EXCEPTION_POINTERS* pExp);

#if defined(__cplusplus)
VOID SEH_Translation( unsigned int u, EXCEPTION_POINTERS* pExp )
{
	throw SE_Exception(u, pExp);
}

SE_Exception::SE_Exception(unsigned int u, EXCEPTION_POINTERS *pExp)
{
	_CreateWinDumpFile(u, pExp);
}
#endif /* __cplusplus */

LONG WINAPI SEH_ExceptionFilter(EXCEPTION_POINTERS* pExp){
	_CreateWinDumpFile( 0, pExp );
	return EXCEPTION_CONTINUE_SEARCH;
}


VOID _CreateWinDumpFile(unsigned int u, EXCEPTION_POINTERS* pExp)
{
	HANDLE h;
	time_t t;
	struct tm *tm;
	CHAR *rslash, *rdot;
	CHAR dump[MAX_PATH] ={0}, exe[MAX_PATH] ={0};
	MINIDUMP_EXCEPTION_INFORMATION info;

	t = time(NULL);
	tm = localtime(&t);

	GetModuleFileNameA(NULL,exe, MAX_PATH);

	rdot = strrchr(exe, '.');
	rdot ? (*rdot = 0) : (0);
	rslash = strrchr(exe,'\\');
	rslash = rslash ? rslash + 1 : NULL;

	sprintf(dump, "%s(%04d%02d%02d%02d%02d%02d).%04d.core.dmp",
		rslash ? rslash : "", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec, 1000 + rand( ));

	h = (HANDLE)CreateFileA(dump, GENERIC_WRITE, FILE_SHARE_READ, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h == INVALID_HANDLE_VALUE){
		return;
	}

	info.ThreadId = GetCurrentThreadId( );
	info.ExceptionPointers = pExp;
	info.ClientPointers = 0;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		h, MiniDumpWithFullMemory , &info, NULL, NULL);

	/** ZIP */
	::CloseHandle(h);
}
BASE_NAME_SPACEE

#endif /* __WINDOWS__ */

