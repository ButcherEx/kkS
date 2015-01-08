#include "BaseLib.h"

// boost::threadpool::fifo_pool thread_pool(10);
// 
// 
// void HellWorld()
// {
// 	printf("%s","boost::threadpool::fifo_pool.\n");
// }
// 
// struct ThreadPoolAutoT
// {
// 	ThreadPoolAutoT()
// 	{
// 		for(int32 i = 0; i < 100; i++)
// 		{
// 			thread_pool.schedule(&HellWorld);
// 		}
// 	}
// }__threadpool_t;

class StaticObject
{
public:
	StaticObject()
	{

	}

	~StaticObject()
	{

	}

	void Say()
	{
		printf("%s", "PoolObject Test.\n");
	}
};

POOL_DECL(StaticObject);
POOL_IMPL(StaticObject);


struct PoolTest
{
public:
	PoolTest()
	{
		const CHAR* message = "CG_ASK_LOGIN";
		uint32_t Id1 = MyCRC(message); // 0,2589650335 1,1179716420 2,3115250875
		uint32_t Id2 = MyCRC32(message); // 882901144
	//	uint32_t Id3 = MyCRC32_1(message); // 3822032882
		bool isEqual = (Id1 == Id2);
		Signle();
		Signle();
		Signle();
	}

	void Signle()
	{
		StaticObjectPtr object = POOL_NEW(StaticObject);
		object->Say();
	}
}__autoPool;



