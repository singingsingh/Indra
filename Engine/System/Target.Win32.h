#ifndef __TARGET_WIN_32__
#define __TARGET_WIN_32__

#include <stdint.h>


#define CACHE_LINE_ALIGNMENT_BYTES	4
#define DEBUGGER_BREAK __debugbreak()

typedef uint32_t MYUINT;

#endif //__TARGET_WIN_32__