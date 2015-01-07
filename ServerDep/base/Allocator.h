/*******************************************************************************

Copyright 2010 by backkom. All rights reserved.
This software is the confidential and proprietary information of backkom.
('Confidential Information'). You shall not disclose such Confidential -
Information and shall use it only in accordance with the terms of the - 
license agreement you entered into with backkom.

*******************************************************************************/

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <PreCompier.h>

BASE_NAME_SPACES

/* memory mark */
typedef struct MemNode {
	int32_t hits;
	int32_t size;
    struct MemNode *next;
} MemNode;
#define MNODE_SIZE			(sizeof(MemNode))
#define MNODECASTO(node)	((MemNode*)node+1)
#define MNODECASFROM(node)	((MemNode*)node-1)

/* memory leak and overwriting  mark */
typedef struct MemDBGHead {
	int32_t size;
    int32_t fLine;
	const char* fName;
} MemDBGHead;
#define DBGOVERFLOW_KEY		(0x7eadbeef)
#define DBGOVERFLOW			(sizeof(int32_t))
#define DBGHEAD_SIZE		(sizeof(MemDBGHead))
#define DBGHEADEX_SIZE		(DBGOVERFLOW+DBGHEAD_SIZE)
#define DBGHEADCASTO(node)	((CHAR*)node+ DBGHEADEX_SIZE)
#define DBGHEADCASTFROM(p)	((MemDBGHead*)((CHAR*)p - DBGHEADEX_SIZE))

/**/
template
<
	int32_t FLAG
>
class PolicyAlignPow2
{
public:
	static int32_t align( int32_t n, int32_t min)
	{
		int32_t max = min * (1<<FLAG);
		if( n <= min ) return min;
		if( n >= max ) return Util::align(n, 16);
		return Util::nextPow2(n);
	}

	static int32_t index(int32_t n, int32_t min)
	{
		int32_t idx = 0,  max = min * (1<<FLAG);
		if( n <= min ){
			idx = 0;
		}else if ( n > max ){
			idx = -1;
		}else {
			n = Util::nextPow2(n)/min;
			while ((n = (n >> 1))) ++idx;
		}
		return idx;
	}
};

template
<
	int32_t FLAG
>
class PolicyAlignSize
{
public:
	static int32_t align(int32_t n, int32_t min)
	{
		int32_t max = min * FLAG;
		if( n <= min ) return min;
		if( n >= max ) return Util::align(n, min);
		return Util::align(n, min);
	}

	static int32_t index(int32_t n, int32_t min)
	{
		int32_t max = min * FLAG;
		if( n <= min ) return 0;
		if( n >  max ) return -1;
		return ((Util::align(n, min) / min ) - 1);
	}
};



/* memory pool */
template
<
	int32_t INDEX = 1,
	typename PolicyAlign = PolicyAlignSize<INDEX>
