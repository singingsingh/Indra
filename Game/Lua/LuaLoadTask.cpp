#include <Game\Lua\LuaLoadTask.h>

#include <Engine\Engine\World.h>
#include <Engine\Engine\Physicsinfo.h>
#include <Engine\Core\SmartPtr.h>
#include <Engine\Core\StringPool.h>

#include <GraphicsDX\Renderer\Sprite\Sprite.h>

#include <Game\Controllers\PlayerController.h>
#include <Game\Controllers\MonsterController.h>
#include <Game\Lua\Lua.h>

#include <string.h>

namespace Game
{

	namespace Lua
	{
		LuaLoadInfo::LuaLoadInfo()
		{
			goName = nullptr;
			spriteFileName = nullptr;
			controllerType = nullptr;
			mass = 0.0f;
			drag = 0.0f;
			hasPhysicsInfo = false;
			hasController = false;
			hasRenderInfo = false;
			hasCollision = false;
			colliderTag = nullptr;
			collidesWith = nullptr;
			numCollidesWith = 0;
			rotDegree = 0.0f;
		}

		LuaLoadInfo::~LuaLoadInfo()
		{
			free( goName );
			goName = nullptr;

			free((void *)controllerType);
			controllerType = nullptr;

			free( spriteFileName );
			spriteFileName = nullptr;

			free( (void*)colliderTag);
			colliderTag = nullptr;

			const char** ptr = collidesWith;
			for (unsigned short i = 0; i < numCollidesWith; i++)
			{
				free((void*)(*ptr));
				(*ptr) = nullptr;
				ptr++;
			}

			free( collidesWith );
			collidesWith = nullptr;
		}
	}

	void LuaLoadTask::ProcessFileContents(uint8_t * i_pBuffer, uint32_t i_BufferBytes)
	{
		if ((i_pBuffer != NULL) && (i_BufferBytes > 0))
		{
			Game::Lua::createGameObject(i_pBuffer, i_BufferBytes, _luaLoadInfo);
		}
	}

	void LuaLoadTask::processReleaseQueue()
	{
		Engine::World* world = Engine::World::GetWorld();

		Engine::SmartPtr< Engine::GameObject > gameObject = world->createGameObject(_luaLoadInfo.goName);
		gameObject->pos = _luaLoadInfo.goPos;
		gameObject->rotDegree = _luaLoadInfo.rotDegree;
		gameObject->UID = _luaLoadInfo.UID;

		Engine::SmartPtr< Engine::PhysicsInfo > physicsInfo;

		Engine::SmartPtr<GraphicsDX::Sprite> sprite;
		if (_luaLoadInfo.hasRenderInfo)
		{
			sprite = GraphicsDX::Sprite::CreateSprite(_luaLoadInfo.spriteFileName);
			sprite->setGameObject(gameObject);
		}

		if (_luaLoadInfo.hasPhysicsInfo)
		{
			physicsInfo  = new Engine::PhysicsInfo(_luaLoadInfo.mass, _luaLoadInfo.drag, gameObject, sprite->getDimentions());
		}

		if (_luaLoadInfo.hasController)
		{
			if (strcmp(_luaLoadInfo.controllerType, "playerController") == 0)
			{
				Game::PlayerController::CreatePlayerController();
				Game::PlayerController* playerController = Game::PlayerController::GetPlayerController();
				playerController->setPhysicsInfo(physicsInfo);
			}
			else if (strcmp(_luaLoadInfo.controllerType, "monsterController") == 0)
			{
				Game::MonsterController::CreateMonsterController();
				Game::MonsterController* monsterController = Game::MonsterController::GetMonsterController();
				monsterController->setPhysicsInfo(physicsInfo);
			}
		}

		if (_luaLoadInfo.hasCollision)
		{
			physicsInfo->colliderTag = Engine::StringPool::Add(_luaLoadInfo.colliderTag);
			physicsInfo->numCollidesWith = _luaLoadInfo.numCollidesWith;
			physicsInfo->collidesWith = new const char*[physicsInfo->numCollidesWith];

			const char** output = physicsInfo->collidesWith;
			const char** input = _luaLoadInfo.collidesWith;

			for (unsigned short i = 0; i < physicsInfo->numCollidesWith; i++)
			{
				(*output) = Engine::StringPool::Add(*(input));
				output++;
				input++;
			}
		}
	}

}	// Game