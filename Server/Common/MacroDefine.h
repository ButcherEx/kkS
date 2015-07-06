#ifndef __MACRO_DEFINE_H__
#define __MACRO_DEFINE_H__

/////////////////////////////////////////////////////////////////////////////////
//��������
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
//������ϢԤ�����
/////////////////////////////////////////////////////////////////////////////////
#ifndef FD_SETSIZE
#define FD_SETSIZE      1024
#endif /* FD_SETSIZE */



///////////////////////////////////////////////////////////////////////
//�������ݺ궨��
///////////////////////////////////////////////////////////////////////

//IP��ַ���ַ���󳤶�
#define IP_SIZE 24


//��Ч��IDֵ
#define INVALID_ID		-1

//�ļ�·�����ַ���󳤶�
#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif


//����ָ��ֵɾ���ڴ�
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
//����ָ��ֵɾ�����������ڴ�
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
//����ָ�����free�ӿ�
#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif

#define INVALID_GUID			((GUID_t)(-1))


///////////////////////////////////////////////////////////////////////
//����Ԥ����궨��
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