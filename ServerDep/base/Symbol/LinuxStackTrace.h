#ifndef STACK_TRACE_H
#define STACK_TRACE_H

#if defined(__LINUX__)

#include <signal.h>
#include <execinfo.h>
#include <sys/types.h>

namespace debug
{
	void stackTrace(std::ostream& os)
	{
		const int len = 200;
		void* buffer[len];
		int nptrs = ::backtrace(buffer, len);
		char** strings = ::backtrace_symbols(buffer, nptrs);
		if (strings)
		{
			for (int i = 0; i < nptrs; ++i)
			{
				// TODO demangle funcion name with abi::__cxa_demangle
				//stack_.append(strings[i]);
				//stack_.push_back('\n');
				os << strings[i] << "\n";
			}
			free(strings);
		}
	}
} // namespace debug

#endif

#endif // STACK_TRACE_H