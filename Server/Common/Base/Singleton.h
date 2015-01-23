// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef BASE_SINGLETON_H
#define BASE_SINGLETON_H

#include <PreCompier.h>


// This doesn't detect inherited member functions!
// http://stackoverflow.com/questions/1966362/sfinae-to-check-for-inherited-member-functions
template<typename T>
struct has_no_destroy
{
	const static bool value = boost::is_pod<T>::value;
};


template<typename T>
class Singleton : boost::noncopyable
{
 public:
  static T& instance()
  {
    boost::call_once(flag_, &Singleton::init);
    return *value_;
  }

 private:
  Singleton();
  ~Singleton();

  static void init()
  {
    value_ = new T();
    if (!has_no_destroy<T>::value)
    {
      ::atexit(destroy);
    }
  }

  static void destroy()
  {
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complete_type dummy; (void) dummy;

    delete value_;
  }

 private:
  static boost::once_flag flag_;
  static T*             value_;
};

template<typename T>
boost::once_flag Singleton<T>::flag_ = BOOST_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;


#endif

