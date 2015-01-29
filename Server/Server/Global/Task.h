/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __TASK_H__
#define __TASK_H__
//////////////////////////////////////////////////////////////////////////
class TaskDelegate
{
public:
	enum 
	{
		STATE_READY,
		STATE_SCHUDULE,
		STATE_EXECUTE
	};
public:
	TaskDelegate(uint32_t interval );
	virtual ~TaskDelegate();
public:
	virtual void UpdateTimeInfo();
public:
	void	Excute();
	virtual uint32_t Do() = 0;
public:
	uint32_t		GetInterval() const		{ return m_TaskInterval; }
	const TimeInfo& GetTimeInfo() const		{ return m_TimeInfo; }
	void			SetState(uint32_t state){ m_State = state; }
	uint32_t		GetState( ) const		{ return m_State; }
protected:
	TimeInfo m_TimeInfo;
	uint32_t m_TaskInterval;
	uint32_t m_State;
};

typedef boost::shared_ptr<TaskDelegate> TaskDelegatePtr;

//////////////////////////////////////////////////////////////////////////
template<class RealTask, int32_t InterVal>
class MakeTaskDelegate : public TaskDelegate
{
public:
	MakeTaskDelegate(RealTask& rTask):TaskDelegate(Interval), m_rTask(rTask){}
	virtual ~MakeTaskDelegate() {}
public:
	virtual uint32_t Do()
	{
		UpdateTimeInfo( );
		m_rTask.Tick(m_TimeInfo);
	}

private:
	RealTask& m_rTask;
};


//////////////////////////////////////////////////////////////////////////
class Task
{
public:
	enum State
	{
		TASK_STOP = 0,

		TASK_INIT,
		TASK_INIT_OK,

		TASK_LOAD,
		TASK_LOAD_OK,

		TASK_EXECUTE,

		TASK_SHUTDOWN,
		TASK_SHUTDOWN_OK,

		TASK_SAVE,
		TASK_SAVE_OK,
	};
public:
	Task();
	virtual ~Task();
public:
	virtual bool		Init() = 0;
	virtual int32_t		GetTaskID() = 0;
public:
	virtual uint32_t	Tick(const TimeInfo& rTimeInfo);
	TaskDelegatePtr		FetchTask( );
	void				AddTask(TaskDelegatePtr taskPtr);

public:
	void				SetState(uint32_t state){ m_State = state; }
	uint32_t			GetState( ) const		{ return m_State; }
private:
	uint32_t m_State;
	TSList<TaskDelegatePtr> m_TaskPtrList;
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
	bool					Create(int32_t maxTask, int32_t maxThread);
	bool					Register(TaskPtr taskPtr);
	void					Init();
	void					Excute();
	void					Exit();
private:
	TimeInfo				m_TimeInfo;
	ThreadPoolPtr			m_ThreadPoolPtr;
	TVector<TaskPtr>		m_TaskPtrVec;
	TList<TaskDelegatePtr>	m_TaskDelegatePtrList;
};

extern TaskManager g_TaskManager;
#endif
