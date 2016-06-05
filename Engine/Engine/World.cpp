#include <Engine\Engine\World.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>
#include <Engine\Core\SmartPtr.h>

#include <string.h>

namespace Engine
{
	DEFINE_MEMORY_MANAGER(World);

	World* World::_instance = NULL;

	World::World()
	{
		DEBUG_PRINT("Creating the world instance.\n");
	}

	World::~World()
	{
		DEBUG_PRINT("Destroying the world instance.\n");
		_gameObjects.clear();
	}

	void World::CreateWorld()
	{
		if (!_instance)
		{
			_instance = new World();
		}
	}

	World* World::GetWorld()
	{
		return _instance;
	}

	void World::DestroyWorld()
	{
		delete _instance;
		_instance = NULL;
	}

	SmartPtr<GameObject> World::createGameObject( const char * gameObjectName )
	{
		for (unsigned int i = 0; i < _gameObjects.size(); i++)
		{
			if (strcmp(gameObjectName, _gameObjects[i]->getName()) == 0)
			{
				DEBUG_PRINT( "A GameObject with name \"%s\" already exists.\n", gameObjectName );
				SmartPtr<GameObject> emptyGO;
				return emptyGO;
			}
		}

		SmartPtr<GameObject> gameObject(new GameObject(gameObjectName));
		_gameObjects.push_back(gameObject);

		return  gameObject;
	}

	SmartPtr<GameObject> World::getGameObjectByName(const char * gameObjectName)
	{
		unsigned int i = 0;
		for (; i < _gameObjects.size(); i++)
		{
			if (strcmp(gameObjectName, _gameObjects[i]->getName()) == 0)
			{
				break;
			}
		}

		if (i < _gameObjects.size())
		{
			return _gameObjects[i];
		}

		return nullptr;
	}

	SmartPtr<GameObject> World::getGameObjectByUID(long i_UID)
	{
		unsigned int i = 0;
		for (; i < _gameObjects.size(); i++)
		{
			if ( _gameObjects[i]->UID == i_UID)
			{
				break;
			}
		}

		if (i < _gameObjects.size())
		{
			return _gameObjects[i];
		}

		return nullptr;
	}
}