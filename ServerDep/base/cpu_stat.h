struct sys_cpu_time
{
    unsigned long long user,old_user;
    unsigned long long nice,old_nice;
    unsigned long long sys,old_sys;
    unsigned long long idle,old_idle;
    unsigned long long wait,old_wait;
    unsigned long long hirq,old_hirq;
    unsigned long long sirq,old_sirq;
};
 
struct sys_uptime
{
    double uptime;
    double idle;
};
 
struct sys_mem_info
{
    unsigned long main_total;
    unsigned long main_free;
    unsigned long main_used;
    unsigned long main_buffers;
    unsigned long main_cached;
    unsigned long swap_total;
    unsigned long swap_free;
    unsigned long swap_used;
    unsigned long swap_cached;
};
 
struct system_stat
{
    sys_cpu_time ct;
    sys_mem_info mi;
    sys_uptime   ut;
};
 
//include some special process's cpu and memory usage information
struct process_stat
{
    char name[16];
    char state;
    int ppid;
    int pgrp;
    int session;
    int tty_nr;
    int tpgid;
    unsigned int flags;
    unsigned long minflt;
    unsigned long cminflt;
    unsigned long majflt;
    unsigned long cmajflt;
    unsigned long utime;
    unsigned long stime;
    long cutime;
    long cstime;
    long priority;
    long nice;
    long threads;
    long iterealvalue;
    unsigned long long starttime;
    unsigned long vsize;
    long rss;
};
 
struct sys_mem_entry
{
    const char *name;
    unsigned long *val;
} ;
 
static const int PROC_STAT   = 0x0001;
static const int PROC_MEM    = 0x0002;
static const int PROC_UPTIME = 0x0004;
static const int PROC_ALL    = PROC_STAT|PROC_MEM|PROC_UPTIME;
 
bool get_system_stat(system_stat& ss,int flag);
 
bool get_system_usage(float& cpu,float& mem);
 
bool get_process_stat(pid_t id,process_stat& ps);
 
bool get_process_usage(pid_t id,float& cpu,float& mem,unsigned long long& uptime);  