#include <Log/LogStream.h>

BASE_NAME_SPACES

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
BOOST_STATIC_ASSERT(sizeof(digits) == 20);

const char digitsHex[] = "0123456789ABCDEF";
BOOST_STATIC_ASSERT(sizeof digitsHex == 17);

// Efficient Integer to String Conversions, by Matthew Wilson.
template<typename T>
size_t convert(char buf[], T value)
{
	T i = value;
	char* p = buf;

	do
	{
		int32_t lsd = static_cast<int32_t>(i % 10);
		i /= 10;
		*p++ = zero[lsd];
	} while (i != 0);

	if (value < 0)
	{
		*p++ = '-';
	}
	*p = '\0';
	std::reverse(buf, p);

	return p - buf;
}

size_t convertHex(char buf[], uintptr_t value)
{
	uintptr_t i = value;
	char* p = buf;

	do
	{
		int32_t lsd = i % 16;
		i /= 16;
		*p++ = digitsHex[lsd];
	} while (i != 0);

	*p = '\0';
	std::reverse(buf, p);

	return p - buf;
}

template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargeBuffer>;



template<int32_t SIZE>
const char* FixedBuffer<SIZE>::debugString()
{
	*cur_ = '\0';
	return data_;
}

template<int32_t SIZE>
void FixedBuffer<SIZE>::cookieStart()
{
}

template<int32_t SIZE>
void FixedBuffer<SIZE>::cookieEnd()
{
}

void LogStream::staticCheck()
{
	_Assert0(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10);
	_Assert0(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10);
	_Assert0(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10);
	_Assert0(kMaxNumericSize - 10 > std::numeric_limits<int64_t>::digits10);
}

template<typename T>
void LogStream::formatInteger(T v)
{
	if (buffer_.avail() >= kMaxNumericSize)
	{
		size_t len = convert(buffer_.current(), v);
		buffer_.add(len);
	}
}

LogStream& LogStream::operator<<(int16_t v)
{
	*this << static_cast<int32_t>(v);
	return *this;
}

LogStream& LogStream::operator<<(uint16_t v)
{
	*this << static_cast<uint32_t>(v);
	return *this;
}

LogStream& LogStream::operator<<(int32_t v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(uint32_t v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(int64_t v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(uint64_t v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(const void* p)
{
	uintptr_t v = reinterpret_cast<uintptr_t>(p);
	if (buffer_.avail() >= kMaxNumericSize)
	{
		char* buf = buffer_.current();
		buf[0] = '0';
		buf[1] = 'x';
		size_t len = convertHex(buf+2, v);
		buffer_.add(len+2);
	}
	return *this;
}

// FIXME: replace this with Grisu3 by Florian Loitsch.
LogStream& LogStream::operator<<(double v)
{
	if (buffer_.avail() >= kMaxNumericSize)
	{
		int32_t len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
		buffer_.add(len);
	}
	return *this;
}

template<typename T>
Fmt::Fmt(const char* fmt, T val)
{
	BOOST_STATIC_ASSERT(boost::is_arithmetic<T>::value == true);

	length_ = snprintf(buf_, sizeof(buf_), fmt, val);
	assert(static_cast<size_t>(length_) < sizeof (buf_));
}

// Explicit instantiations

template Fmt::Fmt(const char* fmt, char);

template Fmt::Fmt(const char* fmt, int16_t);
template Fmt::Fmt(const char* fmt, uint16_t);
template Fmt::Fmt(const char* fmt, int32_t);
template Fmt::Fmt(const char* fmt, uint32_t);

template Fmt::Fmt(const char* fmt, int64_t);
template Fmt::Fmt(const char* fmt, uint64_t);

template Fmt::Fmt(const char* fmt, float);
template Fmt::Fmt(const char* fmt, double);

BASE_NAME_SPACEE