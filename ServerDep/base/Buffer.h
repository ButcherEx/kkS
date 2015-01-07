#ifndef BUFFER_H
#define BUFFER_H

#include <PreCompier.h>

BASE_NAME_SPACES

/// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
///
/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode
class Buffer : boost::noncopyable
{
public:
	Buffer(int32_t cheapPrepend, int32_t initialSize = 1024)
		: buffer_(cheapPrepend + initialSize),
		readerIndex_(cheapPrepend),
		writerIndex_(cheapPrepend),
		kcheapPrepend(cheapPrepend)
	{
		_MY_TRY
		{
			_Assert0((readableBytes() == 0));
			_Assert0((writableBytes() == initialSize));
			_Assert0((prependableBytes() == kcheapPrepend));
		}
		_MY_CATCH
		{
			retrieveAll();
		}
	}

	~Buffer( ){ }
public:
	const char* peek() const
	{ return begin() + readerIndex_; }
public:
	void retrieve(int32_t len)
	{
		_Verify0((len <= readableBytes()));
		if (len < readableBytes())
		{
			readerIndex_ += len;
		}
		else
		{
			retrieveAll();
		}
	}

	void retrieveAll()
	{
		readerIndex_ = kcheapPrepend;
		writerIndex_ = kcheapPrepend;
	}
public:
	int32_t readableBytes() const
	{ return writerIndex_ - readerIndex_; }

	int32_t writableBytes() const
	{ return ((int32_t)buffer_.size()) - writerIndex_; }

	int32_t prependableBytes() const
	{ return readerIndex_; }

	void append(const char* /*restrict*/ data, int32_t len)
	{
		if(writableBytes()>= len)
		{
			memcpy(beginWrite(), data, len);
			hasWritten(len);
		}
	}

	void append(const void* /*restrict*/ data, int32_t len)
	{
		append(static_cast<const char*>(data), len);
	}

	char* beginWrite()
	{ return begin() + writerIndex_; }

	const char* beginWrite() const
	{ return begin() + writerIndex_; }

	void hasWritten(int32_t len)
	{ writerIndex_ += len; }

	char* beginBuffer( )
	{
		return begin();
	}

	///
	/// Append int32_t using network endian
	///
	void appendInt32(int32_t x)
	{
		int32_t be32 = hostToNetwork32(x);
		append(&be32, sizeof(be32));
	}

	///
	/// Read int32_t from network endian
	///
	/// Require: buf->readableBytes() >= sizeof(int32_t)
	int32_t readInt32()
	{
		int32_t result = peekInt32();
		retrieveInt32();
		return result;
	}

public:
	void prepend(const void* /*restrict*/ data, int32_t len)
	{
		_Verify0((len <= prependableBytes()));
		hasPrepend(len);
		const char* d = static_cast<const char*>(data);
		memcpy(begin()+readerIndex_, (void*)d, len);
	}

	void hasPrepend(int32_t len)
	{
		readerIndex_ -= len;
	}
private:
	int32_t peekInt32() const
	{
		_Verify0((readableBytes() >= sizeof(int32_t)));
		int32_t be32 = 0;
		::memcpy(&be32, peek(), sizeof be32);
		return networkToHost32(be32);
	}
	void retrieveInt32()
	{
		retrieve(sizeof(int32_t));
	}


	char* begin()
	{ return &*buffer_.begin(); }

	const char* begin() const
	{ return &*buffer_.begin(); }
private:
	bstd::vector<char> buffer_;
	int32_t readerIndex_;
	int32_t writerIndex_;
	const int32_t kcheapPrepend;
};

BASE_NAME_SPACEE

#endif // BUFFER_H

