#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <Engine\System\Target.h>

namespace Engine
{
	bool _MessagedAssert(const char * i_pExp, const char * i_pMessage, const char * i_pFile, unsigned int i_LineNo);
}

#ifdef NDEBUG

#define MessagedAssert(_Expression,_Message)     ((void)0)
#define Assert(_Expression)  ((void)0)

#else

#define MessagedAssert(_Expression,_Message) if( (!(_Expression)) && !Engine::_MessagedAssert(#_Expression, _Message, __FILE__, __LINE__) ) { DEBUGGER_BREAK; }
#define Assert(_Expression)	if( (!(_Expression))) {DEBUGGER_BREAK;}

#endif  // NDEBUG

#endif // __ASSERT_H__