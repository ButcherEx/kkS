#ifndef PRECOMPIER_H
#define PRECOMPIER_H

#include <PreConfig.h>
#if defined(USE_JEMALLOC)
#include <jemalloc/jemalloc.h>
#endif

//..
#include <string>
#include <algorithm>
#include <limits>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <io.h>
#include <thr/xtimec.h>

// #pragma warning(push)
// #pragma warning(disable:4267)
//#pragma warning(disable:4250)

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/smart_ptr.hpp>
#if defined(USE_BOOST_LIGHTWEIGHT_MUTEX)
#include <boost/smart_ptr/detail/lightweight_mutex.hpp>
#endif
#include <boost/any.hpp>
#include <boost/atomic.hpp>
#include <boost/array.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/list.hpp>
#include <boost/container/map.hpp>
#include <boost/container/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/pool/pool.hpp>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>
#include <boost/progress.hpp>
#include <boost/threadpool.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/filesystem.hpp>
#pragma warning(disable:4250)
#include <boost/filesystem/fstream.hpp>
#pragma warning(default:4250)
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>


#if defined(__WINDOWS__)
//typedef boost::detail::lightweight_mutex Mutex;
//typedef boost::detail::lightweight_mutex::scoped_lock ScopedLock;
class _lightweight_mutex : public boost::noncopyable
{
private:
	CRITICAL_SECTION cs_;
public:
	_lightweight_mutex() { ::InitializeCriticalSectionAndSpinCount(&cs_, 4000); }
	~_lightweight_mutex() { ::DeleteCriticalSection(&cs_); }
	void lock( ) { ::EnterCriticalSection(&cs_);}
	void unlock() { ::LeaveCriticalSection(&cs_);}
	bool try_lock(){ return ::TryEnterCriticalSection(&cs_) == TRUE; }
};
typedef _lightweight_mutex Mutex;
typedef boost::unique_lock<_lightweight_mutex> ScopedLock;
#else
typedef boost::mutex Mutex;
typedef boost::mutex::scoped_lock ScopedLock;
#endif

typedef boost::details::pool::null_mutex NullMutex;


#include <Singleton.h>
#include <Util.h>
#include <HandlerAllocator.h>
#include <Macro.h>
#include <AtomicInt32.h>
#include <CmdDefine.h>
#include <Timer.h>
#include <TimeInfo.h>

namespace bstd  = boost::container;
namespace bsys  = boost::system;
namespace basio = boost::asio;
namespace bfs	= boost::filesystem;
namespace bsys	= boost::system;

BASE_NAME_SPACES
class TcpConnection;
BASE_NAME_SPACEE

//typedef basio::buffered_read_stream<basio::ip::tcp::socket> stream_type;
typedef boost::shared_ptr<typename base::TcpConnection> TcpConnectionPtr;
typedef boost::function<void (const TcpConnectionPtr&) > NewconnectionCallback;
typedef boost::function<void (const TcpConnectionPtr&) > DelconnectionCallback;
typedef boost::function<void (base::TcpConnection*) > RecyleConnectionfromCallback;
typedef boost::function<void (const TcpConnectionPtr&, const tagCmd*)> MessageCallback;
typedef boost::function<CmdPtr (int64_t key, const int32_t bytes)> CmdAllocateCallback;
typedef boost::function<void () > ProcessTerminateCallback;
typedef boost::threadpool::pool ThreadPool;
typedef base::RandUtilImpl<Mutex> RandUitlTS;
typedef base::RandUtilImpl<NullMutex> RandUitl;

#include <EasyUnitTest.h>
#include <Exception.h>
#include <Container.h>
#include <Log.h>

#endif