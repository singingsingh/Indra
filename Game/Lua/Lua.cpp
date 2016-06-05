#include <Game\Lua\Lua.h>

#include <Engine\Util\Assert.h>
#include <Engine\Util\FileUtils.h>

#include <Game\libs\lua\lua.hpp>

namespace Game
{
	namespace Lua
	{
		size_t ReadIntArray(lua_State * i_pState, int i_index, int * o_pInts, size_t i_numInts);
		size_t ReadFloatArray(lua_State * i_pState, int i_index, float * o_pFloats, size_t i_numFloats);
		size_t ReadStringArray(lua_State * i_pState, int i_index, const char** o_pChar, size_t i_numChars);

		Engine::Math::Vector2f GetVector2(lua_State * i_pState, int i_index);
		void parsePlayer(Lua::LuaLoadInfo& i_luaLoadInfo, lua_State* pLuaState);
		void parseNPC(Lua::LuaLoadInfo& i_luaLoadInfo, lua_State* pLuaState);

		void createGameObject(uint8_t* pFileContents, size_t sizeFile, Lua::LuaLoadInfo& i_luaLoadInfo)
		{
			// parse the lua file.
			lua_State * pLuaState = luaL_newstate();

			MessagedAssert(pLuaState != nullptr, "Failed to get Lua state.\n");
			luaL_openlibs(pLuaState);

			if (pFileContents  && sizeFile)
			{
				int	result = 0;

				// Do the initial buffer parsing
				result = luaL_loadbuffer(pLuaState, reinterpret_cast<char *>(pFileContents), sizeFile, nullptr);
				MessagedAssert(result == 0, "Lua syntax error.\n");
				lua_pcall(pLuaState, 0, 0, 0);

				// Find the global variable named "Player" and push it onto stack.
				// If it doesn't find it it pushes a nil value instead
				result = lua_getglobal(pLuaState, "player");
				
				if (result == LUA_TTABLE)
				{
					parsePlayer(i_luaLoadInfo, pLuaState);

					// pop "Player" table
					lua_pop(pLuaState, 1);

					int stack_items = lua_gettop(pLuaState);
					MessagedAssert(stack_items == 0, "Stack is not empty.\n");
					lua_close(pLuaState);
					return;
				}
				lua_pop(pLuaState, 1);

				// Find the global variable named "npc" and push it onto stack.
				// If it doesn't find it it pushes a nil value instead
				result = lua_getglobal(pLuaState, "npc");
				//MessagedAssert(result == LUA_TTABLE, "Invalid lua state.\n");
				if (result == LUA_TTABLE)
				{
					parseNPC(i_luaLoadInfo, pLuaState);
					// pop "npc" table
					lua_pop(pLuaState, 1);

					int stack_items = lua_gettop(pLuaState);
					MessagedAssert(stack_items == 0, "Stack is not empty.\n");
					lua_close(pLuaState);
					return;
				}

				MessagedAssert(false, "Invalid lua file root name.\n");
			}

			int stack_items = lua_gettop(pLuaState);
			MessagedAssert(stack_items == 0, "Stack is not empty.\n");
			lua_close(pLuaState);
		}

