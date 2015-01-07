#ifndef UTIL_H
#define UTIL_H

BASE_NAME_SPACES


inline bool isPow(int32_t val) { return !(val & (val-1)); }
inline int32_t align(int32_t val, int32_t _algin)  { return ((val + _algin - 1) & ~(_algin - 1)); }
inline int32_t nextPow2(int32_t val)
{
	if( val > 1 )
	{
		if(!isPow(val))
		{
			val |= val  >> 1;
			val |= val  >> 2;
			val |= val  >> 4;
			val |= val  >> 8;
			val |= val  >> 16;
			val = val + 1;
		}
	}
	else
	{
		val = 2;
	}

	return val;
}

inline uint32_t crc32(const char* lpData)
{
	uint32_t sum;
	for (sum = 0; *lpData; )
	{
		/*
		* gcc 2.95.2 x86 and icc 7.1.006 compile
		* that operator into the single "rol" opcode,
		* msvc 6.0sp2 compiles it into four opcodes.
		*/
		sum =	sum >> 1 | sum << 31;
		sum += *lpData++;
	}
	return sum;
}

inline void thisThreadSleep( int32_t millisecond )
{
#if defined(__LINUX__)
	usleep(millisecond*1000);
#elif defined(__WINDOWS__)
	Sleep(millisecond);
#else
#error windows or linux required.
#endif
}

inline int32_t networkToHost32(int32_t n32){ return boost::asio::detail::socket_ops::network_to_host_long(n32); }
inline int32_t hostToNetwork32(int32_t h32){ return boost::asio::detail::socket_ops::host_to_network_long(h32); }
inline int32_t networkToHost16(int32_t n16){ return boost::asio::detail::socket_ops::network_to_host_short(n16);}
inline int32_t hostToNetwork16(int32_t h16){ return boost::asio::detail::socket_ops::host_to_network_short(h16);}

template<typename LockType>
class RandUtilImpl
{
public:
	RandUtilImpl(uint32_t seed = 0)
	{
		if( seed != 0 )
		{
			seed_ = seed;
		}
		else
		{
			seed_ = (uint32_t)time(0);
		}
	}

	int32_t rand( )
	{
		int32_t ret;
		ScopedLock lock(lock_);
		ret = (((seed_ = seed_ * 1103515245 + 12345) >> 1 ) & INT32_MAX);
		return ret;
	}

	int32_t range(int32_t low, int32_t high)
	{
		int32_t ret = low;
		ScopedLock lock(lock_);
		if(low <  high)
		{
			ret = (((seed_ = seed_ * 1103515245 + 12345) >> 1 ) & INT32_MAX);
			ret = (ret % (high - low + 1))+low;
		}
		return ret;
	}
private:
	uint32_t seed_;
	LockType lock_;
};


// Taken from google-protobuf stubs/common.h
//
// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda) and others
//
// Contains basic types and utilities used by the rest of the library.

//
// Use implicit_cast as a safe version of static_cast or const_cast
// for upcasting in the type hierarchy (i.e. casting a pointer to Foo
// to a pointer to SuperclassOfFoo or casting a pointer to Foo to
// a const pointer to Foo).
// When you use implicit_cast, the compiler checks that the cast is safe.
// Such explicit implicit_casts are necessary in surprisingly many
// situations where C++ demands an exact type match instead of an
// argument type convertable to a target type.
//
// The From type can be inferred, so the preferred syntax for using
// implicit_cast is the same as for static_cast etc.:
//
//   implicit_cast<ToType>(expr)
//
// implicit_cast would have been part of the C++ standard library,
// but the proposal was submitted too late.  It will probably make
// its way into the language in the future.

template<typename To, typename From>
inline To implicit_cast(From const &f) {
	return f;
}

BASE_NAME_SPACEE

#endif