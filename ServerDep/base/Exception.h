
#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <PreCompier.h>

//////////////////////////////////////////////////////////////////////////
class ExceptionHanlder
{
public:
	void initialize( );
};


//////////////////////////////////////////////////////////////////////////
void __error_assert(const char *file, const char *func, 
					int line, const char *exp, bool throwException);

//////////////////////////////////////////////////////////////////////////
#define _Assert(conditionCheck, msg)\
	{ if(!(conditionCheck)) __error_assert(__FILE__, __FUNCTION__, __LINE__, msg, true); }

#define _Assert0(conditionCheck)\
	{ if(!(conditionCheck)) __error_assert(__FILE__, __FUNCTION__, __LINE__, (#conditionCheck), true); }

//////////////////////////////////////////////////////////////////////////
#define _Verify(conditionCheck, msg)\
{ if(!(conditionCheck)) __error_assert(__FILE__, __FUNCTION__, __LINE__, msg, false); }

#define _Verify0(conditionCheck)\
{ if(!(conditionCheck)) __error_assert(__FILE__, __FUNCTION__, __LINE__, (#conditionCheck), false); }

//////////////////////////////////////////////////////////////////////////
#define _MY_TRY bool __exceptionCatch = false ; try
#define _MY_CATCH  \
	catch(const std::exception &err){\
		__exceptionCatch = true;\
		_Verify(false, err.what());\
	} catch(const std::string &err){\
		__exceptionCatch = true;\
		_Verify(false, err.c_str());\
	}catch(const char* err){\
		__exceptionCatch = true;\
		_Verify(false, err);\
	}catch(...){\
		__exceptionCatch = true;\
		_Verify(false, "");\
	}\
	if(__exceptionCatch)\
	

//////////////////////////////////////////////////////////////////////////
#define __ENTER_FUNCTION {try{
#define __LEAVE_FUNCTION }catch(...){_Assert(0,"");}}

#define __ENTER_FUNCTION_EX {try{
#define __LEAVE_FUNCTION_EX }catch(...){_Verify(0,"");}}


#ifdef EXCEPTION_UNIT_TEST
AUTOTEST_DEF(exceptionUnitTest);
#endif


#endif // __EXCEPTION_H__