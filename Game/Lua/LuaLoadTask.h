#ifndef __LUA_LOAD_TASK_H__

#define __LUA_LOAD_TASK_H__

#include <Engine\Core\ThreadedFileProcessor.h>
#include <Engine\Core\Math\Vector2f.h>

namespace Game
{
	namespace Lua
	{
		struct LuaLoadInfo
		{
			long UID;
			Engine::Math::Vector2f goPos;
			float rotDegree;
			char *goName;
			char *spriteFileName;
			float mass, drag;
			bool hasPhysicsInfo, hasController, hasRenderInfo;
			const char* controllerType;
			bool hasCollision;
			const char* colliderTag;
			unsigned int numCollidesWith;
			const char** collidesWith;

			LuaLoadInfo();
			~LuaLoadInfo();
		};
	}

	class LuaLoadTask : public Engine::ThreadedFileProcessor::LoadTask
	{
		public:

			LuaLoadTask(const char * i_pFilename)
				:Engine::ThreadedFileProcessor::LoadTask(i_pFilename)
			{
			}

			virtual void ProcessFileContents(uint8_t * i_pBuffer, uint32_t i_BufferBytes);
			virtual void processReleaseQueue();

		private:
			Lua::LuaLoadInfo _luaLoadInfo;
	};

}	// Game

#endif	//__LUA_LOAD_TASK_H__