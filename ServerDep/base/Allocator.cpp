/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/
#include<Allocator.h>

BASE_NAME_SPACES

#ifdef ALLOCATOR_UNIT_TEST

typedef Allocator<10> MemPool;

AtomicInt32 all;

#define PERTHREAD (100000)
void MyPool(MemPool* pool)
{
	int32_t bytes = 0; uint32_t allbytes = 0;
	for(int32_t i = 0; i < PERTHREAD; i++){
		bytes = 64 * ( 1 << (GetTickCount()%11));
		allbytes += bytes;
		void *p = pool->allocZ(bytes);
		pool->freeZ(p);
	}
	all.addAndGet(allbytes/1024);
}

void SysPool( )
{
	int32_t bytes = 0; uint32_t allbytes = 0;
	for(int32_t i = 0; i < PERTHREAD; i++){
		bytes = 64 * ( 1 << (GetTickCount()%11));
		allbytes += bytes;
		void *p = malloc(bytes);
		free(p);
	}
	all.addAndGet(allbytes/1024);
}

template<typename F>
void Test(F f, const char* name)
{
	boost::timer t;
#define MAXTHREAD (20)
	all.set(0);

	ThreadPool pool(MAXTHREAD);
	for(int32_t i = 0; i < MAXTHREAD; i++)
		pool.schedule(f);

	pool.wait();

	double elase = t.elapsed();
	int32_t kb = (int32_t)all.get();
	double mbs = kb/elase/1024;
	fprintf(stdout, "%s:%0.5f(Sec),%d(Kb),%f(Mb/Sec)\n",
		name, elase, kb, mbs );
}

void allocatorUT(void*)
{
	{
		MemPool allocZ(4*1024*1024, 64);
		Test(boost::bind(&MyPool, &allocZ), "pool");
	}

	{
		Test(boost::bind(&SysPool), "malloc");
	}

}

AUTOTEST_IMP(AllocatorUnit, allocatorUT);

#endif

BASE_NAME_SPACEE

