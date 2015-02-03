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
	Assert(Ptr->m_refCounter_ == 0);
	Ptr->m_refCounter_++;
	Ptr->m_Sender = sender;

	m_listEventMsg.PushBack(Ptr);

	__LEAVE_FUNCTION
}

void EventMgr::Handle(EventMsg& rMsg)
{
	LOGE(ServerError, "[EventMsg]unhandled eventmsg:%s", rMsg.Name() );
}