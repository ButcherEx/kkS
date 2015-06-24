##ifndef __LIBMEMINTERFACE_H__
#define __LIBMEMINTERFACE_H__

#include "Base.h"
#include "LibMemcachedRef.h"

class LibMemInterface
{
public:
	enum LIBMEMCONSTVAL
	{
		MAXSERVERLISTLENTH = 1024
	};

public:
	LibMemInterface();
	~LibMemInterface();

public:
	bool Init(const CHAR* serveList);

	memcahed_st *GetImpl() { return m_MemC; }
	const memcahed_st *GetImpl() const { return m_MemC; }

	const CHAR* GetServerList( ) const { return m_ServerList; }


	bool SetBehavior(const memcached_behavior_t flag, uint64_t data);
	uint64_t GetBehavior( const memcached_behavior_t flag );

	const CHAR* GetError(memcached_return_t rc) const;

	CHAR* Get(const CHAR* key, uint32_t key_len, size_t *value_len);
	CHAR* Get(const CHAR* master_key, uint32_t master_key_len, const CHAR* key, uint32_t key_len, size_t *value_len);
	bool  Set(const CHAR* key, uint32_t key_len, const CHAR* value, uint32_t value_len, time_t expiration);
	bool  Set(const CHAR* master_key, uint32_t master_key_len,const CHAR* key, uint32_t key_len, const CHAR* value, uint32_t value_len, time_t expiration);
	bool  Del(const CHAR* key, uint32_t key_len, time_t expiration);
private:
	memcached_st m_MemC;
	CHAR m_ServerList[MAXSERVERLISTLENTH+1];
};

#endif // __LIBMEMINTERFACE_H__
