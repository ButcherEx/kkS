/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __TASK_H__
#define __TASK_H__
//////////////////////////////////////////////////////////////////////////
LOG_DECL(ServerTask);
//////////////////////////////////////////////////////////////////////////
class Invoker
{
public:
	enum 
	{
		STATE_IDLE,
		STATE_READY,
		STATE_SCHUDULE,
		STATE_EXECUTE
	};
public:
	Invoker(uint32_t interval );
	virtual ~Invoker();
public:
	virtual void UpdateTimeInfo();
public:
	void			UpdateInvokeTimeLeft(int32_t elapse);
	bool			CanExcuteNow( ) const { return m_InvokeTimeLeft <= 0; }
	void			UpdateSchuduleTime(int32_t elapse) { m_SchuduleTime += elapse; }
	int32_t			GetSchuduleTime( ) const { return m_SchuduleTime; }
	void			UpdateExcuteTime(int32_t elapse) { m_ExcuteTime += elapse; }
	int32_t			GetExcuteTime( ) const { return m_ExcuteTime; }
public:
	void			Invoke();
	virtual uint32_t Do() = 0;
public:
	uint32_t		GetInterval() const		{ return m_Interval; }
	const TimeInfo& GetTimeInfo() const		{ return m_TimeInfo; }
	void			SetState(uint32_t state){ m_State = state; }
	uint32_t		GetState( ) const		{ return m_State; }
protected:
	TimeInfo m_TimeInfo;
	uint32_t m_Interval;
	int32_t	 m_InvokeTimeLeft;
	int32_t	 m_SchuduleTime;
	int32_t	 m_ExcuteTime;
	uint32_t m_State;
};

typedef boost::shared_ptr<Invoker> InvokerPtr;

//////////////////////////////////////////////////////////////////////////
template<class RealTask, int32_t InterVal>
class MakeInvoker : public Invoker
{
public:
	MakeInvoker(RealTask& rTask):Invoker(InterVal), m_rTask(rTask){}
	virtual ~MakeInvoker() {}
public:
	virtual uint32_t Do()
	{
		UpdateTimeInfo( );
		return m_rTask.Tick(m_TimeInfo);
	}

private:
	RealTask& m_rTask;
};


//////////////////////////////////////////////////////////////////////////
class TaskBase
{
public:
	enum State
	{
		TASK_STOP = 0,

		TASK_START,
		TASK_START_EXC,
		TASK_START_OK,

		TASK_LOAD,
		TASK_LOAD_EXC,
		TASK_LOAD_OK,

		TASK_EXECUTE,

		TASK_SHUTDOWN,
		TASK_SHUTDOWN_EXC,
		TASK_SHUTDOWN_OK,

		TASK_FINALSAVE,
		TASK_FINALSAVE_EXC,
		TASK_FINALSAVE_OK,
	};
public:
	TaskBase();
	virtual ~TaskBase();
public:
	virtual uint32_t	Tick(const TimeInfo& rTimeInfo);
	virtual void		Tick_State();
protected:
	virtual	void		Start();
	virtual	void		Load();
	virtual void		Shutdown();
	virtual void		FinalSave();
public:
	virtual void		OnStartOk();
	virtual void		OnLoadOk();
	virtual	void		OnShutdownOk();
	virtual	void		OnFinalSaveOk();
public:
	void				SetState(uint32_t state){ m_State = state; }
	uint32_t			GetState( ) const		{ return m_State; }
private:
	uint32_t m_State;

};
class Task : public TaskBase
{
public:
	Task(){}
	virtual ~Task();
public:
	virtual bool		Init() = 0;
	virtual int32_t		GetTaskID() = 0;
public:
	InvokerPtr			FetchInvoker( );
	void				AddInvoker(InvokerPtr taskPtr);
private:
	TSList<InvokerPtr>	m_InvokerPtrList;
};

typedef boost::shared_ptr<Task> TaskPtr;

//////////////////////////////////////////////////////////////////////////

typedef boost::threadpool::fifo_pool  ThreadPool;
typedef boost::shared_ptr<ThreadPool> ThreadPoolPtr;
class TaskManager
{
public:
	TaskManager();
	~TaskManager();
public:
	bool					Init(int32_t maxTask, int32_t maxThread);
	bool					Register(TaskPtr taskPtr);
	void					Excute();
	void					Exit();
private:
	void					SetAllTaskState(int32_t state);
	void					ExcuteState(int32_t setState, int32_t checkState);
	void					ExcuteAllTaskInit();
	void					ExcuteAllTaskStart();
	void					ExcuteAllTaskLoad();
	void					ExcuteAllTask();
	void					ExcuteAllTaskShutdown();
	void					ExcuteAllTaskFinalSave();
private:
	void					Tick(int32_t elapse);
	void					Tick_Task(int32_t elapse);
	void					Tick_AllInvoker(int32_t elapse);
	void					Tick_Logic(int32_t elapse);
private:
	void					Wait(int32_t sec);
private:
	bool					IsAllTaskInState(int32_t state);
	bool					IsShouldShutdown();
private:
	TimeInfo				m_TimeInfo;
	ThreadPoolPtr			m_ThreadPoolPtr;
	TVector<TaskPtr>		m_TaskPtrVec;
	TList<InvokerPtr>		m_InvokerPtrList;
};

#endif
