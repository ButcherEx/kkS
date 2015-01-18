//#include "stdafx.h"


#include "Packet.h"
#include "GameUtil.h"

Packet::Packet(PBMessage& msg, const CHAR* name)
: m_rMsg(msg), m_PacketId(MyCRC(name))
{
__ENTER_FUNCTION_FOXNET


__LEAVE_FUNCTION_FOXNET
}

Packet::~Packet( )
{
__ENTER_FUNCTION_FOXNET



__LEAVE_FUNCTION_FOXNET
}

