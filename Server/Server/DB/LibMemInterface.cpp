#include "LibMemInterface.h"

LibMemInterface::LibMemInterface()
{
	memcached_create(&m_MemC);
}

LibMemInterface::~LibMemInterface()
{
	memcached_free(&m_MemC);
}

bool LibMemInterface::Init(const CHAR* serveList)
{
	__TRY
	{
		strncpy(m_ServerList, serveList, MAXSERVERLISTLENTH);
		m_ServerList[MAXSERVERLISTLENTH-1] = 0;

		memcached_server_st *servers = memcached_servers_parse(m_ServerList);
		memcached_server_push(&m_MemC, servers);
		memcached_server_free(servers);

		return true;
	}
	__CATCH
	{
		return false;
	}

	return false;
}

bool LibMemInterface::SetBehavior(const memcached_behavior_t flag, uint64_t data)
{
	memcached_return_t rc = memcached_behavior_set(&m_MemC, flag, data);
	return (rc == MEMCACHED_SUCCESS);
}
uint64_t LibMemInterface::GetBehavior( const memcached_behavior_t flag )
{
	return memcached_behavior_get(&m_MemC, flag);
}

const CHAR* LibMemInterface::GetError(memcached_return_t rc) const
{
	return memcached_strerror(NULL, rc);
}

CHAR* LibMemInterface::Get(const CHAR* key, uint32_t key_len, size_t *value_len)
{
	if( NULL == key)
	{
		return NULL;
	}

	uint32_t flags;
	memcached_return_t rc;
	CHAR* values = memcached_get(&m_MemC, key, key_len, value_len, &flags, &rc);
	if( MEMCACHED_SUCCESS != rc)
	{
		//..log
		if(NULL != values)
		{
			::free(values);
		}
		return NULL;
	}

	if(NULL == values)
	{
		log
	}

	return values;
}
CHAR* LibMemInterface::Get(const CHAR* master_key, uint32_t master_key_len, const CHAR* key, uint32_t key_len, size_t *value_len)
{
	if( NULL == key || NULL == master_key)
	{
		return NULL;
	}

	uint32_t flags;
	memcached_return_t rc;
	CHAR* values = memcached_get_by_key(&m_MemC, master_key, master_key_len, key, key_len, value_len, &flags, &rc);
	if( MEMCACHED_SUCCESS != rc)
	{
		//..log
		if(NULL != values)
		{
			::free(values);
		}
		return NULL;
	}

	if(NULL == values)
	{
		log
	}

	return values;
}
bool  LibMemInterface::Set(const CHAR* key, uint32_t key_len, const CHAR* values, uint32_t value_len, time_t expiration)
{
	if( NULL == KEY || NULL == values)
	{
		//.. log
		return false;
	}

	uint32_t flags = 0;
	memcached_return_t rc = memcached_set(&m_MemC, key, key_len, values, value_len, expiration, flags);
	
	bool retVal = (rc == MEMCACHED_SUCCESS || rc == MEMCACHED_BUFFERED);
	if( !retVal )
	{
		//..
	}

	return retVal;
}
bool  LibMemInterface::Set(const CHAR* master_key, uint32_t master_key_len,const CHAR* key, uint32_t key_len, const CHAR* values, uint32_t value_len, time_t expiration)
{
	if(NULL == master_key || NULL == KEY || NULL == values)
	{
		//.. log
		return false;
	}

	uint32_t flags = 0;
	memcached_return_t rc = memcached_set_by_key(&m_MemC, master_key, master_key_len, key, key_len, values, value_len, expiration, flags);

	bool retVal = (rc == MEMCACHED_SUCCESS || rc == MEMCACHED_BUFFERED);
	if( !retVal )
	{
		//..
	}

	return retVal;
}
bool  LibMemInterface::Del(const CHAR* key, uint32_t key_len, time_t expiration)
{
	if( NULL == KEY )
	{
		//.. log
		return false;
	}

	uint32_t flags = 0;
	memcached_return_t rc = memcached_delete(&m_MemC, key, key_len, expiration);

	bool retVal = (rc == MEMCACHED_SUCCESS);
	if( !retVal )
	{
		//..
	}

	return retVal;
}


