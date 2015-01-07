//
// handler_allocator.cpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HANDLER_ALLOCATOR_HPP
#define HANDLER_ALLOCATOR_HPP


BASE_NAME_SPACES

// Class to manage the memory to be used for handler-based custom allocation.
// It contains a single block of memory which may be returned for allocation
// requests. If the memory is in use when an allocation request is made, the
// allocator delegates allocation to the global heap.
template<size_t size>
class HandlerAllocator
  : private boost::noncopyable
{
public:
  HandlerAllocator()
    : in_use_(false)
  {
  }

  void* allocate(std::size_t size)
  {
    if (!in_use_ && size < storage_.size)
    {
      in_use_ = true;
      return storage_.address();
    }

    return malloc(size);
  }

  void deallocate(void* pointer)
  {
    if (pointer == storage_.address())
    {
      in_use_ = false;
    }
    else
    {
      free(pointer);
    }
  }

private:
  // Storage space used for handler-based custom memory allocation.
  boost::aligned_storage<size> storage_;

  // Whether the handler-based custom allocation storage has been used.
  bool in_use_;
};

// Wrapper class template for handler objects to allow handler memory
// allocation to be customised. Calls to operator() are forwarded to the
// encapsulated handler.
template <class Allocator, typename Handler>
class custom_alloc_handler
{
public:
  custom_alloc_handler(Allocator& a, Handler h)
    : allocator_(a),
      handler_(h)
  {
  }

  template <typename Arg1>
  void operator()(Arg1 arg1)
  {
    handler_(arg1);
  }

  template <typename Arg1, typename Arg2>
  void operator()(Arg1 arg1, Arg2 arg2)
  {
    handler_(arg1, arg2);
  }

  friend void* asio_handler_allocate(std::size_t size,
      custom_alloc_handler<Allocator, Handler>* this_handler)
  {
    return this_handler->allocator_.allocate(size);
  }

  friend void asio_handler_deallocate(void* pointer, std::size_t /*size*/,
      custom_alloc_handler<Allocator, Handler>* this_handler)
  {
    this_handler->allocator_.deallocate(pointer);
  }

private:
  Allocator& allocator_;
  Handler handler_;
};

// Helper function to wrap a handler object to add custom allocation.
template <class Allocator, typename Handler>
inline custom_alloc_handler<Allocator, Handler> make_custom_alloc_handler(
    Allocator& a, Handler h)
{
  return custom_alloc_handler<Allocator, Handler>(a, h);
}

BASE_NAME_SPACEE

#endif // HANDLER_ALLOCATOR_HPP

