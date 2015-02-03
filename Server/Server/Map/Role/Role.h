#ifndef __ROLE_H__
#define  __ROLE_H__
#include "PacketWrapper.h"
class Role
{
public:
	Role(Player& rPlayer);
public:
	uint32_t HandlePacket(const CG_LOGIN& rMsg);
private:
	Player& m_rPlayer;
};

#endif //__ROLE_H__


