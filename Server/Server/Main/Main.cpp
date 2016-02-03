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

bool testBitIf(int32_t n, int32_t val)
{
	int32_t powN = (int)pow(10, n-1);
	if (powN > val){
		return false;
	}

	int32_t left = val / powN;
	if (left < 10){
		return left == 1;
	} else {
		int32_t mod = left % 10;
		return mod == 1;
	}

	return false;
}

void add(char*result, const char* s1, char* s2)
{
	int len1 = strlen(s1);
	int len2 = strlen(s2);

	int lenOfResult = 0;
	int carryNum = 0;
	for (int i = len1 - 1,  j = len2 - 1; (i >= 0) || (j >= 0) || (carryNum > 0); i--, j--) {
		int num1 = (i >= 0) ? (s1[i] - '0') : 0;
		int num2 = (j >= 0) ? (s2[j] - '0') : 0;
		int tmpSum = num1 + num2 + carryNum;
		carryNum = tmpSum / 10;
		result[lenOfResult++] = tmpSum % 10 + '0';
	}

	char *pResultStart = result;
	char *pResultEnd = result + lenOfResult - 1;
	for (int i = lenOfResult / 2; i >= 0; i--){
		char tmpChar = *pResultStart;
		*pResultStart++ = *pResultEnd;
		*pResultEnd-- = tmpChar;
	}

	return;
}


void   maxP()
{
#define MAXLEN 1000
	char a1[MAXLEN];
	char a2[MAXLEN];
	static int v1[MAXLEN];
	static int v2[MAXLEN];
	static int v3[MAXLEN];
	int i, j, n, L, z;
	scanf("%d", &n);
	for (j = 0; j<n; j++) {
		scanf("%s%s", a1, a2);

		L = strlen(a1);
		for (i = 0; i<L; i++) v1[i] = a1[L - 1 - i] - '0';

		L = strlen(a2);
		for (i = 0; i<L; i++) v2[i] = a2[L - 1 - i] - '0';

		for (i = 0; i<MAXLEN; i++) v3[i] = v1[i] + v2[i];

		for (i = 0; i<MAXLEN; i++) {
			if (v3[i] >= 10) {
				v3[i + 1] += v3[i] / 10;
				v3[i] = v3[i] % 10;
			}
		}

		printf("Case %d:\n", j + 1);
		printf("%s + %s = ", a1, a2);

		z = 0;
		for (i = MAXLEN - 1; i >= 0; i--) {
			if (z == 0) {
				if (v3[i] != 0) {
					printf("%d", v3[i]);
					z = 1;
				}
			}
			else {
				printf("%d", v3[i]);
			}
		}
		if (z == 0) printf("0");

		printf("\n");
	}
}

int32_t main(int32_t argc, CHAR* argv[])
{	

// #if defined(__WINDOWS__)
// 	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
// #endif
	//maxP();

	#define MAXLEN 1000
	char a1[MAXLEN] = {0};
	char a2[MAXLEN] = { 0 };
	char a3[MAXLEN] = { 0 };
	scanf("%s%s", a1, a2);

	add(a3, a1, a2);
	__ENTER_FUNCTION

	_MY_TRY
	{

		int32_t testVal[4] = { 1000000, 101010, 1111111, 1010101 };

		for (int i = 0; i < 4; i++){
			int val = 0;
			for (int j = 1; j < 8; j++){
				bool isSet = testBitIf(j, testVal[i]);
				if (isSet) val = val + (int)pow(10, j-1);
			}
			printf("%d\n", val);
		}


		FLString<32> f32(32);
		FLString<64> f64((int64_t)555);
		FLString<64> f642((uint64_t)777);

		FLString<128> fString("Hello World");

		Ini ConfigFile("GameConfig.ini");
		int32_t interval = ConfigFile.ReadInt("Login", "PlayerSaveInterval");
		int32_t isopenAdvertisement = ConfigFile.ReadInt("Advertisement", "AdvertisementFirstTime");

		fString += f32;

		fString = f64;
		fString += 0xffffff;

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

		int32_t i = 9;
		int32_t div = i / 9999999;
		Assert(div);
		int32_t val = i / div;

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





