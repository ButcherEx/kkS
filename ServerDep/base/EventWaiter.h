#ifndef WAITER_H
#define WAITER_H
///

#include <PreCompier.h>
#include <EventNotify.h>
BASE_NAME_SPACES
class EventWaiter : private boost::noncopyable
{
public:
	EventWaiter(EventNotifier& nf);
	~EventWaiter( );
public:
	void waitNotifier( );
	void onEnd( );
private:
	EventNotifier& refNotifier_;
	int volatile eventIndex_;
};
BASE_NAME_SPACEE

#endif






