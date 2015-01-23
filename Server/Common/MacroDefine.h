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
#elif defined(__LINUX__)
#define		tvsnprintf			vsnprintf
#define		tstricmp			strcasecmp
#define		tsnprintf			snprintf
#define		zeroMemory(p,len)	memset(p,0,len)
#define		__FUNCTION__		__PRETTY_FUNCTION__
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

//�������ڿ�ִ���ļ��н��Ǻ�����ʾ������ǰҪ�ĳ����KEY��KEY����С��10���ֽ�
#define CLIENT_TO_LOGIN_KEY "�������˶Կͻ��˵ķ����Կ"
#define LOGIN_TO_CLIENT_KEY "�������˶Կͻ��˵ķ����Կ"
//�������ڿ�ִ���ļ��н��Ǻ�����ʾ������ǰҪ�ĳ����KEY��KEY����С��10���ֽ�

#define PACK_COMPART "$-$"//����ָ���
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


//��Ч��IDֵ
#define INVALID_ID		-1

//�ļ�·�����ַ���󳤶�
#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

///////////////////////////////////////////////////////////////////////
//����Ԥ����궨��
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

//�ж�ĳλ�Ƿ���
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