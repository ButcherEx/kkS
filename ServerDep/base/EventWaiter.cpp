#include <EventWaiter.h>

BASE_NAME_SPACES
///
EventWaiter::EventWaiter(EventNotifier& nf)
: refNotifier_(nf),eventIndex_(0)
{

}
///
EventWaiter::~EventWaiter( )
{

}
///
void EventWaiter::waitNotifier( )
{
	refNotifier_.waiterEvent(eventIndex_).wait( );
}
///
void EventWaiter::onEnd( )
{
	refNotifier_.onWaiterEnd( );
	eventIndex_ = ( (eventIndex_ == 0) ? 1 : 0 );
}

BASE_NAME_SPACEE
