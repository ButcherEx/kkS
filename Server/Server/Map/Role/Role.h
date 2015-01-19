#ifndef __ROLE_H__
#define  __ROLE_H__
#include "PacketWrapper.h"
class Role
{
public:
	Role(){}
public:
	uint32_t Handle(const CG_LOGIN& rMsg);
};

#endif //__ROLE_H__


