//#include "stdafx.h"
// #pragma comment(lib, "vld")
// #include <vld.h>
#include "Main.h"
#include "Server.h"
#include "CpuMemStat.h"
//////////////////////////////////////////////////////////////////////////
class GroupMgr : public EventMgr
{
public:
	virtual void Handle(JoinWolrd& rMsg)
	{
		fprintf(stderr, "%s\n", rMsg.m_Message);
	}
};

GroupMgr g_GroupMgr;

void EventMgr_Test()
{
	if(1)//EventMgr/Msg Test
	{
		TimeInfo timeInfo;
		timeInfo.Update();
		EventMsgPtr joinMsg = POOL_NEW(JoinWolrd);
		EventMsgPtr unhandleMsg = POOL_NEW(UnHandledMsg);

		g_GroupMgr.AddEvent(INVALID_GUID, joinMsg);
		g_GroupMgr.AddEvent(INVALID_GUID, unhandleMsg);
		g_GroupMgr.Update(timeInfo);
	}
}

// Use to convert bytes to KB
#define DIV 1024

// Specify the width of the field in which to print the numbers. 
// The asterisk in the format specifier "%*I64d" takes an integer 
// argument and uses it to pad and right justify the number.
#define WIDTH 7

#include <psapi.h>
void PrintMemoryInfo( DWORD processID )
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	// Print the process identifier.

	printf( "\nProcess ID: %u\n", processID );

	// Print information about the memory usage of the process.

	hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID );
	if (NULL == hProcess)
		return;

	if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
	{
		printf( "\tPageFaultCount: 0x%08X\n", pmc.PageFaultCount );
		printf( "\tPeakWorkingSetSize: 0x%08X\n", 
			pmc.PeakWorkingSetSize );
		printf( "\tWorkingSetSize: 0x%08X\n", pmc.WorkingSetSize );
		printf( "\tQuotaPeakPagedPoolUsage: 0x%08X\n", 
			pmc.QuotaPeakPagedPoolUsage );
		printf( "\tQuotaPagedPoolUsage: 0x%08X\n", 
			pmc.QuotaPagedPoolUsage );
		printf( "\tQuotaPeakNonPagedPoolUsage: 0x%08X\n", 
			pmc.QuotaPeakNonPagedPoolUsage );
		printf( "\tQuotaNonPagedPoolUsage: 0x%08X\n", 
			pmc.QuotaNonPagedPoolUsage );
		printf( "\tPagefileUsage: 0x%08X\n", pmc.PagefileUsage ); 
		printf( "\tPeakPagefileUsage: 0x%08X\n", 
			pmc.PeakPagefileUsage );
	}

	CloseHandle( hProcess );

}

int32_t main(int32_t argc, CHAR* argv[])
{	

// #if defined(__WINDOWS__)
// 	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
// #endif

	__ENTER_FUNCTION

	_MY_TRY
	{

		FLString<32> f32(32);
		FLString<64> f64((int64_t)555);
		FLString<64> f642((uint64_t)777);

		FLString<128> fString("Hello World");

		fString += f32;

		fString = f64;

		TIME64_t now = TimeUtil::Now();
		TIME64_t nextNow = TimeUtil::Add(now, 1000);
		TIME64_t beforeNow = TimeUtil::Dec(now, 1000);
		bool isTimeout = TimeUtil::Timout(now, nextNow);
		isTimeout = TimeUtil::Timout(now, beforeNow);
		bool bRet =	g_InstancenManager.Init();
		Assert(bRet);

		//LogCpuMemStat("HZ");

		bRet = g_Server.Init( );
		Assert(bRet);

		g_Server.Loop();

		g_Server.Exit();
	}
	_MY_CATCH
	{
		g_Server.Exit();
	}

	g_InstancenManager.Exit();

	return	0;

	__LEAVE_FUNCTION

	return -1;
}





