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

#define Assert(expr) ((void)0)
#define AssertEx(expr,msg) ((void)0)

//////////////////////////////////////////////////////////////////////////
void __assert__(const CHAR *file, const CHAR *func, int32_t line, const CHAR *exp, bool throwException);
#define Assert(expr) ((void)0)
#define AssertEx(expr,msg) ((void)0)
#define AssertSpecial(expr,msg) ((void)0)
#define MyMessageBox(msg) ((void)0)
/*
//--------------------------------------------------------------------------------  
//
//--------------------------------------------------------------------------------
void __assert__ (const CHAR* file, uint32_t line, const CHAR* func, const CHAR* expr) ;
void __assertex__ (const CHAR* file, uint32_t line, const CHAR* func, const CHAR* expr, const CHAR* msg) ;
void __assertspecial__ (const CHAR* file, uint32_t line, const CHAR* func, const CHAR* expr, const CHAR* msg) ;
void __messagebox__(const CHAR*msg ) ;
//--------------------------------------------------------------------------------  
//
//--------------------------------------------------------------------------------
void __protocol_assert__ (const CHAR* file, uint32_t line, const CHAR* func, const CHAR* expr) ;

#if defined(NDEBUG)
	#define Assert(expr) ((void)0)
	#define AssertEx(expr,msg) ((void)0)
	#define AssertSpecial(expr,msg) ((void)0)
	#define MyMessageBox(msg) ((void)0)
#elif __LINUX__
	#define Assert(expr) {if(!(expr)){__assert__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr);}}
	#define ProtocolAssert(expr) ((void)((expr)?0:(__protocol_assert__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr),0)))
	#define AssertEx(expr,msg) {if(!(expr)){__assertex__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
	#define AssertSpecial(expr,msg) {if(!(expr)){__assertspecial__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
	#define AssertExPass(expr,msg) {if(!(expr)){__assertex__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
	#define MyMessageBox(msg) ((void)0)
#elif __WIN_CONSOLE__ || __WIN32__ || __WINDOWS__
	#define Assert(expr) ((void)((expr)?0:(__assert__(__FILE__,__LINE__,__FUNCTION__,#expr),0)))
	#define AssertEx(expr,msg) ((void)((expr)?0:(__assertex__(__FILE__,__LINE__,__FUNCTION__,#expr,msg),0)))
	#define AssertSpecial(expr,msg) ((void)((expr)?0:(__assertspecial__(__FILE__,__LINE__,__FUNCTION__,#expr,msg),0)))
	#define MyMessageBox(msg) __messagebox__(msg)
#elif __MFC__
	#define Assert(expr) ASSERT(expr)
	#define AssertEx(expr,msg) ((void)0)
	#define AssertSpecial(expr,msg) ((void)0)
	#define MyMessageBox(msg) ((void)0)
#endif
	*/
#endif
