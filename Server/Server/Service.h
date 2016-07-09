#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "BaseLib.h"
//////////////////////////////////////////////////////////////////////////

LOG_DECL(ServiceMgrLog);

//////////////////////////////////////////////////////////////////////////
struct InvokerStatus
{
public:
	enum
	{
		IDLE,
		READY,
		SCHUDULE,
		EXECUTE,
		DIE
	};
};

struct  InvokerType
{
public:
	enum
	{
		ACTIVE,
		PASSIVE,
	};
};

class Invoker
{
public:
	Invoker(uint32 interval, uint32 lifeTime = -1, int32 type = InvokerType::ACTIVE, int32 initState = InvokerStatus::READY);
	virtual ~Invoker();
public:
	virtual void	UpdateTimeInfo();
	void			UpdateInvokeTimeLeft(int32 elapse);
	void			Invoke();
	virtual void	Do() = 0;
public:
	bool			CanExcuteNow() const				{ return m_InvokeTimeLeft <= 0; }
	void			UpdateSchuduleTime(int32 elapse)	{ m_SchuduleTime += elapse; }
	void			ResetIdleTime()						{ m_IdleTime = 0; }
	void			UpdateIdleTime(int32 elapse)		{ m_IdleTime += elapse; }
	int32			GetIdleTime() const					{ return m_IdleTime; }
	int32			GetSchuduleTime() const				{ return m_SchuduleTime; }
	void			UpdateExcuteTime(int32 elapse)		{ m_ExcuteTime += elapse; }
	int32			GetExcuteTime() const				{ return m_ExcuteTime; }
	uint32			GetInterval() const					{ return m_Interval; }
	uint32			GetLifeTime() const					{ return m_LifeTime;  }
	const TimeInfo& GetTimeInfo() const					{ return m_TimeInfo; }
	bool			IsState(uint32 state) const			{ return (m_State == state); }
	void			SetState(uint32 state)				{ m_State = state; }
	uint32			GetState() const					{ return m_State; }
	uint32			GetType() const						{ return m_Type; }
protected:
	TimeInfo		m_TimeInfo;
	uint32			m_Interval;
	uint32			m_LifeTime;
	int32			m_InvokeTimeLeft;
	int32			m_SchuduleTime;
	int32			m_ExcuteTime;
	int32			m_IdleTime;
	uint32			m_State;
	uint32			m_Type;
};
typedef boost::shared_ptr<Invoker> InvokerPtr;

//////////////////////////////////////////////////////////////////////////
template<class InvokerImpl>
class MakeInvoker : public Invoker
{
public:
	MakeInvoker(InvokerImpl& rInvokerImpl, int32 InterVal) :Invoker(InterVal), m_rInvokerImpl(rInvokerImpl){}
	virtual ~MakeInvoker() {}
public:
	virtual void Do()
	{
		UpdateTimeInfo();
		m_rInvokerImpl.Tick(m_TimeInfo);
	}

private:
	InvokerImpl& m_rInvokerImpl;
};


//////////////////////////////////////////////////////////////////////////
struct ServiceStatus
{
public:
	enum State
	{
		STOP = 0,

		START,
		START_EXC,
		START_OK,

		LOAD,
		LOAD_EXC,
		LOAD_OK,

		EXECUTE,

		SHUTDOWN,
		SHUTDOWN_EXC,
		SHUTDOWN_OK,

		FINALSAVE,
		FINALSAVE_EXC,
		FINALSAVE_OK,
	};
};


class Service
{
public:
	virtual bool		Init() = 0;
	virtual int32		GetServiceID() = 0;
public:
	InvokerPtr			FetchInvoker();
	void				AddInvoker(InvokerPtr taskPtr);
public:
	virtual void		Tick();
private:
	virtual void		TickState();
protected:
	virtual	void		Start();
	virtual	void		Load();
	virtual void		Shutdown();
	virtual void		FinalSave();
	virtual void		Stop();
public:
	virtual void		OnStartOk();
	virtual void		OnLoadOk();
	virtual	void		OnShutdownOk();
	virtual	void		OnFinalSaveOk();
public:
	void				SetState(uint32 state){ m_State = state; }
	uint32				GetState() const		{ return m_State; }
	bool				IsState(uint32 state) const { return (m_State == state); }

public:
	Service();
	virtual ~Service();
protected:
	uint32_t m_State;
	TSList<InvokerPtr>	m_InvokerPtrList;
};

typedef boost::shared_ptr<Service> ServicePtr;

//////////////////////////////////////////////////////////////////////////


class Supervisor : public Service
{

public:
	Supervisor();
	virtual ~Supervisor();
protected:
	TList<ServicePtr> m_ServicePtrList;
};
//////////////////////////////////////////////////////////////////////////


typedef boost::threadpool::fifo_pool  ThreadPool;
typedef boost::shared_ptr<ThreadPool> ThreadPoolPtr;

class ServiceMgr
{
public:
	ServiceMgr();
	~ServiceMgr();
public:
	bool					Init(int32 maxTask, int32 maxThread);
	bool					Register(ServicePtr taskPtr);
	void					Excute();
	void					Exit();
private:
	void					SetAllInvokerState_MainThread(int32 state);
	void					SetAllServiceState(int32 state);
	void					ExcuteState(int32 setState, int32 checkState);
	void					ExcuteAllServiceInit();
	void					ExcuteAllServiceStart();
	void					ExcuteAllServiceLoad();
	void					ExcuteAllService();
	void					ExcuteAllServiceShutdown();
	void					ExcuteAllServiceFinalSave();
private:
	void					Tick(int32 elapse);
	void					Tick_Service(int32 elapse);
	void					Tick_AllInvoker(int32 elapse);
	void					Tick_Logic(int32 elapse);
private:
	void					Wait(int32 sec);
private:
	bool					IsAllInvokerInState(int32 state);
	bool					IsAllTaskInState(int32 state);
	bool					IsShouldShutdown();
private:
	TimeInfo				m_TimeInfo;
	ThreadPoolPtr			m_ThreadPoolPtr;
	TVector<ServicePtr>		m_ServicePtrVec;
	TList<InvokerPtr>		m_InvokerPtrList;
};




#endif