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
#define		tgetcwd				_getcwd
#elif defined(__LINUX__)
#define		tvsnprintf			vsnprintf
#define		tstricmp			strcasecmp
#define		tsnprintf			snprintf
#define		zeroMemory(p,len)	memset(p,0,len)
#define		tgetcwd				getcwd
#define		__FUNCTION__		__PRETTY_FUNCTION__
#endif

#ifndef _MAX
#define _MAX(X,Y) (((X)>(Y))?(X):(Y))
#endif

#ifndef _MIN
#define _MIN(X,Y) (((X)<(Y))?(X):(Y))
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


//无效的ID值
#define INVALID_ID		-1

//文件路径的字符最大长度
#ifndef _MAX_PATH
#define _MAX_PATH 260
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


///////////////////////////////////////////////////////////////////////
//调试预定义宏定义
///////////////////////////////////////////////////////////////////////

#define _MY_TRY bool __exceptionCatch = false; try
#define _MY_CATCH  \
	catch(const std::exception &err){\
		__exceptionCatch = true;\
		AssertSpecialEx(false, err.what());\
	} catch(const std::string &err){\
		__exceptionCatch = true;\
		AssertSpecialEx(false, err.c_str());\
	}catch(const char* err){\
		__exceptionCatch = true;\
		AssertSpecialEx(false, err);\
	}catch(...){\
		__exceptionCatch = true;\
		AssertSpecialEx(false, "");\
	}\
	if(__exceptionCatch)\


#define __ENTER_FUNCTION {try{

#define __LEAVE_FUNCTION\
	}catch(const std::exception &err){\
		AssertEx(false, err.what());\
	} catch(const std::string &err){\
		AssertEx(false, err.c_str());\
	}catch(const char* err){\
		AssertEx(false, err);\
	}catch(...){\
		AssertEx(false, __FUNCTION__);\
	}}\

#define __ENTER_FUNCTION_EX {try{
#define __LEAVE_FUNCTION_EX \
	}catch(const std::exception &err){\
		AssertSpecialEx(false, err.what());\
	} catch(const std::string &err){\
		AssertSpecialEx(false, err.c_str());\
	}catch(const char* err){\
		AssertSpecialEx(false, err);\
	}catch(...){\
		AssertSpecialEx(false, __FUNCTION__);\
	}}\



#endif // __MACRO_DEFINE_H__