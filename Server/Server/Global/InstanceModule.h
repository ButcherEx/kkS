/************************************************************************
** ��������ȫ�ֱ����ĳ�ʼ���Լ���������
************************************************************************/


#ifndef __INSTANCE_MODULE_H__
#define __INSTANCE_MODULE_H__

#include "TimeManager.h"
#include "Config.h"

class InstanceManager
{
public:
	bool Init();
	bool Exit();
};

extern InstanceManager g_InstancenManager;

#endif //__INSTANCE_MODULE_H__
