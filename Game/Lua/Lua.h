#ifndef __LUA_H__
#define __LUA_H__

#include <Game\Lua\LuaLoadTask.h>

namespace Game
{
	namespace Lua
	{
		void createGameObject(uint8_t* pFileContents, size_t sizeFile, Lua::LuaLoadInfo& i_luaLoadInfo);
	}
}

#endif	//__LUA_H__