		void parsePlayer(Lua::LuaLoadInfo& i_luaLoadInfo, lua_State* pLuaState)
		{
			int	type = LUA_TNIL;

			//--------------- Get rotation ------------
			lua_pushstring(pLuaState, "UID");
			type = lua_gettable(pLuaState, -2);
			MessagedAssert(type == LUA_TNUMBER, "\"UID\" not provided.\n");
			i_luaLoadInfo.UID = long(lua_tonumber(pLuaState, -1));
			lua_pop(pLuaState, 1);

			//-------------- Get Name ------------------

			// 1. Push the key string
			lua_pushstring(pLuaState, "name");
			// 2. Get the value associated with the key
			type = lua_gettable(pLuaState, -2);
			MessagedAssert(type == LUA_TSTRING, "Parsing Error: Datatype string expected.\n");
			const char * pName = lua_tostring(pLuaState, -1);
			MessagedAssert(pName != NULL, "Parsing Error: null string specified.\n");
			i_luaLoadInfo.goName = _strdup(pName);
			// 3. Remove the value from the stack now that we're done with it
			lua_pop(pLuaState, 1);

			//-------------- Get Position ------------------

			lua_pushstring(pLuaState, "position");
			type = lua_gettable(pLuaState, -2);
			MessagedAssert(type == LUA_TTABLE, "Invalid lua state.\n");
			if (type == LUA_TTABLE)
			{
				float		position[2] = { 0.0f };
				ReadFloatArray(pLuaState, -1, position, 2);
				i_luaLoadInfo.goPos.x = position[0];
				i_luaLoadInfo.goPos.y = position[1];
			}
			// we're done with the "initial_position table - pop it
			lua_pop(pLuaState, 1);

			//--------------- Get rotation ------------
			lua_pushstring(pLuaState, "rotDegree");
			type = lua_gettable(pLuaState, -2);
			MessagedAssert(type == LUA_TNUMBER, "\"rotDegree\" not provided.\n");
			i_luaLoadInfo.rotDegree = float(lua_tonumber(pLuaState, -1));
			lua_pop(pLuaState, 1);

			//-------------- Get physicsInfo ------------------

			lua_pushstring(pLuaState, "physicsInfo");
			type = lua_gettable(pLuaState, -2);
			// It either needs to be a table or not there (no physics applied)
			//MessagedAssert((type == LUA_TNIL) || (type == LUA_TTABLE), "No physics info provided.\n");
			if ((type == LUA_TNIL) || (type == LUA_TTABLE))
			{
				i_luaLoadInfo.hasPhysicsInfo = true;
				lua_pushstring(pLuaState, "mass");
				type = lua_gettable(pLuaState, -2);
				MessagedAssert(type == LUA_TNUMBER, "\"mass\" not provided.\n");

				lua_pushstring(pLuaState, "drag");
				type = lua_gettable(pLuaState, -3);
				MessagedAssert(type == LUA_TNUMBER, "\"drag\" not provided.\n");

				i_luaLoadInfo.mass = float(lua_tonumber(pLuaState, -2));
				i_luaLoadInfo.drag = float(lua_tonumber(pLuaState, -1));

				// pop the "mass" and "drag" values together
				lua_pop(pLuaState, 2);
			}
			// we're done with the "physicsInfo" table - pop it
			// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
			lua_pop(pLuaState, 1);

			//-------------- Get RenderInfo ------------------

			lua_pushstring(pLuaState, "renderInfo");
			type = lua_gettable(pLuaState, -2);

			// It either needs to be a table or not there (no renderInfo applied)
			//MessagedAssert((type == LUA_TNIL) || (type == LUA_TTABLE), "Not renderInfo provided.\n");
			if ((type == LUA_TNIL) || (type == LUA_TTABLE))
			{
				i_luaLoadInfo.hasRenderInfo = true;
				// Get the Sprite name from the Player table
				// 1. Push the key string
				lua_pushstring(pLuaState, "sprite");
				// 2. Get the value associated with the key
				type = lua_gettable(pLuaState, -2);
				MessagedAssert(type == LUA_TSTRING, "Parsing Error: Datatype string expected.\n");
				const char* sName = lua_tostring(pLuaState, -1);
				MessagedAssert(sName != NULL, "Parsing Error: null string specified.\n");
				i_luaLoadInfo.spriteFileName = _strdup(sName);

				// 3. Remove the value from the stack now that we're done with it
				lua_pop(pLuaState, 1);
			}
			// we're done with the "renderInfo" table - pop it
			// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
			lua_pop(pLuaState, 1);

			//-------------- Get playercontroller ------------------

			lua_pushstring(pLuaState, "controller");
			type = lua_gettable(pLuaState, -2);

			// It either needs to be a table or not there (no renderInfo applied)
			//MessagedAssert((type == LUA_TNIL) || (type == LUA_TTABLE), "Not renderInfo provided.\n");
			if (type == LUA_TSTRING)
			{
				i_luaLoadInfo.hasController = true;
				const char* sName = lua_tostring(pLuaState, -1);
				MessagedAssert(sName != NULL, "Parsing Error: null string specified.\n");
				i_luaLoadInfo.controllerType = _strdup(sName);
			}
			// we're done with the "Controller" table - pop it
			// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
			lua_pop(pLuaState, 1);

			//---------------- Collision --------------------------
			lua_pushstring(pLuaState, "collider");
			type = lua_gettable(pLuaState, -2);
			// It either needs to be a table or not there (no renderInfo applied)
			//MessagedAssert((type == LUA_TNIL) || (type == LUA_TTABLE), "Not collision provided.\n");
			if ((type == LUA_TNIL) || (type == LUA_TTABLE))
			{
				i_luaLoadInfo.hasCollision = true;
				// Get the Sprite name from the Player table
				// 1. Push the key string
				lua_pushstring(pLuaState, "tag");
				// 2. Get the value associated with the key
				type = lua_gettable(pLuaState, -2);
				MessagedAssert(type == LUA_TSTRING, "Parsing Error: Datatype string expected.\n");
				const char* sName = lua_tostring(pLuaState, -1);
				MessagedAssert(sName != NULL, "Parsing Error: null string specified.\n");
				i_luaLoadInfo.colliderTag = _strdup(sName);
				// 3. Remove the value from the stack now that we're done with it
				lua_pop(pLuaState, 1);

				lua_pushstring(pLuaState, "numCollisionTag");
				type = lua_gettable(pLuaState, -2);
				MessagedAssert(type == LUA_TNUMBER, "\"numCollisionTag\" not provided.\n");
				i_luaLoadInfo.numCollidesWith = int(lua_tonumber(pLuaState, -1));
				lua_pop(pLuaState, 1);

				if (i_luaLoadInfo.numCollidesWith > 0)
				{
					lua_pushstring(pLuaState, "collidesWith");
					type = lua_gettable(pLuaState, -2);
					MessagedAssert(type == LUA_TTABLE, "Invalid lua state.\n");
					if (type == LUA_TTABLE)
					{
						i_luaLoadInfo.collidesWith = new const char*[i_luaLoadInfo.numCollidesWith];
						ReadStringArray( pLuaState, -1, i_luaLoadInfo.collidesWith, i_luaLoadInfo.numCollidesWith);
					}

					lua_pop(pLuaState, 1);
				}
			}
			// we're done with the "collision" table - pop it
			// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
			lua_pop(pLuaState, 1);
		}

