/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __TASK_H__
#define __TASK_H__

#include "BaseLib.h"

class TaskDelegate
{
public:
	TaskDelegate(uint32_t interval );
	virtual ~TaskDelegate();
public:
	virtual void UpdateTimeInfo();
public:
	virtual uint32_t Excute() = 0;
protected:
	TimeInfo m_TimeInfo;
	uint32_t m_TaskInterval;
};

typedef boost::shared_ptr<TaskDelegate> TaskDeletePtr;

template<class RealTask, int32_t InterVal>
class MakeTaskDelegate : public TaskDelegate
{
public:
	MakeTaskDelegate(RealTask& rTask):TaskDelegate(Interval), m_rTask(rTask){}
	virtual ~MakeTaskDelegate() {}
public:
	virtual uint32_t Excute()
	{
		m_rTask.Tick(m_TimeInfo);
	}

private:
	RealTask& m_rTask;
};

class Task
{
public:
	enum State
	{
		TASK_STOP	= 0,
		TASK_INIT,
		TASK_INIT_OK,
		TASK_LOAD,
		TASK_LOAD_OK,
		TASK_RUN,
		TASK_SHUTDOWN,
		TASK_SHUTDOWN_OK,
	};
public:
	Task();
	virtual ~Task();
public:
	virtual uint32_t Tick(const TimeInfo& rTimeInfo);
	virtual TaskDeletePtr Init();
public:
	TaskDeletePtr FetchTask( );
	void	AddTask(TaskDeletePtr taskPtr);
private:
	TSList<TaskDeletePtr> m_TaskPtrList;
};

class TaskManager
{
public:
	TaskManager();
	~TaskManager();
public:
	bool Init();
	void Exit();
	void Tick();
private:
	TList<TaskDeletePtr> m_TaskPtrList;

	typedef boost::threadpool::fifo_pool  ThreadPool;
	typedef boost::shared_ptr<ThreadPool> ThreadPoolPtr;

	ThreadPoolPtr m_ThreadPoolPtr;
};

#endif
