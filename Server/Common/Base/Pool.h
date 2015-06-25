#ifndef __POOL_H__
#define __POOL_H__

#include "BaseType.h"

class IPool
{
public:
	virtual void ActiveRecyle( ) = 0;
};

template
<
	typename ObjectType,
	int32_t _RecyleInterval = 32,
	int32_t _AllocGranularity = 1,
	int32_t _PoolMaxSize = -1
>
class ObjectPool : public IPool
{
public:
	typedef boost::shared_ptr<ObjectType> ObjectPtr;
	typedef std::pair<ObjectPtr, bool> PtrElem;
	typedef TVector<PtrElem> _ElemCont;
	typedef TList<int32_t> _ElemRef;
private:
	MyLock m_Lock;
	int32_t m_LastRecyleIndex;
	_ElemRef m_FreeList;
	_ElemCont m_ObjectPtrs;
public:
	explicit ObjectPool<ObjectType, _RecyleInterval, _AllocGranularity, _PoolMaxSize>()
	{

	}
public:
	ObjectPtr NewObj()
	{
		__ENTER_FUNCTION_EX

			AutoLock_T Lock(m_Lock);
		if( m_FreeList.Empty() )
		{
			Recyle();
		}

		if( m_FreeList.Empty() )
		{
			Allocate();
		}

		Assert(!m_FreeList.Empty());

		int32_t freeIdx = -1;
		m_FreeList.PopFront(freeIdx);

		Assert((freeIdx >=0 && freeIdx < (int32_t)m_ObjectPtrs.size()));
		Assert(m_ObjectPtrs[freeIdx].first.unique());
		Assert(m_ObjectPtrs[freeIdx].second);

		m_ObjectPtrs[freeIdx].second = false;
		ObjectPtr ptr = m_ObjectPtrs[freeIdx].first;
		return ptr;
		
		__LEAVE_FUNCTION_EX

		ObjectPtr ptr(new ObjectType());
		return ptr;
	}

	void ActiveRecyle()
	{
		__ENTER_FUNCTION_EX

			AutoLock_T Lock(m_Lock);

			const int32_t ObjectSize = (int32_t)m_ObjectPtrs.size();
			for( int32_t i = 0; i < ObjectSize; i++)
			{
				if(!m_ObjectPtrs[i].second)
				{
					if(m_ObjectPtrs[i].first.unique())
					{
						m_ObjectPtrs[i].first->ObjectType::~ObjectType();
						new(m_ObjectPtrs[i].first.get()) ObjectType();

						m_ObjectPtrs[i].second = true;
						m_FreeList.PushBack(i);
					}
				}
			}
		__LEAVE_FUNCTION_EX
	}

private:
	void Recyle( )
	{
		__ENTER_FUNCTION

			const int32_t ObjectSize = (int32_t)m_ObjectPtrs.size();
		for( int32_t idx = 0; idx < ObjectSize; idx++)
		{
			Assert( m_LastRecyleIndex>=0 && m_LastRecyleIndex < (int32_t)m_ObjectPtrs.size() );
			if( m_ObjectPtrs[m_LastRecyleIndex].first.unique())
			{
				m_ObjectPtrs[m_LastRecyleIndex].first->ObjectType::~ObjectType();
				new(m_ObjectPtrs[m_LastRecyleIndex].first.get()) ObjectType();

				m_ObjectPtrs[m_LastRecyleIndex].second = true;
				m_FreeList.PushBack(m_LastRecyleIndex);

				m_LastRecyleIndex = (m_LastRecyleIndex+1) % ObjectSize;

				if( m_FreeList.Size() > _RecyleInterval)
				{
					break;
				}
			}
			else
			{
				m_LastRecyleIndex = (m_LastRecyleIndex+1) % ObjectSize;
			}
		}


		__LEAVE_FUNCTION
	}

	void Allocate()
	{
		__ENTER_FUNCTION
			for(int32_t i  = 0; i < _AllocGranularity; i++)
			{
				ObjectPtr ptr(new ObjectType());
				m_ObjectPtrs.push_back(std::make_pair(ptr, true));
				m_FreeList.PushBack((int32_t)m_ObjectPtrs.size() - 1);
			}

		__LEAVE_FUNCTION
	}
};

#define __POOL_INST(OBJECTTYPE) g##OBJECTTYPE##Pool

#define POOL_DECL(OBJECTTYPE)\
	extern ObjectPool<OBJECTTYPE> __POOL_INST(OBJECTTYPE);\
	typedef ObjectPool<OBJECTTYPE>::ObjectPtr OBJECTTYPE##Ptr

#define POOL_IMPL(OBJECTTYPE)\
	ObjectPool<OBJECTTYPE> __POOL_INST(OBJECTTYPE)

#define POOL_NEW(OBJECTTYPE)\
	__POOL_INST(OBJECTTYPE).NewObj()

#endif