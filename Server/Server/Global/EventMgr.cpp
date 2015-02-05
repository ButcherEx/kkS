#include "EventMgr.h"
#include "LogDefine.h"

EventMgr::EventMgr()
{

}

EventMgr::~EventMgr()
{

}

bool EventMgr::Init()
{
	return true;
}


void EventMgr::Update()
{
	EventMsgPtr Ptr;
	while ( m_listEventMsg.PopFront(Ptr))
	{
		Ptr->Excute(*this);
	}
}

void EventMgr::AddEvent(GUID_t sender, EventMsgPtr Ptr)
{
	__ENTER_FUNCTION

	Assert(Ptr);
	Assert(Ptr->m_refCounter__ == 0);
	Ptr->m_refCounter__++;
	Ptr->m_Sender__ = sender;

	m_listEventMsg.PushBack(Ptr);

	__LEAVE_FUNCTION
}

void EventMgr::Handle(EventMsg& rMsg)
{
	LOG_ERROR(ServerError, "[EventMsg]unhandled eventmsg:%s", rMsg.Name() );
}