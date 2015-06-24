
##ifndef __LIBMEMCACHEDREF_H__
#define __LIBMEMCACHEDREF_H__


#if defined(__WINDOWS__)

#include "windows/libmemcached/memcached.h"
#include "windows/libhashkit/hashkit.h"

#elif defined (__LINUX__)

#include "linux/libmemcached/memcached.h"

#else

#error windows or linux is required.

#endif


#endif
