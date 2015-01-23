//#include "stdafx.h"


#include "Packet.h"

Packet::Packet(PBMessage& msg, const CHAR* name)
: m_rMsg(msg), m_PacketId(MyCRC(name))
{




}

Packet::~Packet( )
{





}

