#ifndef NOTIFY_H
#define NOTIFY_H

#include <Event.h>
BASE_NAME_SPACES
class EventNotifier : private boost::noncopyable
{
public:
	EventNotifier( );
	~EventNotifier( );
public:
	void startWaiter(int32_t lCondition);
	void waitEnd( );
	void onWaiterEnd( );
public:
	Poco::Event& waiterEvent(int32_t n);
private:
	volatile int whichEvent_;
	Poco::Event endEvent_;
	Poco::Event waiterEvent_[2];
	boost::atomic_int32_t condition_;
};
BASE_NAME_SPACEE

#endif

