#ifndef LOG_STREAM_FILE_H
#define LOG_STREAM_FILE_H
#include <PreCompier.h>

BASE_NAME_SPACES

const int32_t kSmallBuffer = 4*1024;
const int32_t kLargeBuffer = 32*1024;

template<int32_t SIZE>
class FixedBuffer : boost::noncopyable
{
public:
	FixedBuffer()
		: cur_(data_)
	{
		setCookie(cookieStart);
	}

	~FixedBuffer()
	{
		setCookie(cookieEnd);
	}

	void append(const char* /*restrict*/ buf, size_t len)
	{
		// FIXME: append partially
		if (implicit_cast<size_t>(avail()) > len)
		{
			memcpy(cur_, buf, len);
			cur_ += len;
		}
	}

	const char* data() const { return data_; }
	int32_t length() const { return static_cast<int32_t>(cur_ - data_); }

	// write to data_ directly
	char* current() { return cur_; }
	int32_t avail() const { return static_cast<int32_t>(end() - cur_); }
	void add(size_t len) { cur_ += len; }

	void reset() { cur_ = data_; }
	void bzero() { ::memset(data_, 0, sizeof(data_)); }

	// for used by GDB
	const char* debugString();
	void setCookie(void (*cookie)()) { cookie_ = cookie; }
	// for used by unit test
	bstd::string asString() const { return bstd::string(data_, length()); }

private:
	const char* end() const { return data_ + sizeof data_; }
	// Must be outline function for cookies.
	static void cookieStart();
	static void cookieEnd();

	void (*cookie_)();
	char data_[SIZE];
	char* cur_;
};

class LogStream : boost::noncopyable
{
	typedef LogStream self;
public:
	typedef FixedBuffer<kSmallBuffer> Buffer;

	self& operator<<(bool v)
	{
		return operator<< (v ? "true" : "false");
	}

	self& operator<<(int16_t);
	self& operator<<(uint16_t);
	self& operator<<(int32_t);
	self& operator<<(uint32_t);
	self& operator<<(int64_t);
	self& operator<<(uint64_t);

	self& operator<<(const void*);

	self& operator<<(float v)
	{
		*this << static_cast<double>(v);
		return *this;
	}
	self& operator<<(double);

	self& operator<<(char v)
	{
		buffer_.append(&v, 1);
		return *this;
	}

	self& operator<<(const char* str)
	{
		if (str)
		{
			buffer_.append(str, strlen(str));
		}
		else
		{
			buffer_.append("(null)", 6);
		}
		return *this;
	}

	self& operator<<(const unsigned char* str)
	{
		return operator<<(reinterpret_cast<const char*>(str));
	}

	self& operator<<(const bstd::string& v)
	{
		buffer_.append(v.c_str(), v.size());
		return *this;
	}

	void append(const char* data, int32_t len) { buffer_.append(data, len); }
	const Buffer& buffer() const { return buffer_; }
	void resetBuffer() { buffer_.reset(); }

private:
	void staticCheck();

	template<typename T>
	void formatInteger(T);

	Buffer buffer_;

	static const int32_t kMaxNumericSize = 32;
};

class Fmt // : boost::noncopyable
{
public:
	template<typename T>
	Fmt(const char* fmt, T val);

	const char* data() const { return buf_; }
	int32_t length() const { return length_; }

private:
	char buf_[32];
	int32_t length_;
};

inline LogStream& operator<<(LogStream& s, const Fmt& fmt)
{
	s.append(fmt.data(), fmt.length());
	return s;
}

BASE_NAME_SPACEE

#endif