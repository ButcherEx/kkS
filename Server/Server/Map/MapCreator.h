/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __MAP_CREATOR_H__
#define __MAP_CREATOR_H__

#include "BaseLib.h"
#include "TaskDefine.h"
#include "EventMgr.h"
#include "Task.h"
#include "EventMsg.h"
#include "MapMgr.h"

class MapCreator : public Service
{
public:
	virtual bool		Init();
	virtual void		Tick(const TimeInfo& rTimeInfo);
	virtual int32_t		GetServiceID() { return ServiceDefine::MAP;} 
public:
	virtual	void		Start();
	virtual	void		Load();
	virtual void		Shutdown();
	virtual void		FinalSave();
public:
	void				InitAllMapMgr();
public:
	void				Exit();
private:
	ServiceManager m_MapTaskManager;
	TVector<MapMgrPtr> m_MapMgrVec; 
};

typedef MakeInvoker<MapCreator, 50> MapCreatorInvoker;


#endif