>
class Allocator : public boost::noncopyable, public boost::enable_shared_from_this< Allocator<INDEX> >
{
	const static int32_t POOL_INDEX = INDEX;
public:
	/** @size memory size */
	void* allocZ(int32_t size)
	{
		MemNode *node;
		int32_t i,  
			newSize = PolicyAlign::align(size, reqUnitSize_),
			j = PolicyAlign::index(newSize, reqUnitSize_);
		if( j >=0 ) {
			lock( );
			if( nodeList_[j].next ){
				node = nodeList_[j].next;
				node->hits += 1;
				nodeList_[j].next = node->next;
				cachedSize_ -= node->size;
				nodeList_[j].hits += 1;
				unlock( );
				return MNODECASTO(node);
			}
			unlock( );
		}

		node = (MemNode*)_sysAlloc(MNODE_SIZE+newSize);
		if(!node) {
			if( j >= 0 ) {
				/** we try to alloc from the larger block */
				lock( );
				for( i = j ; i < POOL_INDEX; i++ ) {
					node = nodeList_[i].next;
					if( nodeList_[i].next ){
						node = nodeList_[i].next;
						node->hits += 1;
						nodeList_[i].next = node->next;
						cachedSize_ -= node->size;
						unlock( );
						return MNODECASTO(node);
					}
				}

				unlock( );
			}

			/** we feel so helpless !*/
			return NULL;
		}

		node->next = 0;
		node->hits = 0;
		node->size = newSize;

		return MNODECASTO(node);
	}
	void* tryAllocZ(int32_t size)
	{
		MemNode *node;
		int32_t i,  
			newSize = PolicyAlign::align(size, reqUnitSize_),
			j = PolicyAlign::index(newSize, reqUnitSize_);

		if( j >= 0 ){
			if( !tryLock( ) ) {
				return NULL;
			}

			if( nodeList_[j].next ) {
				node = nodeList_[j].next;
				node->hits += 1;
				nodeList_[j].next = node->next;
				cachedSize_ -= node->size;
				nodeList_[j].hits += 1;
				unlock( );
				return MNODECASTO(node);
			}
			unlock( );
		} 

		node = (MemNode*)_sysAlloc(MNODE_SIZE+newSize);
		if(!node) {
			if( j >= 0 ) {
				/** we try to alloc from the larger block */
				if( !tryLock( ) ) {
					return NULL;
				}

				for( i = j ; i < POOL_INDEX; i++ ) {
					node = nodeList_[i].next;
					if( nodeList_[i].next ) {
						node = nodeList_[i].next;
						node->hits += 1;
						nodeList_[i].next = node->next;
						cachedSize_ -= node->size;
						unlock( );
						return MNODECASTO(node);
					}
				}

				unlock( );
			}
			/** we feel so helpless !*/
			return NULL;
		}

		node->next = 0;
		node->hits = 0;
		node->size = newSize;

		return MNODECASTO(node);
	}
	void  freeZ(void *p)
	{
		MemNode *node = MNODECASFROM(p);
		int32_t x, hits, i, j = PolicyAlign::index(node->size, reqUnitSize_);

		if( j >= 0 ) {
			lock( );

			hits = node->hits;
			x = cachedSize_ < cachedSizeLimit_ ?
				0 : (node->size * 2);

			if( x > 0 && hits ) {
				MemNode *prev;
				for(i = POOL_INDEX - 1;
					i >= 0 && x > 0;
					i --){
						prev = &nodeList_[i];
						node = nodeList_[i].next;
						for( ; x > 0 && node; ){
							if(node->hits < hits){
								prev->next = node->next;
								x -= node->size;
								_sysFree(node);
								node = prev->next;
							} else {
								prev = node;
								node = node->next;
							}
						}
				}
			}

			if( cachedSizeLimit_ > cachedSize_ ) {
				if( j >= 0 && j < POOL_INDEX) {
					node->next = nodeList_[j].next;
					nodeList_[j].next = node;
					cachedSize_ += node->size;
					unlock( );
					return ;
				}
			}

			unlock( );
		}

		_sysFree(MNODECASFROM(p));
	}
	/* debug head in memory's head */
	void* allocZD(int32_t size, const char *func, int32_t line)
	{
		int32_t newSize = size + DBGHEAD_SIZE + DBGOVERFLOW * 2;
		MemDBGHead *pDBGHead = (MemDBGHead*)allocZ(newSize);
		if(!pDBGHead) return NULL;

		pDBGHead->size = size;
		pDBGHead->fLine = line;
		pDBGHead->fName = func;
		*(int32_t*)((CHAR*)pDBGHead + DBGHEAD_SIZE) = DBGOVERFLOW_KEY;
		*(int32_t*)(DBGHEADCASTO(pDBGHead) + size) = DBGOVERFLOW_KEY;
		return DBGHEADCASTO(pDBGHead);
	}
	void* tryAllocZD(int32_t size, const char *func, int32_t line)
	{
		int32_t newSize = size + DBGHEAD_SIZE+ DBGOVERFLOW * 2;
		MemDBGHead *pDBGHead = (MemDBGHead*)tryAllocZ(newSize);
		if(!pDBGHead) return NULL;

		pDBGHead->size = size;
		pDBGHead->fLine = line;
		pDBGHead->fName = func;

		*(int32_t*)((CHAR*)pDBGHead + DBGHEAD_SIZE) = DBGOVERFLOW_KEY;
		*(int32_t*)(DBGHEADCASTO(pDBGHead) + size) = DBGOVERFLOW_KEY;

		return DBGHEADCASTO(pDBGHead);
	}
	void  freeZD(void *p)
	{
		MemDBGHead *pDBGHead = DBGHEADCASTFROM(p);

		AssertIfLog(((*(int32_t*)((CHAR*)pDBGHead + DBGHEAD_SIZE))) == DBGOVERFLOW_KEY, "");
		AssertIfLog(((*(int32_t*)(DBGHEADCASTO(pDBGHead) + pDBGHead->size))), "");

		freeZ(pDBGHead);
	}
public:
	Allocator(int32_t poolSize, int32_t unit = 64)
	{
		cachedSize_ = 0;
		reqUnitSize_ = Util::nextPow2(unit);
		cachedSizeLimit_ = poolSize;
		memset(nodeList_, 0, sizeof(nodeList_));
	}
	~Allocator( )
	{
		int32_t i;
		MemNode *node;

		for( i = 0; i < POOL_INDEX; i++)
		{
			while(node = nodeList_[i].next)
			{
				nodeList_[i].next = node->next;
				_sysFree(node);
			}
		}
	}
private:
	int32_t cachedSize_;
    int32_t cachedSizeLimit_;
	int32_t reqUnitSize_;
	AtomicInt32 busyLock_;
    
    MemNode nodeList_[POOL_INDEX];

	void lock( )	{ while( busyLock_.compareAndSet(0, 1) != 0) Util::thisThreadSleep(0);  }
	void unlock( )	{ busyLock_.set( 0 ); }
	bool tryLock( ) { return !busyLock_.compareAndSet(0, 1); }
 };

/** allocate(deallocate) memory from system heap */
inline void* _sysAlloc( int32_t size ){ return calloc(1, size); }
inline void  _sysFree( void *p ) { if( p )free(p); }

#ifdef ALLOCATOR_UNIT_TEST
AUTOTEST_DEF(AllocatorUnit);
#endif

BASE_NAME_SPACEE


#endif /*ALLOCATOR_H*/

