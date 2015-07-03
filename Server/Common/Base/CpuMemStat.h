#ifndef __CPU_STATE_H__
#define __CPU_STATE_H__
#include "Base.h"
#if defined(__LINUX__)
//����/proc/[pid]/stat
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
	uint64_t	m_utime; //�������û�̬���е�ʱ�䣬��λΪjiffies
	uint64_t	m_ktime; //�������ں���̬���е�ʱ�䣬��λΪjiffies
	uint64_t	m_wcutime; // �ۼƵĸ���������е�waited-for�����������û�̬���е�ʱ�䣬��λΪjiffies
	uint64_t	m_wcktime; //�ۼƵĸ���������е�waited-for���������ں���̬���е�ʱ�䣬��λΪjiffies
	uint64_t	m_vmsize; // ��page��   �����ڴ��С��
	uint64_t	m_vmrss; // (page) ������ǰפ�������ַ�ռ�Ĵ�С
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