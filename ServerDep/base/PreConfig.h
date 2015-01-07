#ifndef PRECONFIG_H_
#define PRECONFIG_H_

//self-namespace
#define BASE_NAME_SPACES namespace base {
#define BASE_NAME_SPACEE };
#define BASE_NAME_SPACE_USING using namespace base;


// BOOST
#define BOOST_ALL_NO_LIB
#define USE_BOOST_LIGHTWEIGHT_MUTEX
#define BOOST_SPIRIT_THREADSAFE 

#pragma warning(disable:4996)

//self-test
#define SCRIPT_UNIT_TEST
#define TIME_UNIT_TEST
#define EXCEPTION_UNIT_TEST
//#define LOG_UNIT_TEST

// lua
#define LUA_STRING
#define LUA_INT64

//3rd
#define HAVE_LIB_GFLAGS
#define USE_TCMALLOC	
//#define USE_JEMALLOC

//#define X64_PLATFORM

#if defined(__LINUX__)
	#define  HAVE_SYS_UTSNAME_H
#endif

//memory
#ifdef __WINDOWS__
#if defined(USE_JEMALLOC)

void* operator new(size_t size, bool tag);
void operator delete(void* p, bool tag);
void* operator new[](size_t size, bool tag);
void operator delete[](void* p, bool tag);
template<class T> void zdeleter(T*& p){ if(!boost::is_pod<T>::value)p->~T(); je_free(p); p=0;}
#define zmalloc je_malloc	
#define zfree(p) {je_free(p); p=0;}
#define znew	new(true)

#ifdef _DEBUG
#	if defined(_WIN64)
#		pragma comment(lib, "libjemalloc-debug-64")
#	else
#		pragma comment(lib, "libjemalloc-debug")
#	endif
#else
#	if defined(_WIN64)
#		pragma comment(lib, "libjemalloc-64")
#	else
#		pragma comment(lib, "libjemalloc")
#	endif
#endif // #ifdef _DEBUG

#elif defined(USE_TCMALLOC)

#define zmalloc malloc
#define zfree(p) {free(p); p=0;}
#define znew	new
template<class T> void zdeleter(T*& p){ delete p; p=0; }

#ifdef _DEBUG
#	if defined(_WIN64)
#		pragma comment(lib, "libtcmalloc_minimal-debug-64")
#	else
#		pragma comment(lib, "libtcmalloc_minimal-debug")
#	endif
#else
#	if defined(_WIN64)
#		pragma comment(lib, "libtcmalloc_minimal-64")
#	else
#		pragma comment(lib, "libtcmalloc_minimal")
#	endif
#endif // #ifdef _DEBUG

#else

#define zmalloc malloc
#define zfree(p) {free(p); p=0;}
#define znew	new
template<class T> void zdeleter(T*& p){ delete p; p = 0; }


#endif // USE_TCMALLOC
#endif // __WINDOWS__

#endif