static const char* PROC_FILE_STAT    = "/proc/stat";
static const char* PROC_FILE_MEMINFO = "/proc/meminfo";
static const char* PROC_FILE_UPTIME  = "/proc/uptime";
 
static int compare_sys_mem_entry(const void *a, const void *b)
{
    return strcmp(static_cast<const sys_mem_entry*>(a)->name,static_cast<const sys_mem_entry*>(b)->name);
}
 
inline ssize_t file2str(const char* file,char* buf,size_t len,int* pfd=NULL)
{
    int fd = -1;
    if(pfd) fd = *pfd;
 
    if(-1 == fd){
        fd=open(file,O_RDONLY,0);
        if(-1==fd) return -1;
        if(pfd) *pfd = fd;
    }else
        lseek(fd,0,SEEK_SET);
 
    ssize_t ret = read(fd,buf,len-1);
    if(NULL==pfd) close(fd);
    if(ret <= 0) return -1;
    buf[ret] = '\0';
 
    return ret;
}
 
bool get_system_stat(system_stat& ss,int flag)
{
    assert(flag&PROC_ALL);
 
    char buf[2048];
    ssize_t ret;   
 
    if(flag & PROC_STAT){
        static __thread int stat_id = -1;
        ret = file2str(PROC_FILE_STAT,buf,sizeof(buf),&stat_id);
        if (-1==ret) return false;
        sys_cpu_time* ct = &ss.ct;
        if(7!=sscanf(buf,"cpu %Lu %Lu %Lu %Lu %Lu %Lu %Lu",&ct->user,&ct->nice,&ct->sys,&ct->idle,
            &ct->wait,&ct->hirq,&ct->sirq))
            return false;
    }
 
    if(flag & PROC_UPTIME){
        static __thread int uptime_id = -1;
        ret = file2str(PROC_FILE_UPTIME,buf,sizeof(buf),&uptime_id);
        if(-1==ret) return false;
        sys_uptime* ut = &ss.ut;
        if(2!=sscanf(buf,"%lf %lf",&ut->uptime,&ut->idle))
            return false;
    }
 
    if(flag & PROC_MEM){
        static __thread int mem_id = -1;
        ret = file2str(PROC_FILE_MEMINFO,buf,sizeof(buf),&mem_id);
        if(-1==ret) return false;
 
        sys_mem_info *mi = &ss.mi;
        const sys_mem_entry mem_table[] = {
            {"Buffers",      &mi->main_buffers},
            {"Cached",       &mi->main_cached},
            {"MemFree",      &mi->main_free},  
            {"MemTotal",     &mi->main_total}, 
            {"SwapCached",   &mi->swap_cached},
            {"SwapFree",     &mi->swap_free},   
            {"SwapTotal",    &mi->swap_total}
        };
 
        char *beg,*end = buf + ret;
        char *colon,*lf;
        sys_mem_entry key,*sme;
 
        for(beg=buf;beg<end;beg=lf+1){
            colon = strchr(beg,':');
            if(!colon) break;
            *colon++ = '\0';
            lf = strchr(colon,'\n');
            if(!lf) break;
            key.name = beg;
            sme = (sys_mem_entry*)bsearch(&key,mem_table,NUM_ELEMENTS(mem_table),sizeof(sys_mem_entry),compare_sys_mem_entry);
            if(sme) *(sme->val) = ::strtoul(beg=colon,&colon,10);
        }
        mi->main_used = mi->main_total - mi->main_free;
        mi->swap_used = mi->swap_total - mi->swap_free;
    }
 
    return true;
}
 
bool get_system_usage(float& cpu,float& mem)
{
    static __thread system_stat ss = {0};
    if(!get_system_stat(ss,PROC_MEM|PROC_STAT))
        return false;
 
    sys_cpu_time* ct = &ss.ct;
    long long user,nice,sys,idle,wait,hirq,sirq,sum;
    user = ct->user - ct->old_user;
    nice = ct->nice - ct->old_nice;
    sys  = ct->sys - ct->old_sys;
    idle = ct->idle - ct->old_idle;
    if(idle<0) idle = 0;
    wait = ct->wait - ct->old_wait;
    hirq = ct->hirq - ct->old_hirq;
    sirq = ct->sirq - ct->old_sirq;
    ct->old_user = ct->user;
    ct->old_nice = ct->nice;
    ct->old_sys  = ct->sys;
    ct->old_idle = ct->idle;
    ct->old_wait = ct->wait;
    ct->old_hirq = ct->hirq;
    ct->old_sirq = ct->sirq;
 
    sum = user + nice + sys + idle + wait + hirq + sirq;
    if(sum<1) sum = 1;
    cpu = 100.0*(sum - idle)/sum;
 
    sys_mem_info* mi = &ss.mi;
    mem = 100.0*mi->main_used/mi->main_total;
 
    return true;
}
 
bool get_process_stat(pid_t id,process_stat& ps)
{
    char buf[1024],name[64];
 
    sprintf(name,"/proc/%u/stat",id);
    ssize_t ret = file2str(name,buf,sizeof(buf));
    if(-1==ret) return false;
 
    char* beg = strchr(buf,'(');
    if(!beg) return false;
 
    char* end = strchr(++beg,')');
    if(!end) return false;
 
    size_t num = end - beg;
    if(num >= sizeof(name))
        num = sizeof(name) -1;
    memcpy(ps.name,beg,num);
    ps.name[num] = '\0';
 
    return 22/*1+5+1+6+6+1+1+1*/ == sscanf(end+2,
        "%c "
        "%d %d %d %d %d "
        "%u "
        "%lu %lu %lu %lu %lu %lu "
        "%ld %ld %ld %ld %ld %ld "
        "%Lu "
        "%lu "
        "%ld",
        &ps.state, //1
        &ps.ppid,&ps.pgrp,&ps.session,&ps.tty_nr,&ps.tpgid, //5
        &ps.flags,//1
        &ps.minflt,&ps.cminflt,&ps.majflt,&ps.cmajflt,&ps.utime,&ps.stime,//6
        &ps.cutime,&ps.cstime,&ps.priority,&ps.nice,&ps.threads,&ps.iterealvalue, //6
        &ps.starttime,//1
        &ps.vsize,//1
        &ps.rss); //1
}
 
bool get_process_usage(pid_t id,float& cpu,float& mem,unsigned long long& uptime)
{
    system_stat ss;
    if(!get_system_stat(ss,PROC_MEM|PROC_UPTIME))
        return false;
 
    process_stat ps;
    if(!get_process_stat(id,ps))
        return false;
 
    long HZ = sysconf(_SC_CLK_TCK);
    unsigned long long pct = ps.utime+ps.stime;
    uptime = (unsigned long long)ss.ut.uptime - ps.starttime/HZ;
    cpu = uptime ? (100.0*pct/HZ)/uptime : 0.0;
 
    long page_size = sysconf(_SC_PAGESIZE) >> 10;
    mem = 100.0*ps.rss*page_size/ss.mi.main_total;
 
    return true;
}