#ifndef __CONSOLE_PRINT_H__
#define __CONSOLE_PRINT_H__

#include <stdio.h>

namespace Engine
{
	void ConsolePrint(const char * i_fmt, ...);
	void info(const char * file, unsigned int lineNo);
}

#if defined(_DEBUG)  &&  !defined(DISABLE_DEBUG_PRINT)
#define DEBUG_PRINT(fmt, ...) Engine::info(__FILE__, __LINE__);Engine::ConsolePrint((fmt), __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt,...) void(0)
#endif

#endif // __CONSOLE_PRINT_H__