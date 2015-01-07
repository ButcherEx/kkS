#include <Base.h>

#if defined(__WINDOWS__)
_lightweight_mutex::_lightweight_mutex() { ::InitializeCriticalSectionAndSpinCount(&cs_, 4000); }
_lightweight_mutex::~_lightweight_mutex() { ::DeleteCriticalSection(&cs_); }
void _lightweight_mutex::lock( ) { ::EnterCriticalSection(&cs_);}
void _lightweight_mutex::unlock() { ::LeaveCriticalSection(&cs_);}
bool _lightweight_mutex::try_lock(){ return ::TryEnterCriticalSection(&cs_) == TRUE; }

#endif


#pragma warning(push)
#pragma warning(disable:4267)
#pragma warning(disable:4250)
#pragma warning(disable:4244)


#include <libs/system/src/error_code.cpp>
#include <libs/date_time/src/gregorian/greg_names.hpp>
#include <libs/date_time/src/gregorian/greg_month.cpp>
#include <libs/date_time/src/gregorian/greg_weekday.cpp>
#include <libs/date_time/src/gregorian/gregorian_types.cpp>

 #ifdef _MSC_VER 
 namespace boost{ extern "C" void tss_cleanup_implemented(void){} }
 #include <libs/thread/src/win32/thread.cpp>
 #include <libs/thread/src/win32/tss_dll.cpp>
 #include <libs/thread/src/win32/tss_pe.cpp>
 #else
 #include <libs/thread/src/pthread/thread.cpp>
 #include <libs/thread/src/pthread/once.cpp>
 #endif


// #include <libs/filesystem/src/codecvt_error_category.cpp>
// #include <libs/filesystem/src/operations.cpp>
// #include <libs/filesystem/src/path_traits.cpp>
// #include <libs/filesystem/src/path.cpp>
// #include <libs/filesystem/src/portability.cpp>
// #include <libs/filesystem/src/unique_path.cpp>
// #include <libs/filesystem/src/utf8_codecvt_facet.cpp>
// #include <libs/filesystem/src/windows_file_codecvt.cpp>

#pragma warning(pop)