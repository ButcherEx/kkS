//--------------------------------------------------------------------------------
//
// Filename   : Assert.h
//
//--------------------------------------------------------------------------------

#ifndef __ASSERT__H__
#define __ASSERT__H__

// include files
#include "Base.h"

extern int g_Command_Assert ;//控制参数，不提示Assert的对话框，直接忽略
extern int g_Command_IgnoreMessageBox ;//控制参数，跳过MyMessageBox的中断


//////////////////////////////////////////////////////////////////////////
void __assert__(const CHAR *func, int32_t line, const CHAR *exp, bool throwException);

//////////////////////////////////////////////////////////////////////////
#define Assert(expr)				{if(!(expr)){__assert__(__FUNCTION__,__LINE__,#expr, true);}}
#define AssertEx(expr,msg)			{if(!(expr)){__assert__(__FUNCTION__,__LINE__,msg, true);}}
#define AssertSpecial(expr)			{if(!(expr)){__assert__(__FUNCTION__,__LINE__,#expr, false);}}
#define AssertSpecialEx(expr,msg)	{if(!(expr)){__assert__(__FUNCTION__,__LINE__,msg, false);}}


#endif
