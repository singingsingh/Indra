#ifndef __TARGET_WIN_64__
#define __TARGET_WIN_64__

#include <stdint.h>

#define CACHE_LINE_ALIGNMENT_BYTES	8

#define DEBUGGER_BREAK __debugbreak()

typedef uint64_t MYUINT;

#endif //__TARGET_WIN_64__