#ifndef __EVENT_MSG_TEST_H__
#define __EVENT_MSG_TEST_H__

#include "EventMsg.h"

EVENTMSG_DECL_START(JoinWolrd)
	JoinWolrd(){ strncpy(m_Message, "HelloWorld", 32); }
	int32_t m_Coin;
	CHAR m_Message[32];
EVENTMSG_DECL_END(JoinWolrd)



EVENTMSG_DECL_START(UnHandledMsg)
	int32_t m_None;
EVENTMSG_DECL_END(UnHandledMsg)


#endif

