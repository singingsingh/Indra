#ifndef __TARGET_H__
#define __TARGET_H__

#if defined _WIN64
#include <Engine\System\Target.Win64.h>
#elif defined _WIN32
#include <Engine\System\Target.Win32.h>
#else
#error "Must include platform target file."
#endif // WIN32

#ifndef CACHE_LINE_ALIGNMENT_BYTES
#error "Must define CACHE_LINE_ALIGNMENT_BYTES."
#endif //CACHE_LINE_ALIGNMENT_BYTES

#ifndef DEBUGGER_BREAK
#error "Must define DEBUGGER_BREAK function."
#endif

#endif // __TARGET_H__