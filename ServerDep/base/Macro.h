#ifndef MACRO_H
#define MACRO_H

//...
#if defined(__WINDOWS__)

#endif

#if defined(__LINUX__)
	#define __FUNCTION__ __PRETTY_FUNCTION__
#elif defined(__WINDOWS__)
	#define snprintf _snprintf
	#define vsnprintf vsprintf_s
	#define localtime_r localtime_s
#else
	#error windows or linux required.
#endif

#define zeroMemory(p,len) memset(p,0,len)

#define __LS___(x) #x
#define __FLS__(FC,x) ("["FC":"__LS___(x)"]")
#define __FL__	__FLS__(__FUNCTION__, __LINE__)

#ifndef NULL
#define NULL 0
#endif

#endif // MACRO_H