#ifndef __CPU_STATE_H__
#define __CPU_STATE_H__
#include "Base.h"
#if defined(__LINUX__)
//解析/proc/[pid]/stat
class CpuMemStat
{
public:
	enum 
	{
		STAT_UTIME = 13,
		STAT_KTIME = 14,
		STAT_WCUTIME = 15,
		STAT_WCKTIME = 16,
		STAT_COUNT
	};
public:
	CpuMemStat();
public:
	void RebuildCpu();
	float GetCpuRate();
public:
	void RebuildMem();
	uint64_t GetVmSize();
	uint64_t GetVmRSS();
private:
	uint64_t	m_utime; //任务在用户态运行的时间，单位为jiffies
	uint64_t	m_ktime; //该任务在核心态运行的时间，单位为jiffies
	uint64_t	m_wcutime; // 累计的该任务的所有的waited-for进程曾经在用户态运行的时间，单位为jiffies
	uint64_t	m_wcktime; //累计的该任务的所有的waited-for进程曾经在核心态运行的时间，单位为jiffies
	uint64_t	m_vmsize; // （page）   虚拟内存大小。
	uint64_t	m_vmrss; // (page) 该任务当前驻留物理地址空间的大小
	int32_t		m_pid;
	uint64_t	m_lastcputime;
	time_t		m_lastcalltime;
};
#elif defined(__WINDOWS__)
class CpuMemStat
{
public:
	CpuMemStat();
public:
	void RebuildCpu();
	float GetCpuRate();
public:
	void RebuildMem();
	uint64_t GetVmSize();
	uint64_t GetPhsySize();
	uint64_t GetPagedPoolSize();
private:
	void SetTime(int64_t& sysTime, int64_t& ProcessTime);
private:
	uint64_t	m_PhysMem;
	uint64_t	m_VirtMem;
	uint64_t	m_PagedPool;
	int32_t		m_NumberOfProcessors;
	int32_t		m_PageSize;

	int64_t		m_LastSystemTime;  
	int64_t		m_LastProcessTime;
	int64_t		m_SystemTime;  
	int64_t		m_ProcessTime;

	uint64_t	m_SysPhysMemTotal;
	uint64_t	m_SysPhysMemAvail;
	uint64_t	m_SysVirtMemTotal;
	uint64_t	m_SysVirtMemAvail;
};
#endif

void LogCpuMemStat(const CHAR* szDesc);

#endif