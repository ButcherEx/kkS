#ifndef __FLSTRING_H__
#define __FLSTRING_H__

template
<
 int32_t  SIZE
>
class FLString
{
public:
	typedef FLString<SIZE> SelfType;
public:
	FLString( )
	{
		Initial();
	}
	~FLString()
	{

	}
public:
	explicit FLString(const CHAR* src)
	{
		*this = src;
	}

	template<int32_t _SIZE>
	explicit FLString( const FLString<_SIZE>& rSrc)
	{
		*this = rSrc;
	}

	explicit FLString(int16_t n16)
	{
		*this = n16;
	}

	explicit FLString(uint16_t u16)
	{
		*this = u16;
	}


	explicit FLString(int32_t n32)
	{
		*this = n32;
	}

	explicit FLString(uint32_t u32)
	{
		*this = u32;
	}

	explicit FLString(int64_t n64)
	{
		*this = n64;
	}

	explicit FLString(uint64_t u64)
	{
		*this = u64;
	}
public:
	const CHAR* GetStr( ) const { return m_Buffer; }
	int32_t GetLength( ) const { return m_Length; }
	int32_t GetMaxLength() const  { return SIZE; }
	void	Clear() { Initial(); }
public:
	SelfType& operator =(const CHAR* src)
	{
		Initial();
		return ( *this+= src);
	}

	SelfType& operator+=(const CHAR* src)
	{
		if( src != NULL )
		{
			const int32_t srcLen = (int32_t)strlen(src);
			if( srcLen + m_Length > SIZE)
			{
				strncpy(m_Buffer+m_Length, src, (SIZE - m_Length));
				m_Length = SIZE;
			}
			else
			{
				strncpy(m_Buffer+m_Length, src, srcLen);
				m_Length += srcLen;
			}

			Assert(m_Length>=0 && m_Length<=SIZE);
			m_Buffer[m_Length] = 0;
		}

		return *this;
	}

	bool operator == (const CHAR* szText) const
	{
		if( szText != NULL )
		{
			const int32_t Len = strlen(szText);
			if( Len == GetLength() )
			{
				return (strcmp(GetStr(), szText) == 0);
			}

		}

		return false;
	}

	bool operator != (const CHAR* szText) const
	{
		return !(*this == szText);
	}
	bool operator < (const CHAR* szText) const
	{
		if( szText != NULL )
		{
			return (strcmp(GetStr(), szText) < 0);
		}
		return false;
	}
public:

	SelfType& operator=(const SelfType& rSrc)
	{
		return ( *this = rSrc.GetStr() );
	}

	SelfType& operator+=(const SelfType& rSrc)
	{
		return ( *this += rSrc.GetStr() );
	}

	bool operator==(const SelfType& rSrc) const
	{
		return ( *this == rSrc.GetStr() );
	}

	bool operator != (const SelfType& rSrc) const
	{
		return !(*this == rSrc);
	}

	bool operator < (const SelfType& rSrc) const
	{
		return (*this < rSrc.GetStr());
	}


public:
	template<int32_t _SIZE>
	SelfType& operator=(const FLString<_SIZE>& rSrc)
	{
		return ( *this = rSrc.GetStr() );
	}

	template<int32_t _SIZE>
	SelfType& operator+=(const FLString<_SIZE>& rSrc)
	{
		return ( *this += rSrc.GetStr() );
	}

	template<int32_t _SIZE>
	bool operator==(const FLString<_SIZE>& rSrc) const
	{
		return ( *this == rSrc.GetStr() );
	}

	template<int32_t _SIZE>
	bool operator != (const FLString<_SIZE>& rSrc) const
	{
		return !(*this == rSrc.GetStr());
	}

	template<int32_t _SIZE>
	bool operator < (const FLString<_SIZE>& rSrc) const
	{
		return (*this < rSrc.GetStr());
	}

public:

	SelfType& operator=(int16_t n16)
	{
		return (*this = static_cast<int32_t>(n16));
	}
	SelfType& operator=(uint16_t u16)
	{
		return (*this = static_cast<uint32_t>(u16));
	}


	SelfType& operator=(int32_t n32)
	{
		CHAR szBuf[64] = {0};
		tsnprintf(szBuf, 64, "%d", n32);
		return (*this = szBuf);
	}
	SelfType& operator=(uint32_t u32)
	{
		CHAR szBuf[64] = {0};
		tsnprintf(szBuf, 64, "%u", u32);
		return (*this = szBuf);
	}


	SelfType& operator=(int64_t n64)
	{
		CHAR szBuf[64] = {0};
		tsnprintf(szBuf, 64, "%lld", n64);
		return (*this = szBuf);
	}

	SelfType& operator=(uint64_t u64)
	{
		CHAR szBuf[64] = {0};
		tsnprintf(szBuf, 64, "%llu", u64);
		return (*this = szBuf);
	}
public:

	SelfType& operator+=(int16_t n16)
	{
		return (*this += static_cast<int32_t>(n16));
	}
	SelfType& operator+=(uint16_t u16)
	{
		return (*this += static_cast<uint32_t>(u16));
	}


	SelfType& operator+=(int32_t n32)
	{
		CHAR szBuf[64] = {0};
		tsnprintf(szBuf, 64, "%d", n32);
		return (*this += szBuf);
	}
	SelfType& operator+=(uint32_t u32)
	{
		CHAR szBuf[64] = {0};
		tsnprintf(szBuf, 64, "%u", u32);
		return (*this += szBuf);
	}


	SelfType& operator +=(int64_t n64)
	{
		CHAR szBuf[64] = {0};
		tsnprintf(szBuf, 64, "%lld", n64);
		return (*this += szBuf);
	}

	SelfType& operator +=(uint64_t u64)
	{
		CHAR szBuf[64] = {0};
		tsnprintf(szBuf, 64, "%llu", u64);
		return (*this += szBuf);
	}
public:
	int32_t GetInt32( ) const
	{
		return (int32_t)atoi(m_Buffer);
	}

	uint32_t GetUInt32( ) const
	{
		uint32_t u32 = 0;
		sscanf(m_Buffer, "%u", &u32);
		return u32;
	}

	int64_t GetInt64( ) const
	{
		int64_t n64 = 0;
		sscanf(m_Buffer, "%lld", &n64);
		return n64;
	}


	uint64_t GetUInt64( ) const
	{
		uint64_t u64 = 0;
		sscanf(m_Buffer, "%llu", &u64);
		return u64;
	}

private:
	void Initial( )
	{
		m_Length = 0;
		zeroMemory(m_Buffer, sizeof(m_Buffer));
	}
private:
	int32_t m_Length;
	CHAR m_Buffer[SIZE+1];
};

#endif /* __FLSTRING_H__ */