		void parseNPC(Lua::LuaLoadInfo& i_luaLoadInfo, lua_State* pLuaState)
		{
			int	type = LUA_TNIL;

			//--------------- Get rotation ------------
			lua_pushstring(pLuaState, "UID");
			type = lua_gettable(pLuaState, -2);
			MessagedAssert(type == LUA_TNUMBER, "\"UID\" not provided.\n");
			i_luaLoadInfo.UID = long(lua_tonumber(pLuaState, -1));
			lua_pop(pLuaState, 1);

			//-------------- Get Name ------------------

			// 1. Push the key string
			lua_pushstring(pLuaState, "name");
			// 2. Get the value associated with the key
			type = lua_gettable(pLuaState, -2);
			MessagedAssert(type == LUA_TSTRING, "Parsing Error: Datatype string expected.\n");
			const char * pName = lua_tostring(pLuaState, -1);
			MessagedAssert(pName != NULL, "Parsing Error: null string specified.\n");
			i_luaLoadInfo.goName = _strdup(pName);
			// 3. Remove the value from the stack now that we're done with it
			lua_pop(pLuaState, 1);

			//-------------- Get Position ------------------

			lua_pushstring(pLuaState, "position");
			type = lua_gettable(pLuaState, -2);
			MessagedAssert(type == LUA_TTABLE, "Invalid lua state.\n");
			if (type == LUA_TTABLE)
			{
				float		position[2] = { 0.0f };
				ReadFloatArray(pLuaState, -1, position, 2);
				i_luaLoadInfo.goPos.x = position[0];
				i_luaLoadInfo.goPos.y = position[1];
			}
			// we're done with the "initial_position table - pop it
			lua_pop(pLuaState, 1);

			//--------------- Get rotation ------------
			lua_pushstring(pLuaState, "rotDegree");
			type = lua_gettable(pLuaState, -2);
			MessagedAssert(type == LUA_TNUMBER, "\"rotDegree\" not provided.\n");
			i_luaLoadInfo.rotDegree = float(lua_tonumber(pLuaState, -1));
			lua_pop(pLuaState, 1);

			//-------------- Get physicsInfo ------------------

			lua_pushstring(pLuaState, "physicsInfo");
			type = lua_gettable(pLuaState, -2);
			// It either needs to be a table or not there (no physics applied)
			//MessagedAssert((type == LUA_TNIL) || (type == LUA_TTABLE), "No physics info provided.\n");
			if ((type == LUA_TNIL) || (type == LUA_TTABLE))
			{
				i_luaLoadInfo.hasPhysicsInfo = true;
				lua_pushstring(pLuaState, "mass");
				type = lua_gettable(pLuaState, -2);
				MessagedAssert(type == LUA_TNUMBER, "\"mass\" not provided.\n");

				lua_pushstring(pLuaState, "drag");
				type = lua_gettable(pLuaState, -3);
				MessagedAssert(type == LUA_TNUMBER, "\"drag\" not provided.\n");

				i_luaLoadInfo.mass = float(lua_tonumber(pLuaState, -2));
				i_luaLoadInfo.drag = float(lua_tonumber(pLuaState, -1));

				// pop the "mass" and "drag" values together
				lua_pop(pLuaState, 2);
			}
			// we're done with the "physicsInfo" table - pop it
			// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
			lua_pop(pLuaState, 1);

			//-------------- Get RenderInfo ------------------

			lua_pushstring(pLuaState, "renderInfo");
			type = lua_gettable(pLuaState, -2);

			// It either needs to be a table or not there (no renderInfo applied)
			//MessagedAssert((type == LUA_TNIL) || (type == LUA_TTABLE), "Not renderInfo provided.\n");
			if ((type == LUA_TNIL) || (type == LUA_TTABLE))
			{
				i_luaLoadInfo.hasRenderInfo = true;
				// Get the Sprite name from the Player table
				// 1. Push the key string
				lua_pushstring(pLuaState, "sprite");
				// 2. Get the value associated with the key
				type = lua_gettable(pLuaState, -2);
				MessagedAssert(type == LUA_TSTRING, "Parsing Error: Datatype string expected.\n");
				const char* sName = lua_tostring(pLuaState, -1);
				MessagedAssert(sName != NULL, "Parsing Error: null string specified.\n");
				i_luaLoadInfo.spriteFileName = _strdup(sName);

				// 3. Remove the value from the stack now that we're done with it
				lua_pop(pLuaState, 1);
			}
			// we're done with the "renderInfo" table - pop it
			// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
			lua_pop(pLuaState, 1);

			//-------------- Get NPCcontroller ------------------

			lua_pushstring(pLuaState, "controller");
			type = lua_gettable(pLuaState, -2);

			// It either needs to be a table or not there (no renderInfo applied)
			//MessagedAssert((type == LUA_TNIL) || (type == LUA_TTABLE), "Not renderInfo provided.\n");
			if (type == LUA_TSTRING)
			{
				i_luaLoadInfo.hasController = true;
				const char* sName = lua_tostring(pLuaState, -1);
				MessagedAssert(sName != NULL, "Parsing Error: null string specified.\n");
				i_luaLoadInfo.controllerType = _strdup(sName);
			}
			// we're done with the "playerController" table - pop it
			// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
			lua_pop(pLuaState, 1);

			//---------------- Collision --------------------------

			lua_pushstring(pLuaState, "collider");
			type = lua_gettable(pLuaState, -2);
			// It either needs to be a table or not there (no renderInfo applied)
			//MessagedAssert((type == LUA_TNIL) || (type == LUA_TTABLE), "Not collision provided.\n");
			if ((type == LUA_TNIL) || (type == LUA_TTABLE))
			{
				i_luaLoadInfo.hasCollision = true;
				// Get the Sprite name from the Player table
				// 1. Push the key string
				lua_pushstring(pLuaState, "tag");
				// 2. Get the value associated with the key
				type = lua_gettable(pLuaState, -2);
				MessagedAssert(type == LUA_TSTRING, "Parsing Error: Datatype string expected.\n");
				const char* sName = lua_tostring(pLuaState, -1);
				MessagedAssert(sName != NULL, "Parsing Error: null string specified.\n");
				i_luaLoadInfo.colliderTag = _strdup(sName);
				// 3. Remove the value from the stack now that we're done with it
				lua_pop(pLuaState, 1);

				lua_pushstring(pLuaState, "numCollisionTag");
				type = lua_gettable(pLuaState, -2);
				MessagedAssert(type == LUA_TNUMBER, "\"numCollisionTag\" not provided.\n");
				i_luaLoadInfo.numCollidesWith = int(lua_tonumber(pLuaState, -1));
				lua_pop(pLuaState, 1);

				if (i_luaLoadInfo.numCollidesWith > 0)
				{
					lua_pushstring(pLuaState, "collidesWith");
					type = lua_gettable(pLuaState, -2);
					MessagedAssert(type == LUA_TTABLE, "Invalid lua state.\n");
					if (type == LUA_TTABLE)
					{
						i_luaLoadInfo.collidesWith = new const char*[i_luaLoadInfo.numCollidesWith];
						ReadStringArray(pLuaState, -1, i_luaLoadInfo.collidesWith, i_luaLoadInfo.numCollidesWith);
					}

					lua_pop(pLuaState, 1);
				}
			}
			// we're done with the "collision" table - pop it
			// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
			lua_pop(pLuaState, 1);
		}

