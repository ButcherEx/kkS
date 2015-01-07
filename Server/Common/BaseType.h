//
//文件名称：	Type.h
//功能描述：	类型定义，区分不同操作系统间系统调用的设置，所有当前工程
//				内的代码都必须第一个引用此文件
//版本说明：	Windows操作系统需要定义宏：__WINDOWS__
//				Linux操作系统需要定义宏：__LINUX__
//
//

#ifndef __BASE_TYPE_H__
#define __BASE_TYPE_H__


///////////////////////////////////////////////////////////////////////
//标准数据类型定义
///////////////////////////////////////////////////////////////////////
typedef char CHAR;
typedef unsigned char UCHAR;

///////////////////////////////////////////////////////////////////////
//基本数据类型定义
///////////////////////////////////////////////////////////////////////
typedef unsigned long	IP_t;
typedef uint16_t		PacketID_t;
typedef int16_t			ID_t;
typedef uint32_t		ObjID_t;			//场景中固定的所有OBJ拥有不同的ObjID_t									//
typedef ID_t			PlayerID_t;			//连接玩家
typedef int32_t			Time_t; //时间类型
typedef uint64_t		GUID_t;

typedef int16_t			int16;
typedef uint16_t		uint16;
typedef int32_t			int32;
typedef uint32_t		uint32;
typedef int64_t			int64;
typedef uint64_t		uint64;


#if defined(__WINDOWS__)
typedef unsigned long		TID ;
#elif defined(__LINUX__)
typedef pthread_t	TID ;
#endif


#if defined(__WINDOWS__)
#include <Windows.h>
class _lightweight_mutex : public boost::noncopyable
{
private:
	CRITICAL_SECTION cs_;
public:
	_lightweight_mutex();
	~_lightweight_mutex();
	void lock( );
	void unlock();
	bool try_lock();
};
typedef _lightweight_mutex MyLock;
typedef boost::unique_lock<_lightweight_mutex> AutoLock_T;
#else
typedef boost::mutex MyLock;
typedef boost::mutex::scoped_lock AutoLock_T;
#endif




#endif
