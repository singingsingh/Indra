#ifndef __WORLD_H__
#define __WORLD_H__

#include <Engine\Engine\GameObject.h>
#include <Engine\Core\SmartPtr.h>

#include <string>
#include <vector>

namespace Engine
{
	class World
	{
		public:

			static void CreateWorld();
			static World* GetWorld();
			static void DestroyWorld();
			
			SmartPtr<GameObject> createGameObject(const char * gameObjectName);
			SmartPtr<GameObject> getGameObjectByName(const char * gameObjectName);
			SmartPtr<GameObject> getGameObjectByUID(long UID);

		private:

			static World* _instance;
			std::vector< SmartPtr<GameObject> >_gameObjects;

			World();
			~World();
			World(const World & rhs);
			World& operator=(const World & rhs);

			DECLARE_MEMORY_MANAGER;
	};
}

#endif	//__WORLD_H__