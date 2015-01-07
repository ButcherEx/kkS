
#ifndef __GAMEUTIL_H__
#define __GAMEUTIL_H__
///////////////////////////////////////////////////////////////////////
//基本功能性函数定义
///////////////////////////////////////////////////////////////////////

//当前线程挂起一定时间
void	MySleep( uint32_t millionseconds=0 ) ;
TID		MyGetCurrentThreadID( ) ;
CHAR*	MySocketError( ) ;
uint32_t	MyCRC( const CHAR* szString ) ;
uint32_t	MyCRC32(const CHAR* lpData);

//////////////////////////////////////////////////////////////////////////
//数学帮助函数
//////////////////////////////////////////////////////////////////////////
template<class T>
int32_t Float2Int(T TValue)
{
	int32_t iValue = (int32_t)TValue;

	if(TValue-iValue<0.500000f)
	{
		return iValue;
	}
	else
		return iValue+1;

}

struct Flag64 
{
	uint32_t	m_uLowFlags ;
	uint32_t	m_uHighFlags ;

	Flag64()
	{
		CleanUp( ) ;
	}

	bool isSetBit( int32_t bit ) const
	{
		if(bit<32)
		{
			if ( m_uLowFlags & (1<<bit) )
				return true;
		}
		else
		{
			if ( m_uHighFlags & (1<<(bit-32)) )
				return true;
		}

		return false;
	}

	void UpdateBits( int32_t bit, bool& bUpdate )
	{
		if(bit<32)
		{
			if ( bUpdate )
				m_uLowFlags |= (1<<(int32_t)bit);
			else
				m_uLowFlags &= (~(1<<(int32_t)bit));
		}
		else
		{
			if ( bUpdate )
				m_uHighFlags |= (1<<(int32_t)(bit-32));
			else
				m_uHighFlags &= (~(1<<(int32_t)(bit-32)));
		}
	}

	void CleanUp( )
	{
		m_uLowFlags	=	0;
		m_uHighFlags =  0;
	}
};

CHAR		Value2Ascii(CHAR in);
CHAR		Ascii2Value(CHAR in);

bool		Binary2String(const CHAR* pIn,uint32_t InLength,CHAR* pOut);
bool		DBStr2Binary(const CHAR* pIn,uint32_t InLength,CHAR* pOut,uint32_t OutLimit,uint32_t& OutLength);
bool		String2Binary(const CHAR* pIn,uint32_t InLength,CHAR* pOut,uint32_t OutLimit,uint32_t& OutLength);
bool		StrSafeCheck(const CHAR* pIn,uint32_t InLength);
bool		CommandSafeCheck(const CHAR* pIn,uint32_t InLength);


#define		CHECK_STRING_NORMAL		0
#define		CHECK_STRING_CHARNAME	1

CHAR*		ReplaceIllegalString( CHAR* strText, int32_t nLength, int32_t nLevel = CHECK_STRING_NORMAL );
bool		CheckIllegalString( const CHAR* strText, int32_t nLength, int32_t nLevel = CHECK_STRING_NORMAL );

extern		int32_t				g_CmdArgv;

void		DumpStack(const CHAR* type);



#endif
