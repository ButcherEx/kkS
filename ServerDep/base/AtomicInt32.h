#pragma once

BASE_NAME_SPACES

#if defined(__WINDOWS__)
template<typename T> class AtomicUtil{};
template<> class AtomicUtil<int32_t>
{
public:
	static int32_t InterlockedIncrement (int32_t volatile *lpAdden)
	{
		return ::InterlockedIncrement((volatile long*)lpAdden);
	}

	static int32_t InterlockedDecrement (int32_t volatile *lpAddend)
	{
		return ::InterlockedDecrement((volatile long*)lpAddend);
	}

	static int32_t InterlockedExchange(int32_t volatile * _Target, int32_t _Value)
	{
		return ::InterlockedExchange((volatile long*)_Target, (long)_Value);
	}


	static int32_t InterlockedExchangeAdd( int32_t volatile *Addend,int32_t Value)
	{
		return ::InterlockedExchangeAdd((volatile long*)Addend, (long)Value);
	}
	static int32_t InterlockedCompareExchange(int32_t volatile * Destination,int32_t Exchange,int32_t Comperand)
	{
		return ::InterlockedCompareExchange((volatile long*)Destination, (long)Exchange, (long)Comperand);
	}
};
#if defined(_WIN64)
template<> class AtomicUtil<int64_t>
{
public:
	static int64_t InterlockedIncrement (int64_t volatile *lpAdden)
	{
		return ::InterlockedDecrement64((volatile long long*)lpAdden);
	}

	static int64_t InterlockedDecrement (int64_t volatile *lpAddend)
	{
		return ::InterlockedDecrement64((volatile long  long*)lpAddend);
	}

	static int64_t InterlockedExchange(int64_t volatile * _Target, int64_t _Value)
	{
		return ::InterlockedExchange64((volatile long  long*)_Target, (long long)_Value);
	}

	static int64_t InterlockedExchangeAdd( int64_t volatile *Addend,int64_t Value)
	{
		return ::InterlockedExchangeAdd64((volatile long long*)Addend, (long long)Value);
	}
	static int64_t InterlockedCompareExchange(int64_t volatile * Destination,int64_t Exchange,int64_t Comperand)
	{
		return ::InterlockedCompareExchange64((volatile long long*)Destination, (long long)Exchange, (long long)Comperand);
	}
};
#endif // #if defined(_WIN64)
#endif

template<typename T>
class AtomicIntT
{
public:
	AtomicIntT( T b = 0) : val_( b ){ }
public:

	long get( )
	{
		return addAndGet( 0 );
	}

	long set( T l )
	{
		return getAndSet(l);
	}

	long addAndGet( ) 
	{
#if defined(__LINUX__)
		return __sync_add_and_fetch(&val_, 1);
#elif defined(__WINDOWS__)
		return AtomicUtil<T>::InterlockedIncrement(&val_);
#else
	#error windows or linux required.
#endif
	}

	long subAndGet( ) 
	{
#if defined(__LINUX__)
		return __sync_sub_and_fetch(&val_, 1);
#elif defined(__WINDOWS__)
		return AtomicUtil<T>::InterlockedDecrement(&val_);
#else
	#error windows or linux required.
#endif
	}
	long getAndSet( T l) 
	{
#if defined(__LINUX__)
		return __sync_lock_test_and_set(&val_, l);
#elif defined(__WINDOWS__)
		return AtomicUtil<T>::InterlockedExchange(&val_, l);
#else
	#error windows or linux required.
#endif
	}

	long addAndGet( T l)
	{
#if defined(__LINUX__)
		return __sync_fetch_and_add(&value_, l);
#elif defined(__WINDOWS__)
		return AtomicUtil<T>::InterlockedExchangeAdd(&val_, l);
#else
	#error windows or linux required.
#endif
	}

	long compareAndSet(T con, T v)
	{
#if defined(__LINUX__)
		return __sync_val_compare_and_swap(&value_, con, v);
#elif defined(__WINDOWS__)
		return AtomicUtil<T>::InterlockedCompareExchange(&val_, v, con);
#else
	#error windows or linux required.
#endif
	}
private:
	volatile T val_;
};

typedef AtomicIntT<int32_t> AtomicInt32;
#if defined(_WIN64) || defined(__LINUX__)
typedef AtomicIntT<int64_t> AtomicInt64;
#else
class AtomicInt64
{
public:
	AtomicInt64( int64_t b = 0) : val_( b ){ }
public:

	int64_t get( )
	{
		return addAndGet( 0 );
	}

	int64_t set( int64_t l )
	{
		return getAndSet(l);
	}

	int64_t addAndGet( ) 
	{
		ScopedLock autoLock(lock_);
		val_ += 1;
		return val_;
	}

	int64_t subAndGet( ) 
	{
		ScopedLock autoLock(lock_);
		val_ -= 1;
		return val_;
	}
	int64_t getAndSet( int64_t l) 
	{
		ScopedLock autoLock(lock_);
		int64_t old = val_;
		val_ = l;
		return old;
	}

	int64_t addAndGet( int64_t l)
	{
		ScopedLock autoLock(lock_);
		val_ += l;
		return val_;
	}

	int64_t compareAndSet(int64_t con, int64_t v)
	{
		ScopedLock autoLock(lock_);
		int64_t old = val_;
		if(val_ == con) val_ = v;
		return old;
	}
private:
	Mutex lock_;
	volatile int64_t val_;
};

#endif

BASE_NAME_SPACEE

