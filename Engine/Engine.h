#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <Windows.h>

namespace Engine
{
	void Initialize( const char* saveDataFile, HINSTANCE hInstance, const char * pWindowName, unsigned int WindowWidth, unsigned int WindowHeight, const WORD* icon);
	void Shutdown();
}

#endif	//__ENGINE_H__