		int GetLuaTableEntries(lua_State * i_pState, int i_index)
		{
			MessagedAssert(i_pState != nullptr, "Lua state is nullptr.\n");
			MessagedAssert(lua_gettop(i_pState) >= -i_index, "Invalid Lua state.\n");
			MessagedAssert(lua_type(i_pState, i_index) == LUA_TTABLE, "Invalid Lua state.\n");

			lua_len(i_pState, i_index);
			MessagedAssert(lua_type(i_pState, -1) == LUA_TNUMBER, "Invalid Lua state.\n");

			lua_Integer table_entries = lua_tointeger(i_pState, -1);
			lua_pop(i_pState, 1);

			MessagedAssert(table_entries <= INT_MAX, "Invalid Lua state.\n");

			return static_cast<int>(table_entries);
		}

		size_t ReadIntArray(lua_State * i_pState, int i_index, int * o_pInts, size_t i_numInts)
		{
			MessagedAssert(i_pState != nullptr, "Lua state is nullptr.\n");
			MessagedAssert(lua_gettop(i_pState) >= -i_index, "Invalid Lua state.\n");
			MessagedAssert(lua_type(i_pState, i_index) == LUA_TTABLE, "Invalid Lua state.\n");

			size_t	index = 0;

			lua_pushnil(i_pState);

			while (lua_next(i_pState, i_index - 1) != 0)
			{
				if (lua_type(i_pState, -1) == LUA_TNUMBER)
				{
					lua_Integer lValue = lua_tointeger(i_pState, -1);
					MessagedAssert(lValue <= INT_MAX, "Invalid Lua state.\n");

					o_pInts[index] = lValue < INT_MAX ? int(lValue) : INT_MAX;
				}
				else
				{
					DEBUG_PRINT("Expected a number at position %d while reading table as int array.\n", index);
				}

				lua_pop(i_pState, 1);

				if (++index == i_numInts)
				{
					lua_pop(i_pState, 1);
					break;
				}
			}

			return index;
		}

