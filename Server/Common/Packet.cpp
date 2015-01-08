//#include "stdafx.h"


#include "Packet.h"
#include "GameUtil.h"

Packet::Packet(PBMessage& rMsg, const CHAR* name)
: m_rMessage(rMsg), m_PacketId(MyCRC(name))
{
__ENTER_FUNCTION_FOXNET


__LEAVE_FUNCTION_FOXNET
}

Packet::~Packet( )
{
__ENTER_FUNCTION_FOXNET



__LEAVE_FUNCTION_FOXNET
}

