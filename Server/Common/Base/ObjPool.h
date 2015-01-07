// ObjPool.h
//
/////////////////////////////////////////////////////

#ifndef __OBJPOOL_H__
#define __OBJPOOL_H__

#include "Base.h"

template<class T>
class ObjPool
{
public:
	ObjPool( void )
	{
	__ENTER_FUNCTION

		m_papObj	= NULL;
		m_nMaxCount		= -1;
		m_nPosition		= -1;

		//m_Lock;
	__LEAVE_FUNCTION
	}

	~ObjPool( void )
	{
	__ENTER_FUNCTION

		Term() ;

		Assert( m_papObj == NULL );

	__LEAVE_FUNCTION
	}

	bool Init( int32_t nMaxCount )
	{
	__ENTER_FUNCTION

		Assert( nMaxCount > 0 );
		if ( nMaxCount <= 0 )
			return false;

		m_nMaxCount		= nMaxCount;
		m_nPosition		= 0;
		m_papObj	= new T* [m_nMaxCount];

		int32_t i;
		for( i = 0; i < m_nMaxCount; i++ )
		{
			m_papObj[i] = new T;
			if ( m_papObj[i] == NULL )
			{
				Assert( m_papObj[i] != NULL );
				return false;
			}
		}
		return true;

	__LEAVE_FUNCTION

		return false;
	}

	void Term( void )
	{
	__ENTER_FUNCTION

		if ( m_papObj != NULL )
		{
			int32_t i;
			for ( i = 0; i < m_nMaxCount; i++ )
			{
				SAFE_DELETE( m_papObj[i] );
			}

			delete [] m_papObj;
			m_papObj = NULL;
		}

		m_nMaxCount		= -1;
		m_nPosition		= -1;

	__LEAVE_FUNCTION
	}

	T* NewObj( void )
	{
	__ENTER_FUNCTION

		m_Lock.Lock();
		Assert( m_nPosition < m_nMaxCount );
		if ( m_nPosition >= m_nMaxCount )
		{
			m_Lock.Unlock();
			return NULL;
		}

		T *pObj = m_papObj[m_nPosition];
		pObj->SetPoolID( (uint32_t)m_nPosition );
		m_nPosition++;

		m_Lock.Unlock();
		return pObj;

	__LEAVE_FUNCTION

		m_Lock.Unlock();
		return NULL;
	}

	void DeleteObj( T *pObj )
	{
	__ENTER_FUNCTION

		m_Lock.Lock();
		Assert( pObj != NULL );	
		if ( pObj == NULL )
		{
			m_Lock.Unlock();
			return ;
		}

		Assert( m_nPosition > 0 );	
		if ( m_nPosition <= 0 )
		{
			m_Lock.Unlock();
			return ;
		}

		uint32_t uDelIndex = pObj->GetPoolID();
		Assert(uDelIndex < (uint32_t)m_nPosition );
		if (uDelIndex >= (uint32_t)m_nPosition )
		{
			m_Lock.Unlock();
			return ;
		}


		m_nPosition--;
		T *pDelObj			= m_papObj[uDelIndex];
		m_papObj[uDelIndex]	= m_papObj[m_nPosition];
		m_papObj[m_nPosition]	= pDelObj;

		m_papObj[uDelIndex]->SetPoolID(uDelIndex );
		m_papObj[m_nPosition]->SetPoolID( INVALID_ID );

		m_Lock.Unlock();
		return;

	__LEAVE_FUNCTION

		m_Lock.Unlock();
		return;
	}

	int32_t GetCount( void )const
	{
	__ENTER_FUNCTION

		return m_nPosition;

	__LEAVE_FUNCTION

		return -1;
	}

private:
	T				**m_papObj;
	int32_t				m_nMaxCount;
	int32_t				m_nPosition;

	MyLock			m_Lock;
};

#endif
