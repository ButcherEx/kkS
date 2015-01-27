
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
void HostName(bstd::string& hn);
void clearStderrLineBuffer(FILE *file);
//////////////////////////////////////////////////////////////////////////
enum LogColor 
{
	COLOR_DEFAULT,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW
};
void coloredWriteToStderr(const CHAR* message, size_t len, LogColor color);
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
//////////////////////////////////////////////////////////////////////////
CHAR		Value2Ascii(CHAR in);
CHAR		Ascii2Value(CHAR in);
//////////////////////////////////////////////////////////////////////////
bool		Binary2String(const CHAR* pIn,uint32_t InLength,CHAR* pOut);
bool		DBStr2Binary(const CHAR* pIn,uint32_t InLength,CHAR* pOut,uint32_t OutLimit,uint32_t& OutLength);
bool		String2Binary(const CHAR* pIn,uint32_t InLength,CHAR* pOut,uint32_t OutLimit,uint32_t& OutLength);
bool		StrSafeCheck(const CHAR* pIn,uint32_t InLength);
bool		CommandSafeCheck(const CHAR* pIn,uint32_t InLength);

//////////////////////////////////////////////////////////////////////////
#define		CHECK_STRING_NORMAL		0
#define		CHECK_STRING_CHARNAME	1

CHAR*		ReplaceIllegalString( CHAR* strText, int32_t nLength, int32_t nLevel = CHECK_STRING_NORMAL );
bool		CheckIllegalString( const CHAR* strText, int32_t nLength, int32_t nLevel = CHECK_STRING_NORMAL );
//////////////////////////////////////////////////////////////////////////
bool		IsPow2(int32_t val);
int32_t		AlignN(int32_t val, int32_t _algin);
int32_t		NextPow2(int32_t val);
//////////////////////////////////////////////////////////////////////////
class RandInt32
{
public:
	RandInt32(uint32_t seed = 0);
public:
	int32_t Gen( );
	int32_t GenRange(int32_t low, int32_t high);
private:
	uint32_t m_Seed;
};
//////////////////////////////////////////////////////////////////////////
#endif
