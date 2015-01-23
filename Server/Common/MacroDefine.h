#ifndef __MACRO_DEFINE_H__
#define __MACRO_DEFINE_H__

/////////////////////////////////////////////////////////////////////////////////
//基本方法
//////////////////////////////////////////////////////////////////////////
#if defined(__WINDOWS__)
#define		tvsnprintf			vsnprintf
#define		tstricmp			_stricmp
#define		tsnprintf			_snprintf
#define		getpid				_getpid
#define		localtime_r			localtime_s
#define		zeroMemory(p,len)	memset(p,0,len)
#elif defined(__LINUX__)
#define		tvsnprintf			vsnprintf
#define		tstricmp			strcasecmp
#define		tsnprintf			snprintf
#define		zeroMemory(p,len)	memset(p,0,len)
#define		__FUNCTION__		__PRETTY_FUNCTION__
#endif



/////////////////////////////////////////////////////////////////////////////////
//网络信息预定义宏
/////////////////////////////////////////////////////////////////////////////////
#ifndef FD_SETSIZE
#define FD_SETSIZE      1024
#endif /* FD_SETSIZE */



///////////////////////////////////////////////////////////////////////
//基本数据宏定义
///////////////////////////////////////////////////////////////////////

//IP地址的字符最大长度
#define IP_SIZE 24

//↓这样在可执行文件中将是汉字显示，上线前要改成随机KEY↓KEY不得小于10个字节
#define CLIENT_TO_LOGIN_KEY "服务器端对客户端的封包密钥"
#define LOGIN_TO_CLIENT_KEY "服务器端对客户端的封包密钥"
//↑这样在可执行文件中将是汉字显示，上线前要改成随机KEY↑KEY不得小于10个字节

#define PACK_COMPART "$-$"//封包分隔符
#define PACK_COMPART_SIZE strlen(PACK_COMPART)

#ifndef ENCRYPT
#define ENCRYPT(x,xlen,KEY,BeginPlace)	if( (x)!=NULL ) \
		{ \
		\
		CHAR* t_pBuffer = (x); \
		CHAR* pKey = {KEY}; \
		uint32_t KeyLen = (uint32_t)strlen(pKey); \
		for (uint32_t i = 0; i < (xlen); i++) \
			{ \
			*t_pBuffer ^= pKey[(i+BeginPlace)%KeyLen]; \
			t_pBuffer++; \
			} \
		}
#endif

#ifndef ENCRYPT_HEAD
#define ENCRYPT_HEAD(x,KEY)	if( (x)!=NULL ) \
		{ \
		CHAR* t_pBuffer = (x); \
		CHAR* pKey = {KEY}; \
		uint32_t KeyLen = (uint32_t)strlen(pKey); \
		for (uint32_t i = 0; i < PACKET_HEADER_SIZE; i++) \
			{ \
			*t_pBuffer ^= pKey[i%KeyLen]; \
			t_pBuffer++; \
			} \
		}
#endif


//无效的ID值
#define INVALID_ID		-1

//文件路径的字符最大长度
#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

///////////////////////////////////////////////////////////////////////
//调试预定义宏定义
///////////////////////////////////////////////////////////////////////
#if defined(NDEBUG)
#define __ENTER_FUNCTION_FOXNET if(1){
#define __LEAVE_FUNCTION_FOXNET }
#else
#define __ENTER_FUNCTION_FOXNET if(1){
#define __LEAVE_FUNCTION_FOXNET }
#endif


#if defined(NDEBUG)
#define _MY_TRY try
#define _MY_CATCH catch(std::exception& e){ Log::SaveLog(SERVER_ERRORFILE,"[Err]%s.", e.what());}catch(...)
#else
#define _MY_TRY try
#define _MY_CATCH catch(std::exception& e){ Log::SaveLog(SERVER_ERRORFILE,"[Err]%s.", e.what());}catch(...)
#endif



//根据指针值删除内存
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
//根据指针值删除数组类型内存
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
//根据指针调用free接口
#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif

#define INVALID_GUID			((GUID_t)(-1))

//判断某位是否被置
//15.14....3.2.1.0 
#define ISSET0(x) ((x)&0x1)
#define ISSET1(x) ((x)&0x2)
#define ISSET2(x) ((x)&0x4)
#define ISSET3(x) ((x)&0x8)
#define ISSET4(x) ((x)&0x10)
#define ISSET5(x) ((x)&0x20)
#define ISSET6(x) ((x)&0x40)
#define ISSET7(x) ((x)&0x80)
#define ISSET8(x) ((x)&0x100)
#define ISSET9(x) ((x)&0x200)
#define ISSET10(x) ((x)&0x400)
#define ISSET11(x) ((x)&0x800)
#define ISSET12(x) ((x)&0x1000)
#define ISSET13(x) ((x)&0x2000)
#define ISSET14(x) ((x)&0x4000)
#define ISSET15(x) ((x)&0x8000)


#define __ENTER_FUNCTION {try{
#define __LEAVE_FUNCTION }catch(...){AssertEx(false,__FUNCTION__);}}


#define __ENTER_FUNCTION_EX {try{
#define __LEAVE_FUNCTION_EX }catch(...){AssertEx(false,__FUNCTION__);}}


#endif // __MACRO_DEFINE_H__