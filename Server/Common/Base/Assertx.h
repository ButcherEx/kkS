//--------------------------------------------------------------------------------
//
// Filename   : Assert.h
//
//--------------------------------------------------------------------------------

#ifndef __ASSERT__H__
#define __ASSERT__H__

// include files
#include "Base.h"

extern int g_Command_Assert ;//���Ʋ���������ʾAssert�ĶԻ���ֱ�Ӻ���
extern int g_Command_IgnoreMessageBox ;//���Ʋ���������MyMessageBox���ж�


//////////////////////////////////////////////////////////////////////////
void __assert__(const CHAR *func, int32_t line, const CHAR *exp, bool throwException);

//////////////////////////////////////////////////////////////////////////
#define Assert(expr)				{if(!(expr)){__assert__(__FUNCTION__,__LINE__,#expr, true);}}
#define AssertEx(expr,msg)			{if(!(expr)){__assert__(__FUNCTION__,__LINE__,msg, true);}}
#define AssertSpecial(expr)			{if(!(expr)){__assert__(__FUNCTION__,__LINE__,#expr, false);}}
#define AssertSpecialEx(expr,msg)	{if(!(expr)){__assert__(__FUNCTION__,__LINE__,msg, false);}}


#endif
