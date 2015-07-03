
#include "CpuMemStat.h"
#include "Assertx.h"
#include "MacroDefine.h"
#include "Timer.h"

#if defined(__LINUX__)
#include <unistd.h>
#include <sys/param.h>

CpuMemStat::CpuMemStat()
{
	m_pid = getpid();
	m_utime = 0;
	m_ktime = 0;
	m_wcutime = 0;
	m_wcktime = 0;
	m_vmsize = 0;
	m_vmrss = 0;
	m_lastcputime = m_utime + m_ktime + m_wcktime + m_wcutime ;
	m_lastcalltime = TimeUtil::AnsiTime();
}

void CpuMemStat::RebuildCpu()
{
	__ENTER_FUNCTION_EX
	{
		CHAR statFile[MAX_PATH] = {0};
		tsnprintf(statFile, MAX_PATH, "/proc/%d/stat", m_pid);
		FILE* fp = fopen(statFile,"r");
		AssertEx(fp != NULL, statFile);

		CHAR fileBuf[4096] = {0};
		int32_t nReadBytes = fread(fileBuf, sizeof(CHAR), 4096, fp);
		fclose(fp);

		AssertEx(nReadBytes>0&& nReadBytes<sizeof(fileBuf), statFile);

		for(int32_t nIndex = 0, nFileIndex = 0;
			nIndex<nReadBytes&&nFileIndex<STAT_COUNT)
		{
			for(;
				nIndex < nReadBytes && isspace(fileBuf[nIndex]);
				nIndex++);

			if(!(nIndex<nReadBytes))
			{
				break;
			}

			switch (nFileIndex)
			{
			case STAT_UTIME:
				m_utime = strtoul(&fileBuf[nIndex], NULL, 0);
				break;
			case STAT_KTIME:
				m_ktime = strtoul(&fileBuf[nIndex], NULL, 0);
				break;
			case STAT_WCKTIME:
				m_wcktime = strtoul(&fileBuf[nIndex], NULL, 0);
				break;
			case STAT_WCUTIME:
				m_wcutime = strtoul(&fileBuf[nIndex], NULL, 0);
				break;
			default:
				break;
			}

			for(;
				nIndex < nReadBytes && !isspace(fileBuf[nIndex]);
				nIndex++);

			if(!(nIndex<nReadBytes))
			{
				break;
			}

		}
	}
	__LEAVE_FUNCTION_EX
}

float CpuMemStat::GetCpuRate()
{
	__ENTER_FUNCTION_EX
	{
		uint64_t uCpuTime = m_utime + m_ktime + m_wcktime + m_wcutime ;
		time_t nNowTime = TimeUtil::AnsiTime();

		float fRet = 0.0f;
		float fCpuInterval = (float)(uCpuTime - m_lastcalltime) / (float)HZ;
		int32_t nTimeInterval = nNowTime - m_lastcalltime;
		if( nTimeInterval > 0 ) fRet = fCpuInterval / (float)nTimeInterval;

		m_lastcalltime = nNowTime;
		m_lastcputime = uCpuTime;

		return fRet;
	}
	__LEAVE_FUNCTION_EX

		return 0.0f;
}

void CpuMemStat::RebuildMem()
{
	__ENTER_FUNCTION_EX
	{
		CHAR statFile[MAX_PATH] = {0};
		tsnprintf(statFile, MAX_PATH, "/proc/%d/stat", m_pid);
		FILE* fp = fopen(statFile,"r");
		AssertEx(fp != NULL, statFile);

		CHAR fileBuf[4096] = {0};
		int32_t nReadBytes = fread(fileBuf, sizeof(CHAR), 4096, fp);
		fclose(fp);

		AssertEx(nReadBytes>0&& nReadBytes<sizeof(fileBuf), statFile);

		const CHAR* pVMSize = strstr(fileBuf, "VmSize:");
		if(pVMSize != NULL)
		{
			pVMSize += strlen("VmSize:");
			for(;
				pVMSize < &fileBuf[sizeof(fileBuf)-1] && isspace(*pVMSize);
				pVMSize++);
			if( pVMSize < &fileBuf[sizeof(fileBuf)-1] )
			{
				m_vmsize = strtoul(pVMSize, NULL, 0);
			}
		}

		const CHAR* pVMRSS = strstr(fileBuf, "VmRSS:");
		if(pVMRSS != NULL)
		{
			pVMRSS += strlen("VmRSS:");
			for(;
				pVMRSS < &fileBuf[sizeof(fileBuf)-1] && isspace(*pVMRSS);
				pVMRSS++);
			if( pVMRSS < &fileBuf[sizeof(fileBuf)-1] )
			{
				m_vmrss = strtoul(pVMSize, NULL, 0);
			}
		}

	}
	__LEAVE_FUNCTION_EX
}

uint64_t CpuMemStat::GetVmSize()
{
	return m_vmsize;
}

uint64_t CpuMemStat::GetVmRSS()
{
	return m_vmrss;
}
#elif defined(__WINDOWS__)
#include <psapi.h>  
/// 时间转换  
static uint64_t file_time_2_utc(const FILETIME& ftime)  
{  
	LARGE_INTEGER li;  

	li.LowPart = ftime.dwLowDateTime;  
	li.HighPart = ftime.dwHighDateTime;  
	return li.QuadPart;  
}  


