/************************************************************************/
/*                                                                      */
/************************************************************************/

#include "BaseLib.h"
#include "Task.h"
#include "EventMgr.h"
#include "EventMsg.h"

class MapCreator : public Task, public EventMgr
{
public:
	bool		Init();
	void		Excute();
	void		Exit();
	int32_t		GetTaskID();
private:
	TaskManager m_MapTaskManager;
};

