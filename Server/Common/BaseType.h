//
//�ļ����ƣ�	Type.h
//����������	���Ͷ��壬���ֲ�ͬ����ϵͳ��ϵͳ���õ����ã����е�ǰ����
//				�ڵĴ��붼�����һ�����ô��ļ�
//�汾˵����	Windows����ϵͳ��Ҫ����꣺__WINDOWS__
//				Linux����ϵͳ��Ҫ����꣺__LINUX__
//
//

#ifndef __BASE_TYPE_H__
#define __BASE_TYPE_H__


///////////////////////////////////////////////////////////////////////
//��׼�������Ͷ���
///////////////////////////////////////////////////////////////////////
typedef char CHAR;
typedef unsigned char UCHAR;

///////////////////////////////////////////////////////////////////////
//�����������Ͷ���
///////////////////////////////////////////////////////////////////////
typedef unsigned long	IP_t;
typedef uint16_t		PacketID_t;
typedef int16_t			ID_t;
typedef uint32_t		ObjID_t;			//�����й̶�������OBJӵ�в�ͬ��ObjID_t									//
typedef ID_t			PlayerID_t;			//�������
typedef int32_t			Time_t; //ʱ������
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