/// 获得CPU的核数  
static int get_processor_number()  
{  
	 
}  

CpuMemStat::CpuMemStat()
{
	m_PhysMem = 0;
	m_VirtMem = 0;
	m_PagedPool = 0;
	m_LastSystemTime = 0;  
	m_LastProcessTime = 0;
	m_SystemTime = 0;  
	m_ProcessTime = 0;
	m_SysPhysMemTotal = 0;
	m_SysPhysMemAvail = 0;
	m_SysVirtMemTotal = 0;
	m_SysVirtMemAvail = 0;

	SYSTEM_INFO info;  
	GetSystemInfo(&info);  
	m_PageSize = (int32_t)info.dwPageSize;
	m_NumberOfProcessors = (int32_t)info.dwNumberOfProcessors;

	SetTime(m_LastSystemTime, m_LastProcessTime);
}

void CpuMemStat::SetTime(int64_t& sysTime, int64_t& ProcessTime)
{
	__ENTER_FUNCTION_EX
	{
		FILETIME now; 
		FILETIME creation_time;  
		FILETIME exit_time; 
		FILETIME kernel_time;  
		FILETIME user_time; 

		sysTime = 0;
		ProcessTime = 0;

		GetSystemTimeAsFileTime(&now);  
		if (GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,  
			&kernel_time, &user_time) == TRUE)  
		{  
			ProcessTime = (file_time_2_utc(kernel_time) + file_time_2_utc(user_time))   
				/ m_NumberOfProcessors;  

			sysTime = file_time_2_utc(now);  
		} 		
	}
	__LEAVE_FUNCTION_EX
}

#define DIV (20)
void CpuMemStat::RebuildCpu()
{
	__ENTER_FUNCTION_EX
	{
		SetTime(m_SystemTime, m_ProcessTime);
	}
	__LEAVE_FUNCTION_EX
}
float CpuMemStat::GetCpuRate()
{
	__ENTER_FUNCTION_EX
	{
		int64_t sysTimeDiff, processTimeDiff;  

		sysTimeDiff = m_SystemTime - m_LastSystemTime;  
		processTimeDiff = m_ProcessTime - m_LastProcessTime;  
		if( processTimeDiff <= 0 ) return 0.0f;
		// We add time_delta / 2 so the result is rounded.  
		m_LastProcessTime = m_ProcessTime;
		m_LastSystemTime = m_SystemTime;
		return ((int32_t)((sysTimeDiff * 100 + processTimeDiff / 2)) / processTimeDiff)/100.0f;
	}
	__LEAVE_FUNCTION_EX
		return 0.0f;
}
void CpuMemStat::RebuildMem()
{
	__ENTER_FUNCTION_EX
	{
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof (statex);
		GlobalMemoryStatusEx (&statex);

		m_SysPhysMemTotal = (statex.ullTotalPhys >> DIV );
		m_SysPhysMemAvail = (statex.ullAvailPhys >> DIV );
		m_SysVirtMemTotal = (statex.ullTotalVirtual >> DIV );
		m_SysVirtMemAvail = (statex.ullAvailVirtual >> DIV );

		PROCESS_MEMORY_COUNTERS pmc;  
		pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS);
		if(GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))  
		{  
			m_PhysMem = pmc.WorkingSetSize / 1024;  
			m_VirtMem = pmc.PagefileUsage  / 1024;  
			m_PagedPool = pmc.QuotaPagedPoolUsage / 1024;
		}  
	}
	__LEAVE_FUNCTION_EX
}
uint64_t CpuMemStat::GetVmSize()
{
	return m_VirtMem;
}
uint64_t CpuMemStat::GetPhsySize()
{
	return m_PhysMem;
}

uint64_t CpuMemStat::GetPagedPoolSize()
{
	return m_PagedPool;
}

#endif

LOG_DEF(Cpu);

void LogCpuMemStat(const CHAR* szDesc)
{
	__ENTER_FUNCTION_EX
	{
#if defined(__LINUX__) || defined(__WINDOWS__)
		static CpuMemStat __s_CpuMemStat;
		__s_CpuMemStat.RebuildCpu();
		__s_CpuMemStat.RebuildMem();
#ifdef __LINUX__
		LOG_DEBUG(Cpu, 
			"Desc(%s), CPU(%0.2f), VmSize(%llu), VmRSS|Phsy(%llu)\n",
			szDesc != NULL ? szDesc : "Unknown",
			__s_CpuMemStat.GetCpuRate(),
			__s_CpuMemStat.GetVmSize(),
			__s_CpuMemStat.GetVmRSS());
#else
		LOG_DEBUG(Cpu, 
			"Desc(%s), CPU(%0.2f), PhsyMemSize(%llu), VirMemSize(%llu), PagedPoolUsage(%llu)\n",
			szDesc != NULL ? szDesc : "Unknown",
			__s_CpuMemStat.GetCpuRate(),
			__s_CpuMemStat.GetPhsySize(),
			__s_CpuMemStat.GetVmSize(),
			__s_CpuMemStat.GetPagedPoolSize());
#endif

#endif
	}
	__LEAVE_FUNCTION_EX
}

