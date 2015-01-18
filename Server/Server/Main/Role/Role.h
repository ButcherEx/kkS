#ifndef __ROLE_H__
#define  __ROLE_H__

#include "BaseLib.h"
#include "Player.h"
#include "PacketWrapper.h"

class Role
{
public:
	virtual uint32_t Handle(const CG_LOGIN& rMsg);
};

#endif //__ROLE_H__