		size_t ReadFloatArray(lua_State * i_pState, int i_index, float * o_pFloats, size_t i_numFloats)
		{
			MessagedAssert(i_pState != nullptr, "Lua state is nullptr.\n");
			MessagedAssert(lua_gettop(i_pState) >= -i_index, "Invalid Lua state.\n");
			MessagedAssert(lua_type(i_pState, i_index) == LUA_TTABLE, "Invalid Lua state.\n");

			size_t				index = 0;

			lua_pushnil(i_pState);

			while (lua_next(i_pState, i_index - 1) != 0)
			{
				if (lua_type(i_pState, -1) == LUA_TNUMBER)
				{
					o_pFloats[index] = float(lua_tonumber(i_pState, -1));
				}
				else
				{
					DEBUG_PRINT("Expected a number at position %d while reading table as float array.\n", index);
				}

				// pop the value now that we've retrieved it
				lua_pop(i_pState, 1);
				if (++index == i_numFloats)
				{
					lua_pop(i_pState, 1);
					break;
				}
			}
			return index;
		}

		size_t ReadStringArray(lua_State * i_pState, int i_index, const char** o_pchar, size_t i_numStrings)
		{
			MessagedAssert(i_pState != nullptr, "Lua state is nullptr.\n");
			MessagedAssert(lua_gettop(i_pState) >= -i_index, "Invalid Lua state.\n");
			MessagedAssert(lua_type(i_pState, i_index) == LUA_TTABLE, "Invalid Lua state.\n");

			size_t				index = 0;

			lua_pushnil(i_pState);

			while (lua_next(i_pState, i_index - 1) != 0)
			{
				if (lua_type(i_pState, -1) == LUA_TSTRING)
				{
					const char* sName = lua_tostring(i_pState, -1);
					*o_pchar = _strdup(sName);
					o_pchar++;
				}
				else
				{
					DEBUG_PRINT("Expected a number at position %d while reading table as float array.\n", index);
				}

				// pop the value now that we've retrieved it
				lua_pop(i_pState, 1);
				if (++index == i_numStrings)
				{
					lua_pop(i_pState, 1);
					break;
				}
			}
			return index;
		}

