#include <EventNotify.h>

BASE_NAME_SPACES
///
EventNotifier::EventNotifier( ) : whichEvent_(1)
{

}
///
EventNotifier::~EventNotifier( )
{

}
///
void EventNotifier::startWaiter(int32_t lCondition)
{
	whichEvent_ = ((whichEvent_ == 0) ? 1 : 0);
	condition_.store(lCondition);
	waiterEvent_[whichEvent_].set( );
}
///
void EventNotifier::waitEnd( )
{
	endEvent_.wait( );
	endEvent_.reset( );
	waiterEvent_[whichEvent_].reset( );
}
///
void EventNotifier::onWaiterEnd( )
{
	if( condition_.fetch_sub( 1 ) <= 1 )
		endEvent_.set( );
}
///
Poco::Event& EventNotifier::waiterEvent(int n)
{
	return waiterEvent_[n];
}

BASE_NAME_SPACEE
