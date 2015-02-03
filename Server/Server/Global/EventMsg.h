/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __EVENT_MSG_H__
#define __EVENT_MSG_H__
#include "BaseLib.h"

class EventMgr;

class EventMsg
{
public:
	GUID_t	m_Sender__;
	int32_t	m_refCounter__;
public:
	EventMsg():m_Sender__(INVALID_GUID), m_refCounter__(0){}
	virtual ~EventMsg(){}
public:
	virtual void Excute(EventMgr& rEvtMgr) = 0;
	virtual const CHAR* Name() { return "EventMsg"; }
};

//////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<EventMsg> EventMsgPtr;

#define EVENTMSG_DECL_START(EVMSG_IMPL)\
class EVMSG_IMPL : public EventMsg\
{\
public:\
	virtual void Excute(EventMgr& rEvtMgr);\
	virtual const CHAR* Name() { return #EVMSG_IMPL; }\
public:\

#define EVENTMSG_DECL_END(EVMSG_IMPL) };\
	POOL_DECL(EVMSG_IMPL);

#define EVENTMSG_IMPL(EVMSG_IMPL)\
	void EVMSG_IMPL:: Excute(EventMgr& rEvtMgr) { rEvtMgr.Handle(*this); }\
	POOL_IMPL(EVMSG_IMPL);


#endif