		size_t GetFloatArray(lua_State * i_pState, int i_indexTable, const char * i_pArrayKey, float * o_pFloats, size_t i_numFloats)
		{
			MessagedAssert(i_pArrayKey != nullptr, "Invalid float array name.\n");

			lua_pushstring(i_pState, i_pArrayKey);
			i_indexTable--;

			int type = lua_gettable(i_pState, i_indexTable);

			size_t itemsRead = 0;

			if (type == LUA_TTABLE)
				itemsRead = ReadFloatArray(i_pState, i_indexTable, o_pFloats, i_numFloats);
			else
			{
				DEBUG_PRINT("Expected Lua table for %s found value of type %d.\n", i_pArrayKey, type);
			}

			lua_pop(i_pState, 1);

			return itemsRead;
		}

		Engine::Math::Vector2f GetVector2(lua_State * i_pState, int i_index)
		{
			MessagedAssert(i_pState != nullptr, "Lua state is nullptr.\n");
			MessagedAssert(lua_gettop(i_pState) >= -i_index, "Invalid Lua state.\n");
			MessagedAssert(lua_type(i_pState, i_index) == LUA_TTABLE, "Invalid Lua state.\n");

			const size_t		numFloats = 3;
			float				v[numFloats] = { 0 };

			size_t	readFloats = ReadFloatArray(i_pState, i_index, v, numFloats);

			return Engine::Math::Vector2f(v[0], v[1]);
		}

	}
}